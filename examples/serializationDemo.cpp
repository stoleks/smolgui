#include <iostream>
#include <chrono>
#include <thread>

#include <sgui/Gui.h>
#include <sgui/Resources/Layout.h>
#include <sgui/Resources/IconsFontAwesome7.h>

int main()
{
  /**
   * Resources loading
   */
  // fonts
  auto fonts = sgui::FontHolder ();
  fonts.load ("normal", ContentsDir"/Luciole-Regular.ttf");
  fonts.load ("bold",   ContentsDir"/Luciole-Bold.ttf");
  // texture and atlas
  auto atlas = sgui::TextureAtlas (ContentsDir"/atlas.json");
  auto texture = sf::Texture (ContentsDir"/widgets.png");
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
  auto window = sf::RenderWindow (sf::VideoMode ({1900u, 1000u}), "Serialization Demo");
  window.setFramerateLimit (60);

  /**
   * Gui initialization
   */
  auto gui = sgui::Gui ();
  gui.initialize (fonts.get ("normal"), texture, atlas, window);

  /**
   * check that texture size is valid
   */
  const auto tex = std::string ("Icon::reduce");
  const auto texSize = gui.textureSize (tex);
  spdlog::info ("Size of {} is ({}, {}), it should be (128, 128)", tex, texSize.x, texSize.y);
  const auto tex2 = std::string ("Separation::Act::Cent");
  const auto texSize2 = gui.textureSize (tex2);
  spdlog::info ("Size of {} is ({}, {}), it should be (64, 128)", tex2, texSize2.x, texSize2.y);

  /**
   * Load gui layout and set data
   */
  auto mainPanel = layout.get <sgui::Panel> ("mainPanel");
  mainPanel.hasMenu = true;
  auto closablePanel = layout.get <sgui::Panel> ("closablePanel");
  auto constrainedPanel = layout.get <sgui::Panel> ("constrainedPanels");
  // data
  bool displayFunction = false;
  bool compactLayout = false;
  auto sliderValue = 0.1f;
  auto inputValue = 0.f;
  auto multiLine = texts.get ("textMultiLine");
  auto oneLine = texts.get ("textOneLine");
  auto vector = sf::Vector2f ();
  auto vector3 = sf::Vector3f ();

  /**
   * Main App loop
   */
  auto style = sgui::Style ();
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
    auto constraint = layout.get <sgui::Constraints> ("alignment");
    gui.beginFrame ();
    {
      if (gui.beginWindow (closablePanel)) {
        // Open or close
        gui.text ("A window");
        if (gui.button ("Open/Close")) {
          mainPanel.closed = !mainPanel.closed;
        }
        gui.sameLine ();
        gui.text ("Open or close the general demo window");
        gui.separation ();
        if (gui.button ("Add/remove header")) {
          mainPanel.hasHeader = !mainPanel.hasHeader;
        }
        if (gui.button ("Add/remove closable options")) {
          closablePanel.closable = !closablePanel.closable;
        }
        gui.text (texts.get ("centeredText"), {sgui::HorizontalAlignment::Center});
        gui.separation ();
        // set function
        const auto phaseMax = 10.f;
        const auto descrSlider = fmt::format ("Slider from 0 to {}, value is {}", phaseMax, sliderValue);
        gui.slider (sliderValue, 0.f, phaseMax, {descrSlider});
        gui.progressBar (sliderValue / phaseMax, gui.textSize ("b").y * sf::Vector2f (24.f, 4.f));
        gui.sameLine ();
        gui.text (fmt::format ("{} %", sliderValue / phaseMax * 100.f));
        // display it
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
        gui.slider (mainPanel.size.y, 0.05f, 0.7f, {"Slider from 0.05 to 0.7, value is : " + std::to_string (mainPanel.size.y)});
        gui.endWindow ();
      }

      if (gui.beginWindow (mainPanel)) {
        auto getText = false;
        // use menu to change tongue
        gui.beginMenu ();
        if (gui.menuItem ("Switch to french")) {
          texts.setTongue ("french");
          getText = true;
        }
        if (gui.menuItem ("Switch to english")) {
          texts.setTongue ("english");
          getText = true;
        }
        gui.endMenu ();
        if (getText) {
          mainPanel.title = texts.get ("mainWindow");
          closablePanel.title = texts.get ("closableWindow");
          multiLine = texts.get ("textMultiLine");
          oneLine = texts.get ("textOneLine");
        }
        gui.separation ();
        // text and text edit
        gui.text ("Select font size");
        auto& fonts = style.fontSize;
        gui.slider (fonts.title, 12u, 26u, {"Title font size"});
        gui.slider (fonts.subtitle, 10u, 22u, {"Subtitle font size"});
        if (gui.icon (ICON_FA_SQUARE_PLUS, {"Increase normal font size"})) {
          fonts.normal = sgui::clamp (8u, 20u, fonts.normal + 1);
        }
        if (gui.icon (ICON_FA_SQUARE_MINUS, {"Decrease normal font size"})) {
          fonts.normal = sgui::clamp (8u, 20u, fonts.normal - 1);
        }
        gui.inputText (multiLine, {{256.f, 64.f}}, {texts.get ("multiLine")});
        gui.inputText (oneLine, {}, {texts.get ("oneLine")});
        // input number and color 
        gui.separation ();
        const auto shorterList = std::vector <std::string> {"One", "Two", "Three"};
        gui.comboBox (shorterList);
        gui.inputColor (style.fontColor, {"Font color: "});
        gui.inputNumber (inputValue, {"Input number with text!"});
        gui.inputVector2 (vector, {"Input vector2: "});
        gui.inputVector3 (vector3, {"Input vector3: "});
        gui.sameLine ();
        const auto list = std::vector <std::string> {"1", "2", "3", "4", "5", "6", "7", "8"};
        gui.comboBox (list);
        gui.checkBox (compactLayout, {"Compact layout"});
        if (gui.button ("Save layout")) {
          layout.saveInFile (compactLayout);
        }
        // top panel
        constraint.vertical = sgui::VerticalAlignment::Top;
        constraint.horizontal = sgui::HorizontalAlignment::None;
	const auto outSize = constrainedPanel.size;
	constrainedPanel.size *= 3.f;
        gui.beginPanel (constrainedPanel, constraint);
        gui.text ("Top panel in group");
        gui.endPanel ();
	constrainedPanel.size = outSize;
        gui.endWindow ();
      }
      // bottom panel
      constraint.vertical = sgui::VerticalAlignment::Bottom;
      gui.beginPanel (constrainedPanel, constraint);
      gui.text ("Bottom panel");
      gui.endPanel ();
      // top panel
      constraint.vertical = sgui::VerticalAlignment::Top;
      gui.beginPanel (constrainedPanel, constraint);
      gui.text ("Top panel");
      gui.endPanel ();
      // left panel
      constraint.vertical = sgui::VerticalAlignment::None;
      constraint.horizontal = sgui::HorizontalAlignment::Left;
      gui.beginPanel (constrainedPanel, constraint);
      gui.text ("Left panel");
      gui.endPanel ();
      // right panel
      constraint.horizontal = sgui::HorizontalAlignment::Right;
      gui.beginPanel (constrainedPanel, constraint);
      gui.text ("Right panel");
      gui.endPanel ();
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
