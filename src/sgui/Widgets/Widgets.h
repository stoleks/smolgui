#pragma once

namespace sgui
{

/**
 * @brief List widget textures available in gui
 */
enum class Widget
{
  Box,
  WindowBox,
  TitleBox,
  TextBox,
  MultiLineTextBox,
  ItemBox,
  MenuBox,
  MenuItemBox,
  Button,
  TextButton,
  IconButton,
  IconTextButton,
  TitleButton,
  CheckBox,
  Icon,
  SliderBar,
  ScrollerBar,
  Slider,
  Scroller,
  ProgressBar,
  ProgressFilling,
  Separation
};


/**
 * @brief List gui elements states
 */
enum class ItemState
{
  Active,
  Hovered,
  Neutral
};

} // namespace sgui
