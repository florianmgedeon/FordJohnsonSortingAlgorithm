#include "RPN.hpp"

ReversePolishNotation::ReversePolishNotation()
{
}

ReversePolishNotation::~ReversePolishNotation()
{
}

ReversePolishNotation::ReversePolishNotation(const ReversePolishNotation &other)
{
    _stack = other._stack;
}

ReversePolishNotation& ReversePolishNotation::operator=(const ReversePolishNotation &other)
{
    if (this != &other)
    {
        _stack = other._stack;
    }
    return *this;
}

const char *ReversePolishNotation::InvalidExpression::what() const throw()
{
    return "Error";
}

ReversePolishNotation::ReversePolishNotation(char *expression)
{
    int i = 0;
    while (expression[i] != '\0')
    {
        if (!std::isdigit(expression[i]) && expression[i] != '+' && expression[i] != ' ' 
            && expression[i] != '-' && expression[i] != '*' && expression[i] != '/')
        {
            throw InvalidExpression();
        }
        i++;
    }
    if (i < 4)
        throw InvalidExpression();
    std::string str(expression);
    std::istringstream iss(str);
    std::string token;
    int tokenInt = 0;
    std::list<int> a, b;
    while (std::getline(iss, token, ' '))
    {
        if (std::isdigit(token[0]))
        {
            std::stringstream ss(token);
            ss >> tokenInt;
            if (tokenInt < 0 || tokenInt > 9)
                throw InvalidExpression();
            std::list<int> tokenList(1, tokenInt);
            _stack.push(tokenList);
        }
        else
        {
            if (_stack.size() < 2)
                throw InvalidExpression();
            a = _stack.top();
            _stack.pop();
            b = _stack.top();
            _stack.pop();
            std::list<int> result;
            if (token[0] == '+')
            {
                long long int sum = static_cast<long long int>(b.front()) + static_cast<long long int>(a.front());
                if (sum > 2147483647 || sum < -2147483648)
                    throw InvalidExpression();
                result.push_back(b.front() + a.front());
            }
            else if (token[0] == '-')
            {
                long long int diff = static_cast<long long int>(b.front()) - static_cast<long long int>(a.front());
                if (diff > 2147483647 || diff < -2147483648)
                    throw InvalidExpression();
                result.push_back(b.front() - a.front());
            }
            else if (token[0] == '*')
            {
                long long int prod = static_cast<long long int>(b.front()) * static_cast<long long int>(a.front());
                if (prod > 2147483647 || prod < -2147483648)
                    throw InvalidExpression();
                result.push_back(b.front() * a.front());
            }
            else if (token[0] == '/')
            {
                if (a.front() == 0)
                    throw InvalidExpression();
                long long int div = static_cast<long long int>(b.front()) / static_cast<long long int>(a.front());
                if (div > 2147483647 || div < -2147483648)
                    throw InvalidExpression();
                result.push_back(b.front() / a.front());
            }
            _stack.push(result);
        }
    }
    if (_stack.size() != 1)
        throw InvalidExpression();
    int finalResult = _stack.top().front();
    std::cout << finalResult << std::endl;
}