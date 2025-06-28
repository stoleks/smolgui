#include <iostream>

#include <sgui/sgui.h>

int main ()
{
  /**
   * load texture atlas and texture
   */
  auto atlas = sgui::TextureAtlas ();
  atlas.loadFromFile (ContentsDir"/atlas.json");
  // texture
  auto texture = sf::Texture ();
  const std::string textureFile = ContentsDir"/widgets.png";
  if (!texture.loadFromFile (textureFile)) {
    spdlog::error ("Unable to load {}", textureFile);
  }
  
  /**
   * build a vertex array that display all textures
   */
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
  
  /**
   * open window and draw vertex array
   */
  auto window = sf::RenderWindow (sf::VideoMode ({1920u, 1080u}), "Smolgui Demo");
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
