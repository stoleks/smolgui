/**
  MeshFunctions.h
  Purpose : implement functions related to mesh rendering : mesh coordinates or
    mesh texture coordinate, texture rotation by 90°.
  @author A. J.
*/

#pragma once

#include <array>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sgui
{
using Mesh = std::array <sf::Vertex, 6>;

/**
 * Compute mesh quad coordinates
 */
void computeMesh (
  Mesh& mesh,
  const sf::FloatRect& box);

/**
 * compute texture coordinates from a rect
 */
void computeMeshTexture (
  Mesh& Mesh,
  const sf::IntRect& textureRect);

/**
 * rotate a mesh texture by 90°
 */
void rotateMeshTexture (Mesh& mesh);

} // namespace sgui
