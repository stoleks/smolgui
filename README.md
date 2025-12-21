smol gui
==========
A small immediate gui library based on [SFML](https://github.com/SFML/SFML)

Dependencies
-----

* [SFML](https://github.com/SFML/SFML)
* [nlohmann-json](https://github.com/nlohmann/json)
* [spdlog](https://github.com/gabime/spdlog)
* [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders)

Building and integrating into your CMake project
-----

Using FetchContent you can integrate this library like this
```cmake
include (FetchContent)

FetchContent_Declare (SmolGui
  GIT_REPOSITORY https://github.com/stoleks/smolgui
  GIT_TAG        v0.9.6
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable (smolGui)

target_link_libraries (project
  PUBLIC
    SmolGui
)
```

It will integrates SFML::Graphics, spdlog and nlohmann_json with it.

Example code
-----

You can find more complete examples in the examples folder.

```cpp
#include <sgui/Gui.h>
#include <sgui/DefaultFiles.h>
#include <sgui/Resources/IconsFontAwesome7.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Image.hpp>

int main()
{
  // Resources loading
  auto font = sf::Font (sgui::DefaultFont);
  auto atlas = sgui::TextureAtlas (sgui::DefaultAtlas);
  auto texture = sf::Texture (sgui::DefaultTexture);
  // Window initialization
  auto window = sf::RenderWindow (sf::VideoMode ({640u, 480u}), "Minimal Demo");
  window.setFramerateLimit (60);
  // For demo render
  sf::RenderTexture image ({640u, 480u});
  auto exportSuccess = false;
  // Gui initialization
  auto gui = sgui::Gui ();
  gui.initialize (font, texture, atlas, window);
  // Window settings and main loop
  auto mainPanel = sgui::Panel ({ 1.f, 1.f });
  mainPanel.title = fmt::format ("Main window with fontawesome |{}|", ICON_FA_FONT_AWESOME);
  auto timer = sf::Clock ();
  auto combo = std::vector <std::string> { "One", "Two", "Three", "Four" };
  auto style = sgui::Style ();
  while (window.isOpen ())
  {
    // Inputs
    while (const std::optional event = window.pollEvent ())
    {
      if (event->is <sf::Event::Closed> ()) {
        window.close ();
      }
      gui.update (window, event);
    }
    gui.setStyle (style);
    gui.updateTimer (timer.restart ());
    // Gui
    gui.beginFrame ();
    if (gui.beginWindow (mainPanel)) {
      if (gui.button ("Close window")) {
        window.close ();
      }
      gui.text ("Select font size");
      const auto descr = fmt::format ("Title font |{}| size is {}", ICON_FA_FONT, style.fontSize.title);
      gui.slider (style.fontSize.title, 12u, 26u, {descr});
      if (gui.icon (ICON_FA_SQUARE_PLUS, {"Increase normal font size"})) {
        style.fontSize.normal = sgui::clamp (8u, 20u, style.fontSize.normal + 1);
      }
      if (gui.icon (ICON_FA_SQUARE_MINUS, {"Decrease normal font size"})) {
        style.fontSize.normal = sgui::clamp (8u, 20u, style.fontSize.normal - 1);
      }
      gui.text (fmt::format ("|{}| Normal font size is {}", ICON_FA_PEN, style.fontSize.normal));
      const auto selected = gui.comboBox (combo);
      gui.inputColor (style.fontColor, {"Font color: "});
      gui.text (selected);
      const auto pngFile = DemoDir"/minimalDemo.png";
      gui.text (fmt::format ("Saved to file assets/minimalDemo.png with success {}", exportSuccess));
      if (gui.button ("Save demo in file")) {
        image.clear ();
        image.draw (gui);
        image.display ();
        exportSuccess = image.getTexture ().copyToImage ().saveToFile (pngFile);
      }
      gui.endWindow ();
    }
    gui.endFrame ();

    // Drawing
    window.clear ();
    window.draw (gui);
    window.display ();
  }
}
```

![Screenshot of minimal demo](https://github.com/stoleks/smolgui/blob/main/examples/assets/minimalDemo.png)
