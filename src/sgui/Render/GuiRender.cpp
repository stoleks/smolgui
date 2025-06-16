#include "GuiRender.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <spdlog/spdlog.h>

#include "sgui/Resources/LoadTextureAtlas.h"
#include "sgui/Core/Interpolation.h"

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
void GuiRender::updateView (sf::View newView)
{
  mBaseView = newView;
}

/////////////////////////////////////////////////
uint32_t GuiRender::setCurrentClippingLayer (const sf::FloatRect& mask)
{
  // get current view property
  const auto& viewSize = sf::Vector2f (mBaseView.getSize ());
  const auto& viewCenter = sf::Vector2f (mBaseView.getCenter ());
  const auto viewTopLeft = viewCenter - (viewSize / 2.f);

  // build clipping viewport
  const auto& viewport = mBaseView.getViewport ();
  const auto viewportRatioX = viewport.size.x / viewSize.x;
  const auto viewportRatioY = viewport.size.y / viewSize.y;
  const auto portSize = sf::Vector2f (mask.size.x * viewportRatioX, mask.size.y * viewportRatioY);
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
    clippingView.setViewport (sf::FloatRect (portTopLeft, portSize));
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
void GuiRender::moveToClippingLayer (const uint32_t layerId)
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
bool GuiRender::isClipped (const sf::Vector2f& position) const
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
  addThreePatch (box, "ProgressBar::" + toString (state));

  // get filling texture coordinates
  addThreePatch (box, "ProgressFilling::" + toString (state), true, progress);
}

/////////////////////////////////////////////////
void GuiRender::drawText (
  const sf::Vector2f& position,
  const std::string& text,
  const sf::Color& textColor,
  const uint32_t fontSize)
{
  // set text properties
  auto content = sf::Text (*mGuiFont);
  content.setCharacterSize (fontSize);
  content.setPosition (position);
  content.setFillColor (textColor);
  // we use explicit utf8 encoding to handle special character like 'é'
  content.setString (sf::String::fromUtf8 (text.begin (), text.end ()));
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
  const std::string& boxTypeAndState,
  const bool horizontal,
  const float percentToDraw)
{
  // compute box part sizes
  const auto smallestSide = std::min (box.size.x, box.size.y);
  const auto cornerSize = smallestSide * sf::Vector2f (1, 1);
  auto percentCorner = 0.f;
  auto centerSize = box.size;
  auto shift = sf::Vector2f ();
  if (horizontal) {
    shift.x = smallestSide;
    centerSize.x -= 2*smallestSide;
    percentCorner = smallestSide / box.size.x;
  } else {
    shift.y = smallestSide;
    centerSize.y -= 2*smallestSide;
    percentCorner = smallestSide / box.size.y;
  }
  const auto percentMiddle = 1.f - 2.f*percentCorner;

  // compute box part positions
  const auto centerPos = box.position + shift;
  const auto leftPos = box.position;
  auto rightPos = centerPos;
  if (horizontal) {
    rightPos.x += centerSize.x;
  } else {
    rightPos.y += centerSize.y;
  }

  // draw left corner with the right filling
  // the filling percent is simply the total percent divided by the fraction taken by the patch
  const auto pLeft = sgui::clamp (0.f, 1.f, percentToDraw / percentCorner);
  if (percentToDraw > 0.01f && pLeft > 0.01f) {
    const auto left = boxTypeAndState + "::Left";
    auto leftBox = mTexturesUV.texture (left);
    appendMesh (std::move (leftBox), sf::FloatRect (leftPos, cornerSize), horizontal, pLeft);
  }

  // draw middle if box is large enough
  const auto pMiddle  = sgui::clamp (0.f, 1.f, (percentToDraw - percentCorner) / percentMiddle);
  if (centerSize.length () > 0.01f && pMiddle > 0.01f) {
    const auto center = boxTypeAndState + "::Cent";
    auto centerBox = mTexturesUV.texture (center);
    appendMesh (std::move (centerBox), sf::FloatRect (centerPos, centerSize), horizontal, pMiddle);
  }

  // draw right corner with the remaining filling
  const auto pRight = sgui::clamp (0.f, 1.f, (percentToDraw - percentCorner - percentMiddle) / percentCorner);
  if (pRight > 0.01f) {
    const auto right = boxTypeAndState + "::Righ";
    auto rightBox = mTexturesUV.texture (right);
    appendMesh (std::move (rightBox), sf::FloatRect (rightPos, cornerSize), horizontal, pRight);
  }
}

/////////////////////////////////////////////////
void GuiRender::addNinePatch (
  const sf::FloatRect& box,
  const std::string& boxTypeAndState)
{
  // get corner size, we assume that all 4 corner have the same size
  auto topLeftTexture = mTexturesUV.texture (boxTypeAndState + "::Top::Left");
  const auto textureSize = topLeftTexture[4].texCoords - topLeftTexture[0].texCoords;
  const auto smallestSide = std::min (box.size.x, box.size.y);
  const auto cornerSize = std::min (smallestSide / 2.f, textureSize.x) * sf::Vector2f (1.f, 1.f);

  // middle size and unit vector along x or y for convenience
  const auto ux = sf::Vector2f (1.f, 0.f);
  const auto uy = sf::Vector2f (0.f, 1.f);
  const auto middleSize = box.size - 2.f * cornerSize;
  const auto middleSideSize = cornerSize.x*ux + middleSize.y*uy;
  const auto middleTopSize  = middleSize.x*ux + cornerSize.y*uy;

  // TOP PART
  // draw top left corner
  const auto topLeft = sf::FloatRect (box.position, cornerSize);
  appendMesh (std::move (topLeftTexture), topLeft, true);
  // draw top center corner
  const auto topCenter = sf::FloatRect (box.position + cornerSize.x*ux, middleTopSize);
  appendMesh (mTexturesUV.texture (boxTypeAndState + "::Top::Cent"), topCenter, true);
  // draw top right corner
  const auto topRight = sf::FloatRect (box.position + (cornerSize.x + middleSize.x)*ux, cornerSize);
  appendMesh (mTexturesUV.texture (boxTypeAndState + "::Top::Righ"), topRight, true);

  // MIDDLE PART
  // draw center left
  const auto midLeft = sf::FloatRect (box.position + cornerSize.y*uy, middleSideSize);
  appendMesh (mTexturesUV.texture (boxTypeAndState + "::Mid::Left"), midLeft, true);
  // draw center
  const auto midCenter = sf::FloatRect (box.position + cornerSize, middleSize);
  appendMesh (mTexturesUV.texture (boxTypeAndState + "::Mid::Cent"), midCenter, true);
  // draw center right
  const auto midRight = sf::FloatRect (box.position + cornerSize + middleSize.x*ux, middleSideSize);
  appendMesh (mTexturesUV.texture (boxTypeAndState + "::Mid::Righ"), midRight, true);
  
  // BOTTOM PART
  // draw bottom left
  const auto bottomLeft = sf::FloatRect (box.position + (cornerSize.y + middleSize.y)*uy, cornerSize);
  appendMesh (mTexturesUV.texture (boxTypeAndState + "::Bot::Left"), bottomLeft, true);
  // draw bottom
  const auto bottomCenter = sf::FloatRect (box.position + cornerSize + middleSize.y*uy, middleTopSize);
  appendMesh (mTexturesUV.texture (boxTypeAndState + "::Bot::Cent"), bottomCenter, true);
  // draw bottom right
  const auto bottomRight = sf::FloatRect (box.position + cornerSize + middleSize, cornerSize);
  appendMesh (mTexturesUV.texture (boxTypeAndState + "::Bot::Righ"), bottomRight, true);
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
  computeMeshFilling (mesh, percentToDraw, horizontal);

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
  const float percentToDraw,
  const bool horizontal)
{
  // quit if percent is not less than 1
  if (percentToDraw > 0.99f) return;

  // change mesh size
  if (horizontal) {
    const auto meshOrigin = mesh[0].position.x;
    const auto meshWidth = mesh[1].position.x - meshOrigin;
    const auto filledWidth = meshOrigin + meshWidth*percentToDraw;
    mesh[1].position.x = filledWidth;
    mesh[3].position.x = filledWidth;
    mesh[4].position.x = filledWidth;
  } else {
    const auto meshOrigin = mesh[0].position.y;
    const auto meshHeight = mesh[1].position.y - meshOrigin;
    const auto filledHeight = meshOrigin + meshHeight*percentToDraw;
    mesh[1].position.y = filledHeight;
    mesh[3].position.y = filledHeight;
    mesh[4].position.y = filledHeight;
  }

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
    return "Act";
  } else if (state == ItemState::Hovered) {
    return "Hov";
  } else if (state == ItemState::Neutral) {
    return "Neu";
  }
  return "";
}

} // namespace sgui
