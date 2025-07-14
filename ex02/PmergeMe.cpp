#include "PmergeMe.hpp"

long gCompareCount = 0;

PmergeMe::PmergeMe(char **av)
{
    int pairIndex = 1;
    for (int i = 1; av[i]; i++)
    {
        std::istringstream iss(av[i]);
        long num;
        if (!(iss >> num) || num < 0 || num > INT_MAX || !(iss.eof()))
            throw Error();
        _v.push_back(std::make_pair(num, pairIndex));
        _d.push_back(std::make_pair(num, pairIndex));
        if (i % 2 == 0)
            pairIndex++;
    }
    for (size_t i = 0; i < _v.size(); i++)
    {
        for (size_t j = i + 1; j < _v.size(); j++)
        {
            if (_v[i].first == _v[j].first)
                throw Error();
        }
    }
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
    if (this == &other)
        return;
    *this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    _v = other._v;
    _d = other._d;
    return *this;
}

const char *PmergeMe::Error::what() const throw()
{
    return "Error";
}

std::vector<std::pair<int, int> > PmergeMe::getV() const
{
    return _v;
}

std::deque<std::pair<int, int> > PmergeMe::getD() const
{
    return _d;
}