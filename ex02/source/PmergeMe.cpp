#include "PmergeMe.hpp"
#include "debug.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <sys/time.h>

PmergeMe::PmergeMe() : _elapsedVec(0), _elapsedDeque(0)
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
		this->_input = other._input;
		this->_vector = other._vector;
		this->_deque = other._deque;
		this->_elapsedVec = other._elapsedVec;
		this->_elapsedDeque = other._elapsedDeque;
	}
	return *this;
}

PmergeMe::~PmergeMe()
{
}

size_t jac(size_t n)
{
	size_t current = 0;
	size_t prev = 1;
	size_t prevPrev = 0;
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	for (size_t i = 0; i < n - 1; i++)
	{
		current = prev + 2 * prevPrev;
		prevPrev = prev;
		prev = current;
	}
	return current;
}

void PmergeMe::parseInput(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::istringstream iss(argv[i]);
		int value;
		if (!(iss >> value) || value < 0 || !iss.eof())
			throw std::invalid_argument("Invalid input: " + std::string(argv[i]));
		this->_vector.push_back(std::vector<int>(1, value));
		this->_deque.push_back(std::deque<int>(1, value));
	}
	this->_input = this->_vector;
}

void PmergeMe::printResult(void) const
{
	printFlat("Before:", this->_input);
	printFlat("After:", this->_vector);
	// printFlat("After(Deque):", this->_deque);
	std::cout << "Time to process a range of " << this->_input.size() << " elements with std::vector : " << this->_elapsedVec << " us" << std::endl;
	std::cout << "Time to process a range of " << this->_input.size() << " elements with std::deque : " << this->_elapsedDeque << " us" << std::endl;
}

static double now(void)
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

void PmergeMe::start(void)
{
	double start = now();
	sortVector();
	this->_elapsedVec = now() - start;
	#ifdef DEBUG
	std::cout << BWHITE "|----------------------------------|" RESET << std::endl;
	std::cout << BWHITE "|----------END OF VECTOR-----------|" RESET << std::endl;
	std::cout << BWHITE "|----------------------------------|" RESET << std::endl;
	std::cout << std::endl;
	#endif
	start = now();
	sortDeque();
	this->_elapsedDeque = now() - start;

	#ifdef DEBUG
	std::cout << BWHITE "|----------------------------------|" RESET << std::endl;
	std::cout << BWHITE "|----------END OF DEQUE------------|" RESET << std::endl;
	std::cout << BWHITE "|----------------------------------|" RESET << std::endl;
	std::cout << std::endl;
	#endif
	printResult();
}

/* ======================================================================== */
/*                               STD::VECTOR                                */
/* ======================================================================== */

static int mergePairs(VecGroups &c, VecGroups &alone)
{
	int depth = 0;
	for (; c.size() > 1; ++depth)
	{
		VecGroups next;
		for (size_t i = 0; i + 1 < c.size(); i += 2)
		{
			std::vector<int> a = c[i];
			std::vector<int> b = c[i + 1];

			if (a.back() > b.back())
				a.swap(b);
			a.insert(a.end(), b.begin(), b.end());
			next.push_back(a);
		}
		if (c.size() % 2)
			alone.push_back(c.back());
		c = next;
		debug(GREEN "INSIDE PAIRING" RESET, c);
		debug(YELLOW "Alone" RESET, alone);
	}
	return depth;
}

static void splitPairs(VecGroups &c)
{
	VecGroups pairs;
	for (size_t i = 0; i < c.size(); ++i)
	{
		size_t half = c[i].size() / 2;
		pairs.push_back(std::vector<int>(c[i].begin(), c[i].begin() + half));
		pairs.push_back(std::vector<int>(c[i].begin() + half, c[i].end()));
	}
	c = pairs;
}

static void getWinnerLooser(VecGroups &c, VecGroups &alone, VecGroups &main, VecGroups &pend)
{
	size_t pairNumber = 1;
	bool winner = false;
	for (size_t i = 0; i < c.size(); ++i)
	{
		#ifdef DEBUG
		std::stringstream label;
		label << (winner ? "winner " : "looser ") << pairNumber << " - ";
		debugElem(label.str(), c[i], (!(i % 2) && pairNumber != 1) ? RED "=> PEND" : GREEN "=> MAIN");
		#endif
		if (i == 0 || winner)
			main.push_back(c[i]);
		else
			pend.push_back(c[i]);
		winner ^= true;
		if (i % 2)
			++pairNumber;
	}
	if (!alone.empty() && alone.back().size() == c.back().size())
	{
		debugElem("looser alone - ", alone.back(), YELLOW "=> PEND ALONE");
		pend.push_back(alone.back());
		alone.pop_back();
	}
	c.clear();
}


static void insertPend(VecGroups &main, VecGroups &pend)
{
	if (pend.empty())
		return;
	debug(BGREEN "Pend before insertion" RESET, pend);
	debug(BVIOLET "Main before insertion" RESET, main);

	size_t n = pend.size();
	bool hasAlone = (n == main.size() - 1);
	size_t inserted = 0;
	size_t k = 2;
	while (inserted < n)
	{
		size_t groupStart = jac(k - 1);
		size_t groupEnd = std::min(jac(k), n + 1);
		size_t bound = jac(k) + jac(k - 1) - 1;
		for (size_t maxValue = groupEnd; maxValue > groupStart; maxValue--)
		{
			// std::cout << GREEN "bound = " << bound << ", main.size() = " << main.size() << RESET << std::endl;
			// std::cout << RED "maxValue = " << maxValue << ", groupStart = " << groupStart << ", groupEnd = " << groupEnd << RESET << std::endl;

			size_t index = maxValue - 2;
			size_t lo = 0;
			size_t hi = std::min(bound, main.size());
			if (hasAlone && index == n - 1)
				hi = main.size();
			while (lo < hi)
			{
				size_t mid = (lo + hi) / 2;

				if (pend[index].back() > main[mid].back())
					lo = mid + 1;
				else
					hi = mid;
			}
			main.insert(main.begin() + lo, pend[index]);
			inserted++;
		}
		k++;
	}
	pend.clear();
}

void PmergeMe::sortVector(void)
{
	VecGroups &c = this->_vector;
	if (c.size() < 2)
		return;
	VecGroups alone;

	debug(BGREEN "BEFORE PAIRING" RESET, c);
	int depth = mergePairs(c, alone);
	debug(BGREEN "--------afterMerge--------" RESET, c);
	for (; depth > 0; --depth)
	{
		splitPairs(c);
		debug(YELLOW "After spliting pairs" RESET, c);

		VecGroups main;
		VecGroups pend;
		getWinnerLooser(c, alone, main, pend);
		debug(BGREEN "Winner" RESET, main);
		debug(BRED "Looser" RESET, pend);

		insertPend(main, pend);
		c = main;
		debug(YELLOW "After inserting pend" RESET, c);
	}
}


/* ======================================================================== */
/*                                STD::DEQUE                                */
/* ======================================================================== */

static int mergePairs(DequeGroups &c, DequeGroups &alone)
{
	int depth = 0;
	for (; c.size() > 1; ++depth)
	{
		DequeGroups next;
		for (size_t i = 0; i + 1 < c.size(); i += 2)
		{
			std::deque<int> a = c[i];
			std::deque<int> b = c[i + 1];

			if (a.back() > b.back())
				a.swap(b);
			a.insert(a.end(), b.begin(), b.end());
			next.push_back(a);
		}
		if (c.size() % 2)
			alone.push_back(c.back());
		c = next;
		debug(GREEN "INSIDE PAIRING" RESET, c);
		debug(YELLOW "Alone" RESET, alone);
	}
	return depth;
}

static void splitPairs(DequeGroups &c)
{
	DequeGroups pairs;
	for (size_t i = 0; i < c.size(); ++i)
	{
		size_t half = c[i].size() / 2;
		pairs.push_back(std::deque<int>(c[i].begin(), c[i].begin() + half));
		pairs.push_back(std::deque<int>(c[i].begin() + half, c[i].end()));
	}
	c = pairs;
}

static void getWinnerLooser(DequeGroups &c, DequeGroups &alone, DequeGroups &main, DequeGroups &pend)
{
	size_t pairNumber = 1;
	bool winner = false;
	for (size_t i = 0; i < c.size(); ++i)
	{
		#ifdef DEBUG
		std::stringstream label;
		label << (winner ? "winner " : "looser ") << pairNumber << " - ";
		debugElem(label.str(), c[i], (!(i % 2) && pairNumber != 1) ? RED "=> PEND" : GREEN "=> MAIN");
		#endif
		if (i == 0 || winner)
			main.push_back(c[i]);
		else
			pend.push_back(c[i]);
		winner ^= true;
		if (i % 2)
			++pairNumber;
	}
	if (!alone.empty() && alone.back().size() == c.back().size())
	{
		debugElem("looser alone - ", alone.back(), YELLOW "=> PEND ALONE");
		pend.push_back(alone.back());
		alone.pop_back();
	}
	c.clear();
}

static void insertPend(DequeGroups &main, DequeGroups &pend)
{
	if (pend.empty())
		return;
	debug(BGREEN "Pend before insertion" RESET, pend);
	debug(BVIOLET "Main before insertion" RESET, main);

	size_t n = pend.size();
	bool hasAlone = (n == main.size() - 1);
	size_t inserted = 0;
	size_t k = 2;
	while (inserted < n)
	{
		size_t groupStart = jac(k - 1);
		size_t groupEnd = std::min(jac(k), n + 1);
		size_t bound = jac(k) + jac(k - 1) - 1;
		for (size_t maxValue = groupEnd; maxValue > groupStart; maxValue--)
		{
			// std::cout << GREEN "bound = " << bound << ", main.size() = " << main.size() << RESET << std::endl;
			// std::cout << RED "maxValue = " << maxValue << ", groupStart = " << groupStart << ", groupEnd = " << groupEnd << RESET << std::endl;

			size_t index = maxValue - 2;
			size_t lo = 0;
			size_t hi = std::min(bound, main.size());
			if (hasAlone && index == n - 1)
				hi = main.size();
			while (lo < hi)
			{
				size_t mid = (lo + hi) / 2;

				if (pend[index].back() > main[mid].back())
					lo = mid + 1;
				else
					hi = mid;
			}
			main.insert(main.begin() + lo, pend[index]);
			inserted++;
		}
		k++;
	}
	pend.clear();
}

void PmergeMe::sortDeque(void)
{
	DequeGroups &c = this->_deque;
	if (c.size() < 2)
		return;
	DequeGroups alone;

	debug(BGREEN "BEFORE PAIRING" RESET, c);
	int depth = mergePairs(c, alone);
	debug(BGREEN "--------afterMerge--------" RESET, c);
	for (; depth > 0; --depth)
	{
		splitPairs(c);
		debug(YELLOW "After spliting pairs" RESET, c);

		DequeGroups main;
		DequeGroups pend;
		getWinnerLooser(c, alone, main, pend);
		debug(BGREEN "Winner" RESET, main);
		debug(BRED "Looser" RESET, pend);

		insertPend(main, pend);
		c = main;
		debug(YELLOW "After inserting pend" RESET, c);
	}
}
