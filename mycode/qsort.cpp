#include <iostream>
#include "qsort.h"

//compile: g++ qsort.cpp -o qsort -std=c++11

void QuickSort::sort(std::vector<int>& vec)
{
    sort_c(vec, 0, vec.size()-1);
}

void QuickSort::sort_c(std::vector<int>& vec, int p, int r)
{
    if (p >= r) return;

    int q = partion(vec, p, r);
    sort_c(vec, p, q-1);
    sort_c(vec, q+1, r);
}

//获取分区点， 单边扫描
int QuickSort::partion(std::vector<int>& vec, int p, int r)
{
    int pivot = vec[r];
    int i = p;
    for (int j = p; j <= r - 1; ++j)
    {
        if (vec[j] < pivot)
        {
            std::swap(vec[i], vec[j]);
            i++;
        }
    }
    std::swap(vec[i], vec[r]);
    return i;
}

void QuickSort::findRankOfK(std::vector<int>& vec, int k)
{
    sort_c_rank(vec, 0, vec.size()-1, k);
}

void QuickSort::sort_c_rank(std::vector<int>& vec, int p, int r, int k)
{
    if (p >= r) return;
    if (p >= k) return;
    if (r < k-1) return;

    int q = partion(vec, p, r);
    if (q == k-1)
    {//找到元素
        return;
    }
    sort_c_rank(vec, p, q-1, k);
    sort_c_rank(vec, q+1, r, k);
}

int main()
{
    std::vector<int> vec{3, 11, 5, 1, 19, 2, 8, 20};
    QuickSort::sort(vec);

    for (auto& value : vec)
    {
        std::cout<<value<<'\t';
    }
    std::cout<<std::endl;

    std::vector<int> vec1{3, 11, 5, 1, 19, 2, 8, 20};
    QuickSort::findRankOfK(vec1, 5);
    std::cout<<vec1[4]<<std::endl;
     for (auto& value : vec1)
    {
        std::cout<<value<<'\t';
    }
    std::cout<<std::endl;
}