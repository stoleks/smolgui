#pragma once

#include <SFML/Graphics/Color.hpp>
#include "sgui/Widgets/Aspect.h"

namespace sgui
{

/**
 * @brief Store options for drawing widgets
 */
struct WidgetDrawOptions {
  bool horizontal = true;   ///< Specify if a widget is horizontal or vertical
  float progress = 1.f;     ///< Percent to draw for a widget
  WidgetAspect aspect = {}; ///< To control widget aspect
};

/**
 * @brief Store options for drawing text
 */
struct TextDrawOptions {
  sf::Vector2f position = {};         ///< Text position
  sf::Color color = sf::Color::White; ///< Text color
  uint32_t size = 16u;                ///< Font size
};

} // namespace sgui
