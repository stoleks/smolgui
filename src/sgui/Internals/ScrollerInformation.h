#pragma once

#include <SFML/Graphics/Rect.hpp>

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
  void update (const sf::FloatRect& groupBox);
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
private:
  sf::FloatRect mGroupBox;
  sf::Vector2f mMaxCursorPosition = {};
  float mTempSize = 0.f;
  float mScrollSize = 0.f;
};

} // namespace Impl
} // namespace sgui
