#include "SerializeSFML.h"

namespace sf {
  /**
   * convert color to/from json
   */
  void to_json (json& j, const sf::Color& color) {
    j = json {
      ToJson (color, r), 
      ToJson (color, g),
      ToJson (color, b),
      ToJson (color, a)
    };
  }

  void from_json (const json& j, sf::Color& color) {
    FromJson (color, r);
    FromJson (color, g);
    FromJson (color, b);
    FromJson (color, a);
  }
} // namespace sf
