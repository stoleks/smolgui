#include <string>
#include <sstream>
#include <iostream>
#include <spdlog/spdlog.h>

#include "sgui/Gui.h"
#include "sgui/Core/Interpolation.h"
#include "sgui/Resources/Layout.h"
#include "sgui/Resources/SoundPlayer.h"
#include "sgui/Resources/TextContainer.h"
#include "sgui/Resources/IconsFontAwesome7.h"

namespace sgui 
{

/////////////////////////////////////////////////
// Initialization of Gui
/////////////////////////////////////////////////
Gui::Gui ()
  : mFontawesome (SguiContentsDir"/fa-7-free-Solid-900.otf")
{}

/////////////////////////////////////////////////
Gui::Gui (
  sf::Font& font,
  sf::Texture& textures,
  const TextureAtlas& atlas,
  const sf::RenderWindow& window)
  : mFontawesome (SguiContentsDir"/fa-7-free-Solid-900.otf")
{
  initialize (font, textures, atlas, window);
}

/////////////////////////////////////////////////
void Gui::initialize (
  sf::Font& font,
  sf::Texture& textures,
  const TextureAtlas& atlas,
  const sf::RenderWindow& window)
{
  mWindowSize = sf::Vector2f (window.getSize ());
  setResources (font, textures);
  setTextureAtlas (atlas);
  setStyle (Style ());
  setView (window.getDefaultView ());
}

/////////////////////////////////////////////////
void Gui::setResources (
  sf::Font& font,
  sf::Texture& widgetTexture)
{
  mFont = &font; 
  mRender.setResources (widgetTexture);
}

/////////////////////////////////////////////////
void Gui::setTextureAtlas (const TextureAtlas& atlas)
{
  mRender.setTextureAtlas (atlas);
}

/////////////////////////////////////////////////
void Gui::setSounds (SoundHolder& sounds)
{
  mSoundIsOn = true;
  mSoundPlayer.setResource (sounds);
}

/////////////////////////////////////////////////
void Gui::setStyle (
  const Style& newStyle,
  const bool defaultPadding)
{
  mStyle = newStyle;
  // define default style
  if (defaultPadding) {
    mStyle.itemSpacing = mStyle.fontSize.normal / 3.f;
    mPadding = 0.25f * mStyle.fontSize.normal * sf::Vector2f (2.f, 0.5f);
  }
}

/////////////////////////////////////////////////
void Gui::setPixelsPerScroll (const float amount)
{
  mPixelsPerScroll = amount;
}

/////////////////////////////////////////////////
Style& Gui::style ()
{
  return mStyle;
}

/////////////////////////////////////////////////
const Style& Gui::style () const
{
  return mStyle;
}

/////////////////////////////////////////////////
void Gui::setView (const sf::View& view)
{
  // set view to standard gui view
  mRender.updateView (view);
  mPlotter.render.updateView (view);
}


/////////////////////////////////////////////////
// to ease alignement
/////////////////////////////////////////////////
sf::Vector2f Gui::textSize (
  const std::string& text,
  const TextType type) const
{
  return mRender.textSize (text, *mFont, getFontSize (type));
}

/////////////////////////////////////////////////
float Gui::textHeight (const TextType type) const
{
  return getFontSize (type) + 6.f*mPadding.y;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::activePanelSize () const
{
  if (!mGroups.empty ()) {
    const auto& parent = mGroups.top ();
    return parent.size;
  }
  return mWindowSize;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::normalizeSize (const sf::Vector2f& panelSize) const
{
  return panelSize.componentWiseDiv (activePanelSize ());
}
  
/////////////////////////////////////////////////
sf::Vector2f Gui::denormalizeSize (const sf::Vector2f& panelSize) const
{
  return panelSize.componentWiseMul (activePanelSize ());
}



/////////////////////////////////////////////////
// function to manage cursor position
/////////////////////////////////////////////////
void Gui::addSpacing (const sf::Vector2f& amount)
{
  const auto spacing = textHeight () * amount;
  mCursorPosition += spacing;
  updateScrolling ();
}

/////////////////////////////////////////////////
void Gui::addLastSpacing (const float amount)
{
  // add h/v spacing according to parent group
  if (!mGroups.empty () && getParentGroup ().horizontal) {
    addLastHorizontalSpacing (amount);
  } else {
    addLastVerticalSpacing (amount);
  }
}

/////////////////////////////////////////////////
void Gui::addLastHorizontalSpacing (const float amount)
{
  const auto spacing = amount * sf::Vector2f (mLastSpacing.x, 0.f);
  mCursorPosition += spacing;
  updateScrolling ();
}

/////////////////////////////////////////////////
void Gui::addLastVerticalSpacing (const float amount)
{
  const auto spacing = amount * sf::Vector2f (0.f, mLastSpacing.y);
  mCursorPosition += spacing;
  updateScrolling ();
}

/////////////////////////////////////////////////
void Gui::sameLine ()
{
  if (mSameLinePosition.empty ()) {
    mSameLinePosition.push (mCursorPosition);
    mCursorPosition = mResetCursorPosition;
  }
}

/////////////////////////////////////////////////
void Gui::setAnchor ()
{
  // store cursor position
  mAnchors.push (mCursorPosition);
}

/////////////////////////////////////////////////
void Gui::backToAnchor ()
{
  // quit if there are no anchors set
  if (mAnchors.empty ()) {
    spdlog::warn ("There are no anchors set !");
    return;
  }

  // get back to anchored position
  mCursorPosition = mAnchors.top ();
  mAnchors.pop ();
}

/////////////////////////////////////////////////
sf::Vector2f Gui::cursorPosition () const
{
  return mCursorPosition;
}



/////////////////////////////////////////////////
// To know if gui is hovered
/////////////////////////////////////////////////
bool Gui::isActive () const
{
  return mGuiState.hoveredItem != NullID
    || mGuiState.activeItem != NullID
    || mGuiState.keyboardFocus != NullID;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::parentGroupSize ()
{
  if (mGroups.empty ()) {
    return mWindowSize;
  }
  return getParentGroup ().size;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::textureSize (const std::string& texture) const
{
  return mRender.textureSize (texture);
}



/////////////////////////////////////////////////
// Gui state update
/////////////////////////////////////////////////
void Gui::beginFrame ()
{
  // clear all widgets
  mRender.clear ();
  mPlotter.clear ();
  // reset cursor position
  mCursorPosition = sf::Vector2f ();
  // clear hovered item
  mGuiState.hoveredItem = NullID;
}

/////////////////////////////////////////////////
void Gui::endFrame (const float tooltipDelay)
{
  // display active tooltip
  tooltip (tooltipDelay);

  // if left button is not pressed there is no active item
  if (!mInputState.mouseLeftDown) {
    mGuiState.activeItem = NullID;
  }
  // if right button is released we lost all focus
  if (mInputState.mouseRightReleased) {
    mGuiState.activeItem = NullID;
    mGuiState.keyboardFocus = NullID;
  }

  // reset widget count and scroll data
  mWidgetChain = "";
  mCounters.reset ();
  mChecker.reset ();

  // manage all ill-closed group and anchors
  while (!mGroups.empty ()) mGroups.pop ();
  if (!mAnchors.empty ()) {
    spdlog::error ("A setAnchor was called whithout its backToAnchor counterpart !");
    while (!mAnchors.empty ()) mAnchors.pop ();
  }
  while (!mMenuClippingLayer.empty ()) mMenuClippingLayer.pop ();

  // reset inputs
  if (!mInputState.updated) {
    mInputState.mouseDisplacement = sf::Vector2f ();
  }
  mInputState.oldMousePosition = mInputState.mousePosition;
  mInputState.updated = false;
  mInputState.mouseScrolled = false;
  mInputState.mouseLeftReleased = false;
  mInputState.mouseRightReleased = false;
  mInputState.mouseDeltaWheel = 0.f;

  // remove stopped sounds
  mSoundPlayer.removeStoppedSounds ();
  mPreviousWidgetSoundId = mActiveWidgetSoundId;
}

/////////////////////////////////////////////////
void Gui::handleMouseInputs (
  const sf::RenderWindow& window,
  const std::optional <sf::Event>& event)
{
  // mouse position
  const auto view = window.getDefaultView ();
  const auto mousePos = sf::Mouse::getPosition (window);
  const auto position = sf::Vector2f (window.mapPixelToCoords (mousePos, view));
  mInputState.mousePosition = position;
  mInputState.mouseDisplacement = position - mInputState.oldMousePosition;
  mInputState.mouseScrolled = false;
  mInputState.mouseLeftReleased = false;
  mInputState.mouseRightReleased = false;
  mInputState.mouseDeltaWheel = 0.f;
  // is mouse released ?
  if (event->is <sf::Event::MouseButtonReleased> ()) {
    auto button = event->getIf <sf::Event::MouseButtonReleased> ()->button;
    if (button == sf::Mouse::Button::Left) {
      mInputState.mouseLeftReleased = true;
      mInputState.mouseLeftDown = false;
    }
    if (button == sf::Mouse::Button::Right) {
      mInputState.mouseRightReleased = true;
      mInputState.mouseRightDown = false;
    }
  }
  // is mouse pressed ?
  if (event->is <sf::Event::MouseButtonPressed> ()) {
    auto button = event->getIf <sf::Event::MouseButtonPressed> ()->button;
    if (button == sf::Mouse::Button::Left) {
      mInputState.mouseLeftReleased = false;
      mInputState.mouseLeftDown = true;
    }
    if (button == sf::Mouse::Button::Right) {
      mInputState.mouseRightReleased = false;
      mInputState.mouseRightDown = true;
    }
  }
  // is mouse scrolled ?
  if (const auto* scrolled = event->getIf<sf::Event::MouseWheelScrolled> ()) {
    mInputState.mouseDeltaWheel = scrolled->delta;
    mInputState.mouseScrolled = scrolled->wheel == sf::Mouse::Wheel::Horizontal
      || scrolled->wheel == sf::Mouse::Wheel::Vertical;
  }
}

/////////////////////////////////////////////////
void Gui::handleKeyboardInputs (const std::optional <sf::Event>& event)
{
  // by default no key are pressed
  mInputState.keyIsPressed = false;
  mInputState.textIsEntered = false;
  // manage standard key value
  if (event->is <sf::Event::TextEntered> ()) {
    const auto key = event->getIf <sf::Event::TextEntered> ();
    if (key) {
      mInputState.keyPressed = key->unicode;
      mInputState.textIsEntered = true;
    }
  }
  // manage special key with scancode
  if (event->is <sf::Event::KeyPressed> ()) {
    const auto key = event->getIf <sf::Event::KeyPressed> ();
    if (key) {
      mInputState.code = key->code;
      mInputState.keyIsPressed = true;
    }
  }
}

/////////////////////////////////////////////////
void Gui::update (
  const sf::RenderWindow& window,
  const std::optional <sf::Event>& event)
{
  mInputState.updated = true;
  handleMouseInputs (window, event);
  handleKeyboardInputs (event);
  mWindowSize = sf::Vector2f (window.getSize ());
}

/////////////////////////////////////////////////
void Gui::setScreenSize (const sf::Vector2f& size)
{
  mWindowSize = size;
}

/////////////////////////////////////////////////
void Gui::updateTimer ()
{
  const auto dt = mInternalClock.restart ().asSeconds ();
  for (auto& clock : mComboBoxClocks) {
    clock += dt;
  }
  mTipAppearClock += dt;
  mTipDisappearClock += dt;
  mTextCursorClock += dt;
}

/////////////////////////////////////////////////
void Gui::draw (
  sf::RenderTarget& target,
  sf::RenderStates states) const
{
  // set view to standard gui view
  const auto targetView = target.getView ();
  const auto guiView = target.getDefaultView ();
  target.setView (guiView);
  // draw widgets and plot
  target.draw (mRender, states);
  target.draw (mPlotter, states);
  // return to normal target view
  target.setView (targetView);
}



/////////////////////////////////////////////////
// begin/end anchor, menu, window or box
/////////////////////////////////////////////////
bool Gui::beginWindow (
  Panel& settings,
  const Constraints& constraints,
  const WidgetOptions& options)
{
  // if window is closed skip everything
  if (settings.closed) return false;
  mChecker.begin (Impl::GroupType::Window);
  const auto name = initializeActivable ("Window");

  // compute position and create a new global group
  const auto windowSize = settings.size.componentWiseMul (parentGroupSize ());
  const auto position = computePosition (settings, constraints);

  // handle window header
  mCursorPosition = position;
  if (settings.hasHeader) {
    // handle mouse interaction
    const auto titleBoxSize = sf::Vector2f (windowSize.x, textHeight (TextType::Title));
    auto reduceCloseButtonWidth = sf::Vector2f ();
    if (settings.closable) {
      reduceCloseButtonWidth = sf::Vector2f (2.f*(titleBoxSize.y + mPadding.x), 0.f);
    }
    const auto titleBoxWithoutButtons = sf::FloatRect (mCursorPosition, titleBoxSize - reduceCloseButtonWidth);
    const auto drawBox = sf::FloatRect (mCursorPosition, titleBoxSize);
    mRender.setCurrentClippingLayer (drawBox);
    const auto state = interactWithMouse (settings, titleBoxWithoutButtons, name, options.tooltip);

    // draw title box and window name
    mRender.draw (drawBox, {Widget::TitleBox, Slices::Three, state});
    const auto textWidth = textSize (settings.title, TextType::Title).x;
    const auto shiftX = (titleBoxWithoutButtons.size.x - textWidth) / 2.f;
    const auto titlePos = sf::Vector2f {mCursorPosition.x + shiftX, mCursorPosition.y};
    handleTextDrawing (titlePos, settings.title, TextType::Title);

    // reduce or close window
    if (settings.closable) {
      // reduce
      const auto shift = sf::Vector2f (0.75f * mPadding.x, 1.5f * mPadding.y);
      const auto buttonSize = titleBoxSize.y * sf::Vector2f (1, 1);
      const auto reducePos = position + sf::Vector2f (titleBoxWithoutButtons.size.x, 0.f);
      mCursorPosition = reducePos;
      if (clickable (buttonSize, {Widget::TitleButton})) {
        settings.reduced = !(settings.reduced);
      }
      auto iconDown = ICON_FA_CIRCLE_CHEVRON_DOWN;
      if (settings.reduced) {
        iconDown = ICON_FA_CIRCLE_CHEVRON_UP;
      }
      fontawesomeIcon (reducePos + shift, iconDown, getFontSize (TextType::Title));

      // close
      const auto closePos = reducePos + sf::Vector2f (buttonSize.x, 0.f);
      mCursorPosition = closePos;
      if (clickable (buttonSize, {Widget::TitleButton})) {
        settings.closed = true;
      }
      fontawesomeIcon (closePos + shift, ICON_FA_CIRCLE_XMARK, getFontSize (TextType::Title));
    }
    mCursorPosition = position + sf::Vector2f (0.f, titleBoxSize.y);
  }

  // we need to check if window has a menu to compute correct position
  if (settings.hasMenu) {
    mCursorPosition += sf::Vector2f (0.f, textHeight ());
  }
  beginGroup (options.horizontal, mCursorPosition, windowSize);
  auto& thisWindow = mGroups.top ();

  // if window is reduced skip box drawing
  if (settings.reduced) {
    endWindow ();
    return false;
  }

  // update cursor position and draw menu bar if needed
  if (settings.hasMenu) {
    const auto menuPosition = mCursorPosition - sf::Vector2f (0.f, textHeight ());
    const auto menuBox = sf::FloatRect (menuPosition, {windowSize.x, textHeight () + 2.f*mPadding.y});
    mMenuClippingLayer.push (mRender.setCurrentClippingLayer (menuBox));
    thisWindow.menuBarPosition = menuPosition;
    thisWindow.menuBarSize = menuBox.size;
    thisWindow.hasMenuBar = settings.hasMenu;
  }

  // set clipping layer
  const auto windowBox = sf::FloatRect (mCursorPosition, windowSize);
  thisWindow.clippingLayer = mRender.setCurrentClippingLayer (windowBox);
  thisWindow.plotterLayer = mPlotter.render.setCurrentClippingLayer (windowBox);

  // draw window box and handle hovering of the window
  const auto windowStatus = itemStatus (windowBox, name);
  mRender.draw (windowBox, {Widget::Window, Slices::Nine});
  mCursorPosition += sf::Vector2f (mPadding.x, 2.5f*mPadding.y);

  // scroll through window if requested
  settings.isScrolled = scrollThroughPanel (thisWindow, windowBox, windowStatus, options.horizontal);
  return true;
}

/////////////////////////////////////////////////
bool Gui::beginWindow (
  Window& window,
  const TextContainer& texts)
{
  // use panel title as a key to retrieve the localized title, if it exist
  const auto key = window.panel.title;
  if (texts.has (key)) {
    window.panel.title = texts.get (key);
  }
  const auto open = beginWindow (window.panel, window.constraints, window.options);
  // keep track of the panel key, if user want to store panel afterward
  window.panel.title = key;
  return open;
}

/////////////////////////////////////////////////
void Gui::endWindow ()
{
  if (!mGroups.empty ()) {
    // end group and update cursor position and spacing
    const auto active = mGroups.top ();
    updateScrolling ();
    if (mGroupsScrollerData.has (active.groupId)) {
      mGroupsScrollerData.get (active.groupId).endLoop ();
    }
    endGroup ();
    mCursorPosition = active.position;
    // remove clipping and track window not closed
    removeClipping ();
    mChecker.end (Impl::GroupType::Window);
  } else {
    spdlog::warn ("There is no window to end");
  }
}

/////////////////////////////////////////////////
void Gui::beginPanel (
  Panel& settings,
  const Constraints& constraints,
  const WidgetOptions& options)
{
  mChecker.begin (Impl::GroupType::Panel);
  const auto name = initializeActivable ("Panel");

  // compute position and create a new group
  const auto position = computePosition (settings, constraints);
  const auto panelSize = settings.size.componentWiseMul (parentGroupSize ());
  const auto panelBox = sf::FloatRect (position, panelSize);
  const auto clipBox = handleParentClipBox (panelBox);
  beginGroup (options.horizontal, position, panelSize);

  // add clipping layer for the panel box
  auto& panel = mGroups.top ();
  if (settings.clipped) {
    panel.clippingLayer = mRender.setCurrentClippingLayer (clipBox);
    panel.plotterLayer = mPlotter.render.setCurrentClippingLayer (clipBox);
  }

  // draw panel box if requested
  auto state = interactWithMouse (settings, panelBox, name, options.tooltip);
  if (settings.visible) {
    const auto widget = isValid (options.widget) ? options.widget : Widget::Panel;
    const auto slices = isValid (options.slices) ? options.slices : Slices::Nine;
    state = isValid (options.state) ? options.state : state;
    mRender.draw (panelBox, {widget, slices, state});
  }

  // update cursor position
  mCursorPosition = position + mPadding + sf::Vector2f (0.f, 1.5f*mPadding.y);

  // scroll through panel if requested
  if (settings.scrollable) {
    settings.isScrolled = scrollThroughPanel (panel, panelBox, state, options.horizontal);
  }
}

/////////////////////////////////////////////////
void Gui::beginPanel (Window& window)
{
  beginPanel (window.panel, window.constraints, window.options);
}

/////////////////////////////////////////////////
void Gui::endPanel ()
{
  if (!mGroups.empty ()) {
    // end group and update cursor position and spacing
    const auto active = mGroups.top ();
    updateScrolling ();
    if (mGroupsScrollerData.has (active.groupId)) {
      mGroupsScrollerData.get (active.groupId).endLoop ();
    }
    endGroup ();
    mCursorPosition = active.position;
    updateSpacing (active.size);
    // remove clipping layer and track panel not closed by user
    removeClipping ();
    mChecker.end (Impl::GroupType::Panel);
  } else {
    spdlog::warn ("There is no panel to end");
  }
}



/////////////////////////////////////////////////
// Menu related widget
/////////////////////////////////////////////////
void Gui::beginMenu ()
{
  // menu cannot be called without parent
  if (mGroups.empty () && !mGroups.top ().hasMenuBar) {
    spdlog::error ("Menu requires a parent window or panel");
  }

  // assign unique id to the widget
  mChecker.begin (Impl::GroupType::Menu);
  const auto name = initializeActivable ("MenuBar");

  // construct a menu bar according to the parent size
  const auto& parent = mGroups.top ();
  const auto& menuPos = parent.menuBarPosition;
  const auto& menuSize = parent.menuBarSize;
  beginGroup (true, menuPos, menuSize);

  // initialize active item of the menu if needed and clipping layer
  auto& thisMenu = mGroups.top ();
  if (!mGroupsActiveItem.has (thisMenu.groupId)) {
    mGroupsActiveItem.emplace (thisMenu.groupId, 0u);
  }

  // ensure that we are on the right clipping layer
  const auto layerId = mRender.clipping.activeLayer ();
  thisMenu.clippingLayer = mMenuClippingLayer.top ();
  mMenuClippingLayer.pop ();
  mRender.clipping.moveToLayer (thisMenu.clippingLayer);

  // get menu bar status, draws it and go back to the previous clipping layer
  const auto box = sf::FloatRect (menuPos, menuSize);
  mRender.draw (box, {Widget::MenuBox, Slices::Three});
  mRender.clipping.moveToLayer (layerId);
}

/////////////////////////////////////////////////
void Gui::endMenu ()
{
  if (!mGroups.empty ()) {
    // update cursor position
    const auto& active = mGroups.top ();
    const auto menuSpacing = active.size.y + mStyle.itemSpacing;

    // update last spacing
    mLastSpacing.x = 0.f;
    mLastSpacing.y = menuSpacing;

    // end group and track menu not closed
    endGroup ();
    mChecker.end (Impl::GroupType::Menu);
  } else {
    spdlog::warn ("There are no menu to end");
  }
}

/////////////////////////////////////////////////
bool Gui::menuItem (
  const std::string& text,
  const WidgetOptions& options)
{
  // assign unique id to the widget
  auto& parentMenu = mGroups.top ();
  const auto itemId = parentMenu.menuItemCount;
  parentMenu.menuItemCount++;
  const auto name = initializeActivable ("MenuItem");
  // compute item position
  const auto itemPos = parentMenu.lastItemPosition;

  // construct menu item box
  const auto width = 3.f*mPadding.x + textSize (text).x;
  const auto height = textHeight ();
  const auto box = sf::FloatRect (itemPos, sf::Vector2f (width, height));
  parentMenu.lastItemPosition = itemPos + sf::Vector2f (width + mPadding.x, 0.f);

  // ensure that we are on the right clipping layer
  const auto layerId = mRender.clipping.activeLayer ();
  mRender.clipping.moveToLayer (parentMenu.clippingLayer);

  // compute item status
  auto state = itemStatus (box, name, mInputState.mouseLeftReleased || mInputState.mouseLeftDown);
  const auto clicked = (state == ItemState::Active);

  // update overall group status
  if (mGroupsActiveItem.has (parentMenu.groupId)) {
    auto& id = mGroupsActiveItem.get (parentMenu.groupId);
    if (id == itemId) {
      state = ItemState::Active;
    }
    if (clicked) {
      id = itemId;
    }
  }
  mRender.draw (box, {Widget::MenuItemBox, Slices::Three, state});
  parentMenu.isActive = clicked;

  // draw a description over it and go back to previous clipping layer
  handleTextDrawing (itemPos + 1.5f*mPadding, text);
  mRender.clipping.moveToLayer (layerId);

  // update cursor position and return item status
  return state == ItemState::Active && !clicked;
}

/////////////////////////////////////////////////
void Gui::tooltip (const float apparitionDelay)
{
  // reset clock when needed, reset tooltip after some time
  const auto active = mGuiState.tooltip.active;
  if (active && tooltipNeedReset ()) {
    mTipAppearClock = 0.f;
    mTipDisappearClock = 0.f;
    mGuiState.tooltip.active = false;
  }

  // display tooltip 
  const auto activeDisplay = active && mTipAppearClock > apparitionDelay;
  const auto inactiveDisplay = !active && mTipDisappearClock < apparitionDelay;
  if (activeDisplay || inactiveDisplay) {
    // compute tooltip position
    const auto parentBox = mGuiState.hoveredItemBox;
    const auto shift = parentBox.size / 2.f + mPadding;
    mCursorPosition = parentBox.getCenter () + shift;

    // display tooltip
    mRender.setTooltipMode ();
    mGuiState.tooltip.display ();
    mRender.unsetTooltipMode ();
  }
}

/////////////////////////////////////////////////
bool Gui::tooltipNeedReset ()
{
  // if hovered item is a container we don't need to reset tooltip
  const auto end = std::string::npos;
  if (mGuiState.hoveredItem.find ("Panel") != end
  || mGuiState.hoveredItem.find ("Window") != end
  || mGuiState.hoveredItem.find ("MenuBar") != end) {
    return false;
  }

  // if parent item is not active/hovered we need to reset tooltip
  return mGuiState.hoveredItem != mGuiState.tooltip.parent
    && mGuiState.activeItem != mGuiState.tooltip.parent;
}



/////////////////////////////////////////////////
// Decoration
/////////////////////////////////////////////////
void Gui::separation (const float thick)
{
  // compute line position and size
  auto size = sf::Vector2f ();
  if (!mGroups.empty ()) {
    const auto& parent = mGroups.top ();
    const auto thickness = thick * textHeight ();
    if (parent.horizontal) {
      size.x = thickness;
      size.y = parent.size.y - 3.f*mPadding.y;
    } else {
      size.x = parent.size.x - 3.f*mPadding.x;
      size.y = thickness;
    }
  }
  const auto position = computeRelativePosition ();
  const auto box = sf::FloatRect (position, size);
  // render line
  mRender.draw (box, {Widget::Separation, Slices::Three});
  updateSpacing (size);
}

/////////////////////////////////////////////////
void Gui::image (
  const std::string& textureId,
  const sf::Vector2f& size,
  const WidgetOptions& options)
{
  // if size is not specified, we take the texture size by default
  const auto position = computeRelativePosition (options.displacement);
  auto textureSize = size;
  if (textureSize.lengthSquared () < 0.01f) {
    textureSize = mRender.textureSize (textureId);
  }
  // draw texture
  const auto box = sf::FloatRect (position, textureSize);
  const auto slices = isValid (options.slices) ? options.slices : Slices::One;
  mRender.draw (box, {textureId, slices, ItemState::None});
  updateSpacing (textureSize);
}



/////////////////////////////////////////////////
// Button related widget
/////////////////////////////////////////////////
bool Gui::clickable (
  const sf::Vector2f& size,
  const WidgetOptions& options)
{
  // Initialize widget name and position
  const auto name = initializeActivable ("Clickable");
  const auto position = computeRelativePosition (options.displacement);

  // draw widget in its state and update cursor position
  const auto box = sf::FloatRect (position, size);
  const auto state = itemStatus (box, name, mInputState.mouseLeftReleased, options.tooltip);
  const auto widget = isValid (options.widget) ? options.widget : Widget::IconButton;
  const auto slices = isValid (options.slices) ? options.slices : Slices::One;
  mRender.draw (box, {widget, slices, state});
  updateSpacing (size);
  // draw its description
  const auto descrPos = position + sf::Vector2f (size.x, 0.f);
  const auto descrSize = widgetDescription (descrPos, options.description);
  updateSpacing (size + sf::Vector2f {descrSize.x, 0.f});

  // it has been clicked if state is active
  return state == ItemState::Active;
}

/////////////////////////////////////////////////
bool Gui::button (
  const std::string& text,
  const WidgetOptions& options)
{
  // compute text position and construct a button adapted to the text
  const auto position = computeRelativePosition (options.displacement) + 1.5f*mPadding;
  const auto width = std::max (textSize (text).x + 5.f*mPadding.x, 6.f*textHeight ());
  const auto size = sf::Vector2f (width, textHeight ());
  auto clickableOptions = options;
  clickableOptions.widget = Widget::Button;
  clickableOptions.slices = Slices::Three;
  const auto clicked = clickable (size, clickableOptions);
  // draw a text over it
  handleTextDrawing (position, text);
  return clicked;
}

/////////////////////////////////////////////////
bool Gui::icon (
  const std::string& iconName,
  const WidgetOptions& options)
{
  // button part
  const auto size = sf::Vector2f (1.f, 1.f) * textHeight ();
  const auto position = computeRelativePosition (options.displacement);
  const auto clicked = clickable (size, options);
  // draw an icon with fontawesome over it and description next to it
  const auto shift = sf::Vector2f (0.75f * mPadding.x, 1.5f * mPadding.y);
  fontawesomeIcon (position + shift, iconName, getFontSize (TextType::Normal) + 2u);
  return clicked;
}


/////////////////////////////////////////////////
// Box related widget
/////////////////////////////////////////////////
bool Gui::checkBox (
  bool& checked,
  const WidgetOptions& options)
{
  // initialize widget name and position
  const auto name = initializeActivable ("CheckBox");
  const auto position = computeRelativePosition (options.displacement);

  // get status of the widget,
  const auto size = textHeight () * sf::Vector2f (2.f, 1.f);
  const auto box = sf::FloatRect (position, size);
  auto state = itemStatus (box, name, mInputState.mouseLeftReleased, options.tooltip);
  // check or uncheck if asked
  if (state == ItemState::Active) {
    checked = !checked;
  }
  // if checked, draw checkBox as active
  if (checked) {
    state = ItemState::Active;
  }
  mRender.draw (box, {Widget::CheckBox, Slices::One, state});

  // draw text next to the checkbox
  const auto descrPos = position + sf::Vector2f (size.x, 0.f);
  const auto descrSize = widgetDescription (descrPos, options.description);
  // update cursor position
  updateSpacing ({size.x + descrSize.x, size.y});
  return checked;
}


/////////////////////////////////////////////////
// Text related widget
/////////////////////////////////////////////////
void Gui::text (
  const std::string& text,
  const TextOptions& textOptions,
  const WidgetOptions& options)
{
  // compute text position
  auto position = computeRelativePosition (options.displacement);
  // for alignement with other widgets
  position += sf::Vector2f (0.f, 1.5f*mPadding.y);
  
  // format the text to fit in the box if one is furnished
  const auto parent = getParentGroup ();
  auto boxSize = textOptions.boxSize;
  if (boxSize.lengthSquared () < 0.01f) {
    boxSize = parent.size;
  }
  const auto formatted = formatText (text, boxSize, textOptions.type);
  
  // draw text and update cursor position
  auto totalTextSize = sf::Vector2f ();
  for (const auto& line : formatted) {
    // center text vertically if asked
    const auto normalTextSize = textSize (line);
    if (textOptions.vertical == VerticalAlignment::Center) {
      position.y = parent.position.y + 0.5f*(parent.size.y - normalTextSize.y);
    }
    // or horizontally
    if (textOptions.horizontal == HorizontalAlignment::Center) {
      position.x = parent.position.x + 0.5f*(parent.size.x - normalTextSize.x);
    }
    handleTextDrawing (position, line, textOptions.type);
    const auto lineSize = textSize (line);
    const auto lineSpacing = mFont->getLineSpacing (getFontSize (textOptions.type)); 
    totalTextSize.x = std::max (totalTextSize.x, lineSize.x);
    totalTextSize.y += lineSpacing;
    position.y += lineSpacing;
  }
  updateSpacing (totalTextSize + mPadding);
}

/////////////////////////////////////////////////
void Gui::fontawesome (
  const std::string& iconName,
  const TextType type)
{
  const auto position = computeRelativePosition ({});
  const auto shift = sf::Vector2f (0.75f * mPadding.x, 1.5f * mPadding.y);
  const auto fontSize = getFontSize (type) + 2u;
  fontawesomeIcon (position + shift, iconName, fontSize);
  updateSpacing (mRender.textSize (iconName, mFontawesome, fontSize));
}


/////////////////////////////////////////////////
// text input related widget
/////////////////////////////////////////////////
void Gui::inputText (
  std::string& text,
  const TextOptions& textOptions,
  const WidgetOptions& options)
{
  // initialize widget name and position
  const auto name = initializeActivable ("TextInput");
  const auto basePosition = computeRelativePosition (options.displacement);
  if (!mTextCursorPositions.has (name)) {
    mTextCursorPositions.emplace (name, text.length ());
    mTextHasCursor.emplace (name, 1u);
  }

  // draw description before the box
  const auto descriptionSize = widgetDescription (basePosition - 1.5f*mPadding, options.description);
  auto boxPosition = basePosition + sf::Vector2f (descriptionSize.x + mPadding.x, -2.f*mPadding.y);

  // set-up a panel for the text
  if (!mInputTextPanels.has (name)) {
    mInputTextPanels.emplace (name);
    auto& textPanel = mInputTextPanels.get (name);
    textPanel.size = textOptions.boxSize;
  }

  // fit text box into the parent box or in the requested box
  auto& textPanel = mInputTextPanels.get (name);
  const auto inputTextSize = textSize (text);
  if (!mGroups.empty ()) {
    const auto parentWidth = mGroups.top ().size.x - 3.f*mPadding.x;
    auto width = parentWidth - descriptionSize.x;
    // if description takes 90% of parent width, go to line and take full line
    if (width < 0.1f * parentWidth) {
      boxPosition.y += descriptionSize.y;
      width += descriptionSize.x;
    }
    textPanel.size.x = width;
    const auto height = std::max (textPanel.size.y, inputTextSize.y + 2.f*mPadding.y);
    textPanel.size.y = std::max (height, textHeight () + 2.f*mPadding.y);
  // if we are not in a group, use a default minimum size
  } else if (textPanel.size.lengthSquared () < 0.01f) {
    const auto width = textSize ("sample text length").x;
    textPanel.size.x = std::max (width, inputTextSize.x + 2.f*mPadding.x);
    textPanel.size.y = textHeight () + 2.f*mPadding.y;
  }

  // get status of the widget and take keyboard focus if clicked
  const auto box = sf::FloatRect (boxPosition, textPanel.size);
  auto state = itemStatus (box, name, mInputState.mouseLeftDown);
  if (mGuiState.activeItem == name) {
    mGuiState.keyboardFocus = name;
  }

  // if this widget has keyboard focus, handles it
  const auto focused = mGuiState.keyboardFocus == name;
  if (focused) {
    auto& textCursorIndex = mTextCursorPositions.get (name);
    textCursorIndex = clamp (size_t (0), text.size (), textCursorIndex);
    handleKeyInput (text, textCursorIndex);
    state = ItemState::Active;
  }

  // set text options
  auto finalOptions = textOptions;
  finalOptions.boxSize = textPanel.size;
  // take care of size normalization
  textPanel.size = normalizeSize (textPanel.size);
  mCursorPosition = box.position;
  beginPanel (textPanel, {}, {Widget::TextBox, Slices::Nine, state});
  // remove scroller size if needed
  if (textPanel.isScrolled) {
    finalOptions.boxSize.x -= textHeight ();
  }
  // draw formatted text
  auto cursorPosition = computeRelativePosition ({});
  Gui::text (text, finalOptions);
  // draw blinking cursor in the text
  if (focused) {
    drawTextCursor (cursorPosition, name, text, finalOptions);
  }
  endPanel ();
  textPanel.size = denormalizeSize (textPanel.size);

  // update cursor position
  mCursorPosition = basePosition;
  updateSpacing (mPadding + sf::Vector2f { box.size.x + descriptionSize.x, box.size.y });
}


/////////////////////////////////////////////////
void Gui::drawTextCursor (
  sf::Vector2f position,
  const std::string& inputTextId,
  const std::string& text,
  const TextOptions& options)
{
  // get cursor status
  auto& textHasCursor = mTextHasCursor.get (inputTextId);
  auto cursorIndex = mTextCursorPositions.get (inputTextId);
  const auto formattedText = formatText (text, options.boxSize, options.type);
  // compute cursor position in text
  for (const auto& line : formattedText) {
    if (cursorIndex > line.length ()) {
      cursorIndex -= line.length ();
      position.y += textSize (line).y;
    } else {
      position.x += textSize (line.substr (0, cursorIndex)).x;
      break;
    }
  }
  // draw a blinking cursor
  if (textHasCursor == 0u) {
    if (mTextCursorClock < 0.8f) {
      mRender.draw ("|", *mFont, {sgui::round (position), mStyle.fontColor, getFontSize (options.type)});
    } else {
      textHasCursor = 1u;
      mTextCursorClock = 0.f;
    }
  } else if (mTextCursorClock > 0.2f) {
    textHasCursor = 0u;
    mTextCursorClock = 0.f;
  }
}

/////////////////////////////////////////////////
void Gui::inputKey (
  char& key,
  const WidgetOptions& options)
{
  // initialize widget name and position
  const auto name = initializeActivable ("KeyInput");
  auto position = computeRelativePosition (options.displacement);

  // draw description before the box
  const auto descrSize = widgetDescription (position, options.description);
  position.x += descrSize.x;

  // get widget status
  const auto boxSize = textHeight () * sf::Vector2f (1.f, 1.f);
  const auto box = sf::FloatRect (position, boxSize);
  auto state = itemStatus (box, name, mInputState.mouseLeftDown);

  // take keyboard focus if clicked
  if (mGuiState.activeItem == name) {
    mGuiState.keyboardFocus = name;
  }

  // if this widget has keyboard focus update key
  const auto focused = mGuiState.keyboardFocus == name;
  if (focused) {
    state = ItemState::Active;
    key = mInputState.keyPressed;
  }

  // draw char and box
  const auto text = std::string (1, key);
  mRender.draw (box, {Widget::TextBox, Slices::Three, state});
  handleTextDrawing (position + mPadding, text);

  // update cursor position
  updateSpacing ({ boxSize.x + descrSize.x, boxSize.y });
}

/////////////////////////////////////////////////
void Gui::inputColor (
  sf::Color& color,
  const WidgetOptions& options)
{
  // change color with four input number
  const auto min = std::uint8_t (0);
  const auto max = std::uint8_t (255);
  inputNumber (color.r, {options.displacement}, min, max, "r: ", true);
  sameLine ();
  inputNumber (color.g, {}, min, max, "g: ", true);
  sameLine ();
  inputNumber (color.b, {}, min, max, "b: ", true);
  sameLine ();
  inputNumber (color.a, {}, min, max, "a: ", true);
  
  // draw description
  if (options.description != "") {
    sameLine ();
    auto descriptionOptions = options;
    descriptionOptions.displacement = {};
    text (options.description, {}, descriptionOptions);
  }
}


/////////////////////////////////////////////////
// to plot progress bar and spinning wheel
/////////////////////////////////////////////////
void Gui::progressBar (
  const float progress,
  const WidgetOptions& options)
{
  // initialize widget
  const auto name = initializeActivable ("ProgressBar");
  const auto position = computeRelativePosition (options.displacement);

  // draw progress bar and handle its state
  const auto size = options.size * textHeight ();
  const auto box = sf::FloatRect (position, size);
  itemStatus (box, name, false, options.tooltip);
  mRender.draw (box, {Widget::ProgressBar, Slices::Three});
  // draw progress bar filling
  auto drawOptions = WidgetDrawOptions (Widget::ProgressFill, Slices::Three);
  drawOptions.progress = sgui::clamp (0.f, 1.f, progress);
  mRender.draw (box, drawOptions);
  
  // draw text next to the progress bar
  const auto descrPos = position + sf::Vector2f (size.x + mPadding.x, 0.f);
  const auto descrSize = widgetDescription (descrPos, options.description);
  updateSpacing ({size.x + descrSize.x, size.y});
}


/////////////////////////////////////////////////
// to plot functions in gui
/////////////////////////////////////////////////
void Gui::setPlotRange (
  const PlotRange xRange,
  const PlotRange yRange)
{
  mPlotter.xRange = xRange;
  mPlotter.yRange = yRange;
}

/////////////////////////////////////////////////
void Gui::setSample (const uint32_t sample)
{
  mPlotSample = sample;
  mPlotter.setSample (sample);
}

/////////////////////////////////////////////////
void Gui::setPlotBound (const sf::Vector2f& bound)
{
  mPlotBound = bound;
  mPlotIsBounded = true;
}

/////////////////////////////////////////////////
void Gui::unsetPlotBound ()
{
  mPlotIsBounded = false;
}

/////////////////////////////////////////////////
void Gui::plot (
  const std::function<float (float)>& slope,
  const float thickness,
  const sf::Color& lineColor)
{
  // cache plot data to avoid useless computation
  if (!mPlotsData.has (mCounters.plot)) {
    cachePlotData (slope);
  }
  plot (mPlotsData.get (mCounters.plot), thickness, lineColor);
  mCounters.plot++;
}

/////////////////////////////////////////////////
void Gui::plot (
  const std::function<sf::Vector2f (float)>& slope,
  const float thickness,
  const sf::Color& lineColor)
{
  // cache plot data to avoid useless computation
  if (!mPlotsData.has (mCounters.plot)) {
    cachePlotData (slope);
  }
  plot (mPlotsData.get (mCounters.plot), thickness, lineColor);
  mCounters.plot++;
}

/////////////////////////////////////////////////
void Gui::plot (
  const std::vector<sf::Vector2f>& points,
  const float thickness,
  const sf::Color& lineColor)
{
  // keep cursor position as handlePlotBound will modify it
  const auto position = mCursorPosition;
  handlePlotBound ();
  mPlotter.plot (points, position, lineColor, thickness);
}

/////////////////////////////////////////////////
void Gui::forcePlotUpdate ()
{
  mPlotsData.clear ();
}

/////////////////////////////////////////////////
void Gui::cachePlotData (const std::function<float (float)>& slope)
{
  auto slopeData = std::vector <sf::Vector2f> ();
  const auto max = static_cast<float> (mPlotSample);
  for (int i = 0; i < static_cast<int> (mPlotSample); i++) {
    const auto& range = mPlotter.xRange;
    const auto x = sgui::lerp (range.min, range.max, i / max);
    slopeData.emplace_back (x, slope (x));
  }
  mPlotsData.emplace (mCounters.plot, std::move (slopeData));
}

/////////////////////////////////////////////////
void Gui::cachePlotData (const std::function<sf::Vector2f (float)>& slope)
{
  auto slopeData = std::vector <sf::Vector2f> ();
  const auto max = static_cast<float> (mPlotSample);
  for (int i = 0; i < static_cast<int> (mPlotSample); i++) {
    const auto& range = mPlotter.yRange;
    const auto x = sgui::lerp (range.min, range.max, i / max);
    slopeData.emplace_back (slope (x));
  }
  mPlotsData.emplace (mCounters.plot, std::move (slopeData));
}

/////////////////////////////////////////////////
void Gui::handlePlotBound ()
{
  // set plot bound, in a group we enforce a 16:9 ratio
  if (mPlotIsBounded) {
    if (!mGroups.empty ()) {
      auto width = mGroups.top ().size.x - 2.f*mPadding.x;
      if (mPlotBound.x > 0.f) {
        width = sgui::clamp (0.f, mPlotBound.x, width);
      }
      const auto bound = sf::Vector2f (width, width * 0.5625f);
      mPlotter.setBound (bound);
      updateSpacing (bound);
    } else {
      mPlotter.setBound (mPlotBound);
      updateSpacing (mPlotBound);
    }
  } else {
    if (!mGroups.empty ()) {
      const auto width = mGroups.top ().size.x - 2.f*mPadding.x;
      const auto bound = sf::Vector2f (width, width * 0.5625f);
      mPlotter.setBound (bound);
      updateSpacing (bound);
    } else {
      mPlotter.unsetBound ();
    }
  }
}


/////////////////////////////////////////////////
// combo box
/////////////////////////////////////////////////
std::string Gui::comboBox (
  const std::vector <std::string>& list,
  const WidgetOptions& options)
{
  // if list is empty we quit and do nothing
  if (list.empty ()) return "";

  // initialize widget name and position
  const auto comboBoxId = mCounters.comboBox;
  mCounters.comboBox++;
  const auto name = initializeActivable ("ComboBox");
  const auto mainBoxPosition = computeRelativePosition (options.displacement);
  mComboBoxActiveItem.emplace (comboBoxId, list.front ());
  mComboBoxClocks.emplace (comboBoxId, 0.f);

  // compute combo box width
  const auto defaultSize = textHeight ();
  auto maxWidth = 5.f*defaultSize;
  for (const auto& text : list) {
    maxWidth = std::max (maxWidth, textSize (text).x);
  }
  const auto itemWidth = maxWidth + defaultSize + 2.f*mPadding.x;
  const auto itemSize = sf::Vector2f (itemWidth, defaultSize);
  
  // compute if combo box is open
  const auto box = sf::FloatRect (mainBoxPosition, itemSize);
  const auto state = itemStatus (box, name, mInputState.mouseLeftDown);
  // we need this to reset combo box if no item was hovered in previous loop
  auto& clock = mComboBoxClocks.get (comboBoxId);
  if (state == ItemState::Active || state == ItemState::Hovered) {
    clock = std::min (clock, 0.f); // click is stronger than hovering
    // if we click, we want that comboBox stay open "indefinitely"
    if (state == ItemState::Active) {
      clock = -100000.f;
    }
    mGuiState.comboBoxFocus = name;
  }
  const auto isOpen = mGuiState.comboBoxFocus == name || (mGuiState.activeItem == name);

  // close combo box after 500 milliseconds if it is not hovered
  if (clock > 0.5f && (mGuiState.comboBoxFocus == name)) {
    mGuiState.comboBoxFocus = NullID;
  }

  // get selected text of the combo box
  auto& text = mComboBoxActiveItem.get (comboBoxId);
  // handle edge cases where a comboBox "takes" id of another
  if (std::find (list.begin (), list.end (), text) == list.end ()) {
    text = list.front ();
  }

  // compute each drop list item if combo box is active and not clipped
  auto icon = ICON_FA_SQUARE_CARET_DOWN;
  const auto initialPosition = mainBoxPosition + sf::Vector2f (0.f, itemSize.y);
  if (isOpen && !mRender.clipping.isClipped (initialPosition)) {
    const auto itemCount = std::min (std::size_t (6), list.size ());
    const auto panelSize = normalizeSize ({itemSize.x, static_cast <float> (itemCount)*itemSize.y});
    auto panel = Panel ({{}, panelSize}, true, false);
    // open the window that will contains the combo box item
    mCursorPosition = initialPosition;
    beginWindow (panel);
    mCursorPosition -= sf::Vector2f (mPadding.x, 2.5f*mPadding.y);
    const auto scrollerWidth = (list.size () > 6) ? defaultSize : 0.f;
    for (const auto& itemName : list) {
      // get item status and update selected value
      if (dropListItem (clock, text, itemName, {itemWidth - scrollerWidth, itemSize.y})) {
        clock = 1.f;
        text = itemName;
      }
    }
    endWindow ();
    mCursorPosition = mainBoxPosition;
    icon = ICON_FA_SQUARE_CARET_UP;
  }

  // draw combo box selected text
  mRender.draw (box, {Widget::ItemBox, Slices::Three, state});
  handleTextDrawing (box.position + mPadding, text);
  const auto iconPos = box.position + sf::Vector2f (5.f*defaultSize + 2.f*mPadding.x, 0.5f*mPadding.y);
  fontawesomeIcon (iconPos, icon, getFontSize (TextType::Normal) + 2u); 

  // return selected item and update cursor position
  updateSpacing (itemSize);
  return text;
}

/////////////////////////////////////////////////
bool Gui::dropListItem (
  float& clock,
  const std::string& selectedName,
  const std::string& itemName,
  const sf::Vector2f& itemSize)
{
  const auto name = initializeActivable ("DropListItem");

  // get item status
  const auto box = sf::FloatRect (mCursorPosition, itemSize);
  const auto state = itemStatus (box, name, mInputState.mouseLeftDown);
  const auto status = (state == ItemState::Active) || (mGuiState.activeItem == name);
  if (state == ItemState::Hovered) {
    clock = std::min (clock, 0.f);
  }

  // draw item
  if (selectedName == itemName) {
    mRender.draw (box, {Widget::ItemBox, Slices::Three, ItemState::Active});
  } else {
    mRender.draw (box, {Widget::ItemBox, Slices::Three, state});
  }
  handleTextDrawing (box.position + mPadding, itemName);
  mCursorPosition.y += itemSize.y;
  updateScrolling ();

  // return selection status
  return status;
}


/////////////////////////////////////////////////
// scroller for scrollable window/panel
/////////////////////////////////////////////////
bool Gui::scrollThroughPanel (
  Impl::GroupData& panel,
  const sf::FloatRect& panelBox,
  const ItemState panelState,
  const bool horizontal)
{
  if (isPanelScrollable (panel)) {
    // scroll through panel
    if (mGroupsScrollerData.has (panel.groupId)) {
      auto& scrollData = mGroupsScrollerData.get (panel.groupId);
      const auto size = scrollData.size ();
      const auto amount = scroller (scrollData.percent, panelBox, size,panelState, horizontal);
      scrollData.update (panelBox.position - amount);
      mCursorPosition -= amount;
    }
    // reduce group size to account for scroller bar
    if (horizontal) {
      panel.size.y -= textHeight ();
    } else {
      panel.size.x -= textHeight ();
    }
    return true;
  }
  return false;
}

/////////////////////////////////////////////////
bool Gui::isPanelScrollable (const Impl::GroupData& panel)
{
  // panel is scrollable if it possess a scroller
  if (mGroupsScrollerData.has (panel.groupId)) {
    auto& scrollData = mGroupsScrollerData.get (panel.groupId);
    // and if its scroller size is greater than the group size
    scrollData.isScrolled = (panel.horizontal && scrollData.size ().x > panel.size.x)
      || scrollData.size ().y > panel.size.y;
    if (!scrollData.isScrolled) {
      scrollData.percent = 0.f;
    }
    return scrollData.isScrolled;
  }
  // if panel does not possess a scroller, add one
  mGroupsScrollerData.emplace (panel.groupId);
  mGroupsScrollerData.get (panel.groupId).update (panel.position);
  return false;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::scroller (
  float& percent,
  const sf::FloatRect& panelBox,
  const sf::Vector2f& scrollSize,
  const ItemState panelState,
  const bool horizontal)
{
  const auto name = initializeActivable ("Scroller");

  // compute scroller position (right or bottom) and size
  auto pos = sf::Vector2f ();
  auto size = panelBox.size;
  auto extraSize = 2.f * mPadding.y;
  const auto defaultSize = textHeight ();
  if (horizontal) {
    pos = sf::Vector2f (panelBox.position.x - defaultSize, panelBox.position.y + panelBox.size.y);
    size.y = defaultSize;
    extraSize = extraSize + scrollSize.x - size.x;
  } else {
    pos = sf::Vector2f (panelBox.position.x + panelBox.size.x - defaultSize, panelBox.position.y);
    size.x = defaultSize;
    extraSize = extraSize + scrollSize.y - size.y;
  }

  // get scroller status, we always steal active state over the previous widget
  const auto box = sf::FloatRect (pos, size);
  auto state = itemStatus (box, name, mInputState.mouseLeftDown, Tooltip (), true);
  mRender.draw (box, {Widget::Scroller, Slices::Three, state, horizontal});

  // if active, compute current scrolling
  if (mGuiState.activeItem == name) {
    state = ItemState::Active;
    percent = sliderValue (box, 0.f, 1.f, horizontal);
  }

  // if parent is active and scroller is inactive (we can't drag and scroll), scroll with wheel
  const auto parentIsActive = panelState == ItemState::Hovered || panelState == ItemState::Active;
  const auto scrollerIsInactive = state != ItemState::Active;
  if (parentIsActive && scrollerIsInactive && mInputState.mouseScrolled) {
    // note that 0% is the top scrollbar, so if delta > 0 we need to subtract pixels
    auto length = box.size.y;
    if (horizontal) {
      length = box.size.x;
    }
    if (mInputState.mouseDeltaWheel > 0.f) {
      percent = sgui::clamp (0.f, 1.f, (percent*length - mPixelsPerScroll) / length);
    } else {
      percent = sgui::clamp (0.f, 1.f, (percent*length + mPixelsPerScroll) / length);
    }
  }

  // compute scrollbar extra shift
  percent = sgui::clamp (0.f, 1.f, percent);
  const auto shift = scrollerBar (box, state, percent, extraSize, horizontal);
  if (horizontal) {
    return sf::Vector2f (-shift, 0.f);
  }
  return sf::Vector2f (0.f, shift);
}

/////////////////////////////////////////////////
float Gui::sliderBar (
  const sf::FloatRect& parentBox,
  const ItemState state,
  const float percent,
  const bool horizontal)
{
  // compute shift length in parent box
  const auto shift = (parentBox.size.y - parentBox.size.x) * percent;

  // scrollbar is a square box so width = height = smallest side
  auto barBox = parentBox;
  if (horizontal) {
    barBox.position.x -= shift;
    barBox.size.x = barBox.size.y;
  } else {
    barBox.position.y += shift;
    barBox.size.y = barBox.size.x;
  }
  mRender.draw (barBox, {Widget::SliderBar, Slices::One, state});

  // return shift for further uses
  return shift;
}

/////////////////////////////////////////////////
float Gui::scrollerBar (
  const sf::FloatRect& parentBox,
  const ItemState state,
  const float percent,
  const float extraSize,
  const bool horizontal)
{
  // quit if extra size is marginal
  auto shift = 0.f;
  if (std::abs (extraSize) < 1.f) {
    return shift;
  }

  // scrollbar is a square box so width = height = smallest side
  auto barBox = parentBox;
  if (extraSize > 0.f) {
    const auto minSize = textHeight ();
    if (horizontal) {
      barBox.size.x = std::max (minSize, barBox.size.x - extraSize);
      shift = extraSize * percent;
      const auto barShift = (parentBox.size.x - barBox.size.x) * percent;
      const auto maxPosX = (parentBox.position + parentBox.size).x;
      barBox.position.x = std::min (barBox.position.x + barShift, maxPosX - barBox.size.x);
    } else {
      barBox.size.y = std::max (minSize, barBox.size.y - extraSize);
      shift = extraSize * percent;
      const auto barShift = (parentBox.size.y - barBox.size.y) * percent;
      const auto maxPosY = (parentBox.position + parentBox.size).y;
      barBox.position.y = std::min (barBox.position.y + barShift, maxPosY - barBox.size.y);
    }
  }
  mRender.draw (barBox, {Widget::ScrollerBar, Slices::Three, state, horizontal});

  // return shift for further uses
  return shift;
}


/////////////////////////////////////////////////
// begin or end group
/////////////////////////////////////////////////
void Gui::beginGroup (
  const bool horizontal,
  const sf::Vector2f& position,
  const sf::Vector2f& size)
{
  // construct a new group
  auto newGroup = Impl::GroupData ();
  newGroup.horizontal = horizontal;
  newGroup.position = position;
  newGroup.size = size;
  newGroup.lastItemPosition = position + 1.5f*mPadding;
  newGroup.menuItemCount = 0u;
  newGroup.clippingLayer = 0u;
  newGroup.plotterLayer = 0u;
  newGroup.menuBarSize = sf::Vector2f (0, 0);
  newGroup.groupId = std::hash <std::string> {} (mWidgetChain);
  mCounters.group++;

  // add it to the stack
  mGroups.emplace (std::move (newGroup));
}

/////////////////////////////////////////////////
void Gui::endGroup ()
{
  // check if there is in fact stacked group...
  if (mGroups.empty ()) {
    spdlog::warn ("Cannot remove group from empty stack in Gui::endGroup ()");
    return;
  }
  mGroups.pop ();
}


/////////////////////////////////////////////////
// window/panel utility functions
/////////////////////////////////////////////////
sf::Vector2f Gui::computePosition (
  const Panel& panel,
  const Constraints& constraint)
{
  // get parent box shift and size
  const auto parent = getParentGroup ();
  const auto windowSize = parentGroupSize ();
  
  // to compute constrained position
  auto pos = panel.position;
  const auto halfSize = panel.size / 2.f;
  const auto center = windowSize / 2.f;

  // constrain horizontal position with alignment
  auto parentShift = parent.position;
  const auto isAlignedLaterally = constraint.horizontal != HorizontalAlignment::None;
  if (isAlignedLaterally) {
    if (constraint.horizontal == HorizontalAlignment::Center) {
      pos.x = center.x - halfSize.x;
    }
    if (constraint.horizontal == HorizontalAlignment::Right) {
      pos.x = windowSize.x * (1.f - panel.size.x);
    }
    if (constraint.horizontal == HorizontalAlignment::Left) {
      pos.x = parent.position.x;
    }
    parentShift.x = 0.f;
  // fix element relative to window size
  } else {
    if (constraint.relativePosition.x > 0.01f) {
      pos.x = windowSize.x * constraint.relativePosition.x;
    }
  }

  // constraint vertical position
  const auto isAlignedVertically = constraint.vertical != VerticalAlignment::None;
  if (isAlignedVertically) {
    if (constraint.vertical == VerticalAlignment::Center) {
      pos.y = center.y - halfSize.y;
    }
    if (constraint.vertical == VerticalAlignment::Bottom) {
      pos.y = windowSize.y * (1.f - panel.size.y);
    }
    if (constraint.vertical == VerticalAlignment::Top) {
      pos.y = parent.position.y;
    }
    parentShift.y = 0.f;
  // fix element relative to window size
  } else {
    if (constraint.relativePosition.y > 0.01f) {
      pos.y = windowSize.y * constraint.relativePosition.y;
    }
  }

  // if there are no constraints or position set, return cursor position
  if (pos.lengthSquared () < 0.01f && !isAlignedVertically && !isAlignedLaterally) {
    return mCursorPosition;
  }

  // else return constrained position
  return pos + parentShift;
}

/////////////////////////////////////////////////
sf::FloatRect Gui::handleParentClipBox (const sf::FloatRect& box)
{
  // return box if there is no parent
  if (mGroups.empty ()) {
    return box;
  }
  // If there are no intersection, return null size
  auto clipBox = box;
  const auto& parent = mGroups.top ();
  if (box.findIntersection ({parent.position, parent.size}) == std::nullopt) {
    clipBox.size = sf::Vector2f ();
    return clipBox;
  }
  // compute minimal clamped version of the child box
  // clamp size
  const auto bottomRight = box.position + box.size;
  const auto parentBottomRight = parent.position + parent.size;
  const auto newBottomRight = sf::Vector2f (
    std::min (bottomRight.x, parentBottomRight.x),
    std::min (bottomRight.y, parentBottomRight.y)
  );
  // clamp position
  clipBox.position = sf::Vector2f (
    std::max (box.position.x, parent.position.x),
    std::max (box.position.y, parent.position.y)
  );
  clipBox.size = newBottomRight - clipBox.position;
  return clipBox;
}

/////////////////////////////////////////////////
void Gui::removeClipping ()
{
  // go back to no clipping or to previous clipping
  if (!mGroups.empty ()) {
    const auto& group = mGroups.top ();
    mRender.clipping.moveToLayer (group.clippingLayer);
    mPlotter.render.clipping.moveToLayer (group.plotterLayer);
  } else {
    mRender.clipping.disable ();
    mPlotter.render.clipping.disable ();
  }
}

/////////////////////////////////////////////////
ItemState Gui::interactWithMouse (
  Panel& settings,
  const sf::FloatRect& box,
  const std::string& name,
  const Tooltip& tooltip)
{
  // move panel according to mouse displacement
  const auto leftClick = mInputState.mouseLeftDown;
  const auto state = itemStatus (box, name, leftClick, tooltip);
  const auto isActive = mGuiState.activeItem == name;
  if (isActive && settings.movable) {
    settings.position.x += mInputState.mouseDisplacement.x;
    settings.position.y += mInputState.mouseDisplacement.y;
  }
  return state;
}


/////////////////////////////////////////////////
// to compute item status (active, hovered, neutral)
/////////////////////////////////////////////////
ItemState Gui::itemStatus (
  const sf::FloatRect& boundingBox,
  const std::string& item,
  bool condition,
  const Tooltip& tooltip,
  bool forceActive)
{
  auto state = ItemState::Neutral;

  // if mouse collide with the boundingBox
  if (boundingBox.contains (mInputState.mousePosition)
  && !mRender.clipping.isClipped (mInputState.mousePosition)) {
    // enter hovered state
    state = ItemState::Hovered;
    mGuiState.hoveredItem = item;

    // if no widget is active, enter active state
    if ((mGuiState.activeItem == NullID || forceActive) && condition) {
      // we want to store item ID to move it, but we don't want infinite click
      mGuiState.activeItem = item;
      if (mInputState.updated) {
        state = ItemState::Active;
      }
    }

    // update tooltip info
    if (tooltip.active) {
      mGuiState.hoveredItemBox = boundingBox;
      mGuiState.tooltip = tooltip;
      mGuiState.tooltip.parent = item;
    }
  }
  playSound (state);
  return state;
}

/////////////////////////////////////////////////
void Gui::playSound (const ItemState state)
{
  const auto isPanel = mActiveWidgetSoundId == "Panel";
  const auto isWindow = mActiveWidgetSoundId == "Window";
  const auto isMenu = mActiveWidgetSoundId == "MenuBar";
  const auto noRepetition = mActiveWidgetSoundId != mPreviousWidgetSoundId;
  const auto isValidSound = mSoundIsOn && !isWindow && !isPanel && !isMenu && noRepetition;
  if (mInputState.updated && state == ItemState::Active && isValidSound) {
    mSoundPlayer.play (mActiveWidgetSoundId);
  }
}


/////////////////////////////////////////////////
// to handle text related stuff
/////////////////////////////////////////////////
void Gui::handleKeyInput (
  std::string& text,
  size_t& textCursorIndex)
{
  // we only handle key if inputs were updated
  if (!mInputState.updated) return;

  // convert key into utf8 character
  std::string character ("");
  if (mInputState.textIsEntered) {
    sf::Utf<8>::encode (mInputState.keyPressed, std::back_inserter (character));
    // erase character
    if (character == "\b") {
      if (!text.empty ()) {
        // erase last utf-8 character by looking for continuation bytes
        auto cp = text.data () + textCursorIndex;
        while (--cp >= text.data () && ((*cp & 0x80) && !(*cp & 0x40))) {}
        if (cp >= text.data ()) {
          const auto afterCharacter = text.substr (textCursorIndex);
          text.resize (cp - text.data ());
          text += afterCharacter;
          textCursorIndex = cp - text.data ();
        }
      }
      textCursorIndex = std::min (textCursorIndex, text.length ());
    // add character to the text
    } else if (mInputState.keyPressed != U'\u000D') {
      if (textCursorIndex == text.length ()) {
        text += character;
        textCursorIndex = text.length ();
      } else {
        const auto beforeCursor = text.substr (0, textCursorIndex);
        const auto afterCursor = text.substr (textCursorIndex);
        text = beforeCursor + character + afterCursor;
        textCursorIndex += character.length ();
      }
    }
  }

  // move text cursor
  if (mInputState.keyIsPressed) {
    if (mInputState.code == sf::Keyboard::Key::Left) { // to left
      textCursorIndex = clamp (size_t (0), text.length (), textCursorIndex - 1);
    } else if (mInputState.code == sf::Keyboard::Key::Right) { // to right
      textCursorIndex = clamp (size_t (0), text.length (), textCursorIndex + 1);
    } else if (mInputState.code == sf::Keyboard::Key::Home) { // to start
      textCursorIndex = 0;
    } else if (mInputState.code == sf::Keyboard::Key::End) { // to end
      textCursorIndex = text.length ();
    }
  }

}

/////////////////////////////////////////////////
size_t Gui::utf8Length (const std::string& text) const
{
  size_t size = 0;
  for (size_t i = 0; i < text.length (); i++) {
    size++;
    auto c = static_cast <unsigned char> (text [i]);
    if      (c >= 0 && c <= 127) { i += 0; }
    else if ((c & 0xE0) == 0xC0) { i += 1; }
    else if ((c & 0xF0) == 0xE0) { i += 2; }
    else if ((c & 0xF8) == 0xF0) { i += 3; }
    else { return size; }
  }
  return size;
}

/////////////////////////////////////////////////
void Gui::handleTextDrawing (
  const sf::Vector2f& position,
  const std::string& text,
  const TextType type)
{
  const auto fontSize = getFontSize (type);
  // searches fontawesome unicode in the string, delimited by "|" pair
  const auto firstMarkerPos = text.find ("|");
  if (firstMarkerPos != std::string::npos) {
    const auto secondMarkerPos = text.find ("|", firstMarkerPos + 1);
    if (secondMarkerPos != std::string::npos) {
      auto firstPart = std::string ("");
      if (firstMarkerPos > 0) {
        firstPart = text.substr (0, firstMarkerPos - 1);
      }
      // draw text without icon
      const auto textWithoutIcon = firstPart + "    " + text.substr (secondMarkerPos + 1);
      mRender.draw (textWithoutIcon, *mFont, {sgui::round (position), mStyle.fontColor, fontSize});
      // draw fontawesome icon at the right place
      const auto firstPartSize = textSize (firstPart, type);
      const auto iconPos = position + sf::Vector2f (firstPartSize.x + mPadding.x, 0.f);
      const auto faIcon = text.substr (firstMarkerPos + 1, secondMarkerPos - firstMarkerPos - 1);
      fontawesomeIcon (iconPos, faIcon, fontSize);
      return;
    }
  }

  // render plain text if fontawesome is not set or no icons are found
  mRender.draw (text, *mFont, {sgui::round (position), mStyle.fontColor, fontSize});
}

/////////////////////////////////////////////////
void Gui::fontawesomeIcon (
  const sf::Vector2f& position,
  const std::string& icon,
  const uint32_t fontSize)
{
  mRender.draw (icon, mFontawesome, {sgui::round (position), mStyle.fontColor, fontSize});
}

/////////////////////////////////////////////////
uint32_t Gui::getFontSize (const TextType type) const
{
  if (type == TextType::Title) {
    return mStyle.fontSize.title;
  }
  if (type == TextType::Subtitle) {
    return mStyle.fontSize.subtitle;
  }
  if (type == TextType::Footnote) {
    return mStyle.fontSize.footnote;
  }
  return mStyle.fontSize.normal;
}

/////////////////////////////////////////////////
std::vector<std::string> Gui::formatText (
  const std::string& input,
  const sf::Vector2f& boxSize,
  const TextType type) const
{
  auto formattedText = std::vector <std::string> ();
  // if input is contrained by a box
  if (boxSize.lengthSquared () > 0.01f) {
    // read the string word by word
    auto in = std::istringstream (input);
    auto word = std::string ("");
    auto line = std::string ("");
    while (in >> word) {
      // add a new line if current word outpass box boundaries
      const auto lineWidth = textSize (line + word + " ", type).x;
      if (lineWidth >= 0.98f*boxSize.x) {
        formattedText.emplace_back (line);
        line = word + " ";
      } else {
        line += word + " ";
      }
    }
    formattedText.emplace_back (line);
    return formattedText;
  }
  formattedText.emplace_back (input);
  return formattedText;
}


/////////////////////////////////////////////////
// to compute automatic position of widget
/////////////////////////////////////////////////
std::string Gui::initializeActivable (const std::string& key)
{
  mCounters.widget++;
  mWidgetChain += key.front ();
  mActiveWidgetSoundId = key;
  const auto name = key + mWidgetChain + std::to_string (mCounters.widget);
  // use hash every where instead of string ? Not sure it is worth it...
  // const auto hash = std::hash <std::string> {} (name);
  return name;
}

/////////////////////////////////////////////////
void Gui::updateSpacing (const sf::Vector2f& size)
{
  // compute spacing added
  mLastSpacing = size + 2.f*mPadding;
  
  // if group is horizontal, add spacing along x, except if same line was called.
  if (!mGroups.empty () && getParentGroup ().horizontal) {
    mResetCursorPosition = mCursorPosition + sf::Vector2f (0.f, size.y + mPadding.y);
    if (mSameLinePosition.empty ()) {
      mCursorPosition += mLastSpacing;
      updateScrolling ();
      mCursorPosition.y -= mLastSpacing.y;
    } else {
      const auto currentX = mCursorPosition.x + mLastSpacing.x;
      mCursorPosition = mSameLinePosition.top ();
      mCursorPosition.x = std::max (currentX, mCursorPosition.x);
      mSameLinePosition.pop ();
      updateScrolling ();
    }
  // if group is vertical, add spacing along y, except if same line was called
  } else {
    mResetCursorPosition = mCursorPosition + sf::Vector2f (size.x + mPadding.x, 0.f);
    if (mSameLinePosition.empty ()) {
      mCursorPosition += mLastSpacing;
      updateScrolling ();
      mCursorPosition.x -= mLastSpacing.x;
    } else {
      const auto currentY = mCursorPosition.y + mLastSpacing.y;
      mCursorPosition = mSameLinePosition.top ();
      mCursorPosition.y = std::max (currentY, mCursorPosition.y);
      mSameLinePosition.pop ();
      updateScrolling ();
    }
  }
}

/////////////////////////////////////////////////
void Gui::updateScrolling ()
{
  // update current group scrolling size
  if (!mGroups.empty ()) {
    const auto groupId = mGroups.top ().groupId;
    if (mGroupsScrollerData.has (groupId)) {
      mGroupsScrollerData.get (groupId).computeScrollSize (mCursorPosition);
    }
  }
}

/////////////////////////////////////////////////
Impl::GroupData Gui::getParentGroup () 
{
  // if there are no parent, return a default group
  if (mGroups.empty ()) {
    return Impl::GroupData ();
  }

  // if parent is a menu we skip it
  auto parent = mGroups.top ();
  if (parent.menuItemCount != 0) {
    mGroups.pop ();
    if (!mGroups.empty ()) {
      auto menu = parent;
      parent = mGroups.top ();
      mGroups.emplace (std::move (menu));
    } else {
      mGroups.push (parent);
    }
  }
  return parent;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::computeRelativePosition (const sf::Vector2f& displacement) const
{
  // If there is no active group, displacement = position
  const auto isNotNull = displacement.lengthSquared () > 0.01f;
  if (mGroups.empty () && isNotNull) {
    return displacement;
  }

  // If there is an active group, displace relatively to the group position
  if (!mGroups.empty () && isNotNull) {
    const auto& parent = mGroups.top ();
    return parent.position + displacement;
  }

  // If there are no displacement, return initial position (that
  return mCursorPosition;
}

/////////////////////////////////////////////////
bool Gui::isValid (const Widget widget) const
{
  return widget != Widget::None;
}

/////////////////////////////////////////////////
bool Gui::isValid (const Slices slices) const
{
  return slices != Slices::Default;
}

/////////////////////////////////////////////////
bool Gui::isValid (const ItemState state) const
{
  return state != ItemState::None;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::widgetDescription (
  const sf::Vector2f& position,
  const std::string& description)
{
  if (description != "") {
    handleTextDrawing (position + 1.5f*mPadding, description);
    return textSize (description) + 1.5f*mPadding;
  }
  return {};
}

} // namespace sgui
