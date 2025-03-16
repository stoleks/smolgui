#pragma once

#include <array>
#include <string>
#include <unordered_map>

namespace sgui 
{
namespace Parser
{
////////////////////////////////////////////////////////////
/**
 * Mathematical symbols
 */
constexpr char32_t sqrt    = L'√';
constexpr char32_t time    = L'×';
constexpr char32_t dot     = L'·';
constexpr char32_t pm      = L'±';
constexpr char32_t neq     = L'≠';
constexpr char32_t sim     = L'≈';
constexpr char32_t less    = L'<';
constexpr char32_t leq     = L'≤';
constexpr char32_t greater = L'>';
constexpr char32_t geq     = L'≥';

////////////////////////////////////////////////////////////
/**
 * Greek letters
 */
constexpr char32_t Alpha   = L'Α';
constexpr char32_t alpha   = L'α';
constexpr char32_t Beta    = L'Β';
constexpr char32_t beta    = L'β';
constexpr char32_t Gamma   = L'Γ';
constexpr char32_t gamma   = L'γ';
constexpr char32_t Delta   = L'Δ';
constexpr char32_t delta   = L'δ';
constexpr char32_t Epsilon = L'Ε';
constexpr char32_t epsilon = L'ε';
constexpr char32_t Zeta    = L'Ζ';
constexpr char32_t zeta    = L'ζ';
constexpr char32_t Eta     = L'Η';
constexpr char32_t eta     = L'η';
constexpr char32_t Theta   = L'Θ';
constexpr char32_t theta   = L'θ';
constexpr char32_t Iota    = L'Ι';
constexpr char32_t iota    = L'ι';
constexpr char32_t Kappa   = L'Κ';
constexpr char32_t kappa   = L'κ';
constexpr char32_t Lambda  = L'Λ';
constexpr char32_t lambda  = L'λ';
constexpr char32_t Mu      = L'Μ';
constexpr char32_t mu      = L'μ';
constexpr char32_t Nu      = L'Ν';
constexpr char32_t nu      = L'ν';
constexpr char32_t Xi      = L'Ξ';
constexpr char32_t xi      = L'ξ';
constexpr char32_t Omicron = L'Ο';
constexpr char32_t omicron = L'ο';
constexpr char32_t Pi      = L'Π';
constexpr char32_t pi      = L'π';
constexpr char32_t Rho     = L'Ρ';
constexpr char32_t rho     = L'ρ';
constexpr char32_t Sigma   = L'Σ';
constexpr char32_t sigma   = L'σ';
constexpr char32_t Tau     = L'Τ';
constexpr char32_t tau     = L'τ';
constexpr char32_t Upsilon = L'Υ';
constexpr char32_t upsilon = L'υ';
constexpr char32_t Phi     = L'Φ';
constexpr char32_t phi     = L'φ';
constexpr char32_t Chi     = L'Χ';
constexpr char32_t chi     = L'χ';
constexpr char32_t Psi     = L'Ψ';
constexpr char32_t psi     = L'ψ';
constexpr char32_t Omega   = L'Ω';
constexpr char32_t omega   = L'ω';


////////////////////////////////////////////////////////////
/**
 * Parsed Functions
 */
enum class Function {
  None,
  // Display '√'
  sqrt,
  // Display func name
  ln,
  log,
  exp,
  cos,
  sin,
  tan,
  cotan,
  arccos,
  arcsin,
  arctan,
  arccot
};

/**
 * Parsed symbols
 */
enum class Symbol {
  None,
  // Greek
  Alpha,
  alpha,
  Beta,
  beta,
  Gamma,
  gamma,
  Delta,
  delta,
  Epsilon,
  epsilon,
  Zeta,
  zeta,
  Eta,
  eta,
  Theta,
  theta,
  Iota,
  iota,
  Kappa,
  kappa,
  Lambda,
  lambda,
  Mu,
  mu,
  Nu,
  nu,
  Xi,
  xi,
  Omicron,
  omicron,
  Pi,
  pi,
  Rho,
  rho,
  Sigma,
  sigma,
  Tau,
  tau,
  Upsilon,
  upsilon,
  Phi,
  phi,
  Chi,
  chi,
  Psi,
  psi,
  Omega,
  omega,
  // Mathematic
  time,
  dot,
  pm,
  neq,
  sim,
  less,
  leq,
  greater,
  geq
};


////////////////////////////////////////////////////////////
/**
 * Hold string identifier and char linked to symbol
 */
struct SymbolData
{
  SymbolData () = default;
  SymbolData (const std::string& id, char32_t c) : character (c), identifier (id) {}
  // data
  char32_t character;
  std::string identifier;
};


////////////////////////////////////////////////////////////
/**
 * Remove useless space in a mathematical expression
 */
std::string removeSpace (const std::string& expression);

/**
 * Check for next function in mathematical formula
 */
Function parseNextFunction (const std::string& expression);

/**
 * Check for next symbol in mathematical formula
 */
Symbol parseNextSymbol (const std::string& expression);

/**
 * Check if next parentheses are in pair
 */
bool isNextParenthesesValid (const std::string& expression, int start = 0);

/**
 * Check if character is an arithmetic operator +, -, /, *
 */
bool isOperator (const char c);

/**
 * Check if character is a litteral a, b, c, etc.
 */
bool isLitteral (const char c);

/**
 * Translate Function or Symbol into string
 */
std::string toString (const Function function);

/**
 * Translate Symbol into string
 */
std::string toString (const Symbol symbol);

/**
 * Translate Symbol into char
 */
char32_t toChar (const Symbol symbol);

} // namespace Parser
} // namespace sgui

#include "MathParser.tpp"
