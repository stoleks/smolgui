#include "Panel.h"

namespace sgui
{
/////////////////////////////////////////////////
Tooltip::Tooltip (
  const std::function <void (void)>& def,
  const bool lock,
  const bool act)
  : locked (lock),
    active (act),
    display (def)
{}

/////////////////////////////////////////////////
Panel::Panel (
  const sf::Vector2f& pos,
  const sf::Vector2f& siz,
  const bool mov,
  const bool vis,
  const bool clo,
  const bool pclosed,
  const bool red)
  : movable (mov),
    visible (vis),
    closable (clo),
    closed (pclosed),
    reduced (red),
    size (siz),
    position (pos)
{}

/////////////////////////////////////////////////
WidgetOptions::WidgetOptions (
  const std::string& des,
  const Tooltip& inf,
  const bool hor,
  const sf::Vector2f& pos)
  : description (des),
    position (pos),
    horizontal (hor),
    info (inf)
{}

} // namespace sgui
