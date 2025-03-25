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
  const bool has,
  const bool cli,
  const bool pclosed,
  const bool red)
  : clipped (cli),
    closable (clo),
    closed (pclosed),
    hasMenu (has),
    movable (mov),
    reduced (red),
    visible (vis),
    size (siz),
    position (pos)
{}

/////////////////////////////////////////////////
WidgetOptions::WidgetOptions (
  const std::string& des,
  const Tooltip& inf,
  const sf::Vector2f& dis,
  const float len,
  const bool hor)
  : horizontal (hor),
    length (len),
    displacement (dis),
    description (des),
    info (inf)
{}

} // namespace sgui
