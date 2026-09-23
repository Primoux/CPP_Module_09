#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <string>
#include "color.h"

template <typename Elem>
void printElem(Elem const &elem, std::string const &sep)
{
	for (size_t k = 0; k < elem.size(); ++k)
	{
		if (k != 0)
			std::cout << sep;
		std::cout << elem[k];
	}
}

template <typename C>
void printFlat(std::string const &label, C const &c)
{
	std::cout << label;
	for (size_t i = 0; i < c.size(); ++i)
	{
		std::cout << " ";
		printElem(c[i], " ");
	}
	std::cout << std::endl;
}

template <typename C>
void debug(std::string const &title, C const &c)
{
	#ifdef DEBUG
	std::cout << title << std::endl;
	if (c.empty())
	{
		std::cout << BRED "empty" RESET << std::endl << std::endl;
		return;
	}
	std::cout << "size = " << c.size() << ", group size = " << c[0].size() << std::endl;
	for (size_t i = 0; i < c.size(); ++i)
	{
		std::cout << "(";
		printElem(c[i], ", ");
		std::cout << ") ";
	}
	std::cout << std::endl << std::endl;
	#else
	(void)title;
	(void)c;
	#endif
}

template <typename Elem>
void debugElem(std::string const &before, Elem const &elem, std::string const &after)
{
	#ifdef DEBUG
	std::cout << before << "[";
	printElem(elem, " ");
	std::cout << "] " << after << RESET << std::endl;
	#else
	(void)before;
	(void)elem;
	(void)after;
	#endif
}

#endif
