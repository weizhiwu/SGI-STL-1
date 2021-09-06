#ifndef _MYSTL_HASH_FUNC_H
#define _MYSTL_HASH_FUNC_H

#include <stddef.h>

MYSTL_NAMESPACE_BEGIN

template <class _Key> struct hash { };

inline size_t __stl_hash_string(const char* __s)
{
    unsigned long __h = 0;
    for ( ; *__s ++__s)
        __h = 5 * __h + *__s;

    return size_t(__h);
}

struct hash<char*>
{
  size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
};

struct hash<const char*>
{
  size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
};

struct hash<char> 
{
  size_t operator()(char __x) const { return __x; }
};

MYSTL_NAMESPACE_END

#endif