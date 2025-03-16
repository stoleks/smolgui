/**
  LoadLayout.h
  Purpose: save and load gui layout from json.
  @author A. J.
*/

#pragma once

#include <string>

namespace sgui 
{
// forward declaration
class Layout;

/**
 * save/load GuiLayout in json
 */
bool loadFromFile (
  Layout& layout,
  const std::string& file);

void saveInFile (
  const Layout& layout,
  const std::string& file);

} // namespace sgui
