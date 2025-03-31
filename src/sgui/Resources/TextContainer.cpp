#include "TextContainer.h"

#include "LoadLookupTable.h"

namespace sgui
{
/////////////////////////////////////////////////
const std::string& TextContainer::tongue () const
{
  return m_activeTongue;
}

/////////////////////////////////////////////////
void TextContainer::setTongue (const std::string& tongue)
{
  m_activeTongue = tongue;
}

/////////////////////////////////////////////////
bool TextContainer::reload (const std::string& tongue)
{
  // check loading tongue validity
  auto loadingTongue = tongue;
  if (tongue == "") {
    loadingTongue = m_activeTongue;
  }

  // load all files stored for the selected tongue
  auto success = true;
  for (const auto& paths : m_paths.at (loadingTongue)) {
    success = sgui::loadFromFile (m_texts.at (loadingTongue), paths) && success;
  }
  return success;
}

/////////////////////////////////////////////////
bool TextContainer::loadFromFile (
  const std::string& filename,
  const std::string& tongue)
{
  // check loading tongue validity
  auto loadingTongue = tongue;
  if (tongue == "") {
    loadingTongue = m_activeTongue;
  }

  // check that lookup-tables exist for the loading tongue
  if (!m_texts.has (loadingTongue)) {
    m_texts.insert (loadingTongue, {});
  }
  if (!m_paths.has (loadingTongue)) {
    m_paths.insert (loadingTongue, {});
  }

  // load filename
  m_paths.at (loadingTongue).push_back (filename);
  return sgui::loadFromFile (m_texts.at (loadingTongue), filename);
}

/////////////////////////////////////////////////
void TextContainer::saveInFile (const std::string& filename)
{
  sgui::saveInFile (m_texts.at (m_activeTongue), filename);
}

/////////////////////////////////////////////////
void TextContainer::add (
  const std::string& key,
  const std::string& text)
{
  m_texts.at (m_activeTongue).insert (key, text);
}

/////////////////////////////////////////////////
void TextContainer::remove (const std::string& entry)
{
  m_texts.at (m_activeTongue).erase (entry);
}

/////////////////////////////////////////////////
std::string& TextContainer::get (const std::string& entry)
{
  return m_texts.at (m_activeTongue).at (entry);
}

/////////////////////////////////////////////////
const std::string& TextContainer::get (const std::string& entry) const
{
  return m_texts.at (m_activeTongue).at (entry);
}

/////////////////////////////////////////////////
bool TextContainer::has (const std::string& entry) const
{
  if (m_texts.has (m_activeTongue)) {
    return m_texts.at (m_activeTongue).has (entry);
  }
  return false;
}

} // namespace sgui
