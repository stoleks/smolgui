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
  const auto name = initializeActivable ("InputNumber");
  const auto position = computeRelativePosition (options.displacement);

  // compute text box dimension
  auto width = textSize (label + "10000").x;
  if (!fixedWidth) {
    if constexpr (std::is_floating_point_v <Type>) {
      width = std::max (width, textSize (label + fmt::format ("{:.3f}", number)).x);
    } else {
      width = std::max (width, textSize (label + fmt::format ("{}", number)).x);
    }
  }
  const auto boxSize = sf::Vector2f (width + 4.f*mPadding.x, textHeight ());

  // get status of the widget
  const auto box = sf::FloatRect (position, boxSize);
  auto state = itemStatus (box, name, mInputState.mouseLeftDown);
  // take keyboard focus if active
  if (mGuiState.activeItem == name) {
    if (mGuiState.keyboardFocus != name) {
      mActiveInputNumber = "";
    }
    mGuiState.keyboardFocus = name;
  }

  // if this widget has keyboard focus update its number
  const auto focused = mGuiState.keyboardFocus == name;
  handleNumberKeyInput (number, focused, min, max);

  // draw text box
  if (focused) {
    state = ItemState::Active;
  }
  mRender.draw (box, {Widget::TextBox, Slices::Three, state});

  // draw label and number
  auto numberStr = std::string ("");
  if constexpr (std::is_floating_point_v <Type>) {
    numberStr = fmt::format ("{:.3f}", number);
  } else {
    numberStr = fmt::format ("{}", number);
  }
  const auto inputStr = label + numberStr;
  const auto numWidth = textSize (inputStr).x;
  const auto shiftToCenter = sf::Vector2f ((boxSize.x - numWidth - mPadding.x) / 2.f, mPadding.y);
  handleTextDrawing (position + shiftToCenter, inputStr);

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
  const bool focused,
  const Type min,
  const Type max)
{
  if (focused && mInputState.keyIsPressed) {
    // if a key is pressed and is a digit handle if
    const auto key = mInputState.keyPressed;
    const auto digit = std::isdigit (static_cast<unsigned char> (key));
    if (digit || key == '.' || key == '-' || key == L'\b') {
      // handle key and convert it into number
      handleKeyInput (mActiveInputNumber);
      number = convertKeyIntoNumber <Type> (mActiveInputNumber, min, max);
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
  const Type max)
{
  // convert string into number
  auto number = Type (0);
  if (key != "") {
    // manage type
    if constexpr (std::is_same_v <Type, int>) {
      number = std::stoi (mActiveInputNumber);
    } else if constexpr (std::is_same_v <Type, float>) {
      number = std::stof (mActiveInputNumber);
    } else if constexpr (std::is_same_v <Type, double>) {
      number = std::stod (mActiveInputNumber);
    } else {
      number = std::stoul (mActiveInputNumber);
    }
    // clamp value if min and max are furnished and valid
    if (max > min) {
      number = sgui::clamp (min, max, number);
      mActiveInputNumber = fmt::format ("{}", number);
    } 
  }
  // return computed number
  return number;
}

} // namespace sgui
