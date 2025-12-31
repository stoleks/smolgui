#pragma once

#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>

// for readability
using json = nlohmann::json;

namespace sgui
{
/**
 * @brief save json to file
 */
void saveInFile (const json& out, const std::string& file, const bool compact = false);

/**
 * @brief load json from file
 */
json loadFromFile (const std::string& file);

} // namespace sgui
