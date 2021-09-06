#ifndef _MYSTL_TREE_H
#define _MYSTL_TREE_H

MYSTL_NAMESPACE_BEGIN

typedef bool _Rb_tree_Color_type;
const _Rb_tree_Color_type _S_rb_tree_red = false;   //0
const _Rb_tree_Color_type _S_rb_tree_black = true;  //1

struct _Rb_tree_node_base
{
    typedef _Rb_tree_Color_type _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;

    _Color_type _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;

    static _Base_ptr _S_minimum(_Base_ptr __x)
    {
        while (__x->_M_left != 0) x = __x->_M_left;
        return __x;
    }
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base
{
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Value _M_value_field;
};

struct _Rb_tree_base_iterator
{
    _Base_ptr _M_node;
};

template <class _Value, class _Ref, class _Ptr>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator
{
    
};

MYSTL_NAMESPACE_END

#endif