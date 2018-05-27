/* Result supporting functions implementation: inline functions.
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

#ifndef PPL_Result_inlines_hh
#define PPL_Result_inlines_hh 1

#include "assert.hh"

namespace Parma_Polyhedra_Library {

/*! \ingroup PPL_CXX_interface */
inline Result
operator&(Result x, Result y) {
  unsigned res = static_cast<unsigned>(x) & static_cast<unsigned>(y);
  return static_cast<Result>(res);
}

/*! \ingroup PPL_CXX_interface */
inline Result
operator|(Result x, Result y) {
  unsigned res = static_cast<unsigned>(x) | static_cast<unsigned>(y);
  return static_cast<Result>(res);
}

/*! \ingroup PPL_CXX_interface */
inline Result
operator-(Result x, Result y) {
  Result y_neg = static_cast<Result>(~static_cast<unsigned>(y));
  return x & y_neg;
}

/*! \ingroup PPL_CXX_interface */
inline Result_Class
result_class(Result r) {
  Result rc = r & static_cast<Result>(VC_MASK);
  return static_cast<Result_Class>(rc);
}

/*! \ingroup PPL_CXX_interface */
inline Result_Relation
result_relation(Result r) {
  Result rc = r & static_cast<Result>(VR_MASK);
  return static_cast<Result_Relation>(rc);
}

/*! \ingroup PPL_CXX_interface */
inline Result
result_relation_class(Result r) {
  return r & (static_cast<Result>(VR_MASK) | static_cast<Result>(VC_MASK));
}

inline int
result_overflow(Result r) {
  switch (result_class(r)) {
  case VC_NORMAL:
    switch (r) {
    case V_LT_INF:
      return -1;
    case V_GT_SUP:
      return 1;
    default:
      break;
    }
    break;
  case VC_MINUS_INFINITY:
    return -1;
  case VC_PLUS_INFINITY:
    return 1;
  default:
    break;
  }
  return 0;
}

inline bool
result_representable(Result r) {
  return (r & V_UNREPRESENTABLE) != V_UNREPRESENTABLE;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Result_inlines_hh)
