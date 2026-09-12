#include <unistd.h>
#include <iostream>
#include <string>
#include "PmergeMe.hpp"

int main (int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <positive integers>" << std::endl;
		return 1;
	}
	PmergeMe pmergeMe;

	try
	{
		pmergeMe.parseInput(argc, argv);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}
