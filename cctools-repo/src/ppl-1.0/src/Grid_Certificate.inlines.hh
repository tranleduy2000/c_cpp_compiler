/* Grid_Certificate class implementation: inline functions.
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

#ifndef PPL_Grid_Certificate_inlines_hh
#define PPL_Grid_Certificate_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline
Grid_Certificate::Grid_Certificate()
  : num_equalities(0), num_proper_congruences(0) {
  // This is the certificate for a zero-dim universe grid.
  PPL_ASSERT(OK());
}

inline
Grid_Certificate::Grid_Certificate(const Grid_Certificate& y)
  : num_equalities(y.num_equalities),
    num_proper_congruences(y.num_proper_congruences) {
}

inline
Grid_Certificate::~Grid_Certificate() {
}

inline bool
Grid_Certificate::is_stabilizing(const Grid& gr) const {
  return compare(gr) == 1;
}

inline bool
Grid_Certificate::Compare::operator()(const Grid_Certificate& x,
				      const Grid_Certificate& y) const {
  // For an efficient evaluation of the multiset ordering based
  // on this LGO relation, we want larger elements to come first.
  return x.compare(y) == 1;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Grid_Certificate_inlines_hh)
