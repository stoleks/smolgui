#include "LoadTextureAtlas.h"

#include "TextureAtlas.h"
#include "sgui/Serialization/LoadJson.h"
#include "sgui/Serialization/SerializeGUI.h"
#include "sgui/Serialization/SerializeSFML.h"

namespace sgui
{
/////////////////////////////////////////////////
// TextureAtlas load/save
/////////////////////////////////////////////////
bool loadFromFile (
  TextureAtlas& atlas,
  const std::string& file)
{
  // parse file into json
  json allEntries = sgui::loadFromFile (file);

  // save json data into atlas
  for (auto& entry : allEntries.items ()) {
    const auto key = entry.key ();
    auto frames = entry.value ().get <Frames> ();
    atlas.add (key, std::move (frames.texture), frames.count);
  }
  return true;
}

/////////////////////////////////////////////////
void saveInFile (
  const TextureAtlas& atlas,
  const std::string& file)
{
  json out;
  // save data into json
  for (const auto& entry : atlas) {
    out [entry.first] = entry.second;
  }
  // write json
  saveInFile (out, file);
}


/////////////////////////////////////////////////
void to_json (json& j, const Frames& frames) {
  j = json {
    ToJson (frames, count),
    ToJson (frames, texture)
  };
}

/////////////////////////////////////////////////
void from_json (const json& j, Frames& frames) {
  FromJson (frames, count);
  FromJson (frames, texture);
}

} // namespace sgui
