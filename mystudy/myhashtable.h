#ifndef _MYSTL_HASH_TABLE_H
#define _MYSTL_HASH_TABLE_H

MYSTL_NAMESPACE_BEGIN

template <class _Val>
struct _Hashtable_node
{
    _Hashtable_node* _M_next;
    _Val _M_val;
};

template <class _Val, class _Key, class _HashFcn, class _ExtraceKey, class _EqualKey, class _Alloc = alloc>
class hashtable;

template <class _Val, class _Key, class _HashFcn, class _ExtraceKey, class _EqualKey, class _Alloc = alloc>
struct _Hashtable_iterator;

template <class _Val, class _Key, class _HashFcn, class _ExtraceKey, class _EqualKey, class _Alloc = alloc>
struct _Hashtable_iterator
{
    typedef hashtable<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,_Alloc>
          _Hashtable;
    typedef _Hashtable_iterator<_Val, _Key, _HashFcn, 
                                _ExtractKey, _EqualKey, _Alloc>
            iterator;
    typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, 
                                        _ExtractKey, _EqualKey, _Alloc>
            const_iterator;
    typedef _Hashtable_node<_Val> _Node;

    typedef forward_iterator_tag iterator_category;
    typedef _Val value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef _Val& reference;
    typedef _Val* pointer;

    _Node* _M_cur;
    _Hashtable* _M_ht;

    _Hashtable_iterator(_Node* __n, _Hashtable* __tab) : _M_cur(__n), _M_ht(__tab) {}
    reference operator*() const { return _M_cur->_M_val; }
    pointer operator->() const { return &(operator*()); }

    iterator& operator++();
    iterator operator++(int);

    bool operator==(const iterator& __it) const
    { return _M_cur == __it._M_cur; }
    bool operator!=(const iterator& __it) const
    { return _M_cur != __it._M_cur; }
};

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, class _All>
_Hashtable_iterator<_Val, _Key, _HF, _Exk, _EqK, _All>&
_Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>::operator++()
{
    const _Node* __old = _M_cur;
    _M_cur = _M_cur->_M_next;
    if (!_M_cur)
    {
        size_type __bucket = _M_ht->_M_bkt_num(__old->_M_val);
        while (!_M_cur && ++__bucket < _M_ht->_M_buckets.size())
            _M_cur = _M_ht->_M_buckets[__bucket];
    }
    return *this;
}

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, class _All>
_Hashtable_iterator<_Val, _Key, _HF, _Exk, _EqK, _All>&
_Hashtable_iterator<_Val,_Key,_HF,_ExK,_EqK,_All>::operator++(int)
{
    iterator __tmp = *this;
    ++*this;
    return __tmp;
}

// Note: assumes long is at least 32 bits.
enum { __stl_num_primes = 28 };

static const unsigned long __stl_prime_list[__stl_num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long __n)
{
  const unsigned long* __first = __stl_prime_list;
  const unsigned long* __last = __stl_prime_list + (int)__stl_num_primes;
  const unsigned long* pos = lower_bound(__first, __last, __n);
  return pos == __last ? *(__last - 1) : *pos;
}

template <class _Val, class _Key, class _HashFcn,
          class _ExtractKey, class _EqualKey, class _Alloc>
class hashtable 
{
public:
    typedef _Key key_type;
    typedef _Val value_type;
    typedef _HashFcn hasher;
    typedef _EqualKey key_equal;

    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;

    hasher hash_funct() const { return _M_hash; }
    key_equal key_eq() const { return _M_equals; }

    typedef _Hashtable_node<_Val> _Node;

    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    typedef simple_alloc<_Node, _Alloc> _M_node_allocator_type;
    _Node* _M_get_node() { return _M_node_allocator_type::allocate(1); }
    void _M_put_node(_Node* __p) { _M_node_allocator_type::deallocate(__p, 1); }

private:
    hasher                _M_hash;
    key_equal             _M_equals;
    _ExtractKey           _M_get_key;
    vector<_Node*, _Alloc> _M_buckets;
    size_type             _M_num_elements;

public:
    typedef _Hashtable_iterator<_Val,_Key,_HashFcn,_ExtractKey,_EqualKey,_Alloc>
          iterator;

    hashtable(size_type __n,
            const _HashFcn&    __hf,
            const _EqualKey&   __eql,
            const _ExtractKey& __ext,
            const allocator_type& __a = allocator_type())
    : __HASH_ALLOC_INIT(__a)
      _M_hash(__hf),
      _M_equals(__eql),
      _M_get_key(__ext),
      _M_buckets(__a),
      _M_num_elements(0)
    {
        _M_initialize_buckets(__n);
    }

    ~hashtable() { clear(); }
    
    hashtable& operator=(const hashtable& __ht)
    {
        if (&__ht != this)
        {
            clear();
            _M_hash = __ht._M_hash;
            _M_equals = __ht._M_equals;
            _M_get_key = __ht._M_get_key;
            _M_copy_from(__ht);
        }
        return *this;
    }

    size_type size() const { return _M_num_elements; }
    bool empty() const { return size() == 0; }

    void swap(hashtable& __ht)
    {
        __STD::swap(_M_hash, __ht._M_hash);
        __STD::swap(_M_equals, __ht._M_equals);
        __STD::swap(_M_get_key, __ht._M_get_key);
        _M_buckets.swap(__ht._M_buckets);
        __STD::swap(_M_num_elements, __ht._M_num_elements);
    }

    iterator begin()
    { 
        for (size_type __n = 0; __n < _M_buckets.size(); ++__n)
        if (_M_buckets[__n])
            return iterator(_M_buckets[__n], this);
        return end();
    }

    iterator end() { return iterator(0, this); }

    size_type bucket_count() const { return _M_buckets.size(); }

     pair<iterator, bool> insert_unique(const value_type& __obj)
    {
        resize(_M_num_elements + 1);
        return insert_unique_noresize(__obj);
    }

    iterator insert_equal(const value_type& __obj)
    {
        resize(_M_num_elements + 1);
        return insert_equal_noresize(__obj);
    }

    pair<iterator, bool> insert_unique_noresize(const value_type& __obj);
    iterator insert_equal_noresize(const value_type& __obj);

    size_type _M_bkt_num_key(const key_type& __key, size_t __n) const
    {
        return _M_hash(__key) % __n;
    }

    size_type _M_bkt_num_key(const key_type& __key) const
    {
        return _M_bkt_num_key(__key, _M_buckets.size());
    }

    size_type _M_bkt_num(const value_type& __obj) const
    {
        return _M_bkt_num_key(_M_get_key(__obj));
    }

    _Node* _M_new_node(const value_type& __obj)
    {
        _Node* __n = _M_get_node();
        __n->_M_next = 0;
        construct(&__n->_M_val, __obj);
        return __n;
    }

    void _M_delete_node(_Node* __n)
    {
        destroy(&__n->_M_val);
        _M_put_node(__n);
    }

    size_type _M_next_size(size_type __n) const
    { return __stl_next_prime(__n); }

    void _M_initialize_buckets(size_type __n)
    {
        const size_type __n_buckets = _M_next_size(__n);
        _M_buckets.reserve(__n_buckets);
        _M_buckets.insert(_M_buckets.end(), __n_buckets, (_Node*) 0);
        _M_num_elements = 0;
    }

    void resize(size_type __num_elements_hint);
    void clear();

     void insert_unique(const value_type* __f, const value_type* __l)
    {
        size_type __n = __l - __f;
        resize(_M_num_elements + __n);
        for ( ; __n > 0; --__n, ++__f)
        insert_unique_noresize(*__f);
    }

    void insert_equal(const value_type* __f, const value_type* __l)
    {
        size_type __n = __l - __f;
        resize(_M_num_elements + __n);
        for ( ; __n > 0; --__n, ++__f)
        insert_equal_noresize(*__f);
    }

    reference find_or_insert(const value_type& __obj);

    iterator find(const key_type& __key)
    {
        size_type __n = _M_bkt_num_key(__key);
        _Node* __first;
        for (__first = _M_buckets[__n]; __first && !_M_equals(_M_get_key(__first->_M_val), __key); __first = __first->_M_next)
        {}

        return iterator(__first, this);
    }

    pair<iterator, iterator> 
    equal_range(const key_type& __key);

    size_type erase(const key_type& __key);
    void erase(const iterator& __it);
    void erase(iterator __first, iterator __last);

    void _M_erase_bucket(const size_type __n, _Node* __first, _Node* __last);
    void _M_erase_bucket(const size_type __n, _Node* __last);
    void _M_copy_from(const hashtable& __ht);
}; 

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
void hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::resize(size_type __num_elements_hint)
{
    const size_type __old_n = _M_buckets.size();
    if (__num_elements_hint > __old_n)
    {
        const size_type __n = _M_next_size(__num_elements_hint);
        if (__n > __old_n)
        {
            vector<_Node*, _All> __tmp(__n, (_Node*)(0), _M_buckets.get_allocator());
            for (size_type __bucket = 0; __bucket < __old_n; ++__bucket)
            {
                _Node* __first = _M_buckets[__bucket];
                while (__first)
                {
                    size_type __new_bucket = _M_bkt_num(__first->_M_val, __n);
                    _M_buckets[__bucket] = __first->_M_next;
                    __first->_M_next = __tmp[__new_bucket];
                    __tmp[__new_bucket] = __first;
                    __first = _M_buckets[__bucket];  
                }  
            } 
            _M_buckets.swap(__tmp);
        }
    }
}

template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::insert_equal_noresize(const value_type& __obj)
{
    const size_type __n = _M_bkt_num(__obj);
    _Node* __first = _M_buckets[__n];

    for (_Node* __cur = __first; __cur; __cur = __cur->_M_next)
    {
        if (_M_equals(_M_get_key(__cur->_M_val), _M_get_key(__obj)))
        {
            _Node* __tmp = _M_new_node(__obj);
            __tmp->_M_next = __cur->_M_next;
            __cur->_M_next = __tmp;
            ++_M_num_elements;
            return iterator(__tmp, this);
        }
    }

    _Node* __tmp = _M_new_node(__obj);
    __tmp->_M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return iterator(__tmp, this);
}


template <class _Val, class _Key, class _HF, class _Ex, class _Eq, class _All>
pair<typename hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>::iterator, bool> 
hashtable<_Val,_Key,_HF,_Ex,_Eq,_All>
  ::insert_unique_noresize(const value_type& __obj)
{
    const size_type __n = _M_bkt_num(__obj);
    _Node* __first = _M_buckets[__n];

    for (_Node* __cur = __first; __cur; __cur = __cur->_M_next)
    {
        if (_M_equals(_M_get_key(__cur->_M_val), _M_get_key(__obj)))
        {
            return pair<iterator, bool>(iterator(__cur, this), false);
        }
    }

    _Node* __tmp = _M_new_node(__obj);
    __tmp->_M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return pair<iterator, bool>(iterator(__tmp, this), true);
}

MYSTL_NAMESPACE_END

#endif