#pragma once

#include <string>
#include <functional>
#include <SFML/Graphics/Rect.hpp>
#include "sgui/Widgets/Options.h"

namespace sgui
{

/////////////////////////////////////////////////
/**
 * @brief store panel position, size and some parameters
 */
struct Panel
{
  std::string title = "";     ///< title of the panel
  sf::Vector2f position = {}; ///< position of the panel
  sf::Vector2f size = {};     ///< size of the panel
  bool hasHeader     = true;   ///< has it an header ?
  bool hasMenu       = false;  ///< has it a menu ?
  bool isTransparent = false;  ///< does it block click ?
  bool isVisible     = true;   ///< is it rendered ?
  bool isClosable    = false;  ///< is it closable ?
  bool isMovable     = true;   ///< is it movable ?
  bool isScrollable  = true;   ///< is it scrollable ?
  bool isClosed      = false;  ///< is it closed ?
  bool isReduced     = false;  ///< is it reduced (header only) ?
  bool isClipped     = true;   ///< is contents clipped ?
  bool isScrolled    = false;  ///< is it scrolled ?
};

/////////////////////////////////////////////////
/**
 * @brief small struct to ease use of beginPanel and beginWindow in Gui
 */
struct Window
{
  Panel panel = {};             ///< panel of the window
  Constraints constraints = {}; ///< position constraints 
  WidgetOptions options = {};   ///< options of the window
};

} // namespace sgui
