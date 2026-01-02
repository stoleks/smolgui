namespace sgui {

/**
 *  slider
 */
/////////////////////////////////////////////////
template <typename Type>
void Gui::slider (
  Type& value,
  const Type min,
  const Type max,
  const WidgetOptions& options)
{
  // initialize widget if and position
  const auto name = initializeActivable ("Slider");
  const auto position = computeRelativePosition (options.displacement);

  // get status of the widget
  auto dimVector = options.size;
  if (options.horizontal) {
    std::swap (dimVector.x, dimVector.y);
  }
  const auto size = textHeight () * dimVector;
  const auto box = sf::FloatRect (position, size);
  auto state = itemStatus (box, name, mInputState.mouseLeftDown, options.tooltip);
  mRender.draw (box, {Widget::Slider, Slices::Three, state, !options.horizontal});

  // if active, update value depending on bar position
  if (mGuiState.activeItem == name) {
    state = ItemState::Active;
    value = sliderValue (box, min, max, !options.horizontal);
  }

  // draw text next to the slider
  const auto descrPos = position + sf::Vector2f (size.x + mPadding.x, 0.f);
  const auto descrSize = widgetDescription (descrPos, options.description);
  // update cursor position
  updateSpacing ({size.x + descrSize.x, size.y});

  // compute scrollBar relative position
  const auto percent = sgui::remap (min, max, 0.05f, 0.95f, value);
  sliderBar (box, state, percent, !options.horizontal);
}

/////////////////////////////////////////////////
template <typename Type>
Type Gui::sliderValue (
  const sf::FloatRect& box,
  const Type min,
  const Type max,
  const bool horizontal)
{
  // compute shift from x or y relative position
  const auto relativePos = mInputState.mousePosition - box.position;
  if (horizontal) {
    return sgui::remap (0.f, box.size.x, min, max, relativePos.x);
  } 
  return sgui::remap (0.f, box.size.y, min, max, relativePos.y);
}


/**
 *  input
 */
/////////////////////////////////////////////////
template <typename Type>
void Gui::inputNumber (
  Type& number,
  const WidgetOptions& options,
  const Type min,
  const Type max,
  const std::string& label,
  const bool fixedWidth)
{
  // Initialize widget name and position
  auto numStr = formatNumberToString (number);
  const auto name = initializeActivable ("NumberInput");
  const auto position = computeRelativePosition (options.displacement);
  if (!mTextCursorPositions.has (name)) {
    mTextCursorPositions.emplace (name, numStr.length ());
    mTextHasCursor.emplace (name, 1u);
  }

  // compute text box dimension
  auto width = textSize (label + "10000").x;
  if (!fixedWidth) {
    const auto numStr = formatNumberToString (number);
    width = std::max (width, textSize (label + numStr).x);
  }
  const auto boxSize = sf::Vector2f (width + 4.f*mPadding.x, textHeight ());

  // get status of the widget
  const auto box = sf::FloatRect (position, boxSize);
  auto state = itemStatus (box, name, mInputState.mouseLeftDown);
  // take keyboard focus if active
  if (mGuiState.activeItem == name) {
    if (mGuiState.keyboardFocus != name) {
      mActiveInputNumberStr = formatNumberToString (number);
      mTextCursorPositions.get (name) = mActiveInputNumberStr.length ();
    }
    mGuiState.keyboardFocus = name;
  }

  // if this widget has keyboard focus update its number
  auto& cursorIndex = mTextCursorPositions.get (name);
  const auto focused = mGuiState.keyboardFocus == name;
  handleNumberKeyInput (number, cursorIndex, focused, min, max);

  // draw text box
  if (focused) {
    state = ItemState::Active;
    numStr = mActiveInputNumberStr;
  } else {
    numStr = formatNumberToString (number);
  }
  mRender.draw (box, {Widget::TextBox, Slices::Three, state});

  // draw label and number
  const auto inputStr = label + numStr;
  const auto numWidth = textSize (numStr).x;
  const auto shiftToCenter = sf::Vector2f ((boxSize.x - numWidth) / 2.f - mPadding.x, mPadding.y);
  handleTextDrawing (position + shiftToCenter, inputStr);
  if (focused) {
    const auto labelShift = sf::Vector2f (textSize (label).x, 0.f);
    drawTextCursor (position + shiftToCenter + labelShift, name, numStr, {});
  }

  // draw description
  const auto descrPos = position + sf::Vector2f (boxSize.x + mPadding.x, 0);
  const auto descrSize = widgetDescription (descrPos, options.description);
  // update cursor position
  updateSpacing ({boxSize.x + descrSize.x, boxSize.y});
}

/////////////////////////////////////////////////
template <typename Type>
void Gui::inputVector2 (
  sf::Vector2<Type>& vector,
  const WidgetOptions& options,
  const sf::Vector2<Type>& min,
  const sf::Vector2<Type>& max)
{
  inputNumber (vector.x, {options.displacement}, min.x, max.x, "x: ", true);
  sameLine ();
  inputNumber (vector.y, {}, min.y, max.y, "y: ", true);
  
  // draw description
  if (options.description != "") {
    sameLine ();
    auto descriptionOptions = options;
    descriptionOptions.displacement = {};
    text (options.description, {}, descriptionOptions);
  }
}

/////////////////////////////////////////////////
template <typename Type>
void Gui::inputVector3 (
  sf::Vector3<Type>& vector,
  const WidgetOptions& options,
  const sf::Vector3<Type>& min,
  const sf::Vector3<Type>& max)
{
  inputNumber (vector.x, {options.displacement}, min.x, max.x, "x: ", true);
  sameLine ();
  inputNumber (vector.y, {}, min.y, max.y, "y: ", true);
  sameLine ();
  inputNumber (vector.z, {}, min.z, max.z, "z: ", true);

  // draw description
  if (options.description != "") {
    sameLine ();
    auto descriptionOptions = options;
    descriptionOptions.displacement = {};
    text (options.description, {}, descriptionOptions);
  }
}

/////////////////////////////////////////////////
template <typename Type>
void Gui::handleNumberKeyInput (
  Type& number,
  size_t& cursorIndex,
  const bool focused,
  const Type min,
  const Type max)
{
  if (focused && (mInputState.textIsEntered || mInputState.keyIsPressed)) {
    // if a key is pressed and is a digit handle if
    const auto key = mInputState.keyPressed;
    const auto isDigit = std::isdigit (static_cast<unsigned char> (key));
    const auto validPoint = key == '.' && mActiveInputNumberStr.find (".") == std::string::npos;
    if (isDigit || validPoint || key == L'\b') {
      // handle key and convert it into number
      handleKeyInput (mActiveInputNumberStr, cursorIndex);
      number = convertKeyIntoNumber <Type> (mActiveInputNumberStr, min, max);
      cursorIndex = clamp (size_t (0), mActiveInputNumberStr.length (), cursorIndex);
    }
    // if enter is pressed we lost focus
    if (key == L'\n') {
      mGuiState.keyboardFocus = NullID;
    }
  }
}

/////////////////////////////////////////////////
template <typename Type>
Type Gui::convertKeyIntoNumber (
  std::string& key,
  const Type min,
  const Type max) const
{
  // convert string into number
  auto number = Type (0);
  const auto numPos = key.find_last_of (' ');
  const auto numStr = key.substr (std::min (numPos + 1, key.length ()));
  if (numStr != "" && key != "." && key != "0") {
    // manage type
    if constexpr (std::is_same_v <Type, int>) {
      number = std::stoi (key);
    } else if constexpr (std::is_same_v <Type, float>) {
      number = std::stof (key);
    } else if constexpr (std::is_same_v <Type, double>) {
      number = std::stod (key);
    } else {
      number = std::stoul (key);
    }
    // clamp value if min and max are furnished and valid
    if (max > min) {
      number = sgui::clamp (min, max, number);
      key = formatNumberToString (number);
    } 
  }
  // return computed number
  return number;
}

/////////////////////////////////////////////////
template <typename Type>
std::string Gui::formatNumberToString (const Type& number) const
{
  if constexpr (std::is_floating_point_v <Type>) {
    if (number < 1000.f) {
      return fmt::format ("{:7.2f}", number);
    }
    return fmt::format ("{:7.2}", number);
  }
  return fmt::format ("{}", number);
}

} // namespace sgui
