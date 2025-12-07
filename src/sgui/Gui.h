#pragma once

#include <stack>
#include <string>

#include "sgui/Widgets/Style.h"
#include "sgui/Widgets/Panel.h"
#include "sgui/Internals/ScrollerInformation.h"
#include "sgui/Internals/GuiGroup.h"
#include "sgui/Internals/GuiStates.h"
#include "sgui/Internals/Counters.h"

#include "sgui/Core/ObjectPool.h"
#include "sgui/Core/Interpolation.h"
#include "sgui/Render/Plotter.h"
#include "sgui/Render/GuiRender.h"
#include "sgui/Resources/SoundPlayer.h"
#include "sgui/Resources/TextContainer.h"

namespace sgui 
{
/**
 * @brief implement gui following the immediate gui principles, like dear-imgui, but
 *   with more control over the textures used for widgets, sounds effects and animations.
 */
class Gui : public sf::Drawable, public sf::Transformable
{
public:
  /**
   * @brief load fontawesome and initialize window size.
   */
  Gui ();
  /**
   * @brief initialize window size and all required resources
   *   this need to be called once before beginFrame/endFrame
   */
  void initialize (
         sf::Font& font,
         sf::Texture& widgetTexture,
         const TextureAtlas& widgetAtlas,
         const sf::RenderWindow& window);

  ///////////////////////////////////////////////
  /**
   * @brief set resources
   */
  void setResources (
         sf::Font& font,
         sf::Texture& widgetTexture);
  /**
   * @brief set texture atlas
   */
  void setTextureAtlas (const TextureAtlas& widgetAtlas);
  /**
   * @brief this must be called once if user want to have sound effects
   */
  void setSounds (SoundHolder& sounds);
  /**
   * @brief to change font size, color and item padding.
   */
  void setStyle (
         const Style& newStyle,
         const bool defaultPadding = true);
  /**
   * @brief set scroll wheel strength (should be ~ 20.f)
   */
  void setPixelsPerScroll (const float amount);

  ///////////////////////////////////////////////
  /**
   * @brief to set gui view
   */
  void setView (const sf::View& view);
  /**
   * @brief this function must be called at the start of every loop.
   */
  void beginFrame ();
  /**
   * @brief this function must be called at the end of every loop.
   * @param tooltipDelay delay before apparition of tooltip.
   */
  void endFrame (const float tooltipDelay = 0.5f);
  /**
   * @brief set-up inputs for gui interaction.
   * @param window window on which gui is drawn.
   * @param event obtained through pollEvent.
   */
  void update (
         const sf::RenderWindow& window,
         const std::optional <sf::Event>& event);
  /**
   * @brief set screen size, its required for panel and window 
   * to call it if no Gui::update was called
   */
  void setScreenSize (const sf::Vector2f& size);
  /**
   * @brief to allow apparition of tooltip and animation to occur.
   * @param deltaT is the current time of the frame
   */
  void updateTimer (const sf::Time& deltaT);

  ///////////////////////////////////////////////
  /**
   * @brief get current style of the gui
   */
  Style& style ();
  /**
   * @brief get current style of the gui
   */
  const Style& style () const;
  /**
   * @brief get normal size of text in gui
   */
  sf::Vector2f textSize (
      const std::string& text,
      const TextType type = TextType::Normal) const;
  /**
   * @brief to have standard height size across gui code
   */
  float textHeight (const TextType type = TextType::Normal) const;
  /**
   * @brief get active panel or window size
   */
  sf::Vector2f activePanelSize () const;
  /**
   * @brief add spacing
   * @param amount multiply this vector by the normal font size and add it to (x, y)
   */
  void addSpacing (const sf::Vector2f& amount);
  /**
   * @brief to add (amount > 0) or remove (amount < 0) last widget spacing
   */
  void addLastSpacing (const float amount = 1.f);
  /**
   * @brief register a position at which you can go back with backToAnchor.
   */
  void setAnchor ();
  /**
   * @brief go back to the _last_ setAnchor position set. 
   */
  void backToAnchor ();
  /**
   * @brief get current cursor position of the gui
   */
  sf::Vector2f cursorPosition () const;
  /**
   * @brief go back to previous line/column, next to the last widget
   */
  void sameLine ();
  /**
   * @brief to know if some part of the gui are active or hovered. It must be 
   *   called before endFrame.
   */
  bool isActive () const;
  /**
   * @brief to get size of the active parent group
   */
  sf::Vector2f parentGroupSize ();
  /**
   * @brief to get texture size as stored in the uv mapper
   */
  sf::Vector2f textureSize (const std::string& texture) const;

  ///////////////////////////////////////////////
  /**
   * @brief windows in which widgets will be arranged, their position are 
   *   automatically computed. Return true if it is not reduced or closed.
   * @param settings store panel size, position and properties
   * @param constraint store panel's constraints on position
   * @param options store special information (tooltip, horizontal)
   */
  bool beginWindow (
         Panel& settings,
         const Constraints& constraint = {},
         const WidgetOptions& options = {});
  /**
   * @brief build a window using window.panel.title as a key in texts.
   * @param window contains settings, constraints and options.
   * @param texts contains window title
   */
  bool beginWindow (
         Window& window,
         const TextContainer& texts = {});
  /**
   * @brief enWindow need to be called to clean state after beginWindow.
   */
  void endWindow ();

  /**
   * @brief static box in which widget will be arranged.
   * @param settings store panel size, position and properties
   * @param constraint store panel's constraints on position
   * @param options store special information (tooltip, horizontal)
   */
  void beginPanel (
         Panel& settings,
         const Constraints& constraint = {},
         const WidgetOptions& options = {});
  /**
   * @brief build a panel.
   * @param window contains settings, constraints and options.
   */
  void beginPanel (Window& window);
  /**
   * @brief endPanel need to be called to clean state after beginPanel.
   */
  void endPanel ();

  ///////////////////////////////////////////////
  /**
   * @brief menu bar in which menu item can be arranged. Note that it need to be
   *   called in a window or box with hasMenu set to true
   */
  void beginMenu ();
  /**
   * @brief endMenu need to be called to clean state after beginMenu.
   */
  void endMenu ();
  /**
   * @brief menu item are clickable button aligned along menu bar. It need to be
   *   enclosed in a beginMenu; { menuItem } endMenu;
   * @param text printed on the menu item.
   * @param info optional tooltip.
   */
  bool menuItem (
         const std::string& text,
         const WidgetOptions& options = {});

  ///////////////////////////////////////////////
  // as a general note for all widgets, displacement in options (if different from zero) 
  // will override automatic placement and are relative to group
  /**
   * @brief draw a simple line that span the current group width
   */
  void separation (const float thickness = 0.75f);
  /**
   * @brief draw an image
   */
  void image (
         const std::string& textureId,
         const sf::Vector2f& size = {},
         const WidgetOptions& options = {});
  /**
   * @brief display a button that return true if pressed
   */
  template <Widget ButtonType = Widget::Button>
  bool button (
         const sf::Vector2f& size,
         const WidgetOptions& options = {});
  /**
   * @brief button with a text displayed over it
   */
  bool textButton (
         const std::string& text,
         const WidgetOptions& options = {});
  /**
   * @brief clickable icon that work like a button
   */
  bool icon (
         const std::string& iconName,
         const WidgetOptions& options = {});
  /**
   * @brief display textured box that can be checked
   */
  bool checkBox (
         bool& checked,
         const WidgetOptions& options = {});

  ///////////////////////////////////////////////
  /**
   * @brief display text, if a box size is given it will be formatted to fit in
   */
  void text (
         const std::string& text,
         const TextOptions& textOptions = {},
         const WidgetOptions& options = {});

  ///////////////////////////////////////////////
  /**
   * @brief modify a number value through text
   * @param options contains displacement and description
   * @param min and max are used to clamb number, but only if they are both different from 0.
   * @param label is a text that appear in the number text box [ label: number ]
   * @param fixedWidth if true, the box will have the width to fit label + "10000" in it.
   */
  template <typename Type>
  void inputNumber (
         Type& number,
         const WidgetOptions& options = {},
         const Type min = 0,
         const Type max = 0,
         const std::string& label = "",
         const bool fixedWidth = false);
  /**
   * @brief modify a vector2 value through text, using two inputNumber call
   */
  template <typename Type>
  void inputVector2 (
         sf::Vector2<Type>& vector,
         const WidgetOptions& options = {},
         const sf::Vector2<Type>& min = {},
         const sf::Vector2<Type>& max = {});
  /**
   * @brief modify a vector3 value through text, using three inputNumber call
   */
  template <typename Type>
  void inputVector3 (
         sf::Vector3<Type>& vector,
         const WidgetOptions& options = {},
         const sf::Vector3<Type>& min = {},
         const sf::Vector3<Type>& max = {});
  /**
   * @brief modify a color value through text
   */
  void inputColor (
         sf::Color& color,
         const WidgetOptions& options = {});
  /**
   * @brief modify text on one or multiple line
   */
  void inputText (
         std::string& text,
         const TextOptions& textOptions = {},
         const WidgetOptions& options = {});
  /**
   * @brief modify a specific character
   */
  void inputKey (
         char& key,
         const WidgetOptions& options = {});

  ///////////////////////////////////////////////
  /**
   * @brief display advancement of a value between 0 and 1
   */
  void progressBar (
         const float progress,
         const sf::Vector2f& size,
         const WidgetOptions& options = {});

  ///////////////////////////////////////////////
  /**
   * @brief display selected item and all others selectables items when clicked
   */
  std::string comboBox (
         const std::vector <std::string>& list,
         const WidgetOptions& options = {});

  ///////////////////////////////////////////////
  /**
   * @brief slider to vary value between min and max
   */
  template <typename Type>
  void slider (
         Type& value,
         const Type min,
         const Type max,
         const WidgetOptions& options = {});

  ///////////////////////////////////////////////
  /**
   * @brief set plot range and number of points sampled
   */
  void setPlotRange (
         const PlotRange xRange,
         const PlotRange yRange);
  void setSample (const uint32_t sample);
  /** 
   * @brief set plot size. This depend on the context of use. In a window/panel, 
   *   plot bound will be limited to the window/panel width and height. Also a
   *   16:9 ratio is enforced when there are no bound set.
   */
  void setPlotBound (const sf::Vector2f& bound);
  /**
   * @brief remove plot bound.
   */
  void unsetPlotBound ();
  /**
   * @brief plot a function R -> R. The data is cached and not recomputed
   *   until user request an update with forcePlotUpdate.
   */
  void plot (
         const std::function<float (float)>& slope,
         const float thickness = 1.f,
         const sf::Color& lineColor = sf::Color::White);
  /**
   * @brief plot a function R -> R², i.e. a set of points. The data is cached
   *   and not recomputed until user request an update with forcePlotUpdate.
   */
  void plot (
         const std::function<sf::Vector2f (float)>& slope,
         const float thickness = 1.f,
         const sf::Color& lineColor = sf::Color::White);
  /**
   * @brief plot a set of points.
   */
  void plot (
         const std::vector<sf::Vector2f>& points,
         const float thickness = 1.f,
         const sf::Color& lineColor = sf::Color::White);
  /**
   * @brief force cache update
   */
  void forcePlotUpdate ();
private:
  /**
   * draw gui
   */
  void draw (
         sf::RenderTarget& target,
         sf::RenderStates states) const override;
  // to remove or add last vertical or horizontal spacing
  void addLastVerticalSpacing (const float amount = 1.f);
  void addLastHorizontalSpacing (const float amount = 1.f);
  // begin/end a group (subjacent struct of window/box/etc.)
  void beginGroup (
         const bool horizontal,
         const sf::Vector2f& position,
         const sf::Vector2f& size);
  void endGroup ();
  // window or panel utility functions
  sf::Vector2f computePosition (
         const Panel& settings,
         const Constraints& constraint);
  // for nested clipping layer, this need to be called before beginGroup.
  sf::FloatRect handleParentClipBox (const sf::FloatRect& box);
  // this need to be called after endGroup to clean clipping layer.
  void removeClipping ();
  // move a group with the mouse if its possible
  ItemState interactWithMouse (
         Panel& settings,
         const sf::FloatRect& box,
         const ItemID& name,
         const Tooltip& info);
  // cache plot data
  void cachePlotData (const std::function<float (float)>& slope);
  void cachePlotData (const std::function<sf::Vector2f (float)>& slope);
  // set plot bound depending of the current context
  void handlePlotBound ();
  // for scrollable panel or window
  bool isPanelScrollable (const Impl::GroupData& panel);
  bool scrollThroughPanel (
         Impl::GroupData& panel,
         const sf::FloatRect& panelBox,
         const ItemState panelState,
         const bool horizontal);
  sf::Vector2f scroller (
         float& scrollPercent,
         const sf::FloatRect& panelBox,
         const sf::Vector2f& scrollSize,
         const ItemState panelState,
         const bool horizontal);
  // to handle scroll bar in a slider
  float sliderBar (
         const sf::FloatRect& parentBox,
         const ItemState state,
         const float scrollPercent,
         const bool horizontal);
  // to handle scroll bar in a scroller
  float scrollerBar (
         const sf::FloatRect& parentBox,
         const ItemState state,
         const float scrollPercent,
         const float extraSize,
         const bool horizontal);
  // to handle shifting value of scroller/slider
  template <typename Type>
  Type sliderValue (
         const sf::FloatRect& box,
         const Type min,
         const Type max,
         const bool horizontal);
  // draw tooltip of the hovered item
  void tooltip (const float apparitionDelay);
  bool tooltipNeedReset ();
  // to manage each item in a dropList
  bool dropListItem (
         const std::string& selectedName,
         const std::string& itemName,
         const sf::Vector2f& initialPos,
         const sf::Vector2f& itemSize);
  // Utility function that return status of widget, it is not clickable by default
  ItemState itemStatus (
         const sf::FloatRect& boundingbox,
         const ItemID& item,
         const bool condition = false,
         const Tooltip& tooltip = {},
         const bool forceActive = false);
  // handle all edge case and special keys
  void handleKeyInput (
         std::string& text,
         const bool textIsTooLarge = false);
  // handle key for inputNumber
  template <typename Type>
  void handleNumberKeyInput (
         Type& number,
         const bool focused,
         const Type min,
         const Type max);
  template <typename Type>
  Type convertKeyIntoNumber (
         std::string& key,
         const Type min,
         const Type max);
  // format text to fit in a box
  std::string formatText (
         const std::string& text,
         const sf::Vector2f& boxSize,
         const TextType type = TextType::Normal);
  // get font size
  uint32_t getFontSize (const TextType type) const;
  // format text for fontawesome use
  void handleTextDrawing (
         const sf::Vector2f& position,
         const std::string& text,
        const TextType fontSize = TextType::Normal);
  // draw a fontawesome icon
  void fontawesomeIcon (
        const sf::Vector2f& position,
        const std::string& icon,
        const uint32_t fontSize);
  // to compute widget name 
  std::string initializeActivable (const std::string& key);
  // to compute position relative to the cursor/group
  sf::Vector2f computeRelativePosition (const sf::Vector2f& displacement = {});
  // to draw text description of widgets
  sf::Vector2f widgetDescription (
         const sf::Vector2f& position,
         const std::string& description);
  // to compute widget spacing and scrolling
  void updateSpacing (const sf::Vector2f& size);
  void updateScrolling ();
  Impl::GroupData getParentGroup ();
  // to process sfml events and store it in the internal state
  void handleMouseInputs (
         const sf::RenderWindow& window,
         const std::optional <sf::Event>& event);
  void handleKeyboardInputs (const std::optional <sf::Event>& event);
  // to play sounds
  void playSound (const ItemState state);
private:
  bool mSoundIsOn = false;
  // plot parameters
  bool mPlotIsBounded = false;
  uint32_t mPlotSample = 50u;
  // Tooltip clock
  float mTipAppearClock = 0.f;
  float mTipDisappearClock = 100.f;
  // Scroll intensity
  float mPixelsPerScroll = 20.f;
  // data to keep track of same line call
  sf::Vector2f mResetCursorPosition = {};
  std::stack <sf::Vector2f> mSameLinePosition;
  // counters to keep track of gui objects
  Impl::Counters mCounters = {};
  Impl::GroupChecker mChecker = {};
  // alignement data
  sf::Vector2f mCursorPosition = {};
  sf::Vector2f mPadding = {6.f, 1.5f};
  sf::Vector2f mLastSpacing = {};
  sf::Vector2f mWindowSize;
  sf::Vector2f mPlotBound;
  std::string mActiveInputNumber;
  // to play sound
  std::string mPreviousWidgetSoundId = "";
  std::string mActiveWidgetSoundId = "";
  SoundPlayer mSoundPlayer;
  // render for gui, plot and primitive shape
  Style mStyle;
  sf::Font* mFont;
  sf::Font mFontawesome;
  Plotter mPlotter;
  GuiRender mRender;
  PrimitiveShapeRender mColorRender;
  // inputs and gui state
  Impl::InputState mInputState;
  Impl::InternalItemState mGuiState;
  // gui internal data
  std::stack <sf::Vector2f> mAnchors;
  std::stack <uint32_t> mMenuClippingLayer;
  std::stack <Impl::GroupData> mGroups;
  ObjectPool <uint32_t> mGroupsActiveItem;
  ObjectPool <uint32_t> mComboBoxActiveItem;
  ObjectPool <Panel, std::string> mInputTextPanels;
  ObjectPool <std::vector <sf::Vector2f>> mPlotsData;
  ObjectPool <Impl::ScrollerInformation> mGroupsScrollerData;
};

} // namespace sgui

#include "Gui.tpp"
