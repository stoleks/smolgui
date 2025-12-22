#pragma once

#include <SFML/System/Vector2.hpp>

namespace sgui
{

/////////////////////////////////////////////////
/**
 * @brief List all possible vertical alignment
 */
enum class VerticalAlignment
{
  Top,
  Center,
  Bottom,
  None
};

/**
 * @brief List all possible horizontal alignment
 */
enum class HorizontalAlignment
{
  Left,
  Center,
  Right,
  None
};

/////////////////////////////////////////////////
/**
 * @brief Store constraints on position for gui panel. Alignment always precede relative position.
 */
struct Constraints
{
  // zero means no constraints for position
  VerticalAlignment vertical = VerticalAlignment::None;
  HorizontalAlignment horizontal = HorizontalAlignment::None;
  sf::Vector2f relativePosition = {};
};

} // namespace sgui
