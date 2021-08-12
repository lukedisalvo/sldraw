// system includes
#include <stack>
#include <stdexcept>
#include <iostream>


// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter.hpp"
#include "interpreter_semantic_error.hpp"

//this is the parse method for the Interpreter class
//determines if there is a valid parse, and then creates the ast
bool Interpreter::parse(std::istream & expression) noexcept
{
	TokenSequenceType tokens = tokenize(expression);
	bool flag = true;
	if (!validParse(tokens))
	{
		flag = false;
	}
	else
	{
		try
		{
			ast = readFromTokens(tokens);
		}
		catch (...)
		{
			flag = false;
		}
	}
 	return flag;
}

//this is the eval method for the Interpreter class
//evaluates the ast and returns the correct expression
Expression Interpreter::eval()
{
	Expression exp = env.updateEvaluate(ast);
  	return exp;
}

//this is the readFromTokens private method for the Interpreter class
//creates the ast
Expression Interpreter::readFromTokens(TokenSequenceType &tokens)
{		
	if (!tokens.empty())
	{
		std::string token = tokens.front();
		tokens.pop_front();
		Expression exp;
		if (token == "(")
		{
			token = tokens.front();
			tokens.pop_front();
			if (!token_to_atom(token, exp.head))
			{
				throw InterpreterSemanticError("Error Invalid atom");
			}
			while (tokens[0] != ")")
			{
				exp.tail.push_back(readFromTokens(tokens));
			}
			tokens.pop_front();
		}
		else
		{
			if (!token_to_atom(token, exp.head))
			{
				throw InterpreterSemanticError("Error Invalid atom");
			}	 
		}
		return exp;
	}

}

void Interpreter::setGraphics()
{
	graphics = env.getGraphics();
}

std::vector<Atom> Interpreter::getGraphics()
{
	return graphics;
}

//this is the validParse private method for the Interpreter class
//checks to see if the parse has the correct sequence
bool Interpreter::validParse(TokenSequenceType &tokens)
{
	if (tokens.size() < 3)
	{
		return false;
	}	
  	int openCount = 0;
    int closeCount = 0;
   	for (size_t i = 0; i < tokens.size(); i++)
    {
  		if (tokens[i] == "(")
  		{
            openCount++;
  		}
  		else if (tokens[i] == ")")
  		{
  			closeCount++;
  		}
  		if (openCount == closeCount && ((i + 1) != tokens.size()))
  		{
  			return false;
  		}	
  	}  
  	bool flag = false;
  	if (openCount == 0 || closeCount == 0)
    {
    	flag = false;
    }
   	if (openCount == closeCount)
    {
        flag = true;
    }
    return flag;

}

void Interpreter::reset()
{
	ast.head.type = NoneType;
	ast.tail.clear();
	env.reset();
}
