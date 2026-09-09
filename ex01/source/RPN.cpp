#include "RPN.hpp"
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <cctype>
#include <stdexcept>

RPN::RPN()
{
}

RPN::RPN(RPN const &original) : _stackList(original._stackList)
{
}

RPN::~RPN()
{
}

RPN &RPN::operator=(RPN const &other)
{
	if (this != &other)
	{
		this->_stackList = other._stackList;
	}
	return *this;
}



void RPN::push(int value)
{
	this->_stackList.push(value);
}

static bool isOperator(const std::string &token)
{
	return token == "+" || token == "-" || token == "*" || token == "/";
}

static bool isNumber(const std::string &token)
{
	int isNegative = 0;

	if (token.empty())
		return false;
	if (token[0] == '-')
		isNegative = 1;
	for (size_t i = isNegative; i < token.size(); ++i)
	{
		if (!std::isdigit(static_cast<unsigned char>(token[i])))
			return false;
	}
	return true;
}

void RPN::handleInput(const std::string &input)
{
	std::istringstream iss(input);
	std::string token;
	while (iss >> token)
	{
		if (isOperator(token))
		{
			if (_stackList.size() < 2)
			{
				throw std::runtime_error("Error: Not enough operands for operation.");
			}
			int b = _stackList.top();
			_stackList.pop();
			int a = _stackList.top();
			_stackList.pop();
			if (token == "+")
				push(a + b);
			else if (token == "-")
				push(a - b);
			else if (token == "*")
				push(a * b);
			else if (token == "/")
			{
				if (b == 0)
					throw std::runtime_error("Error: Division by zero.");
				push(a / b);
			}
			// std::clog << "Performed operation: " << a << " " << token << " " << b << " = " << _stackList.top() << std::endl;
		}
		else if (isNumber(token))
		{
			int value = std::atoi(token.c_str());
			if (value == 0 && token != "0")
			{
				throw std::runtime_error("Error: Invalid token '" + token + "'.");
			}
			else if (value > 9)
			{
				throw std::runtime_error("Error: Numbers greater than 9 are not allowed.");
			}
			push(value);
		}
		else
		{
			throw std::runtime_error("Error: Invalid token '" + token + "'.");
		}
	}
}

void RPN::result()
{
	if (_stackList.size() != 1)
	{
		throw std::runtime_error("Error: Invalid RPN expression. Stack size is not 1.");
	}
	std::cout << _stackList.top() << std::endl;
}
