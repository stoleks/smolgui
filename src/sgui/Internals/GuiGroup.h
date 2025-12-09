#pragma once

#include <cstdint>
#include <SFML/System/Vector2.hpp>

namespace sgui 
{
namespace Impl
{

/**
 * @brief store window and panel internal data
 */
struct GroupData
{
  bool isActive = false;
  bool horizontal = false;
  bool hasMenuBar = false;
  uint32_t groupId = 0u;
  uint32_t clippingLayer = 0u;
  uint32_t plotterLayer = 0u;
  uint32_t menuItemCount = 0u;
  sf::Vector2f lastItemPosition = {};
  sf::Vector2f menuBarPosition = {};
  sf::Vector2f menuBarSize = {};
  sf::Vector2f position = {};
  sf::Vector2f size = {};
};

} // namespace sgui
} // namespace Impl
