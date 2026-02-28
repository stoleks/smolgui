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
  auto mainPanel = sgui::Panel ({{}, { 1.f, 1.f }});
  mainPanel.title = fmt::format ("Main window with fontawesome |{}| !", ICON_FA_FONT_AWESOME);
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
    window.clear (sf::Color::White);
    window.draw (gui);
    window.display ();
  }
}
