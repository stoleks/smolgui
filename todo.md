Things to do
==========

Bugs
-----

- [ ] There is still an edge case for scrolling when window size increase and we are at the bottom of the scroll bar
- [ ] Fix tooltip as it is not displayed right now. Find source of the bug: clock bug ? display bug ? Function bug ?

Improvements
-----

- [ ] Change how text position is computed to align with description position
- [ ] Add bidirectionnal automatic scrolling
- [ ] Clarify WidgetOptions with sub-structures so that each variable has a clear meaning ?
- [ ] Add Constraints to WidgetOptions instead of displacement ?
- [ ] Change how menu work so that any widgets can be put in it.
- [ ] Add a custom Text classes that handles batch drawing, like GuiRender ?
- [ ] Clean-up the vast amount of bool in sgui::Panel...
- [ ] Implement option to control appearance of every widget, using Widget::Image to indicate custom texture and then using custom slices and user texture
- [ ] Add a structure that contains Slices, Widget and texture information
