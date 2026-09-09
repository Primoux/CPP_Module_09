	#include "BitcoinExchange.hpp"
	#include <fstream>
	#include <sstream>
	#include <iostream>
	#include <algorithm>
	#include <cctype>

	const int ERROR_INVALID_DATE =  -1;
	const std::string FILENAME = "data.csv";

	BitcoinExchange::BitcoinExchange()
	{
	}

	BitcoinExchange::~BitcoinExchange()
	{
	}

	BitcoinExchange::BitcoinExchange(BitcoinExchange const &original) :
		_db(original._db)
	{
	}

	BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &other)
	{
		if (this != &other)
		{
			this->_db = other._db;
		}
		return *this;
	}

	static int parseAndValidateDate(const std::string &date_str)
	{
		if (date_str.length() != 10)
			return ERROR_INVALID_DATE;

		if (date_str[4] != '-' || date_str[7] != '-')
			return ERROR_INVALID_DATE;

		for (std::string::size_type i = 0; i < date_str.length(); ++i)
		{
			if (i == 4 || i == 7)
				continue;
			if (!std::isdigit(static_cast<unsigned char>(date_str[i])))
				return ERROR_INVALID_DATE;
		}

		int year = std::atoi(date_str.substr(0, 4).c_str());
		int month = std::atoi(date_str.substr(5, 2).c_str());
		int day = std::atoi(date_str.substr(8, 2).c_str());

		if (year < 0 || month < 1 || month > 12 || day < 1)
			return ERROR_INVALID_DATE;

		int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		if (month == 2)
		{
			bool is_leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
			if (is_leap)
				days_in_month[1] = 29;
		}

		if (day > days_in_month[month - 1])
			return ERROR_INVALID_DATE;

		return (year * 10000) + (month * 100) + day;
	}

	void BitcoinExchange::loadDatabase(void)
	{
		std::ifstream file(FILENAME.c_str());
		if (!file.is_open())
		{
			throw std::runtime_error("Error: could not open file " + FILENAME);
		}

		int i = 0;
		std::string line;
		bool firstLine = true;

		while (std::getline(file, line))
		{
			const std::string delimiter = ",";
			std::string::size_type delimiter_pos = line.find(delimiter);

			if (firstLine && delimiter_pos == std::string::npos)
			{
				throw std::runtime_error("Error: invalid CSV format (missing header) in " + FILENAME);
			}
			else if (delimiter_pos == std::string::npos)
			{
				throw std::runtime_error("Error: invalid CSV format (missing delimiter) in " + FILENAME);
			}

			std::string date_str = line.substr(0, delimiter_pos);
			std::string rate_str = line.substr(delimiter_pos + delimiter.length());

			if (firstLine)
			{
				firstLine = false;
				if (rate_str == "exchange_rate" && date_str == "date")
					continue;
				else
				{
					throw std::runtime_error("Error: invalid CSV header in " + FILENAME + ". Expected 'date,exchange_rate', got '" + line + "'");
					return;
				}
				continue;
			}

			int date_int = parseAndValidateDate(date_str);
			if (date_int == ERROR_INVALID_DATE)
			{
				throw std::runtime_error("Error: invalid date format in database: " + date_str);
			}

			std::istringstream rate_iss(rate_str);
			double rate = 0.0;
			if (!(rate_iss >> rate) || !rate_iss.eof())
			{
				throw std::runtime_error("Error: invalid rate format in " + FILENAME + " for date " + date_str);
			}

			this->_db[date_int] = rate;
			i++;
		}

		// std::cout << "Loaded " << i << " lines from database" << std::endl;
		file.close();
	}

	int BitcoinExchange::startProcessingInput(std::ifstream &file) const
	{
		std::string line;
		bool firstLine = true;
		int i = 0;

		while (std::getline(file, line))
		{
			i++;


			const std::string delimiter = " | ";
			std::string::size_type delimiter_pos = line.find(delimiter);

			if (firstLine && delimiter_pos == std::string::npos)
			{
				std::cout << "Error: bad input header (expected 'date | value') => " << line << std::endl;
				return 1;
			}
			else if (delimiter_pos == std::string::npos)
			{
				std::cout << "Error: bad input missing separator on line " << i <<  " => " << line << std::endl;
				continue;
			}
			std::string rate_str = line.substr(delimiter_pos + delimiter.length());
			std::string date_str = line.substr(0, delimiter_pos);
			if (firstLine)
			{
				firstLine = false;
				if (rate_str == "value" && date_str == "date")
					continue;
				else
				{
					std::cout << "Error: bad input header (expected 'date | value') => " << line << std::endl;
					return 1;
				}
				continue;
			}
			int date_int = parseAndValidateDate(date_str);
			if (date_int == ERROR_INVALID_DATE)
			{
				std::cout << "Error: bad date format => " << date_str << std::endl;
				continue;
			}

			std::istringstream rate_iss(rate_str);
			double rate = 0.0;
			if (!(rate_iss >> rate) || !rate_iss.eof())
			{
				std::cout << "Error: bad value format => " << rate_str << std::endl;
				continue;
			}
			else if (rate < 0 || (rate == 0 && (1.0 / rate) < 0.0))
			{
				std::cout << "Error: not a positive number => " << rate_str << std::endl;
				continue;
			}
			else if (rate > 1000)
			{
				std::cout << "Error: number superior than 1000 => " << rate_str << std::endl;
				continue;
			}

			double result = getExchangeRate(date_int);
			if (result == -1)
			{
				std::cout << "Error: no database" << std::endl;
				return 1;
			}
			if (result == -2)
			{
				std::cout << "Error: no exchange rate available for date => " << date_str << std::endl;
				continue;
			}
			std::cout << date_str << " => " << rate << " = " << result * rate << std::endl;
		}
		return 0;
	}

	std::ostream &operator<<(std::ostream &o, const BitcoinExchange &obj)
	{
		const std::map<int, double> &db = obj.getDatabase();
		for (std::map<int, double>::const_iterator it = db.begin(); it != db.end(); ++it)
		{
			o << "Date: " << it->first << ", Rate: " << it->second << std::endl;
		}
		return o;
	}

	double BitcoinExchange::getExchangeRate(int date) const
	{
		std::map<int, double>::const_iterator it = this->_db.lower_bound(date);
		if (it == this->_db.end())
		{
			if (this->_db.empty())
				return -1;
			return (--it)->second;
		}
		else if (it->first == date)
		{
			return it->second;
		}
		else if (it == this->_db.begin())
		{
			return -2;
		}
		else
		{
			return (--it)->second;
		}
	}

	//GETTER

	const std::map<int, double> &BitcoinExchange::getDatabase() const
	{
		return this->_db;
	}
