#include "bsearch.h"
#include <iostream>

//compile:  g++ .\bsearch.cpp -o bsearch -std=c++11

int BinarySearch::search(std::vector<int>& vec, int value)
{
    int low = 0;
    int high = vec.size() - 1;

    while (low <= high)
    {
        int mid = low + ((high - low) >> 1);
        if (vec[mid] == value)
            return mid;
        else if (vec[mid] < value) 
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

int main()
{
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int find = BinarySearch::search(vec, 6);
    if (find == -1)
        std::cout<<"not find"<<std::endl;
    else
        std::cout<<"find pos:"<<find<<std::endl;
}