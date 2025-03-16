/**
  interpolation.h
  Purpose : implement function for interpolation between values
  @author A. J.
*/

#pragma once

#include <cmath>
#include <string>
#include <type_traits>

// forward declaration
namespace sf { class Color; }

namespace sgui
{

/**
  Linear interpolation between min and max if value belong to [0, 1], with 
  extrapolation otherwise (see clampedLerp).
 */
template <typename Type>
constexpr Type lerp (
  const Type& min,
  const Type& max,
  const float value);

/**
  Interpolate between two color, value is clamped to [0, 1].
 */
sf::Color lerp (
  const sf::Color& a,
  const sf::Color& b,
  const float value);

/**
 * Clamp value between min and max
 */
template <typename Type>
constexpr Type clamp (
  const Type min,
  const Type max,
  const Type value);

/**
  Clamped linear interpolation, it always return a value in [min, max] regardless
  of if value value is in [0, 1] or not
 */
template <typename Type>
constexpr Type clampedLerp (
  const Type& min,
  const Type& max,
  const float value);

/**
  Inverse linear interpolation. Return in [0, 1] range if value is in [min, max].
 */
template <typename Type>
constexpr float inverseLerp (
  const Type& min,
  const Type& max,
  const Type& value);

/**
  Remap a value from [minInput, maxInput] range to [minOutput, maxOutput],
  depending on where value is in [minInput, maxInput].
 */
template <typename InputType,
          typename OutputType>
constexpr OutputType remap (
  const InputType& minInput,
  const InputType& maxInput,
  const OutputType& minOutput,
  const OutputType& maxOutput,
  const InputType& value);

/**
 * convert floating point value into string
 * without useless zero after dot
 */
template <typename Type>
std::string toString (
  const Type number,
  const uint32_t precision = 0u);

} // namespace sgui

#include "Interpolation.tpp"
