#pragma once

#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>

// for readability
using json = nlohmann::json;

namespace sgui
{
/**
 * save/load json to file
 */
void saveInFile (const json& out, const std::string& file, const bool compact = false);
json loadFromFile (const std::string& file);

} // namespace sgui
