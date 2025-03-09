#include "TextureMeshes.h"

#include <spdlog/spdlog.h>

#include "MeshFunctions.h"
#include "resources/TextureAtlas.h"

namespace sgui
{
/////////////////////////////////////////////////
void TextureMeshes::computeTextureMapping (
  const TextureAtlas& map)
{
  // cache every textures specified in the atlas
  uint32_t count = 0;
  mTextureMeshes.resize (map.texturesCount ());
  for (const auto& entry : map) {
    // keep track of texture index
    const auto texName = entry.first;
    const auto framesCount = entry.second.count;
    mIndexAndFrames [texName] = FrameAndIndex (framesCount, count);
    // cache texture
    for (uint32_t frame = 0; frame < framesCount; frame++) {
      const auto textureRect = map.textureRect (texName, frame);
      if (textureRect.has_value ()) {
        computeMeshTexture (mTextureMeshes [count], textureRect.value ());
      } else {
        computeMeshTexture (mTextureMeshes [count], sf::IntRect ({0, 0}, {16, 16}));
      }
      count++;
    }
  }
}

/////////////////////////////////////////////////
Mesh TextureMeshes::texture (
  const std::string& texture,
  const uint32_t frame) const
{
  // check that texture exist !
  const auto idx = mIndexAndFrames.find (texture);
  if (idx == std::end (mIndexAndFrames)) {
    return mTextureMeshes [std::begin (mIndexAndFrames)->second.textureIndex];
  }
  // return first frame if frame is out of bond
  if (frame >= idx->second.framesCount) {
    return mTextureMeshes [idx->second.textureIndex];
  }
  // return asked frame if everything is ok
  return mTextureMeshes [idx->second.textureIndex + frame];
}

} // namespace sgui
