#pragma once

#include <string>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <SFML/Graphics/Rect.hpp>

namespace sgui
{
/**
 * @brief Store frames count and textures to ease serialization and for better interface
 */
struct Frames {
  Frames () = default;
  uint32_t count = 1;
  sf::IntRect texture = {};
};

/**
 * @brief Contains texture position and sub-box of all sprites for a given sprite sheet.
 * Animations ca be handled with the following restrictions:
 *    - texture rect need to represent the first frame
 *    - all frames stand in one line on the sprite sheet
 *    - all frames have the same size as the first one
 */
class TextureAtlas
{
public:
  /**
   * @brief Build an empty atlas
   */
  TextureAtlas () = default;
  /**
   * @brief Build texture atlas from file
   * @param filename File from which atlas is loaded
   */
  TextureAtlas (const std::string& filename);
  /**
   * @brief Load texture atlas from file
   * @param filename File from which atlas is loaded
   * @return `true` if loading was successful
   */
  bool loadFromFile (const std::string& filename);
  /**
   * @brief Add a texture to the atlas
   * @param entry Texture name
   * @param textureRect Texture position and size
   * @param framesCount Number of frames in the animation if needed (1 by default)
   */
  void add (
         const std::string& entry,
         sf::IntRect&& textureRect,
         const uint32_t framesCount = 1);
  /**
   * @brief Remove a texture from the atlas
   * @param entry Texture name
   */
  void remove (const std::string& entry);
  /**
   * @brief Clear all textures from the atlas
   */
  void clear ();
  /**
   * @brief Get number of textures stored in the atlas
   * @return Number of textures stored.
   */
  uint32_t texturesCount () const { return mTexturesCount; }
  /**
   * @brief Get texture rect of an entry, if the entry exist
   * @param entry Texture name
   * @param frame Texture frame it its part of an animation
   * @return Texture rect if it's stored in the atlas, `std::nullopt` otherwise
   */
  std::optional <sf::IntRect> textureRect (
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
  sf::Vector2u mTextureDimension = {};
  std::unordered_map <std::string, Frames> mAtlas;
};

} // namespace sgui
