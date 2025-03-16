/**
  ScrollerInformation.h
  Purpose: store scroller internal information
  @author: A. J.
*/

#pragma once

#include <SFML/System/Vector2.hpp>

namespace sgui
{
/**
 * class to manage group scrolling
 */
class ScrollerInformation
{
public:
  ScrollerInformation (const bool horizontal);
  /**
   * start a new computation cycle
   */
  void newCycle ();
  /**
   * increase scroll area size or scroll through it
   */
  void scroll (const float amount);
  void setScrollSize (const float size);
  void computeScrollSize (const sf::Vector2f& spacing);
  /**
   * get scrolling size and percent
   */
  float size () const;
  float percent () const;
  /**
   * to get current temporary scroll size
   */
  float currentSize () const;
private:
  bool mHorizontal;
  float mTempSize = 0.f;
  float mScrollSize = 0.f;
  float mScrollPercent = 0.f;
};

} // namespace sgui
