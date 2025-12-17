#include "sgui/Internals/Counters.h"
#include <spdlog/spdlog.h>

namespace sgui::Impl
{

/////////////////////////////////////////////////
GroupChecker::GroupChecker ()
{
  mCounters [GroupType::Window] = 0u;
  mCounters [GroupType::Panel] = 0u;
  mCounters [GroupType::Menu] = 0u;
}

/////////////////////////////////////////////////
void GroupChecker::reset ()
{
  reset (GroupType::Window);
  reset (GroupType::Panel);
  reset (GroupType::Menu);
}

/////////////////////////////////////////////////
void GroupChecker::reset (const GroupType group)
{
  if (mCounters [group] != 0u) {
    mCounters [group] = 0u;
    const auto n = name (group);
    spdlog::error ("A begin{} was called without its end{} counterpart !", n, n);
  }
}

/////////////////////////////////////////////////
std::string GroupChecker::name (const GroupType group)
{
  if (group == GroupType::Window) return "Window";
  if (group == GroupType::Panel)  return "Panel";
  return "Menu";
}

/////////////////////////////////////////////////
void GroupChecker::begin (const GroupType group)
{
  mCounters [group]++;
}

/////////////////////////////////////////////////
void GroupChecker::end (const GroupType group)
{
  if (mCounters [group] > 0u) {
    mCounters [group]--;
  } else {
    const auto n = name (group);
    spdlog::error ("You can't call a end{} without its begin{} counterpart !", n, n);
  }
}

} // namespace sgui::Impl
