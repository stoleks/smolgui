#pragma once

#include <array>
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace sgui
{
constexpr float Pi = 3.14159265359f;
constexpr float PiRad = 3.14159265359f / 180.f;

/////////////////////////////////////////////////
/**
 * @brief Define a line with two points
 */
template <typename Type>
struct Line {
  Line () = default;
  Line (const sf::Vector2<Type>& begin, const sf::Vector2<Type>& end);
  /**
   * @brief get direction of the line
   */
  sf::Vector2<Type> direction () const;
  sf::Vector2<Type> tail = {}; ///< tail of the line
  sf::Vector2<Type> head = {}; ///< head of the line
};

// typedef for common line type
using LineFloat = Line<float>;


/////////////////////////////////////////////////
/**
 * @brief Define a Circle with a center and a radius
 */
template <typename Type>
struct Circle
{
  Circle () = default;
  Circle (const sf::Vector2<Type>& center, Type r);
  Type radius = Type (0);        ///< radius of the circle
  sf::Vector2<Type> center = {}; ///< center of the circle
};

// typedef for common circle type
using CircleFloat = Circle<float>;


/////////////////////////////////////////////////
/**
 * @brief Return center of a set of points
 */
template<typename Type>
constexpr sf::Vector2f centroid (const std::vector<sf::Vector2 <Type>>& vertices);

} // namespace sgui

// impl
#include "sgui/Core/Shapes.tpp"
