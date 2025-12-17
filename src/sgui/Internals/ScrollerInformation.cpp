#include "sgui/Internals/ScrollerInformation.h"
#include <algorithm>

namespace sgui::Impl
{

/////////////////////////////////////////////////
void ScrollerInformation::update (const sf::Vector2f& groupPosition)
{
  mGroupPosition = groupPosition;
  mCurrentPositionMax = mGroupPosition;
}

/////////////////////////////////////////////////
void ScrollerInformation::computeScrollSize (const sf::Vector2f& cursorPos)
{
  mCurrentPositionMax.x = std::max (mCurrentPositionMax.x, cursorPos.x);
  mCurrentPositionMax.y = std::max (mCurrentPositionMax.y, cursorPos.y);
}

/////////////////////////////////////////////////
sf::Vector2f ScrollerInformation::size () const
{
  return mPositionMax - mGroupPosition;
}

/////////////////////////////////////////////////
void ScrollerInformation::endLoop ()
{
  mPositionMax = mCurrentPositionMax;
}

} // namespace sgui::Impl
