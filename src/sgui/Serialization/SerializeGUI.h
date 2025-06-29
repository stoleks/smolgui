/**
  SerializeGUI.h
  Purpose: Serialize/deserialize gui objects
  @author: A. J.
*/

#pragma once

#include <nlohmann/json.hpp>

#include "sgui/Widgets/Options.h"

// for readability
using json = nlohmann::json;

namespace sgui
{
// forward declaration
struct Icon;
struct Style;
struct Panel;
struct Window;
struct FontSize;
struct LayoutEntry;

/**
 * convert FontSize to/from json
 */
void to_json (json& j, const FontSize& size);
void from_json (const json& j, FontSize& size);

/**
 * convert Style to/from json
 */
void to_json (json& j, const Style& style);
void from_json (const json& j, Style& style);

/**
 * convert Panel to/from json
 */
void to_json (json& j, const Panel& panel);
void from_json (const json& j, Panel& panel);

/**
 * convert Constraints to/from json
 */
void to_json (json& j, const Constraints& constraint);
void from_json (const json& j, Constraints& constraint);

/**
 * convert Window to/from json
 */
void to_json (json& j, const Window& window);
void from_json (const json& j, Window& window);

/**
 * convert Icon to/from json
 */
void to_json (json& j, const Icon& icon);
void from_json (const json& j, Icon& icon);

/**
 * convert LayoutEntry to/from json
 */
void to_json (json& j, const LayoutEntry& entry);
void from_json (const json& j, LayoutEntry& entry);

/**
 * convert VerticalAlignment to/from json
 */
NLOHMANN_JSON_SERIALIZE_ENUM (VerticalAlignment, {
  {VerticalAlignment::None,   "VerticalAlignment::None"},
  {VerticalAlignment::Top,    "VerticalAlignment::Top"},
  {VerticalAlignment::Bottom, "VerticalAlignment::Bottom"},
  {VerticalAlignment::Center, "VerticalAlignment::Center"},
})

/**
 * convert HOrizontalAlignment to/from json
 */
NLOHMANN_JSON_SERIALIZE_ENUM (HorizontalAlignment, {
  {HorizontalAlignment::None,   "HorizontalAlignment::None"},
  {HorizontalAlignment::Left,   "HorizontalAlignment::Left"},
  {HorizontalAlignment::Right,  "HorizontalAlignment::Right"},
  {HorizontalAlignment::Center, "HorizontalAlignment::Center"},
})

} // namespace sgui
