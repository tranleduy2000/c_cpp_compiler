/* Test Pointset_Powerset<PH>::Pointset_Powerset(Grid),
        Pointset_Powerset<PH>::Pointset_Powerset(Pointset_Powerset(Grid)).
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

// Constructs the powerset of polyhedra from a grid.
bool
test01() {
  Variable x(0);
  Variable y(1);

  Grid grid(2);
  grid.add_congruence((2*x %= 1) / 0);
  grid.add_congruence(y %= 0);

  Pointset_Powerset<C_Polyhedron> pps(grid);

  Pointset_Powerset<C_Polyhedron> known_pps(2);

  known_pps.add_constraint(2*x == 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = pps.begin();
  C_Polyhedron phi = i->pointset();
  print_constraints(phi, "*** phi ***");

  return ok;
}

// Constructs the powerset of polyhedra from an empty grid.
bool
test02() {
  Variable x(0);
  Variable y(1);

  Grid grid(2, EMPTY);

  Pointset_Powerset<C_Polyhedron> pps(grid);

  Pointset_Powerset<C_Polyhedron> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of polyhedra from a powerset of grids.
bool
test03() {
  Variable x(0);
  Variable y(1);

  Grid grid(2);
  grid.add_congruence((2*x %= 1) / 0);
  grid.add_congruence(y %= 0);

  Pointset_Powerset<Grid> pps_gr(grid);

  Pointset_Powerset<C_Polyhedron> pps(pps_gr);

  Pointset_Powerset<C_Polyhedron> known_pps(2);

  known_pps.add_constraint(2*x == 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<C_Polyhedron>::const_iterator i = pps.begin();
  C_Polyhedron phi = i->pointset();
  print_constraints(phi, "*** phi ***");

  return ok;
}

// Constructs the powerset of bd shapes from a grid.
bool
test04() {
  Variable x(0);
  Variable y(1);

  Grid gr(2);
  gr.add_congruence((2*x %= 1) / 0);
  gr.add_congruence(y %= 0);

  Pointset_Powerset<TBD_Shape> pps(gr);

  Pointset_Powerset<TBD_Shape> known_pps(2);
  known_pps.add_constraint(2*x == 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TBD_Shape>::const_iterator i = pps.begin();
  TBD_Shape bdsi = i->pointset();
  print_constraints(bdsi, "*** bdsi ***");

  return ok;
}

// Constructs the powerset of bd shapes from an empty grid.
bool
test05() {
  Variable x(0);
  Variable y(1);

  Grid gr(2, EMPTY);

  Pointset_Powerset<TBD_Shape> pps(gr);

  Pointset_Powerset<TBD_Shape> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of bd shapes from a powerset of grids.
bool
test06() {
  Variable x(0);
  Variable y(1);

  Grid gr(2);
  gr.add_congruence((2*x %= 1) / 0);
  gr.add_congruence(y %= 0);

  Pointset_Powerset<Grid> pps_gr(gr);

  Pointset_Powerset<TBD_Shape> pps(pps_gr);

  Pointset_Powerset<TBD_Shape> known_pps(2);

  known_pps.add_constraint(2*x == 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TBD_Shape>::const_iterator i = pps.begin();
  TBD_Shape bdsi = i->pointset();
  print_constraints(bdsi, "*** bdsi ***");

  return ok;
}

// Constructs the powerset of octagonal_shapes from a grid.
bool
test07() {
  Variable x(0);
  Variable y(1);

  Grid gr(2);
  gr.add_congruence((2*x %= 1) / 0);
  gr.add_congruence(y %= 0);

  Pointset_Powerset<TOctagonal_Shape> pps(gr);

  Pointset_Powerset<TOctagonal_Shape> known_pps(2);
  known_pps.add_constraint(2*x == 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TOctagonal_Shape>::const_iterator i = pps.begin();
  TOctagonal_Shape osi = i->pointset();
  print_constraints(osi, "*** osi ***");

  return ok;
}

// Constructs the powerset of octagonal_shapes from an empty grid.
bool
test08() {
  Variable x(0);
  Variable y(1);

  Grid gr(2, EMPTY);

  Pointset_Powerset<TOctagonal_Shape> pps(gr);

  Pointset_Powerset<TOctagonal_Shape> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of octagonal_shapes from a powerset of grids.
bool
test09() {
  Variable x(0);
  Variable y(1);

  Grid gr(2);
  gr.add_congruence((2*x %= 1) / 0);
  gr.add_congruence(y %= 0);

  Pointset_Powerset<Grid> pps_gr(gr);

  Pointset_Powerset<TOctagonal_Shape> pps(pps_gr);

  Pointset_Powerset<TOctagonal_Shape> known_pps(2);

  known_pps.add_constraint(2*x == 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TOctagonal_Shape>::const_iterator i = pps.begin();
  TOctagonal_Shape osi = i->pointset();
  print_constraints(osi, "*** osi ***");

  return ok;
}

// Constructs the powerset of boxes from a grid.
bool
test10() {
  Variable x(0);
  Variable y(1);

  Grid gr(2);
  gr.add_congruence((2*x %= 1) / 0);
  gr.add_congruence(y %= 0);

  Pointset_Powerset<TBox> pps(gr);

  Pointset_Powerset<TBox> known_pps(2);
  known_pps.add_constraint(2*x == 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TBox>::const_iterator i = pps.begin();
  TBox phi = i->pointset();
  print_constraints(phi, "*** phi ***");

  return ok;
}

// Constructs the powerset of boxes from an empty grid.
bool
test11() {
  Variable x(0);
  Variable y(1);

  Grid gr(2, EMPTY);

  Pointset_Powerset<TBox> pps(gr);

  Pointset_Powerset<TBox> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of boxes from a powerset of grids.
bool
test12() {
  Variable x(0);
  Variable y(1);

  Grid gr(2);
  gr.add_congruence((2*x %= 1) / 0);
  gr.add_congruence(y %= 0);

  Pointset_Powerset<Grid> pps_gr(gr);

  Pointset_Powerset<TBox> pps(pps_gr);

  Pointset_Powerset<TBox> known_pps(2);

  known_pps.add_constraint(2*x == 1);

  bool ok = (pps == known_pps);

  Pointset_Powerset<TBox>::const_iterator i = pps.begin();
  TBox boxi = i->pointset();
  print_constraints(boxi, "*** boxi ***");

  return ok;
}

// Constructs the powerset of grids from a grid.
bool
test13() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Grid gr(4);
  gr.add_congruence(x %= 2);
  gr.add_constraint(z == 1);

  Pointset_Powerset<Grid> pps1(gr);
  Pointset_Powerset<Grid> pps2(4, EMPTY);
  pps2.add_disjunct(gr);

  bool ok = (pps1 == pps2);

  print_congruences(gr, "*** gr ***");
  Pointset_Powerset<Grid>::const_iterator i = pps1.begin();
  Grid gri = i->pointset();
  print_congruences(gri, "*** gri ***");
  Pointset_Powerset<Grid>::const_iterator i2 = pps2.begin();
  Grid gri2 = i2->pointset();
  print_congruences(gri2, "*** gri2 ***");

  return ok && pps1.OK();
}

// Constructs the powerset of boxes from an empty grid.
bool
test14() {
  Variable x(0);
  Variable y(1);

  Grid gr(2, EMPTY);

  Pointset_Powerset<TBox> pps(gr);

  Pointset_Powerset<TBox> known_pps(2, EMPTY);

  bool ok = (pps == known_pps);

  return ok;
}

// Constructs the powerset of grids from a powerset of grids.
bool
test15() {
  Variable x(0);
  Variable y(1);

  Grid grid(2);
  grid.add_congruence((2*x %= 1) / 0);
  grid.add_congruence(y %= 0);

  Pointset_Powerset<Grid> pps_gr(grid);

  Pointset_Powerset<Grid> pps(pps_gr);

  Pointset_Powerset<Grid> known_pps(2);

  known_pps.add_constraint(2*x == 1);
  known_pps.add_congruence(y %= 0);

  bool ok = (pps == known_pps);

  Pointset_Powerset<Grid>::const_iterator i = pps.begin();
  Grid gri = i->pointset();
  print_congruences(gri, "*** gri ***");

  return ok;
}

// Constructs the powerset of NNC polyhedra from a powerset of grids
// where set of grids is omega reduced but the constructed set
// of NNC polyhedra is not omega reduced.
bool
test16() {
  Variable x(0);
  Variable y(1);

  Grid grid1(2);
  grid1.add_congruence((x %= 0) / 2);
  grid1.add_congruence((y %= 0) / 2);
  Grid grid2(2);
  grid2.add_congruence((x %= 1) / 2);
  grid2.add_congruence((y %= 1) / 0);

  Pointset_Powerset<Grid> pps_gr(grid1);
  pps_gr.add_disjunct(grid2);

  // At this stage, pps_gr is omega reduced but pps_gr.reduced flag will
  // be set to false.
  // So we add this test to set the omega reduction pps_gr.reduced
  // flag to true.
  pps_gr.is_topologically_closed();

  Pointset_Powerset<NNC_Polyhedron> pps(pps_gr);

  // pps is not omega reduced.
  bool ok = (pps.size() == 2);

  Pointset_Powerset<NNC_Polyhedron> known_pps(2);

  ok = ok && (pps == known_pps) && pps.OK();

  Pointset_Powerset<NNC_Polyhedron>::const_iterator i = pps.begin();
  NNC_Polyhedron phi = i->pointset();
  print_constraints(phi, "*** phi ***");

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
  DO_TEST(test16);
END_MAIN
