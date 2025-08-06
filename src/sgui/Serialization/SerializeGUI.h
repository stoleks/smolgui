#pragma once

#include <nlohmann/json.hpp>
#include "sgui/Widgets/Panel.h"
#include "sgui/Resources/Layout.h"
#include "sgui/Resources/TextureAtlas.h"
#include "SerializeSFML.h"

// for readability
using json = nlohmann::json;

namespace sgui
{
/**
 * Fontsize
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (FontSize, normal, title, subtitle, footnote);
/**
 * Style
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Style, fontSize, fontColor, itemSpacing, itemInnerSpacing);
/**
 * Panel
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Panel, movable, visible, closable, size, position, title);
/**
 * Constraints
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Constraints, vertical, horizontal, relativePosition);
/**
 * Window
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Window, panel, constraints);
/**
 * Icon
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Icon, name, position);
/**
 * LayoutEntry
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (LayoutEntry, icon, panel, position, constraints, window);

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
