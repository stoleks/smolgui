#pragma once

#include <string>
#include <SFML/Graphics/Rect.hpp>

namespace sgui 
{

// forward declarations
using ItemID = std::string;
struct Tooltip;

namespace Impl
{
/**
 * @brief store ID of active/hovered item, hovered item box and tooltip
 */
struct InternalItemState
{
  ItemID activeItem = NullItemID;
  ItemID hoveredItem = NullItemID;
  ItemID keyboardFocus = NullItemID;
  ItemID comboBoxFocus = NullItemID;
  sf::FloatRect hoveredItemBox;
  Tooltip tooltip;
};

/**
 * @brief store input state from a frame to another
 */
struct InputState
{
  bool updated = false; // necessary to avoid double input
  // mouse button state
  bool mouseScrolled = false;
  bool mouseLeftDown = false;
  bool mouseRightDown = false;
  bool mouseMiddleDown = false;
  bool mouseLeftReleased = false;
  bool mouseRightReleased = false;
  bool mouseMiddleReleased = false;
  // mouse position state
  float mouseDeltaWheel = 0.f;
  sf::Vector2f mousePosition = {};
  sf::Vector2f oldMousePosition = {};
  sf::Vector2f mouseDisplacement = {};
  // keyboard state
  bool keyIsPressed = false;
  char32_t keyPressed;
};

} // namespace sgui
} // namespace Impl
