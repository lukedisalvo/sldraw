#include "catch.hpp"

#include <string>
#include <iostream>

#include "expression.hpp"

TEST_CASE( "Test Type Inference", "[types]" ) 
{

  Atom a;
  
  //testing if token is True
  std::string token = "True";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == true);

  //testing if token is False
  token = "False";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == false);

  //testing if token is a positive number
  token = "1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1);
  
  //testing if token is a negative number
  token = "-1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == -1);

  //testing if token is positive number with a decimal point
  token = "2.75";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 2.75);

  //testing if token is negative number with a decimal point
  token = "-2.75";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == -2.75);

  //testing if token is a positive number in exponential form 
  token = "1e3";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1000);

  //testing if token is a number in exponential form (negative exponent)
  token = "1e-3";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1e-3);

  //testing if token is a negative number in exponential form
  token = "-1e-3";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == -1e-3);

  //testing if token is a valid symbol
  token = "var";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == "var");

  //testing if token is an invalid symbol
  token = "1abc";
  REQUIRE(!token_to_atom(token, a));

  //testing if token is valid symbol
  token = "var1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == token);

    //testing if token is valid symbol
  token = "begin";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == token);

}

TEST_CASE( "Test Expression Constructors", "[types]" ) 
{

  Expression exp1;

  REQUIRE(exp1 == Expression());
}

TEST_CASE( "Test Expression == operator for double", "[types]" ) 
{

  Expression exp1(2.0);

  REQUIRE(exp1 == Expression(2.0));

}

TEST_CASE( "Test Expression == operator for bool", "[types]" ) 
{

  Expression exp1(true);
  REQUIRE(exp1 == Expression(true));

  Expression exp2(false);
  REQUIRE(exp2 == Expression(false));

}

TEST_CASE( "Test Expression == operator for symbol", "[types]" ) 
{
  std::string test = "test";
  Expression exp1(test);
  REQUIRE(exp1 == Expression(test));

  std::string test2 = "anotherTest";
  Expression exp2(test2);
  REQUIRE(exp2 == Expression(test2));

}
TEST_CASE( "Test Expression == operator for tail", "[types]" ) 
{

  Expression exp1;
  exp1.head.type = BooleanType;
  exp1.head.value.bool_value = false;
  exp1.tail.push_back(true);
  exp1.tail.push_back(true);
  exp1.tail.push_back(true);

  Expression exp2;
  exp2.head.type = BooleanType;
  exp2.head.value.bool_value = false;
  exp2.tail.push_back(true);
  exp2.tail.push_back(true);
  exp2.tail.push_back(true);

  REQUIRE(exp1 == exp2);
}

TEST_CASE( "Test Expression == operator failures", "[types]" ) 
{

  //checking different head types
  Expression exp1(2.0);
  Expression exp2(false);
  bool check;
  if (exp1 == exp2)
  {
    //check = true;
  }
  else
  {
    check = false;
  }
  REQUIRE(check == false);

  //checking that they are type boolean, but of different values
  Expression exp3(true);
  Expression exp4(false);
  bool check2;
  if (exp3 == exp4)
  {
    //check2 = true;
  }
  else
  {
    check2 = false;
  }
  REQUIRE(check2 == false);

  //checking that they are type SymbolType, but of different values
  std::string test = "test";
  Expression exp5(test);
  Expression exp6(test);
  REQUIRE(exp5 == exp6);

  //checking that the tail size is the same
  Expression exp7;
  exp7.head.type = SymbolType;
  exp7.head.value.sym_value = "suh";
  Expression exp8;
  exp8.head.type = SymbolType;
  exp8.head.value.sym_value = "what21";
  bool check4;
  if (exp7 == exp8)
  {
    //check4 = true;
  }
  else
  {
    check4 = false;
  }
  REQUIRE(check4 == false);

  //checking that the tail size is the same
  Expression exp9(true);
  exp9.tail.push_back(false);
  exp9.tail.push_back(false);
  Expression exp10(true);
  bool check5;
  if (exp9 == exp10)
  {
    //check5 = true;
  }
  else
  {
    check5 = false;
  }
  REQUIRE(check5 == false);

  //checking expression of numbertype but different values
  Expression exp11(2.7);
  Expression exp12(2.0);
  bool check6;
  if (exp11 == exp12)
  {
    //check6 = true;
  }
  else
  {
    check6 = false;
  }
  REQUIRE(check6 == false);

  //checking that the tail size is the same
  Expression exp13(true);
  exp13.tail.push_back(false);
  exp13.tail.push_back(false);
  Expression exp14(true);
  exp14.tail.push_back(false);
  exp14.tail.push_back(false);
  REQUIRE(exp13 == exp14);

  //checking that the tail size is the same
  Expression exp15(2.0);
  exp15.tail.push_back(false);
  exp15.tail.push_back(false);
  Expression exp16(2.0);
  exp16.tail.push_back(false);
  bool check7;
  if (exp15 == exp16)
  {
    //check7 = true;
  }
  else
  {
    check7 = false;
  }
  REQUIRE(check7 == false);

  std::string test4 = "suh";
  Expression exp17(test4);
  bool yuh = true;
  Expression newExp(yuh);
  exp17.tail.push_back(newExp);
  Expression exp18(test);
  bool check8;
  if (exp17 == exp18)
  {
    //check8 = true;
  }
  else
  {
    check8 = false;
  }
  REQUIRE(check8 == false);



}

TEST_CASE( "Test Expression == operator test 3", "[types]" )
{

  Expression exp1;
  exp1.head.type = NumberType;
  exp1.head.value.num_value = 2.6;

  Expression exp2;
  exp2.head.type = NumberType;
  exp2.head.value.num_value = 2.6;

  REQUIRE(exp1 == exp2);

  Expression exp3;
  exp3.head.type = SymbolType;
  exp3.head.value.sym_value = "testing";

  Expression exp4;
  exp4.head.type = SymbolType;
  exp4.head.value.sym_value = "testing";
  REQUIRE(exp3 == exp4);

  Expression exp5;
  exp3.head.type = BooleanType;
  exp3.head.value.bool_value = true;

  Expression exp6;
  exp4.head.type = BooleanType;
  exp4.head.value.bool_value = true;
  REQUIRE(exp5 == exp6);
}


TEST_CASE( "Test Expression bool/double/string", "[types]" )
{

  bool test = false;
  Expression exp1(test);
  REQUIRE(exp1.head.type == BooleanType);
  REQUIRE(exp1.head.value.bool_value == false);

  double test2 = 2.5;
  Expression exp2(test2);
  REQUIRE(exp2.head.type == NumberType);
  REQUIRE(exp2.head.value.num_value == 2.5);

  std::string test3 = "yo";
  Expression exp3(test3);
  REQUIRE(exp3.head.type == SymbolType);
  REQUIRE(exp3.head.value.sym_value == "yo");

}

