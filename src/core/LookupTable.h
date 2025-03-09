/**
  LookupTable.h
  Purpose: define generic look-up table, that map a key to a specific value
  @author A. J.
*/

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
   * default constructor for serialization
   */
  LookupTable () = default;
  /**
   * clear data of the table
   */
  void clear ();
  /**
   * insert or erase entry in the table
   */
  void insert (
         const Key& key,
         Value&& value);
  void insert (
         const Key& key,
         const Value& value);
  void erase (const Key& key);
  /**
   * get entry value at corresponding key
   */
  Value& at (const Key& key);
  const Value& at (const Key& key) const;
  /**
   * return true if Key exist
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
