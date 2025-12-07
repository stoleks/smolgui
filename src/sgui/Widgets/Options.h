#pragma once

#include <SFML/System/Vector2.hpp>
#include "Constraints.h"

namespace sgui
{

/////////////////////////////////////////////////
/**
 * @brief Defines number of slices for stretchable widgets
 */
enum class Slices {
  One,
  Three,
  Nine
};

/**
 * @brief Defines type of tiling for center part of widgets
 */
enum class TileMode {
  Stretch, // extend central part
  Hide,    // don't draw central part
  Repeat,  // repeat central part
  Rotate   // repeat and rotate central part
};

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
 * @brief define identifier for widgets
 */
using ItemID = std::string;
const auto NullItemID = ItemID ("Null");

/////////////////////////////////////////////////
/**
 * @brief store tooltip in a function with its parent widget ID
 */
struct Tooltip
{
  Tooltip () = default;
  Tooltip (
         const std::function <void (void)>& def,
         const bool locked = false,
         const bool active = true);
  // data
  ItemID parent;
  bool locked = false;
  bool active = false;
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
  WidgetOptions (
        const std::string& description,
        const Tooltip& info = {},
        const sf::Vector2f& displacement = {},
        const float length = 4.5f,
        const bool horizontal = false);
  /**
   * @brief When only a tooltip is needed
   */
  WidgetOptions (const Tooltip& inf) : info (inf) {}
  /**
   * @brief When only the relative displacement is needed
   */
  WidgetOptions (const sf::Vector2f disp) : displacement (disp) {}
  // data
  bool horizontal = false;
  float length = 4.5f;
  sf::Vector2f displacement = {};
  std::string description = "";
  Tooltip info = {};
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
