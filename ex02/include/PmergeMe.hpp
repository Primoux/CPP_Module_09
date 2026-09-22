#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <cstddef>
#include <string>

class PmergeMe {
	private:
		std::vector<std::vector<int> > _vector;
		std::deque<std::vector<int> > _deque;
	public:
		PmergeMe();
		~PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		void parseInput(int argc, char ** argv);
		void start(void);
		void sortVec(void);
		void sortDeque(void);
		void mergeVec(int &recursion_depth);
		void makePairsVec(int recursion_depth);
};

#endif
