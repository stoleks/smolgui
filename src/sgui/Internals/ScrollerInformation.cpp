#include "ScrollerInformation.h"

#include "spdlog/spdlog.h"

#include "sgui/Core/Interpolation.h"

namespace sgui
{
namespace Impl
{
/////////////////////////////////////////////////
void ScrollerInformation::update (const sf::FloatRect& groupBox)
{
  mGroupBox = groupBox;
}

/////////////////////////////////////////////////
void ScrollerInformation::computeScrollSize (const sf::Vector2f& cursorPos)
{
  mMaxCursorPosition.x = std::max (mMaxCursorPosition.x, cursorPos.x);
  mMaxCursorPosition.y = std::max (mMaxCursorPosition.y, cursorPos.y);
}

/////////////////////////////////////////////////
sf::Vector2f ScrollerInformation::size () const
{
  return mMaxCursorPosition - mGroupBox.position;
}

} // namespace Impl
} // namespace sgui
