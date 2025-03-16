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
  TextBox,
  MultiLineTextBox,
  ItemBox,
  MenuBox,
  MenuItemBox,
  TitleBox,
  FootnoteBox,
  Button,
  TextButton,
  IconButton,
  IconTextButton,
  TitleButton,
  SkillButton,
  TreeConnection,
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
