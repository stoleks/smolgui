#pragma once

#include <string>
#include <functional>
#include <SFML/System/Vector2.hpp>

#include "Options.h"

namespace sgui
{

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
  bool clipped    = true;
  bool closable   = false;
  bool closed     = false;
  bool hasMenu    = false;
  bool movable    = true;
  bool reduced    = false;
  bool visible    = true;
  bool scrollable = true;
  sf::Vector2f size = {};
  sf::Vector2f position = {};
  std::string title = "";
};

/////////////////////////////////////////////////
/**
 * @brief small struct to ease use of beginPanel and beginWindow in Gui
 */
struct Window
{
  Window () = default;
  // data
  Panel panel;
  Constraints constraints;
  WidgetOptions options;
};

} // namespace sgui
