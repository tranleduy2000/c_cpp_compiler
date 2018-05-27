/* Grid_Certificate class implementation
   (non-inline member functions).
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
#include "Grid_Certificate.defs.hh"
#include "Grid.defs.hh"
#include "assert.hh"
#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

PPL::Grid_Certificate::Grid_Certificate(const Grid& gr)
  : num_equalities(0), num_proper_congruences(0) {

  // As in Polyhedron assume that `gr' contains at least one point.
  PPL_ASSERT(!gr.marked_empty());
  if (gr.space_dimension() == 0)
    return;
  // One of the systems must be in minimal form.
  if (gr.congruences_are_up_to_date()) {
    if (gr.congruences_are_minimized()) {
      num_proper_congruences = gr.con_sys.num_proper_congruences();
      num_equalities = gr.con_sys.num_equalities();
    }
    else
      if (gr.generators_are_up_to_date() && gr.generators_are_minimized()) {
	// Calculate number of congruences from generators.
 	num_proper_congruences
	  = gr.gen_sys.num_parameters() + 1 /* Integrality cg. */;
	num_equalities = gr.space_dimension() + 1 - gr.gen_sys.num_rows();
      }
      else {
	// Minimize `gr' congruence system.  As in Polyhedron assume
	// that `gr' contains at least one point.
        Grid& mgr = const_cast<Grid&>(gr);
	bool empty = Grid::simplify(mgr.con_sys, mgr.dim_kinds);
	// Avoid possible compiler warning.
	PPL_USED(empty);
	PPL_ASSERT(!empty);
	mgr.set_congruences_minimized();

	num_proper_congruences = mgr.con_sys.num_proper_congruences();
	num_equalities = mgr.con_sys.num_equalities();
      }
  }
  else {
    if (!gr.generators_are_minimized()) {
      // Minimize `gr' generator system.  As in Polyhedron assume that
      // `gr' contains at least one point.
      Grid& mgr = const_cast<Grid&>(gr);
      Grid::simplify(mgr.gen_sys, mgr.dim_kinds);
      // If gen_sys contained rows before being reduced, it should
      // contain at least a single point afterward.
      PPL_ASSERT(!mgr.gen_sys.empty());
      mgr.set_generators_minimized();
    }
    // Calculate number of congruences from generators.
    num_proper_congruences
      = gr.gen_sys.num_parameters() + 1 /* Integrality cg. */;
    num_equalities
      = gr.space_dimension() + 1 - gr.gen_sys.num_rows();
  }
}

int
PPL::Grid_Certificate::compare(const Grid_Certificate& y) const {
  PPL_ASSERT(OK() && y.OK());
  if (num_equalities == y.num_equalities) {
    if (num_proper_congruences == y.num_proper_congruences)
      return 0;
    else
      return (num_proper_congruences > y.num_proper_congruences) ? 1 : -1;
  }
  return (num_equalities > y.num_equalities) ? 1 : -1;
}

int
PPL::Grid_Certificate::compare(const Grid& gr) const {
  Grid_Certificate gc(gr);
  return compare(gc);
}

bool
PPL::Grid_Certificate::OK() const {
  // There is nothing to test.
  return true;
}
