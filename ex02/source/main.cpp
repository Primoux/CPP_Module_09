#include <unistd.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "PmergeMe.hpp"

int main (int argc, char **argv)
{
	std::cout << std::fixed << std::setprecision(0);
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <positive integers>" << std::endl;
		return 2;
	}
	PmergeMe pmergeMe;
	try
	{
		pmergeMe.parseInput(argc, argv);
		pmergeMe.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}

	return 0;
}
