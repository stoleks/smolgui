#pragma once

#include "sgui/Widgets/Constraints.h"
#include "sgui/Widgets/Aspect.h"

namespace sgui
{

/////////////////////////////////////////////////
/**
 * @brief Text type
 */
enum class TextType {
  Footnote,
  Subtitle,
  Normal,
  Title
};

/////////////////////////////////////////////////
/**
 * @brief store tooltip in a function with its parent widget ID
 */
struct Tooltip
{
  bool locked = false;                      ///< is tooltip locked ?
  bool active = false;                      ///< is tooltip active ?
  std::string parent = "";                  ///< widget parent identifier
  std::function <void (void)> display = {}; ///< function called to display tooltip
};

/////////////////////////////////////////////////
/**
 * @brief Store widget options : tooltip, position, description
 */
struct WidgetOptions
{
  bool horizontal = false;           ///< Is widget horizontal or vertical ?
  sf::Vector2f size = { 6.f, 1.f };  ///< For slider and progress bar
  sf::Vector2f displacement = {};    ///< To override automatic position
  std::string description = "";      ///< Widget description
  Tooltip tooltip = {};              ///< Tooltip specification
  WidgetAspect aspect = {};          ///< To control widget aspect
};

/**
 * @brief Store text options : boxSize, centered, type, etc.
 */
struct TextOptions
{
  VerticalAlignment vertical = VerticalAlignment::None;       ///< vertical alignment
  HorizontalAlignment horizontal = HorizontalAlignment::None; ///< horizontal alignment
  TextType type = TextType::Normal;                           ///< type of text
  sf::Vector2f boxSize = {};                                  ///< box that contains text, no box = no limit
};

} // namespace sgui
