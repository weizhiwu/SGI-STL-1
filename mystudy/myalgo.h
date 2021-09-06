#ifndef _MYSTL_ALGO_H
#define _MYSTL_ALGO_H

template <class _BidirectionalIter>
inline void reverse(_BidirectionalIter __first, _BidirectionalIter __last)
{
    __reverse(__first, __last, __ITERATOR_CATEGORY(__first));
}

template <class _BidirectionalIter>
void __reverse(_BidirectionalIter __first, _BidirectionalIter __last, bidirectional_iterator_tag)
{
    while (true)
    {
        if (__first == __last || __first == --__last)
            return;
        else
            iter_swap(__first++, __last);
    }
}

template <class _RandomAccessIter>
void __reverse(_RandomAccessIter __first, _RandomAccessIter __last, random_access_iterator_tag)
{
    while (__first < __last)
        iter_swap(__first++, --__last);
}

template <class _Distance>
inline _Distance __random_number(_Distance __n)
{
    return rand() % __n;
}

template <class _RandomAccessIter>
inline void random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first == __last) return;

    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    {
        iter_swap(__i, __first + __random_number((__i - __first) + 1));
    }
}

#endif