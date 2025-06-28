/**
  widgets.h
  Purpose: list widget textures available in gui, and their states
  @author A. J.
*/

#pragma once

namespace sgui
{

/**
 * list widget textures available in gui
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
 * list gui elements states
 */
enum class ItemState
{
  Active,
  Hovered,
  Neutral
};

} // namespace sgui
