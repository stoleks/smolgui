#pragma once

#include <nlohmann/json.hpp>
#include "sgui/Resources/Layout.h"
#include "sgui/Resources/TextureAtlas.h"
#include "sgui/Serialization/SerializeSFML.h"

namespace sgui
{
/**
 * Fontsize
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (FontSize, normal, title, subtitle, footnote);
/**
 * Style
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Style, fontSize, fontColor, itemSpacing);
/**
 * Panel
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Panel, movable, visible, closable, hasHeader, size, position, title);
/**
 * Constraints
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Constraints, vertical, horizontal, relativePosition);
/**
 * Window
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (Window, panel, constraints);
/**
 * LayoutEntry
 */
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT (LayoutEntry, panel, position, constraints, window);

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
