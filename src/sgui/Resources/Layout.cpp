#include "Layout.h"
#include "LoadLayout.h"

namespace sgui
{
/////////////////////////////////////////////////
bool Layout::loadFromFile (const std::string& set)
{
  if (!mLayoutEntries.has (set)) {
    mSetFilenames.push_back (set);
    mLayoutEntries.insert (set, std::vector <std::string> ());
  }
  return sgui::loadFromFile (*this, set);
}

/////////////////////////////////////////////////
void Layout::saveInFile ()
{
  for (const auto& set : mSetFilenames) {
    sgui::saveInFile (*this, set);
  }
}

/////////////////////////////////////////////////
std::vector <std::string> Layout::entries (
  const std::string& set) const
{
  if (mLayoutEntries.has (set)) {
    return mLayoutEntries.at (set);
  }
  return std::vector <std::string> ();
}

/////////////////////////////////////////////////
void Layout::add (
  const std::string& set,
  const std::string& entry,
  LayoutEntry&& data)
{
  if (mLayoutEntries.has (set)) {
    mLayoutEntries.at (set).push_back (entry);
  } else {
    mSetFilenames.push_back (set);
    mLayoutEntries.insert (set, std::vector <std::string> ());
  }
  mEntries.insert (entry, std::move (data));
}

} // namespace sgui
