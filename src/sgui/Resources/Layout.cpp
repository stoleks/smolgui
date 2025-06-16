#include "Layout.h"
#include "LoadLayout.h"

namespace sgui
{
/////////////////////////////////////////////////
bool Layout::loadFromFile (const std::string& set)
{
  if (m_layoutEntries.find (set) == std::end (m_layoutEntries)) {
    m_setFilenames.push_back (set);
    m_layoutEntries.insert ({set, std::vector <std::string> ()});
  }
  return sgui::loadFromFile (*this, set);
}

/////////////////////////////////////////////////
void Layout::saveInFile ()
{
  for (const auto& set : m_setFilenames) {
    sgui::saveInFile (*this, set);
  }
}

/////////////////////////////////////////////////
std::vector <std::string> Layout::entries (
  const std::string& set) const
{
  if (m_layoutEntries.find (set) != std::end (m_layoutEntries)) {
    return m_layoutEntries.at (set);
  }
  return std::vector <std::string> ();
}

/////////////////////////////////////////////////
void Layout::add (
  const std::string& set,
  const std::string& entry,
  LayoutEntry&& data)
{
  if (m_layoutEntries.find (set) != std::end (m_layoutEntries)) {
    m_layoutEntries.at (set).push_back (entry);
  } else {
    m_setFilenames.push_back (set);
    m_layoutEntries.insert ({set, std::vector <std::string> ()});
  }
  m_entries.insert ({entry, std::move (data)});
}

} // namespace sgui
