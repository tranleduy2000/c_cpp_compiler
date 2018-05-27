/* Test Grid::relation_with(g).
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

// Empty grid and point.
bool
test01() {
  Variable A(0);

  Grid gr(2, EMPTY);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(grid_point(A)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Universe and point.
bool
test02() {
  Variable A(0);

  Grid gr(2);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(grid_point(A)) == Poly_Gen_Relation::subsumes());

  return ok;
}

// Lined grid and point.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(B));
  gs.insert(grid_line(A));

  Grid gr(gs);
  print_congruences(gr, "*** gr ***");

  bool ok
    = (gr.relation_with(grid_point(A + B)) == Poly_Gen_Relation::subsumes());

  return ok;
}

// Equality and point.
bool
test04() {
  Variable A(0);

  Grid gr(2);
  gr.add_constraint(A == 0);
  print_congruences(gr, "*** gr ***");

  bool ok
    = (gr.relation_with(grid_point(2*A)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Congruences and points.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A - B %= 1) / 2);
  gr.add_congruence((A %= 1) / 3);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(grid_point()) == Poly_Gen_Relation::nothing()
	     && gr.relation_with(grid_point(-B)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Congruence and parameter.
bool
test06() {
  Variable A(0);

  Grid gr(2);
  gr.add_congruence(2*A %= 0);
  print_congruences(gr, "*** gr ***");

  bool ok
    = (gr.relation_with(parameter(A, 2)) == Poly_Gen_Relation::subsumes());

  return ok;
}

// Congruence and line.
bool
test07() {
  Variable A(0);

  Grid gr(2);
  gr.add_congruence(2*A %= 0);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(grid_line(A)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Space dimension exception.
bool
test08() {
  Variable A(0);
  Variable C(2);

  Grid gr(2);

  try {
    gr.relation_with(grid_line(A + C));
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Zero dimension universe grid.
bool
test09() {
  Grid gr(0);

  bool ok = (gr.relation_with(grid_point()) == Poly_Gen_Relation::subsumes());

  return ok;
}

// Empty grid and point.
bool
test10() {
  Variable A(0);

  Grid gr(2, EMPTY);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(point(A)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Universe and point.
bool
test11() {
  Variable A(0);

  Grid gr(2);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(point(A)) == Poly_Gen_Relation::subsumes());

  return ok;
}

// Lined grid and point.
bool
test12() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(B));
  gs.insert(grid_line(A));

  Grid gr(gs);
  print_congruences(gr, "*** gr ***");

  bool ok
    = (gr.relation_with(point(A + B)) == Poly_Gen_Relation::subsumes());

  return ok;
}

// Equality and point.
bool
test13() {
  Variable A(0);

  Grid gr(2);
  gr.add_constraint(A == 0);
  print_congruences(gr, "*** gr ***");

  bool ok
    = (gr.relation_with(point(2*A)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Congruences and points.
bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A - B %= 1) / 2);
  gr.add_congruence((A %= 1) / 3);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(point()) == Poly_Gen_Relation::nothing()
	     && gr.relation_with(point(-B)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Congruence and parameter.
bool
test15() {
  Variable A(0);

  Grid gr(2);
  gr.add_congruence(2*A %= 0);
  print_congruences(gr, "*** gr ***");

  bool ok
    = (gr.relation_with(ray(A)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Congruence and line.
bool
test16() {
  Variable A(0);

  Grid gr(2);
  gr.add_congruence(2*A %= 0);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(line(A)) == Poly_Gen_Relation::nothing());

  return ok;
}

// Space dimension exception.
bool
test17() {
  Variable A(0);
  Variable C(2);

  Grid gr(2);

  try {
    gr.relation_with(line(A + C));
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Zero dimension universe grid.
bool
test18() {
  Grid gr(0);

  bool ok = (gr.relation_with(point()) == Poly_Gen_Relation::subsumes());

  return ok;
}

// Lined grid and line.
bool
test19() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(B));
  gs.insert(grid_line(A));

  Grid gr(gs);
  print_congruences(gr, "*** gr ***");

  bool ok
    = (gr.relation_with(line(A + B)) == Poly_Gen_Relation::nothing()
       && gr.relation_with(line(A)) == Poly_Gen_Relation::subsumes());

  return ok;
}

// Lined grid and line.
bool
test20() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point());
  gs.insert(grid_point(B));
  gs.insert(grid_line(A));

  Grid gr(gs);
  print_congruences(gr, "*** gr ***");

  bool ok
    = (gr.relation_with(closure_point(A + B)) == Poly_Gen_Relation::subsumes()
       && gr.relation_with(closure_point(A)) == Poly_Gen_Relation::subsumes());

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
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
  DO_TEST(test20);
END_MAIN
