#include "PmergeMe.hpp"
#include <sstream>
#include <iostream>

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe& other)
{
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
	}
	return *this;
}

PmergeMe::~PmergeMe()
{
}

std::vector<int> PmergeMe::parseInput(int argc, char** argv)
{
	std::vector<int> input;
	for (int i = 1; i < argc; ++i)
	{
		std::istringstream iss(argv[i]);
		int value;
		if (!(iss >> value) || value < 0 || !iss.eof())
		{
			throw std::invalid_argument("Invalid input: " + std::string(argv[i]));
		}
		input.push_back(value);
	}
	return input;
}


