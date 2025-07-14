#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <list>

class ReversePolishNotation
{
    private:
        std::stack<std::list<int> > _stack;
    public:
        ReversePolishNotation();
        ~ReversePolishNotation();
        ReversePolishNotation(const ReversePolishNotation &other);
        ReversePolishNotation &operator=(const ReversePolishNotation &other);
        ReversePolishNotation(char *expression);
        class InvalidExpression : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
};

#endif