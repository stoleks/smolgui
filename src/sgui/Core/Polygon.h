/**
  Polygon.h
  Purpose : Simple data structure to make regular polygons and compute their center.
  @author A. J.
*/

#pragma once

#include <cmath>
#include "Shapes.h"

namespace sgui
{
/////////////////////////////////////////////////
/**
 * Polygon is a series of N points, ranged in
 * clock-wise order and that form a convex shape.
 */
template <size_t N>
struct Polygon {
  std::array <sf::Vector2f, N> points;
};

// typedef for common polygon type
using Tetragon = Polygon <4>;
using Hexagon  = Polygon <6>;
using Octogon  = Polygon <8>;

/**
 * make a regular polygon, like hexagon or octogon
 */
template <size_t N>
Polygon<N> makeRegularPolygon (
  const sf::Vector2f& center,
  const float size);


/////////////////////////////////////////////////
/**
 * Return center of polygon
 */
template <size_t N>
constexpr sf::Vector2f centroid (const Polygon<N>& polygon);

/**
 * return a box bounding polygon
 */
template <size_t N>
constexpr sf::FloatRect boundingBox (const Polygon<N>& polygon);

} // namespace sgui


#include "Polygon.tpp"
