#ifndef _MYSTL_VECTOR_H
#define _MYSTL_VECTOR_H

MYSTL_NAMESPACE_BEGIN

template <class _Tp, class _Alloc>
class _Vector_base 
{
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    _Vector_base(const _Alloc&) : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    _Vector_base(size_t __n, const _Alloc&) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = _M_allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + __n;
    }

    ~_Vector_base() { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }

protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    typedef simple_alloc<_Tp, _Alloc> _M_data_allocator;
    _Tp* _M_allocate(size_t __n) { return _M_data_allocator::allocate(__n); }
    void _M_deallocate(_Tp* __p, size_t __n)
    {
        _M_data_allocator::deallocate(__p, __n);
    }
};

template <class _Tp, class _Alloc>
class vector : protected _Vector_base<_Tp, _Alloc>
{
    typedef _Vector_base<_Tp, _Alloc> _Base;
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return _Base::get_allocator(); }

    iterator begin() { return _M_start; }
    const_iterator begin() const { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator end() const { return _M_finish; }

    size_type size() const
    { return size_type(end() - begin()); }
    size_type max_size() const
    { return size_type(-1) / sizeof(_Tp); }
    size_type capacity() const
    { return size_type(_M_end_of_storage - begin()); }
    bool empty() const
    { return begin() == end(); }

    reference operator[](size_type __n) { return *(begin() + __n); }

public:
    vector(size_type __n, const _Tp& __value, const allocator_type& __a = allocator_type()) : _Base(__n, __a)
    {
        _M_finish = unitialized_fill_n(_M_start, __n, __value);
    }

    vector(const vector<_Tp, _Alloc>& __x) : _Base(__x.size(), __x.get_allocator())
    {
        _M_finish = uninitialized_copy(__x.begin(), __x.end(), _M_start);
    }

    ~vector() { destroy(_M_start, _M_finish); }

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }

    void reserve(size_type __n)
    {
        if (capacity() < __n)
        {
            const size_type __old_size = size();
            iterator __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
            destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __tmp;
            _M_finish = __tmp + __old_size;
            _M_end_of_storage = _M_start + __n;
        }
    }

    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage)
        {
            construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_aux(end(), __x);
    }

    void _M_insert_aux(iterator __position, const _Tp& __x);

    void swap(vector<_Tp, _Alloc>& __x)
    {
        std::swap(_M_start, __x._M_start);
        std::swap(_M_finish, __x._M_finish);
        std::swap(_M_end_of_storage, __x._M_end_of_storage);
    }

    iterator insert(iterator __position, const _Tp& __x)
    {
        size_type __n = __position - begin();
        if (_M_finish != _M_end_of_storage && __position == end()) {
        construct(_M_finish, __x);
        ++_M_finish;
        }
        else
        _M_insert_aux(__position, __x);
        return begin() + __n;
    }

    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
        {
            copy(__position + 1, _M_finish, __position);
        }

        --_M_finish;
        destroy(_M_finish);
        return __position;
    }

    void clear() { erase(begin(), end()); }

    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }

    iterator _M_allocate_and_copy(size_type __n, const_iterator __first, const_iterator __last)
    {
        iterator __result = _M_allocate(__n);
        {
            uninitialized_copy(__first, __last, __result);
            return __result;
        }
    }

    void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }
    void _M_fill_assign(size_type __n, const _Tp& __val);

    void insert(iterator __pos, size_type __n, const _Tp& __x)
    {
        _M_fill_insert(__pos, __n, __x);
    }

    void _M_fill_insert (iterator __pos, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_t __n, const value_type& __val)
{
    if (__n > capacity())
    {
        vector<_Tp, _Alloc> __tmp(__n, __val, get_allocator());
        __tmp.swap(*this);
    }
    else if (__n > size())
    {
        fill(begin(), end(), __val);
        _M_finish = uninitialized_fill_n(_M_finish, __n - size(), __val);
    }
    else
        erase(fill_n(begin(), __n, __val), end());
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x)
{
    if (_M_finish != _M_end_of_storage)
    {
        construct(_M_finish, *(_M_finish - 1));
        ++_M_finish;
        _Tp __x_copy = __x;
        copy_backward(__position, _M_finish - 2, _M_finish - 1);
        *__position = __x_copy;
    }
    else
    {
        const size_type __old_size = size();
        const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
        iterator __new_start = _M_allocate(__len);
        iterator __new_finish = __new_start;
        {
            __new_finish = uninitialized_copy(_M_start, __position, __new_start);
            construct(__new_finish, __x);
            ++__new_finish;
            __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
        }

        //销毁之前空间，导致以前迭代器失效
        destroy(begin(), end());
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);

        //设置容器新地址位置
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage = __new_start + __len;
    }
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0)
    {
        if (size_type(_M_end_of_storage - _M_finish) >= __n)
        {//空间足够
            _Tp __x_copy = __x;
            const size_type __elems_after = _M_finish - __position;
            iterator __old_finish = _M_finish;
            if (__elems_after > __n) 
            {
                uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
                _M_finish += __n;
                copy_backward(__position, __old_finish - __n, __old_finish);
                fill(__position, __position + __n, __x_copy);
            }
            else
            {
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - __elems_after;
                uninitialized_copy(__position, __old_finish, _M_finish);
                _M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
        {
            const size_type __old_size = size();       
            const size_type __len = __old_size + max(__old_size, __n);
            iterator __new_start = _M_allocate(__len);
            iterator __new_finish = __new_start;
            {
                __new_finish = uninitialized_copy(_M_start, __position, __new_start);
                __new_finish = uninitialized_fill_n(__new_finish, __n, __x);
                __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
            }
            destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __new_start;
            _M_finish = __new_finish;
            _M_end_of_storage = __new_start + __len;
        }
    }
}

MYSTL_NAMESPACE_END

#endif