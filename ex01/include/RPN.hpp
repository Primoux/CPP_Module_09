#ifndef RPN_HPP
#define RPN_HPP

#include <ostream>
#include <stack>
#include <list>

class RPN
{
  private:
	std::stack<int, std::list<int> > _stackList;
	void push(int value);
	RPN(RPN const &original);

  public:
	// CONSTRUCTOR
	RPN();
	~RPN();

	// OPERATOR
	RPN &operator=(RPN const &other);

	// MEMBER FUNCTION
	void handleInput(const std::string &input);
	void result();
};

#endif // RPN_HPP
