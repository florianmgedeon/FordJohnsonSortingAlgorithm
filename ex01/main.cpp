#include "RPN.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Usage: ./rpn \"[expression]\"" << std::endl;
        return 1;
    }
    try
    {
        ReversePolishNotation rpn(av[1]);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}