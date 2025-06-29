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
  // fonts
  auto fonts = sgui::FontHolder ();
  fonts.load ("normal", ContentsDir"/Luciole-Regular.ttf");
  fonts.load ("bold",   ContentsDir"/Luciole-Bold.ttf");
  fonts.load ("math",   ContentsDir"/latinmodern-math.otf");
  // textures atlas
  auto atlas = sgui::TextureAtlas ();
  atlas.loadFromFile (ContentsDir"/atlas.json");
  // texture
  auto texture = sf::Texture ();
  const std::string textureFile = ContentsDir"/widgets.png";
  if (!texture.loadFromFile (textureFile)) {
    spdlog::error ("Unable to load {}", textureFile);
  }
  // texts
  auto texts = sgui::TextContainer ();
  texts.loadFromFile (ContentsDir"/english_demo.json", "english");
  texts.loadFromFile (ContentsDir"/french_demo.json", "french");
  texts.setTongue ("english");
  // sounds
  auto sounds = sgui::SoundHolder ();
  sounds.load ("Button",       ContentsDir"/wood1.wav");
  sounds.load ("CheckBox",     ContentsDir"/wood2.wav");
  sounds.load ("Slider",       ContentsDir"/wood2.wav");
  sounds.load ("Scroller",     ContentsDir"/wood1.wav");
  sounds.load ("MenuItem",     ContentsDir"/wood2.wav");
  sounds.load ("InputText",    ContentsDir"/metallic1.wav");
  sounds.load ("InputKey",     ContentsDir"/metallic2.wav");
  sounds.load ("InputNumber",  ContentsDir"/metallic1.wav");
  sounds.load ("DropList",     ContentsDir"/metallic2.wav");
  sounds.load ("DropListItem", ContentsDir"/metallic1.wav");
  
  /**
   * Window initialization
   */
  auto window = sf::RenderWindow (sf::VideoMode ({1900u, 1000u}), "Smolgui Demo");
  window.setFramerateLimit (60);

  /**
   * Gui initialization
   */
  auto gui = sgui::Gui ();
  gui.setResources (fonts.get ("normal"), texture, atlas);
  gui.setSounds (sounds);
  gui.setStyle (style);
  gui.setView (window);

  /**
   * Some gui settings and data
   */
  auto panel = sgui::Panel ();
  panel.position = { 16.f, 256.f};
  panel.size = {800.f, 640.f};
  panel.title = texts.get ("mainWindow");
  auto panel2 = sgui::Panel ();
  panel2.position = panel.position + sf::Vector2f (panel.size.x + 10.f, 0.f);
  panel2.size = {800.f, 520.f};
  panel2.title = texts.get ("closableWindow");
  panel2.closable = true;
  auto panel3 = sgui::Panel ();
  panel3.position = panel2.position + sf::Vector2f (panel2.size.x + 10.f, -60.f);
  panel3.size = {128.f, 40.f};
  panel3.movable = true;
  auto sliderValue = 0.1f;
  auto inputValue = 0.f;
  auto multiLine = texts.get ("textMultiLine");
  auto oneLine = texts.get ("textOneLine");
  auto oneLine2 = texts.get ("textOneLine");
  auto oneLine3 = texts.get ("textOneLine");
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
        const auto phaseMax = 10.f;
        const auto descrSlider = fmt::format ("Slider from 0 to {}, value is {}", phaseMax, sliderValue);
        gui.slider (sliderValue, 0.f, phaseMax, {descrSlider});
        gui.progressBar (sliderValue / phaseMax, gui.normalSizeOf ("b").y * sf::Vector2f (24.f, 4.f));
        gui.sameLine ();
        gui.text (fmt::format ("{} %", sliderValue / phaseMax * 100.f));
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
        gui.endWindow ();
      }
      // second window
      if (gui.beginWindow (panel)) {
        auto getText = false;
        if (gui.textButton ("Switch to french")) {
          texts.setTongue ("french");
          getText = true;
        }
        if (gui.textButton ("Switch to english")) {
          texts.setTongue ("english");
          getText = true;
        }
        if (getText) {
          multiLine = texts.get ("textMultiLine");
          oneLine = texts.get ("textOneLine");
          panel.title = texts.get ("mainWindow");
          panel2.title = texts.get ("closableWindow");
        }
        gui.separation ();

        gui.text ("Select font size");
        auto& fonts = style.fontSize;
        gui.slider (fonts.title, 12u, 26u, {"Title font size"});
        gui.slider (fonts.subtitle, 10u, 22u, {"Subtitle font size"});
        if (gui.iconTextButton ("plus", "Increase normal font size")) {
          fonts.normal = sgui::clamp (8u, 20u, fonts.normal + 1);
        }
        if (gui.iconButton ("minus")) {
          fonts.normal = sgui::clamp (8u, 20u, fonts.normal - 1);
        }
        gui.sameLine ();
        gui.text ("Decrease normal font size");
        gui.inputColor (style.fontColor, {"Font color: "});
        gui.separation ();
        gui.inputText (multiLine, {256.f, 64.f}, {texts.get ("multiLine")});
        gui.inputText (oneLine, {}, {texts.get ("oneLine")});
        gui.separation ();
        gui.inputNumber (inputValue, {"Input number with text!"});
        gui.inputText (oneLine2, {}, {texts.get ("oneLine")});
        gui.inputVector2 (vector, {"Input vector2: "});
        gui.inputText (oneLine3, {}, {texts.get ("oneLine")});
        gui.inputVector3 (vector3, {"Input vector3: "});
        gui.inputText (oneLine3, {}, {texts.get ("oneLine")});
        gui.inputVector2 (vector, {"Input vector2: "});
        gui.endWindow ();
      }

      auto constraint = sgui::Constraints ();
      constraint.vertical = sgui::VerticalAlignment::Center;
      gui.beginPanel (panel3, constraint);
      gui.text ("Just a panel");
      gui.text ("With scrollable");
      gui.addSpacing ({0.f, 12.f});
      gui.text ("text");
      gui.endPanel ();
      
      constraint.vertical = sgui::VerticalAlignment::Bottom;
      gui.beginPanel (panel3, constraint);
      gui.text ("Bottom panel");
      gui.endPanel ();
      
      constraint.vertical = sgui::VerticalAlignment::Top;
      gui.beginPanel (panel3, constraint);
      gui.text ("Top panel");
      gui.endPanel ();
      
      auto horiConstraint = sgui::Constraints ();
      horiConstraint.horizontal = sgui::HorizontalAlignment::Left;
      gui.beginPanel (panel3, horiConstraint);
      gui.text ("Left panel");
      gui.endPanel ();
      
      horiConstraint.horizontal = sgui::HorizontalAlignment::Right;
      gui.beginPanel (panel3, horiConstraint);
      gui.text ("Right panel");
      gui.endPanel ();
    }
    gui.endFrame ();

    /**
     * Drawing
     */
    window.clear ();
    window.draw (gui);
    // TEST
    const auto expression ("(3 * 4) / (#sin(x - 5 / #sqrt(3) + #sqrt (4 * e^(x-6)) + 4) + 5)");
    sgui::Formula formula (fonts.get ("math"));
    formula.printFormula (expression, window, gui.style ());
    // TEST
    window.display ();
  }
}
