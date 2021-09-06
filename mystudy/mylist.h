#ifndef _MYSTL_LIST_H
#define _MYSTL_LIST_H

MYSTL_NAMESPACE_BEGIN

struct _List_node_base
{
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp>
struct _List_node : public _List_node_base
{
    _Tp _M_data;
};

struct _List_iterator_base
{
    typedef size_t  size_type;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag iterator_category;

    _List_node_base* _M_node;

    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base() {}

    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
};

template <class _Tp, class _Ref, class _Ptr>
struct _List_iterator : public _List_iterator_base
{
    typedef _List_iterator<_Tp,_Tp&,_Tp*>             iterator;
    typedef _List_iterator<_Tp,_Ref,_Ptr>             _Self;

    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef _List_node<_Tp> _Node;

    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}

    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    pointer operator->() const { return &(operator*()); }

    _Self& operator++() {
        this->_M_incr();
        return *this;
    }

    _Self operator++(int) {
        _Self __tmp = *this;
        this->_M_incr();
        return __tmp;
    }
};

template <class _Tp, class _Alloc>
class _List_base
{
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    //dummy node
    _List_base(const allocator_type&)
    {
        _M_node = _M_get_node();
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }

    ~_List_base()
    {
        clear();
        _M_put_node(_M_node);
    }

    void clear();

protected:
    typedef simple_alloc<_List_node<_Tp>, _Alloc> _Alloc_type;
    _List_node<_Tp>* _M_get_node() { return _Alloc_type::allocate(1); }
    void _M_put_node(_List_node<_Tp>* __p) { _Alloc_type::deallocate(__p, 1); } 

    _List_node<_Tp>* _M_node;
};

template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*) _M_node->_M_next;
    while (__cur != _M_node)
    {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        _M_put_node(__tmp);
    }

    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
}

template <class _Tp, class _Alloc>
class list : protected _List_base<_Tp, _Alloc>
{
    typedef _List_base<_Tp, _Alloc> _Base;
protected:
    typedef void* _Void_pointer;

public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef _List_node<_Tp> _Node;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return _Base::get_allocator(); }

    typedef _List_iterator<_Tp,_Tp&,_Tp*>             iterator;
    typedef _List_iterator<_Tp,const _Tp&,const _Tp*> const_iterator;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = _M_get_node();
        _Construct(&__p->_M_data, __x);
        return __p;
    }

public:
    explicit list(const allocator_type& __a = allocator_type()) : _Base(__a) {}

    iterator begin() { return (_Node*)(_M_node->_M_next); }
    iterator end() { return _M_node; }

    bool empty() const { return _M_node->_M_next == _M_node; }
    size_type size() const 
    {
        size_type __result = 0;
        distance(begin(), end(), __result);
        return __result;
    }
    reference front() { return *begin(); }
    reference back() { return *(--end()); }
    void swap(list<_Tp, _Alloc>& __x) { std::swap(_M_node, __x._M_node); }

    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        __tmp->_M_next = __position._M_node;
        __tmp->_M_prev = __position._M_node->_M_prev;
        __position._M_node->_M_prev->_M_next = __tmp;
        __position._M_node->_M_prev = __tmp;
    }

    void insert(iterator __position, const _Tp* __first, const _Tp* __last);
    void insert(iterator __position, const_iterator __first, const_iterator __last);

    void insert(iterator __pos, size_type __n, const _Tp& __x)
    { _M_fill_insert(__pos, __n, __x); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);

    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }

    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_put_node(__n);
        return iterator((_Node*) __next_node);
    }

    iterator erase(iterator __first, iterator __last);
    void clear() { _Base::clear(); }

    void resize(size_type __new_size, const _Tp& __x);

    void pop_front() { erase(begin()); }

    void pop_back() { 
        iterator __tmp = end();
        erase(--__tmp);
    }

    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);

    void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }
    void _M_fill_assign(size_type __n, const _Tp& __val);

    void transfer(iterator __position, iterator __first, iterator __last)
    {
        // Remove [first, last) from its old position.
        __last._M_node->_M_prev->_M_next     = __position._M_node;
        __first._M_node->_M_prev->_M_next    = __last._M_node;
        __position._M_node->_M_prev->_M_next = __first._M_node; 

        // Splice [first, last) into its new position.
        _List_node_base* __tmp      = __position._M_node->_M_prev;
        __position._M_node->_M_prev = __last._M_node->_M_prev;
        __last._M_node->_M_prev     = __first._M_node->_M_prev; 
        __first._M_node->_M_prev    = __tmp;
    }

    void splice(iterator __position, list& __x)
    {
        if (!__x.empty())
            this->transfer(__position, __x.begin(), __x.end());
    }

    void splice(iterator __position, list&, iterator __i) {
        iterator __j = __i;
        ++__j;
        if (__position == __i || __position == __j) return;
        this->transfer(__position, __i, __j);
    }
    void splice(iterator __position, list&, iterator __first, iterator __last) {
        if (__first != __last) 
         this->transfer(__position, __first, __last);
    }

    void remove(const _Tp& __value);
    void unique();
    void merge(list& __x);
    void reverse();
    void sort();
};

template <class _Tp, class _Alloc>
inline bool 
operator==(const list<_Tp,_Alloc>& __x, const list<_Tp,_Alloc>& __y)
{
    typedef typename list<_Tp,_Alloc>::const_iterator const_iterator;
    const_iterator __end1 = __x.end();
    const_iterator __end2 = __y.end();

    const_iterator __i1 = __x.begin();
    const_iterator __i2 = __y.begin();
    while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2) {
        ++__i1;
        ++__i2;
    }
    return __i1 == __end1 && __i2 == __end2;
}

template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::insert(iterator __position, const _Tp* __first, const _Tp* __last)
{
    for (; __first != __last; ++__first)
        insert(__position, *__first);
}

template <class _Tp, class _Alloc>
typename list<_Tp,_Alloc>::iterator list<_Tp, _Alloc>::erase(iterator __first, iterator __last)
{
    while (__first != __last)
        erase(__first++);
    return __last;
}

template <class _Tp, class _Alloc>
list<_Tp, _Alloc>& list<_Tp, _Alloc>::operator=(const list<_Tp, _Alloc>& __x)
{
    if (this != &__x)
    {
        iterator __first1 = begin();
        iterator __last1 = end();
        const_iterator __first2 = __x.begin();
        const_iterator __last2 = __x.end();
        while (__first1 != __last1 && __first2 != __last2)
            *__first1++ = *__first2++;
        if (__first2 == __last2)
            erase(__first1, __last1);
        else
            insert(__last1, __first2, __last2);
    }
}

template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::remove(const _Tp& __value)
{
    iterator __first = begin();
    iterator __last = end();
    while (__first != __last)
    {
        iterator __next = __first;
        ++__next;
        if (*__first == __value) erase(__first);
        __first = __next;
    }
}

template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::merge(list<_Tp, _Alloc>& __x)
{
    iterator __first1 = begin();
    iterator __last1 = end();
    iterator __first2 = __x.begin();
    iterator __last2 = __x.end();
    while (__first1 != __last1 && __first2 != __last2)
    {
        if (*__first2 < *__first1)
        {
            iterator __next = __first2;
            transfer(__first1, __first2, ++__next);
            __first2 = __next;
        }
        else
            ++__first1;
    }
    if (__first2 != __last2) transfer(__last1, __first2, __last2);
}

inline void __List_base_reverse(_List_node_base* __p)
{
    _List_node_base* __tmp = __p;
    do 
    {
        std::swap(__tmp->_M_next, __tmp->_M_prev);
        __tmp = __tmp->_M_prev;
    } while (__tmp != __p);
}

template <class _Tp, class _Alloc>
inline void list<_Tp, _Alloc>::reverse() 
{
    __List_base_reverse(this->_M_node);
}    

template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::sort()
{
    // Do nothing if the list has length 0 or 1.
    if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node)
    {
        list<_Tp, _Alloc> __carry;
        list<_Tp, _Alloc> __counter[64];
        int __fill = 0;
        while (!empty())
        {
            __carry.splice(__carry.begin(), *this, begin());
            int __i = 0;
            while (__i < __fill && !__counter[__i].empty())
            {
                __counter[__i].merge(__carry);
                __carry.swap(__counter[__i++]);
            }
            __carry.swap(__counter[__i]);
            if (__i == __fill) ++__fill;
        }

        for (int __i = 1; __i < __fill; ++__i)
            __counter[__i].merge(__counter[__i-1]);
        swap(__counter[__fill-1]);
    }
}

MYSTL_NAMESPACE_END

#endif