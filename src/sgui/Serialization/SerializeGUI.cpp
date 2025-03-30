#include "SerializeGUI.h"

#include "SerializeSFML.h"
#include "sgui/Resources/Layout.h"
#include "sgui/Widgets/Style.h"
#include "sgui/Widgets/Panel.h"

namespace sgui 
{
/**
 * convert FontSize to/from json
 */
void to_json (json& j, const FontSize& size) {
  j = json {
    {"normal",   size.normal},
    {"title",    size.title},
    {"subtitle", size.subtitle},
    {"footnote", size.footnote}
  };
}

void from_json (const json& j, FontSize& size) {
  j.at ("normal")  .get_to (size.normal);
  j.at ("title")   .get_to (size.title);
  j.at ("subtitle").get_to (size.subtitle);
  j.at ("footnote").get_to (size.footnote);
}


/**
 * convert Style to/from json
 */
void to_json (json& j, const Style& style) {
  j = json {
    {"fontSize",         style.fontSize},
    {"fontColor",        style.fontColor},
    {"itemSpacing",      style.itemSpacing},
    {"itemInnerSpacing", style.itemInnerSpacing}
  };
}

void from_json (const json& j, Style& style) {
  j.at ("fontSize")        .get_to (style.fontSize);
  j.at ("fontColor")       .get_to (style.fontColor);
  j.at ("itemSpacing")     .get_to (style.itemSpacing);
  j.at ("itemInnerSpacing").get_to (style.itemInnerSpacing);
}


/**
 * convert Panel to/from json
 */
void to_json (json& j, const Panel& panel) {
  j = json {
    {"movable",  panel.movable},
    {"visible",  panel.visible},
    {"closable", panel.closable},
    {"size",     panel.size},
    {"position", panel.position}
  };
}

void from_json (const json& j, Panel& panel) {
  j.at ("movable") .get_to (panel.movable);
  j.at ("visible") .get_to (panel.visible);
  j.at ("closable").get_to (panel.closable);
  j.at ("size")    .get_to (panel.size);
  j.at ("position").get_to (panel.position);
  j.at ("title")   .get_to (panel.title);
}


/**
 * convert Constraints to/from json
 */
void to_json (json& j, const Constraints& constraint) {
  j = json {
    {"centeredVerticaly",   constraint.centeredVerticaly},
    {"centeredHorizontaly", constraint.centeredHorizontaly},
    {"pixelsFromTop",       constraint.pixelsFromTop},
    {"pixelsFromBottom",    constraint.pixelsFromBottom},
    {"pixelsFromLeft",      constraint.pixelsFromLeft},
    {"pixelsFromRight",     constraint.pixelsFromRight}
  };
}

void from_json (const json& j, Constraints& constraint) {
  j.at ("centeredVerticaly")  .get_to (constraint.centeredVerticaly);
  j.at ("centeredHorizontaly").get_to (constraint.centeredHorizontaly);
  j.at ("pixelsFromTop")      .get_to (constraint.pixelsFromTop);
  j.at ("pixelsFromBottom")   .get_to (constraint.pixelsFromBottom);
  j.at ("pixelsFromLeft")     .get_to (constraint.pixelsFromLeft);
  j.at ("pixelsFromRight")    .get_to (constraint.pixelsFromRight);
}


/**
 * convert Icon to/from json
 */
void to_json (json& j, const Icon& icon) {
  j = json {
    {"icon",     icon.name},
    {"position", icon.position}
  };
}

void from_json (const json& j, Icon& icon) {
  j.at ("name")    .get_to (icon.name);
  j.at ("position").get_to (icon.position);
}


/**
 * convert LayoutEntry to/from json
 */
void to_json (json& j, const LayoutEntry& entry) {
  j = json {
    {"icon",        entry.icon},
    {"panel",       entry.panel},
    {"position",    entry.position},
    {"constraints", entry.constraints}
  };
}

void from_json (const json& j, LayoutEntry& entry) {
  j.at ("icon")       .get_to (entry.icon);
  j.at ("panel")      .get_to (entry.panel);
  j.at ("position")   .get_to (entry.position);
  j.at ("constraints").get_to (entry.constraints);
}

} // namespace sgui
