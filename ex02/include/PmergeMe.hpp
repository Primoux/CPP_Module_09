#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <cstddef>

class PmergeMe {
	private:
		static std::vector<int> jacobsthalOrder(std::size_t count);

		static std::size_t binarySearchPosVector(const std::vector<int>& seq, const std::vector<int>& values, int target);
		static std::size_t binarySearchPosDeque(const std::deque<int>& seq, const std::vector<int>& values, int target);

		static std::vector<int> fordJohnsonVector(std::vector<int> idx, const std::vector<int>& values);
		static std::deque<int>  fordJohnsonDeque(std::deque<int> idx, const std::vector<int>& values);
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();

		static std::vector<int> parseInput(int argc, char** argv);

		static std::vector<int> sortVector(const std::vector<int>& input);
		static std::deque<int>  sortDeque(const std::vector<int>& input);
};

#endif
