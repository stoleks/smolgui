/**
  UserInterface.h
  Purpose: wrapper for gui, to ease use of certain functions.
  @author A. J.
*/

#pragma once

#include "gui.h"
#include "resources/Layout.h"

namespace sgui 
{
/**
 * to ease use of external gui functions
 */
struct UserInterface {
  UserInterface (const Gui& gui, const Layout& layout, TextContainer& texts);
  // data
  Gui gui;
  Layout layout;
  TextContainer& texts;
};


/**
 * to offer a simpler interface when using gui layout and text container for
 * begin window/panel and end window/panel
 */
// Window
bool beginWindow (
  UserInterface& ui,
  const std::string& element,
  const bool hasMenu = false,
  const bool horizontal = false,
  const Tooltip& info = Tooltip ());
void endWindow (UserInterface& ui);
// Panel
void beginPanel (
  UserInterface& ui,
  const std::string& element,
  const bool clipped = false,
  const bool horizontal = false,
  const Tooltip& info = Tooltip ());
void endPanel (UserInterface& ui);


/**
 * Same for drop list. These create automatically a panel to hold the drop list.
 */
void dropList (
  UserInterface& ui,
  uint32_t& selected,
  const std::vector<std::string>& list,
  const std::string& element,
  const uint32_t phantomElements = 0);

} // namespace sgui
