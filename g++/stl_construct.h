/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_CONSTRUCT_H
#define __SGI_STL_INTERNAL_CONSTRUCT_H

#include <new.h> // for placement new

__STL_BEGIN_NAMESPACE

// =================================== construct() ================================== //
template <class T1, class T2>
inline void construct(T1 *p, const T2 &value)
{
    new (p) T1(value); // placement new ; T1::T1(value)
}

// =================================== destroy() ================================== //
// 版本一：特化版
template <class T>
inline void destroy(T *pointer)
{
    pointer->~T();
}

// 版本二：泛化版
// === 一级 === //
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    // 通过 value_type 获取迭代器所指对象的型别。
    __destroy(first, last, value_type(first));
}

// === 二级 === //
// 接受两个迭代器，以 __type_trais<T> 判断是否有 trivial destructor 。
// trivial destructor，直译：微不足道的析构，也就是默认析构函数。
// 因为默认的析构函数是什么也不做的。
// 判断是否有 trivial destructor，也就是判断是否有重载的析构函数。

// 这里的萃取技术，实际上就是穷举法。即：若 T 为 C++ 自带的类型，则一律视为
// _true_type，否则一律视为 _false_type 。
template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T *)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor; 
    __destroy_aux(first, last, trivial_destructor());
}

// === 三级 === //
// 若元素的型别（value type）为 none trivial destructor，则逐一调用析构函数。
template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for (; first < last; ++first)
        destroy(&*first);
}
//	若元素的型别（value type）为 trivial destructor，则不做任何处理。
template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}

//	特化版
inline void destroy(char *, char *) {}
inline void destroy(wchar_t *, wchar_t *) {}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_CONSTRUCT_H */