#include "UserInterface.h"

namespace sgui 
{
/////////////////////////////////////////////////
UserInterface::UserInterface (
  const Gui& g,
  const Layout& l,
  TextContainer& t)
  : gui (g), layout (l), texts (t)
{}

/////////////////////////////////////////////////
bool beginWindow (
  UserInterface& ui,
  const std::string& element,
  const bool hasMenu,
  const bool horizontal,
  const Tooltip& info)
{
  auto& panel = ui.layout.get <Panel> (element);
  const auto& title = ui.texts.get (element);
  if (ui.layout.has <Constraint> (element)) {
    const auto constraint = ui.layout.get <Constraint> (element);
    return ui.gui.beginWindow (panel, title, constraint, hasMenu, horizontal, info);
  }
  return ui.gui.beginWindow (panel, title, Constraint (), hasMenu, horizontal, info);
}

/////////////////////////////////////////////////
void endWindow (UserInterface& ui)
{
  ui.gui.endWindow ();
}


/////////////////////////////////////////////////
void beginPanel (
  UserInterface& ui,
  const std::string& element,
  const bool clipped,
  const bool horizontal,
  const Tooltip& info)
{
  auto& panel = ui.layout.get <Panel> (element);
  const auto constraint = ui.layout.get <Constraint> (element);
  return ui.gui.beginPanel (panel, constraint, clipped, horizontal, info);
}

/////////////////////////////////////////////////
void endPanel (UserInterface& ui)
{
  ui.gui.endPanel ();
}


/////////////////////////////////////////////////
void dropList (
  UserInterface& ui,
  uint32_t& selected,
  const std::vector<std::string>& list,
  const std::string& element,
  const uint32_t phantomElements)
{
  auto& panel = ui.layout.get <Panel> (element);
  ui.gui.beginPanel (panel, Constraint (), true);
  {
    ui.gui.dropList (selected, list, phantomElements);
  }
  ui.gui.endPanel ();
}

} // namespace sgui
