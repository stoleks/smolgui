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
         const bool closed = false,
         const bool reduced = false);
  // data
  bool clipped = false;
  bool closable = false;
  bool closed   = false;
  bool movable  = true;
  bool reduced  = false;
  bool visible  = true;
  sf::Vector2f size;
  sf::Vector2f position;
};

/////////////////////////////////////////////////
/**
 * @brief store constraints on position for gui panel
 */
struct Constraint
{
  Constraint () = default;
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
  WidgetOptions (
        const std::string& description,
        const Tooltip& info = {},
        const bool horizontal = false,
        const sf::Vector2f& position = {});
  // data
  bool horizontal = false;
  std::string description = "";
  sf::Vector2f position = {};
  Tooltip info = {};
};

} // namespace sgui
