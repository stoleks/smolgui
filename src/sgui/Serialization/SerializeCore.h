/**
  serializeCore.h
  Purpose : Implement to/from json conversion for LookupTable and ObjectPool.
  @author A. J.
*/

#pragma once

#include <unordered_map>
#include <nlohmann/json.hpp>

#include "sgui/Core/ObjectPool.h"

// for readability
using json = nlohmann::json;

namespace sgui
{
/**
 * convert unordered_map  to/from json
 */
template <typename Key, typename Type>
void to_json (json& j, const std::unordered_map <Key, Type>& table)
{
  // note that user should use NLOHMANN_JSON_SERIALIZE_ENUM if they
  // want to have human readable enumeration in json if Key is an Enum
  for (const auto& element : table) {
    j [element.first] = element.second;
  }
}

template <typename Key, typename Type>
void from_json (const json& j, std::unordered_map <Key, Type>& table)
{
  // note that user should use NLOHMANN_JSON_SERIALIZE_ENUM if they
  // want to have human readable enumeration in json if Key is an Enum
  for (auto elem = j.begin (); elem != j.end (); elem++) {
    const Key key = elem.key ();
    const Type value = elem.value ();
    table.insert ({key, value});
  }
}


/**
 * convert object pool to/from json
 */
template <typename Object, typename ObjectId>
void to_json (json& j, const ObjectPool<Object, ObjectId>& pool)
{
  for (const auto id : pool.ids ()) {
    j [id] = pool.get (id);
  }
}

template <typename Object, typename ObjectId>
void from_json (const json& j, ObjectPool<Object, ObjectId>& pool)
{
  for (auto elem = j.begin (); elem != j.end (); elem++) {
    const ObjectId id = elem.key ();
    const Object value = elem.value ();
    pool.add (value, id);
  }
}

} // namespace sgui
