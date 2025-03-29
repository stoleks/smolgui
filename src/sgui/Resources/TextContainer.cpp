#include "TextContainer.h"

#include "LoadLookupTable.h"

namespace sgui
{
/////////////////////////////////////////////////
const std::string& TextContainer::tongue () const
{
  return mActiveTongue;
}

/////////////////////////////////////////////////
void TextContainer::setTongue (const std::string& tongue)
{
  mActiveTongue = tongue;
}

/////////////////////////////////////////////////
bool TextContainer::reload (const std::string& tongue)
{
  // check loading tongue validity
  auto loadingTongue = tongue;
  if (tongue == "") {
    loadingTongue = mActiveTongue;
  }

  // load all files stored for the selected tongue
  auto success = true;
  for (const auto& paths : mPaths.at (loadingTongue)) {
    success = sgui::loadFromFile (mTexts.at (loadingTongue), paths) && success;
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
    loadingTongue = mActiveTongue;
  }

  // check that lookup-tables exist for the loading tongue
  if (!mTexts.has (loadingTongue)) {
    mTexts.insert (loadingTongue, {});
  }
  if (!mPaths.has (loadingTongue)) {
    mPaths.insert (loadingTongue, {});
  }

  // load filename
  mPaths.at (loadingTongue).push_back (filename);
  return sgui::loadFromFile (mTexts.at (loadingTongue), filename);
}

/////////////////////////////////////////////////
void TextContainer::saveInFile (const std::string& filename)
{
  sgui::saveInFile (mTexts.at (mActiveTongue), filename);
}

/////////////////////////////////////////////////
void TextContainer::add (
  const std::string& key,
  const std::string& text)
{
  mTexts.at (mActiveTongue).insert (key, text);
}

/////////////////////////////////////////////////
void TextContainer::remove (const std::string& entry)
{
  mTexts.at (mActiveTongue).erase (entry);
}

/////////////////////////////////////////////////
std::string& TextContainer::get (const std::string& entry)
{
  return mTexts.at (mActiveTongue).at (entry);
}

/////////////////////////////////////////////////
const std::string& TextContainer::get (const std::string& entry) const
{
  return mTexts.at (mActiveTongue).at (entry);
}

} // namespace sgui
