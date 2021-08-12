#include "tokenize.hpp"
#include <cctype>
#include <sstream>
#include <iostream>

TokenSequenceType tokenize(std::istream & seq)
{
	//create a TokenSequenceType, which is a deque
  	TokenSequenceType tokens;
    std::string line; 
  	while (std::getline(seq, line))  //while the input can still be tokenized
  	{
  		std::istringstream iss(line);
        std::string token;
        while (iss >> token)
        {
            bool added = false;
            bool comment = false; 
            std::string newToken;
            for (int i = 0; i < token.length(); i++)
            {
                std::string pToken;
                if (token[i] == ';') { //if the character is ";" break the loop, nothing after ; should be added. 
                    comment = true;
                }
                else if (token.length() == 1) { //if it is single character push straight to tokens
                    tokens.push_back(token);
                }
                else if (token[i] == '(') { //if the character is "(" push to tokens
                    pToken.push_back(token[i]);
                    tokens.push_back(pToken);
                }
                else if (token[i] == ')') { //if the character is ")" push to tokens
                    if (!added) //adding token before ) shows up
                    {
                        tokens.push_back(newToken);
                        added = true;
                    }
                    pToken.push_back(token[i]);
                    tokens.push_back(pToken);
                }
                else { //create a new string with out ( or )
                    newToken.push_back(token[i]);
                }
                if (comment) {
                    break;
                }
            }
            //if token has not been added, add it to tokens
            if (newToken.length() > 0 && !added) {
                tokens.push_back(newToken);
            }       
            if (comment) {
                break;
            }  
        }
    }
  	return tokens;
}
