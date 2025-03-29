/**
  GuiRender.h
  Purpose: class that manage rendering for the gui and allow a better separation between logical and graphical stuff.
  @author A. J.
*/

#pragma once

#include <vector>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "TextureMeshes.h"
#include "meshFunctions.h"
#include "sgui/Core/Shapes.h"
#include "sgui/Widgets/Widgets.h"
#include "sgui/Resources/TextureAtlas.h"

namespace sgui
{
/**
 * @brief manage rendering for the gui
 */
class GuiRender : public sf::Drawable, public sf::Transformable
{
public:
  /**
   * @brief set assets
   */
  void setResources (
         sf::Font& font,
         sf::Texture& sprite,
         const TextureAtlas& atlas);
  /**
   * @brief clear all widgets and text
   */
  void clear ();
  /**
   * @brief get text size as if it was drawn on screen
   */
  sf::Vector2f textSize (
         const std::string& text,
         const uint32_t fontSize) const;
  /**
   * @brief update view on which the gui is drawn
   */
  void updateView (sf::View newView);
  /**
   * @brief set current clipping layer used to render UI. Every call will create a
   * new layer with its own independent meshes and return its id.
   */
  uint32_t setCurrentClippingLayer (const sf::FloatRect& mask);
  /**
   * @brief get current active clipping layer
   */
  uint32_t currentClippingLayer () const;
  /**
   * @brief to reuse a previously set clipping layer
   */
  void moveToClippingLayer (const uint32_t layerId);
  /**
   * @brief go back to base layer with no clipping
   */
  void noClipping ();
  /**
   * @brief to tell if a position is visible or not (clipped)
   */
  bool isClipped (const sf::Vector2f& position) const;
  /**
   * @brief interface to draw Gui standard element
   */
  template <Widget Type>
  void draw (
         const sf::FloatRect& box,
         const ItemState state,
         const bool horizontal = true);
  /**
   * @brief interface to draw Gui connection
   */
  template <Widget ConnectionType>
  void draw (
         const sf::Vector2f& begin,
         const sf::Vector2f& end,
         const float thickness,
         const ItemState state);
  /**
   * @brief interface to draw Gui icon
   */
  void drawIcon (
         const sf::FloatRect& box,
         const std::string& name);
  /**
   * @brief interface to draw Gui progress bar
   */
  void drawProgressBar (
         const sf::FloatRect& box,
         const float progress);
  /**
   * @brief interface to draw Gui text using utf8
   */
  void drawText (
         const sf::Vector2f& position,
         const std::string& text,
         const sf::Color& textColor,
         const uint32_t fontSize);
  /**
   * @brief get draw calls count
   */
  uint32_t drawCalls () const;
  /**
   * @brief set specific render for tooltip
   */
  void setTooltipMode ();
  /**
   * @brief unset specific render for tooltip
   */
  void unsetTooltipMode ();
private:
  /**
   * to initialize clipping layers and their mesh
   */
  void initializeClippingLayers ();
  void initializeMeshArray ();
  /**
   * to draw slider, title box, etc. i.e. widget that posses
   * 2 corner with fixed size and a stretched center patch
   */
  void addThreePatch (
         const sf::FloatRect& box,
         const ItemState state,
         const std::string& boxType,
         const bool horizontal = true,
         const float percentToDraw = 1.f);
  /**
   * to draw three horizontal patch with equal size
   */
  void addEqualSizePatch (
         const sf::FloatRect& box,
         const ItemState state,
         const std::string& boxType);
  /**
   * to draw window box, panel box, etc. with 4 corner
   * with fixed size and 5 stretched center patch
   */
  void addNinePatch (
         const sf::FloatRect& box,
         const ItemState state,
         const std::string& boxType);
  /**
   * add mesh to the draw batch
   */
  void appendMesh (
         Mesh&& mesh,
         const sf::FloatRect& box,
         const bool horizontal = true,
         const float percentToDraw = 1.f);
  /**
   * for partial mesh rendereing
   */
  void computeMeshFilling (
         Mesh& mesh,
         const float percentToDraw);
  /**
   * overload of Drawable::draw
   */
  void draw (
         sf::RenderTarget& target,
         sf::RenderStates states) const override;
  /**
   * to avoid duplications
   */
  void drawLayer (
         sf::RenderTarget& target,
         sf::RenderStates states,
         uint32_t layer) const;
  /**
   * to convert type to name
   */
  template <Widget Type>
  constexpr std::string toString () const;
  std::string toString (const ItemState state) const;
private:
  // define on which render to work
  bool mTooltipMode;
  // clipping layer
  uint32_t mActiveLayer = 0;
  std::vector <uint32_t> mWidgetLayers;
  std::vector <uint32_t> mTooltipLayers;
  sf::View mBaseView;
  std::vector <sf::View> mClippingLayers;
  // font and texture
  sf::Font* mGuiFont;
  sf::Texture* mGuiTexture;
  // widget mesh
  std::vector <sf::VertexArray> mWidgets;
  std::vector <std::vector <sf::Text>> mTexts;
  TextureMeshes mTexturesUV;
};

} // namespace sgui

#include "GuiRender.tpp"
