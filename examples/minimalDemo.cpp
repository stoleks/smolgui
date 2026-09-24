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
  const auto windowSize = sf::Vector2u { 640u, 480u };
  auto window = sf::RenderWindow (sf::VideoMode (windowSize), "Minimal Demo");
  // For demo render
  sf::RenderTexture image (windowSize);
  // Gui initialization
  auto gui = sgui::Gui (font, texture, atlas, window);
  // Window settings and main loop
  auto mainPanel = sgui::Panel {.title = fmt::format ("Small demo with fontawesome !")};
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
      const auto descr = fmt::format (
        "Title font <fa>{}</fa> size <fa>{}</fa> is {}",
        ICON_FA_FONT, ICON_FA_TEXT_HEIGHT, style.fontSize.title
      );
      gui.slider (style.fontSize.title, 12u, 26u, {.description = descr});
      if (gui.icon (ICON_FA_SQUARE_PLUS, {.description = "Increase normal font size"})) {
        style.fontSize.normal = sgui::clamp (8u, 20u, style.fontSize.normal + 1);
      }
      if (gui.icon (ICON_FA_SQUARE_MINUS, {.description = "Decrease normal font size"})) {
        style.fontSize.normal = sgui::clamp (8u, 20u, style.fontSize.normal - 1);
      }
      gui.text (fmt::format ("<fa>{}</fa> Normal font size is {}", ICON_FA_PEN, style.fontSize.normal));
      gui.inputColor (style.fontColor, {.description = "font color"});
      if (gui.button ("Save demo in file")) {
        image.clear (sf::Color::White);
        image.draw (gui);
        image.display ();
        image.getTexture ().copyToImage ().saveToFile (DemoDir"/minimalDemo.png");
      }
      gui.endWindow ();
    }
    gui.endFrame ();

    // Drawing
    window.clear (sf::Color::White);
    window.draw (gui);
    window.display ();
  }
}
