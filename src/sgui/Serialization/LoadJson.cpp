#include "sgui/Serialization/LoadJson.h"

namespace sgui
{
/////////////////////////////////////////////////
void saveInFile (
  const json& out,
  const std::string& file,
  const bool compact)
{
  auto output = std::ofstream (file);
  if (compact) {
    output << out << std::endl;
  } else {
    output << std::setw (2) << out << std::endl;
  }
  output.close ();
}

/////////////////////////////////////////////////
json loadFromFile (const std::string& file)
{
  auto input = std::ifstream (file);
  auto j = json::parse (input);
  input.close ();
  return j;
}

} // namespace sgui
