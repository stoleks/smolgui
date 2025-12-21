#include <cmath>
#include <spdlog/spdlog.h>
#include "sgui/Render/ClippingLayers.h"

namespace sgui 
{
/////////////////////////////////////////////////
uint32_t ClippingLayers::initialize ()
{
  mLayers.clear ();
  mLayers.push_back (baseView);
  mActiveLayer = 0u;
  return mActiveLayer;
}

/////////////////////////////////////////////////
uint32_t ClippingLayers::setCurrentLayer (const sf::FloatRect& mask)
{
  // get current view property
  const auto& viewSize = sf::Vector2f (baseView.getSize ());
  const auto& viewCenter = sf::Vector2f (baseView.getCenter ());
  const auto viewTopLeft = viewCenter - (viewSize / 2.f);

  // build clipping viewport
  const auto& viewport = baseView.getViewport ();
  const auto viewportRatioX = viewport.size.x / viewSize.x;
  const auto viewportRatioY = viewport.size.y / viewSize.y;
  const auto portSize = sf::Vector2f (mask.size.x * viewportRatioX, mask.size.y * viewportRatioY);
  auto portTopLeft = (mask.position - viewTopLeft);
  portTopLeft.x = (portTopLeft.x * viewportRatioX) + viewport.position.x;
  portTopLeft.y = (portTopLeft.y * viewportRatioY) + viewport.position.y;

  // add a new clipping layer if it is valid
  if ((portSize.x >= 0) && (portSize.y >= 0)) {
    mActiveLayer = mLayers.size ();
    auto clippingView = sf::View (sf::FloatRect (
      { std::round (mask.position.x), std::round (mask.position.y) },
      { std::round (mask.size.x), std::round (mask.size.y) }
    ));
    clippingView.setViewport (sf::FloatRect (portTopLeft, portSize));
    mLayers.push_back (clippingView);
  }

  // return the active layer id
  return mActiveLayer;
}

/////////////////////////////////////////////////
uint32_t ClippingLayers::activeLayer () const
{
  return mActiveLayer;
}

/////////////////////////////////////////////////
void ClippingLayers::moveToLayer (const uint32_t layerId)
{
  if (layerId < mLayers.size ()) {
    mActiveLayer = layerId;
  } else {
    spdlog::error ("ClippingLayers::moveToClippingLayer: the layer's id is incorrect, clipping layer will not be moved");
  }
}

/////////////////////////////////////////////////
void ClippingLayers::disable ()
{
  mActiveLayer = 0u;
}

/////////////////////////////////////////////////
bool ClippingLayers::isClipped (const sf::Vector2f& position) const
{
  const auto view = mLayers [mActiveLayer];
  const auto viewBox = sf::FloatRect (
    view.getCenter () - view.getSize () / 2.f,
    view.getSize ()
  );
  return !viewBox.contains (position);
}

/////////////////////////////////////////////////
const sf::View& ClippingLayers::at (const uint32_t layerId) const
{
  return mLayers.at (layerId);
}

} // namespace sgui
