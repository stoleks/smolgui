#pragma once

namespace sgui
{

/////////////////////////////////////////////////
/**
 * @brief Defines number of slices for stretchable widgets
 */
enum class Slices {
  One,
  Three,
  Nine
};

/**
 * @brief Defines type of tiling for center part of 9-slices widgets
 */
enum class TileMode {
  Stretch, // extend central part
  Hide,    // don't draw central part
  Repeat,  // repeat central part
  Rotate   // repeat and rotate central part
};

} // namespace sgui
