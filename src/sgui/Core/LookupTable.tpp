namespace sgui
{
/////////////////////////////////////////////////
template <typename Value,
          typename Key>
void LookupTable <Value, Key>::clear ()
{
  mTable.clear ();
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
void LookupTable <Value, Key>::insert (
  const Key& key,
  Value&& value)
{
  mTable [key] = std::move (value);
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
void LookupTable <Value, Key>::insert (
  const Key& key,
  const Value& value)
{
  mTable [key] = value;
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
void LookupTable <Value, Key>::erase (
  const Key& key)
{
  mTable.erase (key);
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
Value& LookupTable <Value, Key>::at (
  const Key& key)
{
  return const_cast <Value&> (std::as_const (*this).at (key));
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
const Value& LookupTable <Value, Key>::at (
  const Key& key) const
{
  if (mTable.empty ()) {
    spdlog::error ("LookuTable::at: You are trying to access element in an empty look-up table, program will stop here.");
    assert (false);
  }
  if (mTable.find (key) == std::end (mTable)) {
    spdlog::warn ("LookupTable::at: Your key {} is not valid, will return first element", key);
    return std::begin (mTable)->second;
  }
  return mTable.at (key);
}

/////////////////////////////////////////////////
template <typename Value,
          typename Key>
bool LookupTable <Value, Key>::has (
  const Key& key) const
{
  return mTable.find (key) != std::end (mTable);
}

} // namespace sgui
