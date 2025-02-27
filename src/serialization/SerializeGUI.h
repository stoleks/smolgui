/**
  SerializeGUI.h
  Purpose: Serialize/deserialize gui objects
  @author: A. J.
*/

#pragma once

#include "nlohmann/json.hpp"

// for readability
using json = nlohmann::json;

namespace sgui
{
// forward declaration
struct Icon;
struct Style;
struct Panel;
struct FontSize;
struct Constraint;
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
 * convert Constraint to/from json
 */
void to_json (json& j, const Constraint& constraint);
void from_json (const json& j, Constraint& constraint);

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

} // namespace sgui
