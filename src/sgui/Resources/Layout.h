/**
  Layout.h
  Purpose: Allows to store an load a gui layout.
  @author A. J.
*/

#pragma once

#include <string>
#include <vector>

#include "sgui/Core/LookupTable.h"
#include "sgui/Widgets/Style.h"
#include "sgui/Widgets/Panel.h"

namespace sgui
{
/////////////////////////////////////////////////
/**
 * Data structure stored in Layout. Its a dumb std::variant basically...
 */
struct LayoutEntry
{
  LayoutEntry () = default;
  // data
  Icon icon;
  Panel panel;
  sf::Vector2f position;
  Constraints constraints;
};


/////////////////////////////////////////////////
/**
 * return special prefix to avoid name collision and to infer type from file
 */
template <typename LayoutData>
constexpr std::string layoutTypeName ()
{
  if constexpr (std::is_same_v <LayoutData, Icon>) {
    return std::string ("Icon::");
  } else if constexpr (std::is_same_v <LayoutData, Panel>) {
    return std::string ("Panel::");
  } else if constexpr (std::is_same_v <LayoutData, sf::Vector2f>) {
    return std::string ("Position::");
  } else {
    return std::string ("Constraints::");
  }
  return "Err::";
}


/////////////////////////////////////////////////
class Layout
{
public:
  /**
   * load a set from a file
   */
  bool loadFromFile (const std::string& set);
  /**
   * save all set in their respecting file
   */
  void saveInFile ();
  /**
   * get entries names in a set
   */
  std::vector<std::string> entries (const std::string& set) const;
  /**
   * add or remove an entry to the layout
   */
  void add (
         const std::string& set,
         const std::string& entry,
         LayoutEntry&& data);
  /**
   * test if entry exist
   */
  template <typename LayoutType>
  bool has (const std::string& entry) const;
  /**
   * get entry data without any sanity check
   */
  template <typename LayoutType>
  LayoutType& get (
         const std::string& entry,
         const bool addLayoutType = true);
  template <typename LayoutType>
  const LayoutType& get (
         const std::string& entry,
         const bool addLayoutType = true) const;
private:
  std::vector <std::string> mSetFilenames;
  LookupTable <LayoutEntry> mEntries;
  LookupTable <std::vector <std::string>> mLayoutEntries;
};

} // namespace sgui

#include "Layout.tpp"
