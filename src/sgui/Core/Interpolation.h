#pragma once

#include <cmath>
#include <string>
#include <cstdint>
#include <type_traits>

// forward declaration
namespace sf { 
  class Color;
  template <typename T> class Vector2;
  template <typename T> class Vector3;
}

namespace sgui
{
/**
 * @brief Round sf::Vector2 components
 */
template <typename Type>
constexpr sf::Vector2<Type> round (const sf::Vector2<Type>& value);

/**
 * @brief Round sf::Vector3 components
 */
template <typename Type>
constexpr sf::Vector2<Type> round (const sf::Vector3<Type>& value);

/**
  @brief Linear interpolation between min and max if value belong to [0, 1], with 
  extrapolation otherwise (see clampedLerp).
 */
template <typename Type>
constexpr Type lerp (
  const Type& min,
  const Type& max,
  const float value);

/**
  @brief Interpolate between two color, value is clamped to [0, 1].
 */
sf::Color lerp (
  const sf::Color& a,
  const sf::Color& b,
  const float value);

/**
 * @brief Clamp value between min and max
 */
template <typename Type>
constexpr Type clamp (
  const Type min,
  const Type max,
  const Type value);

/**
  @brief Clamped linear interpolation, it always return a value in [min, max] regardless
  of if value is in [0, 1] or not
 */
template <typename Type>
constexpr Type clampedLerp (
  const Type& min,
  const Type& max,
  const float value);

/**
  @brief Inverse linear interpolation. Return in [0, 1] range if value is in [min, max].
 */
template <typename Type>
constexpr float inverseLerp (
  const Type& min,
  const Type& max,
  const Type& value);

/**
  @brief Remap a value from [minInput, maxInput] range to [minOutput, maxOutput],
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

} // namespace sgui

#include "Interpolation.tpp"
