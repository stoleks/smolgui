/**
  LoadLookupTable.h
  Purpose : Implement basic routines to save and load lookup table.
  @author A. J.
*/

#pragma once

#include "sgui/Serialization/LoadJson.h"
#include "sgui/Serialization/SerializeCore.h"

namespace sgui
{
/**
 * save/load look-up table from file
 */
/////////////////////////////////////////////////
template <typename Key,
          typename Type>
bool loadFromFile (
  std::unordered_map <Key, Type>& table,
  const std::string& file)
{
  // parse from json
  json j = sgui::loadFromFile (file);
  std::unordered_map <Key, Type> tableData = j;
  for (const auto& data : tableData) {
    table.insert ({data.first, data.second});
  }
  return true;
}

/////////////////////////////////////////////////
template <typename Key,
          typename Type>
void saveInFile (
  const std::unordered_map <Key, Type>& table,
  const std::string& file)
{
  // parse in json
  json out = table;

  // write in file
  auto output = std::ofstream (file);
  output << std::setw (2) << out << std::endl;
  output.close ();
}

} // namespace sgui
