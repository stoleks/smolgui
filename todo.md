Things to do
==========

Bugs
-----

- [ ] Fix incorrect text clipping when panel are scrolled
- [ ] Fix incorrect cursor position after call to sameLine()
- [ ] Fix incorrect render of plot axis

Improvements
-----

- [ ] Rework how scrolling size are computed, store panel position + highest cursor position to compute size of the scroll panel.
- [ ] Improve combo box appearance : one button that open a scrolling panel when hovered (like a tooltip)
- [ ] Add a "clickable" function that takes a box and handle all clickable aspect of gui
- [ ] Add bidirectionnal automatic scrolling
- [ ] Separate head and body of window, with option to skip head
- [ ] Add option (enum Stretchable) to handle stretchable image with 1, 3 or 9 parts
- [ ] Use this enum to clarify which widgets are drawn with 1 / 3 / 9 parts
- [ ] Add a sprite sheet utilitary class to automatize texture and atlas generation
- [ ] Change nomenclatura for texture. Remove Icon::, use two small cap letter code for state and position and use _ instead of :: (it will allow to generate entry with independant filename)
