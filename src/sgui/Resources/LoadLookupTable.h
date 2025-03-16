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
template <typename Value,
          typename Enum>
bool loadFromFile (
  LookupTable <Value, Enum>& table,
  const std::string& file);

/////////////////////////////////////////////////
template <typename Value,
          typename Enum>
void saveInFile (
  const LookupTable <Value, Enum>& table,
  const std::string& file);

} // namespace sgui

#include "LoadLookupTable.tpp"
