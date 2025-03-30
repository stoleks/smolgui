namespace sgui
{
/////////////////////////////////////////////////
template <typename Resource, typename Identifier>
template <typename... Args>
bool ResourcesHolder <Resource, Identifier>::load (
  const Identifier& id,
  const std::string& filename,
  Args&&... args)
{
  // For Layout or TextContainer, data can be stored in separate files
  if constexpr (std::is_same_v <Resource, Layout> || std::is_same_v <Resource, TextContainer>) {
    // add data to the previously loaded ones
    const auto res = m_resources.find (id);
    if (res != std::end (m_resources)) {
      if (!res->second->loadFromFile (filename, std::forward <Args> (args)...)) {
        return false;
      }
      // store file name for reload
      m_filesPath.at (id).push_back (filename);
      return true;
    }
  }
  // For all other type, information are stored in one file
  auto newRes = std::make_unique <Resource> ();
  if constexpr (std::is_same_v <Resource, sf::Font>) {
    if (!newRes->openFromFile (filename, std::forward <Args> (args)...)) {
      return false;
    }
  } else {
    if (!newRes->loadFromFile (filename, std::forward <Args> (args)...)) {
      return false;
    }
  }
  // hold it
  holdResource (std::move (newRes), id, filename);
  return true;
}

/////////////////////////////////////////////////
template <typename Resource, typename Identifier>
template <typename... Args>
bool ResourcesHolder<Resource, Identifier>::reload (
  const Identifier& id,
  Args&&... args)
{
  // get previously loaded resource
  const auto res = m_resources.find (id);
  const auto filesPath = m_filesPath.find (id);
  // reload it from all its files
  if (filesPath != std::end (m_filesPath) && res != std::end (m_resources)) {
    res->second = std::move (std::make_unique <Resource> ());
    for (const auto& file : filesPath->second) {
      if constexpr (std::is_same_v <Resource, sf::Font>) {
        return res->second->openFromFile (file, std::forward <Args> (args)...);
      } else {
        return res->second->loadFromFile (file, std::forward <Args> (args)...);
      }
    }
  } else {
    spdlog::error ("ResourcesHolder::reload (): id {} was not previously loaded", id);
  }
  return false;
}

/////////////////////////////////////////////////
template <typename Resource, typename Identifier>
const Resource& ResourcesHolder <Resource, Identifier>::get (const Identifier& id) const
{
  // Code crash if user forgot to load a resource
  if (m_resources.empty ()) {
    spdlog::error ("ResourcesHolder::get (): you tryied to access element in an empty resource holder.");
    assert (false);
  }
  // Return asked resource OR the first one stored if it doesn't exist
  const auto resource = m_resources.find (id);
  if (resource != std::end (m_resources)) {
    return *(resource->second);
  }
  spdlog::error ("ResourcesHolder::get (): can't find resource {}, will return first element", id);
  return *(std::begin (m_resources)->second);
}

/////////////////////////////////////////////////
template <typename Resource, typename Identifier>
Resource& ResourcesHolder <Resource, Identifier>::get (const Identifier& id)
{
  return const_cast <Resource&> (std::as_const (*this).get (id));
}


/////////////////////////////////////////////////
template <typename Resource, typename Identifier>
void ResourcesHolder <Resource, Identifier>::holdResource (
  std::unique_ptr <Resource>&& resource,
  const Identifier& id,
  const std::string& filename)
{
  // Store resource in the container
  const auto res = m_resources.insert (std::make_pair (id, std::move (resource)));
  const auto files = std::vector <std::string> { filename };
  const auto path = m_filesPath.insert (std::make_pair (id, files));
  // this code should never be executed in theory...
  if (!res.second || !path.second) {
    spdlog::error ("ResourcesHolder::holdResource (): failed to insert resource {} and its path. Is id unique ?", id);
    assert (false);
  }
}

} // namespace sgui
