#include "lru.h"
#include <iostream>

//compile:  g++ lru.cpp -o lru -std=c++11

int main()
{
    std::vector<std::vector<int>> vec = {{1, 1, 1}, {1, 2, 2}, {1, 3, 2}, {2, 1}, {1, 4, 4}, {2, 2}};
    Solution so;
    auto result = so.LRU(vec, 3);
    for (auto v : result)
    {
        std::cout<<v<<'\t';
    }
    std::cout<<std::endl;
}