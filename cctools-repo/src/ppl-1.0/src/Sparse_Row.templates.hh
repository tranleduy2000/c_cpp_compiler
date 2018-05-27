/* Sparse_Row class implementation: non-inline template functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Sparse_Row_templates_hh
#define PPL_Sparse_Row_templates_hh 1

namespace Parma_Polyhedra_Library {


template <typename Func1, typename Func2>
void
Sparse_Row::combine_needs_first(const Sparse_Row& y,
                                const Func1& f, const Func2& g) {
  if (this == &y) {
    for (iterator i = begin(), i_end = end(); i != i_end; ++i)
      g(*i, *i);
  } else {
    iterator i = begin();
    // This is a const reference to an internal iterator, that is kept valid.
    // If we just stored a copy, that would be invalidated by the calls to
    // reset().
    const iterator& i_end = end();
    const_iterator j = y.begin();
    const_iterator j_end = y.end();
    while (i != i_end && j != j_end)
      if (i.index() == j.index()) {
        g(*i, *j);
        if (*i == 0)
          i = reset(i);
        else
          ++i;
        ++j;
      } else
        if (i.index() < j.index()) {
          f(*i);
          if (*i == 0)
            i = reset(i);
          else
            ++i;
        } else
          j = y.lower_bound(j, i.index());
    while (i != i_end) {
      f(*i);
      if (*i == 0)
        i = reset(i);
      else
        ++i;
    }
  }
}

template <typename Func1, typename Func2>
void
Sparse_Row::combine_needs_second(const Sparse_Row& y,
                                 const Func1& g,
                                 const Func2& /* h */) {
  iterator i = begin();
  for (const_iterator j = y.begin(), j_end = y.end(); j != j_end; ++j) {
    i = insert(i, j.index());
    g(*i, *j);
    if (*i == 0)
      i = reset(i);
  }
}

template <typename Func1, typename Func2, typename Func3>
void
Sparse_Row::combine(const Sparse_Row& y, const Func1& f,
                    const Func2& g, const Func3& h) {
  if (this == &y) {
    for (iterator i = begin(), i_end = end(); i != i_end; ++i)
      g(*i, *i);
  } else {
    iterator i = begin();
    // This is a const reference to an internal iterator, that is kept valid.
    // If we just stored a copy, that would be invalidated by the calls to
    // reset() and insert().
    const iterator& i_end = end();
    const_iterator j = y.begin();
    const_iterator j_end = y.end();
    while (i != i_end && j != j_end) {
      if (i.index() == j.index()) {
        g(*i, *j);
        if (*i == 0)
          i = reset(i);
        else
          ++i;
        ++j;
      } else
        if (i.index() < j.index()) {
          f(*i);
          if (*i == 0)
            i = reset(i);
          else
            ++i;
        } else {
          PPL_ASSERT(i.index() > j.index());
          i = insert(i, j.index());
          h(*i, *j);
          if (*i == 0)
            i = reset(i);
          else
            ++i;
          ++j;
        }
    }
    PPL_ASSERT(i == i_end || j == j_end);
    while (i != i_end) {
      f(*i);
      if (*i == 0)
        i = reset(i);
      else
        ++i;
    }
    while (j != j_end) {
      i = insert(i, j.index());
      h(*i, *j);
      if (*i == 0)
        i = reset(i);
      ++j;
    }
  }
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Sparse_Row_templates_hh)
