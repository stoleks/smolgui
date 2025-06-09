#include "SerializeGUI.h"

#include "SerializeSFML.h"
#include "sgui/Resources/Layout.h"
#include "sgui/Widgets/Style.h"
#include "sgui/Widgets/Panel.h"

namespace sgui 
{
////////////////////////////////////////////////////////////
/**
 * convert FontSize to/from json
 */
void to_json (json& j, const FontSize& size) {
  j = json {
    ToJson (size, normal),
    ToJson (size, title),
    ToJson (size, subtitle),
    ToJson (size, footnote)
  };
}

void from_json (const json& j, FontSize& size) {
  FromJson (size, normal);
  FromJson (size, title);
  FromJson (size, subtitle);
  FromJson (size, footnote);
}

////////////////////////////////////////////////////////////
/**
 * convert Style to/from json
 */
void to_json (json& j, const Style& style) {
  j = json {
    ToJson (style, fontSize),
    ToJson (style, fontColor),
    ToJson (style, itemSpacing),
    ToJson (style, itemInnerSpacing)
  };
}

void from_json (const json& j, Style& style) {
  FromJson (style, fontSize);
  FromJson (style, fontColor);
  FromJson (style, itemSpacing);
  FromJson (style, itemInnerSpacing);
}

////////////////////////////////////////////////////////////
/**
 * convert Panel to/from json
 */
void to_json (json& j, const Panel& panel) {
  j = json {
    ToJson (panel, movable),
    ToJson (panel, visible),
    ToJson (panel, closable),
    ToJson (panel, size),
    ToJson (panel, position)
  };
}

void from_json (const json& j, Panel& panel) {
  FromJson (panel, movable);
  FromJson (panel, visible);
  FromJson (panel, closable);
  FromJson (panel, size);
  FromJson (panel, position);
  FromJson (panel, title);
}

////////////////////////////////////////////////////////////
/**
 * convert Constraints to/from json
 */
void to_json (json& j, const Constraints& constraint) {
  j = json {
    ToJson (constraint, centeredVerticaly),
    ToJson (constraint, centeredHorizontaly),
    ToJson (constraint, pixelsFromTop),
    ToJson (constraint, pixelsFromBottom),
    ToJson (constraint, pixelsFromLeft),
    ToJson (constraint, pixelsFromRight)
  };
}

void from_json (const json& j, Constraints& constraint) {
  FromJson (constraint, centeredVerticaly);
  FromJson (constraint, centeredHorizontaly);
  FromJson (constraint, pixelsFromTop);
  FromJson (constraint, pixelsFromBottom);
  FromJson (constraint, pixelsFromLeft);
  FromJson (constraint, pixelsFromRight);
}

////////////////////////////////////////////////////////////
/**
 * convert Window to/from json
 */
void to_json (json& j, const Window& window) {
  j = json {
    ToJson (window, panel),
    ToJson (window, constraints)
  };
}

void from_json (const json& j, Window& window) {
  FromJson (window, panel);
  FromJson (window, constraints);
}

////////////////////////////////////////////////////////////
/**
 * convert Icon to/from json
 */
void to_json (json& j, const Icon& icon) {
  j = json {
    ToJson (icon, name),
    ToJson (icon, position)
  };
}

void from_json (const json& j, Icon& icon) {
  FromJson (icon, name);
  FromJson (icon, position);
}

////////////////////////////////////////////////////////////
/**
 * convert LayoutEntry to/from json
 */
void to_json (json& j, const LayoutEntry& entry) {
  j = json {
    ToJson (entry, icon),
    ToJson (entry, panel),
    ToJson (entry, position),
    ToJson (entry, constraints),
    ToJson (entry, window)
  };
}

void from_json (const json& j, LayoutEntry& entry) {
  FromJson (entry, icon);
  FromJson (entry, panel);
  FromJson (entry, position);
  FromJson (entry, constraints);
  FromJson (entry, window);
}

} // namespace sgui
