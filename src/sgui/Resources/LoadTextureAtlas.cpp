#include "LoadTextureAtlas.h"

#include "TextureAtlas.h"
#include "LoadLookupTable.h"
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
  // define simple equality lambda to test entries
  auto equal = [] (const std::string& a, const std::string& b) {
    return a == b;
  };

  // parse file into json
  auto allEntries = loadFromFile (file);

  // save json data into atlas
  for (auto& entry : allEntries.items ()) {
    const auto key = entry.key ();
    // get atlas dimension
    if (key == "TextureDimension") {
      atlas.setTextureDimension (entry.value ().get <sf::Vector2u> ());
    // get every atlas entries
    } else {
      auto frames = entry.value ().get <Frames> ();
      atlas.add (key, std::move (frames.texture), frames.count);
    }
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
  out ["TextureDimension"] = atlas.textureDimension ();
  for (const auto& entry : atlas) {
    out [entry.first] = entry.second;
  }
  // write json
  saveInFile (out, file);
}


/////////////////////////////////////////////////
void to_json (json& j, const Frames& frames) {
  j = json {
    {"count",   frames.count},
    {"texture", frames.texture}
  };
}

/////////////////////////////////////////////////
void from_json (const json& j, Frames& frames) {
  j.at ("count")  .get_to (frames.count);
  j.at ("texture").get_to (frames.texture);
}

} // namespace sgui
