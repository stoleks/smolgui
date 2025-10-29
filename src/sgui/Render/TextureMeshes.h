#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <SFML/Graphics/Vertex.hpp>

namespace sgui
{
// forward declaration and alias
class TextureAtlas;
using Mesh = std::array <sf::Vertex, 6>;

/**
 * @brief Compute texture meshes for a given texture map
 */
class TextureMeshes
{
public:
  /**
   * @brief compute texture mapping, this should be called before any texture() call
   */
  void computeTextureMapping (const TextureAtlas& textures);
  /**
   * @brief get computed texture mesh of an entry
   */
  Mesh texture (
         const std::string& textureID,
         const uint32_t frame = 0) const;
  /**
   * to iterate through stored meshes
   */
  auto begin () { return std::begin (mTextureMeshes); }
  auto end ()   { return std::end (mTextureMeshes); }
  auto begin () const { return std::cbegin (mTextureMeshes); }
  auto end ()   const { return std::cend (mTextureMeshes); }
private:
  // For a clearer internal interface
  struct FrameAndIndex {
    FrameAndIndex () = default;
    FrameAndIndex (uint32_t count, uint32_t index) :
      framesCount (count), textureIndex (index)
    {}
    uint32_t framesCount = 0u;
    uint32_t textureIndex = 0u;
  };
private:
  std::vector <Mesh> mTextureMeshes;
  std::unordered_map <std::string, FrameAndIndex> mIndexAndFrames;
};

} // namespace sgui
