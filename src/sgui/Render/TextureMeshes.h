/**
  TextureMeshes.h
  Purpose: compute texture meshes for a given texture map. 
    computeTextureMapping should be called before any attempt to call texture.
  @author A. J.
*/

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

class TextureMeshes
{
public:
  /**
   * compute texture mapping, this should be called first
   */
  void computeTextureMapping (const TextureAtlas& textures);
  /**
   * get computed texture mesh
   */
  Mesh texture (
         const std::string& textureID,
         const uint32_t frame = 0) const;
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
