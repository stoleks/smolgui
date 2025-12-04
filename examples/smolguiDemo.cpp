#include <iostream>
#include <chrono>
#include <thread>

#include "sgui/sgui.h"
#include "sgui/Parser/Formula.h"

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
  // texture and atlas
  auto atlas = sgui::TextureAtlas (ContentsDir"/atlas.json");
  auto texture = sf::Texture (ContentsDir"/widgets.png");
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
  auto gui = sgui::Gui (window);
  gui.setResources (fonts.get ("normal"), texture);
  gui.setTextureAtlas (atlas);
  gui.setSounds (sounds);
  gui.setStyle (style);
  gui.setView (window);

  /**
   * Some gui settings and data
   */
  auto mainPanel = sgui::Panel ();
  mainPanel.position = { 200.f, 256.f};
  mainPanel.size = { 0.315f, 0.64f };
  mainPanel.title = texts.get ("mainWindow");
  mainPanel.hasMenu = true;
  auto closablePanel = sgui::Panel ();
  closablePanel.position = mainPanel.position + sf::Vector2f (mainPanel.size.x*window.getSize ().x + 10.f, 0.f);
  closablePanel.size = { 0.315f, 0.5f };
  closablePanel.title = texts.get ("closableWindow");
  closablePanel.closable = true;
  // data
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
  auto tongue = std::string ("");
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
    // first window
    if (gui.beginWindow (closablePanel)) {
      // Open or close
      gui.text ("A window");
      gui.sameLine ();
      if (gui.textButton ("Open/Close")) {
        mainPanel.closed = !mainPanel.closed;
      }
      gui.sameLine ();
      gui.text ("Open or close the general demo window");
      // Display a function
      const auto phaseMax = 10.f;
      const auto descrSlider = fmt::format ("Slider from 0 to {}, value is {}", phaseMax, sliderValue);
      gui.slider (sliderValue, 0.f, phaseMax, {descrSlider});
      gui.progressBar (sliderValue / phaseMax, gui.textSize ("b").y * sf::Vector2f (24.f, 4.f));
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
      gui.slider (mainPanel.size.y, 0.05f, 0.7f, {"Slider from 0.05 to 0.7, value is : " + std::to_string (mainPanel.size.y)});
      gui.endWindow ();
    }
    // second window
    if (gui.beginWindow (mainPanel)) {
      auto currentTongue = std::string ("");
      gui.beginMenu ();
      if (gui.menuItem ("Switch to french")) {
        currentTongue = "french";
      }
      if (gui.menuItem ("Switch to english")) {
        currentTongue = "english";
      }
      gui.endMenu ();
      if (currentTongue != tongue) {
        tongue = currentTongue;
        texts.setTongue (tongue);
        multiLine = texts.get ("textMultiLine");
        oneLine = texts.get ("textOneLine");
        mainPanel.title = texts.get ("mainWindow");
        closablePanel.title = texts.get ("closableWindow");
      }

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
      gui.inputText (multiLine, {{256.f, 64.f}}, {texts.get ("multiLine")});
      gui.inputText (oneLine, {}, {texts.get ("oneLine")});
      gui.inputNumber (inputValue, {"Input number with text!"});
      gui.inputText (oneLine2, {}, {texts.get ("oneLine")});
      gui.inputVector2 (vector, {"Input vector2: "});
      gui.inputText (oneLine3, {}, {texts.get ("oneLine")});
      gui.inputVector3 (vector3, {"Input vector3: "});
      gui.inputText (oneLine3, {}, {texts.get ("oneLine")});
      gui.inputVector2 (vector, {"Input vector2: "});
      gui.endWindow ();
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
