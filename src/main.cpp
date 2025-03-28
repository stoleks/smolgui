#include <iostream>
#include <chrono>
#include <thread>

#include "sgui/sgui.h"
#include "sgui/parser/Formula.h"

int main()
{
  /**
   * Resources loading
   */
  auto style = sgui::Style ();
  style.fontColor = sf::Color::White;
  const std::string fontFile = "../../contents/Averia-Bold.ttf";
  auto font = sf::Font ();
  if (!font.openFromFile (fontFile)) {
    spdlog::error ("Unable to load {}", fontFile);
    std::this_thread::sleep_for (std::chrono::seconds (2));
  }
  const std::string mathFontFile = "../../contents/latinmodern-math.otf";
  auto mathFont = sf::Font ();
  if (!mathFont.openFromFile (mathFontFile)) {
    spdlog::error ("Unable to load {}", mathFontFile);
    std::this_thread::sleep_for (std::chrono::seconds (2));
  }
  const std::string atlasFile = "../../contents/atlases.json";
  auto atlas = sgui::TextureAtlas ();
  if (!atlas.loadFromFile (atlasFile)) {
    spdlog::error ("Unable to load {}", atlasFile);
    std::this_thread::sleep_for (std::chrono::seconds (2));
  }
  const std::string textureFile = "../../contents/widget.png";
  auto texture = sf::Texture ();
  if (!texture.loadFromFile (textureFile)) {
    spdlog::error ("Unable to load {}", textureFile);
    std::this_thread::sleep_for (std::chrono::seconds (2));
  }
  
  /**
   * Window initialization
   */
  auto window = sf::RenderWindow (sf::VideoMode ({1920u, 1080u}), "Card generator project");
  window.setFramerateLimit (60);

  /**
   * Gui initialization
   */
  auto gui = sgui::Gui ();
  gui.setResources (font, texture, atlas);
  gui.setStyle (style);

  /**
   * Some gui settings
   */
  auto panel = sgui::Panel ();
  panel.position = { 16.f, 256.f};
  panel.size = {640.f, 640.f};
  panel.closable = true;
  panel.title = "General demo";
  auto panel2 = sgui::Panel ();
  panel2.position = panel.position + sf::Vector2f (panel.size.x + 10.f, 0.f);
  panel2.size = {520.f, 520.f};
  panel2.title = "Closable window";
  panel2.closable = true;
  auto panel3 = sgui::Panel ();
  panel3.position = panel2.position + sf::Vector2f (panel2.size.x + 10.f, 0.f);
  panel3.size = {128.f, 40.f};
  panel3.movable = true;
  auto constraint = sgui::Constraints ();
  constraint.centeredVerticaly = true;
  auto sliderValue = 0.1f;
  auto inputValue = 0.f;
  auto text = std::string ("You can edit this text on multiple lines !");
  auto text2 = std::string ("You can edit this text !");
  auto vector = sf::Vector2f ();
  auto vector3 = sf::Vector3f ();
  bool displayFunction = false;

  /**
   * Main App loop
   */
  auto timer = sf::Clock ();
  auto t = sf::Clock ();
  auto timeSinceLastUpdate = sf::Time::Zero;
  const auto timePerFrame = sf::seconds (1.f / 60.f);
  while (window.isOpen ())
  {
    /**
     * Input and logic
     */
    const auto dt = timer.restart ();
    timeSinceLastUpdate += dt;
    while (timeSinceLastUpdate > timePerFrame)
    {
      timeSinceLastUpdate -= timePerFrame;
      while (const std::optional event = window.pollEvent ())
      {
        if (event->is <sf::Event::Closed> ()) {
          window.close ();
        }
        gui.update (window, event);
      }
      gui.setStyle (style);
      gui.updateTimer (dt);
    }

    /**
     * Gui
     */
    gui.beginFrame ();
    {
      // first window
      if (gui.beginWindow (panel2)) {
        // Open or close
        gui.text ("A window");
        gui.sameLine ();
        if (gui.textButton ("Open/Close")) {
          panel.closed = !panel.closed;
        }
        gui.sameLine ();
        gui.text ("Open or close the general demo window");
        gui.separation ();
        // Display a function
        gui.slider (sliderValue, 0.f, 10.f, {"Slider from 0 to 10, value is : " + std::to_string (sliderValue)});
        gui.checkBox (displayFunction, {"Display a function"});
        if (displayFunction) {
          auto func = [t, sliderValue] (float x) {
            const auto time = t.getElapsedTime ().asSeconds ();
            const auto value = std::sin (sliderValue*time + 10.f*x);
            return value;
          };
          gui.setSample (100u);
          gui.setPlotRange ({0, 1}, {-1.1, 1.1});
          gui.plot (func, 2.f);
          gui.forcePlotUpdate ();
        }
        // Change window size
        gui.separation ();
        gui.slider (panel.size.y, 50.f, 700.f, {"Slider from 0 to 700, value is : " + std::to_string (panel.size.y)});
      }
      gui.endWindow ();
      // second window
      if (gui.beginWindow (panel)) {
        if (gui.textButton ("Test button")) {
          spdlog::info ("Test button\n");
        }
        gui.separation ();

        gui.text ("Select font size");
        auto& fonts = style.fontSize;
        gui.slider (fonts.title, 12u, 26u, {"Title font size"});
        gui.slider (fonts.subtitle, 10u, 22u, {"Subtitle font size"});
        gui.slider (fonts.normal, 8u, 20u, {"Normal font size"});
        gui.inputColor (style.fontColor, {"Font color: "});
        gui.separation ();
        gui.inputText (text, {256.f, 64.f}, {"Editable on multiple lines : "});
        gui.inputText (text2, {}, {"Editable on one line"});
        gui.separation ();
        gui.inputNumber (inputValue, {"Input number with text!"});
        gui.inputVector2 (vector, {"Input vector2: "});
        gui.inputVector3 (vector3, {"Input vector3: "});
      }
      gui.endWindow ();

      gui.beginPanel (panel3, constraint);
      gui.text ("Just a panel");
      gui.text ("With scrollable");
      gui.addSpacing ({0.f, 12.f});
      gui.text ("text");
      gui.endPanel ();
    }
    gui.endFrame ();

    /**
     * Drawing
     */
    window.clear ();
    gui.draw (window);
    // TEST
    const auto expression ("(3 * 4) / (#sin(x - 5 / #sqrt(3) + #sqrt (4 * e^(x-6)) + 4) + 5)");
    sgui::Formula formula (mathFont);
    formula.printFormula (expression, window, gui.style ());
    std::wstring alphabet;
    for (auto s : sgui::Parser::Symbols) {
      alphabet += s.second.character;
    }
    sf::Text txt (mathFont);
    txt.setPosition (gui.cursorPosition ());
    txt.setCharacterSize (1.5f*gui.style ().fontSize.title);
    txt.setString (alphabet);
    window.draw (txt);
    // TEST
    window.display ();
  }
}
