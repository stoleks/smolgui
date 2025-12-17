#pragma once

#include <string>
#include <functional>
#include <SFML/System/Vector2.hpp>

#include "sgui/Widgets/Options.h"

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
         const sf::Vector2f& s,
         const sf::Vector2f& pos = {},
         const bool vis = true,
         const bool header = true)
    : visible (vis), hasHeader (header), position (pos), size (s) {}
  // data
  bool clipped    = true;
  bool closable   = false;
  bool closed     = false;
  bool visible    = true;
  bool hasHeader  = true;
  bool hasMenu    = false;
  bool movable    = true;
  bool reduced    = false;
  bool scrollable = true;
  bool isScrolled = false;
  sf::Vector2f position = {};
  sf::Vector2f size = {};
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
  Panel panel = {};
  Constraints constraints = {};
  WidgetOptions options = {};
};

} // namespace sgui
