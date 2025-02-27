#include "TextureMeshes.h"

#include "spdlog/spdlog.h"
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
    mTexturesIndex [texName] = count;
    // cache texture
    for (uint32_t frame = 0; frame < framesCount; frame++) {
      computeMeshTexture (
        mTextureMeshes [count],
        map.textureRect (texName, frame)
      );
      count++;
    }
  }
}

/////////////////////////////////////////////////
Mesh TextureMeshes::texture (
  const std::string& texture,
  const uint32_t frame) const
{
  const auto idx = mTexturesIndex.find (texture);
  if (idx == std::end (mTexturesIndex)) {
    spdlog::warn ("TextureMeshes::texture '{}' was not found in map, will return first entry", texture);
    return mTextureMeshes [std::begin (mTexturesIndex)->second];
  }
  return mTextureMeshes [idx->second + frame];
}

} // namespace sgui
