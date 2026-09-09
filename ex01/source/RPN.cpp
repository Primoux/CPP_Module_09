#include "RPN.hpp"
#include <sstream>
#include <cstdlib>
#include <iostream>

RPN::RPN()
{
}

RPN::~RPN()
{
}

RPN &RPN::operator=(const RPN &other)
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

void RPN::handleInput(const std::string &input)
{
	std::istringstream iss(input);
	std::string token;
	while (iss >> token)
	{
		if (token == "+" || token == "-" || token == "*" || token == "/")
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
		else
		{
			int value = std::atoi(token.c_str());
			if (value == 0 && token != "0")
			{
				throw std::runtime_error("Error: Invalid token '" + token + "'.");
			}
			else if (value > 10)
			{
				throw std::runtime_error("Error: Numbers greater than 10 are not allowed.");
			}
			push(value);
		}
	}
}

void RPN::result()
{
	if (_stackList.size() != 1)
	{
		throw std::runtime_error("Error: Invalid RPN expression. Stack size is not 1.");
	}
	std::cout << "Result: " << _stackList.top() << std::endl;
}
