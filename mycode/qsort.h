#ifndef _QUICK_SORT_H
#define _QUICK_SORT_H

#include <vector>

class QuickSort
{
public:
    static void sort(std::vector<int>& vec);
    static void sort_c(std::vector<int>& vec, int p, int r);
    static int partion(std::vector<int>& vec, int p, int r);

    //找到第k大的元素
    static void findRankOfK(std::vector<int>& vec, int k);
    static void sort_c_rank(std::vector<int>& vec, int p, int r, int k);
};

#endif