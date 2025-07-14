#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#ifndef COUNT_COMPARE
# define COUNT_COMPARE 0
#endif

#include <iostream>
#include <sys/time.h>
#include <vector>
#include <deque>
#include <sstream>
#include <climits>

extern long gCompareCount;

template <typename T>
T smallArraySort(T c)
{
    if (c.size() == 1)
    {
        if (COUNT_COMPARE)
            std::cout << "Number of comparisons: 0" << std::endl;
        return c;
    }
    if (c.size() == 2)
    {
        gCompareCount = 1;
        if (c[0].first > c[1].first)
        {
            T temp;
            temp.push_back(c[1]);
            temp.push_back(c[0]);
            if (COUNT_COMPARE)
                std::cout << "Number of comparisons: " << gCompareCount << std::endl;
            return temp;
        }
        if (COUNT_COMPARE)
            std::cout << "Number of comparisons: " << gCompareCount << std::endl;
        return c;
    }
    if (c.size() == 3)
    {
        if (COUNT_COMPARE)
            gCompareCount = 3;
        if (c[0].first > c[1].first)
            std::swap(c[0], c[1]);
        if (c[0].first > c[2].first)
            std::swap(c[0], c[2]);
        if (c[1].first > c[2].first)
            std::swap(c[1], c[2]);
        if (COUNT_COMPARE)
            std::cout << "Number of comparisons: " << gCompareCount << std::endl;
        return c;
    }
    return c;
}

template <typename T>
void selectFirstBigger(T& c, T& firstBigger)
{
    for (size_t i = 0; i < c.size(); i += 2)
    {
        if (i + 1 == c.size())
            break;
        if (COUNT_COMPARE)
            gCompareCount++;
        if (c[i].first > c[i + 1].first)
        {
            firstBigger.push_back(c[i]);
            c[i].second = -1;
        }
        else
        {
            firstBigger.push_back(c[i + 1]);
            c[i + 1].second = -1;
        }
    }
    for (size_t i = 0; i < c.size(); i++)
    {
        if (c[i].second == -1)
        {
            c.erase(c.begin() + i);
            i--;
        }
    }
}

template <typename T>
std::vector<T> pairSortRepeat(T c)
{
    //the main vector firstBiggerSorted holds many vectors that hold the numbers with their indexes
    //one subvectors size is the size of the whole pair with 2 elements e.g.: (11 2)
    std::vector<T> firstBiggerSorted;

    //fill firstBiggerSorted with subvectors of size 2
    T temp; //subvector
    for (size_t i = 0; i < c.size(); i++)
    {
        if (c.size() % 2 != 0 && i == c.size() - 1)//insert last odd element
        {
            temp.push_back(c[i]);
            firstBiggerSorted.push_back(temp);
            break;
        }
        temp.push_back(c[i]);
        if (temp.size() == 2)
        {
            firstBiggerSorted.push_back(temp);
            temp.clear();
        }
    }

    size_t pairSize = 2;
    while (firstBiggerSorted.size() > 1)
    {
        //sort the 2 elements within a subvector
        for (size_t j = 0; j < firstBiggerSorted.size(); j++)
        {
            if (firstBiggerSorted[j].size() != pairSize)
                continue;
            gCompareCount++;
            if (firstBiggerSorted[j][(pairSize / 2) - 1] > firstBiggerSorted[j][pairSize - 1])
            {
                size_t k = 0;
                while (((pairSize / 2) - 1) >= k)
                {
                    std::swap(firstBiggerSorted[j][(pairSize / 2) - 1 - k], firstBiggerSorted[j][pairSize - 1 - k]);
                    k++;
                }
            }
        }
        pairSize *= 2;
        //merge the subvectors. add the elements of the next subvector to the current one
        for (size_t j = 0; j < firstBiggerSorted.size(); j += 2)
        {
            if (j + 1 < firstBiggerSorted.size())
            {
                for (size_t k = 0; k < firstBiggerSorted[j + 1].size(); k++)
                {
                    firstBiggerSorted[j].push_back(firstBiggerSorted[j + 1][k]);
                    firstBiggerSorted[j + 1][k].second = -1;
                }
            }
        }
        //erase the elements with -1 index
        for (size_t j = 0; j < firstBiggerSorted.size(); j++)
        {
            for (size_t k = 0; k < firstBiggerSorted[j].size(); k++)
            {
                if (firstBiggerSorted[j][k].second == -1)
                {
                    firstBiggerSorted[j].erase(firstBiggerSorted[j].begin() + k);
                    k--;
                }
            }
        }
        //delete empty subvectors
        for (size_t j = 0; j < firstBiggerSorted.size(); j++)
        {
            if (firstBiggerSorted[j].empty())
            {
                firstBiggerSorted.erase(firstBiggerSorted.begin() + j);
                j--;
            }
        }
        if (firstBiggerSorted.size() == 1)
        {
            //sort the 2 elements within the last subvector
            for (size_t j = 0; j < firstBiggerSorted.size(); j++)
            {
                if (firstBiggerSorted[j].size() != pairSize)
                    continue;
                gCompareCount++;
                if (firstBiggerSorted[j][(pairSize / 2) - 1] > firstBiggerSorted[j][pairSize - 1])
                {
                    size_t k = 0;
                    while (((pairSize / 2) - 1) >= k)
                    {
                        std::swap(firstBiggerSorted[j][(pairSize / 2) - 1 - k], firstBiggerSorted[j][pairSize - 1 - k]);
                        k++;
                    }
                }
            }
        }
    }
    return firstBiggerSorted;
}

template <typename T>
std::vector<long long> generateJacobsthalSequence(long originalCSize, const T& firstBiggerSorted)
{
    std::vector<long long> jacobsthal;
    for (long i = 0; i < originalCSize; i++) {
        if (i == 0)
            jacobsthal.push_back(1);
        else if (i == 1)
            jacobsthal.push_back(1);
        else
            jacobsthal.push_back(jacobsthal[i - 1] + 2 * jacobsthal[i - 2]);
    }
    if (jacobsthal.size() == 2)
    {
        jacobsthal.erase(jacobsthal.begin());
        return jacobsthal;
    }
    jacobsthal.erase(jacobsthal.begin());
    if (firstBiggerSorted.size() == 2)
        jacobsthal.push_back(3);
    for (size_t i = 0; i < jacobsthal.size(); i++)
        jacobsthal[i]--;
    return jacobsthal;
}

template <typename T>
int binarySearch(const T& firstBiggerSorted, const T& c, int l, int k)
{
    int left = 0;
    int right = k - 1;
    if (right < 0)
        right = 0;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (COUNT_COMPARE)
            gCompareCount++;
        if (c[l].first > firstBiggerSorted[mid].first)
            left = mid + 1;
        else if (c[l].first == firstBiggerSorted[mid].first) {
            left = mid;
            break;
        } else
            right = mid - 1;
    }
    return left;
}

template <typename T>
int binarySearchLastElement(const T& firstBiggerSorted, const T& c)
{
    int left = 0;
    int right = firstBiggerSorted.size() - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (COUNT_COMPARE)
            gCompareCount++;
        if (c[0].first > firstBiggerSorted[mid].first)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return left;
}

template <typename T>
void cPairSearch(T& c, T& firstBiggerSorted, size_t k, int *oldJacob)
{
    for (size_t l = 0; l < c.size(); l++)
    {
        if (c[l].second == firstBiggerSorted[k].second) // found a pair from firstBiggerSorted in c
        {
            int left = binarySearch(firstBiggerSorted, c, l, k);
            c[l].second = -1;
            if (left <= *oldJacob)
                (*oldJacob)++;
            firstBiggerSorted.insert(firstBiggerSorted.begin() + left, c[l]);
            c.erase(c.begin() + l);
            break;
        }
    }
}

template <typename T>
T firstBiggerInsertionSort(T firstBiggerSorted)
{
    if (firstBiggerSorted.size() <= 3)
        return smallArraySort(firstBiggerSorted);
    //figure out the biggest pairSize possible for the firstBiggerSorted.size() where we would only have one subvector
    size_t pairSize = 2;
    while (pairSize < firstBiggerSorted.size())
        pairSize *= 2;
    if (pairSize > firstBiggerSorted.size())
        pairSize /= 2;
    //generate jacobsthal
    std::vector<long long> jacobsthal = generateJacobsthalSequence(firstBiggerSorted.size() / 2, firstBiggerSorted);
    T main;
    T pend;
    T odd;
    T ignored;
    while (pairSize > 1)
    {
        size_t pairCounter = 0;
        pairCounter = firstBiggerSorted.size() / pairSize;
        // a vector of pairs that hold the indexes of the b and a elements' biggest numbers 
        // when getting firstbiggersorted at the beginning, make out b and a elements and save indexes in pairs
        std::vector< std::pair<int, int> > pairedIndexes;
        size_t i = 0;
        size_t x = (pairSize / 2) - 1;
        while (i < pairCounter)
        {
            pairedIndexes.push_back(std::make_pair(firstBiggerSorted[x].second, firstBiggerSorted[x + (pairSize / 2)].second));
            x += pairSize;
            i++;
        }
        pairedIndexes.erase(pairedIndexes.begin());
        //init main with b1 a1
        i = 0;
        while (i < pairSize && i < firstBiggerSorted.size())
        {
            main.push_back(firstBiggerSorted[i]);
            i++;
        }
        for (size_t j = 0; j < pairSize; j++)
            firstBiggerSorted.erase(firstBiggerSorted.begin());
        pairCounter--;
        //fill main with rest of as & init pend with rest of bs
        while (pairCounter * 2 > 0)
        {
            for (size_t k = 0; k < pairSize / 2; k++)
                pend.push_back(firstBiggerSorted[k]);
            for (size_t k = 0; k < pairSize / 2; k++)
                firstBiggerSorted.erase(firstBiggerSorted.begin());
            for (size_t k = 0; k < pairSize / 2; k++)
                main.push_back(firstBiggerSorted[k]);
            for (size_t k = 0; k < pairSize / 2; k++)
                firstBiggerSorted.erase(firstBiggerSorted.begin());
            pairCounter--;
        }
        //init odd with lone element
        //rest of as are ignored
        if (firstBiggerSorted.size() >= pairSize / 2)//odd and ignored or only odd
        {
            for (size_t j = 0; j < pairSize / 2; j++)
                odd.push_back(firstBiggerSorted[j]);
            if (firstBiggerSorted.size() > pairSize / 2)
            {
                for (size_t j = pairSize / 2; j < firstBiggerSorted.size(); j++)
                    ignored.push_back(firstBiggerSorted[j]);
            }
            while (!firstBiggerSorted.empty())
                firstBiggerSorted.erase(firstBiggerSorted.begin());
        }
        else if (firstBiggerSorted.size() < pairSize / 2 && firstBiggerSorted.size() > 0)//only ignored
        {
            for (size_t j = 0; j < firstBiggerSorted.size(); j++)
                ignored.push_back(firstBiggerSorted[j]);
            while (!firstBiggerSorted.empty())
                firstBiggerSorted.erase(firstBiggerSorted.begin());
        }
        //insertion of pend odd and ignored into main then into firstBiggerSorted

        //binary insert pend
        size_t xjac = 0;
        size_t y = 0;
        size_t insertedCount = 0;
        int comparePrev = 0;
        while (!pend.empty() && y < jacobsthal.size())
        {
            if (y > 0)
                comparePrev = xjac += insertedCount + 1;
            xjac = jacobsthal[y] + 1;
            if (y == 0)//skip first element because inserted at beginning
                xjac++;
            if (y > 0)
                xjac += insertedCount + 1; //+1 for very first b1
            if (xjac > main.size())
                xjac = main.size() / (pairSize / 2);

            int insertedInK = 0;
            bool first = true;
            //insert pend elements into main from jacobsthal to end or jacobsthal to jacobsthal
            for (int k = (xjac * (pairSize / 2) - 1); k >= 0; k -= (pairSize / 2))
            {
                if (first == true && pairSize == 2)
                {
                    first = true;
                    k = xjac * (pairSize / 2) + 1;
                }
                first = false;
                k += insertedInK;
                insertedInK = 0;
                if (pend.empty())
                    break;
                if (k > static_cast<int>(main.size()))
                    k = main.size() - 1;
                if (y > 0)
                {
                    if (k == comparePrev)
                        break;
                }
                size_t e = 0; //bPosition
                int eIndex = 0;
                //find the other half of main[k] in pairedIndexes
                bool neverFound = true;
                for (size_t j = 0; j < pairedIndexes.size(); j++)
                {
                    if (main[k].second == pairedIndexes[j].second)
                    {
                        eIndex = pairedIndexes[j].first;
                        neverFound = false;
                        break;
                    }
                }
                if (neverFound)
                    continue;
                //at which index e does pend have the .second == to eIndex
                for (size_t j = 0; j < pend.size(); j++)
                {
                    if (pend[j].second == eIndex)
                    {
                        e = j;
                        break;
                    }
                }
                //binary insertion pend element into main
                int left = 0;
                int right = k - (pairSize / 2);
                if (right < 0)
                    right = 0;
                right /= (pairSize / 2);
                std::vector<std::pair<int, int> > select;
                for (size_t j = 1; j * (pairSize / 2) - 1 < main.size(); j++)
                    select.push_back(main[j * (pairSize / 2) - 1]);
                while (left <= right)
                {
                    int mid = left + (right - left) / 2;
                    gCompareCount++;
                    if (pend[e].first > select[mid].first)
                        left = mid + 1;
                    else
                        right = mid - 1;
                }
                left *= (pairSize / 2);
                select.clear();
                int startInsert = e - (pairSize / 2) + 1;
                for (size_t j = 0; j < pairSize / 2; j++)
                {
                    if (startInsert <= comparePrev)
                        comparePrev++;
                    main.insert(main.begin() + left + j, pend[startInsert]);
                    insertedInK++;
                    startInsert++;
                }
                insertedCount++;
                //delete from pairedIndexes
                for (size_t j = 0; j < pairedIndexes.size(); j++)
                {
                    if (pend[e].second == pairedIndexes[j].first)
                    {
                        pairedIndexes.erase(pairedIndexes.begin() + j);
                        break;
                    }
                }
                //delete from pend the number on index e and the ones before that until we deleted pairSize / 2 elements from pend
                for (size_t j = 0; j < pairSize / 2; j++)
                {
                    pend.erase(pend.begin() + e);
                    e--;
                }
            }
            y++;
        }

        //binary insert odd
        if (!odd.empty())
        {
            size_t e = odd.size() - 1;
            int left = 0;
            int right = main.size() - 1;
            right /= (pairSize / 2);
            std::vector<std::pair<int, int> > select;
            for (size_t j = 1; j * (pairSize / 2) - 1 < main.size(); j++)
                select.push_back(main[j * (pairSize / 2) - 1]);
            while (left <= right)
            {
                int mid = left + (right - left) / 2;
                gCompareCount++;
                if (odd[e].first > select[mid].first)
                    left = mid + 1;
                else
                    right = mid - 1;
            }
            left *= (pairSize / 2);
            select.clear();
            for (size_t j = 0; j < pairSize / 2; j++)
                main.insert(main.begin() + left + j, odd[j]);
            //delete all from odd
            while(!odd.empty())
                odd.erase(odd.begin());
        }

        // //insert ignored at the end
        for (size_t j = 0; j < ignored.size(); j++)
            main.push_back(ignored[j]);
        ignored.clear();
        //insert main into firstBiggerSorted
        firstBiggerSorted.clear();
        for (size_t j = 0; j < main.size(); j++)
            firstBiggerSorted.push_back(main[j]);

        //clear all
        main.clear();
        pend.clear();
        odd.clear();
        pairedIndexes.clear();
        pairSize /= 2;
    }
    return firstBiggerSorted;
}

template <typename T>
T mergeInsertion(T c)
{
    if (c.size() <= 3)
        return smallArraySort(c);
    bool cIsOdd = false;
    if (c.size() % 2 != 0)
        cIsOdd = true;
    long originalCSize = c.size();
    T firstBigger;
    selectFirstBigger(c, firstBigger);
    std::vector<T> firstBiggerSortedVector = pairSortRepeat(firstBigger);
    T firstBiggerSorted = firstBiggerSortedVector[0];
    if (firstBiggerSorted.size() == 2)
    {
            if (firstBiggerSortedVector.size() > 1)
            {
                for (size_t j = 0; j < firstBiggerSortedVector[1].size(); j++)
                    firstBiggerSorted.push_back(firstBiggerSortedVector[1][j]);
            }
    }
    firstBiggerSorted = firstBiggerInsertionSort(firstBiggerSorted);
    std::vector<long long> jacobsthal = generateJacobsthalSequence(originalCSize, firstBiggerSorted);
    for (size_t i = 0; i < c.size(); i++)
    {
        if (c[i].second == firstBiggerSorted[0].second)
        {
            c[i].second = -1;
            firstBiggerSorted.insert(firstBiggerSorted.begin(), c[i]);
            c.erase(c.begin() + i);
            break;
        }
    }    
    // i is for jacobsthal iterator
    // k is for firstBiggerSorted iterator
    size_t i = 0;
    size_t j = 0;
    size_t insertedAmount = 0;
    int oldJacob = 0;
    while(!c.empty() && i < jacobsthal.size())
    {
        j = jacobsthal[i];
        if (i == 0)
            j++;
        if (i > 0)
            j += insertedAmount + 1;
        insertedAmount = 0;
        if (j >= firstBiggerSorted.size())
            j = firstBiggerSorted.size() - 1;
        if (c.size() == 1 && cIsOdd == true) // last odd element in c
        {
            int left = binarySearchLastElement(firstBiggerSorted, c);
            firstBiggerSorted.insert(firstBiggerSorted.begin() + left, c[0]);
            c.erase(c.begin());
        }
        for (int k = j; k >= 0; k--)
        {
            if (i != 0 && k == oldJacob)
                break;
            cPairSearch(c, firstBiggerSorted, k, &oldJacob);
            insertedAmount++;
        }
        if (i > 0)
            oldJacob = j + insertedAmount + 1;
        i++;
    }
    if (COUNT_COMPARE)
        std::cout << "Number of comparisons: " << gCompareCount << std::endl;
    gCompareCount = 0;
    return firstBiggerSorted;
}

class PmergeMe
{
    private:
        std::vector<std::pair<int, int> > _v;
        std::deque<std::pair<int, int> > _d;

    public:
        PmergeMe();
        ~PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        PmergeMe(char **av);
        class Error : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
        std::vector<std::pair<int, int> > getV() const;
        std::deque<std::pair<int, int> > getD() const;
};

#endif