#pragma once

#include <SFML/System/Vector2.hpp>
#include "sgui/Widgets/Constraints.h"
#include "sgui/Widgets/Widgets.h"
#include "sgui/Widgets/ItemStates.h"

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
  Tooltip () = default;
  Tooltip (const std::function <void (void)>& d, const bool l = false, const bool a = true)
    : locked (l), active (a), display (d) {}
  // data
  bool locked = false;
  bool active = false;
  std::string parent = "";
  std::function <void (void)> display;
};

/////////////////////////////////////////////////
/**
 * @brief Store widget options : tooltip, position, description
 */
struct WidgetOptions
{
  WidgetOptions () = default;
  /**
   * @brief Full constructor, but with options if only description is required
   */
  WidgetOptions (const std::string& des, const Tooltip& t = {}, const sf::Vector2f& d = {})
    : displacement (d), description (des), tooltip (t) {}
  /**
   * @brief When only a tooltip is needed
   */
  WidgetOptions (const Tooltip& inf) : tooltip (inf) {}
  /**
   * @brief When only the relative displacement is needed
   */
  WidgetOptions (const sf::Vector2f disp) : displacement (disp) {}
  /**
   * @brief To control panel appearance
   */
  WidgetOptions (const Widget w, const Slices s = Slices::One, const ItemState is = ItemState::None)
    : widget (w), slices (s), state (is) {}
  // data
  bool horizontal = false;           ///< Is widget horizontal or vertical ?
  sf::Vector2f size = { 6.f, 1.f };  ///< For slider and progress bar
  sf::Vector2f displacement = {};    ///< To override automatic position
  std::string description = "";      ///< Widget description
  Tooltip tooltip = {};              ///< Tooltip
  Widget widget = Widget::None;      ///< Widget type
  Slices slices = Slices::Default;   ///< Change default widget slicing
  ItemState state = ItemState::None; ///< To force a specific state for a widget
};

/**
 * @brief Store text options : boxSize, centered, type, etc.
 */
struct TextOptions
{
  TextOptions () = default;
  TextOptions (const VerticalAlignment alignment) : vertical (alignment) {}
  TextOptions (const HorizontalAlignment alignment) : horizontal (alignment) {}
  TextOptions (const sf::Vector2f& size) : boxSize (size) {}
  // data
  VerticalAlignment vertical = VerticalAlignment::None;
  HorizontalAlignment horizontal = HorizontalAlignment::None;
  TextType type = TextType::Normal;
  sf::Vector2f boxSize = {};
};

} // namespace sgui
