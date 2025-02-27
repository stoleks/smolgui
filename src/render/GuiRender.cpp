#include "GuiRender.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "resources/LoadTextureAtlas.h"
#include "core/Interpolation.h"
#include "spdlog/spdlog.h"

namespace sgui
{
/////////////////////////////////////////////////
void GuiRender::setResources (
  sf::Font& font,
  sf::Texture& texture,
  const TextureAtlas& atlas)
{
  // get texture and font
  mGuiFont = &font;
  mGuiTexture = &texture;

  // pre-compute texture quad
  mTexturesUV.computeTextureMapping (atlas);

  // initialize clipping layer
  mBaseView = sf::View ();
  initializeClippingLayers ();
}


/////////////////////////////////////////////////
void GuiRender::clear ()
{
  // clear widgets, text and layers and initialize them
  mTexts.clear ();
  mWidgets.clear ();
  mWidgetLayers.clear ();
  mTooltipLayers.clear ();
  mClippingLayers.clear ();
  initializeClippingLayers ();
}

/////////////////////////////////////////////////
void GuiRender::initializeClippingLayers ()
{
  initializeMeshArray ();
  mClippingLayers.push_back (mBaseView);
  mTexts.emplace_back (std::vector <sf::Text> ());
}

/////////////////////////////////////////////////
void GuiRender::initializeMeshArray ()
{
  mWidgets.emplace_back (sf::VertexArray ());
  mWidgets.back ().setPrimitiveType (sf::PrimitiveType::Triangles);
}

/////////////////////////////////////////////////
sf::Vector2f GuiRender::textSize (
  const std::string& text,
  const uint32_t fontSize) const
{
  const auto content = sf::Text (*mGuiFont, text, fontSize);
  const auto bound = content.getLocalBounds ();
  return bound.size;
}


/////////////////////////////////////////////////
// Clipping
/////////////////////////////////////////////////
void GuiRender::updateView (
  sf::View newView)
{
  mBaseView = newView;
}

/////////////////////////////////////////////////
uint32_t GuiRender::setCurrentClippingLayer (
  const sf::FloatRect& mask)
{
  // get current view property
  const auto& viewSize = sf::Vector2f (mBaseView.getSize ());
  const auto& viewCenter = sf::Vector2f (mBaseView.getCenter ());
  const auto viewTopLeft = viewCenter - (viewSize / 2.f);

  // build clipping viewport
  const auto& viewport = mBaseView.getViewport ();
  const auto viewportRatioX = viewport.size.x / viewSize.x;
  const auto viewportRatioY = viewport.size.y / viewSize.y;
  const auto portSize = sf::Vector2f (
    mask.size.x * viewportRatioX, mask.size.y * viewportRatioY
  );
  auto portTopLeft = (mask.position - viewTopLeft);
  portTopLeft.x = (portTopLeft.x * viewportRatioX) + viewport.position.x;
  portTopLeft.y = (portTopLeft.y * viewportRatioY) + viewport.position.y;

  // reserve memory for mesh and layer
  mActiveLayer = mClippingLayers.size ();
  if (mTooltipMode) {
    mTooltipLayers.emplace_back (mActiveLayer);
  } else {
    mWidgetLayers.emplace_back (mActiveLayer);
  }
  initializeMeshArray ();
  mTexts.emplace_back (std::vector <sf::Text> ());

  // add new clipping layer
  if ((portSize.x >= 0) && (portSize.y >= 0)) {
    auto clippingView = sf::View (sf::FloatRect (
      { std::round (mask.position.x), std::round (mask.position.y) },
      { std::round (mask.size.x), std::round (mask.size.y) }
    ));
    clippingView.setViewport (sf::FloatRect (
      { portTopLeft.x, portTopLeft.y },
      { portSize.x, portSize.y }
    ));
    mClippingLayers.push_back (clippingView);
  }

  // return the active layer id
  return mActiveLayer;
}

/////////////////////////////////////////////////
uint32_t GuiRender::currentClippingLayer () const
{
  return mActiveLayer;
}

/////////////////////////////////////////////////
void GuiRender::moveToClippingLayer (
  const uint32_t layerId)
{
  if (layerId < mClippingLayers.size ()) {
    mActiveLayer = layerId;
  } else {
    spdlog::error ("GuiRender::moveToClippingLayer: the layer's id is incorrect, clipping layer will not be moved");
  }
}

/////////////////////////////////////////////////
void GuiRender::noClipping ()
{
  mActiveLayer = 0;
}

/////////////////////////////////////////////////
bool GuiRender::isClipped (
  const sf::Vector2f& position) const
{
  const auto view = mClippingLayers [mActiveLayer];
  const auto viewBox = sf::FloatRect (
    view.getCenter () - view.getSize () / 2.f,
    view.getSize ()
  );
  return !viewBox.contains (position);
}



/////////////////////////////////////////////////
// draw
/////////////////////////////////////////////////
void GuiRender::drawIcon (
  const sf::FloatRect& box,
  const std::string& name)
{
  // get texture mesh and apppend it
  auto newIcon = mTexturesUV.texture ("Icon::" + name);
  appendMesh (std::move (newIcon), box);
}

/////////////////////////////////////////////////
void GuiRender::drawProgressBar (
  const sf::FloatRect& box,
  const float progress)
{
  // add bar mesh
  const auto state = ItemState::Neutral;
  addThreePatch (box, state, "ProgressBar");

  // get filling texture coordinates
  addThreePatch (box, state, "ProgressFilling", true, progress);
}

/////////////////////////////////////////////////
void GuiRender::drawText (
  const sf::Vector2f& position,
  const std::string& text,
  const sf::Color& textColor,
  const uint32_t fontSize)
{
  // set text properties
  auto content = sf::Text (*mGuiFont, text, fontSize);
  content.setPosition (position);
  content.setFillColor (textColor);
  // draw text
  mTexts [mActiveLayer].emplace_back (std::move (content));
}

/////////////////////////////////////////////////
uint32_t GuiRender::drawCalls () const
{
  auto drawCalls = 0u;
  for (const auto& textLayer : mTexts) {
    drawCalls += textLayer.size ();
  }
  drawCalls += mClippingLayers.size ();
  return drawCalls;
}

/////////////////////////////////////////////////
void GuiRender::setTooltipMode ()
{
  mTooltipMode = true;
}

/////////////////////////////////////////////////
void GuiRender::unsetTooltipMode ()
{
  mTooltipMode = false;
}


/////////////////////////////////////////////////
// Implementation of draw interfaces
/////////////////////////////////////////////////
void GuiRender::addThreePatch (
  const sf::FloatRect& box,
  const ItemState state,
  const std::string& boxType,
  const bool horizontal,
  const float percentToDraw)
{
  // compute box part sizes
  const auto smallest = std::min (box.size.x, box.size.y);
  const auto cornerSize = smallest * sf::Vector2f (1, 1);
  auto percentCorner = 0.f;
  auto centerSize = box.size;
  auto shift = sf::Vector2f (0, 0);
  if (horizontal) {
    shift.x = smallest;
    centerSize.x -= 2*smallest;
    percentCorner = smallest / box.size.x;
  } else {
    shift.y = smallest;
    centerSize.y -= 2*smallest;
    percentCorner = smallest / box.size.y;
  }
  const auto percentCenter = 1.f - 2.f*percentCorner;

  // compute box part positions
  const auto centerPos = box.position + shift;
  const auto leftPos = box.position;
  auto rightPos = centerPos;
  if (horizontal) {
    rightPos.x += centerSize.x;
  } else {
    rightPos.y += centerSize.y;
  }

  // draw center if box is large enough
  const auto boxState = toString (state);
  if (centerSize.length () > 0.01f) {
    const auto center = boxType + "::" + boxState + "::Center";
    const auto pCenter = sgui::clamp (0.f, 1.f, (percentToDraw - percentCorner) / percentCenter);
    auto centerBox = mTexturesUV.texture (center);
    appendMesh (std::move (centerBox), sf::FloatRect (centerPos, centerSize), horizontal, pCenter);
  }

  // left corner with asked percent of filling
  const auto left = boxType + "::" + boxState + "::Left";
  const auto pLeft = sgui::clamp (0.f, 1.f, percentToDraw / percentCorner);
  auto leftBox = mTexturesUV.texture (left);
  appendMesh (std::move (leftBox), sf::FloatRect (leftPos, cornerSize), horizontal, pLeft);

  // right corner with asked percent of filling
  if (percentToDraw - percentCorner - percentCenter > 0.01f) {
    const auto right = boxType + "::" + boxState + "::Right";
    const auto pRight = 1.f - (1.f - percentToDraw) / percentCorner;
    auto rightBox = mTexturesUV.texture (right);
    appendMesh (std::move (rightBox), sf::FloatRect (rightPos, cornerSize), horizontal, pRight);
  }
}

/////////////////////////////////////////////////
void GuiRender::addNinePatch (
  const sf::FloatRect& box,
  const ItemState state,
  const std::string& boxType)
{
  // Patch size should be : size asked - 2 * corner mesh sizes 
  // except if size asked < 2 * corner mesh size
  // compute patch sizes
  const auto scale = sf::Vector2f (box.size.x, box.size.y / 3.f);
  const auto shift = sf::Vector2f (0.f, scale.y);
  const auto pos = box.position;

  // draw top box
  const auto topBox = sf::FloatRect (pos, scale);
  addEqualSizePatch (topBox, state, boxType + "::Top");

  // draw mid box
  const auto middleBox = sf::FloatRect (pos + shift, scale);
  addEqualSizePatch (middleBox, state, boxType + "::Middle");

  // draw bottom box
  const auto bottomBox = sf::FloatRect (pos + 2.f*shift, scale);
  addEqualSizePatch (bottomBox, state, boxType + "::Bottom");
}

/////////////////////////////////////////////////
void GuiRender::addEqualSizePatch (
  const sf::FloatRect& box,
  const ItemState state,
  const std::string& boxType)
{
  // compute common values
  const auto boxState = toString (state);
  const auto size = sf::Vector2f (box.size.x / 3.f, box.size.y);
  const auto pos = box.position;
  const auto shift = sf::Vector2f (size.x, 0.f);

  // draw left
  const auto left = boxType + "::" + boxState + "::Left";
  auto leftBox = mTexturesUV.texture (left);
  appendMesh (std::move (leftBox), sf::FloatRect (pos, size), true);

  // draw center
  const auto center = boxType + "::" + boxState + "::Center";
  auto centerBox = mTexturesUV.texture (center);
  appendMesh (std::move (centerBox), sf::FloatRect (pos + shift, size), true);

  // draw right
  const auto right = boxType + "::" + boxState + "::Right";
  auto rightBox = mTexturesUV.texture (right);
  appendMesh (std::move (rightBox), sf::FloatRect (pos + 2.f*shift, size), true);
}

/////////////////////////////////////////////////
void GuiRender::appendMesh (
  Mesh&& mesh,
  const sf::FloatRect& box,
  const bool horizontal,
  const float percentToDraw)
{
  // quit if percent to be drawn is null
  if (percentToDraw < 0.01f) return;

  // compute mesh coordinates
  computeMesh (mesh, box);
  computeMeshFilling (mesh, percentToDraw);

  // rotate it if needed
  if (!horizontal) {
    rotateMeshTexture (mesh);
  }

  // append it to active meshes
  auto& activeMesh = mWidgets [mActiveLayer];
  for (uint32_t i = 0; i < mesh.size (); i++) {
    activeMesh.append (std::move (mesh [i]));
  }
}

/////////////////////////////////////////////////
void GuiRender::computeMeshFilling (
  Mesh& mesh,
  const float percentToDraw)
{
  // quit if percent is not less than 1
  if (percentToDraw - 1.f < 0.01f) return;

  // change mesh size
  const auto meshOrigin = mesh[0].position.x;
  const auto meshWidth = mesh[1].position.x - meshOrigin;
  const auto filledWidth = meshOrigin + meshWidth*percentToDraw;
  mesh[1].position.x = filledWidth;
  mesh[3].position.x = filledWidth;
  mesh[4].position.x = filledWidth;

  // and adapt its texture
  const auto texOrigin = mesh[0].texCoords.x;
  const auto texWidth = mesh[1].texCoords.x - texOrigin;
  const auto filledTexWidth = texOrigin + texWidth*percentToDraw;
  mesh[1].texCoords.x = filledTexWidth;
  mesh[3].texCoords.x = filledTexWidth;
  mesh[4].texCoords.x = filledTexWidth;
}


/////////////////////////////////////////////////
// Overload of Drawable
/////////////////////////////////////////////////
void GuiRender::draw (
  sf::RenderTarget& target,
  sf::RenderStates states) const
{
  // set transform and texture
  states.transform *= getTransform ();
  states.texture = mGuiTexture;

  // draw each widgets
  for (const auto layer : mWidgetLayers) {
    drawLayer (target, states, layer);
  }
  // draw windows titles
  drawLayer (target, states, 0);
  // draw each tooltips
  for (const auto layer : mTooltipLayers) {
    drawLayer (target, states, layer);
  }

  // go back to standard view
  target.setView (mBaseView);
}

/////////////////////////////////////////////////
void GuiRender::drawLayer (
  sf::RenderTarget& target,
  sf::RenderStates states,
  uint32_t layer) const
{
  target.setView (mClippingLayers [layer]);
  target.draw (mWidgets [layer], states);
  for (auto text : mTexts [layer]) {
    target.draw (text, states);
  }
}

/////////////////////////////////////////////////
std::string GuiRender::toString (const ItemState state) const
{
  if (state == ItemState::Active) {
    return "Active";
  } else if (state == ItemState::Hovered) {
    return "Hovered";
  } else if (state == ItemState::Neutral) {
    return "Neutral";
  }
  return "";
}

} // namespace sgui
