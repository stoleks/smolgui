#pragma once

#include <string>
#include <functional>
#include <SFML/System/Vector2.hpp>

namespace sgui
{
/**
 * @brief define identifier for widgets
 */
using ItemID = std::string;
const auto NullItemID = ItemID ("Null");

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
 * @brief store panel posiiton, size and some parameters
 */
struct Panel
{
  Panel () = default;
  Panel (
         const sf::Vector2f& position,
         const sf::Vector2f& size,
         const bool movable = true,
         const bool visible = true,
         const bool closable = false,
         const bool hasMenu = false,
         const bool clipped = true,
         const bool closed = false,
         const bool reduced = false);
  // data
  bool clipped  = true;
  bool closable = false;
  bool closed   = false;
  bool hasMenu  = false;
  bool movable  = true;
  bool reduced  = false;
  bool visible  = true;
  sf::Vector2f size = {};
  sf::Vector2f position = {};
  std::string title = "";
};

/////////////////////////////////////////////////
/**
 * @brief store constraints on position for gui panel
 */
struct Constraints
{
  Constraints () = default;
  // data, zero means no constrain
  bool centeredHorizontaly = false;
  bool centeredVerticaly   = false;
  uint32_t pixelsFromBottom = 0u;
  uint32_t pixelsFromLeft   = 0u;
  uint32_t pixelsFromRight  = 0u;
  uint32_t pixelsFromTop    = 0u;
};

/////////////////////////////////////////////////
/**
 * @brief store widget options : tooltip, position, description
 */
struct WidgetOptions
{
  WidgetOptions () = default;
  /**
   * @brief full constructor, but with options if only description is required
   */
  WidgetOptions (
        const std::string& description,
        const Tooltip& info = {},
        const sf::Vector2f& displacement = {},
        const float length = 4.5f,
        const bool horizontal = false);
  /**
   * @brief when only a tooltip is needed
   */
  WidgetOptions (const Tooltip& inf) : info (inf) {}
  /**
   * @brief when only the relative displacement is needed
   */
  WidgetOptions (const sf::Vector2f disp) : displacement (disp) {}
  // data
  bool horizontal = false;
  float length = 4.5f;
  sf::Vector2f displacement = {};
  std::string description = "";
  Tooltip info = {};
};

} // namespace sgui
