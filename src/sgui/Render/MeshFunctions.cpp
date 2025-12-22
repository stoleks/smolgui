#include "sgui/Render/MeshFunctions.h"

namespace sgui
{
/////////////////////////////////////////////////
void computeMesh (Mesh& mesh, const sf::FloatRect& box)
{
  const auto left   = box.position.x;
  const auto top    = box.position.y;
  const auto width  = box.size.x;
  const auto height = box.size.y;
  // first triangle (top left)
  mesh[0].position = sf::Vector2f (left,         top);
  mesh[1].position = sf::Vector2f (left + width, top);
  mesh[2].position = sf::Vector2f (left,         top + height);
  // second triangle (bottom right)
  mesh[3].position = sf::Vector2f (left + width, top);
  mesh[4].position = sf::Vector2f (left + width, top + height);
  mesh[5].position = sf::Vector2f (left,         top + height);
}

/////////////////////////////////////////////////
void computeMeshTexture (Mesh& mesh, const sf::IntRect& textureBox)
{
  const auto left   = static_cast<float> (textureBox.position.x);
  const auto top    = static_cast<float> (textureBox.position.y);
  const auto width  = static_cast<float> (textureBox.size.x);
  const auto height = static_cast<float> (textureBox.size.y);
  // first triangle (top left)
  mesh[0].texCoords = sf::Vector2f (left,         top);
  mesh[1].texCoords = sf::Vector2f (left + width, top);
  mesh[2].texCoords = sf::Vector2f (left,         top + height);
  // second triangle (bottom right)
  mesh[3].texCoords = sf::Vector2f (left + width, top);
  mesh[4].texCoords = sf::Vector2f (left + width, top + height);
  mesh[5].texCoords = sf::Vector2f (left,         top + height);
}

/////////////////////////////////////////////////
void rotateMeshTexture (Mesh& mesh)
{
  // Rotate texture
  const auto topLeftCorner = mesh[0].texCoords;
  // top left -> bottom left
  mesh[0].texCoords = mesh[2].texCoords;
  // bottom left -> bottom right
  mesh[2].texCoords = mesh[4].texCoords;
  mesh[5].texCoords = mesh[4].texCoords;
  // bottom right -> top right
  mesh[4].texCoords = mesh[3].texCoords;
  // top right -> top left
  mesh[3].texCoords = topLeftCorner;
  mesh[1].texCoords = topLeftCorner;
}

} // namespace sgui
