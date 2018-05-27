/* Test Pointset_Powerset<PH>::add_disjunct().
        Pointset_Powerset<PH>::drop_disjunct().
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

// Powerset of C polyhedra: add_disjunct().
bool
test01() {
  Variable x(0);
  Constraint_System cs;
  Pointset_Powerset<C_Polyhedron> c_ps(1, EMPTY);

  cs.clear();
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  c_ps.add_disjunct(C_Polyhedron(cs));

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 3);
  c_ps.add_disjunct(C_Polyhedron(cs));

  c_ps.add_constraint(x == 1);

  Pointset_Powerset<NNC_Polyhedron> nnc_ps(c_ps);

  Pointset_Powerset<C_Polyhedron>::const_iterator c_i = c_ps.begin();
  C_Polyhedron c_phi = c_i->pointset();
  print_constraints(c_phi, "*** c_phi ***");
  Pointset_Powerset<NNC_Polyhedron>::const_iterator nnc_i = nnc_ps.begin();
  NNC_Polyhedron nnc_phi = nnc_i->pointset();
  print_constraints(nnc_phi, "*** nnc_phi ***");

  return c_ps.OK() && nnc_ps.OK();
}

// Powerset of NNC polyhedra: add_disjunct().
bool
test02() {
  Variable x(0);
  Constraint_System cs;
  Pointset_Powerset<NNC_Polyhedron> nnc_ps(1, EMPTY);

  cs.clear();
  cs.insert(x > 0);
  cs.insert(x <= 1);
  nnc_ps.add_disjunct(NNC_Polyhedron(cs));

  cs.clear();
  cs.insert(x >= 0);
  cs.insert(x < 1);
  nnc_ps.add_disjunct(NNC_Polyhedron(cs));

  Pointset_Powerset<C_Polyhedron> c_ps(nnc_ps);

  Pointset_Powerset<C_Polyhedron>::const_iterator c_i = c_ps.begin();
  C_Polyhedron c_phi = c_i->pointset();
  print_constraints(c_phi, "*** c_phi ***");
  Pointset_Powerset<NNC_Polyhedron>::const_iterator nnc_i = nnc_ps.begin();
  NNC_Polyhedron nnc_phi = nnc_i->pointset();
  print_constraints(nnc_phi, "*** nnc_phi ***");

  return nnc_ps.OK() && c_ps.OK();
}

// Powerset of C polyhedra: add_disjunct() and drop_disjunct().
bool
test03() {
  Variable x(0);
  Pointset_Powerset<C_Polyhedron> c_ps(1, EMPTY);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  Constraint_System cs1 = cs;
  c_ps.add_disjunct(C_Polyhedron(cs));
  c_ps.drop_disjunct(c_ps.begin());

  bool ok = c_ps.empty();

  Constraint_System cs2 = cs1;
  c_ps.add_disjunct(C_Polyhedron(cs1));

  cs.insert(x >= 0);
  cs.insert(x <= 3);
  c_ps.add_disjunct(C_Polyhedron(cs));
  c_ps.drop_disjuncts(c_ps.begin(), c_ps.end());

  bool ok1 = c_ps.empty();

  return ok && ok1;
}

// Powerset of C polyhedra: add_disjunct().
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

  phi.upper_bound_assign(phi1);
  print_constraints(phi, "*** phi ***");

  return ok && ok1;
}

// Powerset of boxes: add_disjunct().
bool
test05() {
  Variable x(0);
  Variable y(1);
  Pointset_Powerset<TBox> pps_box(2, EMPTY);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  TBox box(2);
  box.add_constraints(cs);
  pps_box.add_disjunct(box);

  Constraint_System cs1;
  cs1.insert(y >= 3);
  cs1.insert(y <= 5);
  TBox box1(2);
  box1.add_constraints(cs1);
  pps_box.add_disjunct(box1);

  Pointset_Powerset<TBox>::const_iterator i = pps_box.begin();
  TBox pps_boxi = i->pointset();
  i++;
  TBox pps_boxi1 = i->pointset();

  bool ok = pps_boxi.OK() && pps_boxi == box;

  print_constraints(box, "*** box ***");
  print_constraints(pps_boxi, "*** pps_boxi ***");

  bool ok1 = pps_boxi1.OK() && pps_boxi1 == box1;

  print_constraints(box1, "*** box1 ***");
  print_constraints(pps_boxi1, "*** pps_boxi1 ***");

  pps_boxi.upper_bound_assign(pps_boxi1);
  print_constraints(pps_boxi, "*** pps_boxi ***");

  return ok && ok1;
}

// Powerset of boxes: add_disjunct().
bool
test06() {
  Variable x(0);

  Pointset_Powerset<TBox> ps(1, EMPTY);
  Constraint_System cs;
  cs.clear();
  cs.insert(x >= 0);
  cs.insert(x < 2);
  ps.add_disjunct(TBox(cs));

  cs.clear();
  cs.insert(x >= 1);
  cs.insert(x <= 3);
  ps.add_disjunct(TBox(cs));

  print_constraints(ps, "*** ps ***");

  ps.add_constraint(x == 1);

  Pointset_Powerset<TBox> known_result(1, EMPTY);
  TBox box(1);
  box.add_constraint(x == 1);
  known_result.add_disjunct(box);

  bool ok = (ps == known_result);

  print_constraints(ps, "*** ps.add_constraint(x == 1) ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

// Powerset of boxes: add_disjuncts().
bool
test07() {
  Variable x(0);
  Variable y(1);
  Pointset_Powerset<TBox> pps_box(2, EMPTY);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  TBox box(2);
  box.add_constraints(cs);
  pps_box.add_disjunct(box);

  Constraint_System cs1;
  cs1.insert(y >= 3);
  cs1.insert(y <= 5);
  TBox box1(2);
  box1.add_constraints(cs1);
  pps_box.add_disjunct(box1);

  Pointset_Powerset<TBox>::const_iterator i = pps_box.begin();
  TBox pps_boxi = i->pointset();
  i++;
  TBox pps_boxi1 = i->pointset();

  bool ok = pps_boxi.OK() && pps_boxi == box;

  print_constraints(box, "*** box ***");
  print_constraints(pps_boxi, "*** pps_boxi ***");

  bool ok1 = pps_boxi1.OK() && pps_boxi1 == box1;

  print_constraints(box1, "*** box1 ***");
  print_constraints(pps_boxi1, "*** pps_boxi1 ***");

  pps_boxi.upper_bound_assign(pps_boxi1);
  print_constraints(pps_boxi, "*** pps_boxi ***");

  return ok && ok1;
}

// Powerset of boxes: add_disjunct(), drop_disjuncts().
bool
test08() {
  Variable x(0);
  Pointset_Powerset<TBox> pps_box(1, EMPTY);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 2);
  Constraint_System cs1 = cs;
  pps_box.add_disjunct(TBox(cs));
  pps_box.drop_disjunct(pps_box.begin());

  bool ok = pps_box.empty();

  Constraint_System cs2 = cs1;
  pps_box.add_disjunct(TBox(cs1));

  cs.insert(x >= 0);
  cs.insert(x <= 3);
  pps_box.add_disjunct(TBox(cs));
  pps_box.drop_disjuncts(pps_box.begin(), pps_box.end());

  bool ok1 = pps_box.empty();

  return ok && ok1;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
END_MAIN
