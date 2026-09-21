#include "PmergeMe.hpp"
#include <sstream>
#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <string>
#include "color.h"


std::vector<std::vector<int> > aloneVec;

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

template <typename T>
static void debug(std::string const &title, T const &vec)
{
	std::cout << title << std::endl;
	std::cout << "vec size = " << vec.size() << std::endl;
	std::cout << "vec[0] size = " << vec[0].size() << std::endl;
	for (size_t j = 0; j < vec.size(); ++j)
	{
		std::cout << "(";
		for (size_t k = 0; k < vec[j].size(); k++)
		{
			if (k != 0)
				std::cout << ", ";
			std::cout << vec[j][k];
		}
		std::cout << ") ";
	}
	std::cout << std::endl;
	if (!aloneVec.empty())
	{
		std::cout << "Alone: ";
		for (size_t j = 0; j < aloneVec.size(); ++j)
		{
			std::cout << "(";
			for (size_t k = 0; k < aloneVec[j].size(); k++)
			{
				if (k != 0)
					std::cout << ", ";
				std::cout << aloneVec[j][k];
			}
			std::cout << ") ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

std::string vecToString(std::vector<int> &vec)
{
	std::stringstream ss;
	for (size_t i = 0; i < vec.size(); i++)
	{
		ss << vec[i];
		if (i != vec.size() - 1)
			ss << " ";
	}
	return (ss.str());
}

void debug(std::string const &title, std::vector<int> const &vec)
{
	std::cout << title << std::endl;
	std::cout << "vec size = " << vec.size() << std::endl;
	std::cout << "(";
	for (size_t k = 0; k < vec.size(); k++)
	{
		if (k != 0)
			std::cout << ", ";
		std::cout << vec[k];
	}
	std::cout << ")" << std::endl;
}

int searchSize(std::vector<std::vector<int> > &vec, size_t size)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (size  == vec[i].size())
		{
			return (size);
		}
	}
	return (0);
}

template <typename T>
static void debug_insert(std::string const &title, T const &vec, T const &vecLooser, T const &vecWinner)
{
	size_t pairNumber = 1;
	std::cout << title << vec[0].size() << std::endl;
	bool winner = false;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		std::cout << (winner ? "winner " : "looser ") << pairNumber << " - [";
		for (size_t j = 0; j < vec[i].size(); ++j)
		{
			std::cout << vec[i][j] << (j < vec[i].size() - 1 ? " " : "");
		}
		std::cout << "] => " << ((!(i % 2) && pairNumber != 1) ? RED "PEND" : GREEN "MAIN")<< "\n" RESET;
		winner ^= true;
		if (i % 2)
			++pairNumber;
	}
	if (searchSize(aloneVec, vec[0].size()))
	{
		std::cout << "looser " << pairNumber << " - [" <<vecToString(aloneVec.back());
		std::cout << "] => " << RED "PEND ALONE" RESET << std::endl;

		aloneVec.pop_back();
	}
	std::cout << "\n\n";
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
		std::vector<int> vec;
		vec.push_back(value);
		input.push_back(vec);
	}
	this->_vector = input;
	std::deque<std::vector<int> > dequeInput(input.begin(), input.end());
	this->_deque = dequeInput;
}

void PmergeMe::mergeVec(int &recursion_depth)
{
	for (; this->_vector.size() != 1; recursion_depth++)
	{
		std::vector<std::vector<int> > next;

		for (size_t i = 0; i + 1 < this->_vector.size(); i += 2)
		{
			std::vector<int> a = this->_vector[i];
			std::vector<int> b = this->_vector[i + 1];

			if (a.back() > b.back())
				a.swap(b);

	 	   a.insert(a.end(), b.begin(), b.end());
	 	   next.push_back(a);
		}

		if (this->_vector.size() % 2)
			aloneVec.push_back(this->_vector.back());
		this->_vector = next;
		debug("INSIDE PAIRING", this->_vector);
	}
}

void PmergeMe::makePairsVec(int recursion_depth)
{
	for (; recursion_depth > 0; recursion_depth--)
	{
		std::vector<std::vector<int> > pairs;
		for (size_t elem = 0; elem < this->_vector.size(); elem++)
		{
			size_t half = this->_vector[elem].size() / 2;
			std::vector<int> pair1(this->_vector[elem].begin(), this->_vector[elem].begin() + half);
			std::vector<int> pair2(this->_vector[elem].begin() + half, this->_vector[elem].end());
			pairs.push_back(pair1);
			pairs.push_back(pair2);

		}
		this->_vector = pairs;

		debug(BRED "Inside make pairs" RESET, this->_vector);
		//
		debug_insert("Groups of size ", this->_vector);
	}
}



void PmergeMe::sortVec(void)
{
	debug("BEFORE PAIRING", this->_vector);
	if (this->_vector.size() < 2)
		return;
	int recursion_depth = 0;

	mergeVec(recursion_depth);
	debug("--------afterMerge--------", this->_vector);
	makePairsVec(recursion_depth);
	debug("--------AFTER MAKING PAIRS--------", this->_vector);
}

void PmergeMe::start(void)
{
	this->sortVec();
	std::cout << std::endl;
	// this->sortDeque();
}
