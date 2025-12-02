Things to do
==========

Bugs
-----

- [ ] Fix incorrect render of plot axis

Improvements
-----

- [ ] Improve combo box appearance : one button that open a scrolling panel when hovered (like a tooltip)
- [ ] Add a "clickable" function that takes a box and handle all clickable aspect of gui
- [ ] Add bidirectionnal automatic scrolling
- [ ] Add option (enum Stretchable) to handle stretchable image with 1, 3 or 9 parts
- [ ] Use this enum to clarify which widgets are drawn with 1 / 3 / 9 parts
- [ ] Add a generic "image" function to draw an image with stretchable options
- [ ] Add a sprite sheet utilitary class to automatize texture and atlas generation -> based on TextureCollage from cardPrinter project
- [ ] Change nomenclatura for texture. Remove Icon::, use two small cap letter code for state and position and use _ instead of :: (it will allow to generate entry with independant filename)
- [ ] Change iconButton and icon so that they use fontawesome icons
- [ ] Change how texture are named and called for widgets
- [ ] Merge Button, IconButton IconTextButton and TextButton -> keep only textButton and change it to button, add an option to change its texture if required
- [ ] Clarify WidgetOptions with sub-structures so that each variable has a clear meaning
- [ ] Add Constraints to WidgetOptions instead of displacement ?
- [ ] Add a DefaultResources struct that store default font and texture ?
