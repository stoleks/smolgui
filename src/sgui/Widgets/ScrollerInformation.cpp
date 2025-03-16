#include "ScrollerInformation.h"

#include "sgui/Core/Interpolation.h"

namespace sgui
{
/////////////////////////////////////////////////
ScrollerInformation::ScrollerInformation (
  const bool horizontal)
  : mHorizontal (horizontal)
{}

/////////////////////////////////////////////////
void ScrollerInformation::newCycle ()
{
  mScrollSize = mTempSize;
  mTempSize = 0.f;
}

/////////////////////////////////////////////////
void ScrollerInformation::setScrollSize (
  const float size)
{
  mTempSize = size;
}

/////////////////////////////////////////////////
void ScrollerInformation::computeScrollSize (
  const sf::Vector2f& spacing)
{
  if (mHorizontal) {
    mTempSize += spacing.x;
  } else {
    mTempSize += spacing.y;
  }
}

/////////////////////////////////////////////////
void ScrollerInformation::scroll (
  const float amount)
{
  mScrollPercent = sgui::clamp (0.f, 1.f, amount);
}

/////////////////////////////////////////////////
float ScrollerInformation::currentSize () const
{
  return mTempSize;
}

/////////////////////////////////////////////////
float ScrollerInformation::size () const
{
  return mScrollSize;
}

/////////////////////////////////////////////////
float ScrollerInformation::percent () const
{
  return mScrollPercent;
}

} // namespace sgui
