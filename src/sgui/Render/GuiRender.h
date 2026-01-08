#pragma once

#include <vector>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "sgui/Core/Shapes.h"
#include "sgui/Render/DrawOptions.h"
#include "sgui/Render/TextureMeshes.h"
#include "sgui/Render/MeshFunctions.h"
#include "sgui/Render/ClippingLayers.h"
#include "sgui/Resources/TextureAtlas.h"

namespace sgui
{

/**
 * @brief Handle rendering for the gui 
 */
class GuiRender : public sf::Drawable, public sf::Transformable
{
public:
  /**
   * @brief Set resources used in render. It should be called before any `draw` call
   * @param sprite Texture used
   */
  void setResources (sf::Texture& sprite);
  /**
   * @brief Update view on which the gui is drawn
   */
  void updateView (const sf::View& newView);
  /**
   * @brief Set current clipping layer used to render UI. Every call will create a
   * new layer with its own independent meshes and return its id.
   * @return Index of the current clipping layer
   */
  uint32_t setCurrentClippingLayer (const sf::FloatRect& mask);
  /**
   * @brief Set texture atlas used in render. It should be called before any `draw` call
   * @param atlas Texture atlas used
   */
  void setTextureAtlas (const TextureAtlas& atlas);
  /**
   * @brief Clear all widgets and text
   */
  void clear ();
  /**
   * @brief interface to draw Gui standard element
   * @param box Rect of the widget to draw
   * @param options Contains information about Widget type, orientation and state 
   */
  void draw (
      const sf::FloatRect& box,
      const WidgetDrawOptions& options = {});
  /**
   * @brief interface to draw Gui text using utf8
   * @param text Text to display
   * @param font Font used to display text
   * @param options Contains font size, color and positions of text 
   */
  void draw (
      const std::string& text,
      const sf::Font& font,
      const TextDrawOptions& options = {});
  /**
   * @brief Get text size as if it was drawn on screen
   * @param text Text from which we want the size
   * @param font Font of the text
   * @param fontSize Size of the text font
   * @return Size of the text as it would be displayed
   */
  sf::Vector2f textSize (
      const std::string& text,
      const sf::Font& font,
      const uint32_t fontSize) const;
  /**
   * @brief set specific render for tooltip
   */
  void setTooltipMode ();
  /**
   * @brief unset specific render for tooltip
   */
  void unsetTooltipMode ();
  /**
   * @brief allow access to a texture size, as stored in the atlas
   * @param texture Key to access texture
   * @return Texture size in the sprite sheet
   */
  sf::Vector2f textureSize (const std::string& texture) const;
public:
  /**
   * @brief to handle clipping layers
   */
  ClippingLayers clipping;
private:
  /**
   * to initialize clipping layers and their mesh
   */
  void initializeClippingLayers ();
  /**
   * to draw slider, title box, etc. i.e. widget that posses
   * 2 corner with fixed size and a stretched center patch
   */
  void addThreeSlices (const sf::FloatRect& box, const std::string& widgetState, const bool horizontal = true, const float percentToDraw = 1.f);
  /**
   * to draw window box, panel box, etc. with 4 corner
   * with fixed size and 5 stretched center patch
   */
  void addNineSlices (const sf::FloatRect& box, const std::string& widgetState);
  /**
   * add mesh to the draw batch
   */
  void appendMesh (Mesh&& mesh, const sf::FloatRect& box, const bool horizontal = true, const float percentToDraw = 1.f);
  /**
   * for partial mesh rendereing
   */
  void computeMeshFilling (Mesh& mesh, const float percentToDraw, const bool horizontal = true);
  /**
   * overload of Drawable::draw
   */
  void draw (sf::RenderTarget& target, sf::RenderStates states) const override;
  /**
   * to avoid duplications
   */
  void drawLayer (sf::RenderTarget& target, sf::RenderStates states, uint32_t layer) const;
  /**
   * to convert type to name
   */
  std::string toString (const Widget widget) const;
  std::string toString (const ItemState state) const;
private:
  // define on which render we work
  bool mTooltipMode;
  std::vector <uint32_t> mWidgetLayers;
  std::vector <uint32_t> mTooltipLayers;
  // font and texture
  sf::Texture* mGuiTexture;
  // widget mesh
  std::vector <sf::VertexArray> mWidgets;
  std::vector <std::vector <sf::Text>> mTexts;
  TextureMeshes mTexturesUV;
};

} // namespace sgui
