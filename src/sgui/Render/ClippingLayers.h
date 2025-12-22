#pragma once

#include <vector>
#include <cstdint>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

namespace sgui 
{

/**
 * @brief Handle clipping layers to only draw things in a given box
 */
class ClippingLayers
{
public:
  /**
   * @brief Initialize clipping layers with a base view
   */
  uint32_t initialize ();
  /**
   * @brief Set current clipping layer used 
   * @return Index of the current clipping layer
   */
  uint32_t setCurrentLayer (const sf::FloatRect& mask);
  /**
   * @brief Get current active clipping layer index
   * @return Index of the active clipping layer
   */
  uint32_t activeLayer () const;
  /**
   * @brief To reuse a previously set clipping layer
   */
  void moveToLayer (const uint32_t layerId);
  /**
   * @brief Go back to base layer with no clipping
   */
  void disable ();
  /**
   * @brief To tell if a position is visible or not (clipped)
   * @return `true` if position is not visible, `false` if it is visible
   */
  bool isClipped (const sf::Vector2f& position) const;
  /**
   * @brief Return stored clipping layers
   */
  const sf::View& at (const uint32_t layerId) const;
public:
  sf::View baseView = {}; ///< Base view of the clipping layers
private:
  // clipping layer
  uint32_t mActiveLayer = 0;
  std::vector <sf::View> mLayers;
};

} // namespace sgui
