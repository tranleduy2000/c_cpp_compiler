/* A collection of useful convex polyhedra algorithms: inline functions.
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

#ifndef PPL_algorithms_hh
#define PPL_algorithms_hh 1

#include "NNC_Polyhedron.defs.hh"
#include "Pointset_Powerset.defs.hh"
#include <utility>
#include "assert.hh"

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  If the poly-hull of \p p and \p q is exact it is assigned
  to \p p and <CODE>true</CODE> is returned,
  otherwise <CODE>false</CODE> is returned.

  \relates Polyhedron
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename PH>
bool
poly_hull_assign_if_exact(PH& p, const PH& q);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Polyhedron */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename PH>
bool
poly_hull_assign_if_exact(PH& p, const PH& q) {
  PH poly_hull = p;
  NNC_Polyhedron nnc_p(p);
  poly_hull.poly_hull_assign(q);
  std::pair<PH, Pointset_Powerset<NNC_Polyhedron> >
    partition = linear_partition(q, poly_hull);
  const Pointset_Powerset<NNC_Polyhedron>& s = partition.second;
  typedef Pointset_Powerset<NNC_Polyhedron>::const_iterator iter;
  for (iter i = s.begin(), s_end = s.end(); i != s_end; ++i)
    // The polyhedral hull is exact if and only if all the elements
    // of the partition of the polyhedral hull of `p' and `q' with
    // respect to `q' are included in `p'
    if (!nnc_p.contains(i->pointset()))
      return false;
  p = poly_hull;
  return true;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_algorithms_hh)
