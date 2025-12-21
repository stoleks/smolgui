#include <iostream>

#include <sgui/sgui.h>
#include <sgui/DefaultFiles.h>
#include <sgui/Serialization/LoadJson.h>
#include <sgui/Serialization/LoadTextureAtlas.h>
#include <sgui/Resources/TextureCollage.h>

int main ()
{
  // build collage of all widgets
  spdlog::info ("Texture collage");
  const std::string textureFile = DemoDir"/widgets_collage.png";
  auto collage = sgui::TextureCollage (SguiContentsDir"/widgets/");
  if (!collage.image ().saveToFile (textureFile)) {
    spdlog::warn ("Unable to save {}", textureFile);
  }
  const std::string atlasFile = DemoDir"/atlas_collage.json";
  spdlog::info ("Save atlas to {}", atlasFile);
  sgui::saveInFile (collage.atlas (), atlasFile);

  // load texture atlas and texture
  spdlog::info ("Load texture created in {}", textureFile);
  auto texture = sf::Texture (textureFile);
  spdlog::info ("Load atlas created in {}", atlasFile);
  auto atlas = sgui::TextureAtlas (atlasFile);
  
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
