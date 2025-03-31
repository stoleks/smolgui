/**
  SerializeSFML.h
  Purpose: Serialize/deserialize sfml object in json
  @author: A. J.
*/

#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector3.hpp>

#include "Macro.h"

// for readability
using json = nlohmann::json;

namespace sf 
{
/**
 * convert color to/from json
 */
void to_json (json& j, const sf::Color& color);
void from_json (const json& j, sf::Color& color);

/**
 * convert Vector2 to/from json
 */
template <typename T>
void to_json (json& j, const sf::Vector2<T>& vector) {
  j = json {
    ToJson (vector, x),
    ToJson (vector, y)
  };
}

template <typename T>
void from_json (const json& j, sf::Vector2<T>& vector) {
  FromJson (vector, x);
  FromJson (vector, y);
}

/**
 * convert Vector3 to/from json
 */
template <typename T>
void to_json (json& j, const sf::Vector3<T>& vector) {
  j = json {
    ToJson (vector, x),
    ToJson (vector, y),
    ToJson (vector, z)
  };
}

template <typename T>
void from_json (const json& j, sf::Vector3<T>& vector) {
  FromJson (vector, x);
  FromJson (vector, y);
  FromJson (vector, z);
}

/**
 * convert Rect to/from json
 */
template <typename T>
void to_json (json& j, const sf::Rect<T>& rect) {
  j = json {
    ToJson (rect, position),
    ToJson (rect, size)
  };
}

template <typename T>
void from_json (const json& j, sf::Rect<T>& rect) {
  FromJson (rect, position);
  FromJson (rect, size);
}

} // namespace sf
