#pragma once

#include <SFML/System/Vector2.hpp>

namespace sgui
{
/**
 * @brief class to store group scroller position and to compute its size
 */
class ScrollerInformation
{
public:
  /**
   * @brief construct a scrollbar along x or y axis
   */
  ScrollerInformation (const bool horizontal);
  /**
   * @brief start a new computation cycle of the scroll size, should be called every
   *   beginFrame in the gui
   */
  void newCycle ();
  /**
   * @brief scroll over the group by a given amount, it will be clamped to [0, 1]
   */
  void scroll (const float amount);
  /**
   * @brief set scrollbar size
   */
  void setScrollSize (const float size);
  /**
   * @brief compute scrollbar size according to the group internal size
   */
  void computeScrollSize (const sf::Vector2f& spacing);
  /**
   * @brief get scrollbar size of the group
   */
  float size () const;
  /**
   * @brief get percent of scroller in the scrollbar
   */
  float percent () const;
  /**
   * @brief get current temporary scrollbar size
   */
  float currentSize () const;
private:
  bool mHorizontal;
  float mTempSize = 0.f;
  float mScrollSize = 0.f;
  float mScrollPercent = 0.f;
};

} // namespace sgui
