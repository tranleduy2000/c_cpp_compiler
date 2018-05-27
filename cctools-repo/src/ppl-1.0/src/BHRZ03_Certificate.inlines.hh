/* BHRZ03_Certificate class implementation: inline functions.
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

#ifndef PPL_BHRZ03_Certificate_inlines_hh
#define PPL_BHRZ03_Certificate_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline
BHRZ03_Certificate::BHRZ03_Certificate()
  : affine_dim(0), lin_space_dim(0), num_constraints(0), num_points(1),
    num_rays_null_coord() {
  // This is the certificate for a zero-dim universe polyhedron.
  PPL_ASSERT(OK());
}

inline
BHRZ03_Certificate::BHRZ03_Certificate(const BHRZ03_Certificate& y)
  : affine_dim(y.affine_dim), lin_space_dim(y.lin_space_dim),
    num_constraints(y.num_constraints), num_points(y.num_points),
    num_rays_null_coord(y.num_rays_null_coord) {
}

inline
BHRZ03_Certificate::~BHRZ03_Certificate() {
}

inline bool
BHRZ03_Certificate::is_stabilizing(const Polyhedron& ph) const {
  return compare(ph) == 1;
}

inline bool
BHRZ03_Certificate::Compare::operator()(const BHRZ03_Certificate& x,
					const BHRZ03_Certificate& y) const {
  // For an efficient evaluation of the multiset ordering based
  // on this LGO relation, we want larger elements to come first.
  return x.compare(y) == 1;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_BHRZ03_Certificate_inlines_hh)
