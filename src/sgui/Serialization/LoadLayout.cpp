#include "LoadLayout.h"

#include <spdlog/spdlog.h>

#include "sgui/Serialization/LoadJson.h"
#include "sgui/Serialization/SerializeGUI.h"
#include "sgui/Serialization/SerializeSFML.h"

namespace sgui
{
/////////////////////////////////////////////////
bool loadFromFile (
  Layout& layout,
  const std::string& file)
{
  json allEntries = loadFromFile (file);

  // save json data into layout
  for (auto& entry : allEntries.items ()) {
    const auto key = entry.key ();
    const auto end = std::string::npos;
    // test if entry is a constraint
    if (key.find (layoutTypeName <Constraints> ()) != end) {
      auto data = LayoutEntry ();
      data.constraints = entry.value ().get <Constraints> ();
      layout.add (file, key, std::move (data));
    }
    // a position
    else if (key.find (layoutTypeName <sf::Vector2f> ()) != end) {
      auto data = LayoutEntry ();
      data.position = entry.value ().get <sf::Vector2f> ();
      layout.add (file, key, std::move (data));
    }
    // a panel
    else if (key.find (layoutTypeName <Panel> ()) != end) {
      auto data = LayoutEntry ();
      data.panel = entry.value ().get <Panel> ();
      layout.add (file, key, std::move (data));
    }
    // or an icon
    else if (key.find (layoutTypeName <Icon> ()) != end) {
      auto data = LayoutEntry ();
      data.icon = entry.value ().get <Icon> ();
      layout.add (file, key, std::move (data));
    }
    // or a window
    else if (key.find (layoutTypeName <Window> ()) != end) {
      auto data = LayoutEntry ();
      data.window = entry.value ().get <Window> ();
      layout.add (file, key, std::move (data));
    }
    // inform user about invalid format
    else {
      spdlog::warn ("loadFromFile (Layout&): {} is not a valid gui layout entry in {}", key, file);
      return false;
    }
  }
  return true;
}

/////////////////////////////////////////////////
void saveInFile (
  const Layout& layout,
  const std::string& file)
{
  json out;

  // save data into json
  for (const auto& entry : layout.entries (file)) {
    const auto end = std::string::npos;
    // constraint
    if (entry.find (layoutTypeName <Constraints> ()) != end) {
      out [entry] = layout.get <Constraints> (entry, false);
    }
    // position
    else if (entry.find (layoutTypeName <sf::Vector2f> ()) != end) {
      out [entry] = layout.get <sf::Vector2f> (entry, false);
    }
    // panel
    else if (entry.find (layoutTypeName <Panel> ()) != end) {
      out [entry] = layout.get <Panel> (entry, false);
    }
    // icon
    else if (entry.find (layoutTypeName <Icon> ()) != end) {
      out [entry] = layout.get <Icon> (entry, false);
    }
    // window
    else if (entry.find (layoutTypeName <Window> ()) != end) {
      out [entry] = layout.get <Window> (entry, false);
    }
  }

  // write json
  saveInFile (out, file);
}

} // namespace sgui
