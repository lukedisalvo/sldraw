#include "catch.hpp"

#include <string>
#include <sstream>
#include <iostream>

#include "tokenize.hpp"

TEST_CASE( "Test Tokenizer with expected input", "[tokenize]" )
{

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);
  
  TokenSequenceType tokens = tokenize(iss);
  
  // for (int i = 0; i < tokens.size(); i++)
  // {
  //     std::cout << tokens[i] << " ";
  // }
  
  REQUIRE( tokens.size() == 17 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == "begin" );
  REQUIRE( tokens[2] == "(" );
  REQUIRE( tokens[3] == "define" );
  REQUIRE( tokens[4] == "r" );
  REQUIRE( tokens[5] == "10" );
  REQUIRE( tokens[6] == ")" );
  REQUIRE( tokens[7] == "(" );
  REQUIRE( tokens[8] == "*" );
  REQUIRE( tokens[9] == "pi" );
  REQUIRE( tokens[10] == "(" );
  REQUIRE( tokens[11] == "*" );
  REQUIRE( tokens[12] == "r" );
  REQUIRE( tokens[13] == "r" );
  REQUIRE( tokens[14] == ")" );
  REQUIRE( tokens[15] == ")" );
  REQUIRE( tokens[16] == ")" );
}

TEST_CASE( "Test Tokenizer with truncated input", "[tokenize]" ) {

  std::string program = "(f";

  std::istringstream iss(program);
  
  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 2 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == "f" );
}

TEST_CASE( "Test Tokenizer with single non-keyword", "[tokenize]" )
{

  std::string program = "hello";

  std::istringstream iss(program);

  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 1 );
  REQUIRE( tokens[0] == "hello" );
}

TEST_CASE( "Test Tokenizer with empty input", "[tokenize]" ) 
{

  std::string program;

  std::istringstream iss(program);

  TokenSequenceType tokens = tokenize(iss);
  
  //REQUIRE(tokens.size() == 0);
  REQUIRE(tokens.empty());
}

TEST_CASE( "Test Tokenizer with empty expression", "[tokenize]" ) 
{

  std::string program = "( )";

  std::istringstream iss(program);

  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 2 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == ")" );

}

TEST_CASE( "Test Tokenizer with ; in expression", "[tokenize]" ) 
{

  std::string program = "(we are going to stop; nothing after this)";

  std::istringstream iss(program);

  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 6 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == "we" );
  REQUIRE( tokens[2] == "are" );
  REQUIRE( tokens[3] == "going" );
  REQUIRE( tokens[4] == "to" );
  REQUIRE( tokens[5] == "stop" );
}

