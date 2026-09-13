#include "PmergeMe.hpp"
#include <sstream>
#include <iostream>
#include <utility>
#include <algorithm>


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
	for (size_t j = 0; j < vec.size(); ++j)
	{
		std::cout << "(";
		for (size_t k = 0; k < vec[j].size(); k++)
		{
			if (k != 0)
				std::cout << ", ";
			std::cout << "[" << vec[j][k] << "]";
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
				std::cout << "[" << aloneVec[j][k] << "]";
			}
			std::cout << ") ";
		}
		std::cout << std::endl;
	}
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

void PmergeMe::sortVec(void)
{
	debug("BEFORE PAIRING", this->_vector);
	if (this->_vector.size() < 2)
		return;
	for (; this->_vector.size() != 1;)
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
		    next.push_back(this->_vector.back());
		if (this->_vector.size() % 2 != 0)
		{
			aloneVec.push_back(this->_vector.back());
			// suppression of the last element in the next vector
			next.pop_back();
		}

		this->_vector = next;
		debug("AFTER PAIRING", this->_vector);

	}
}

// void PmergeMe::sortDeque(void)
// {
// 	debug("BEFORE PAIRING", this->_deque);
// 	if (this->_deque.size() < 2)
// 		return;
// 	bool hisAlone = false;
// 	if (this->_deque.size() % 2 != 0)
// 	{
// 		hisAlone = true;
// 		alone = this->_deque[this->_deque.size() - 1][0];
// 		this->_deque.pop_back();
// 	}

// 	for (; this->_deque.size() != 1;)
// 	{
// 		std::deque<std::vector<int> > next;

// 		for (size_t i = 0; i + 1 < this->_deque.size(); i += 2)
// 		{
// 		    std::vector<int> a = this->_deque[i];
// 		    std::vector<int> b = this->_deque[i + 1];

// 		    if (a.back() > b.back())
// 		        a.swap(b);

// 	 	   a.insert(a.end(), b.begin(), b.end());
// 	 	   next.push_back(a);
// 		}

// 		if (this->_deque.size() % 2)
// 		    next.push_back(this->_deque.back());

// 		this->_deque = next;
// 		debug("AFTER PAIRING", this->_deque);

// 	}
// }

void PmergeMe::start(void)
{
	this->sortVec();
	std::cout << std::endl;
	// this->sortDeque();
}
