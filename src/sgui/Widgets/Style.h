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
  uint32_t footnote = 8u;
  uint32_t subtitle = 10u;
  uint32_t normal   = 12u;
  uint32_t title    = 16u;
};

/**
 * @brief define gui font style and padding
 */
struct Style
{
  Style () = default;
  // font related
  FontSize fontSize;
  sf::Color fontColor = sf::Color::Black;
  // widget related
  float itemSpacing = 4.8f;
  float itemInnerSpacing = 4.f;
};

/////////////////////////////////////////////////
/**
 * @brief define identifier for icon
 */
using IconID = std::string;

/**
 * @brief define standard gui icon information
 */
struct Icon
{
  Icon () = default;
  // data
  IconID name;
  sf::Vector2f position;
};

} // namespace sgui
