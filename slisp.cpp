#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter.hpp"

static Expression run(const std::string & program, Interpreter &interp, bool &caught)
{
  	std::istringstream iss(program); 
    Expression result;
  	bool ok = interp.parse(iss);
  	if(!ok)
  	{
   		std::cout << "Error could not parse" << std::endl;
  	}
  	try
	{
		result = interp.eval();
	}
	catch (const std::exception & ex) 
	{
		interp.reset();
		caught = true;
      	std::cerr << ex.what() << std::endl;
    }		
  	return result;
}

Expression runfile(const std::string & fname, Interpreter &interp, bool &caught)
{
  	std::ifstream ifs(fname);
  	if (!ifs.good())
  	{
  		std::cout << "Error filename is not found" << std::endl;
  	}
    Expression result;
  	bool ok = interp.parse(ifs);
  	if (!ok)
  	{
    	std::cout << "Error could not parse" << std::endl;
  	}
  	try
	{
		result = interp.eval();
	}
	catch (const std::exception & ex) 
	{
		interp.reset();
		caught = true;
      	std::cerr << ex.what() << std::endl;
    }	
  	return result;
}

void print(Expression exp)
{
	if (exp.head.type == NumberType)
	{
		std::cout << "(" << exp.head.value.num_value << ")" <<  std::endl;
	}
	else if (exp.head.type == BooleanType)
	{
		if (exp.head.value.bool_value)
		{
			std::cout << "(True)" <<  std::endl;
		}
		else
		{
			std::cout << "(False)" <<  std::endl;
		}
		
	}
	else if (exp.head.type == SymbolType)
	{
		std::cout << "(" << exp.head.value.sym_value << ")" <<  std::endl;
	}	
}

int main(int argc, char **argv)
{
	Expression result;
	Interpreter interp;
	bool caught;
	while (true)
	{
		if (std::cin.eof()) //if the input in the EOF input, break
		{
			break;
		}
		if (argc == 1)
		{
			std::string input;	
			std::cout << "slisp> ";
			std::getline(std::cin, input);
			if (std::cin.eof()) //if the input in the EOF input, break
			{
				break;
			}
			result = run(input, interp, caught);
			if (!caught)
			{
				print(result);
			}
		}
		else if (argc == 2) //for a file
		{
			std::string input = std::string(argv[1]);
			if (input.substr(input.length() - 4, input.length()) == ".slp")
			{
				result = runfile(input, interp, caught);
				if (!caught)
				{
					print(result);
					return EXIT_SUCCESS;
				}	
			}
			else
			{
				result = run(input, interp, caught);
			}
		}
		else if (argc == 3)
		{
			if (std::string(argv[1]) != "-e")
			{
				//return EXIT_FAILURE;
			}
			std::string input = std::string(argv[2]);
			result = run(input, interp, caught);
			if (!caught)
			{
				print(result);
				return EXIT_SUCCESS;
			}

		}
		
	}
}
