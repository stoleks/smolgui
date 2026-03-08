#pragma once

#include <cstdint>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sgui::Impl
{

/**
 * @brief store window and panel internal data
 */
struct GroupData
{
  bool isActive = false;
  bool horizontal = false;
  bool hasMenuBar = false;
  size_t identifier = 0u;
  uint32_t clippingLayer = 0u;
  uint32_t plotterLayer = 0u;
  uint32_t menuItemCount = 0u;
  sf::Vector2f lastItemPosition = {};
  sf::FloatRect menuBox = {};
  sf::FloatRect box = {};
};

/**
 * @brief store group identifier and bounding box
 */
struct GroupHoverBox
{
  size_t identifier = 0u;
  sf::FloatRect box = {};
};

} // namespace sgui::Impl
