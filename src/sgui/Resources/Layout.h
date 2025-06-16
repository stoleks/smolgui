#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "sgui/Widgets/Style.h"
#include "sgui/Widgets/Panel.h"

namespace sgui
{
/////////////////////////////////////////////////
/**
 * @brief Data structure stored in Layout. Its a dumb std::variant basically...
 */
struct LayoutEntry
{
  LayoutEntry () = default;
  // data
  Icon icon;
  Panel panel;
  sf::Vector2f position;
  Constraints constraints;
  Window window;
};


/////////////////////////////////////////////////
/**
 * @brief return special prefix to avoid name collision and to infer type from file
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
  } else if constexpr (std::is_same_v <LayoutData, Window>) {
    return std::string ("Window::");
  } else {
    return std::string ("Constraints::");
  }
  return "Err::";
}


/////////////////////////////////////////////////
/**
 * @brief allow to store and load Gui layou
 */
class Layout
{
public:
  /**
   * @brief load layouts from file
   */
  bool loadFromFile (const std::string& set);
  /**
   * @brief save all layouts in their respecting file
   */
  void saveInFile ();
  /**
   * @brief get entries names in a layout
   */
  std::vector<std::string> entries (const std::string& set) const;
  /**
   * @brief add or remove an entry to the layout
   */
  void add (
         const std::string& set,
         const std::string& entry,
         LayoutEntry&& data);
  /**
   * @brief test if entry exist
   */
  template <typename LayoutType>
  bool has (const std::string& entry) const;
  /**
   * @brief get entry data without any sanity check
   */
  template <typename LayoutType>
  LayoutType& get (
         const std::string& entry,
         bool addLayoutType = true);
  /**
   * @brief get mutable entry data without any sanity check
   */
  template <typename LayoutType>
  const LayoutType& get (
         const std::string& entry,
         bool addLayoutType = true) const;
private:
  std::vector <std::string> m_setFilenames;
  std::unordered_map <std::string, LayoutEntry> m_entries;
  std::unordered_map <std::string, std::vector <std::string>> m_layoutEntries;
};

} // namespace sgui

#include "Layout.tpp"
