#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>

// module includes
#include "expression.hpp"

class Environment{
public:
  Environment();
  Expression updateEvaluate(Expression ast);
  void reset();
  std::vector<Atom> getGraphics();
private:

  // Environment is a mapping from symbols to expressions or procedures
  enum EnvResultType {ExpressionType, ProcedureType};
  struct EnvResult
  {
    EnvResultType type;
    Expression exp;
    Procedure proc;
  };
  //map
  std::map<Symbol, EnvResult> envmap;
  std::vector<Atom> graphics;

  //P2 method definitions
  Expression define(Expression exp);
  Expression begin(Expression exp);
  Expression evaluateIf(Expression exp);
  Expression evaluate(Expression ast);
  Expression simpleLogic(Expression exp);
  Expression complexLogic(Expression exp);
  bool simpleExpression(Expression exp);

  //P3 method definitions
  Expression drawGUI(Expression exp);
  Expression makeLine(Expression exp);
  Expression makePoint(Expression exp);
  Expression makeArc(Expression exp);

};

Expression procNot(const std::vector<Atom> & args);

Expression procAnd(const std::vector<Atom> & args);

Expression procOr(const std::vector<Atom> & args);

Expression procLessThan(const std::vector<Atom> & args);

Expression procLessThanEq(const std::vector<Atom> & args);

Expression procGreaterThan(const std::vector<Atom> & args);

Expression procGreaterThanEq(const std::vector<Atom> & args);

Expression procEqual(const std::vector<Atom> & args);

Expression procAdd(const std::vector<Atom> & args);

Expression procSubtractOrNeg(const std::vector<Atom> & args);

Expression procMultiply(const std::vector<Atom> & args);

Expression procDivide(const std::vector<Atom> & args);

Expression procLog10(const std::vector<Atom> & args);

Expression procPow(const std::vector<Atom> & args);

Expression procSin(const std::vector<Atom> & args);

Expression procCos(const std::vector<Atom> & args);

Expression procArctan(const std::vector<Atom> & args);




#endif
