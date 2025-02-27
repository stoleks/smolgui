/**
  Shapes.h
  Purpose : Defines structure for Circle and Line, so that they can be used to be  
    drawn or for collision. They have minimal functionalities.
  @author A. J.
*/

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
 * Line is defined by two point
 */
template <typename Type>
struct Line {
  Line () = default;
  Line (const sf::Vector2<Type>& begin,
        const sf::Vector2<Type>& end);
  /**
   * get direction of the line
   */
  sf::Vector2<Type> direction () const;
  /**
   * data
   */
  sf::Vector2<Type> tail;
  sf::Vector2<Type> head;
};

// typedef for common line type
using LineFloat = Line<float>;


/////////////////////////////////////////////////
/**
 * Circle is a center and radius
 */
template <typename Type>
struct Circle
{
  Circle () = default;
  Circle (const sf::Vector2<Type>& center, Type r);
  /**
   * data
   */
  Type radius;
  sf::Vector2<Type> center;
};

// typedef for common circle type
using CircleFloat = Circle<float>;


/////////////////////////////////////////////////
/**
 * Return center of a set of points
 */
template<typename Type>
constexpr sf::Vector2f centroid (
  const std::vector<sf::Vector2 <Type>>& vertices);

} // namespace sgui

// impl
#include "Shapes.tpp"
