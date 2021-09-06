#ifndef _MYSTL_HASH_SET_H
#define _MYSTL_HASH_SET_H

MYSTL_NAMESPACE_BEGIN

template <class _Value, class _HashFcn, class _EqualKey, class _Alloc>
class hash_set
{
private:
    typedef hashtable<_Value, _Value, _HashFcn, _Identity<_Value>, 
                    _EqualKey, _Alloc> _Ht;
    _Ht _M_ht;

public:
     typedef typename _Ht::key_type key_type;
    typedef typename _Ht::value_type value_type;
    typedef typename _Ht::hasher hasher;
    typedef typename _Ht::key_equal key_equal;
    typedef typename _Ht::size_type size_type;
    typedef typename _Ht::difference_type difference_type;
    typedef typename _Ht::const_pointer pointer;
    typedef typename _Ht::const_pointer const_pointer;
    typedef typename _Ht::const_reference reference;
    typedef typename _Ht::const_reference const_reference;
    typedef typename _Ht::allocator_type allocator_type;

    hash_set()
    : _M_ht(100, hasher(), key_equal(), allocator_type()) {}

    size_type size() const { return _M_ht.size(); }

    pair<iterator, bool> insert(const value_type& __obj)
    {
      pair<typename _Ht::iterator, bool> __p = _M_ht.insert_unique(__obj);
      return pair<iterator,bool>(__p.first, __p.second);
    }
};

MYSTL_NAMESPACE_END

#endif