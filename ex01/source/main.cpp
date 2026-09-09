#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2 || std::string(argv[1]).empty())
	{
		std::cerr << "Error: Invalid number of arguments." << std::endl;
		return 2;
	}
	std::string input = argv[1];
	RPN rpn;
	try
	{
		rpn.handleInput(input);
		rpn.result();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
