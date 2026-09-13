#include "PmergeMe.hpp"
#include <sstream>
#include <iostream>
#include <utility>
#include <algorithm>

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
		this->_deque = other._deque;
		this->_vector = other._vector;
	}
	return *this;
}

PmergeMe::~PmergeMe()
{
}

static void debug(std::string const &title, std::vector<std::vector<int> > const &vec)
{
	std::cout << title << std::endl;
	for (size_t j = 0; j < vec.size(); ++j)
	{
		std::cout << "(";
		for (size_t k = 0; k < vec.size(); k++)
		{
			if (k != 0)
				std::cout << ", ";
			std::cout << "[" << vec[j][k] << "]";
		}
		std::cout << ") ";
	}
	std::cout << std::endl;
}


void PmergeMe::parseInput(int argc, char** argv)
{
	std::vector<std::vector<int> > input;
	for (int i = 1; i < argc; ++i)
	{
		std::istringstream iss(argv[i]);
		int value;
		if (!(iss >> value) || value < 0 || !iss.eof())
		{
			throw std::invalid_argument("Invalid input: " + std::string(argv[i]));
		}
		input.push_back(std::vector<int>(1, value));
	}
	this->_vector = input;
	std::deque<std::vector<int> > dequeInput(input.begin(), input.end());
	// this->_deque = dequeInput;
	debug("Parsed input deque", this->_vector);
	// debug("Parsed input vector", this->_vector);
}

void PmergeMe::sortVec(void)
{
	if (this->_vector.size() < 2)
		return;

	std::vector<std::vector<int> > pairs;
	bool hisAlone = false;
	int alone = -1;
	// debug("DEBUG VECTOR", this->_vector);

	for (size_t i = 0; i + 1 < this->_vector.size(); i += 2)
	{
		int a = this->_vector[i][0];
		int b = this->_vector[i + 1][0];
		std::vector<int> pair;

		if (a > b)
		{
			pair.push_back(b);
			pair.push_back(a);
		}
		else
		{
			pair.push_back(a);
			pair.push_back(b);
		}
		pairs.push_back(pair);
	}
	if (this->_vector.size() % 2 != 0)
	{
		hisAlone = true;
		alone = this->_vector[this->_vector.size() - 1][0];
		this->_vector.pop_back();
	}

	debug("AFTER PAIRING", pairs);


	for (size_t i = 0; i + 1 < pairs.size(); i += 2)
	{
		int a = pairs[i].back();
		int b = pairs[i + 1].back();
		if (a > b)
		{
			std::swap_ranges(pairs[i].begin(), pairs[i].end(), pairs[i + 1].begin());
		}
	}

	debug("FINISH ??", pairs);
	if (hisAlone)
		std::cout << "alone: " << alone << std::endl;

}

void PmergeMe::sortDeque(void)
{

}

void PmergeMe::start(void)
{
	this->sortVec();
}
