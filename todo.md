Things to do
==========

Bugs
-----

- [ ] There is still an edge case for scrolling when window size increase and we are at the bottom of the scroll bar...

Improvements
-----

- [ ] Add bidirectionnal automatic scrolling
- [ ] Use stretch enum to clarify which widgets are drawn with 1 / 3 / 9 parts
- [ ] Add a generic "image" function to draw an image with stretchable options
- [ ] Change nomenclatura for texture. Remove Icon::, use two small cap letter code for state and position and use _ instead of :: (it will allow to generate entry with independant filename)
- [ ] Change how texture are named and called for widgets
- [ ] Merge Button, IconButton IconTextButton and TextButton -> keep only textButton and change it to button, add an option to change its texture if required
- [ ] Clarify WidgetOptions with sub-structures so that each variable has a clear meaning ?
- [ ] Add Constraints to WidgetOptions instead of displacement ?
- [ ] Add a DefaultResources struct that store default font and texture ?
