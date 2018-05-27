/* Test Pointset_Powerset<PH>::affine_preimage(),
        Pointset_Powerset<PH>::generalized_affine_preimage(),
        Pointset_Powerset<PH>::bounded_affine_preimage().
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

// Powerset of C polyhedra: affine_preimage().
bool
test01() {
  Variable x(0);
  Variable y(1);
  Pointset_Powerset<C_Polyhedron> c_ps(2, EMPTY);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  C_Polyhedron ph(2);
  ph.add_constraints(cs);
  c_ps.add_disjunct(ph);

  Constraint_System cs1;
  cs1.insert(y >= 3);
  cs1.insert(y <= 5);
  C_Polyhedron ph1(2);
  ph1.add_constraints(cs1);
  c_ps.add_disjunct(ph1);

  c_ps.affine_preimage(x, x + y);

  ph.affine_preimage(x, x + y);
  ph1.affine_preimage(x, x + y);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = c_ps.begin();
  C_Polyhedron phi = i->pointset();
  i++;
  C_Polyhedron phi1 = i->pointset();

  bool ok = phi.OK() && phi == ph;

  print_constraints(ph, "*** ph ***");
  print_constraints(phi, "*** phi ***");

  bool ok1 = phi1.OK() && phi1 == ph1;

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(phi1, "*** phi1 ***");

  return ok && ok1;
}

// Powerset of polyhedra: generalized_affine_preimage(
//                          const Linear_Expression&,
//                          Relation_Symbol relsym,
//                          const Linear_Expression&).
bool
test02() {
  Variable x(0);
  Variable y(1);
  Pointset_Powerset<C_Polyhedron> c_ps(2, EMPTY);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  C_Polyhedron ph(2);
  ph.add_constraints(cs);
  c_ps.add_disjunct(ph);

  Constraint_System cs1;
  cs1.insert(y >= 3);
  cs1.insert(y <= 5);
  C_Polyhedron ph1(2);
  ph1.add_constraints(cs1);
  c_ps.add_disjunct(ph1);

  c_ps.generalized_affine_preimage(2*y, LESS_OR_EQUAL, 4*y);

  ph.generalized_affine_preimage(2*y, LESS_OR_EQUAL, 4*y);
  ph1.generalized_affine_preimage(2*y, LESS_OR_EQUAL, 4*y);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = c_ps.begin();
  C_Polyhedron phi = i->pointset();
  i++;
  C_Polyhedron phi1 = i->pointset();

  bool ok = phi.OK() && phi == ph;

  print_constraints(ph, "*** ph ***");
  print_constraints(phi, "*** phi ***");

  bool ok1 = phi1.OK() && phi1 == ph1;

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(phi1, "*** phi1 ***");

  return ok && ok1;
}

// Powerset of polyhedra: bounded_affine_preimage(
//                          Variable,
//                          Relation_Symbol relsym,
//                          const Linear_Expression&,
//                          Coefficient_traits::const_reference).
bool
test03() {
  Variable x(0);
  Variable y(1);
  Pointset_Powerset<C_Polyhedron> c_ps(2, EMPTY);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  C_Polyhedron ph(2);
  ph.add_constraints(cs);
  c_ps.add_disjunct(ph);

  Constraint_System cs1;
  cs1.insert(y >= 3);
  cs1.insert(y <= 5);
  C_Polyhedron ph1(2);
  ph1.add_constraints(cs1);
  c_ps.add_disjunct(ph1);

  c_ps.bounded_affine_preimage(y, x, 2*y, 5);

  ph.bounded_affine_preimage(y, x, 2*y, 5);
  ph1.bounded_affine_preimage(y, x, 2*y, 5);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = c_ps.begin();
  C_Polyhedron phi = i->pointset();
  i++;
  C_Polyhedron phi1 = i->pointset();

  bool ok = phi.OK() && phi == ph;

  print_constraints(ph, "*** ph ***");
  print_constraints(phi, "*** phi ***");

  bool ok1 = phi1.OK() && phi1 == ph1;

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(phi1, "*** phi1 ***");

  return ok && ok1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
