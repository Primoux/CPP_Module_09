#include <iostream>
#include <fstream>
#include <sstream>
#include "BitcoinExchange.hpp"




int main(int argc, char **argv)
{
	std::cout.precision(8);
	if (argc != 2)
	{
		std::cerr << "Usage: ./btc <input_file>" << std::endl;
		return 2;
	}
	std::ifstream file(argv[1]);
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file " << argv[1] << std::endl;
		return 1;
	}
	BitcoinExchanger exchanger;
	try
	{
		exchanger.loadDatabase();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	if (exchanger.startProcessingInput(file) != 0)
	{
		return 1;
	}
	return 0;
}
