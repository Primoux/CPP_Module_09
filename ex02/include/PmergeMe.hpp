#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <vector>
#include <cstddef>
#include <string>

typedef std::vector<std::vector<int> >	VecGroups;
typedef std::deque<std::deque<int> >	DequeGroups;

class PmergeMe {
	private:
		VecGroups	_input;
		VecGroups	_vector;
		DequeGroups	_deque;
		double		_elapsedVec;
		double		_elapsedDeque;

		void sortVector(void);
		void sortDeque(void);
		void printResult(void) const;
	public:
		PmergeMe();
		~PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		void parseInput(int argc, char ** argv);
		void start(void);
};

size_t jac(size_t n);

#endif
