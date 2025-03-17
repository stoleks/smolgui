#include "Gui.h"

#include <string>
#include <sstream>
#include <iostream>

#include "sgui/Core/Interpolation.h"
#include "sgui/Resources/Layout.h"
#include "sgui/Resources/TextContainer.h"

namespace sgui 
{
/**
 * ----------------------------------------------
 * Initialization of Gui
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
Gui::Gui ()
  : mRender ()
{
  // set internal state
  mGuiState.keyboardFocus = NullItemID;
  mGuiState.activeItem = NullItemID;
  mGuiState.hoveredItem = NullItemID;

  // define default style
  mStyle.itemSpacing = mStyle.fontSize.normal / 2.5f;
  mStyle.itemInnerSpacing = mStyle.fontSize.normal / 3.f;
  mPadding = 0.25f * mStyle.fontSize.normal * sf::Vector2f (2.f, 0.5f);
}

/////////////////////////////////////////////////
void Gui::setResources (
  sf::Font& font,
  sf::Texture& widgetTexture,
  const TextureAtlas& widgetAtlas)
{
  mRender.setResources (font, widgetTexture, widgetAtlas);
}

/////////////////////////////////////////////////
void Gui::setStyle (const Style& newStyle)
{
  mStyle = newStyle;
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


/**
 * ----------------------------------------------
 *  to ease alignement
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
sf::Vector2f Gui::normalSizeOf (const std::string& text) const
{
  return mRender.textSize (text, mStyle.fontSize.normal);
}

/////////////////////////////////////////////////
sf::Vector2f Gui::titleSizeOf (const std::string& text) const
{
  return mRender.textSize (text, mStyle.fontSize.title);
}

/////////////////////////////////////////////////
sf::Vector2f Gui::subtitleSizeOf (const std::string& text) const
{
  return mRender.textSize (text, mStyle.fontSize.subtitle);
}

/////////////////////////////////////////////////
sf::Vector2f Gui::footnoteSizeOf (const std::string& text) const
{
  return mRender.textSize (text, mStyle.fontSize.footnote);
}

/////////////////////////////////////////////////
const FontSize& Gui::fontSize () const
{
  return mStyle.fontSize;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::buttonSize () const
{
  return buttonHeight () * sf::Vector2f (1.f, 1.f);
}

/////////////////////////////////////////////////
sf::Vector2f Gui::textSize () const
{
  return 1.f*mStyle.fontSize.normal * sf::Vector2f (1.f, 1.f);
}

/////////////////////////////////////////////////
sf::Vector2f Gui::titleSize () const
{
  return 1.f*mStyle.fontSize.title * sf::Vector2f (1.f, 1.f);
}

/////////////////////////////////////////////////
sf::Vector2f Gui::activePanelSize () const
{
  const auto thickness = 0.75f * normalTextHeight ();
  if (!mGroups.empty ()) {
    const auto& parent = mGroups.top ();
    if (parent.horizontal) {
      return sf::Vector2f (thickness, parent.size.y);
    }
    return sf::Vector2f (parent.size.x, thickness);
  }
  return sf::Vector2f (mWindowSize.x, thickness);
}


/**
 * ----------------------------------------------
 *  function to manage cursor position
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::setAnchor ()
{
  // store cursor position
  mAnchors.push (mCursorPosition);

  // store current scroll size
  if (!mGroups.empty ()) {
    const auto groupId = mGroups.top ().groupId;
    if (mGroupsScrollerInformation.has (groupId)) {
      const auto& scrollData = mGroupsScrollerInformation.get (groupId);
      const auto size = scrollData.currentSize ();
      mAnchorsScroll.push (size);
    }
  }
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

  // get back to previous scroll size
  if (!mGroups.empty ()) {
    const auto groupId = mGroups.top ().groupId;
    if (mGroupsScrollerInformation.has (groupId)) {
      auto& scrollData = mGroupsScrollerInformation.get (groupId);
      scrollData.setScrollSize (mAnchorsScroll.top ());
      mAnchorsScroll.pop ();
    }
  }
}

/////////////////////////////////////////////////
void Gui::addVerticalSpacing (const float amount)
{
  const auto spacing = amount * sf::Vector2f (0.f, mStyle.fontSize.normal);
  mCursorPosition += spacing;
  updateScrolling (spacing);
}

/////////////////////////////////////////////////
void Gui::addHorizontalSpacing (const float amount)
{
  const auto spacing = amount * sf::Vector2f (mStyle.fontSize.normal, 0.f);
  mCursorPosition += spacing;
  updateScrolling (spacing);
}

/////////////////////////////////////////////////
void Gui::addSpacing (const sf::Vector2f& amount)
{
  const auto spacing = static_cast<float> (mStyle.fontSize.normal) * amount;
  mCursorPosition += spacing;
  updateScrolling (spacing);
}

/////////////////////////////////////////////////
void Gui::addLastSpacing (const float amount)
{
  // add h/v spacing according to parent group
  if (!mGroups.empty ()) {
    // if group is a menu we skip it
    auto topGroup = mGroups.top ();
    if (topGroup.menuItemCount != 0) {
      mGroups.pop ();
      if (!mGroups.empty ()) {
        auto menu = topGroup;
        topGroup = mGroups.top ();
        mGroups.emplace (std::move (menu));
      } else {
        mGroups.push (topGroup);
      }
    }
    // remove relevant last spacing
    if (topGroup.horizontal) {
      addLastHorizontalSpacing (amount);
    } else {
      addLastVerticalSpacing (amount);
    }
  } else {
    addLastVerticalSpacing (amount);
  }
}

/////////////////////////////////////////////////
void Gui::addLastHorizontalSpacing (const float amount)
{
  const auto spacing = amount * sf::Vector2f (mLastSpacing.x, 0.f);
  mCursorPosition += spacing;
  updateScrolling (spacing);
}

/////////////////////////////////////////////////
void Gui::addLastVerticalSpacing (const float amount)
{
  const auto spacing = amount * sf::Vector2f (0.f, mLastSpacing.y);
  mCursorPosition += spacing;
  updateScrolling (spacing);
}

/////////////////////////////////////////////////
void Gui::sameLine ()
{
  mResetCount++;
  addLastVerticalSpacing (-1.f);
  addLastHorizontalSpacing ();
}

/////////////////////////////////////////////////
void Gui::sameColumn ()
{
  mResetCount++;
  addLastHorizontalSpacing (-1.f);
  addLastVerticalSpacing ();
}

/////////////////////////////////////////////////
sf::Vector2f Gui::cursorPosition () const
{
  return mCursorPosition;
}


/**
 * ----------------------------------------------
 * To know if gui is hovered
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
bool Gui::isActive () const
{
  return mGuiState.hoveredItem != NullItemID
    || mGuiState.activeItem != NullItemID
    || mGuiState.keyboardFocus != NullItemID;
}


/**
 * ----------------------------------------------
 * Gui state update
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::beginFrame ()
{
  // clear all widgets
  mRender.clear ();
  mPlotter.clear ();

  // reset cursor position
  mCursorPosition = sf::Vector2f (0.f, 0.f);

  // clear hovered item
  mGuiState.hoveredItem = NullItemID;
}

/////////////////////////////////////////////////
void Gui::endFrame (const float tooltipDelay)
{
  testPassCount++;
  // display active tooltip
  tooltip (tooltipDelay);

  // if left button is not pressed there is no active item
  if (!mInputState.mouseLeftDown) {
    mGuiState.activeItem = NullItemID;
  }
  // if right button is released we lost all focus
  if (mInputState.mouseRightReleased) {
    mGuiState.activeItem = NullItemID;
    mGuiState.keyboardFocus = NullItemID;
  }

  // reset same line counter
  mResetCount = 0u;
  mPreviousResetCount = 0u;
  // reset widget count and scroll data
  mPlotCount = 0u;
  mGroupCount = 0u;
  mWidgetCount = 0u;
  for (auto& scrollData : mGroupsScrollerInformation) {
    scrollData.newCycle ();
  }

  // inform user if they didn't closed correctly something
  checkEqualToZero (mBeginWindowCount, "beginWindow", "endWindow");
  checkEqualToZero (mBeginPanelCount, "beginPanel", "endPanel");
  checkEqualToZero (mBeginMenuCount, "beginMenu", "endMenu");

  // manage all ill-closed group and anchors
  checkItsEmpty (mGroups, "beginGroup", "endGroup");
  checkItsEmpty (mAnchors, "setAnchor", "backToAnchor");
  while (!mAnchorsScroll.empty ()) {
    mAnchorsScroll.pop ();
  }

  // reset inputs
  if (!mInputState.updated) {
    mInputState.mouseDisplacement = sf::Vector2f ();
  }
  mInputState.oldMousePosition = mInputState.mousePosition;
  mInputState.updated = false;
}

/////////////////////////////////////////////////
void Gui::checkEqualToZero (
  const uint32_t counter,
  const std::string& open,
  const std::string& close)
{
  if (counter > 0u) {
    spdlog::error ("A {} was called without its {} counterpart !", open, close);
  }
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
  // manage standard key value
  if (event->is <sf::Event::TextEntered> ()) {
    auto key = event->getIf <sf::Event::TextEntered> ();
    if (key) {
      mInputState.keyPressed = key->unicode;
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
void Gui::updateTimer (const sf::Time& deltaT)
{
  const auto dt = deltaT.asSeconds ();
  mTipAppearClock += dt;
  mTipDisappearClock += dt;
}

/////////////////////////////////////////////////
void Gui::draw (sf::RenderWindow& window)
{
  // set view to standard gui view
  const auto windowView = window.getView ();
  const auto guiView = window.getDefaultView ();
  window.setView (guiView);
  // draw widgets and plot
  mRender.updateView (guiView);
  window.draw (mRender);
  mPlotter.draw (window);
  // return to normal window view
  window.setView (windowView);
}

/////////////////////////////////////////////////
uint32_t Gui::drawCalls () const
{
  return mRender.drawCalls () + 1;
}


/**
 * ----------------------------------------------
 * begin/end anchor, menu, window or box
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
bool Gui::beginWindow (
  Panel& settings,
  const std::string& title,
  const Constraint& constraint,
  const bool hasMenu,
  const bool horizontal,
  const Tooltip& info)
{
  // assign unique id to the widget
  const auto name = "Window" + std::to_string (mWidgetCount);
  mWidgetCount++;
  mBeginWindowCount++;

  // compute position and create a new global group
  const auto position = computePosition (settings, constraint);
  beginGroup (horizontal, position, settings.size);
  auto& thisWindow = mGroups.top ();

  // if window is closed skip everything
  if (settings.closed) return false;

  // handle mouse interaction
  const auto activeSize = sf::Vector2f (settings.size.x, titleTextHeight ());
  const auto closeSize = sf::Vector2f (1.7f*(activeSize.y) + mPadding.x, 0.f);
  const auto activeBox = sf::FloatRect (position, activeSize - closeSize);
  const auto drawBox = sf::FloatRect (position, activeSize);
  setClipping (drawBox, 0.f);
  const auto state = interactWithMouse (settings, activeBox, name, info);

  // draw title box and window name
  mRender.draw <Widget::TitleBox> (drawBox, state);
  const auto textWidth = titleSizeOf (title).x;
  const auto shiftX = (activeBox.size.x - textWidth) / 2.f;
  mRender.drawText (
    sanitizePosition (position + sf::Vector2f (shiftX, 0.f)),
    title,
    mStyle.fontColor,
    mStyle.fontSize.title
  );

  // reduce or close window
  if (settings.closable) {
    // reduce
    const auto buttonSize = activeSize.y * sf::Vector2f (1, 1);
    mCursorPosition = position + sf::Vector2f (activeBox.size.x, 0.f);
    if (button <Widget::TitleButton> (buttonSize)) {
      settings.reduced = !(settings.reduced);
    }
    addLastVerticalSpacing (-1.f);
    icon ("reduce", buttonSize);

    // close
    addLastVerticalSpacing (-1.f);
    addLastHorizontalSpacing ();
    addHorizontalSpacing (-0.5f);
    if (button <Widget::TitleButton> (buttonSize)) {
      settings.closed = true;
    }
    addLastVerticalSpacing (-1.f);
    icon ("close", buttonSize);
  }
  mCursorPosition = position + sf::Vector2f (0.f, activeSize.y);

  // if window is reduced skip box drawing
  if (settings.reduced) return false;

  // set clipping layer
  auto& groupLayer = thisWindow.clippingLayer;
  const auto activeShift = sf::Vector2f (0.f, activeSize.y);
  const auto panelBox = sf::FloatRect (position + activeShift, settings.size);
  if (hasMenu) {
    groupLayer = setClipping (panelBox, 0.f, buttonHeight ());
  } else {
    groupLayer = setClipping (panelBox, 0.f);
  }

  // update cursorPosition and draw menu bar if needed
  if (hasMenu) {
    thisWindow.menuBarPosition = mCursorPosition;
    thisWindow.menuBarSize = sf::Vector2f (settings.size.x, buttonHeight ());
    thisWindow.hasMenuBar = hasMenu;
    // update cursor position
    mCursorPosition.y += buttonHeight ();
  }

  // draw window box and handle hovering of the window
  const auto winBox = sf::FloatRect (mCursorPosition, settings.size);
  const auto winStatus = itemStatus (winBox, name);
  mRender.draw <Widget::Box> (winBox, ItemState::Neutral);
  mCursorPosition += mPadding;
  mCursorPosition.y += 1.5f * mPadding.y;

  // scroll through window if requested
  if (isPanelScrollable (thisWindow)) {
    mCursorPosition -= scrollPanel (thisWindow.groupId, winBox, winStatus, horizontal);
    // reduce group size to account scroller
    if (horizontal) {
      thisWindow.size.y -= normalTextHeight ();
    } else {
      thisWindow.size.x -= normalTextHeight ();
    }
  }
  thisWindow.innerPosition = mCursorPosition;
  return true;
}

/////////////////////////////////////////////////
void Gui::endWindow ()
{
  // update cursor position and last spacing
  if (!mGroups.empty ()) {
    // end group
    const auto active = mGroups.top ();
    endGroup ();

    // go back to no clipping or to previous clipping
    if (!mGroups.empty ()) {
      const auto id = mGroups.top ().clippingLayer;
      mRender.moveToClippingLayer (id);
    } else {
      mRender.noClipping ();
    }

    // update cursor position and spacing
    mCursorPosition = active.position + computeSpacing (active.size);

    // track window not closed by user
    mBeginWindowCount--;
  } else {
    spdlog::warn ("There are no window to end");
  }
}

/////////////////////////////////////////////////
void Gui::beginPanel (
  Panel& settings,
  const Constraint& constraint,
  const bool clipped,
  const bool horizontal,
  const Tooltip& info)
{
  // assign unique id to the widget
  const auto name = "Panel" + std::to_string (mWidgetCount);
  mWidgetCount++;
  mBeginPanelCount++;

  // compute position and create a new group
  auto position = computePosition (settings, constraint);
  beginGroup (horizontal, position, settings.size);

  // add clipping layer for the panel box
  const auto panelBox = sf::FloatRect (position, settings.size);
  const auto panelStatus = itemStatus (panelBox, name);
  auto& panel = mGroups.top ();
  if (clipped) {
    auto& groupLayer = panel.clippingLayer;
    groupLayer = setClipping (panelBox);
  }

  // draw panel box if requested
  const auto state = interactWithMouse (settings, panelBox, name, info);
  if (settings.visible) {
    mRender.draw <Widget::Box> (panelBox, state);
  }

  // update cursor position
  mCursorPosition = position + mPadding;
  mCursorPosition.y += 1.5f * mPadding.y;

  // scroll through panel if requested
  if (isPanelScrollable (panel)) {
    mCursorPosition -= scrollPanel (panel.groupId, panelBox, state, horizontal);
    if (horizontal) {
      panel.size.y -= normalTextHeight ();
    } else {
      panel.size.x -= normalTextHeight ();
    }
  }
  panel.innerPosition = mCursorPosition;
}

/////////////////////////////////////////////////
void Gui::endPanel ()
{
  if (!mGroups.empty ()) {
    // end group
    const auto active = mGroups.top ();
    endGroup ();

    // go back to no clipping or to previous clipping
    if (!mGroups.empty ()) {
      const auto id = mGroups.top ().clippingLayer;
      mRender.moveToClippingLayer (id);
    } else {
      mRender.noClipping ();
    }

    // update cursor position and spacing
    mCursorPosition = active.position + computeSpacing (active.size);

    // track box not closed by user
    mBeginPanelCount--;
  } else {
    spdlog::warn ("There are no panel to end");
  }
}



/**
 * ----------------------------------------------
 * Menu related widget
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::beginMenu ()
{
  // menu cannot be called without parent
  if (mGroups.empty () && !mGroups.top ().hasMenuBar) {
    spdlog::error ("Menu require a parent window or panel");
  }

  // assign unique id to the widget
  const auto name = "MenuBar" + std::to_string (mWidgetCount);
  mWidgetCount++;
  mBeginMenuCount++;

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
  thisMenu.clippingLayer = parent.clippingLayer;

  // get menu bar status
  const auto box = sf::FloatRect (menuPos, menuSize);
  mRender.draw <Widget::MenuBox> (box, ItemState::Neutral);

  // scroll through panel if requested
  auto& panel = mGroups.top ();
  if (isPanelScrollable (panel)) {
    const auto panelStatus = itemStatus (box, name);
    mCursorPosition -= scrollPanel (panel.groupId, box, panelStatus, true);
  }
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

    // end group
    endGroup ();

    // track menu not closed by user
    mBeginMenuCount--;
  } else {
    spdlog::warn ("There are no menu to end");
  }
}

/////////////////////////////////////////////////
bool Gui::menuItem (
  const std::string& text,
  const Tooltip& info)
{
  // assign unique id to the widget
  auto& parentMenu = mGroups.top ();
  const auto itemId = parentMenu.menuItemCount;
  const auto name = "MenuItem" + std::to_string (mWidgetCount);
  parentMenu.menuItemCount++;
  mWidgetCount++;

  // compute text position
  const auto itemPos = parentMenu.lastItemPosition;
  const auto pos = itemPos + 1.5f*mPadding;

  // construct menu item box
  const auto width = 3.f*mPadding.x + subtitleSizeOf (text).x;
  const auto height = subtitleTextHeight ();
  const auto box = sf::FloatRect (itemPos, sf::Vector2f (width, height));
  parentMenu.lastItemPosition =
    itemPos + sf::Vector2f (width + mPadding.x, 0.f);

  // ensure that we are on the right clipping layer
  const auto layerId = mRender.currentClippingLayer ();
  mRender.moveToClippingLayer (parentMenu.clippingLayer);

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
  mRender.draw <Widget::MenuItemBox> (box, state);
  parentMenu.isActive = clicked;

  // draw a text over it
  mRender.drawText (
    sanitizePosition (pos),
    text,
    mStyle.fontColor,
    mStyle.fontSize.subtitle
  );

  // go back to previous clipping layer
  mRender.moveToClippingLayer (layerId);

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


/**
 * ----------------------------------------------
 * Logical agencement
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::separation ()
{
  // compute line position and size
  const auto position = mCursorPosition + mPadding.y*sf::Vector2f (1.f, 0.f);
  const auto size = activePanelSize () - 3.f*mPadding;
  const auto box = sf::FloatRect (position, size);

  // render line
  mRender.draw <Widget::Separation> (box, ItemState::Neutral);
  updateSpacing (size);
}


/**
 * ----------------------------------------------
 * Button related widget
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
bool Gui::textButton (
  const std::string& text,
  const Tooltip& info,
  const sf::Vector2f& position)
{
  // compute text position
  auto pos = mCursorPosition + 1.5f*mPadding;
  computeRelativePosition (pos, position);

  // construct a button adapted to the text
  const auto height = buttonHeight ();
  const auto size = height * sf::Vector2f (6.f, 1.f);
  const auto clicked = button <Widget::TextButton> (size, info, position);

  // draw a text over it
  mRender.drawText (
    sanitizePosition (pos),
    text,
    mStyle.fontColor,
    mStyle.fontSize.normal
  );
  return clicked;
}

/////////////////////////////////////////////////
bool Gui::iconButton (
  const IconID& iconName,
  const sf::Vector2f& size,
  const Tooltip& info,
  const sf::Vector2f& position)
{
  // compute text position
  auto pos = mCursorPosition;
  computeRelativePosition (pos, position);

  // construct a button adapted to the icon
  const auto clicked = button <Widget::IconButton> (size, info, position);

  // draw an icon over it
  mRender.drawIcon (sf::FloatRect (pos, size), iconName);
  return clicked;
}

/////////////////////////////////////////////////
bool Gui::iconTextButton (
  const IconID& iconName,
  const std::string& text,
  const Tooltip& info,
  const sf::Vector2f& position)
{
  // compute text position
  auto pos = mCursorPosition;
  computeRelativePosition (pos, position);

  // construct a button adapted to the icon
  const auto height = buttonHeight ();
  const auto size = height * sf::Vector2f (6.f, 1.f);
  const auto clicked =
    button <Widget::IconTextButton> (size, info, position);

  // draw an icon over it
  pos.x += 0.5f * mPadding.x;
  mRender.drawIcon (sf::FloatRect (pos, buttonSize ()), iconName);

  // add a shifted text besides it
  const auto shift = sf::Vector2f (buttonSize ().x, 0);
  mRender.drawText (
    sanitizePosition (pos + shift + mPadding),
    text,
    mStyle.fontColor,
    mStyle.fontSize.normal
  );
  return clicked;
}

/////////////////////////////////////////////////
void Gui::icon (
  const IconID& name,
  const sf::Vector2f& size,
  const Tooltip& info,
  const sf::Vector2f& position)
{
  // compute icon position
  auto pos = mCursorPosition;
  computeRelativePosition (pos, position);

  // draw icon
  mRender.drawIcon (sf::FloatRect (sf::Vector2f (pos), size), name);

  // handle icon hovering and tooltip
  itemStatus (sf::FloatRect (sf::Vector2f (pos), size), name, false, info);

  // update cursor position
  updateSpacing (size);
}

/**
 * ----------------------------------------------
 * Box related widget
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::checkBox (
  bool& checked,
  const std::string& text,
  const Tooltip& info,
  const sf::Vector2f& position)
{
  // assign unique id to the checkBox
  const auto name = "CheckBox" + std::to_string (mWidgetCount);
  mWidgetCount++;

  // compute check box position
  auto pos = mCursorPosition;
  computeRelativePosition (pos, position);

  // get status of the widget,
  const auto size = normalTextHeight () * sf::Vector2f (2.f, 1.f);
  const auto box = sf::FloatRect (pos, size);
  auto state = itemStatus (box, name, mInputState.mouseLeftReleased, info);
  if (state == ItemState::Active) {
    checked = !checked;
  }

  // draw checkBox state and update cursor position
  if (checked) {
    state = ItemState::Active;
  }
  mRender.draw <Widget::CheckBox> (box, state);

  // draw text next to the checkbox
  mRender.drawText (
    sanitizePosition (pos + sf::Vector2f (size.x + mPadding.x, 0.f)),
    text,
    mStyle.fontColor,
    mStyle.fontSize.normal
  );

  // update cursor position
  const auto textLength = normalSizeOf (text + "g").x;
  updateSpacing (size + sf::Vector2f (textLength, 0.f));
}

/**
 * ----------------------------------------------
 * Text related widget
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::text (
  const std::string& text,
  const sf::Vector2f& boxSize,
  const sf::Vector2f& position)
{
  // compute text position
  auto pos = mCursorPosition;
  computeRelativePosition (pos, position);

  // format the text to fit in the box if one is furnished
  const auto fontSize = mStyle.fontSize.normal;
  const auto formatted = formatText (text, boxSize, fontSize);

  // draw text
  mRender.drawText (
    sanitizePosition (pos),
    formatted,
    mStyle.fontColor,
    fontSize
  );
  // update cursor position
  updateSpacing (normalSizeOf (formatted + "g"));
}


/**
 * ----------------------------------------------
 *  text input related widget
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::inputColor (
  sf::Color& color,
  const std::string& description,
  const sf::Vector2f& position)
{
  // keep initial position
  auto initialPos = mCursorPosition;
  computeRelativePosition (initialPos, position);

  // change color with four input number
  const auto verticalPos = mCursorPosition.y;
  inputNumber (color.r, "", std::uint8_t (0), std::uint8_t (255), "r: ", position);
  sameLine ();
  inputNumber (color.g, "", std::uint8_t (0), std::uint8_t (255), "g: ");
  sameLine ();
  inputNumber (color.b, "", std::uint8_t (0), std::uint8_t (255), "b: ");
  sameLine ();
  inputNumber (color.a, "", std::uint8_t (0), std::uint8_t (255), "a: ");
  sameLine ();

  // draw description next to the four boxes
  const auto spacing = mLastSpacing;
  text (description);

  // update cursor position
  mCursorPosition = initialPos;
  const auto textWidth = normalSizeOf (description + "g").x;
  updateSpacing (sf::Vector2f (4.f*spacing.x + textWidth, spacing.y));
}

/////////////////////////////////////////////////
void Gui::inputText (
  std::string& text,
  const std::string& description,
  const sf::Vector2f& boxSizeParam,
  const sf::Vector2f& position)
{
  // assign unique id to the widget
  const auto name = "inputText" + std::to_string (mWidgetCount);
  mWidgetCount++;

  // compute widget position
  auto pos = mCursorPosition;
  computeRelativePosition (pos, position);

  // draw description before the box
  auto descriptionSize = sf::Vector2f ();
  if (description != "") {
    mRender.drawText (
      sanitizePosition (pos),
      description,
      mStyle.fontColor,
      mStyle.fontSize.normal
    );
    descriptionSize = normalSizeOf (description + "g");
    pos.x += descriptionSize.x;
  }

  // format text to fit in the parent box or the requested box
  // TODO: Encapsulate text in a scrollable box
  auto boxSize = boxSizeParam;
  const auto boxLength = boxSizeParam.length ();
  const auto textWidth = normalSizeOf (text).x;
  if (!mGroups.empty ()) {
    auto width = mGroups.top ().size.x - 2.f*mPadding.x - descriptionSize.x;
    if (width < 0.f) {
      pos.y += descriptionSize.y;
      width += descriptionSize.x;
    }
    if (boxLength < 0.01f) {
      boxSize = sf::Vector2f (width, normalTextHeight ());
    } else {
      boxSize.x = std::min (boxSize.x, width - mPadding.x);
    }
  } else if (boxLength < 0.01f) {
    const auto width = normalSizeOf ("sample text length").x;
    boxSize = sf::Vector2f (width, normalTextHeight ());
    boxSize.x = std::max (boxSize.x, textWidth + 2.5f*mPadding.x);
  }

  // get status of the widget
  const auto box = sf::FloatRect (pos, boxSize);
  auto state = itemStatus (box, name, mInputState.mouseLeftDown);

  // take keyboard focus if clicked
  if (mGuiState.activeItem == name) {
    mGuiState.keyboardFocus = name;
  }

  // if this widget has keyboard focus
  const auto focused = mGuiState.keyboardFocus == name;
  if (focused) {
    state = ItemState::Active;
    if (mInputState.keyIsPressed) {
      const auto isTooLarge = (boxLength < 0.01f) && (textWidth >= 0.94f*boxSize.x);
      handleKeyInput (text, isTooLarge);
    }
  }

  // draw formatted text and box arround it
  const auto formatted = formatText (text, boxSize, mStyle.fontSize.normal);
  mRender.draw <Widget::TextBox> (box, state);
  mRender.drawText (
    sanitizePosition (pos + mPadding),
    formatted,
    mStyle.fontColor,
    mStyle.fontSize.normal
  );

  // update cursor position
  updateSpacing (boxSize + sf::Vector2f (descriptionSize.x, 0.f));
}

/////////////////////////////////////////////////
void Gui::inputKey (
  char& key,
  const std::string& description,
  const sf::Vector2f& position)
{
  // assign unique id to the widget
  const auto name = "inputKey" + std::to_string (mWidgetCount);
  mWidgetCount++;

  // compute widget position
  auto pos = mCursorPosition;
  computeRelativePosition (pos, position);

  // draw description before the box
  mRender.drawText (
    sanitizePosition (pos),
    description,
    mStyle.fontColor,
    mStyle.fontSize.normal
  );
  const auto descrWidth = normalSizeOf (description + "g").x;
  pos.x += descrWidth;

  // get widget status
  const auto boxSize = normalTextHeight () * sf::Vector2f (1.f, 1.f);
  const auto box = sf::FloatRect (pos, boxSize);
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
  mRender.draw <Widget::TextBox> (box, state);
  mRender.drawText (
    sanitizePosition (pos + mPadding),
    text,
    mStyle.fontColor,
    mStyle.fontSize.normal
  );

  // update cursor position
  updateSpacing (boxSize + sf::Vector2f (descrWidth, 0.f));
}


/**
 * ----------------------------------------------
 * to plot progress bar and spinning wheel
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::progressBar (
  const float progress,
  const sf::Vector2f& size,
  const Tooltip& info,
  const sf::Vector2f& position)
{
  // assign unique id to the widget
  const auto name = "ProgressBar" + std::to_string (mWidgetCount);
  mWidgetCount++;

  // compute icon position
  auto pos = mCursorPosition;
  computeRelativePosition (pos, position);

  // draw progress bar
  const auto box = sf::FloatRect (pos, size);
  mRender.drawProgressBar (box, sgui::clamp (0.f, 1.f, progress));

  // handle icon hovering
  itemStatus (sf::FloatRect (pos, size), name, false, info);

  // update cursor position
  updateSpacing (size);
}


/**
 * ----------------------------------------------
 * to plot functions in gui
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::setPlotRange (
  const PlotRange xRange,
  const PlotRange yRange)
{
  mPlotter.setRangeX (xRange);
  mPlotter.setRangeY (yRange);
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
  if (!mPlotsData.has (mPlotCount)) {
    cachePlotData (slope);
  }
  plot (mPlotsData.get (mPlotCount), thickness, lineColor);
  mPlotCount++;
}

/////////////////////////////////////////////////
void Gui::plot (
  const std::function<sf::Vector2f (float)>& slope,
  const float thickness,
  const sf::Color& lineColor)
{
  // cache plot data to avoid useless computation
  if (!mPlotsData.has (mPlotCount)) {
    cachePlotData (slope);
  }
  plot (mPlotsData.get (mPlotCount), thickness, lineColor);
  mPlotCount++;
}

/////////////////////////////////////////////////
void Gui::plot (
  const std::vector<sf::Vector2f>& points,
  const float thickness,
  const sf::Color& lineColor)
{
  // keep cursor position as handlePlotBound will modify it
  const auto pos = mCursorPosition;
  handlePlotBound ();
  mPlotter.plot (points, pos, lineColor, thickness);
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
    const auto range = mPlotter.rangeX ();
    const auto x = sgui::lerp (range.min, range.max, i / max);
    slopeData.emplace_back (x, slope (x));
  }
  mPlotsData.emplace (mPlotCount, std::move (slopeData));
}

/////////////////////////////////////////////////
void Gui::cachePlotData (const std::function<sf::Vector2f (float)>& slope)
{
  auto slopeData = std::vector <sf::Vector2f> ();
  const auto max = static_cast<float> (mPlotSample);
  for (int i = 0; i < static_cast<int> (mPlotSample); i++) {
    const auto range = mPlotter.rangeX ();
    const auto x = sgui::lerp (range.min, range.max, i / max);
    slopeData.emplace_back (slope (x));
  }
  mPlotsData.emplace (mPlotCount, std::move (slopeData));
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


/**
 * ----------------------------------------------
 * drop list
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::dropList (
  uint32_t& selected,
  const std::vector<std::string>& list,
  const uint32_t phantomElements,
  const sf::Vector2f& position)
{
  // if list is empty we quit and do nothing
  if (list.empty ()) return;

  // initialize drop list id and item height
  const auto globalName = "DropList" + std::to_string (mWidgetCount);
  mWidgetCount++;

  // compute drop list width
  auto maxWidth = 0.f;
  for (const auto& text : list) {
    maxWidth = std::max (maxWidth, normalSizeOf (text).x);
  }
  const auto itemWidth = 2.f*mPadding.x + maxWidth;
  const auto itemSize = sf::Vector2f (itemWidth, normalTextHeight ());

  // compute drop list initial position
  auto initialPos = mCursorPosition;
  computeRelativePosition (initialPos, position);

  // compute each drop list item
  auto counter = 0u;
  auto selName = std::string ("");
  if (selected < list.size ()) {
    selName = list [selected];
  }
  for (const auto& itemName : list) {
    // compute item position
    auto itemPos = sf::Vector2f (initialPos) + sf::Vector2f (0.f, counter*itemSize.y);

    // get item status and update selected value
    if (dropListItem (selName, itemName, itemPos, itemSize)) {
      selected = counter;
    }
    counter++;
  }

  // update cursor position
  counter += phantomElements;
  updateSpacing (sf::Vector2f (itemSize.x, counter * itemSize.y));
}

/////////////////////////////////////////////////
bool Gui::dropListItem (
  const std::string& selectedName,
  const std::string& itemName,
  const sf::Vector2f& itemPosition,
  const sf::Vector2f& itemSize)
{
  // set item id
  const auto name = "DropListItem" + std::to_string (mWidgetCount);
  mWidgetCount++;

  // get item status
  const auto box = sf::FloatRect (itemPosition, itemSize);
  const auto state = itemStatus (box, name, mInputState.mouseLeftDown);
  const auto status = (state == ItemState::Active)
    && (mGuiState.activeItem == name);

  // draw item
  if (selectedName == itemName) {
    mRender.draw <Widget::ItemBox> (box, ItemState::Active);
  } else {
    mRender.draw <Widget::ItemBox> (box, state);
  }
  mRender.drawText (
    sanitizePosition (box.position + mPadding),
    itemName,
    mStyle.fontColor,
    mStyle.fontSize.normal
  );

  // return selection status
  return status;
}


/**
 * ----------------------------------------------
 *  scroller for scrollable window/panel
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
bool Gui::isPanelScrollable (const GroupData& panel)
{
  // panel is scrollable if it possess a scroller
  if (mGroupsScrollerInformation.has (panel.groupId)) {
    auto& scrollData = mGroupsScrollerInformation.get (panel.groupId);
    // and if its scroller size is greater than its size
    if (panel.horizontal) {
      return scrollData.size () > panel.size.x;
    }
    return scrollData.size () > panel.size.y;
  }

  // if panel does not posses a scroller add one
  mGroupsScrollerInformation.emplace (panel.groupId, panel.horizontal);
  return false;
}

/////////////////////////////////////////////////
sf::Vector2f Gui::scrollPanel (
  const uint32_t id,
  const sf::FloatRect& box,
  const ItemState state,
  const bool horizontal)
{
  // scroll through panel
  if (mGroupsScrollerInformation.has (id)) {
    auto& scrollData = mGroupsScrollerInformation.get (id);
    auto percent = scrollData.percent ();
    const auto size = scrollData.size ();
    const auto amount = scroller (percent, box, state, size, horizontal);
    scrollData.scroll (percent);
    return amount;
  }

  return sf::Vector2f (0.f, 0.f);
}

/////////////////////////////////////////////////
sf::Vector2f Gui::scroller (
  float& percent,
  const sf::FloatRect& panelBox,
  const ItemState panelStatus,
  const float scrollSize,
  const bool horizontal)
{
  // assign unique id to the widget
  const auto name = "Scroller" + std::to_string (mWidgetCount);
  mWidgetCount++;

  // compute scroller position (right or bottom) and size
  auto pos = sf::Vector2f ();
  auto size = panelBox.size;
  auto extraSize = scrollSize + (2.f * mPadding.y);
  if (horizontal) {
    pos = sf::Vector2f (panelBox.position.x, panelBox.position.y + panelBox.size.y);
    pos.y -= normalTextHeight ();
    size.y = normalTextHeight ();
    extraSize -= size.x;
  } else {
    pos = sf::Vector2f (panelBox.position.x + panelBox.size.x, panelBox.position.y);
    pos.x -= normalTextHeight ();
    size.x = normalTextHeight ();
    extraSize -= size.y;
  }

  // get scroller status, we always steal active state over the previous widget
  const auto box = sf::FloatRect (pos, size);
  auto state = itemStatus (box, name, mInputState.mouseLeftDown, Tooltip (), true);
  mRender.draw <Widget::Scroller> (box, state, horizontal);

  // if active, compute current scrolling
  if (mGuiState.activeItem == name) {
    state = ItemState::Active;
    percent = sliderValue (box, 0.f, 1.f, horizontal);
  }

  // if parent is active and scroller is inactive (we can't drag and scroll), scroll with wheel
  const auto parentIsActive = panelStatus == ItemState::Hovered || panelStatus == ItemState::Active;
  const auto scrollerIsInactive = state != ItemState::Active;
  if (parentIsActive && scrollerIsInactive && mInputState.mouseScrolled) {
    // move by 5 % per scroll TODO: add a function to change this percentage
    const auto dx =  std::abs (0.05f * mInputState.mouseDeltaWheel);
    auto gain = 0.f;
    if (horizontal) {
      gain = sgui::remap (0.f, box.size.x, 0.f, 1.f, dx * box.size.x);
    } else {
      gain = sgui::remap (0.f, box.size.y, 0.f, 1.f, dx * box.size.y);
    }
    // note that 0% is the top scrollbar, so if delta > 0 we need to subtract gain
    if (mInputState.mouseDeltaWheel > 0.f) {
      percent = sgui::clamp (0.f, 1.f, percent - gain);
    } else {
      percent = sgui::clamp (0.f, 1.f, percent + gain);
    }
  }

  // compute scrollbar extra shift
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
  mRender.draw <Widget::SliderBar> (barBox, state, horizontal);

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
  auto shift = 0.f;

  // scrollbar is a square box so width = height = smallest side
  auto barBox = parentBox;
  if (extraSize > 0.f) {
    const auto minSize = buttonHeight ();
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
  mRender.draw <Widget::ScrollerBar> (barBox, state, horizontal);

  // return shift for further uses
  return shift;
}


/**
 * ----------------------------------------------
 * begin or end group
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::beginGroup (
  const bool horizontal,
  const sf::Vector2f& position,
  const sf::Vector2f& size)
{
  // construct a new group
  auto newGroup = GroupData ();
  newGroup.horizontal = horizontal;
  newGroup.position = position;
  newGroup.size = size;
  newGroup.lastItemPosition = position + 1.5f*mPadding;
  newGroup.menuItemCount = 0;
  newGroup.clippingLayer = 0;
  newGroup.menuBarSize = sf::Vector2f (0, 0);
  newGroup.groupId = mGroupCount;
  mGroupCount++;

  // add it to the stack
  mGroups.emplace (std::move (newGroup));
}

/////////////////////////////////////////////////
void Gui::endGroup ()
{
  // check if there is in fact stacked group...
  if (mGroups.empty ()) {
    spdlog::warn ("Cannot remove group from empty stack in Gui::enGroup ()");
    return;
  }
  mGroups.pop ();
}


/**
 * ----------------------------------------------
 * window/panel utility functions
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
sf::Vector2f Gui::computePosition (
  const Panel& settings,
  const Constraint& constraint)
{
  // get parent shift and size
  auto positionShift = sf::Vector2f ();
  auto constraintSize = mWindowSize;
  if (!mGroups.empty ()) {
    const auto& parent = mGroups.top ();
    positionShift += parent.position;
    constraintSize = parent.size;
  }
  // constrain position
  return constrainPosition (
    settings.position, positionShift, constraintSize, settings.size, constraint
  );
}

/////////////////////////////////////////////////
uint32_t Gui::setClipping (
  const sf::FloatRect& panelBox,
  const float activeHeight,
  const float menuHeight,
  const bool isPanel)
{
  // compute clip position and size
  auto layerBox = panelBox;
  layerBox.size.y += activeHeight + menuHeight;
  if (isPanel) {
    layerBox.position.y -= activeHeight;
  }

  // set clipping layer and return its id
  return mRender.setCurrentClippingLayer (layerBox);
}

/////////////////////////////////////////////////
ItemState Gui::interactWithMouse (
  Panel& settings,
  const sf::FloatRect& box,
  const ItemID& name,
  const Tooltip& info)
{
  // move panel according to mouse displacement
  const auto leftClick = mInputState.mouseLeftDown;
  const auto state = itemStatus (box, name, leftClick, info);
  const auto isActive = mGuiState.activeItem == name;
  if (isActive && settings.movable) {
    settings.position.x += mInputState.mouseDisplacement.x;
    settings.position.y += mInputState.mouseDisplacement.y;
  }
  return state;
}



/**
 * ----------------------------------------------
 * sanitize position to avoid blurry text
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
sf::Vector2f Gui::sanitizePosition (const sf::Vector2f& position) const
{
  return sf::Vector2f (std::round (position.x), std::round (position.y));
}


/**
 * ----------------------------------------------
 * standard height
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
float Gui::footnoteTextHeight () const
{
  return mStyle.fontSize.footnote + 4.f*mPadding.y;
}

/////////////////////////////////////////////////
float Gui::subtitleTextHeight () const
{
  return mStyle.fontSize.subtitle + 4.f*mPadding.y;
}

/////////////////////////////////////////////////
float Gui::titleTextHeight () const
{
  return mStyle.fontSize.title + 4.f*mPadding.y;
}

/////////////////////////////////////////////////
float Gui::normalTextHeight () const
{
  return mStyle.fontSize.normal + 4.f*mPadding.y;
}

/////////////////////////////////////////////////
float Gui::buttonHeight () const
{
  return mStyle.fontSize.normal + 6.f*mPadding.y;
}


/**
 * ----------------------------------------------
 * to compute item status (active, hovered, neutral)
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
ItemState Gui::itemStatus (
  const sf::FloatRect& boundingBox,
  const ItemID& item,
  bool condition,
  const Tooltip& info,
  bool forceActive)
{
  auto state = ItemState::Neutral;

  // if mouse collide with the boundingBox
  if (boundingBox.contains (mInputState.mousePosition)
  && !mRender.isClipped (mInputState.mousePosition)) {
    // enter hovered state
    state = ItemState::Hovered;
    mGuiState.hoveredItem = item;

    // if no widget is active, enter active state
    if ((mGuiState.activeItem == NullItemID || forceActive) && condition) {
      // we want to store item ID to move it, but we don't want infinite click
      mGuiState.activeItem = item;
      if (mInputState.updated) {
        state = ItemState::Active;
      }
    }

    // update tooltip info
    if (info.active) {
      mGuiState.hoveredItemBox = boundingBox;
      mGuiState.tooltip = info;
      mGuiState.tooltip.parent = item;
    }
  }

  return state;
}


/**
 * ----------------------------------------------
 * to handle text related stuff
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
void Gui::handleKeyInput (
  std::string& text,
  const bool textIsTooLarge)
{
  // we only handle key if inputs were updated
  if (!mInputState.updated) return;
  const char keyPressed = mInputState.keyPressed;
  // erase last character
  if (keyPressed == L'\b') {
    if (!text.empty ()) {
      // erase both return at line and last char
      if (text.back () == L'\n') {
        text.pop_back();
        if (!text.empty ()) {
          text.pop_back ();
        }
      // erase last char
      } else {
        text.pop_back ();
      }
    }

  // add return at line
  } else if (keyPressed == L'\n') {
    text += L'\n';

  // add character to the text
  } else if (!textIsTooLarge) {
    text += keyPressed;
  }
}

/////////////////////////////////////////////////
std::string Gui::formatText (
  const std::string& input,
  const sf::Vector2f& boxSize,
  const uint32_t fontSize)
{
  // if input is contrained by a box
  if (boxSize.x > 0.f) {
    auto formattedText = std::string ("");
    // read the string word by word
    auto in = std::istringstream (input);
    auto word = std::string ("");
    auto line = std::string ("");
    while (in >> word) {
      // add a new line if current word outpass box boundaries
      line += " " + word;
      const auto lineWidth = mRender.textSize (line, fontSize).x;
      if (lineWidth >= 0.98f*boxSize.x) {
        formattedText += "\n" + word;
        line = word;
      } else {
        formattedText += " " + word;
      }
    }
    return formattedText;
  }
  return input;
}


/**
 * ----------------------------------------------
 * to compute automatic position of widget
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
sf::Vector2f Gui::computeSpacing (const sf::Vector2f& size)
{
  // Compute spacing relative to the group
  auto spacing = sf::Vector2f ();
  if (!mGroups.empty ()) {
    // if group is a menu we skip it
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

    // if group is horizontal, add along x. If there are more than two sameLine,
    // we need to preserve x spacing, instead of going back to window base x pos.
    const auto backToInnerPos = mResetDifference < 2;
    const auto padding = mStyle.itemInnerSpacing * sf::Vector2f (1.f, 1.f) + 2.f*mPadding;
    if (parent.horizontal) {
      // update and store spacing 
      spacing.x = size.x + padding.x;
      mLastSpacing.x = spacing.x;
      mLastSpacing.y = size.y + padding.y;
      // get parent pos if less than two sameLine was called
      if (backToInnerPos) {
        mCursorPosition.y = parent.innerPosition.y;
      }
      mCursorPosition.x += spacing.x;
    // else update with standard spacing along y
    } else {
      // update and store spacing
      spacing.y = size.y + padding.y;
      mLastSpacing.x = size.x + padding.x;
      mLastSpacing.y = spacing.y;
      // get parent pos if less than two sameLine was called
      if (backToInnerPos) {
        mCursorPosition.x = parent.innerPosition.x;
      }
      mCursorPosition.y += spacing.y;
    }
  // If no group is set, we just write vertically
  } else {
    // update spacing and position
    spacing.y = size.y + mStyle.itemSpacing + mPadding.y;
    mLastSpacing.x = size.x + mStyle.itemSpacing + mPadding.x;
    mLastSpacing.y = spacing.y;
    mCursorPosition += spacing;
  }
  return spacing;
}

/////////////////////////////////////////////////
void Gui::updateSpacing (const sf::Vector2f& size)
{
  // manage same line call, if difference > 0, a sameLine was just
  // called and we need to go back to the previous line. If difference
  // continue to increase, user is calling several sameLine
  auto difference = mResetCount - mPreviousResetCount;
  if (mResetDifference == difference) {
    mPreviousResetCount = mResetCount;
  }
  mResetDifference = difference;

  // update cursor position
  const auto spacing = computeSpacing (size);

  // update scrolling size and spacing 
  updateScrolling (spacing);
}

/////////////////////////////////////////////////
void Gui::updateScrolling (const sf::Vector2f& spacing)
{
  // update current group scrolling size
  if (!mGroups.empty ()) {
    const auto groupId = mGroups.top ().groupId;
    if (mGroupsScrollerInformation.has (groupId)) {
      auto& scrollData = mGroupsScrollerInformation.get (groupId);
      scrollData.computeScrollSize (spacing);
    }
  }
}


/**
 * ----------------------------------------------
 * to compute constrained position
 * ----------------------------------------------
 */
/////////////////////////////////////////////////
sf::Vector2f Gui::constrainPosition (
  const sf::Vector2f& position,
  const sf::Vector2f& positionShift,
  const sf::Vector2f& windowSize,
  const sf::Vector2f& elementSize,
  const Constraint& constraint)
{
  const auto halfSize = elementSize / 2.f;
  const auto center = windowSize / 2.f;
  auto pos = position;

  // center element if requested
  if (constraint.centeredVerticaly) {
    pos.y = center.y - halfSize.y;
  }
  if (constraint.centeredHorizontaly) {
    pos.x = center.x - halfSize.x;
  }

  // fix element relative to side of the window
  if (constraint.pixelsFromTop != 0) {
    pos.y = constraint.pixelsFromTop;
  }
  if (constraint.pixelsFromBottom != 0) {
    pos.y = windowSize.y - constraint.pixelsFromBottom - elementSize.y;
  }
  if (constraint.pixelsFromLeft != 0) {
    pos.x = constraint.pixelsFromLeft;
  }
  if (constraint.pixelsFromRight != 0) {
    pos.x = windowSize.x - constraint.pixelsFromRight - elementSize.x;
  }

  // if there are no constraints, return cursor position
  if (pos.length () < 0.01f) {
    return mCursorPosition;
  }
  // else return constrained position
  return pos + positionShift;
}

/////////////////////////////////////////////////
void Gui::computeRelativePosition (
  sf::Vector2f& position,
  const sf::Vector2f& displacement)
{
  // If there is no active group, displacement = position
  const auto isNotNull = displacement.length () > 0.01f;
  if (mGroups.empty () && isNotNull) {
    position = displacement;
  }

  // If there is an active group, displace relatively to the group position
  if (!mGroups.empty ()) {
    const auto& parent = mGroups.top ();
    if (isNotNull) {
      position = parent.position + displacement;
    // Manage sameLine/sameColumn edge case, we need to add previous space,
    // otherwise this sameLine command will send the next widget right on the
    // previous one
    } else if (mResetCount - mPreviousResetCount == 2) {
      if (parent.horizontal) {
        position.y += mLastSpacing.y;
        mCursorPosition.y += mLastSpacing.y;
      } else {
        position.x += mLastSpacing.x;
        mCursorPosition.x += mLastSpacing.x;
      }
    }
  }
}

} // namespace sgui
