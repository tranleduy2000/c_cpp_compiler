/* Test Pointset_Powerset<PH>::add_constraint().
        Pointset_Powerset<PH>::add_constraints().
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

// Powerset of C polyhedra: add_constraint().
bool
test01() {
  Variable x(0);
  Constraint c = (x >= 0);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);
  ps.add_disjunct(C_Polyhedron(1));
  ps.add_constraint(c);
  Constraint c1 = (x >= 1);
  ps.add_constraint(c1);
  bool ok = !ps.is_empty();

  return ok && ps.OK();
}

// Powerset of C polyhedra: add_constraints().
bool
test02() {
  Variable x(0);
  Constraint_System cs;
  cs.insert(x >= 3);
  cs.insert(x <= 4);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);
  ps.add_disjunct(C_Polyhedron(1));
  ps.add_constraints(cs);
  cs.insert(x <= 3);
  ps.add_constraints(cs);
  bool ok = !ps.is_empty();
  cs.insert(x <= 2);
  ps.add_constraints(cs);
  ok &= ps.is_empty();

  return ok && ps.OK();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
