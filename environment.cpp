#include "environment.hpp"
#include "expression.hpp"
#include "interpreter.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

#include "interpreter_semantic_error.hpp"

//this is the constructor for the environment class
//instantiate all the procedures and expressions in the private variable map envmap
Environment::Environment()
{
  envmap["not"] = {ProcedureType, Expression(), &procNot};
  envmap["and"] = {ProcedureType, Expression(), &procAnd};
  envmap["or"] = {ProcedureType, Expression(), &procOr};
  envmap["<"] = {ProcedureType, Expression(), &procLessThan};
  envmap["<="] = {ProcedureType, Expression(), &procLessThanEq};
  envmap[">"] = {ProcedureType, Expression(), &procGreaterThan};
  envmap[">="] = {ProcedureType, Expression(), &procGreaterThanEq};
  envmap["="] = {ProcedureType, Expression(), &procEqual};
  envmap["+"] = {ProcedureType, Expression(), &procAdd};
  envmap["-"] = {ProcedureType, Expression(), &procSubtractOrNeg};
  envmap["*"] = {ProcedureType, Expression(), &procMultiply};
  envmap["/"] = {ProcedureType, Expression(), &procDivide};
  envmap["log10"] = {ProcedureType, Expression(), &procLog10};
  envmap["pow"] = {ProcedureType, Expression(), &procPow};
  envmap["sin"] = {ProcedureType, Expression(), &procSin};
  envmap["cos"] = {ProcedureType, Expression(), &procCos};
  envmap["arctan"] = {ProcedureType, Expression(), &procArctan};
  envmap["begin"] = {ProcedureType, Expression(), NULL};
  envmap["if"] = {ProcedureType, Expression(), NULL};
  envmap["define"] = {ProcedureType, Expression(), NULL};
  envmap["pi"] = {ExpressionType, Expression(atan2(0, -1)), NULL};

}

//this is the updateEvaluate method for the environment class
//public method to call the private evaluate method
Expression Environment::updateEvaluate(Expression ast)
{
	Expression exp = evaluate(ast);
	return exp; 	
}

//this is the evaluate method for the environment class
//private method to call that evaluates the the ast
Expression Environment::evaluate(Expression ast)
{
	Expression exp; 
	bool astCheck = false;
	if (ast.head.type == NumberType)
	{
		astCheck = true;
	}
	else if (ast.head.type == BooleanType)
	{
		astCheck = true;
	}
	else if (ast.head.value.sym_value == "begin")
	{
		exp = begin(ast);
	}
	else if (ast.head.value.sym_value == "define")
	{
		exp = define(ast);
	}
	else if (ast.head.value.sym_value == "if")
	{
		exp = evaluateIf(ast);
	}
	else if (ast.head.value.sym_value == "point")
	{
		exp = makePoint(ast);
	}
	else if (ast.head.value.sym_value == "line")
	{
		exp = makeLine(ast);
	}
	else if (ast.head.value.sym_value == "arc")
	{
		exp = makeArc(ast);
	}
	else if (ast.head.value.sym_value == "draw")
	{
		exp = drawGUI(ast);
	}
	else //is not special
	{
		if (simpleExpression(ast))
		{
			exp = simpleLogic(ast);
		}
		else //expression is complex
		{
			exp = complexLogic(ast);
		}
	}
	if (astCheck)
	{
		return ast;
	}
	return exp;
}

//this is the simpleLogic method for the environment class
//private method that evaluates simple logic for the ast
//i.e. tails of the ast do not have tails
Expression Environment::simpleLogic(Expression exp)
{
	std::map<Symbol, EnvResult>::iterator itr;
	itr = envmap.find(exp.head.value.sym_value);
	Expression newExp;
	//if you can not find the expression in the map, throw an error
	if (itr != envmap.end())
	{
		if (exp.tail.empty())
		{
			if (itr->second.type != ExpressionType)
			{
				throw InterpreterSemanticError("Error not an expression type");
			}
			return itr->second.exp;
		}
		//make a vector of atoms and loop through the tail filling the vector
		std::vector<Atom> args;
		for (size_t i = 0; i < exp.tail.size(); i++)
		{
			Atom a;
			if (exp.tail[i].head.type == BooleanType)
			{	
				a.type = BooleanType;
				a.value.bool_value = exp.tail[i].head.value.bool_value;
			}
			else if (exp.tail[i].head.type == NumberType)
			{
				a.type = NumberType;
				a.value.num_value = exp.tail[i].head.value.num_value;
			}
			else if (exp.tail[i].head.type == SymbolType)
			{		
				std::map<Symbol, EnvResult>::iterator newItr;
				newItr = envmap.find(exp.tail[i].head.value.sym_value);
				if (newItr == envmap.end())
				{
					throw InterpreterSemanticError("Error atom not defined");
				}
				Expression newExp = newItr->second.exp;
				if (newExp.head.type == BooleanType)
				{
					a.type = BooleanType;
					a.value.bool_value = newExp.head.value.bool_value;
				}
				else if (newExp.head.type == NumberType)
				{
					a.type = NumberType;
					a.value.num_value = newExp.head.value.num_value;
				}		
			}
			args.push_back(a);
		}
		newExp = itr->second.proc(args);		
	}
	else
	{
		throw InterpreterSemanticError("Error could not find procedure");
	}
	return newExp;	
}

//this is the complexLogic method for the environment class
//private method that evaluates complex logic for the ast
//i.e. tails of the ast do have tails
Expression Environment::complexLogic(Expression exp)
{
	std::map<Symbol, EnvResult>::iterator itr;
	itr = envmap.find(exp.head.value.sym_value);
	Expression returnExp;
	//if you can not find the expression in the map, throw an error
	if (itr != envmap.end()) {
		//make a vector of atoms and loop through the tail filling the vector
		int size = exp.tail.size();
		int i = 0;
		Expression tempExp;
		tempExp.head.type = exp.head.type;
		tempExp.head.value.sym_value = exp.head.value.sym_value;
		while (i < size) {
			std::vector<Atom> args;
			if (exp.tail[i].tail.empty()) {  //is just a number
				Expression newExp;
				newExp.head.type = NumberType;
				newExp.head.value.num_value = exp.tail[i].head.value.num_value;
				tempExp.tail.push_back(newExp);
			}
			else {
				for (size_t j = 0; j < exp.tail[i].tail.size(); j++) {
					Atom a;
					a.type = exp.tail[i].tail[j].head.type;
					if (exp.tail[i].tail[j].head.type == BooleanType) {	
						a.value.bool_value = exp.tail[i].tail[j].head.value.bool_value;
					}
					else if (exp.tail[i].tail[j].head.type == NumberType) {
						a.value.num_value = exp.tail[i].tail[j].head.value.num_value;
					}
					else if (exp.tail[i].tail[j].head.type == SymbolType) {
						Expression newExp = simpleLogic(exp.tail[i].tail[j]);
						a.type = NumberType;
						a.value.num_value = newExp.head.value.num_value;
					}
					args.push_back(a);
				}
				Expression newExp;
				std::map<Symbol, EnvResult>::iterator itr2;
				itr2 = envmap.find(exp.tail[i].head.value.sym_value);
				newExp = itr2->second.proc(args);
				tempExp.tail.push_back(newExp);
			}
			i++;
		}
		returnExp = simpleLogic(tempExp);	
	}
	else {
		throw InterpreterSemanticError("Error could not find procedure");
	}
	return returnExp;
}

//this is the simpleExpression method for the environment class
//private method that checks if the ast is simple
bool Environment::simpleExpression(Expression exp)
{
	//if the head type isnt a symbol throw an error, meaning it is a procedure
	if (exp.head.type != 4)
	{
		throw InterpreterSemanticError("Error invalid arguments");
	}
	//as long as the expression do not have tails
	bool simpleExpression = true;
	for (size_t i = 0; i < exp.tail.size(); i++)
	{
		if (!exp.tail[i].tail.empty())
		{
			simpleExpression = false;
		}
	}
	return simpleExpression;
}

//this is the begin method for the environment class
//method that evaluates the ast if the head is begin
Expression Environment::begin(Expression exp)
{
	//check that begin is the head of the exp
	if (exp.head.type != SymbolType)
	{
		throw InterpreterSemanticError("Error head does not start with begin/a symbol");
	}
	Expression returnExp;
	Expression newExp;
	for (size_t i = 0; i < exp.tail.size(); i++)
	{
		newExp = evaluate(exp.tail[i]);
		returnExp.tail.push_back(newExp);
	}
	return returnExp.tail[exp.tail.size() - 1];

}

//this is the define method for the environment class
//method that evaluates the ast if the head is define
Expression Environment::define(Expression exp)
{
	if (exp.tail.size() != 2) {
		throw InterpreterSemanticError("Error too many/less arguements");
	}	
	if (exp.tail[0].head.type != SymbolType) {
		throw InterpreterSemanticError("Error trying to define something that is not a symbol");
	}
	std::map<Symbol, EnvResult>::iterator itr;
	itr = envmap.find(exp.tail[0].head.value.sym_value);
	if (itr != envmap.end()) {
		throw InterpreterSemanticError("Error symbol has already been defined");
	}
	if (!exp.tail[1].tail.empty()) {  //complex, if what you are defining is an procedure, arc, line or point
		Expression newExp;
		std::string newSym = exp.tail[0].head.value.sym_value;
		if (exp.tail[1].head.value.sym_value == "arc") {
			newExp = makeArc(exp.tail[1]);
			envmap[newSym] = {ExpressionType, newExp, NULL};
		}
		else if (exp.tail[1].head.value.sym_value == "line") {
			newExp = makeLine(exp.tail[1]);
			envmap[newSym] = {ExpressionType, newExp, NULL};
		}
		else if (exp.tail[1].head.value.sym_value == "point") {
			newExp = makePoint(exp.tail[1]);
			envmap[newSym] = {ExpressionType, newExp, NULL};
		}
		else { //procedure 
			newExp = simpleLogic(exp.tail[1]);
			double newVal = newExp.head.value.num_value;
			envmap[newSym] = {ExpressionType, Expression(newVal), NULL};
		}
		itr = envmap.find(newSym);
	}
	else //if what you are defining is just a number or boolean
	{
		std::string newSym = exp.tail[0].head.value.sym_value;
		std::map<Symbol, EnvResult>::iterator newItr;
		newItr = envmap.find(exp.tail[1].head.value.sym_value);
		if (newItr != envmap.end()) {
			envmap[newSym] = {ExpressionType, newItr->second.exp, NULL};
		}
		else if (exp.tail[1].head.type == BooleanType) { //if a boolean
			bool newB = exp.tail[1].head.value.bool_value;
			envmap[newSym] = {ExpressionType, Expression(newB), NULL};		
		}
		else  { //a number
			double newVal = exp.tail[1].head.value.num_value;
			envmap[newSym] = {ExpressionType, Expression(newVal), NULL};
		}
		itr = envmap.find(newSym);
	}
	return itr->second.exp;
}


Expression Environment::evaluateIf(Expression exp)
{
	Expression newExp;
	if (simpleExpression(exp)) //if true or false
	{
		if (exp.tail[0].head.type != BooleanType) {
			throw InterpreterSemanticError("Error not a valid type");
		}
		if (exp.tail.size() != 3) {
			throw InterpreterSemanticError("Error too many or too less arguments");
		} 		
		newExp.head.type = NumberType;
		if (exp.tail[0].head.value.bool_value) {
			newExp.head.value.num_value = exp.tail[1].head.value.num_value;
		}
		else {
			newExp.head.value.num_value = exp.tail[2].head.value.num_value;
		}		
	}
	else //comparators
	{
		if (exp.tail.size() != 3) {
			throw InterpreterSemanticError("Error too many or too less arguments");
		}
		Expression returnExp; 
		for (size_t i = 0; i < exp.tail.size(); i++) {  //loop through the tail and simplify the expression
			if (!exp.tail[i].tail.empty()) {
				newExp = simpleLogic(exp.tail[i]);
				returnExp.tail.push_back(newExp);
			}
			else {
				returnExp.tail.push_back(exp.tail[i]);
			}
		}
		if (returnExp.tail[0].head.value.bool_value) { //depending on bool value of first tail, return second or third tail
			std::map<Symbol, EnvResult>::iterator itr;
			if (returnExp.tail[1].head.type == SymbolType) {
				itr = envmap.find(returnExp.tail[1].head.value.sym_value);
				Expression e = itr->second.exp;
				return e;
			}
			if (returnExp.tail[1].head.type == BooleanType || returnExp.tail[1].head.type == NumberType) {
				return returnExp.tail[1];
			}

		}
		else {  //is false
			std::map<Symbol, EnvResult>::iterator itr;
			if (returnExp.tail[2].head.type == SymbolType) {
				itr = envmap.find(returnExp.tail[2].head.value.sym_value);
				Expression e = itr->second.exp;
				return e;
			}
			if (returnExp.tail[2].head.type == BooleanType || returnExp.tail[2].head.type == NumberType) {
				return returnExp.tail[2];
			}
		}
	}
	return newExp;
}

//P3 method definitions
Expression Environment::drawGUI(Expression exp) {
	if (exp.head.type != SymbolType) {
		throw InterpreterSemanticError("Error head does not start with draw/a symbol");
	}
	Expression drawExp, newExp;
	std::map<Symbol, EnvResult>::iterator itr;
	for (size_t i = 0; i < exp.tail.size(); i++) {
		itr = envmap.find(exp.tail[i].head.value.sym_value);
		if (itr == envmap.end()) {
			newExp = evaluate(exp.tail[i]);
		}
		else {
			newExp = itr->second.exp;
		}
		drawExp.tail.push_back(newExp);
	}
	for (size_t i = 0; i < drawExp.tail.size(); i++) {  //create the vector of atoms, graphics
		Atom a;
		// if (drawExp.tail[i].head.type == BooleanType) {	
		// 	a.value.bool_value = drawExp.tail[i].head.value.bool_value;
		// 	a.type = BooleanType;
		// }
		// else if (drawExp.tail[i].head.type == NumberType) {
		// 	a.value.num_value = drawExp.tail[i].head.value.num_value;
		// 	a.type = NumberType;
		// }
		// else if (drawExp.tail[i].head.type == SymbolType) {
		// 	Expression newExp = simpleLogic(drawExp.tail[i]);
		// 	a.type = NumberType;
		// 	a.value.num_value = newExp.head.value.num_value;
		// }
		if (drawExp.tail[i].head.type == PointType) {
			a.value.point_value.x = drawExp.tail[i].head.value.point_value.x;
    		a.value.point_value.y = drawExp.tail[i].head.value.point_value.y;
    		a.type = PointType;
		}
		else if (drawExp.tail[i].head.type == LineType) {
			Point point1 = drawExp.tail[i].head.value.line_value.first;
    		Point point2 = drawExp.tail[i].head.value.line_value.second;
    		a.value.line_value.first.x = point1.x;
    		a.value.line_value.first.y = point1.y;
    		a.value.line_value.second.x = point2.x;
    		a.value.line_value.second.y = point2.y;			
			a.type = LineType;
		}
		else if (drawExp.tail[i].head.type == ArcType) {
			Point point1 = drawExp.tail[i].head.value.arc_value.center;
    		Point point2 = drawExp.tail[i].head.value.arc_value.start;
    		double angle = drawExp.tail[i].head.value.arc_value.span;
    		a.value.arc_value.center.x = point1.x;
    		a.value.arc_value.center.y = point1.y;		
    		a.value.arc_value.start.x = point2.x;
    		a.value.arc_value.start.y = point2.y;
    		a.value.arc_value.span = angle; 
    		a.type = ArcType;
		}
		graphics.push_back(a);
	}	
	Expression returnExp;
	return returnExp;					
}

//creates a point expression
Expression Environment::makePoint(Expression exp)
{
	std::tuple<double, double> Point; 
	if (exp.tail.size() != 2)
	{
		throw InterpreterSemanticError("Error invalid amount of arguments to make a point");
	}
	for (int i = 0; i < exp.tail.size(); i++)
	{
		std::map<Symbol, EnvResult>::iterator itr;
		itr = envmap.find(exp.tail[i].head.value.sym_value);
		if (itr != envmap.end()) //if the x or y coordinate is a procedure type
		{
			Expression newExp = simpleLogic(exp.tail[i]);
			if (i == 0)
			{
				std::get<0>(Point) = newExp.head.value.num_value;
			}
			else if (i == 1)
			{
				std::get<1>(Point) = newExp.head.value.num_value;
			}	
		}
		else //just a number
		{
			if (i == 0)
			{
				std::get<0>(Point) = exp.tail[i].head.value.num_value;
			}
			else if (i == 1)
			{
				std::get<1>(Point) = exp.tail[i].head.value.num_value;
			}
			
		}
	}
	//make the new point
	Expression pointExp = Expression(Point);
	return pointExp;
}

//creates a line expression
Expression Environment::makeLine(Expression exp)
{
	std::tuple<double, double> Point1; 
	std::tuple<double, double> Point2; 
	if (exp.tail.size() != 2)
	{
		throw InterpreterSemanticError("Error invalid amount of arguments to make a line");
	}
	for (int i = 0; i < exp.tail.size(); i++)
	{
		if (exp.tail[i].head.value.sym_value == "point")
		{
			Expression pointExp = makePoint(exp.tail[i]); 
			if (i == 0)
			{
				std::get<0>(Point1) = pointExp.head.value.point_value.x;
				std::get<1>(Point1) = pointExp.head.value.point_value.y;
			}
			else if (i == 1)
			{
				std::get<0>(Point2) = pointExp.head.value.point_value.x;
				std::get<1>(Point2) = pointExp.head.value.point_value.y;
			}
		}
		else if (exp.tail[i].head.type == SymbolType)
		{
			std::map<Symbol, EnvResult>::iterator itr;
			itr = envmap.find(exp.tail[i].head.value.sym_value);
			Expression lineExp = itr->second.exp;
			if (i == 0)
			{
				std::get<0>(Point1) = lineExp.head.value.point_value.x;
				std::get<1>(Point1) = lineExp.head.value.point_value.y;
			}
			else if (i == 1)
			{
				std::get<0>(Point2) = lineExp.head.value.point_value.x;
				std::get<1>(Point2) = lineExp.head.value.point_value.y;
			}
		}
		else
		{
			throw InterpreterSemanticError("Error Line must be made of points");
		}
	}
	Expression lineExp = Expression(Point1, Point2);
	return lineExp;
}

//creates an arc expression
Expression Environment::makeArc(Expression exp)
{
	std::tuple<double, double> Point1; 
	std::tuple<double, double> Point2; 
	double angle = 0;
	if (exp.tail.size() != 3) {
		throw InterpreterSemanticError("Error invalid amount of arguments to make an arc");
	}
	for (int i = 0; i < exp.tail.size(); i++)
	{
		std::map<Symbol, EnvResult>::iterator itr;
		itr = envmap.find(exp.tail[i].head.value.sym_value);
		if (itr != envmap.end()) //a part of the formula is a symbol
		{
			Expression pointExp = itr->second.exp;
			if (i == 0) {
				std::get<0>(Point1) = pointExp.head.value.point_value.x;
				std::get<1>(Point1) = pointExp.head.value.point_value.y;
			}
			else if (i == 1) {
				std::get<0>(Point2) = pointExp.head.value.point_value.x;
				std::get<1>(Point2) = pointExp.head.value.point_value.y;
			}
			else if (i == 2) {
				Expression newExp;
				if (simpleExpression(exp.tail[i]))
				{
					newExp = simpleLogic(exp.tail[i]);
				}
				else //expression is complex
				{
					newExp = complexLogic(exp.tail[i]);
				}
				angle = newExp.head.value.num_value;
			}
		}
		else
		{
			if (exp.tail[i].head.value.sym_value == "point") {
				Expression pointExp = makePoint(exp.tail[i]); 
				if (i == 0) {
					std::get<0>(Point1) = pointExp.head.value.point_value.x;
					std::get<1>(Point1) = pointExp.head.value.point_value.y;
				}
				else if (i == 1) {
					std::get<0>(Point2) = pointExp.head.value.point_value.x;
					std::get<1>(Point2) = pointExp.head.value.point_value.y;
				}
			}
			else if (i == 2) {
				angle = exp.tail[i].head.value.num_value;
			}
		}
	}
	Expression arcExp = Expression(Point1, Point2, angle);
	return arcExp;
}

std::vector<Atom> Environment::getGraphics()
{
	return graphics;
}

void Environment::reset()
{
	envmap.clear();
	envmap["not"] = {ProcedureType, Expression(), &procNot};
 	envmap["and"] = {ProcedureType, Expression(), &procAnd};
  	envmap["or"] = {ProcedureType, Expression(), &procOr};
  	envmap["<"] = {ProcedureType, Expression(), &procLessThan};
  	envmap["<="] = {ProcedureType, Expression(), &procLessThanEq};
  	envmap[">"] = {ProcedureType, Expression(), &procGreaterThan};
  	envmap[">="] = {ProcedureType, Expression(), &procGreaterThanEq};
  	envmap["="] = {ProcedureType, Expression(), &procEqual};
  	envmap["+"] = {ProcedureType, Expression(), &procAdd};
  	envmap["-"] = {ProcedureType, Expression(), &procSubtractOrNeg};
  	envmap["*"] = {ProcedureType, Expression(), &procMultiply};
  	envmap["/"] = {ProcedureType, Expression(), &procDivide};
  	envmap["log10"] = {ProcedureType, Expression(), &procLog10};
  	envmap["pow"] = {ProcedureType, Expression(), &procPow};
  	envmap["sin"] = {ProcedureType, Expression(), &procSin};
  	envmap["cos"] = {ProcedureType, Expression(), &procCos};
  	envmap["arctan"] = {ProcedureType, Expression(), &procArctan};
  	envmap["pi"] = {ExpressionType, Expression(atan2(0, -1)), NULL};
  	envmap["begin"] = {ProcedureType, Expression(), NULL};
  	envmap["if"] = {ProcedureType, Expression(), NULL};
  	envmap["define"] = {ProcedureType, Expression(), NULL};
}

//this is the procNot helper method for environment
//takes the inverse of the logic of the atom
Expression procNot(const std::vector<Atom> & args)
{
	Expression exp;
	if (args.size() > 1)
	{
		throw InterpreterSemanticError("Error too many arguments");
	}
	if (args[0].type == 1) //make sure type is BooleanType
	{
		
		if (!args[0].value.bool_value) //if false, make true
		{
			exp.head.type = BooleanType;
			exp.head.value.bool_value = true;
		}
		else //if true make false
		{
			exp.head.type = BooleanType;
			exp.head.value.bool_value = false;
		}
	}
	else
	{
		throw InterpreterSemanticError("Error not a BooleanType");
	}
	return exp;
	
}

//this is the procAnd helper method for the environment
//&& expression
Expression procAnd(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = BooleanType;
	exp.head.value.bool_value = true;
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i].type != BooleanType)
		{
			throw InterpreterSemanticError("Error not a BooleanType");
		}		
		if (!args[i].value.bool_value)
		{
			exp.head.value.bool_value = false;
			break;
		}
	}
	return exp;
}

//this is the procOr helper method for the environment
// || expression
Expression procOr(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = BooleanType;
	exp.head.value.bool_value = false;
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i].type != BooleanType)
		{
			throw InterpreterSemanticError("Error not a BooleanType");
		}	
		if (args[i].value.bool_value)
		{
			exp.head.value.bool_value = true;
			//break;
		}
	}
	return exp;
}

//this is the procLessThan helper method for the environment
//returns true if the first atom is less than the second
Expression procLessThan(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = BooleanType;
	if (args.size() != 2)
	{
		throw InterpreterSemanticError("Error too many/not enough arguments");
	}
	if (args[0].type != NumberType && args[1].type != NumberType)
	{
		throw InterpreterSemanticError("Error invalid types");
	}
	if (args[0].value.num_value < args[1].value.num_value)
	{
		exp.head.value.bool_value = true;
	}
	else
	{
		exp.head.value.bool_value = false;
	}
	return exp;
}

//this is the procLessThanEq helper method for the environment
//returns true if the first atom is less than or equal to the second
Expression procLessThanEq(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = BooleanType;
	if (args.size() != 2)
	{
		throw InterpreterSemanticError("Error too many/not enough arguments");
	}
	if (args[0].type != NumberType && args[1].type != NumberType)
	{
		throw InterpreterSemanticError("Error invalid types");
	}
	if (args[0].value.num_value <= args[1].value.num_value)
	{
		exp.head.value.bool_value = true;
	}
	else
	{
		exp.head.value.bool_value = false;
	}
	return exp;
}

//this is the procGreaterThan helper method for the environment
//returns true if the first atom is greater than to the second
Expression procGreaterThan(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = BooleanType;
	if (args.size() != 2)
	{
		throw InterpreterSemanticError("Error too many/not enough arguments");
	}
	if (args[0].type != NumberType && args[1].type != NumberType)
	{
		throw InterpreterSemanticError("Error invalid types");
	}
	if (args[0].value.num_value > args[1].value.num_value)
	{
		exp.head.value.bool_value = true;
	}
	else
	{
		exp.head.value.bool_value = false;
	}
	return exp;
}

//this is the procGreaterThanEq helper method for the environment
//returns true if the first atom is greater than or equal to the second
Expression procGreaterThanEq(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = BooleanType;
	if (args.size() != 2)
	{
		throw InterpreterSemanticError("Error too many/not enough arguments");
	}
	if (args[0].type != NumberType && args[1].type != NumberType)
	{
		throw InterpreterSemanticError("Error invalid types");
	}
	if (args[0].value.num_value >= args[1].value.num_value)
	{
		exp.head.value.bool_value = true;
	}
	else
	{
		exp.head.value.bool_value = false;
	}
	return exp;
}

//this is the procEqual helper method for the environment
//returns true if the first atom is equal to the second
Expression procEqual(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = BooleanType;
	if (args.size() != 2)
	{
		throw InterpreterSemanticError("Error too many/not enough arguments");
	}
	if (args[0].type != NumberType && args[1].type != NumberType)
	{
		throw InterpreterSemanticError("Error invalid types");
	}
	if (args[0].value.num_value == args[1].value.num_value)
	{
		exp.head.value.bool_value = true;
	}
	else
	{
		exp.head.value.bool_value = false;
	}
	return exp;
} 

//this is the procAdd helper method for the environment
//adds all the atoms num_values together, m-ary
Expression procAdd(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double sum = 0;
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}	
		sum = sum + args[i].value.num_value;
	}
	exp.head.value.num_value = sum;
	return exp;
}

//this is the procSubtractOrNeg helper method for the environment
//subtracts the first value from the second value if size == 2
//if size == 1, takes the negative of the num_value
Expression procSubtractOrNeg(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double diff = 0;
	if (args.size() == 1) //negation of the value
	{
		if (args[0].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}
		double number = args[0].value.num_value;
		exp.head.value.num_value = -(number);
	}	
	else if (args.size() == 2)//subtraction
	{
		if (args[0].type != 2 && args[1].type != 2)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}	
		diff = args[0].value.num_value - args[1].value.num_value;
		exp.head.value.num_value = diff;
	}
	else
	{
		throw InterpreterSemanticError("Error too many/less arguements");
	}	
	return exp;
} 

//this is the procMultiply helper method for the environment
//multiplies all the atoms num_values together, m-ary
Expression procMultiply(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double sum = 1;
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}	
		sum = sum * args[i].value.num_value;
	}
	exp.head.value.num_value = sum;
	return exp;
}

//this is the procDivide helper method for the environment
//divides the first value from the second value
Expression procDivide(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double div = 0;	
	if (args.size() == 2)//subtraction
	{
		if (args[0].type != NumberType && args[1].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}	
		div = args[0].value.num_value / args[1].value.num_value;
		exp.head.value.num_value = div;
	}
	else
	{
		throw InterpreterSemanticError("Error too many/less arguements");
	}
	return exp;
}

//this is the procLog10 helper method for the environment
//takes the log base 10 of the first value
Expression procLog10(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double num = 0;	
	if (args.size() == 1) 
	{
		if (args[0].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}	
		num = log10(args[0].value.num_value);
		exp.head.value.num_value = num;
	}
	else
	{
		throw InterpreterSemanticError("Error too many/less arguements");
	}
	return exp;
}

//this is the procPow helper method for the environment
//takes the power with the base of the first value, raised to the second value
Expression procPow(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double num = 0;	
	if (args.size() == 2)
	{
		if (args[0].type != NumberType && args[1].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}	
		num = pow(args[0].value.num_value, args[1].value.num_value);
		exp.head.value.num_value = num;
	}
	else
	{
		throw InterpreterSemanticError("Error too many/less arguements");
	}
	return exp;
}

Expression procSin(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double num = 0;	
	if (args.size() == 1)
	{
		if (args[0].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}
		num = sin(args[0].value.num_value);
		if (num < .001)
		{
			num = 0;
		}
		exp.head.value.num_value = num;
	}
	else
	{
		throw InterpreterSemanticError("Error too many/less arguements");
	}
	return exp;
}

Expression procCos(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double num = 0;	
	if (args.size() == 1)
	{
		if (args[0].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}	
		num = cos(args[0].value.num_value);
		exp.head.value.num_value = num;
	}
	else
	{
		throw InterpreterSemanticError("Error too many/less arguements");
	}
	return exp;
}

Expression procArctan(const std::vector<Atom> & args)
{
	Expression exp;
	exp.head.type = NumberType;
	double num = 0;	
	if (args.size() == 2)
	{
		if (args[0].type != NumberType || args[1].type != NumberType)
		{
			throw InterpreterSemanticError("Error not a NumberType");
		}	
		num = atan2(args[0].value.num_value, args[1].value.num_value);
		exp.head.value.num_value = num;
	}
	else
	{
		throw InterpreterSemanticError("Error too many/less arguements");
	}
	return exp;
}






