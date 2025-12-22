#pragma once

#include <string>
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

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
  Panel (const sf::FloatRect& box, const bool vis = true, const bool header = true)
    : visible (vis), hasHeader (header), position (box.position), size (box.size) {}
  // data
  bool clipped    = true;     ///< is contents clipped ?
  bool closable   = false;    ///< is it closable ?
  bool closed     = false;    ///< is it closed ?
  bool visible    = true;     ///< is it rendered ?
  bool hasHeader  = true;     ///< has it an header ?
  bool hasMenu    = false;    ///< has it a menu ?
  bool movable    = true;     ///< is it movable ?
  bool reduced    = false;    ///< is it reduced (header only) ?
  bool scrollable = true;     ///< is it scrollable ?
  bool isScrolled = false;    ///< is it scrolled ?
  sf::Vector2f position = {}; ///< position of the panel
  sf::Vector2f size = {};     ///< size of the panel
  std::string title = "";     ///< title of the panel
};

/////////////////////////////////////////////////
/**
 * @brief small struct to ease use of beginPanel and beginWindow in Gui
 */
struct Window
{
  Window () = default;
  // data
  Panel panel = {};             ///< panel of the window
  Constraints constraints = {}; ///< position constraints 
  WidgetOptions options = {};   ///< options of the window
};

} // namespace sgui
