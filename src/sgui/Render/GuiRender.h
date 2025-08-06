#pragma once

#include <vector>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "TextureMeshes.h"
#include "MeshFunctions.h"
#include "sgui/Core/Shapes.h"
#include "sgui/Widgets/Widgets.h"
#include "sgui/Resources/TextureAtlas.h"

namespace sgui
{
/**
 * @brief Store options for drawing widgets
 */
struct WidgetDrawOptions {
  // Helper ctors
  WidgetDrawOptions () = default;
  WidgetDrawOptions (const float p) : progress (p) {}
  WidgetDrawOptions (const ItemState& s) : state (s) {}
  WidgetDrawOptions (const ItemState& s, bool h) : horizontal (h), state (s) {}
  WidgetDrawOptions (const std::string& n) : name (n) {}
  // data
  bool horizontal = true;
  float progress = 1.f;
  ItemState state = ItemState::Neutral;
  std::string name = "";
};

/**
 * @brief Handle rendering for the gui 
 */
class GuiRender : public sf::Drawable, public sf::Transformable
{
public:
  /**
   * @brief Set resources used in render. It should be called before any `draw` call
   * @param font Font used
   * @param sprite Texture used
   * @param atlas Texture atlas used
   */
  void setResources (
         sf::Font& font,
         sf::Texture& sprite,
         const TextureAtlas& atlas);
  /**
   * @brief Clear all widgets and text
   */
  void clear ();
  /**
   * @brief Get text size as if it was drawn on screen
   * @param text Text from which we get the size
   * @param fontSize Size of the text font
   * @return Size of the text as it would be displayed
   */
  sf::Vector2f textSize (
         const std::string& text,
         const uint32_t fontSize) const;
  /**
   * @brief Update view on which the gui is drawn
   */
  void updateView (sf::View newView);
  /**
   * @brief Set current clipping layer used to render UI. Every call will create a
   * new layer with its own independent meshes and return its id.
   * @return Index of the current clipping layer
   */
  uint32_t setCurrentClippingLayer (const sf::FloatRect& mask);
  /**
   * @brief Get current active clipping layer index
   * @return Index of the active clipping layer
   */
  uint32_t currentClippingLayer () const;
  /**
   * @brief To reuse a previously set clipping layer
   */
  void moveToClippingLayer (const uint32_t layerId);
  /**
   * @brief Go back to base layer with no clipping
   */
  void noClipping ();
  /**
   * @brief To tell if a position is visible or not (clipped)
   * @return `true` if position is not visible, `false` if it is visible
   */
  bool isClipped (const sf::Vector2f& position) const;
  /**
   * @brief interface to draw Gui standard element
   * @param Type Widget type to draw, as defined in `Widget`
   * @param box Rect of the widget to draw
   * @param state State of the widget to draw
   * @param horizontal If widget is horizontal (`true`) or vertical (`false`). It's mportant for sliders.
   */
  template <Widget Type>
  void draw (
         const sf::FloatRect& box,
         const WidgetDrawOptions options = {});
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
         const std::string& boxTypeAndState,
         const bool horizontal = true,
         const float percentToDraw = 1.f);
  /**
   * to draw window box, panel box, etc. with 4 corner
   * with fixed size and 5 stretched center patch
   */
  void addNinePatch (
         const sf::FloatRect& box,
         const std::string& boxTypeAndState);
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
         const float percentToDraw,
         const bool horizontal = true);
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
