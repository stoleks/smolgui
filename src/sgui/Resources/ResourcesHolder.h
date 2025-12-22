#pragma once

#include <vector>
#include <memory>
#include <string>
#include <cassert>
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace sgui
{
/**
 * @brief load and hold resources of all kind (image, music, etc.), that can be
 *  retrieved with Identifier that are comparable. Note that Resource should 
 *  implement a "bool loadFromFile" function for this holder to work. sf::Font
 *  use openFromFile, but it's handled with type test.
 */
template <typename Resource, typename Identifier = std::string>
class ResourcesHolder
{
public:
  /**
   * @brief load resource from file. Note that for TextContainer or GuiLayout, you can store
   * resource in several files and call load with the same id but different files name.
   */
  template <typename... Parameters>
  bool load (
         const Identifier& id,
         const std::string& filename,
         Parameters&&... args);
  /**
   * @brief reload resource from file (will not work if load was not called before)
   */
  template <typename... Parameters>
  bool reload (
         const Identifier& id,
         Parameters&&... args);
  /**
   * @brief get resource from its identifier, return first element if id was not registered
   */
  const Resource& get (const Identifier& id) const;
  /**
   * @brief get mutable resource from its identifier, return first element if id was not registered
   */
  Resource& get (const Identifier& id);
private:
  // Hold resource and check that it has a unique id
  void holdResource (
         std::unique_ptr <Resource>&& resource,
         const Identifier& id,
         const std::string& filename);
private:
  std::unordered_map <Identifier, std::vector <std::string>> m_filesPath;
  using PtrResource = std::unique_ptr <Resource>;
  std::unordered_map <Identifier, PtrResource> m_resources;
};

} // namespace sgui

/**
 * Define common resource holder type for sfml and sgui resources
 */
// Forward declaration
namespace sf {
  class Texture;
  class Shader;
  class Font;
  class SoundBuffer;
}
namespace sgui {
  class TextContainer;
  class TextureAtlas;
  class Layout;
  // sfml
  using TextureHolder = ResourcesHolder <sf::Texture>;
  using ShaderHolder  = ResourcesHolder <sf::Shader>;
  using SoundHolder   = ResourcesHolder <sf::SoundBuffer>;
  using FontHolder    = ResourcesHolder <sf::Font>;
  // sgui
  using LayoutHolder  = ResourcesHolder <Layout>;
  using AtlasHolder   = ResourcesHolder <TextureAtlas>;
  using TextHolder    = ResourcesHolder <TextContainer>;
}

#include "sgui/Resources/ResourcesHolder.tpp"
