#include "TextureAtlas.h"

#include <spdlog/spdlog.h>

#include "LoadTextureAtlas.h"

namespace sgui
{
/////////////////////////////////////////////////
TextureAtlas::TextureAtlas (const std::string& filename)
{
  if (!loadFromFile (filename)) {
    spdlog::error ("TextureAtlas: unable to load {}", filename);
  }
}

/////////////////////////////////////////////////
bool TextureAtlas::loadFromFile (const std::string& filename)
{
  return sgui::loadFromFile (*this, filename);
}

/**
 * Add and remove entry
 */
/////////////////////////////////////////////////
void TextureAtlas::add (
  const std::string& entry,
  sf::IntRect&& textureRect,
  const uint32_t frameCount)
{
  // store frame count and first frame texture rect
  auto frames = Frames ();
  frames.count = frameCount;
  frames.texture = textureRect;
  mTexturesCount += frameCount;
  mAtlas.insert ({entry, std::move (frames)});
}

/////////////////////////////////////////////////
void TextureAtlas::remove (
  const std::string& entry)
{
  mAtlas.erase (entry);
}

/**
 * getter and setter
 */
/////////////////////////////////////////////////
const sf::Vector2u& TextureAtlas::textureDimension () const
{
  return mTextureDimension;
}

/////////////////////////////////////////////////
void TextureAtlas::setTextureDimension (const sf::Vector2u& dim)
{
  mTextureDimension = dim;
}

/**
 * get entry information
 */
/////////////////////////////////////////////////
std::optional <sf::IntRect> TextureAtlas::textureRect (
  const std::string& entry,
  const uint32_t frame) const
{
  const auto func = std::string ("TextureAtlas::textureRect :");
  // check that entry exist
  const auto tex = mAtlas.find (entry);
  if (tex == std::end (mAtlas)) {
    spdlog::error ("{} {} is not a valid entry in the atlas, will return null texture", func, entry);
    return std::nullopt;
  }
  // check that frame is in range, return first frame if not
  const auto frames = tex->second;
  if (frame >= frames.count) {
    spdlog::warn ("{} Trying to access frame out of the animation, will return frame 0", func);
    return std::optional (frames.texture);
  }
  // return asked frame
  const auto shift = sf::Vector2i (frames.texture.size.x * frame, 0);
  return std::optional (
    sf::IntRect (frames.texture.position + shift, frames.texture.size)
  );
}

} // namespace sgui
