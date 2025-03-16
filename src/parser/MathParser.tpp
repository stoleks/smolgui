namespace sgui
{
namespace Parser
{
////////////////////////////////////////////////////////////
/**
 *  Macro to ease registration of symbols and functions
 */
#define FunctionEntry(Value) { Function::Value, #Value }
#define SymbolEntry(Value)   { Symbol::Value,   {#Value, Value} }

////////////////////////////////////////////////////////////
/**
 * Store all identifier for functions parsed
 */
const std::unordered_map <Function, std::string> Functions =
{
  FunctionEntry (ln),
  FunctionEntry (log),
  FunctionEntry (exp),
  FunctionEntry (sqrt),
  FunctionEntry (cos),
  FunctionEntry (sin),
  FunctionEntry (tan),
  FunctionEntry (cotan),
  FunctionEntry (arccos),
  FunctionEntry (arcsin),
  FunctionEntry (arctan),
  FunctionEntry (arccot)
};

////////////////////////////////////////////////////////////
/**
 * Store all character and identifier for symbols parsed
 */
const std::unordered_map <Symbol, SymbolData> Symbols =
{
  SymbolEntry (Alpha),
  SymbolEntry (alpha),
  SymbolEntry (Beta),
  SymbolEntry (beta),
  SymbolEntry (Gamma),
  SymbolEntry (gamma),
  SymbolEntry (Delta),
  SymbolEntry (delta),
  SymbolEntry (Epsilon),
  SymbolEntry (epsilon),
  SymbolEntry (Zeta),
  SymbolEntry (zeta),
  SymbolEntry (Eta),
  SymbolEntry (eta),
  SymbolEntry (Theta),
  SymbolEntry (theta),
  SymbolEntry (Iota),
  SymbolEntry (iota),
  SymbolEntry (Kappa),
  SymbolEntry (kappa),
  SymbolEntry (Lambda),
  SymbolEntry (lambda),
  SymbolEntry (Mu),
  SymbolEntry (mu),
  SymbolEntry (Nu),
  SymbolEntry (nu),
  SymbolEntry (Xi),
  SymbolEntry (xi),
  SymbolEntry (Omicron),
  SymbolEntry (omicron),
  SymbolEntry (Pi),
  SymbolEntry (pi),
  SymbolEntry (Rho),
  SymbolEntry (rho),
  SymbolEntry (Sigma),
  SymbolEntry (sigma),
  SymbolEntry (Tau),
  SymbolEntry (tau),
  SymbolEntry (Upsilon),
  SymbolEntry (upsilon),
  SymbolEntry (Phi),
  SymbolEntry (phi),
  SymbolEntry (Chi),
  SymbolEntry (chi),
  SymbolEntry (Psi),
  SymbolEntry (psi),
  SymbolEntry (Omega),
  SymbolEntry (omega),
  SymbolEntry (time),
  SymbolEntry (dot),
  SymbolEntry (pm),
  SymbolEntry (neq),
  SymbolEntry (sim),
  SymbolEntry (less),
  SymbolEntry (leq),
  SymbolEntry (greater),
  SymbolEntry (geq)
};

} // namespace Parser
} // namespace sgui
