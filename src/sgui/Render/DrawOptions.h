#pragma once

#include <SFML/Graphics/Text.hpp>
#include "sgui/Widgets/Widgets.h"
#include "sgui/Widgets/ItemStates.h"

namespace sgui
{

/**
 * @brief Store options for drawing widgets
 */
struct WidgetDrawOptions {
  WidgetDrawOptions () = default;
  /**
   * @brief Constructor for most widgets
   */
  WidgetDrawOptions (const Widget w, const Slices s = Slices::One, const ItemState is = ItemState::Neutral, const bool h = true)
    : horizontal (h), widget (w), slices (s), state (is) {}
  /**
   * @brief Constructor for image
   */
  WidgetDrawOptions (const std::string& n, const Slices s = Slices::One, const ItemState is = ItemState::None)
    : image (n), slices (s), state (is) {}
  // data
  bool horizontal = true;               ///< Specify if a widget is horizontal or vertical
  float progress = 1.f;                 ///< Percent to draw for a widget
  std::string image = "";               ///< Texture key for an image
  Widget widget = Widget::Image;        ///< Specify widget type
  Slices slices = Slices::One;          ///< Specify if widget is composed of 1/3/9 slices
  ItemState state = ItemState::Neutral; ///< Specify widget state
};

/**
 * @brief Store options for drawing text
 */
struct TextDrawOptions {
  TextDrawOptions () = default;
  /**
   * @brief Helper constructo
   */
  TextDrawOptions (const sf::Vector2f& p, const sf::Color& c, uint32_t s)
    : position (p), color (c), size (s) {}
  // data
  sf::Vector2f position = {};         ///< Text position
  sf::Color color = sf::Color::White; ///< Text color
  uint32_t size = 16u;                ///< Font size
};

} // namespace sgui
