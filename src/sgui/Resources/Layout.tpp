namespace sgui
{
/////////////////////////////////////////////////
template <typename LayoutType>
bool Layout::has (const std::string& entry) const
{
  const auto end = std::string::npos;
  for (const auto& set : m_setFilenames) {
    for (const auto& key : m_layoutEntries.at (set)) {
      if (key.find (layoutTypeName <LayoutType> ()) != end && key.find (entry) != end) {
        return true;
      }
    }
  }
  return false;
}

/////////////////////////////////////////////////
template <typename LayoutType>
LayoutType& Layout::get (
  const std::string& entry,
  bool addLayoutType)
{
  return const_cast <LayoutType&> (std::as_const (*this).get <LayoutType> (entry, addLayoutType));
}

/////////////////////////////////////////////////
template <typename LayoutType>
const LayoutType& Layout::get (
  const std::string& entry,
  bool addLayoutType) const
{
  // get key. We need these two cases for serialization ease
  auto key = entry;
  if (addLayoutType) {
    key = layoutTypeName <LayoutType> () + entry;
  }

  // return requested entry
  if constexpr (std::is_same_v <LayoutType, Panel>) {
    return m_entries.at (key).panel;
  } else if constexpr (std::is_same_v <LayoutType, sf::Vector2f>) {
    return m_entries.at (key).position;
  } else if constexpr (std::is_same_v <LayoutType, Window>) {
    return m_entries.at (key).window;
  } else {
    return m_entries.at (key).constraints;
  }
}

} // namespace sgui
