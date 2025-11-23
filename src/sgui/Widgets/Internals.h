#pragma once

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

/**
 * @brief store window and panel internal data
 */
struct GroupData
{
  bool isActive = false;
  bool horizontal = false;
  bool hasMenuBar = false;
  uint32_t groupId = 0u;
  uint32_t clippingLayer = 0u;
  uint32_t menuItemCount = 0u;
  sf::Vector2f lastItemPosition = {};
  sf::Vector2f menuBarPosition = {};
  sf::Vector2f menuBarSize = {};
  sf::Vector2f innerPosition = {};
  sf::Vector2f position = {};
  sf::Vector2f size = {};
};

} // namespace sgui
} // namespace Impl
