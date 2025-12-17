#pragma once

#include <string>
#include <SFML/Graphics/Rect.hpp>

namespace sgui 
{

struct Tooltip;
const auto NullID = std::string ("Null");

namespace Impl
{

/**
 * @brief store ID of active/hovered item, hovered item box and tooltip
 */
struct InternalItemState
{
  std::string activeItem = NullID;
  std::string hoveredItem = NullID;
  std::string keyboardFocus = NullID;
  std::string comboBoxFocus = NullID;
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

} // namespace Impl
} // namespace sgui
