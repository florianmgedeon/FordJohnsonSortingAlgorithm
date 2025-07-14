#include "PmergeMe.hpp"

void checkOrder(std::vector<std::pair<int, int> > &v)
{
    for (size_t i = 0; i < v.size() - 1; i++)
    {
        if (v[i].first > v[i + 1].first)
            throw std::runtime_error("Error: vector is not sorted");
    }
}

int main (int ac, char **av)
{
    if (ac < 3)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    
    try
    {
        int o = 0;
        PmergeMe pm(av);
        std::cout << "Before: ";
        std::vector<std::pair<int, int> > vUnsorted = pm.getV();
        for (std::vector<std::pair<int, int> >::iterator it = vUnsorted.begin(); it != vUnsorted.end(); it++)
        {
            if (o == 5)
            {
                std::cout << it->first << " [...]";
                break;
            }
            std::cout << it->first << " ";
            o++;
        }
        std::cout << std::endl;
        struct timeval start1, end1;
        gettimeofday(&start1, NULL);
        std::vector<std::pair<int, int> > vSorted = mergeInsertion(pm.getV());
        gettimeofday(&end1, NULL);
        std::cout << "After:  ";
        o = 0;
        for (std::vector<std::pair<int, int> >::iterator it = vSorted.begin(); it != vSorted.end(); it++)
        {
            if (o == 5)
            {
                std::cout << it->first << " [...]";
                break;
            }
            std::cout << it->first << " ";
            o++;
        }
        std::cout << std::endl;
        double duration1 = (end1.tv_sec - start1.tv_sec) * 1000000.0 + (end1.tv_usec - start1.tv_usec);
        std::cout << "Time to process a range of " << ac - 1 << " elements with std::vector : " << duration1 << " us" << std::endl;
        struct timeval start2, end2;
        gettimeofday(&start2, NULL);
        std::deque<std::pair<int, int> > dSorted = mergeInsertion(pm.getD());
        gettimeofday(&end2, NULL);
        double duration2 = (end2.tv_sec - start2.tv_sec) * 1000000.0 + (end2.tv_usec - start2.tv_usec);
        std::cout << "Time to process a range of " << ac - 1 << " elements with std::deque  : " << duration2 << " us" << std::endl;
        checkOrder(vSorted);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
