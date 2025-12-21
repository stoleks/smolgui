#include <SFML/Graphics/RenderTarget.hpp>
#include "sgui/Render/GuiRender.h"
#include "sgui/Core/Interpolation.h"
#include "sgui/Serialization/LoadTextureAtlas.h"

namespace sgui
{
/////////////////////////////////////////////////
void GuiRender::setResources (sf::Texture& texture)
{
  // get texture and initialize clipping layer
  mGuiTexture = &texture;
  initializeClippingLayers ();
}

/////////////////////////////////////////////////
void GuiRender::updateView (const sf::View& newView)
{
  clipping.baseView = newView;
}

/////////////////////////////////////////////////
uint32_t GuiRender::setCurrentClippingLayer (const sf::FloatRect& mask)
{
  // reserve memory for meshes and layer
  mWidgets.emplace_back (sf::VertexArray ());
  mWidgets.back ().setPrimitiveType (sf::PrimitiveType::Triangles);
  mTexts.emplace_back (std::vector <sf::Text> ());
  // store active layer id for tooltip/widgets
  const auto activeLayer = clipping.setCurrentLayer (mask);
  if (mTooltipMode) {
    mTooltipLayers.emplace_back (activeLayer);
  } else {
    mWidgetLayers.emplace_back (activeLayer);
  }
  // return active layer id
  return activeLayer;
}

/////////////////////////////////////////////////
void GuiRender::setTextureAtlas (const TextureAtlas& atlas)
{
  // pre-compute texture quad
  mTexturesUV.computeTextureMapping (atlas);
}

/////////////////////////////////////////////////
void GuiRender::clear ()
{
  // clear widgets, text and layers and initialize them
  mTexts.clear ();
  mWidgets.clear ();
  mWidgetLayers.clear ();
  mTooltipLayers.clear ();
  initializeClippingLayers ();
}

/////////////////////////////////////////////////
void GuiRender::initializeClippingLayers ()
{
  const auto layer = clipping.initialize ();
  mWidgetLayers.emplace_back (layer);
  mWidgets.emplace_back (sf::VertexArray ());
  mWidgets.back ().setPrimitiveType (sf::PrimitiveType::Triangles);
  mTexts.emplace_back (std::vector <sf::Text> ());
}

/////////////////////////////////////////////////
sf::Vector2f GuiRender::textSize (
  const std::string& text,
  const sf::Font& font,
  const uint32_t fontSize) const
{
  auto content = sf::Text (font);
  content.setCharacterSize (fontSize);
  content.setString (sf::String::fromUtf8 (text.begin (), text.end ()));
  return content.getLocalBounds ().size;
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
sf::Vector2f GuiRender::textureSize (const std::string& texture) const
{
  const auto mesh = mTexturesUV.texture (texture);
  return mesh[4].texCoords - mesh[0].texCoords;
}

/////////////////////////////////////////////////
// draw
/////////////////////////////////////////////////
void GuiRender::draw (
  const sf::FloatRect& box,
  const WidgetDrawOptions& options)
{
  const auto widgetCode = toString (options.widget);
  const auto stateCode = toString (options.state);
  if (options.slices == Slices::One) {
    auto newWidget = mTexturesUV.texture (widgetCode + options.image + stateCode);
    appendMesh (std::move (newWidget), box);
  }
  if (options.slices == Slices::Three) {
    addThreeSlices (box, widgetCode + stateCode, options.horizontal, options.progress);
  }
  if (options.slices == Slices::Nine) {
    addNineSlices (box, widgetCode + stateCode);
  }
}

/////////////////////////////////////////////////
void GuiRender::draw (
  const std::string& text,
  const sf::Font& font,
  const TextDrawOptions& options)
{
  // set text properties
  auto content = sf::Text (font);
  content.setCharacterSize (options.size);
  content.setPosition (options.position);
  content.setFillColor (options.color);
  // we use explicit utf8 encoding to handle special character like 'é'
  content.setString (sf::String::fromUtf8 (text.begin (), text.end ()));
  // draw text
  mTexts.at (clipping.activeLayer ()).emplace_back (std::move (content));
}

/////////////////////////////////////////////////
// To string
/////////////////////////////////////////////////
std::string GuiRender::toString (const Widget widget) const
{
  if      (widget == Widget::Panel)        { return "panel"; }
  else if (widget == Widget::Window)       { return "window"; }
  else if (widget == Widget::TextBox)      { return "text_box"; }
  else if (widget == Widget::ItemBox)      { return "item_box"; }
  else if (widget == Widget::MenuBox)      { return "menu_box"; }
  else if (widget == Widget::MenuItemBox)  { return "menu_item_box"; }
  else if (widget == Widget::TitleBox)     { return "title_box"; }
  else if (widget == Widget::Button)       { return "button"; }
  else if (widget == Widget::IconButton)   { return "icon_button"; }
  else if (widget == Widget::TitleButton)  { return "title_button"; }
  else if (widget == Widget::CheckBox)     { return "check_box"; }
  else if (widget == Widget::SliderBar)    { return "slider_bar"; }
  else if (widget == Widget::ScrollerBar)  { return "scroller_bar"; }
  else if (widget == Widget::Slider)       { return "slider"; }
  else if (widget == Widget::Scroller)     { return "scroller"; }
  else if (widget == Widget::ProgressBar)  { return "progress_bar"; }
  else if (widget == Widget::ProgressFill) { return "filling"; }
  else if (widget == Widget::Separation)   { return "separation"; }
  return "";
}

/////////////////////////////////////////////////
std::string GuiRender::toString (const ItemState state) const
{
  if      (state == ItemState::Active)  { return "_a"; }
  else if (state == ItemState::Hovered) { return "_h"; }
  else if (state == ItemState::Neutral) { return "_n"; }
  return "";
}

/////////////////////////////////////////////////
// Implementation of draw interfaces
/////////////////////////////////////////////////
void GuiRender::addThreeSlices (
  const sf::FloatRect& box,
  const std::string& widgetTypeState,
  const bool horizontal,
  const float percentToDraw)
{
  // get texture x/y ratio
  auto leftTexture = mTexturesUV.texture (widgetTypeState + "l");
  const auto textureSize = leftTexture[4].texCoords - leftTexture[0].texCoords;

  // compute end part size
  auto endSize = box.size;
  if (horizontal) {
    endSize.x = std::round (endSize.y * textureSize.x / textureSize.y);
  } else {
    endSize.y = std::round (endSize.x * textureSize.x / textureSize.y);
  }

  // compute middle part size
  auto percentEnd = 0.f;
  auto centerSize = box.size;
  if (horizontal) {
    centerSize.x = std::max (0.f, centerSize.x - 2.f*endSize.x);
    percentEnd = endSize.x / box.size.x;
  } else {
    centerSize.y = std::max (0.f, centerSize.y - 2*endSize.y);
    percentEnd = endSize.y / box.size.y;
  }
  const auto percentMiddle = 1.f - 2.f*percentEnd;

  // compute box part positions
  const auto leftPos = box.position;
  auto centerPos = box.position;
  auto rightPos = box.position;
  if (horizontal) {
    centerPos.x += endSize.x;
    rightPos.x += endSize.x + centerSize.x;
  } else {
    centerPos.y += endSize.y;
    rightPos.y += endSize.y + centerSize.y;
  }

  // draw left corner with the right filling
  // the filling percent is simply the total percent divided by the fraction taken by the slice
  const auto pLeft = sgui::clamp (0.f, 1.f, percentToDraw / percentEnd);
  if (percentToDraw > 0.01f && pLeft > 0.01f) {
    auto leftBox = mTexturesUV.texture (widgetTypeState + "l");
    appendMesh (std::move (leftBox), sf::FloatRect (leftPos, endSize), horizontal, pLeft);
  }

  // draw middle if box is large enough
  const auto pMiddle  = sgui::clamp (0.f, 1.f, (percentToDraw - percentEnd) / percentMiddle);
  if (centerSize.length () > 0.01f && pMiddle > 0.01f) {
    auto centerBox = mTexturesUV.texture (widgetTypeState + "c");
    appendMesh (std::move (centerBox), sf::FloatRect (centerPos, centerSize), horizontal, pMiddle);
  }

  // draw right corner with the remaining filling
  const auto pRight = sgui::clamp (0.f, 1.f, (percentToDraw - percentEnd - percentMiddle) / percentEnd);
  if (pRight > 0.01f) {
    auto rightBox = mTexturesUV.texture (widgetTypeState + "r");
    appendMesh (std::move (rightBox), sf::FloatRect (rightPos, endSize), horizontal, pRight);
  }
}

/////////////////////////////////////////////////
void GuiRender::addNineSlices (
  const sf::FloatRect& box,
  const std::string& widgetTypeState)
{
  // get corner size, we assume that all 4 corner have the same size
  auto topLeftTexture = mTexturesUV.texture (widgetTypeState + "tl");
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
  appendMesh (mTexturesUV.texture (widgetTypeState + "tc"), topCenter, true);
  // draw top right corner
  const auto topRight = sf::FloatRect (box.position + (cornerSize.x + middleSize.x)*ux, cornerSize);
  appendMesh (mTexturesUV.texture (widgetTypeState + "tr"), topRight, true);

  // MIDDLE PART
  // draw center left
  const auto midLeft = sf::FloatRect (box.position + cornerSize.y*uy, middleSideSize);
  appendMesh (mTexturesUV.texture (widgetTypeState + "ml"), midLeft, true);
  // draw center
  const auto midCenter = sf::FloatRect (box.position + cornerSize, middleSize);
  appendMesh (mTexturesUV.texture (widgetTypeState + "mc"), midCenter, true);
  // draw center right
  const auto midRight = sf::FloatRect (box.position + cornerSize + middleSize.x*ux, middleSideSize);
  appendMesh (mTexturesUV.texture (widgetTypeState + "mr"), midRight, true);
  
  // BOTTOM PART
  // draw bottom left
  const auto bottomLeft = sf::FloatRect (box.position + (cornerSize.y + middleSize.y)*uy, cornerSize);
  appendMesh (mTexturesUV.texture (widgetTypeState + "bl"), bottomLeft, true);
  // draw bottom
  const auto bottomCenter = sf::FloatRect (box.position + cornerSize + middleSize.y*uy, middleTopSize);
  appendMesh (mTexturesUV.texture (widgetTypeState + "bc"), bottomCenter, true);
  // draw bottom right
  const auto bottomRight = sf::FloatRect (box.position + cornerSize + middleSize, cornerSize);
  appendMesh (mTexturesUV.texture (widgetTypeState + "br"), bottomRight, true);
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
  auto& activeMesh = mWidgets.at (clipping.activeLayer ());
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
  // draw each tooltips
  for (const auto layer : mTooltipLayers) {
    drawLayer (target, states, layer);
  }

  // go back to standard view
  target.setView (clipping.baseView);
}

/////////////////////////////////////////////////
void GuiRender::drawLayer (
  sf::RenderTarget& target,
  sf::RenderStates states,
  uint32_t layer) const
{
  target.setView (clipping.at (layer));
  target.draw (mWidgets.at (layer), states);
  for (auto text : mTexts.at (layer)) {
    target.draw (text, states);
  }
}

} // namespace sgui
