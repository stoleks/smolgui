smol gui
==========
A small immediate gui library based on [SFML](https://github.com/SFML/SFML)

Dependencies
-----

* [SFML](https://github.com/SFML/SFML)
* [nlohmann-json](https://github.nlohmann/json.git)
* [spdlog](https://github.com/gabime/spdlog)

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
#include "sgui/sgui.h"

int main()
{
  // Resources loading
  auto style = sgui::Style ();
  style.fontColor = sf::Color::White;
  auto font = sf::Font (ContentsDir"/Luciole-Regular.ttf");
  auto atlas = sgui::TextureAtlas (ContentsDir"/atlas.json");
  auto texture = sf::Texture (ContentsDir"/widgets.png");
  // Window initialization
  auto window = sf::RenderWindow (sf::VideoMode ({640u, 480u}), "Minimal Demo");
  window.setFramerateLimit (60);
  // Gui initialization
  auto gui = sgui::Gui ();
  gui.setResources (font, texture);
  gui.setTextureAtlas (atlas);
  gui.setStyle (style);
  gui.setView (window);
  // Window settings and main loop
  auto mainPanel = sgui::Panel ({ 0.f, 0.f}, { 1.f, 1.f });
  mainPanel.title = std::string("Main window");
  auto timer = sf::Clock ();
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
      if (gui.textButton ("Close window")) {
        window.close ();
      }
      gui.text ("Select font size");
      gui.slider (style.fontSize.title, 12u, 26u, {"Title font size"});
      if (gui.iconTextButton ("plus", "Increase normal font size")) {
        style.fontSize.normal = sgui::clamp (8u, 20u, style.fontSize.normal + 1);
      }
      if (gui.iconTextButton ("minus", "Decrease normal font size")) {
        style.fontSize.normal = sgui::clamp (8u, 20u, style.fontSize.normal - 1);
      }
      gui.inputColor (style.fontColor, {"Font color: "});
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

![Screenshot of minimal demo](https://github.com/stoleks/smolgui/blob/main/examples/minimalDemo.png)
