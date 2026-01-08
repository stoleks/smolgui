#pragma once

#include "sgui/Widgets/Widgets.h"
#include "sgui/Widgets/ItemStates.h"

namespace sgui
{

/**
 * @brief Store data required to fully customize how a widget is drawn
 */
struct WidgetAspect
{
  WidgetAspect () = default;
  /**
   * @brief Constructor for widgets with default texture name
   */
  WidgetAspect (const Widget w, const Slices s = Slices::Default, const ItemState is = ItemState::None)
    : widget (w), slices (s), state (is) {}
  /**
   * @brief Constructor for widgets with custom texture name
   */
  WidgetAspect (const std::string& img, const Slices s = Slices::Default, const ItemState is = ItemState::None)
    : image (img), slices (s), state (is) {}
  // data
  std::string image = "";            ///< To use custom widget texture, you should use Widget::Image and put the texture index here
  Widget widget = Widget::None;      ///< Change default widget type
  Slices slices = Slices::Default;   ///< Change default widget slicing
  ItemState state = ItemState::None; ///< To force a specific state
};

} // namespace sgui
