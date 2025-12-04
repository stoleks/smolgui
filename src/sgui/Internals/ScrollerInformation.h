#pragma once

#include <SFML/System/Vector2.hpp>

namespace sgui
{
namespace Impl
{
/**
 * @brief class to store group scroller position and to compute its size
 */
class ScrollerInformation
{
public:
  /**
   * @brief set group box 
   */
  void update (const sf::Vector2f& groupPosition);
  /**
   * @brief compute scrollbar size according to the group internal size
   */
  void computeScrollSize (const sf::Vector2f& cursorPosition);
  /**
   * @brief get scrollbar size of the group
   */
  sf::Vector2f size () const;
public:
  /**
   * @brief store current percent of scroll bar
   */
  float percent = 0.f;
  sf::Vector2f mPositionMax = {};
  sf::Vector2f mGroupPosition = {};
};

} // namespace Impl
} // namespace sgui
