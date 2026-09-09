#ifndef BITCOIN_EXCHANGER_HPP
#define BITCOIN_EXCHANGER_HPP

#include <ostream>
#include <map>

class BitcoinExchanger
{
  private:
	std::map <int , double> _db;

  public:
	// CONSTRUCTOR
	BitcoinExchanger();
	~BitcoinExchanger();
	BitcoinExchanger(BitcoinExchanger const &original);

	// OPERATOR
	BitcoinExchanger &operator=(BitcoinExchanger const &other);
	// MEMBER FUNCTION
	void loadDatabase(void);
	double getExchangeRate(int date) const;
	int startProcessingInput(std::ifstream &file) const;
	//GETTER
	const std::map<int, double> &getDatabase() const;
};
std::ostream &operator<<(std::ostream &o, const BitcoinExchanger &obj);

#endif // !BITCOIN_EXCHANGER_HPP
