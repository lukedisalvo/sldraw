// system includes
#include "expression.hpp"
#include <cmath>
#include <limits>
#include <cctype>
#include <sstream>
#include <iostream>
#include <locale> 

//bool expression
Expression::Expression(bool tf)
{
    head.type = BooleanType;
    head.value.bool_value = tf;
    tail.clear();
}

//number expression
Expression::Expression(double num)
{
    head.type = NumberType;
    head.value.num_value = num;
    tail.clear();
}

//symbol expression
Expression::Expression(const std::string & sym)
{
    head.type = SymbolType;
    head.value.sym_value = sym;
    tail.clear();
}

//point expression
Expression::Expression(std::tuple<double,double> value)
{
    head.type = PointType;
    head.value.point_value.x = std::get<0>(value);
    head.value.point_value.y = std::get<1>(value);
    tail.clear();
}

//line expression
Expression::Expression(std::tuple<double,double> start, std::tuple<double,double> end)
{
    //set type to head and create two points
    head.type = LineType;
    Point point1;
    Point point2;

    //set the x and y values of the tuples
    point1.x = std::get<0>(start);
    point1.y = std::get<1>(start);
    point2.x = std::get<0>(end);
    point2.y = std::get<1>(end);

    //set the line values equal to the points
    head.value.line_value.first = point1;
    head.value.line_value.second = point2;
}

//arc expression
Expression::Expression(std::tuple<double,double> center, std::tuple<double,double> start, double angle)
{
    //set type to head and create two points
    head.type = ArcType;
    Point point1;
    Point point2;

    //set the x and y values of the tuples
    point1.x = std::get<0>(center);
    point1.y = std::get<1>(center);
    point2.x = std::get<0>(start);
    point2.y = std::get<1>(start);

    //set the arc values equal to the points, and the span to the angle
    head.value.arc_value.center = point1;
    head.value.arc_value.start = point2;
    head.value.arc_value.span = angle;
}

bool Expression::operator==(const Expression & exp) const noexcept
{
    bool flag = true;
    //check to see if the head type is the same
    if (head.type != exp.head.type)
    {
        flag = false;
    }
    else
    {
        //depending on the value of the type in the enum check to see if they are equal
        if (head.type == NoneType && exp.head.type == NoneType)
        {
            flag = true; 
        }   
        else if (head.type == BooleanType)
        {
            if (head.value.bool_value != exp.head.value.bool_value)
            {
                flag = false;
            }  
        }
        else if (head.type == NumberType)
        {    
            if (head.value.num_value != exp.head.value.num_value)
            {
                flag = false;
            }
        }
        else if (head.type == SymbolType)
        {
            if (head.value.sym_value != exp.head.value.sym_value)
            {
                flag = false;
            }
        }
    }
    if (tail.size() != exp.tail.size())
    {
        flag = false;
    }
    return flag;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp)
{
    // if (exp.head.type == 0)
    // {
    //     return out;
    // }  
    // else if (exp.head.type == 1)
    // {
    //     out << "(" << exp.head.value.bool_value << ")" << "->";
    // }
    // else if (exp.head.type == 2)
    // {
    //     out << "(" << exp.head.value.num_value << ")" << "->";
    // }
    // else if (exp.head.type == 4)
    // {
    //     out << "(" << exp.head.value.sym_value << ")" << "->";
    // }
    // for (size_t i = 0; i < exp.tail.size(); i++)
    // {
    //     if (exp.tail[i].head.type == 1)
    //     {
    //         out << "(" << exp.tail[i].head.value.bool_value << ")" << " ";
    //     }
    //     else if (exp.tail[i].head.type == 2)
    //     {
    //         out << "(" << exp.tail[i].head.value.num_value << ")" << " ";
    //     }
    //     else if (exp.tail[i].head.type == 4)
    //     {
    //         out << "(" << exp.tail[i].head.value.sym_value << ")" << " ";
    //     }
    // }
    // return out;
}

bool token_to_atom(const std::string & token, Atom & atom)
{
    bool flag;
    //return true if it a token is valid. otherwise, return false.
    if (token == "True") //if the token is True, set the atom properties accordingly
    {
        atom.type = BooleanType;
        atom.value.bool_value = true;
        flag = true;
    } 
    else if (token == "False") //if the token is false, set the atom properties accordingly
    {
        atom.type = BooleanType;
        atom.value.bool_value = false;
        flag = true;
    }
    else if (isNumber(token)) //if the token is a number, set the atom properties accordingly
    {
        double number = std::stod(token);
        atom.type = NumberType;
        atom.value.num_value = number;
        flag = true;
    }
    else if (isSymbol(token)) //if the token is symbol, set the atom properties accordingly
    {
        atom.type = SymbolType;
        atom.value.sym_value = token;
        flag = true;
    }
    else 
    {
        flag = false;
    }
    return flag; 
}

//helper method for expression.cpp
//determines if the string passed in is a symbol or not 
 //if the first character of the token isnt a character, return false
bool isSymbol(const std::string & token)
{ 
    return !my_isdigit(token[0]);
}

//helper method for expression.cpp
//determines if the string passed in a number or not
bool isNumber(const std::string & token)
{
    bool exponential = false;
    bool negative = false; 
    bool decimal = false;
    for (int i = 0; i < token.length(); i++)
    {
        if (token[i] == 'e' || token[i] == 'E')
        {
            exponential = true;
        } 
        else if (token[i] == '-' && token.length() > 1)
        {
            negative = true; 
        } 
        else if (token[i] == '.')
        {
            decimal = true;
        } 
        else if (!my_isdigit(token[i]))
        {
            return false;
        }
    }
    return true;
}

bool my_isdigit(char ch)
{
    std::locale loc;
    return std::isdigit(ch, loc);
}

