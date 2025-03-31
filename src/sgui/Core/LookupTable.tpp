namespace sgui
{
/////////////////////////////////////////////////
template <typename Value,
          typename Key>
void LookupTable <Value, Key>::clear ()
{
  m_table.clear ();
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
void LookupTable <Value, Key>::insert (const Key& key,
  Value&& value)
{
  m_table [key] = std::move (value);
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
void LookupTable <Value, Key>::insert (const Key& key,
  const Value& value)
{
  m_table [key] = value;
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
void LookupTable <Value, Key>::erase (const Key& key)
{
  m_table.erase (key);
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
Value& LookupTable <Value, Key>::at (const Key& key)
{
  return const_cast <Value&> (std::as_const (*this).at (key));
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
const Value& LookupTable <Value, Key>::at (const Key& key) const
{
  if (m_table.empty ()) {
    spdlog::error ("LookuTable::at: You are trying to access element in an empty look-up table, program will stop here.");
    assert (false);
  }
  if (m_table.find (key) == std::end (m_table)) {
    spdlog::warn ("LookupTable::at: Your key {} is not valid, will return first element", key);
    return std::begin (m_table)->second;
  }
  return m_table.at (key);
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
bool LookupTable <Value, Key>::has (const Key& key) const
{
  return m_table.find (key) != std::end (m_table);
}

} // namespace sgui
