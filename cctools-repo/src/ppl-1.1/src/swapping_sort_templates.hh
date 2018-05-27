/* Sorting objects for which copies cost more than swaps.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://bugseng.com/products/ppl/ . */

#ifndef PPL_swapping_sort_templates_hh
#define PPL_swapping_sort_templates_hh 1

#include <vector>
#include <algorithm>

namespace Parma_Polyhedra_Library {

namespace Implementation {

template <typename RA_Container, typename Compare>
struct Indirect_Sort_Compare {
  typedef typename RA_Container::size_type size_type;

  Indirect_Sort_Compare(const RA_Container& cont,
                        size_type base = 0,
                        Compare comp = Compare())
    : container(cont), base_index(base), compare(comp) {
  }

  bool operator()(size_type i, size_type j) const {
    return compare(container[base_index + i], container[base_index + j]);
  }

  const RA_Container& container;
  const size_type base_index;
  const Compare compare;
}; // struct Indirect_Sort_Compare

template <typename RA_Container>
struct Indirect_Unique_Compare {
  typedef typename RA_Container::size_type size_type;

  Indirect_Unique_Compare(const RA_Container& cont, size_type base = 0)
    : container(cont), base_index(base) {
  }

  bool operator()(size_type i, size_type j) const {
    return container[base_index + i] == container[base_index + j];
  }

  const RA_Container& container;
  const size_type base_index;
}; // struct Indirect_Unique_Compare

template <typename RA_Container>
struct Indirect_Swapper {
  typedef typename RA_Container::size_type size_type;

  Indirect_Swapper(RA_Container& cont, size_type base = 0)
    : container(cont), base_index(base) {
  }

  void operator()(size_type i, size_type j) const {
    using std::swap;
    swap(container[base_index + i], container[base_index + j]);
  }

  RA_Container& container;
  const size_type base_index;
}; // struct Indirect_Swapper

template <typename RA_Container1, typename RA_Container2>
struct Indirect_Swapper2 {
  typedef typename RA_Container1::size_type size_type;

  Indirect_Swapper2(RA_Container1& cont1, RA_Container2& cont2)
    : container1(cont1), container2(cont2) {
  }

  void operator()(size_type i, size_type j) const {
    using std::swap;
    swap(container1[i], container1[j]);
    swap(container2[i], container2[j]);
  }

  RA_Container1& container1;
  RA_Container2& container2;
}; // struct Indirect_Swapper2

template <typename Sort_Comparer, typename Unique_Comparer, typename Swapper>
typename Sort_Comparer::size_type
indirect_sort_and_unique(typename Sort_Comparer::size_type num_elems,
                         Sort_Comparer sort_cmp,
                         Unique_Comparer unique_cmp,
                         Swapper indirect_swap) {
  typedef typename Sort_Comparer::size_type index_type;
  // `iv' is a vector of indices for the portion of rows to be sorted.
  PPL_ASSERT(num_elems >= 2);
  std::vector<index_type> iv;
  iv.reserve(num_elems);
  for (index_type i = 0, i_end = num_elems; i != i_end; ++i)
    iv.push_back(i);

  typedef typename std::vector<index_type>::iterator Iter;
  const Iter iv_begin = iv.begin();
  Iter iv_end = iv.end();

  // Sort `iv' by comparing the rows indexed by its elements.
  std::sort(iv_begin, iv_end, sort_cmp);

  // Swap the indexed rows according to `iv':
  // for each index `i', the element that should be placed in
  // position dst = i is the one placed in position src = iv[i].
  for (index_type i = num_elems; i-- > 0; ) {
    if (i != iv[i]) {
      index_type dst = i;
      index_type src = iv[i];
      do {
        indirect_swap(src, dst);
        iv[dst] = dst;
        dst = src;
        src = iv[dst];
      } while (i != src);
      iv[dst] = dst;
    }
  }

  // Restore `iv' indices to 0 .. num_elems-1 for the call to unique.
  for (index_type i = num_elems; i-- > 0; )
    iv[i] = i;

  // Unique `iv' by comparing the rows indexed by its elements.
  iv_end = std::unique(iv_begin, iv_end, unique_cmp);

  const index_type num_sorted = static_cast<index_type>(iv_end - iv_begin);
  const index_type num_duplicates = num_elems - num_sorted;
  if (num_duplicates == 0)
    return 0;

  // There were duplicates: swap the rows according to `iv'.
  index_type dst = 0;
  while (dst < num_sorted && dst == iv[dst])
    ++dst;
  if (dst == num_sorted)
    return num_duplicates;
  do {
    const index_type src = iv[dst];
    indirect_swap(src, dst);
    ++dst;
  }
  while (dst < num_sorted);
  return num_duplicates;
}

template <typename Iter>
Iter
swapping_unique(Iter first, Iter last) {
  return swapping_unique(first, last, std::iter_swap<Iter, Iter>);
}

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_swapping_sort_templates_hh)
