#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

#include <ostream>
#include <map>

class BitcoinExchange
{
  private:
	std::map <int , double> _db;

  public:
	// CONSTRUCTOR
	BitcoinExchange();
	~BitcoinExchange();
	BitcoinExchange(BitcoinExchange const &original);

	// OPERATOR
	BitcoinExchange &operator=(BitcoinExchange const &other);
	// MEMBER FUNCTION
	void loadDatabase(void);
	double getExchangeRate(int date) const;
	int startProcessingInput(std::ifstream &file) const;
	//GETTER
	const std::map<int, double> &getDatabase() const;
};
std::ostream &operator<<(std::ostream &o, const BitcoinExchange &obj);

#endif // !BITCOIN_EXCHANGE_HPP
