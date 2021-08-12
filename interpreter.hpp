#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
#include <string>
#include <istream>


// module includes
#include "expression.hpp"
#include "environment.hpp"
#include "tokenize.hpp"

// Interpreter has
// Environment, which starts at a default
// parse method, builds an internal AST
// eval method, updates Environment, returns last result
class Interpreter
{
public:
  bool parse(std::istream & expression) noexcept;
  Expression eval();
  void setGraphics();
  std::vector<Atom> getGraphics();
  void reset();

private:
  Environment env;
  Expression ast;
  std::vector<Atom> graphics;

  //helper methods
  Expression readFromTokens(TokenSequenceType &tokens);
  bool validParse(TokenSequenceType &tokens);

};

#endif
