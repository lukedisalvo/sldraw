
// TODO (clwyatt): tests for parsing partial numbers e.g. 1abc

// TODO (clwyatt): more coverage of exceptions

#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp"

static Expression run(const std::string & program)
{
  
  std::istringstream iss(program);
    
  Interpreter interp;
    
  bool ok = interp.parse(iss);
  if(!ok){
    std::cerr << "[GRADER] Failed to parse: " << program << std::endl; 
  }
  REQUIRE(ok == true);

  Expression result;
  REQUIRE_NOTHROW(result = interp.eval());

  return result;
}

Expression runfile(const std::string & fname)
{

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  if(!ok){
    std::cerr << "[GRADER] Failed to parse file: " << fname << std::endl; 
  }
  REQUIRE(ok == true);

  Expression result;
  REQUIRE_NOTHROW(result = interp.eval());

  return result;
}

//PASSED
TEST_CASE( "Test Interpreter parser with expected input", "[interpreter]" ) 
{

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);
 
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == true);
}

//PASSED
TEST_CASE( "Test Interpreter parser with numerical literals", "[interpreter]" ) 
{

  std::vector<std::string> programs = {"(1)", "(+1)", "(+1e+0)", "(1e-0)"};
  
  for(auto program : programs)
  {
    std::istringstream iss(program);
 
    Interpreter interp;

    bool ok = interp.parse(iss);

    REQUIRE(ok == true);
  }
}

//PASSED
TEST_CASE( "Test Interpreter parser with truncated input", "[interpreter]" ) 
{

  {
    std::string program = "(f";
    std::istringstream iss(program);
  
    Interpreter interp;
    bool ok = interp.parse(iss);
    REQUIRE(ok == false);
  }

  {
    std::string program = "(begin (define r 10) (* pi (* r r";
    std::istringstream iss(program);

    Interpreter interp;
    bool ok = interp.parse(iss);
    REQUIRE(ok == false);
  }
}

//PASSED
TEST_CASE( "Test Interpreter parser with extra input", "[interpreter]" ) 
{

  std::string program = "(begin (define r 10) (* pi (* r r))) )";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

//PASSED
TEST_CASE( "Test Interpreter parser with single non-keyword", "[interpreter]" ) 
{

  std::string program = "hello";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

//PASSED
TEST_CASE( "Test Interpreter parser with empty input", "[interpreter]" )
 {

  std::string program;
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

//PASSED
TEST_CASE( "Test Interpreter parser with empty expression", "[interpreter]" ) 
{

  std::string program = "( )";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

//PASSED
TEST_CASE( "Test Interpreter parser with bad number string", "[interpreter]" )
 {

  std::string program = "(1abc)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

//PASSED
TEST_CASE( "Test Interpreter parser with incorrect input. Regression Test", "[interpreter]" ) 
{

  std::string program = "(+ 1 2) (+ 3 4)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok == false);
}

//EXAMPLE, PASSED
TEST_CASE( "Test parse with example in spec", "[interpreter]" ) 
{
    std::string program = "(begin (define a 1) (define b pi) (if (< a b) b a))";
    std::istringstream iss(program);
  
    Interpreter interp;

    bool ok = interp.parse(iss);

    REQUIRE(ok == true);
}

//PASSED
TEST_CASE( "Test Interpreter result with literal expressions", "[interpreter]" ) 
{

  { // Boolean True
    std::string program = "(True)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  { // Boolean False
    std::string program = "(False)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }
  
  { // Number
    std::string program = "(4)";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }

  { // Symbol
    std::string program = "(pi)";
    Expression result = run(program);
    REQUIRE(result == Expression(atan2(0, -1)));
  }

}

//PASSED
TEST_CASE( "Test Interpreter result with simple procedures (add)", "[interpreter]" ) 
{

  { // add, binary case
    std::string program = "(+ 1 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(3.));
  }
  
  { // add, 3-ary case
    std::string program = "(+ 1 2 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  { // add, 6-ary case
    std::string program = "(+ 1 2 3 4 5 6)";
    Expression result = run(program);
    REQUIRE(result == Expression(21.));
  }
}
  
//PASSED
TEST_CASE( "Test Interpreter special form: if", "[interpreter]" ) 
{

  {
    std::string program = "(if True (4) (-4))";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }
  
  {
    std::string program = "(if False (4) (-4))";
    Expression result = run(program);
    REQUIRE(result == Expression(-4.));
  }
} 

//PASSED
TEST_CASE( "Test 4 from slsp, if", "[interpreter]" ) 
{
    std::string program = "(if (< 1 2) (- 5 6) True)";
    Expression result = run(program);
    REQUIRE(result == Expression(-1.));
}

//PASSED
TEST_CASE( "Test Interpreter special forms: begin and define", "[interpreter]" ) 
{

  {
    std::string program = "(define answer 42)";
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer 42)\n(answer))";
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }
  
  {
    std::string program = "(begin (define answer (+ 9 11)) (answer))";
    Expression result = run(program);
    REQUIRE(result == Expression(20.));
  }

  {
    std::string program = "(begin (define a 1) (define b 1) (+ a b))";
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }
}

//PASSED
TEST_CASE( "Test a complex expression", "[interpreter]" ) 
{
  {
    std::string program = "(+ (+ 10 1) (+ 30 (+ 1 1)))";
    Expression result = run(program);
    REQUIRE(result == Expression(43.));
  }
}

//PASSED
TEST_CASE( "Test relational procedures", "[interpreter]" ) 
{

  {
    std::vector<std::string> programs = {"(< 1 2)",
           "(<= 1 2)",
           "(<= 1 1)",
           "(> 2 1)",
           "(>= 2 1)",
           "(>= 2 2)",
           "(= 4 4)"};
    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(true));
    }
  }

  {
    std::vector<std::string> programs = {"(< 2 1)",
           "(<= 2 1)",
           "(<= 1 0)",
           "(> 1 2)",
           "(>= 1 2)",
           "(>= 2 3)",
           "(= 0 4)"};
    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(false));
    }
  }
}

//PASSED
TEST_CASE( "Test arithmetic procedures", "[interpreter]" ) {

  {
    std::vector<std::string> programs = {"(+ 1 -2)",
           "(+ -3 1 1)",
           "(- 1)",
           "(- 1 2)",
           "(* 1 -1)",
           "(* 1 1 -1)",
           "(/ -1 1)",
           "(/ 1 -1)"};
    for(auto s : programs)
    {
      Expression result = run(s);
      REQUIRE(result == Expression(-1.));
    }
  }
}

//PASSED
TEST_CASE( "Test logical procedures", "[interpreter]" )
 {

  REQUIRE(run("(not True)") == Expression(false));
  REQUIRE(run("(not False)") == Expression(true));

  REQUIRE(run("(and True True)") == Expression(true));
  REQUIRE(run("(and True False)") == Expression(false));
  REQUIRE(run("(and False True)") == Expression(false));
  REQUIRE(run("(and False False)") == Expression(false));
  REQUIRE(run("(and True True False)") == Expression(false));

  REQUIRE(run("(or True True)") == Expression(true));
  REQUIRE(run("(or True False)") == Expression(true));
  REQUIRE(run("(or False True)") == Expression(true));
  REQUIRE(run("(or False False)") == Expression(false));
  REQUIRE(run("(or True True False)") == Expression(true));
}

TEST_CASE( "Test trig procedures", "[interpreter]" ) {

  REQUIRE(run("(sin pi)") == Expression(0.));
  REQUIRE(run("(cos pi)") == Expression(-1.));
  REQUIRE(run("(arctan 1 0)") == Expression(atan2(1,0)));

  std::vector<std::string> programs = {"(sin 0 1)",
               "(cos 0 1)", 
               "(arctan 0)",
               "(sin True)",
               "(cos True)",
               "(arctan 1 False)"};
  for(auto s : programs){
    Interpreter interp;
    std::istringstream iss(s);
    
    REQUIRE(interp.parse(iss));
    
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
  }
}

//PASSED
TEST_CASE( "Test some semantically invalid expresions", "[interpreter]" ) 
{
  
  std::vector<std::string> programs = {"(@ none)", // no such procedure
               "(- 1 1 2)", // too many arguments
               "(define if 1)", // redefine special form
               "(define pi 3.14)"}; // redefine builtin symbol
    for(auto s : programs){
      Interpreter interp;

      std::istringstream iss(s);
      
      bool ok = interp.parse(iss);

      REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

//PASSED
TEST_CASE( "Test math ", "[interpreter]" ) 
{
  REQUIRE(run("(log10 10)") == Expression(1.));
  REQUIRE(run("(log10 100)") == Expression(2.));
  REQUIRE(run("(log10 1000)") == Expression(3.));

  REQUIRE(run("(pow 10 1)") == Expression(10.));
  REQUIRE(run("(pow 10 2)") == Expression(100.));
  REQUIRE(run("(pow 10 3)") == Expression(1000.));
}

//PASSED
TEST_CASE( "Test some semantically invalid math expresions", "[interpreter]" ) 
{
  
  std::vector<std::string> programs = {"(log10)",
               "(log10 a)",
               "(log10 1 2 3)",
               "(pow 10)",
               "(pow 10 1 2)",
               "(pow 10 a)",
               "(pow a 10)",
               "(pow a b)",
  };
    for(auto s : programs){
      Interpreter interp;
      std::istringstream iss(s);
      
      bool ok = interp.parse(iss);

      REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

TEST_CASE("Test graphic types", "[interpreter]") 
{

  {
    std::string program = "(point 0 0)";
    std::istringstream iss(program);
    Interpreter interp;

    REQUIRE(interp.parse(iss));
    REQUIRE(interp.eval() == Expression(std::make_tuple(0., 0.)));
  }
  {
    std::string program = "(line (point 0 0) (point 10 0))";
    std::istringstream iss(program);
    Interpreter interp;

    REQUIRE(interp.parse(iss));
    REQUIRE(interp.eval() ==
            Expression(std::make_tuple(0., 0.), std::make_tuple(10., 0.)));
  }
  {
    std::string program = "(arc (point 0 0) (point 10 0) pi)";
    std::istringstream iss(program);
    Interpreter interp;

    REQUIRE(interp.parse(iss));
    REQUIRE(interp.eval() == Expression(std::make_tuple(0., 0.),
                                        std::make_tuple(10., 0.),
                                        atan2(0, -1)));
  }
}

//PASSED
TEST_CASE( "Test file tests/test0.slp", "[interpreter]" ) 
{

  std::string fname = TEST_FILE_DIR + "/test0.slp";

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  REQUIRE(ok == false);
}

//PASSED
TEST_CASE( "Test syntactically INCORRECT files", "[interpreter]" ) 
{

  std::string fname = TEST_FILE_DIR + "/test1.slp";

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  REQUIRE(ok == false);
}

TEST_CASE( "Test syntactically correct file using CRLF (Windows) line endings.", "[interpreter]" ) {

  std::string input = TEST_FILE_DIR + "/test_crlf.slp";

  std::string expect = input + ".expected";
  Expression result = runfile(input);
  Expression expected_result = runfile(expect);
  
  REQUIRE(result == expected_result);
  REQUIRE(result == Expression(-1.));
}

// //PASSED
// TEST_CASE( "Test all syntactically and semantically CORRECT files. Test 2", "[interpreter]" ) 
// {

//     std::string base = TEST_FILE_DIR + "/test";
//     std::string input = base + std::to_string(2) + ".slp";
//     std::string expect = input + ".expected";
//     Expression result = runfile(input);
//     Expression expected_result = runfile(expect);
  
//     REQUIRE(result == expected_result);
// }

// // PASSED
// TEST_CASE( "Test all syntactically and semantically CORRECT files. Test 3", "[interpreter]" ) 
// {

//     std::string base = TEST_FILE_DIR + "/test";
//     std::string input = base + std::to_string(3) + ".slp";
//     std::string expect = input + ".expected";
//     Expression result = runfile(input);
//     Expression expected_result = runfile(expect);
  
//     REQUIRE(result == expected_result);
// }

// //PASSED 
// TEST_CASE( "Test all syntactically and semantically CORRECT files. Test 4", "[interpreter]" ) 
// {

//     std::string base = TEST_FILE_DIR + "/test";
//     std::string input = base + std::to_string(4) + ".slp";
//     std::string expect = input + ".expected";
//     Expression result = runfile(input);
//     Expression expected_result = runfile(expect);
  
//     REQUIRE(result == expected_result);
// }

// //NOT PASSED
// TEST_CASE( "Test all syntactically and semantically CORRECT files. Test 5", "[interpreter]" ) 
// {
//     std::string base = TEST_FILE_DIR + "/test";
//     std::string input = base + std::to_string(5) + ".slp";
//     std::string expect = input + ".expected";
//     Expression result = runfile(input);
//     Expression expected_result = runfile(expect);
  
//     REQUIRE(result == expected_result);
// }

//PASSED
TEST_CASE( "Test all syntactically and semantically CORRECT files.", "[interpreter]" ) 
{

  const int START_TEST = 2;
  const int LAST_TEST = 5;
  std::string base = TEST_FILE_DIR + "/test";
  
  for(int i = START_TEST; i <= LAST_TEST; ++i){
    
    std::string input = base + std::to_string(i) + ".slp";
    std::string expect = input + ".expected";
    Expression result = runfile(input);
    Expression expected_result = runfile(expect);
  
    REQUIRE(result == expected_result);
  }
}

//PASSED
TEST_CASE( "Test reset method", "[interpreter]" ) 
{

    std::string program = "(begin (define a 1) (define b 1) (+ a b))";
        
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);

    REQUIRE(ok == true);

    Expression result;
    REQUIRE_NOTHROW(result = interp.eval());

    interp.reset();

    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
 
}

//PASSED
TEST_CASE( "Test invalid defines", "[interpreter]" ) 
{

    std::string program = "(define a 12 a)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(define 12 12)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);
 
}

//PASSED
TEST_CASE( "Test if method", "[interpreter]" ) 
{

    std::string program = "(if 12 1 2)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(if True 1 2 3)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

    std::string program3 = "(if (< 1 2) 1 2 3)";
    Interpreter interp3;
    std::istringstream iss3(program2);
    bool ok3 = interp3.parse(iss3);
    REQUIRE(ok3 == true);
    REQUIRE_THROWS_AS(interp3.eval(), InterpreterSemanticError);

    //when a complex if is false
    std::string program4 = "(if (< 2 1) 1 2)";
    Expression result = run(program4);
    REQUIRE(result == Expression(2.0));
 
}

//PASSED
TEST_CASE( "Test invalid not", "[interpreter]" ) 
{
    std::vector<Atom> v;

    std::string program = "(Not True True True)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(not 12)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

    std::string program3 = "(not sup)";
    Interpreter interp3;
    std::istringstream iss3(program3);
    bool ok3 = interp3.parse(iss3);
    REQUIRE(ok3 == true);
    REQUIRE_THROWS_AS(interp3.eval(), InterpreterSemanticError);
 
}

//PASSED
TEST_CASE( "Test invalid and", "[interpreter]" ) 
{

    std::string program = "(and True 12 12)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(and 12 12 True)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);
 
}

//PASSED
TEST_CASE( "Test invalid or", "[interpreter]" ) 
{

    std::string program = "(or True 12 sup)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(or 12 sup True)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);
 
}

//PASSED
TEST_CASE( "Test invalid less than/ greater than", "[interpreter]" ) 
{

    std::string program = "(< 1 1 2)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(< True sup)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

    std::string program3 = "(> 1 1 2)";
    Interpreter interp3;
    std::istringstream iss3(program3);
    bool ok3 = interp3.parse(iss3);
    REQUIRE(ok3 == true);
    REQUIRE_THROWS_AS(interp3.eval(), InterpreterSemanticError);

    std::string program4 = "(> True sup)";
    Interpreter interp4;
    std::istringstream iss4(program4);
    bool ok4 = interp4.parse(iss4);
    REQUIRE(ok4 == true);
    REQUIRE_THROWS_AS(interp4.eval(), InterpreterSemanticError);
 
}

//PASSED
TEST_CASE( "Test invalid less than/ greater than equal", "[interpreter]" ) 
{

    std::string program = "(<= 1 1 2)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(<= True sup)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

    std::string program3 = "(>= 1 1 2)";
    Interpreter interp3;
    std::istringstream iss3(program3);
    bool ok3 = interp3.parse(iss3);
    REQUIRE(ok3 == true);
    REQUIRE_THROWS_AS(interp3.eval(), InterpreterSemanticError);

    std::string program4 = "(>= True sup)";
    Interpreter interp4;
    std::istringstream iss4(program4);
    bool ok4 = interp4.parse(iss4);
    REQUIRE(ok4 == true);
    REQUIRE_THROWS_AS(interp4.eval(), InterpreterSemanticError);
 
}

//PASSED
TEST_CASE( "Test invalid equal", "[interpreter]" ) 
{

    std::string program = "(= 1 1 2)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(= True sup)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);
}

//PASSED
TEST_CASE( "Test invalid add", "[interpreter]" ) 
{

    std::string program = "(+ 1 True sup)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(+ True sup)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

}

//PASSED
TEST_CASE( "Test invalid subtraction and neg", "[interpreter]" ) 
{

    std::string program = "(- sup)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(- True sup)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

}

//PASSED
TEST_CASE( "Test invalid mulitply", "[interpreter]" ) 
{

    std::string program = "(* sup True)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(* True sup 1)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

}

//PASSED
TEST_CASE( "Test invalid divide", "[interpreter]" ) 
{

    std::string program = "(/ sup True)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(/ 1 2 1)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

}

//PASSED
TEST_CASE( "Test invalid log10", "[interpreter]" ) 
{

    std::string program = "(log10 True)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(log10 sup)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

}

//PASSED
TEST_CASE( "Test invalid pow", "[interpreter]" ) 
{

    std::string program = "(pow True sup)";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::string program2 = "(pow sup 1)";
    Interpreter interp2;
    std::istringstream iss2(program2);
    bool ok2 = interp2.parse(iss2);
    REQUIRE(ok2 == true);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

}

//PASSED
TEST_CASE( "Test test_arc file.", "[interpreter]" ) 
{
    std::string input = TEST_FILE_DIR + "/test_arc.slp";
    Expression result = runfile(input);
    Expression expected;
    REQUIRE(result == expected);

}

//PASSED
TEST_CASE( "Test test_arc_simple file.", "[interpreter]" ) 
{
    std::string input = TEST_FILE_DIR + "/test_arc_simple.slp";
    Expression result = runfile(input);
    Expression expected;
    REQUIRE(result == expected);
}

//PASSED
TEST_CASE( "Test test_badeval file.", "[interpreter]" ) 
{
    std::string input = TEST_FILE_DIR + "/test_badeval.slp";
    std::ifstream ifs(input);
    REQUIRE(ifs.good() == true);
  
    Interpreter interp;
    
    bool ok = interp.parse(ifs);
    if(!ok)
    {
        std::cerr << "[GRADER] Failed to parse file: " << input << std::endl; 
    }
    REQUIRE(ok == true);

    Expression result;
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
}

//PASSED
TEST_CASE( "Test test_badparse file.", "[interpreter]" ) 
{
    std::string input = TEST_FILE_DIR + "/test_badparse.slp";
    std::ifstream ifs(input);
    REQUIRE(ifs.good() == true);
  
    Interpreter interp;
    
    bool ok = interp.parse(ifs);
    REQUIRE(ok == false);
}

//PASSED
TEST_CASE( "Test test_car file.", "[interpreter]" ) 
{
    std::string input = TEST_FILE_DIR + "/test_car.slp";
    Expression result = runfile(input);
    Expression expected;
    REQUIRE(result == expected);
}

//PASSED
TEST_CASE( "Test test_line file.", "[interpreter]" ) 
{
    std::string input = TEST_FILE_DIR + "/test_line.slp";
    Expression result = runfile(input);
    Expression expected;
    REQUIRE(result == expected);
}

//PASSED
TEST_CASE( "Test test_point file.", "[interpreter]" ) 
{
    std::string input = TEST_FILE_DIR + "/test_point.slp";
    Expression result = runfile(input);
    Expression expected;
    REQUIRE(result == expected);
}

//PASSED
TEST_CASE( "Test define point/line/arc", "[interpreter]" ) 
{
    std::tuple<double, double> Point1; 
    std::get<0>(Point1) = 1;
    std::get<1>(Point1) = 1;
    std::string program = "(define a (point 1 1))";
    Expression result = run(program);

    REQUIRE(result == Expression(Point1));

    std::tuple<double, double> Point2;
    std::get<0>(Point2) = 5;
    std::get<1>(Point2) = 5;

    std::string program2 = "(define a (line (point 1 1) (point 5 5)))";
    Expression result2 = run(program2);

    REQUIRE(result2 == Expression(Point1, Point2));

    double angle = 3.14;
    std::string program3 = "(define a (arc (point 1 1) (point 5 5) angle))";
    Expression result3 = run(program3);
    REQUIRE(result3 == Expression(Point1, Point2, angle));   
}

//PASSED
TEST_CASE( "Test  invalid point/line/arc", "[interpreter]" ) 
{

    std::tuple<double, double> Point1; 
    std::get<0>(Point1) = 1;
    std::get<1>(Point1) = 1;
    std::string program = "(define a (point 1 1 1))";
    Interpreter interp;
    std::istringstream iss(program);
    bool ok = interp.parse(iss);
    REQUIRE(ok == true);
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);

    std::tuple<double, double> Point2;
    std::get<0>(Point2) = 5;
    std::get<1>(Point2) = 5;

    Interpreter interp2;
    std::string program2 = "(define a (line (point 1 1) (point 5 5) (point 10 10)))";
    std::istringstream iss2(program2);
    bool ok2 = interp.parse(iss2);
    REQUIRE_THROWS_AS(interp2.eval(), InterpreterSemanticError);

    Interpreter interp3;
    double angle = 3.14;
    std::string program3 = "(define a (arc (point 1 1) (point 5 5) angle angle2))";
    std::istringstream iss3(program3);
    bool ok3 = interp.parse(iss3);
    REQUIRE_THROWS_AS(interp3.eval(), InterpreterSemanticError);
}



