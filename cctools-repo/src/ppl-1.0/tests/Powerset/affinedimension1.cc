/* Test Pointset_Powerset<PH>::affine_dimension().
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

// Powerset of C polyhedra: affine_dimension().
bool
test01() {
  Variable x(0);
  Variable y(1);
  Variable z(1);
  Pointset_Powerset<C_Polyhedron> c_ps(3, EMPTY);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  cs.insert(z <= 2);
  cs.insert(z >= 2);
  C_Polyhedron ph(3);
  ph.add_constraints(cs);
  c_ps.add_disjunct(ph);

  Constraint_System cs1;
  cs1.insert(y >= 3);
  cs1.insert(y <= 5);
  cs1.insert(x == 6);
  C_Polyhedron ph1(3);
  ph1.add_constraints(cs1);
  c_ps.add_disjunct(ph1);

  dimension_type d = c_ps.affine_dimension();

  bool ok = (d == 3);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = c_ps.begin();
  C_Polyhedron phi = i->pointset();
  i++;
  C_Polyhedron phi1 = i->pointset();

  print_constraints(phi, "*** phi ***");
  print_constraints(phi1, "*** phi1 ***");

  c_ps.add_constraint(z == 2);

  dimension_type d1 = c_ps.affine_dimension();

  bool ok1 = (d1 == 2);

  Pointset_Powerset<C_Polyhedron>::const_iterator j = c_ps.begin();
  C_Polyhedron phj = j->pointset();
  j++;
  C_Polyhedron phj1 = j->pointset();

  print_constraints(phj, "*** phj ***");
  print_constraints(phj1, "*** phj1 ***");

  return ok && ok1;
}

// Powerset of NNC polyhedra: affine_dimension().
bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(1);
  Pointset_Powerset<NNC_Polyhedron> c_ps(3, EMPTY);
  Constraint_System cs;
  cs.insert(x > 0);
  cs.insert(x <= 2);
  cs.insert(z <= 2);
  cs.insert(z >= 2);
  NNC_Polyhedron ph(3);
  ph.add_constraints(cs);
  c_ps.add_disjunct(ph);

  Constraint_System cs1;
  cs1.insert(y >= 3);
  cs1.insert(y <= 5);
  cs1.insert(x == 6);
  NNC_Polyhedron ph1(3);
  ph1.add_constraints(cs1);
  c_ps.add_disjunct(ph1);

  dimension_type d = c_ps.affine_dimension();

  bool ok = (d == 3);

  Pointset_Powerset<NNC_Polyhedron>::const_iterator i = c_ps.begin();
  NNC_Polyhedron phi = i->pointset();
  i++;
  NNC_Polyhedron phi1 = i->pointset();

  print_constraints(phi, "*** phi ***");
  print_constraints(phi1, "*** phi1 ***");

  c_ps.add_constraint(z == 2);

  dimension_type d1 = c_ps.affine_dimension();

  bool ok1 = (d1 == 2);

  Pointset_Powerset<NNC_Polyhedron>::const_iterator j = c_ps.begin();
  NNC_Polyhedron phj = j->pointset();
  j++;
  NNC_Polyhedron phj1 = j->pointset();

  print_constraints(phj, "*** phj ***");
  print_constraints(phj1, "*** phj1 ***");

  return ok && ok1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
