namespace sgui
{
/**
 * Line functions
 */
/////////////////////////////////////////////////
template <typename Type>
Line<Type>::Line (
  const sf::Vector2<Type>& begin,
  const sf::Vector2<Type>& end)
  : tail (begin), head (end)
{}

/////////////////////////////////////////////////
template <typename Type>
sf::Vector2<Type> Line<Type>::direction () const
{
  return head - tail;
}

/**
 * Circle functions
 */
/////////////////////////////////////////////////
template <typename Type>
Circle<Type>::Circle (
  const sf::Vector2<Type>& c,
  Type r)
  : radius (r),
    center (c)
{}


/**
* utility functions, center of a polygon/set of points
*/
/////////////////////////////////////////////////
template<typename Type>
constexpr sf::Vector2f centroid (
  const std::vector<sf::Vector2 <Type>>& vertices)
{
  sf::Vector2f centroid;
  float signedArea = 0.f;
  // compute centroid
  for (uint32_t i = 0; i < vertices.size (); i++) {
    auto current = sf::Vector2f (vertices[i]);
    auto next = sf::Vector2f (vertices[(i + 1) % vertices.size()]);
    auto a = (current.x * next.y) - (current.y * next.x);
    signedArea += a;
    centroid += (current + next) * a;
  }
  // add the prefactor
  centroid /= (3.f * signedArea);
  return centroid;
}

} // namespace sgui
