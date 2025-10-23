#include <iostream>
#include <chrono>
#include <thread>

#include "sgui/Gui.h"
#include "sgui/Resources/Layout.h"

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
  // layout
  auto layout = sgui::Layout ();
  layout.loadFromFile (ContentsDir"/layout.json");
  
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
  gui.setStyle (style);
  gui.setView (window);

  /**
   * Load gui layout and set data
   */
  auto mainPanel = layout.get <sgui::Panel> ("mainPanel");
  auto closablePanel = layout.get <sgui::Panel> ("closablePanel");
  auto constrainedPanel = layout.get <sgui::Panel> ("constrainedPanels");
  // data
  auto sliderValue = 0.1f;
  bool displayFunction = false;
  bool compactLayout = false;

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
      if (gui.beginWindow (closablePanel)) {
        // Open or close
        gui.text ("A window");
        gui.sameLine ();
        if (gui.textButton ("Open/Close")) {
          mainPanel.closed = !mainPanel.closed;
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
        gui.slider (mainPanel.size.y, 50.f, 700.f, {"Slider from 0 to 700, value is : " + std::to_string (mainPanel.size.y)});
        gui.endWindow ();
      }

      if (gui.beginWindow (mainPanel)) {
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
          mainPanel.title = texts.get ("mainWindow");
          closablePanel.title = texts.get ("closableWindow");
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
        gui.checkBox (compactLayout, {"Compact layout"});
        if (gui.textButton ("Save layout")) {
          layout.saveInFile (compactLayout);
        }
        gui.endWindow ();
      }

      // scrollabel centered panel
      auto constraint = layout.get <sgui::Constraints> ("alignment");
      constraint.vertical = sgui::VerticalAlignment::Center;
      gui.beginPanel (constrainedPanel, constraint); {
        gui.text ("Scrollable panel,");
        gui.addSpacing ({0.f, 12.f});
        gui.text ("with text.");
      } gui.endPanel ();
      // bottom panel
      constraint.vertical = sgui::VerticalAlignment::Bottom;
      gui.beginPanel (constrainedPanel, constraint); {
        gui.text ("Bottom panel");
      } gui.endPanel ();
      // top panel
      constraint.vertical = sgui::VerticalAlignment::Top;
      gui.beginPanel (constrainedPanel, constraint); {
        gui.text ("Top panel");
      } gui.endPanel ();
      // left panel
      constraint.vertical = sgui::VerticalAlignment::None;
      constraint.horizontal = sgui::HorizontalAlignment::Left;
      gui.beginPanel (constrainedPanel, constraint); {
        gui.text ("Left panel");
      } gui.endPanel ();
      // right panel
      constraint.horizontal = sgui::HorizontalAlignment::Right;
      gui.beginPanel (constrainedPanel, constraint); {
        gui.text ("Right panel");
      } gui.endPanel ();
    }
    gui.endFrame ();

    /**
     * Drawing
     */
    window.clear ();
    window.draw (gui);
    window.display ();
  }
}
