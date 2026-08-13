#pragma once

#include <string>
#include "sgui/Widgets/Widgets.h"
#include "sgui/Widgets/ItemStates.h"

namespace sgui
{

/**
 * @brief Store data required to fully customize how a widget is drawn
 */
struct WidgetAspect
{
  std::string image = "";            ///< To use custom widget texture, you should use Widget::Image and put the texture index here
  Widget widget = Widget::None;      ///< Change default widget type
  Slices slices = Slices::Default;   ///< Change default widget slicing
  ItemState state = ItemState::None; ///< To force a specific state
};

} // namespace sgui
