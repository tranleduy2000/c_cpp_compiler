/* Test Pointset_Powerset<PH>::affine_image(),
        Pointset_Powerset<PH>::generalized_affine_image(),
        Pointset_Powerset<PH>::bounded_affine_image().
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

// Powerset of polyhedra: affine_image().
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

  c_ps.affine_image(x, x + y);

  ph.affine_image(x, x + y);
  ph1.affine_image(x, x + y);

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

// Powerset of boxes: affine_image() and intersection_assign().
bool
test02() {
  Variable x(0);
  Constraint_System cs;
  Pointset_Powerset<TBox> ps1(1, EMPTY);

  cs.clear();
  cs.insert(x > 0);
  cs.insert(x <= 1);
  ps1.add_disjunct(TBox(cs));

  cs.clear();
  cs.insert(x >= 0);
  cs.insert(x < 1);
  ps1.add_disjunct(TBox(cs));

  Pointset_Powerset<TBox> ps2(ps1);
  ps2.affine_image(x, 2*x+1, 2);

  print_constraints(ps1, "*** ps1 ***");
  print_constraints(ps2, "*** ps2 ***");

  ps1.intersection_assign(ps2);

  Pointset_Powerset<TBox> known_result(1, EMPTY);
  TBox box(1);
  box.add_constraint(2*x >= 1);
  box.add_constraint(2*x <= 2);
  known_result.add_disjunct(box);

  bool ok = (ps1 == known_result);

  print_constraints(ps1, "*** ps1.intersect_assign(ps2) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

// Powerset of Boxes: affine_image().
bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Pointset_Powerset<TBox> ps(3, EMPTY);
  for (int i = -10; i <= 9; ++i) {
    TBox pps_box(3, UNIVERSE);
    pps_box.add_constraint(i <= x);
    pps_box.add_constraint(x <= i+1);
    const TBox::interval_type& ix = pps_box.get_interval(x);
    TBox::interval_type iy = ix*ix;
    pps_box.set_interval(y, iy);
    ps.add_disjunct(pps_box);
  }

  print_constraints(ps, "*** ps ***");

  ps.affine_image(z, y+2*x+1, 2);

  print_constraints(ps, "*** ps ***");

  return ps.OK();
}

// Powerset of polyhedra: generalized_affine_image(
//                          const Linear_Expression&,
//                          Relation_Symbol relsym,
//                          const Linear_Expression&).
bool
test04() {
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

  c_ps.generalized_affine_image(x - y, GREATER_OR_EQUAL, Linear_Expression(3));

  ph.generalized_affine_image(x - y, GREATER_OR_EQUAL, Linear_Expression(3));
  ph1.generalized_affine_image(x - y, GREATER_OR_EQUAL, Linear_Expression(3));

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

// Powerset of polyhedra: bounded_affine_image(
//                          Variable,
//                          Relation_Symbol relsym,
//                          const Linear_Expression&,
//                          Coefficient_traits::const_reference)..
bool
test05() {
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

  c_ps.bounded_affine_image(y, Linear_Expression(0), 2*y, 5);

  ph.bounded_affine_image(y, Linear_Expression(0), 2*y, 5);
  ph1.bounded_affine_image(y, Linear_Expression(0), 2*y, 5);

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
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
