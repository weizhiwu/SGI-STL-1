#ifndef _MYSTL_DEQUE_H
#define _MYSTL_DEQUE_H

MYSTL_NAMESPACE_BEGIN

inline size_t __deque_buf_size(size_t __size)
{
    return __size < 512 ? size_t(512 / __size) : size_t(1);
}

template <class _Tp, class _Ref, class _Ptr>
struct _Deque_iterator
{
    typedef _Deque_iterator<_Tp, _Tp&, _Tp*>             iterator;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }

    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Ptr pointer;
    typedef _Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator _Self;

    _Tp* _M_cur;
    _Tp* _M_first;
    _Tp* _M_last;
    _Map_pointer _M_node;

    _Deque_iterator(_Tp* __x, _Map_pointer __y) : _M_cur(__x), _M_first(*__y),
        _M_last(*__y + _S_buffer_size()), _M_node(__y) {}

    reference operator*() const { return *_M_cur; }

    void _M_set_node(_Map_pointer __new_node)
    {
        _M_node = __new_node;
        _M_first = *__new_node;
        _M_last = _M_first + difference_type(_S_buffer_size());
    }

    _Self& operator++() 
    {
        ++_M_cur;
        if (_M_cur == _M_last)
        {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }

    _Self operator++(int)  {
        _Self __tmp = *this;
        ++*this;
        return __tmp;
    }

    _Self& operator--()
    {
        if (_M_cur == _M_first)
        {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
        return *this;
    }

     _Self& operator+=(difference_type __n)
    {
        difference_type __offset = __n + (_M_cur - _M_first);
        if (__offset >= 0 && __offset < difference_type(_S_buffer_size()))
        _M_cur += __n;
        else {
        difference_type __node_offset =
            __offset > 0 ? __offset / difference_type(_S_buffer_size())
                    : -difference_type((-__offset - 1) / _S_buffer_size()) - 1;
        _M_set_node(_M_node + __node_offset);
        _M_cur = _M_first + 
            (__offset - __node_offset * difference_type(_S_buffer_size()));
        }
        return *this;
    }

     _Self operator+(difference_type __n) const
    {
        _Self __tmp = *this;
        return __tmp += __n;
    }

    reference operator[](difference_type __n) const { return *(*this + __n); }
};

template <class _Tp, class _Alloc>
class _Deque_base
{
public:
    typedef _Deque_iterator<_Tp,_Tp&,_Tp*>             iterator;

    _Deque_base(const allocator_type&, size_t __num_elements)
        : _M_map(0), _M_map_size(0), _M_start(), _M_finish()
    {
        _M_initialize_map(__num_elements);
    }
    ~_Deque_base();    

protected:
    void _M_initialize_map(size_t);
    void _M_create_nodes(_Tp** __nstart, _Tp** __nfinish);
    void _M_destroy_nodes(_Tp** __nstart, _Tp** __nfinish);
    enum { _S_initial_map_size = 8 };

    _Tp** _M_map;
    size_t _M_map_size;
    iterator _M_start;
    iterator _M_finish;

    typedef simple_alloc<_Tp, _Alloc>  _Node_alloc_type;
    typedef simple_alloc<_Tp*, _Alloc> _Map_alloc_type;

    _Tp* _M_allocate_node()
    { return _Node_alloc_type::allocate(__deque_buf_size(sizeof(_Tp))); }
    void _M_deallocate_node(_Tp* __p)
    { _Node_alloc_type::deallocate(__p, __deque_buf_size(sizeof(_Tp))); }

    _Tp** _M_allocate_map(size_t __n) 
    { return _Map_alloc_type::allocate(__n); }
    void _M_deallocate_map(_Tp** __p, size_t __n) 
    { _Map_alloc_type::deallocate(__p, __n); }
};

template <class _Tp, class _Alloc>
_Deque_base<_Tp, _Alloc>::~_Deque_base()
{
    if (_M_map)
    {
        _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1);
        _M_deallocate_map(_M_map, _M_map_size);
    }
}

template <class _Tp, class _Alloc>
void _Deque_base<_Tp, _Alloc>::_M_initialize_map(size_t __num_elements)
{
    size_t __num_nodes = __enum_elements / __deque_buf_size(sizeof(_Tp)) + 1;
    _M_map_size = max((size_t) _S_initial_map_size, __num_nodes + 2);
    _M_map = _M_allocate_map(_M_map_size);

    _Tp** __nstart = _M_map + (_M_map_size - __num_nodes) / 2;
    _Tp** __nfinish = __nstart + __num_nodes;
    _M_create_nodes(__nstart, __nfinish);
    _M_start._M_set_node(__nstart);
    _M_finish._M_set_node(__nfinish - 1);
    _M_start._M_cur = _M_start._M_first;
    _M_finish._M_cur = _M_finish._M_first + __num_elements % __deque_buf_size(sizeof(_Tp));   
}

template <class _Tp, class _Alloc>
void _Deque_base<_Tp, _Alloc>::_M_create_nodes(_Tp** __nstart, _Tp** __nfinish)
{
    _Tp** __cur;
    for (__cur = __nstart; __cur < __nfinish; ++__cur)
        *__cur = _M_allocate_node();
}

template <class _Tp, class _Alloc>
class deque : protected _Deque_base<_Tp, _Alloc>
{
    typedef _Deque_base<_Tp, _Alloc> _Base;
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef pointer* _Map_pointer;

    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }

    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }

    reference operator[](size_type __n)
    { return _M_start[difference_type(__n)]; }

    reference front() { return *_M_start; }
    reference back() 
    {
        iterator __tmp = _M_finish;
        --__tmp;
        return *__tmp;
    }

    size_type size() const { return _M_finish - _M_start; }

    explicit deque(size_type __n) : _Base(allocator_type(), __n)
    { _M_fill_initialize(value_type()); }

    ~deque() { destroy(_M_start, _M_finish); }

    deque& operator= (const deque& __x)
    {
        const size_type __len = size();
        if (&__x != this)
        {
            if (__len >= __x.size())
                erase(copy(__x.begin(), __x.end(), _M_start), _M_finish);
            else
            {
                const_iterator __mid = __x.begin() + difference_type(__len);
                copy(__x.begin(), __mid, _M_start);
                insert(_M_finish, __mid, __x.end());
            }
        }
        return *this;
    }

    void _M_fill_assign(size_type __n, const _Tp& __val)
    {
        if (__n > size()) {
        fill(begin(), end(), __val);
        insert(end(), __n - size(), __val);
        }
        else {
        erase(begin() + __n, end());
        fill(begin(), end(), __val);
        }
    }

public:
    void push_back(const value_type& __t)
    {
        if (_M_finish._M_cur != _M_finish._M_last - 1)
        {
            construct(_M_finish._M_cur, __t);
            ++_M_finish._M_cur;
        }
        else
            _M_push_back_aux(__t);
    }

    void push_front(const value_type& __t)
    {
        if (_M_start._M_cur != _M_start._M_first)
        {
            construct(_M_start._M_cur - 1, __t);
            --_M_start._M_cur;
        }
        else
            _M_push_front_aux(__t);
    }

    void pop_back()
    {
        if (_M_finish._M_cur != _M_finish._M_first)
        {
            --_M_finish._M_cur;
            destroy(_M_finish._M_cur);
        }
        else
            _M_pop_back_aux();
    }

    void pop_front()
    {
        if (_M_start._M_cur != _M_start._M_last - 1)
        {
            destroy(_M_start._M_cur);
            ++_M_start._M_cur;
        }
        else
            _M_pop_front_aux();
    }

    iterator insert(iterator position, const value_type& __x)
    {
        if (position._M_cur == _M_start._M_cur)
        {
            push_front(__x);
            return _M_start;
        }
        else if (position._M_cur == _M_finish._M_cur)
        {
            push_back(__x);
            iterator __tmp = _M_finish;
            --__tmp;
            return __tmp;
        }
        else
        {
            return _M_insert_aux(position, __x);
        }
    }

    void _M_fill_insert(iterator __pos, size_type __n, const value_type& __x); 

    void resize(size_type __new_size, const value_type& __x)
    {
        const size_type __len = size();
        if (__new_size < __len)
            erase(_M_start + __new_size, _M_finish);
        else
            insert(_M_finish, __new_size - __len, __x);
    }

    void _M_fill_initialize(const value_type& __value);

    iterator erase(iterator __pos)
    {
        iterator __next = __pos;
        ++__next;
        difference_type __index = __pos - _M_start;
        if (size_type(__index) < (this->size() >> 1))
        {
            copy_backward(_M_start, __pos, __next);
            pop_front();
        }
        else
        {
            copy(__next, _M_finish, __pos);
            pop_back();
        }
        return _M_start + __index;
    }

    void _M_push_back_aux(const value_type&);
    void _M_push_front_aux(const value_type&);
    void _M_pop_back_aux();
    void _M_pop_front_aux();

    iterator _M_insert_aux(iterator __pos, const value_type& __x);
    void _M_insert_aux(iterator __pos, size_type __n, const value_type& __x);

    iterator _M_reserve_elements_at_front(size_type __n)
    {
        size_type __vacancies = _M_start._M_cur - _M_start._M_first;
        if (__n > __vacancies)
            _M_new_elements_at_front(__n - __vacancies);
        return _M_start - difference_type(__n);
    }

    void _M_new_elements_at_front(size_type __new_elements);
    void _M_new_elements_at_back(size_type __new_elements);

protected:
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add + 1 > _M_map_size - (_M_finish._M_node - _M_map))
            _M_reallocate_map(__nodes_to_add, false);
    }

    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);
};

// Called only if _M_finish._M_cur == _M_finish._M_last - 1.
template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_push_back_aux(const value_type& __t)
{
    value_type __t_copy = __t;
    _M_reserve_map_at_back();
    *(_M_finish._M_node + 1) = _M_allocate_node();
    {
        construct(_M_finish._M_cur, __t_copy);
        _M_finish._M_set_node(_M_finish._M_node + 1);
        _M_finish._M_cur = _M_finish._M_first;
    }
}

// Called only if _M_finish._M_cur == _M_finish._M_first.
template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_pop_back_aux()
{
    _M_deallocate_node(_M_finish._M_first);
    _M_finish._M_set_node(_M_finish._M_node - 1);
    _M_finish._M_cur = _M_finish._M_last - 1;
    destroy(_M_finish._M_cur);
}

template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_new_elements_at_front(size_type __new_elems)
{
    size_type __new_nodes = (__new_elems + _S_buffer_size() - 1) / _S_buffer_size();
    _M_reserve_map_at_front(__new_nodes);
    size_type __i;
    for (__i = 1; __i <= __new_nodes; ++__i)
        *(_M_start._M_node - __i) = _M_allocate_node();
}

template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_reallocate_map(size_type __nodes_to_add, bool __add_at_front)
{
    size_type __old_num_nodes = _M_finish._M_node - _M_start._M_node + 1;
    size_type __new_num_nodes = __old_num_nodes + __nodes_to_add;

    _Map_pointer __new_nstart;
    if (_M_map_size > 2 * __new_num_nodes)
    {
        __new_nstart = _M_map + (_M_map_size - __new_num_nodes) / 2 
                     + (__add_at_front ? __nodes_to_add : 0);
        if (__new_nstart < _M_start._M_node)
        copy(_M_start._M_node, _M_finish._M_node + 1, __new_nstart);
        else
        copy_backward(_M_start._M_node, _M_finish._M_node + 1, 
                        __new_nstart + __old_num_nodes);
    }
    else
    {
        size_type __new_map_size = 
        _M_map_size + max(_M_map_size, __nodes_to_add) + 2;

        _Map_pointer __new_map = _M_allocate_map(__new_map_size);
        __new_nstart = __new_map + (__new_map_size - __new_num_nodes) / 2
                            + (__add_at_front ? __nodes_to_add : 0);
        copy(_M_start._M_node, _M_finish._M_node + 1, __new_nstart);
        _M_deallocate_map(_M_map, _M_map_size);

        _M_map = __new_map;
        _M_map_size = __new_map_size;
    }

    _M_start._M_set_node(__new_nstart);
    _M_finish._M_set_node(__new_nstart + __old_num_nodes - 1);
}

MYSTL_NAMESPACE_END

#endif