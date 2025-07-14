#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
    readDatabase();
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
    _prices = other._prices;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this == &other)
        return *this;
    _prices = other._prices;
    return *this;
}

const char *BitcoinExchange::CannotOpenFile::what() const throw()
{
    return "Error: cannot open file";
}

bool isValidDate(const std::string& date)
{
    int year, month, day;
    char dash1, dash2;
    std::istringstream dateStream(date);
    if (!(dateStream >> year >> dash1 >> month >> dash2 >> day))
    {
        return false;
    }
    if (year < 1 || month < 1 || month > 12 || day < 1)
    {
        return false;
    }
    static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int maxDays = daysInMonth[month - 1];
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
    {
        maxDays = 29;
    }
    if (day <= maxDays)
    {
        return true;
    }
    return false;
}

//Check if a valid inputfile argument is provided
//Handle errors if the file cannot be opened
//Open and read the input file provided as an argument
//Validate the format of each line (date and value)
//Handle errors for invalid dates and values
BitcoinExchange::BitcoinExchange(std::string filename)
{
    std::ifstream inputfile(filename.c_str());
    if (!inputfile.is_open())
    {
        throw CannotOpenFile();
    }
    readDatabase();
    //ANALYSE FIRST LINE
    std::string line;
    const std::string expectedHeader = "date | value";
    if (!getline(inputfile, line))
    {
        std::cerr << "Error: getline failed" << std::endl;
        return;
    }
    if (line.empty())
    {
        std::cerr << "Error: first line is empty" << std::endl;
    }
    if (line != expectedHeader)
    {
        std::cerr << "Error: first line is not date | value" << std::endl;
    }
    //ANALYSE THE REST OF THE FILE
    while(getline(inputfile, line))
    {
        //CHECK DATE
        if (line.empty())
        {
            std::cerr << "Error: line is empty" << std::endl;
            continue;
        }
        if (line.size() < 10)
        {
            std::cerr << "Error: invalid date format" << std::endl;
            continue;
        }
        if (!std::isdigit(line[0]) || !std::isdigit(line[1]) || !std::isdigit(line[2]) || !std::isdigit(line[3]) 
            || line[4] != '-' || !std::isdigit(line[5]) || !std::isdigit(line[6]) || line[7] != '-' || !std::isdigit(line[8]) || !std::isdigit(line[9]))
        {
            std::cerr << "Error: invalid date format" << std::endl;
            continue;
        }
        if (!isValidDate(line.substr(0, 10)))
        {
            std::cerr << "Error: bad input => " << line.substr(0, 10) << std::endl;
            continue;
        }
        //CHECK SEPARATOR
        if (line.size() > 11 && line[11] != '|')
        {
            std::cerr << "Error: no seperator found" << std::endl;
            continue;
        }
        if (line.size() > 10 && line[10] != ' ')
        {
            std::cerr << "Error: no space before separator" << std::endl;
            continue;
        }
        if (line.size() >= 13 && line[12] != ' ')
        {
            std::cerr << "Error: no space after separator" << std::endl;
            continue;
        }
        //CHECK VALUE
        if (line.size() <= 13)
        {
            std::cerr << "Error: value is empty" << std::endl;
            continue;
        }
        if (line.size() == 14 && !std::isdigit(line[13]))
        {
            std::cerr << "Error: invalid value" << std::endl;
            continue;
        }
        if (line.size() >= 15 && !std::isdigit(line[13]) && line[13] != '+')
        {
            if (line[13] == '-' && std::isdigit(line[14]))
            {
                std::cerr << "Error: not a positive number" << std::endl;
                continue;
            }
            std::cerr << "Error: invalid value" << std::endl;
            continue;
        }
        if (line.size() >= 15 && line[13] == '+' && !std::isdigit(line[14]))
        {
            std::cerr << "Error: invalid value" << std::endl;
            continue;
        }
        size_t i = 0;
        bool continueLoop = false;
        if (line[13] == '+')
        {
            i++;
        }
        while ((13 + i) < line.size() && line[13 + i] != '\n')
        {
            if (!std::isdigit(line[13 + i]) && line[13 + i] != '.')
            {
                std::cerr << "Error: invalid value" << std::endl;
                continueLoop = true;
                break;
            }
            i++;
        }
        if (continueLoop == true)
        {
            continue;
        }
        if (line[13] == '.' || line[13 + i - 1] == '.')
        {
            std::cerr << "Error: invalid value" << std::endl;
            continue;
        }
        if (line[13] == '+' && line[14] == '.')
        {
            std::cerr << "Error: invalid value" << std::endl;
            continue;
        }
        //EXTRACT VALUE AND DATE
        size_t startValue = 13;
        if (line[13] == '+')
        {
            startValue++;
        }
        std::string valueString = line.substr(startValue);
        std::istringstream valueStream(valueString);
        float value;
        valueStream >> value;
        if (valueStream.fail() || !valueStream.eof())
        {
            std::cerr << "Error: invalid float format" << std::endl;
            continue;
        }
        if (value > 1000)
        {
            std::cerr << "Error: too large a number" << std::endl;
            continue;
        }
        std::string date = line.substr(0, 10);
        //SAVING, CALCULATING AND PRINTING
        std::string closestDate = findClosestDate(date);
        if (closestDate.empty())
        {
            std::cerr << "Error: no closest lower date found" << std::endl;
            continue;
        }
        std::cout << date << " => " << value << " = " << (value * _prices[closestDate]) << std::endl;
    }
}

//Open and read the CSV file containing Bitcoin prices
//Store the data in the _prices map
//Error handling when reading into _prices map
void BitcoinExchange::readDatabase()
{
    std::ifstream inputfile("data.csv");
    if (!inputfile.is_open())
    {
        throw CannotOpenFile();
    }
    else
    {
        std::string line;
        const std::string expectedHeader = "date,exchange_rate";
        if (!getline(inputfile, line) || line.empty() || line != expectedHeader)
        {
            throw ErrorReadingDatabase();
        }
        while(getline(inputfile, line))
        {
            std::istringstream linestream(line);
            std::string date;
            float price;
            char extraChar;
            if (getline(linestream, date, ',') && !date.empty() && linestream >> price)
            {
                if (linestream >> extraChar)
                {
                    throw ErrorReadingDatabase();
                }
                _prices[date] = price;
            }
            else
            {
                throw ErrorReadingDatabase();
            }
        }
    }
}

const char *BitcoinExchange::ErrorReadingDatabase::what() const throw()
{
    return "Error: wrong format found when reading file";
}

std::string BitcoinExchange::findClosestDate(std::string date)
{
    std::map<std::string, float>::iterator it = _prices.lower_bound(date);
    if (it == _prices.end() || it->first != date)
    {
        if (it == _prices.begin())
        {
            return "";
        }
        --it;
    }
    return it->first;
}