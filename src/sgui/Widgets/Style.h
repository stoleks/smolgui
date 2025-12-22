#pragma once

#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace sgui
{
/////////////////////////////////////////////////
/**
 * @brief font size container
 */
struct FontSize
{
  FontSize () = default;
  // data
  uint32_t footnote = 10u;
  uint32_t subtitle = 14u;
  uint32_t normal   = 16u;
  uint32_t title    = 18u;
};

/**
 * @brief define gui font style and padding
 */
struct Style
{
  Style () = default;
  // font related
  FontSize fontSize = {};
  sf::Color fontColor = sf::Color::White;
  // widget related
  float itemSpacing = 5.f;
};

} // namespace sgui
