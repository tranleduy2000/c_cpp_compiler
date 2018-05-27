/* Interval_Info class implementation: inline functions.
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

#ifndef PPL_Interval_Info_inlines_hh
#define PPL_Interval_Info_inlines_hh 1

#include <iomanip>

namespace Parma_Polyhedra_Library {

template <typename Policy>
inline void
Interval_Info_Null<Policy>::m_swap(Interval_Info_Null<Policy>&) {
}

template <typename Policy>
inline void
Interval_Info_Null<Policy>::ascii_dump(std::ostream&) const {
}

template <typename Policy>
inline bool
Interval_Info_Null<Policy>::ascii_load(std::istream&) {
  return true;
}

template <typename Policy>
inline void
Interval_Info_Null_Open<Policy>::ascii_dump(std::ostream& s) const {
  s << (open ? "open" : "closed");
}

template <typename Policy>
inline bool
Interval_Info_Null_Open<Policy>::ascii_load(std::istream& s) {
  std::string str;
  if (!(s >> str))
    return false;
  if (str == "open") {
    open = true;
    return true;
  }
  if (str == "closed") {
    open = false;
    return true;
  }
  return false;
}

template <typename T, typename Policy>
inline void
Interval_Info_Bitset<T, Policy>::m_swap(Interval_Info_Bitset<T, Policy>& y) {
  using std::swap;
  swap(bitset, y.bitset);
}

template <typename T, typename Policy>
inline void
Interval_Info_Bitset<T, Policy>::ascii_dump(std::ostream& s) const {
  const std::ios::fmtflags old_flags = s.setf(std::ios::hex,
                                              std::ios::basefield);
  s << bitset;
  s.flags(old_flags);
}

template <typename T, typename Policy>
inline bool
Interval_Info_Bitset<T, Policy>::ascii_load(std::istream& s) {
  const std::ios::fmtflags old_flags = s.setf(std::ios::hex,
                                              std::ios::basefield);
  s >> bitset;
  s.flags(old_flags);
  return !s.fail();
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Interval_Info_Null */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Policy>
inline void
swap(Interval_Info_Null<Policy>& x, Interval_Info_Null<Policy>& y) {
  x.m_swap(y);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Interval_Info_Bitset */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T, typename Policy>
inline void
swap(Interval_Info_Bitset<T, Policy>& x, Interval_Info_Bitset<T, Policy>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Interval_Info_inlines_hh)
