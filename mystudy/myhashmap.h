#ifndef _MYSTL_HASH_MAP_H
#define _MYSTL_HASH_MAP_H

MYSTL_NAMESPACE_BEGIN

template <class _Key, class _Tp, class _HashFcn, class _EqualKey, class _Alloc>
class hash_map
{
private:
    typedef hashtable<pair<const _Key, _Tp>, _Key, _HashFcn, _Select1st<pair<const _Key,_Tp>>,_EqualKey,_Alloc> _Ht;
    _Ht _M_ht;

public:
    typedef typename _Ht::key_type key_type;
    typedef _Tp data_type;
    typedef _Tp mapped_type;
    typedef typename _Ht::value_type value_type;

    hash_map() : _M_ht(100, hasher(), key_equal(), allocator_type()) {}

    pair<iterator,bool> insert(const value_type& __obj)
    { return _M_ht.insert_unique(__obj); }

    _Tp& operator[](const key_type& __key)
    { return _M_ht.find_or_insert(value_type(__key, _Tp())).second; }
};

MYSTL_NAMESPACE_END

#endif