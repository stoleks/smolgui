#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

namespace sgui
{
/**
 * For widgets that need a pair of begin and end function call
 */
enum class GroupType {
  Window,
  Panel,
  Menu
};

namespace Impl
{
/**
 * @brief class to store simple counters
 */
struct Counters
{
  /**
   * @brief reset all counter
   */
  void reset () { *this = Counters (); }
  // data
  uint32_t plot = 0u;
  uint32_t group = 0u;
  uint32_t widget = 0u;
  uint32_t comboBox = 0u;
};

/**
 * @brief handles error messages for user misusing window, panel or menu
 */
class GroupChecker
{
public:
  /**
   * @brief initialize internals counters
   */
  GroupChecker ();
  /**
   * @brief reset all counters and display an error message if required
   */
  void reset ();
  /**
   * @brief keep track of begin calls
   */
  void begin (const GroupType group);
  /**
   * @brief keep track of end calls
   */
  void end (const GroupType group);
private:
  // impl
  void reset (const GroupType group);
  std::string name (const GroupType group);
private:
  std::unordered_map <GroupType, uint32_t> mCounters;
};

} // namespace Impl
} // namespace sgui
