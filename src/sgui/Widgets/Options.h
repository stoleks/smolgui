#pragma once

#include <SFML/System/Vector2.hpp>
#include "sgui/Widgets/Constraints.h"

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
  Tooltip (const std::function <void (void)>& def,
    const bool lock = false,
    const bool activ = true)
    : locked (lock), active (activ), display (def) {}
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
  WidgetOptions (const std::string& des,
    const Tooltip& inf = {},
    const sf::Vector2f& disp = {},
    const float len = 4.5f,
    const bool hor = false)
    : horizontal (hor), length (len), displacement (disp), description (des), tooltip (inf) {}
  /**
   * @brief When only a tooltip is needed
   */
  WidgetOptions (const Tooltip& inf) : tooltip (inf) {}
  /**
   * @brief When only the relative displacement is needed
   */
  WidgetOptions (const sf::Vector2f disp) : displacement (disp) {}
  // data
  bool horizontal = false;
  float length = 4.5f;
  sf::Vector2f displacement = {};
  std::string description = "";
  Tooltip tooltip = {};
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
