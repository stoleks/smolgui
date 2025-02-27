/**
  TextureAtlas.h
  Purpose: Contains texture position and sub-box of all sprites for a given 
    sprite sheet. Animation can be handled with the following restriction:
      - texture rect need to represent the first frame
      - all frames stand in one line on the sprite sheet
      - all frames have the same size as the first one
  @author A. J.
*/

#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Graphics/Rect.hpp>

namespace sgui
{
// for serialization simplicity and better interface
struct Frames {
  uint32_t count;
  sf::IntRect texture;
};

// class definition
class TextureAtlas
{
public:
  /**
   * load texture atlas from file
   */
  bool loadFromFile (const std::string& filename);
  /**
   * add or remove an entry to the atlas
   */
  void add (
         const std::string& entry,
         sf::IntRect&& textureRect,
         const uint32_t frameCount = 0);
  void remove (const std::string& entry);
  /**
   * get or set texture dimension of the atlas
   */
  const sf::Vector2u& textureDimension () const;
  void setTextureDimension (const sf::Vector2u& dim);
  /**
   * get element count in the atlas
   */
  uint32_t texturesCount () const { return mTexturesCount; }
  /**
   * get texture rect of an entry
   */
  sf::IntRect textureRect (
         const std::string& entry,
         const uint32_t frame = 0) const;
  /**
   * iterator to store TextureAtlas in json
   */
  auto begin () { return std::begin (mAtlas); }
  auto end ()   { return std::end (mAtlas); }
  auto begin () const { return std::cbegin (mAtlas); }
  auto end ()   const { return std::cend (mAtlas); }
private:
  uint32_t mTexturesCount = { 0 };
  sf::Vector2u mTextureDimension;
  std::unordered_map <std::string, Frames> mAtlas;
};

} // namespace sgui
