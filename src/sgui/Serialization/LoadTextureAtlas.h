#pragma once

#include <nlohmann/json.hpp>
#include "sgui/Resources/TextureAtlas.h"
#include "SerializeSFML.h"

// for readability
using json = nlohmann::json;

namespace sgui
{
/**
 * Convert Frames to/from json
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Frames, count, texture);

/**
 * save/load sprite Atlas
 */
bool loadFromFile (
  TextureAtlas& atlas,
  const std::string& file);

void saveInFile (
  const TextureAtlas& atlas,
  const std::string& file);

} // namespace sgui
