#include <iostream>
#include <chrono>
#include <thread>

#include "gui/sgui.h"

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
  panel.position = { 128.f, 128.f};
  panel.size = {640.f, 640.f};
  auto panel2 = sgui::Panel ();
  panel2.position = panel.position + sf::Vector2f (panel.size.x + 20.f, 0.f);
  panel2.size = {520.f, 520.f};
  panel2.closable = true;
  auto sliderValue = 0.1f;
  auto inputValue = 0.f;
  auto text = std::string ("You can edit this text on multiple lines !");
  auto text2 = std::string ("You can edit this text !");
  auto vector = sf::Vector2f ();
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
    const auto dt = timer.restart ();
    timeSinceLastUpdate += dt;
    /**
     * Input and logic
     */
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
    gui.beginFrame ();
    {
      // first window
      if (gui.beginWindow (panel2, "Closable window")) {
        // Open or close
        if (gui.textButton ("Open/Close")) {
          panel.closed = !panel.closed;
        }
        gui.setAnchor ();
        gui.sameLine ();
        gui.text ("Open or close the general demo window");
        gui.backToAnchor ();
        gui.separation ();
        // Display a function
        gui.slider (sliderValue, 0.f, 10.f, "Slider from 0 to 1, value is : " + std::to_string (sliderValue));
        gui.checkBox (displayFunction, "Display a function");
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
        gui.slider (panel.size.y, 50.f, 700.f, "Slider from 0 to 700, value is : " + std::to_string (sliderValue));
      }
      gui.endWindow ();
      // second window
      if (gui.beginWindow (panel, "General demo")) {
        if (gui.textButton ("Test button")) {
          spdlog::info ("Test button\n");
        }
        gui.separation ();

        gui.text ("Select font size");
        auto& fonts = style.fontSize;
        gui.slider (fonts.title, 12u, 26u, "Title font size");
        gui.slider (fonts.subtitle, 10u, 22u, "Subtitle font size");
        gui.slider (fonts.normal, 8u, 20u, "Normal font size");
        gui.inputColor (style.fontColor, "Font color");
        gui.separation ();
        gui.inputText (text, "Editable on multiple lines : ", {256.f, 64.f});
        gui.inputText (text2, "Editable on one line");
        gui.separation ();
        gui.inputNumber (inputValue, "Input number with text!");
        gui.inputVector2 (vector);
      }
      gui.endWindow ();
    }
    gui.endFrame ();
    /**
     * Drawing
     */
    window.clear ();
    gui.draw (window);
    window.display ();
  }
}
