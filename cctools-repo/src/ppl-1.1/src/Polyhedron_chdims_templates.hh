/* Polyhedron class implementation (non-inline template operators that
   may change the dimension of the vector space).
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

#ifndef PPL_Polyhedron_chdims_templates_hh
#define PPL_Polyhedron_chdims_templates_hh 1

namespace Parma_Polyhedra_Library {

template <typename Linear_System1, typename Linear_System2>
void
Polyhedron::add_space_dimensions(Linear_System1& sys1,
                                 Linear_System2& sys2,
                                 Bit_Matrix& sat1,
                                 Bit_Matrix& sat2,
                                 dimension_type add_dim) {

  typedef typename Linear_System2::row_type sys2_row_type;

  PPL_ASSERT(sys1.topology() == sys2.topology());
  PPL_ASSERT(sys1.space_dimension() == sys2.space_dimension());
  PPL_ASSERT(add_dim != 0);

  sys1.set_space_dimension(sys1.space_dimension() + add_dim);
  sys2.add_universe_rows_and_space_dimensions(add_dim);

  // The resulting saturation matrix will be as follows:
  // from row    0    to      add_dim-1       : only zeroes
  //          add_dim     add_dim+num_rows-1  : old saturation matrix

  // In fact all the old generators saturate all the new constraints
  // because the polyhedron has not been embedded in the new space.
  sat1.resize(sat1.num_rows() + add_dim, sat1.num_columns());
  // The old matrix is moved to the end of the new matrix.
  for (dimension_type i = sat1.num_rows() - add_dim; i-- > 0; )
    swap(sat1[i], sat1[i+add_dim]);
  // Computes the "sat_c", too.
  sat2.transpose_assign(sat1);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Polyhedron_chdims_templates_hh)
