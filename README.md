smol gui
==========
A "small" immediate gui library based on [SFML](https://github.com/SFML/SFML), that aims to ease use of personal assets and resources.

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

It will integrates SFML::Graphics, spdlog and nlohmann_json with it. You compiler should supports c++17.

If you want to build the demos, you can set the BUILD_EXAMPLES command to ON 
```console
  cmake -D BUILD_SGUI_EXAMPLES=ON
```

You can also build documentation with doxygen.

Basic usage
-----

This library offers an easy to use immediate gui for SFML user, every functions and classes are in the namespace sgui.
This library requires a sf::Window, a sf::Font, a sf::Texture that contains widgets textures and a sgui::TextureAtlas that defines the sprites position and size of every UI widgets in the sprite sheet.
You can look at the default [texture](https://github.com/stoleks/smolgui/blob/main/contents/widgets.png) and [atlas](https://github.com/stoleks/smolgui/blob/main/contents/atlas.json) in the contents/ folder.

For a basic usage, all you need is in sgui/sgui.h header.

```cpp
#include <sgui/sgui.h>

...

auto window = sf::RenderWindow (sf::VideoMode ({640u, 480u}), "A window");
auto font = sf::Font ("my_font.ttf");
auto texture = sf::Texture ("my_texture.png");
auto atlas = sgui::TextureAtlas ("my_atlas.json");
auto gui = sgui::Gui (font, texture, atlas, window);
```

If you want to use default texture, atlas and font furnished with this library, you can use sgui::DefaultTexture, sgui::DefaultAtlas and sgui::DefaultFont.
When your sgui::Gui is initialized, you need to update its inputs and internal timer:

```cpp
while (window.isOpen ())
{
  while (const std::optional event = window.pollEvent ())
  {
    if (event->is <sf::Event::Closed> ()) {
      window.close ();
    }
    gui.update (window, event);
  }
  gui.updateTimer ();
}
```

Once this is done, you can begin to use gui features, anywhere in the main loop, but you need to enclose the functions called by a pair of Gui::beginFrame and Gui::endFrame call.

```cpp
gui.beginFrame ();
// your gui code needs to be in-between this two functions calls.
gui.endFrame ();
```

After the call to Gui::endFrame, you can draw the gui like a SFML drawable 

```cpp
window.clear ();
window.draw (gui);
window.display ();
```

All widgets in the gui are automatically placed vertically by default.
If you want to organize them, you have two kind of container: a "Window" or a "Panel". 
A window is closable, reducable and may have a menu bar, but both have a texture and are movable by default. 
To specify their dimensions, you need to make a sgui::Panel and set its absolute pixel position and its relative fractional size. 
The size is relative to the container in which it is opened, its parent.
If no container is opened, its parent is simply the SFML window.
If you don't specify a position or set-it to (0.f, 0.f), the container will be automatically placed after the last widget opened.
```cpp
gui.beginFrame ();
// set two Panel, one for a window that will take all the sfml window, one for a panel that will be half of its parent size
auto guiWindow = sgui::Panel ({{}, {1.f, 1.f});
auto childPanel = sgui::Panel ({{}, {0.5f, 0.5f});
// open a window 
if (gui.beginWindow (guiWindow)) {
  gui.text ("Text in the window before the panel");
  // open a panel half the size of the window in it
  gui.beginPanel (childPanel);
  gui.text ("Text in the child panel");
  gui.endPanel ();
  gui.text ("Text in the window after the panel");
  gui.endWindow ();
}
// open a panel after the window, half the size of the SFML window and at 40 pixels from the top-left corner
auto freePanel = sgui::Panel ({{40.f 40.f}, {0.5f, 0.5f}});
gui.beginPanel (freePanel)
gui.text ("Text in the free panel");
gui.endPanel ();
gui.endFrame ();
```
Please note that if you want to move your Panel or Window, you need to declare your sgui::Panel outside of the main loop.


Resources management
-----

Several classes are defined to ease resources management and to give more control outside of the code to user.
- Layout allow to save and load from file gui layout with a simple key system. 
It is used to store positions and constraints of panel and window.
- TextureCollage allow to make a collage of several images in a specified folder in one texture, with automatic atlas generation.
- TextContainer allow to store and load text in multiple language, with one file per language.
- ResourcesHolder ease loading and management of resources.
- TextureAtlas allow to define where widgets textures are located in the sprite sheet.


List of widgets implemented
-----

Here is the complete list of widgets implemented, you should look a the smolgui demo if you want examples of uses: 
- text: display a text
- image: display an image in your sprite sheet
- separation : draw a line of separation
- button: clickable button with a text
- icon: clickable button with a font awesome icon
- checkBox: clickable check box
- menuItem: button in a window menu.
- inputNumber: to modify a number
- inputVector2: to modify a sf::Vector2
- inputVector3: to modify a sf::Vector3
- inputColor: to modifiy a sf::Color
- inputText: to modify a text
- inputKey: to modify a char
- progressBar: draw a progress bar
- comboBox: display a selected std::string and a drop-down list of std::string
- slider: to change a number between two value
- plot: to display a function

All widgets have a WidgetOptions parameter that allow to add a text description, change its position, add a tooltip.
Options can also, set its render or change its size, depending on the widget.

Complete example code
-----

You can find demo with more widgets used in the examples folder.

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
  auto gui = sgui::Gui (font, texture, atlas, window);
  // Window settings and main loop
  auto mainPanel = sgui::Panel ({ 1.f, 1.f });
  mainPanel.title = fmt::format ("Main window with fontawesome |{}|", ICON_FA_FONT_AWESOME);
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
    gui.updateTimer ();
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
      gui.inputColor (style.fontColor, {"font color"});
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


