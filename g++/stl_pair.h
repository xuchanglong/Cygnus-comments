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

/**
 * 这是一个内部头文件，供其他 STL 头文件使用，
 * 正常情况下，程序员不应该使用该文件。
*/
/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_PAIR_H
#define __SGI_STL_INTERNAL_PAIR_H

__STL_BEGIN_NAMESPACE

/**
 * 仅仅是包含了两个变量的结构体，作为 map 的键-值对。
 * 结构体中默认情况下变量和函数均是 public 。
*/
template <class T1, class T2>
struct pair
{
  typedef T1 first_type;
  typedef T2 second_type;

  T1 first;
  T2 second;
  /**
   * T1 和 T2 若是对象，则赋值临时对象。
   * 若是基本数据类型，则赋值为 0 。
  */
  pair() : first(T1()), second(T2()) {}
  pair(const T1 &a, const T2 &b) : first(a), second(b) {}

#ifdef __STL_MEMBER_TEMPLATES
  template <class U1, class U2>
  pair(const pair<U1, U2> &p) : first(p.first), second(p.second)
  {
  }
#endif
};

template <class T1, class T2>
inline bool operator==(const pair<T1, T2> &x, const pair<T1, T2> &y)
{
  return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
inline bool operator<(const pair<T1, T2> &x, const pair<T1, T2> &y)
{
  return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1 &x, const T2 &y)
{
  return pair<T1, T2>(x, y);
}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_PAIR_H */

// Local Variables:
// mode:C++
// End:
