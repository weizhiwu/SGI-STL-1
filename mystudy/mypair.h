#ifndef _MYSTL_PAIR_H
#define _MYSTL_PAIR_H

//pair重写实现

#include "myconfig.h"

MYSTL_NAMESPACE_BEGIN

template <class _T1, class _T2>
struct pair 
{
    typedef _T1 first_type;
    typedef _T2 second_type;

    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}

    template <class _U1, class _U2>
    pair(const pair<_U1, _U2>& p) : first(p.first), second(p.second) {}
};

template <class _T1, class _T2>
inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first == __y.first && __x.second == __y.second;
}

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first ||
        (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _T1, class _T2>
inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
{
    return pair<_T1, _T2>(__x, __y);
}

MYSTL_NAMESPACE_END

#endif