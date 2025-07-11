#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace sgui
{
template <typename Type>
using LookupTable = std::unordered_map <std::string, Type>;
/**
 * @brief store text accessible with a string key, with option to load and store text
 *   for a given tongue, so it has simple support for localisation.
 */
class TextContainer {
public:
  /**
   * @brief get current tongue used
   */
  const std::string& tongue () const;
  /**
   * @brief set tongue used 
   */
  void setTongue (const std::string& tongue);
  /**
   * @brief reload all texts for the active tongue
   */
  bool reload (const std::string& tongue = "");
  /**
   * @brief load texts from a filename for this given tongue. By default it uses active tongue.
   */
  bool loadFromFile (
         const std::string& filename,
         const std::string& tongue = "");
  /**
   * @brief save texts of the current tongue 
   */
  void saveInFile (const std::string& filename);
  /**
   * @brief add a text with a given key to the container for the current tongue
   */
  void add (
         const std::string& key,
         const std::string& text);
  /**
   * @brief remove text from the container for the current tongue
   */
  void remove (const std::string& key);
  /**
   * @brief get mutable text entry for the current tongue
   */
  std::string& get (const std::string& entry);
  /**
   * @brief get text entry for the current tongue
   */
  const std::string& get (const std::string& entry) const;
  /**
   * @brief check that entry is stored
   */
  bool has (const std::string& entry) const;
private:
  std::string m_activeTongue = "english";
  LookupTable <std::vector <std::string>> m_paths;
  LookupTable <LookupTable <std::string>> m_texts;
};

} // namespace sgui
