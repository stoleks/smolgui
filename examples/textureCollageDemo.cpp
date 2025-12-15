#include <iostream>

#include <sgui/sgui.h>
#include <sgui/DefaultFiles.h>
#include <sgui/Serialization/LoadJson.h>
#include <sgui/Serialization/LoadTextureAtlas.h>
#include <sgui/Resources/TextureCollage.h>

int main ()
{
  // build collage of all widgets
  auto collage = sgui::TextureCollage (ContentsDir"/widgets/");
  if (!collage.image ().saveToFile (ExamplesDir"/widgets_collage.png")) {
    spdlog::warn ("Unable to save {}", ExamplesDir"/widgets_collage.png");
  }
  sgui::saveInFile (collage.atlas (), ExamplesDir"/atlas_collage.json");

  // load texture atlas and texture
  auto atlas = sgui::TextureAtlas (ExamplesDir"/atlas_collage.json");
  auto texture = sf::Texture (ExamplesDir"/widgets_collage.png");
  
  // build a vertex array that display all textures
  sf::VertexArray sprites;
  sprites.setPrimitiveType (sf::PrimitiveType::Triangles);
  auto textureMeshes = sgui::TextureMeshes ();
  textureMeshes.computeTextureMapping (atlas);
  for (auto& mesh : textureMeshes) {
    for (auto& vertex : mesh) {
      vertex.position = vertex.texCoords;
      sprites.append (vertex);
    }
  }
  
  // open window and draw vertex array
  auto window = sf::RenderWindow (sf::VideoMode ({1920u, 1080u}), "Texture collage demo");
  window.setFramerateLimit (60);
  auto view = window.getDefaultView ();
  view.zoom (1.5f);
  window.setView (view);
  while (window.isOpen ())
  {
    while (const std::optional event = window.pollEvent ())
    {
      if (event->is <sf::Event::Closed> ()) {
        window.close ();
      }
    }
    window.clear ();
    window.draw (sprites, &texture);
    window.display ();
  }
}
