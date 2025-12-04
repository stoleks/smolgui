#include "ScrollerInformation.h"
#include <algorithm>

namespace sgui
{
namespace Impl
{
/////////////////////////////////////////////////
void ScrollerInformation::update (const sf::Vector2f& groupPosition)
{
  mGroupPosition = groupPosition;
}

/////////////////////////////////////////////////
void ScrollerInformation::computeScrollSize (const sf::Vector2f& cursorPos)
{
  mPositionMax.x = std::max (mPositionMax.x, cursorPos.x);
  mPositionMax.y = std::max (mPositionMax.y, cursorPos.y);
}

/////////////////////////////////////////////////
sf::Vector2f ScrollerInformation::size () const
{
  return mPositionMax - mGroupPosition;
}

} // namespace Impl
} // namespace sgui
