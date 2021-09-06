#ifndef _MYSTL_QUEUE_H
#define _MYSTL_QUEUE_H

MYSTL_NAMESPACE_BEGIN

template <class _Tp, class _Sequence = deque<_Tp>>
class queue
{
public:
    typedef typename _Sequence::value_type      value_type;
    typedef typename _Sequence::size_type       size_type;
    typedef          _Sequence                  container_type;

    typedef typename _Sequence::reference       reference;
    typedef typename _Sequence::const_reference const_reference;

    _Sequence c;

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }
    void push(const value_type& __x) { c.push_back(__x); }
    void pop() { c.pop_front(); }
};

MYSTL_NAMESPACE_END

#endif