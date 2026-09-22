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

size_t jac(size_t depth)
{
	size_t current = 0;
	size_t prev = 1;
	size_t prevPrev = 0;
	if (depth == 0)
		return 0;
	if (depth == 1)
		return 1;
	for (size_t i = 0; i < depth - 1; i++)
	{
		// std::cout << "Current = " << current << " prev = " << prev << " prevPrev = " << prevPrev << std::endl;
		current = (prev) + 2 * (prevPrev);
		prevPrev = prev;
		prev = current;
	}
	return(current);
}

template <typename T>
static void debug(std::string const &title, T const &vec)
{
	#ifndef DEBUG
    return;
	#endif
	if (vec.empty())
	{
		std::cout << title << std::endl;
		std::cout << BRED "vec is empty" RESET << std::endl;
		return;
	}
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
	#ifndef DEBUG
    return;
	#endif
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
		if (size == vec[i].size())
			return (i);
	}
	return (-1);
}

template <typename T>
static void getWinnerLooser(T &vec, T &vecLooser, T &vecWinner)
{
	size_t pairNumber = 1;
	bool winner = false;
	size_t i = 0;
	for (; i < vec.size(); ++i)
	{
		#ifdef DEBUG
			std::cout << (winner ? "winner " : "looser ") << pairNumber << " - [";
			for (size_t j = 0; j < vec[i].size(); ++j)
			{
				std::cout << vec[i][j] << (j < vec[i].size() - 1 ? " " : "");
			}
			std::cout << "] => " << ((!(i % 2) && pairNumber != 1) ? RED "PEND" : GREEN "MAIN")<< "\n" RESET;
		#endif
		if (i == 0 || winner)
			vecWinner.push_back(vec[i]);
		else
			vecLooser.push_back(vec[i]);
		winner ^= true;

		if (i % 2)
			++pairNumber;
	}
	int index = searchSize(aloneVec, vec.back().size());

	if (index != - 1)
	{
		#ifdef DEBUG
		std::cout << "looser " << pairNumber << " - [" << vecToString(aloneVec.back());
		std::cout << "] => " << YELLOW "PEND ALONE" RESET << std::endl;
		#endif
		vecLooser.push_back(aloneVec.back());
		aloneVec.pop_back();
	}
	vec.clear();

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

void createOrderJac(std::vector<std::vector<int> > &pend)
{
	std::vector<std::vector<int> > ordered;

	size_t n = pend.size();
	size_t k = 2;
	while (ordered.size() < n)
	{
		size_t lowerBound = jac(k -1);
		size_t higherBound = std::min(jac(k), n + 1);
		for (size_t maxValue = higherBound; maxValue > lowerBound; maxValue--)
			ordered.push_back(pend[maxValue - 2]);
		k++;
	}
	pend = ordered;
	debug(BMAGENTA "After creating order" RESET, pend);
}


template <typename T>
void insertPend(T &main, T &pend)
{
	if (pend.empty())
		return;
	createOrderJac(pend);
	while (pend.size())
	{
		std::vector<int> elem = pend.front();
		#ifdef DEBUG
		std::cout << "Inserting pend: [" << vecToString(elem) << "] into main" << std::endl;
		#endif
		size_t lowerBound = 0;
		size_t higherBound = main.size();
		while (lowerBound < higherBound)
		{
			size_t mid = (lowerBound + higherBound) / 2;

			if (elem.back() > main[mid].back())
					lowerBound = mid + 1;
			else
			higherBound = mid;
		}
		main.insert(main.begin() + lowerBound, pend.front());
		pend.erase(pend.begin());
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

		debug(YELLOW "Inside make pairs" RESET, this->_vector);
		std::vector<std::vector<int> > main;
		std::vector<std::vector<int> > pend;
		getWinnerLooser(this->_vector, pend, main);
		debug(BGREEN "Winner" RESET, main);
		debug(BRED "Looser" RESET, pend);

		insertPend(main, pend);
		this->_vector = main;
		debug(YELLOW "After inserting pend" RESET, this->_vector);

	}
}



void PmergeMe::sortVec(void)
{
	debug(BGREEN "BEFORE PAIRING" RESET, this->_vector);
	if (this->_vector.size() < 2)
		return;
	int recursion_depth = 0;

	mergeVec(recursion_depth);
	debug(BGREEN "--------afterMerge--------" RESET, this->_vector);
	makePairsVec(recursion_depth);
	debug(BYELLOW "--------AFTER MAKING PAIRS--------" RESET, this->_vector);
}

void printStart(std::vector<std::vector<int> > &vec)
{
	std::cout << "Before: ";
	for (size_t i = 0; i < vec.size(); ++i)
	{
		std::cout << "(";
		for (size_t j = 0; j < vec[i].size(); ++j)
		{
			std::cout << vec[i][j];
			if (j != vec[i].size() - 1)
				std::cout << " ";
		}
		std::cout << ")";
		if (i != vec.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
}

void printResult(std::vector<std::vector<int> > &vec)
{
	std::cout << "After: ";
	for (size_t i = 0; i < vec.size(); ++i)
	{
		std::cout << "(";
		for (size_t j = 0; j < vec[i].size(); ++j)
		{
			std::cout << vec[i][j];
			if (j != vec[i].size() - 1)
				std::cout << " ";
		}
		std::cout << ")";
		if (i != vec.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::start(void)
{
	printStart(this->_vector);
	sortVec();
	// sortDeque();
	printResult(this->_vector);
}
