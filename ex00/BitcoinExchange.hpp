#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

class BitcoinExchange
{
    private:
        std::map<std::string, float> _prices;

    public:
        BitcoinExchange();
        ~BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);
        BitcoinExchange(std::string filename);
        class CannotOpenFile : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
        void readDatabase();
        class ErrorReadingDatabase : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
        std::string findClosestDate(std::string date);
};

#endif