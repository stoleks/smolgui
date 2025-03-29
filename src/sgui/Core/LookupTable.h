#pragma once

#include <string>
#include <algorithm>
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace sgui
{
/////////////////////////////////////////////////
template <typename Value,
          typename Key = std::string>
class LookupTable
{
public:
  /**
   * @brief default constructor for serialization
   */
  LookupTable () = default;
  /**
   * @brief clear data of the table
   */
  void clear ();
  /**
   * @brief insert by moving entry to the table
   */
  void insert (
         const Key& key,
         Value&& value);
  /**
   * @brief insert by copying entry to the table
   */
  void insert (
         const Key& key,
         const Value& value);
  /**
   * @brief erase entry if it exist
   */
  void erase (const Key& key);
  /**
   * @brief get mutable entry value at corresponding key
   */
  Value& at (const Key& key);
  /**
   * @brief get entry value at corresponding key
   */
  const Value& at (const Key& key) const;
  /**
   * @brief check if entry exist for this key
   */
  bool has (const Key& key) const;
  /**
   * to iterate through the table
   */
  auto begin () { return std::begin (mTable); }
  auto end   () { return std::end (mTable); }
  auto begin () const { return std::cbegin (mTable); }
  auto end   () const { return std::cend (mTable); }
private:
  std::unordered_map <Key, Value> mTable;
};

} // namespace sgui

#include "LookupTable.tpp"
