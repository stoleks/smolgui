namespace sgui
{
/////////////////////////////////////////////////
template <typename Type>
constexpr sf::Vector2<Type> round (const sf::Vector2<Type>& value)
{
  return sf::Vector2<Type> { std::round (value.x), std::round (value.y) };
}

/////////////////////////////////////////////////
template <typename Type>
constexpr sf::Vector2<Type> round (const sf::Vector3<Type>& value)
{
  return sf::Vector3<Type> { std::round (value.x), std::round (value.y), std::round (value.z) };
}

/////////////////////////////////////////////////
template <typename Type>
constexpr Type lerp (
  const Type& min,
  const Type& max,
  const float value)
{
  if constexpr (std::is_same_v <Type, sf::Color>) {
    const uint32_t red = lerp (min.r, max.r, value);
    const uint32_t green = lerp (min.g, max.g, value);
    const uint32_t blue = lerp (min.b, max.b, value);
    const uint32_t alpha = lerp (min.a, max.a, value);
    return sf::Color (red, green, blue, alpha);
  }
  return (1.f - value)*min + value*max;
}

/////////////////////////////////////////////////
template <typename Type>
constexpr Type clamp (
  const Type min,
  const Type max,
  const Type value)
{
  return std::min (max, std::max (value, min));
}

/////////////////////////////////////////////////
template <typename Type>
constexpr Type clampedLerp (
  const Type& min,
  const Type& max,
  const float value)
{
  return clamp (min, max, lerp (min, max, value));
}

/////////////////////////////////////////////////
template <typename Type>
constexpr float inverseLerp (
  const Type& min,
  const Type& max,
  const Type& value)
{
  auto fMin = static_cast<float> (min);
  auto fMax = static_cast<float> (max);
  auto fValue = static_cast<float> (value);
  return clamp (0.f, 1.f, (fValue - fMin) / (fMax - fMin));
}

/////////////////////////////////////////////////
template <typename InputType,
          typename OutputType>
constexpr OutputType remap (
  const InputType& minInput,
  const InputType& maxInput,
  const OutputType& minOutput,
  const OutputType& maxOutput,
  const InputType& value)
{
  auto t = inverseLerp (minInput, maxInput, value);
  return clampedLerp (minOutput, maxOutput, t);
}

/////////////////////////////////////////////////
template <typename Type>
std::string toString (
  const Type num,
  const uint32_t precision)
{
  // convert number into string
  auto numStr = std::to_string (num);
  // remove extra zero of the string
  const auto dotPos = numStr.find_first_of ('.');
  if (dotPos != std::string::npos) {
    auto zeroPos = numStr.size () - 1;
    while (numStr[zeroPos] == '0' && zeroPos > dotPos){
      --zeroPos;
    }
    numStr.erase (zeroPos + 1, std::string::npos);
  }
  if (precision > 0u) {
    if (dotPos + precision < numStr.size ()) {
      numStr.erase (dotPos + precision, std::string::npos);
    }
  }
  return numStr;
}

} // namespace sgui
