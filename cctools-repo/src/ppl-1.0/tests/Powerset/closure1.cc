/* Test Pointset_Powerset<PH>::topological_closure_assign().
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

#include "ppl_test.hh"

namespace {

// Powerset of C polyhedra: topological_closure_assign().

bool
test01() {
  Variable x(0);
  Pointset_Powerset<NNC_Polyhedron> ps(1);
  Constraint_System cs;
  cs.clear();
  cs.insert(x > 5);
  cs.insert(x > 8);
  ps.add_constraints(cs);

  ps.topological_closure_assign();

  bool ok = ps.OK();

  Pointset_Powerset<NNC_Polyhedron> known_ps(1);
  cs.clear();
  cs.insert(x >= 5);
  cs.insert(x >= 8);
  known_ps.add_constraints(cs);

  ok = ok && ps.contains(known_ps);

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
