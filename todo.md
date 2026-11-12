Things to do
==========

Bugs
-----

- [ ] There is still an edge case for scrolling when window size increase and we are at the bottom of the scroll bar
- [ ] Fix tooltip as it is not displayed right now. Find source of the bug: clock bug ? display bug ? Function bug ?
- [ ] When a new panel is opened, this can lead to a wrong scrolling state of a previously opened panel, if the new panel change the internal name of the old panel.

Improvements
-----

- [ ] Add a function to simplify insertion of fontawesome icons in text, to avoid fmt::format (fmt::runtime ()) mess
- [ ] Add bidirectionnal automatic scrolling
- [ ] Add Constraints to WidgetOptions instead of displacement ?
- [ ] Change how menu work so that any widgets can be put in it.
- [ ] Add a custom Text classes that handles batch drawing, like GuiRender ?
- [ ] Add a csv to json loader and saver for texture atlas
