/* C_Polyhedron class implementation (non-inline functions).
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

#include "ppl-config.h"
#include "C_Polyhedron.defs.hh"
#include "NNC_Polyhedron.defs.hh"
#include "Grid.defs.hh"
#include "algorithms.hh"

namespace PPL = Parma_Polyhedra_Library;

PPL::C_Polyhedron::C_Polyhedron(const NNC_Polyhedron& y, Complexity_Class)
  : Polyhedron(NECESSARILY_CLOSED, y.space_dimension(), UNIVERSE) {
  const Constraint_System& cs = y.constraints();
  for (Constraint_System::const_iterator i = cs.begin(),
	 cs_end = cs.end(); i != cs_end; ++i) {
    const Constraint& c = *i;
    add_constraint(c.is_strict_inequality() ? (Linear_Expression(c) >= 0) : c);
  }
  PPL_ASSERT_HEAVY(OK());
}

PPL::C_Polyhedron::C_Polyhedron(const Congruence_System& cgs)
  : Polyhedron(NECESSARILY_CLOSED,
               check_space_dimension_overflow(cgs.space_dimension(),
                                              NECESSARILY_CLOSED,
                                              "C_Polyhedron(cgs)",
                                              "the space dimension of cgs "
                                              "exceeds the maximum allowed "
                                              "space dimension"),
	       UNIVERSE) {
  add_congruences(cgs);
}

PPL::C_Polyhedron::C_Polyhedron(Congruence_System& cgs, Recycle_Input)
  : Polyhedron(NECESSARILY_CLOSED,
               check_space_dimension_overflow(cgs.space_dimension(),
                                              NECESSARILY_CLOSED,
                                              "C_Polyhedron(cgs, recycle)",
                                              "the space dimension of cgs "
                                              "exceeds the maximum allowed "
                                              "space dimension"),
	       UNIVERSE) {
  add_congruences(cgs);
}

PPL::C_Polyhedron::C_Polyhedron(const Grid& grid, Complexity_Class)
  : Polyhedron(NECESSARILY_CLOSED,
               check_space_dimension_overflow(grid.space_dimension(),
                                              NECESSARILY_CLOSED,
                                              "C_Polyhedron(grid)",
                                              "the space dimension of grid "
                                              "exceeds the maximum allowed "
                                              "space dimension"),
	       UNIVERSE) {
  add_constraints(grid.constraints());
}

bool
PPL::C_Polyhedron::poly_hull_assign_if_exact(const C_Polyhedron& y) {
  // Dimension-compatibility check.
  if (space_dimension() != y.space_dimension())
    throw_dimension_incompatible("poly_hull_assign_if_exact(y)", "y", y);
#define USE_BHZ09 0
#define USE_BFT00 1
#if USE_BHZ09 // [BagnaraHZ09]
  return BHZ09_poly_hull_assign_if_exact(y);
#elif USE_BFT00 // [BemporadFT00TR].
  return BFT00_poly_hull_assign_if_exact(y);
#else // Old implementation.
  return PPL::poly_hull_assign_if_exact(*this, y);
#endif
#undef USE_BHZ09
#undef USE_BFT00
}
