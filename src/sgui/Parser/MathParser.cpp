#include "MathParser.h"

#include <stack>
#include <cstdint>
#include <iostream>

namespace sgui
{
namespace Parser
{
////////////////////////////////////////////////////////////
std::string removeSpace (const std::string& expression)
{
  auto formula = std::string ();
  for (const auto& character : expression) {
    if (character != ' ') {
      if (character == '*') {
        formula.push_back (dot);
      } else {
        formula.push_back (character);
      }
    }
  }
  return formula;
}

////////////////////////////////////////////////////////////
Function parseNextFunction (const std::string& expression)
{
  // quit if first symbol is not a hashtag #
  if (expression.front () != '#') {
    return Function::None;
  }

  // search each function in the expression
  for (const auto& function : Functions) {
    // test if found function has the correct parentheses
    const auto found = expression.find (function.second);
    if (found == 1 && isNextParenthesesValid (expression, function.second.size () + 1)) {
      return function.first;
    }
  }

  // return no function
  return Function::None;
}

////////////////////////////////////////////////////////////
Symbol parseNextSymbol (const std::string& expression) 
{
  // quit if first symbol is not a hashtag #
  if (expression.front () != '#') {
    return Symbol::None;
  }

  // search each symbol in the expression
  for (const auto& symbol : Symbols) {
    // test if found symbol has the correct parentheses
    const auto& symbolStr = symbol.second.identifier;
    const auto found = expression.find (symbolStr);
    if (found == 1 && isNextParenthesesValid (expression, symbolStr.size () + 1)) {
      return symbol.first;
    }
  }

  // return no symbol
  return Symbol::None;
}

////////////////////////////////////////////////////////////
bool isNextParenthesesValid (const std::string& expression, int start) 
{
  // Check all pairs of parentheses
  std::stack<int> parentheses;
  for (uint32_t i = start; i < expression.size () - 1; i++) {
    if (expression[i] == '(') {
      parentheses.push (i);
    }
    if (expression[i] == ')') {
      // if a pair is broken, its invalid
      if (parentheses.empty ()) {
        return false;
      }
      parentheses.pop ();
    }
  }
  // if the only parenthesis remaining is the first one, its valid
  return !parentheses.empty () && parentheses.top () == start;
}

////////////////////////////////////////////////////////////
bool isOperator (const char c)
{
  return (c == '+') 
    || (c == '-') 
    || (c == '*') 
    || (c == dot) 
    || (c == '/') 
    || (c == '^')
    || (c == '_');
}

////////////////////////////////////////////////////////////
bool isDigit (const char c)
{
  return (c >= '0' && c <= '9');
}

////////////////////////////////////////////////////////////
bool isLitteral (const char c)
{
 return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

////////////////////////////////////////////////////////////
bool isParenthesis (const char c)
{
  return (c == '(' || c == ')');
}

////////////////////////////////////////////////////////////
/**
 * Translate Function or Symbol into string
 */
std::string toString (const Function function)
{
  return Functions.at (function);
}

/**
 * Translate Symbol into string
 */
std::string toString (const Symbol symbol)
{
  return Symbols.at (symbol).identifier;
}

/**
 * Translate Symbol into char
 */
char32_t toChar (const Symbol symbol)
{
  return Symbols.at (symbol).character;
}

} // namespace Parser
} // namespace sgui
