/*
 * This file is mostly a copy of the std::array implementation developed by the
 * LLVM Team, University of Illinois at Urbana-Champaign, http://llvm.org.
 * See CREDIT.TXT file available at http://libcxx.llvm.org/
 *
 * Copyright (c) 2009-2013 by the contributors listed in CREDITS.TXT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *					Anthony Mallet on Tue Jun 18 2013
 */

#ifndef H_GENOM3_CXX_IDLSEQUENCE
#define H_GENOM3_CXX_IDLSEQUENCE

#include <cstddef>
#include <stdexcept>

namespace genom {
  template <typename T, size_t N>
  struct bounded_sequence {
    // types:
    typedef T                                     value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef value_type*                           iterator;
    typedef const value_type*                     const_iterator;
    typedef value_type*                           pointer;
    typedef const value_type*                     const_pointer;
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    value_type e[N];
    size_type n;

    // No explicit construct/copy/destroy for aggregate type

    void fill(const value_type &u) { std::fill_n(e, N, u); }

    // iterators:
    iterator begin() { return iterator(e); }
    iterator end() { return iterator(e + n); }
    const_iterator begin() const { return const_iterator(e); }
    const_iterator end() const { return const_iterator(e + n); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const {
      return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const {
      return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); }

    // capacity:
    size_type size() const { return n; }
    void resize(size_type l, value_type u = value_type()) {
      if (l > N) throw std::out_of_range("genom::bounded_sequence::resize");
      if (l > n) std::fill_n(e + n, l-n, u);
    }
    size_type max_size() const { return N; }
    bool empty() const { return n == 0; }

    // element access:
    reference operator[](size_type i) { return e[i]; }
    const_reference operator[](size_type i) const { return e[i]; }
    reference at(size_type i) {
      if (i >= n) throw std::out_of_range("genom::bounded_sequence::at");
      return e[i];
    }
    const_reference at(size_type i) const {
      if (i >= n) throw std::out_of_range("genom::bounded_sequence::at");
      return e[i];
    }

    reference front() { return e[0]; }
    const_reference front() const { return e[0]; }
    reference back() {return e[n > 0 ? n-1 : 0]; }
    const_reference back() const { return e[n > 0 ? n-1 : 0]; }
    value_type *data() { return e; }
    const value_type *data() const { return e; }

    // modifiers
    void swap(bounded_sequence &a) {
      std::swap_ranges(e, e + std::min(n, a.n), a.e);
      for(size_type i = std::min(n, a.n); i < a.n; i++) e[i] = a[i];
      std::swap(n, a.n);
    }
    void clear() { n = 0; }
  };
}

template <typename T, size_t N> inline bool
operator==(const genom::bounded_sequence<T, N> &x,
                const genom::bounded_sequence<T, N> &y)
{
  if (x.n != y.n) return false;
  return std::equal(x.e, x.e + x.n, y.e);
}

template <typename T, size_t N> inline bool
operator!=(const genom::bounded_sequence<T, N> &x,
                const genom::bounded_sequence<T, N> &y)
{
  return !(x == y);
}

template <typename T, size_t N> inline bool
operator<(const genom::bounded_sequence<T, N> &x,
                const genom::bounded_sequence<T, N> &y)
{
  return std::lexicographical_compare(x.e, x.e + x.n, y.e, y.e + y.n);
}

template <typename T, size_t N> inline bool
operator>(const genom::bounded_sequence<T, N> &x,
                const genom::bounded_sequence<T, N> &y)
{
  return y < x;
}

template <typename T, size_t N> inline bool
operator<=(const genom::bounded_sequence<T, N> &x,
                const genom::bounded_sequence<T, N> &y)
{
    return !(y < x);
}

template <typename T, size_t N> inline bool
operator>=(const genom::bounded_sequence<T, N> &x,
                const genom::bounded_sequence<T, N> &y)
{
    return !(x < y);
}

namespace std {
  template <typename T, size_t N> inline void
  swap(const genom::bounded_sequence<T, N> &x,
       const genom::bounded_sequence<T, N> &y)
  {
    x.swap(y);
  }
}

#endif /* H_GENOM3_CXX_IDLSEQUENCE */
