#pragma once

#include <array>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sgui
{
using Mesh = std::array <sf::Vertex, 6>;

/**
 * @brief Compute mesh quad coordinates
 */
void computeMesh (Mesh& mesh, const sf::FloatRect& box);

/**
 * @brief compute texture coordinates from a rect
 */
void computeMeshTexture (Mesh& Mesh, const sf::IntRect& textureRect);

/**
 * @brief Rotate a mesh texture by 90°
 */
void rotateMeshTexture (Mesh& mesh);

} // namespace sgui
