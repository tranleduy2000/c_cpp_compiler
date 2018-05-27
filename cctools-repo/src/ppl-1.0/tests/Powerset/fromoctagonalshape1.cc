/* Test Pointset_Powerset<PH>::Pointset_Powerset(BD_Shape<T>),
   Pointset_Powerset<PH>::Pointset_Powerset(Pointset_Powerset(BD_Shape<T>)).
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

// Constructs the powerset of polyhedra from an octagonal shape.
bool
test01() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2);
  os.add_constraint(x >= 0);
  os.add_constraint(x - y <= 1);
  os.add_constraint(y >= 0);

  Pointset_Powerset<C_Polyhedron> pps(os);

  Pointset_Powerset<C_Polyhedron> known_pps(2);
  known_pps.add_constraint(x >= 0);
  known_pps.add_constraint(x - y <= 1);
  known_pps.add_constraint(y >= 0);

  bool ok = (pps == known_pps);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = pps.begin();
  C_Polyhedron phi = i->pointset();
  print_constraints(phi, "*** phi ***");

  return ok;
}

// Constructs the powerset of polyhedra from an empty octagonal shape.
bool
test02() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2, EMPTY);

  Pointset_Powerset<C_Polyhedron> pps(os);

  Pointset_Powerset<C_Polyhedron> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of polyhedra from a powerset of octagonal shapes.
bool
test03() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 1);
  cs.insert(x <= 1);
  cs.insert(y <= 0);
  TOctagonal_Shape os(cs);

  // The complexity should be ignored.
  Pointset_Powerset<TOctagonal_Shape> pps_os(os, POLYNOMIAL_COMPLEXITY);

  // The complexity should be ignored.
  Pointset_Powerset<C_Polyhedron> pps(pps_os, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<C_Polyhedron> known_pps(2);
  known_pps.add_constraint(x == 1);
  known_pps.add_constraint(y <= 0);

  bool ok = (pps == known_pps);

  Pointset_Powerset<C_Polyhedron>::const_iterator j = pps.begin();
  C_Polyhedron phj = j->pointset();
  print_constraints(phj, "*** pps disjunct ***");

  return ok;
}

// Constructs the powerset of bd shapes from an octagonal shape.
bool
test04() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2);
  os.add_constraint(x == 0);
  os.add_constraint(x + y <= 3);
  os.add_constraint(y >= 1);

  Pointset_Powerset<TBD_Shape> pps(os);

  Pointset_Powerset<TBD_Shape> known_pps(2);
  known_pps.add_constraint(x == 0);
  known_pps.add_constraint(y >= 1);
  known_pps.add_constraint(y <= 3);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TBD_Shape>::const_iterator i = pps.begin();
  TBD_Shape bdsi = i->pointset();
  print_constraints(bdsi, "*** bdsi ***");

  return ok;
}

// Constructs the powerset of bd shapes from an empty octagonal shape.
bool
test05() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2, EMPTY);

  Pointset_Powerset<TBD_Shape> pps(os);

  Pointset_Powerset<TBD_Shape> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of bd shapes from a powerset of octagonal shapes.
bool
test06() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 1);
  cs.insert(x - y <= 1);
  cs.insert(y <= 0);
  TOctagonal_Shape os(cs);

  // The complexity should be ignored.
  Pointset_Powerset<TOctagonal_Shape> pps_os(os, POLYNOMIAL_COMPLEXITY);

  // The complexity should be ignored.
  Pointset_Powerset<TBD_Shape> pps(pps_os, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<TBD_Shape> known_pps(2);
  known_pps.add_constraint(x == 1);
  known_pps.add_constraint(y == 0);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TBD_Shape>::const_iterator i_bds = pps.begin();
  TBD_Shape bdsi = i_bds->pointset();
  print_constraints(bdsi, "*** bdsi ***");

  return ok;
}

// Constructs the powerset of octagonal_shapes from an octagonal shape.
bool
test07() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  TOctagonal_Shape os(4);
  os.add_constraint(x >= 2);
  os.add_constraint(x + y <= 4);
  os.add_constraint(z == 1);

  Pointset_Powerset<TOctagonal_Shape> pps1(os);
  Pointset_Powerset<TOctagonal_Shape> pps2(4, EMPTY);
  pps2.add_disjunct(os);

  bool ok = (pps1 == pps2);

  Pointset_Powerset<TOctagonal_Shape>::const_iterator i = pps1.begin();
  TOctagonal_Shape osi = i->pointset();
  print_constraints(osi, "*** osi ***");
  Pointset_Powerset<TOctagonal_Shape>::const_iterator i2 = pps2.begin();
  TOctagonal_Shape osi2 = i2->pointset();
  print_constraints(osi2, "*** osi2 ***");

  return ok && pps1.OK();
}

// Constructs the powerset of octagonal_shapes from an empty octagonal shape.
bool
test08() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2, EMPTY);

  Pointset_Powerset<TOctagonal_Shape> pps(os);

  Pointset_Powerset<TOctagonal_Shape> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of octagonal_shapes from a powerset of
// octagonal_shapes.
bool
test09() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 1);
  cs.insert(x <= 1);
  cs.insert(y <= 0);
  TOctagonal_Shape os(cs);

  // The complexity should be ignored.
  Pointset_Powerset<TOctagonal_Shape> pps_os(os, POLYNOMIAL_COMPLEXITY);

  // The complexity should be ignored.
  Pointset_Powerset<TOctagonal_Shape> pps(pps_os, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<TOctagonal_Shape> known_pps(2);
  known_pps.add_constraint(x == 1);
  known_pps.add_constraint(y <= 0);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TOctagonal_Shape>::const_iterator j = pps.begin();
  TOctagonal_Shape osj = j->pointset();
  print_constraints(osj, "*** pps disjunct ***");

  return ok;
}

// Constructs the powerset of boxes from an octagonal shape.
bool
test10() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2);
  os.add_constraint(x >= 0);
  os.add_constraint(x + y <= 4);
  os.add_constraint(x - y == 3);
  os.add_constraint(y >= 0);

  // Complexity should be ignored.
  Pointset_Powerset<TBox> pps(os, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<TBox> known_pps(2);
  known_pps.add_constraint(x >= 3);
  known_pps.add_constraint(2*x <= 7);
  known_pps.add_constraint(y >= 0);
  known_pps.add_constraint(2*y <= 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TBox>::const_iterator i = pps.begin();
  TBox phi = i->pointset();
  print_constraints(phi, "*** phi ***");

  return ok;
}

// Constructs the powerset of boxes from an empty octagonal shape.
bool
test11() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2, EMPTY);

  Pointset_Powerset<TBox> pps(os);

  Pointset_Powerset<TBox> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of boxes from a powerset of octagonal shapes.
bool
test12() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 1);
  cs.insert(x - y <= 1);
  cs.insert(y <= 0);
  TOctagonal_Shape os(cs);

  // The complexity should be ignored.
  Pointset_Powerset<TOctagonal_Shape> pps_os(os, POLYNOMIAL_COMPLEXITY);

  // The complexity should be ignored.
  Pointset_Powerset<TBox> pps(pps_os, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<TBox> known_pps(2);
  known_pps.add_constraint(x == 1);
  known_pps.add_constraint(y == 0);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TBox>::const_iterator i_box = pps.begin();
  TBox boxi = i_box->pointset();
  print_constraints(boxi, "*** boxi ***");

  return ok;
}

// Constructs the powerset of grids from an octagonal shape.
bool
test13() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2);
  os.add_constraint(x >= 0);
  os.add_constraint(x + y <= 4);
  os.add_constraint(x - y == 3);
  os.add_constraint(y >= 0);

  // Complexity should be ignored.
  Pointset_Powerset<Grid> pps(os, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<Grid> known_pps(2);
  known_pps.add_constraint(x - y == 3);

  bool ok = (pps == known_pps);

  Pointset_Powerset<Grid>::const_iterator i = pps.begin();
  Grid phi = i->pointset();
  print_congruences(phi, "*** phi ***");

  return ok;
}

// Constructs the powerset of grids from an empty octagonal shape.
bool
test14() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape os(2, EMPTY);

  Pointset_Powerset<Grid> pps(os);

  Pointset_Powerset<Grid> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of grids from a powerset of octagonal shapes.
bool
test15() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x >= 1);
  cs.insert(x - y <= 1);
  cs.insert(y <= 0);
  TOctagonal_Shape os(cs);

  // The complexity should be ignored.
  Pointset_Powerset<TOctagonal_Shape> pps_os(os, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<TOctagonal_Shape>::const_iterator i_os = pps_os.begin();
  TOctagonal_Shape osi = i_os->pointset();
  print_constraints(osi, "*** osi ***");

  // The complexity should be ignored.
  Pointset_Powerset<Grid> pps(pps_os, POLYNOMIAL_COMPLEXITY);

  Pointset_Powerset<Grid> known_pps(2);
  known_pps.add_constraint(x == 1);
  known_pps.add_constraint(y == 0);

  bool ok = (pps == known_pps);

  Pointset_Powerset<Grid>::const_iterator i_gr = pps.begin();
  Grid gri = i_gr->pointset();
  print_congruences(gri, "*** gri ***");

  return ok;
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
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
END_MAIN
