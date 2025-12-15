#pragma once

#include <string>
#include <SFML/Graphics/Image.hpp>
#include "TextureAtlas.h"

namespace sgui 
{
/**
 * @brief Options for collage
 */
struct CollageOptions {
  CollageOptions () = default;
  CollageOptions (const sf::Color& c);
  // data
  bool useMask = false;
  sf::Color maskColor = sf::Color::White;
};

/**
 * @brief Allow user to make a collage of several textures in one image
 */
class TextureCollage {
public:
  struct ImageInfo {
    sf::Image image;
    std::string name = "unnamed";
  };
public:
  /**
   * @brief build a texture from a collage of all images in the directory
   */
  TextureCollage (
      const std::string& directory,
      const CollageOptions& options = {});
  /**
   * @brief get texture collage
   */
  const sf::Image& image () const { return m_collage; }
  /**
   * @brief get texture atlas
   */
  TextureAtlas& atlas () { return m_atlas; }
  const TextureAtlas& atlas () const { return m_atlas; }
private:
  sf::Image m_collage;
  TextureAtlas m_atlas;
};

} // namespace sgui
