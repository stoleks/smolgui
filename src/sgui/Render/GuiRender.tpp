namespace sgui
{
/////////////////////////////////////////////////
template <Widget Type>
void GuiRender::draw (
  const sf::FloatRect& box,
  const WidgetDrawOptions options)
{
  const auto typeName = toString <Type> () + "::";
  const auto state = toString (options.state);
  // Widget composed of three patches (stretchable)
  if constexpr (Type == Widget::Slider
  || Type == Widget::ScrollerBar
  || Type == Widget::Scroller
  || Type == Widget::ProgressBar
  || Type == Widget::ProgressFilling
  || Type == Widget::TitleBox
  || Type == Widget::TextBox
  || Type == Widget::ItemBox
  || Type == Widget::Separation
  || Type == Widget::TextButton) {
    addThreePatch (box, typeName + state, options.horizontal, options.progress);

  // Widgets composed of nine patches (stretchable)
  } else if constexpr (Type == Widget::Box
  || Type == Widget::MultiLineTextBox
  || Type == Widget::WindowBox) {
    addNinePatch (box, typeName + state);

  // Icons
  } else if constexpr (Type == Widget::Icon) {
    auto newWidget = mTexturesUV.texture (typeName + options.name);
    appendMesh (std::move (newWidget), box);

  // Widgets composed of one block
  } else {
    auto newWidget = mTexturesUV.texture (typeName + state);
    appendMesh (std::move (newWidget), box, options.horizontal);
  }
}

/////////////////////////////////////////////////
template <Widget Type>
constexpr std::string GuiRender::toString () const 
{
  if constexpr (Type == Widget::Box) {
    return "Box";
  } else if constexpr (Type == Widget::WindowBox) {
    return "WindowBox";
  } else if constexpr (Type == Widget::TextBox) {
    return "TextBox";
  } else if constexpr (Type == Widget::MultiLineTextBox) {
    return "MultiLineTextBox";
  } else if constexpr (Type == Widget::ItemBox) {
    return "ItemBox";
  } else if constexpr (Type == Widget::MenuBox) {
    return "MenuBox";
  } else if constexpr (Type == Widget::MenuItemBox) {
    return "MenuItemBox";
  } else if constexpr (Type == Widget::TitleBox) {
    return "TitleBox";
  } else if constexpr (Type == Widget::Button) {
    return "Button";
  } else if constexpr (Type == Widget::TextButton) {
    return "TextButton";
  } else if constexpr (Type == Widget::IconButton) {
    return "IconButton";
  } else if constexpr (Type == Widget::TitleButton) {
    return "TitleButton";
  } else if constexpr (Type == Widget::CheckBox) {
    return "CheckBox";
  } else if constexpr (Type == Widget::Icon) {
    return "Icon";
  } else if constexpr (Type == Widget::SliderBar) {
    return "SliderBar";
  } else if constexpr (Type == Widget::ScrollerBar) {
    return "ScrollerBar";
  } else if constexpr (Type == Widget::Slider) {
    return "Slider";
  } else if constexpr (Type == Widget::Scroller) {
    return "Scroller";
  } else if constexpr (Type == Widget::ProgressBar) {
    return "ProgressBar";
  } else if constexpr (Type == Widget::ProgressFilling) {
    return "ProgressFilling";
  } else if constexpr (Type == Widget::Separation) {
    return "Separation";
  } else {
    return "None";
  }
}

} // namespace sgui
