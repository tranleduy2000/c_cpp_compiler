/* Test Box::Box(const Grid&, Complexity_Class).
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

// Minimized rectilinear grid.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(B));
  gr.add_grid_generator(grid_point(3*A + B));
  gr.add_grid_generator(grid_point(3*A + 3*B));

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(2, UNIVERSE);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Skew grid.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(  A +   B));
  gr.add_grid_generator(grid_point(2*A + 3*B));
  gr.add_grid_generator(grid_point(  A + 4*B));

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(2, UNIVERSE);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Skew grid, with a divisor.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(2*A));
  gr.add_grid_generator(grid_point(  A + 2*B, 2));

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(2, UNIVERSE);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Grid containing a line.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_line(A + 2*B));
  gr.add_grid_generator(grid_point(C, 2));

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(3, UNIVERSE);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Universe grid.
bool
test05() {
  Grid gr(3, UNIVERSE);

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(3, UNIVERSE);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Grid which is a single point.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(16*A + 6*B - 6*C, 7));

  print_congruences(gr1, "*** gr1 before ***");

  Rational_Box box1(gr1);

  Rational_Box known_box(3);
  known_box.add_constraint(7*A == 16);
  known_box.add_constraint(7*B == 6);
  known_box.add_constraint(7*C == -6);

  bool ok = (box1 == known_box);

  print_constraints(box1, "*** box1 ***");
  print_congruences(gr1, "*** gr1 after ***");
  print_constraints(known_box, "*** known_box ***");

  if (ok) {
    Grid gr2(box1);

    print_congruences(gr2, "*** gr2 before ***");

    Rational_Box box2(gr2);

    ok = (box2 == known_box);

    print_constraints(box2, "*** box2 ***");
    print_congruences(gr2, "*** gr2 after ***");
    print_constraints(known_box, "*** known_box ***");
  }

  return ok;
}

// Empty grid.
bool
test07() {
  Grid gr1(3, EMPTY);

  print_congruences(gr1, "*** gr1 before ***");

  Rational_Box box1(gr1);

  Rational_Box known_box(3, EMPTY);

  bool ok = (box1 == known_box);

  print_constraints(box1, "*** box1 ***");
  print_congruences(gr1, "*** gr1 after ***");
  print_constraints(known_box, "*** known_box ***");

  if (ok) {
    Grid gr2(box1);

    print_congruences(gr2, "*** gr2 before ***");

    Rational_Box box2(gr2);

    ok = (box2 == known_box);

    print_constraints(box2, "*** box2 ***");
    print_congruences(gr2, "*** gr2 after ***");
    print_constraints(known_box, "*** known_box ***");
  }

  return ok;
}

// A grid with redundant generators.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A + B));
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(2*A));

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(3);
  known_box.add_constraint(C == 0);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// A grid defined by congruences.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((A + 2*C %= 0) / 2);
  gr.add_congruence((A %= 0) / 5);
  gr.add_constraint(2*B == 3);

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(3);
  known_box.add_constraint(2*B == 3);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Grid which is a single point.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr1(3, EMPTY);
  gr1.add_grid_generator(grid_point(16*A + 14*B - 6*C, 7));

  print_congruences(gr1, "*** gr1 before ***");

  Rational_Box box1(gr1);

  Rational_Box known_box(3);
  known_box.add_constraint(7*A == 16);
  known_box.add_constraint(B == 2);
  known_box.add_constraint(7*C == -6);

  bool ok = (box1 == known_box);

  print_constraints(box1, "*** box1 ***");
  print_congruences(gr1, "*** gr1 after ***");
  print_constraints(known_box, "*** known_box ***");

  if (ok) {
    Grid gr2(box1);

    print_congruences(gr2, "*** gr2 before ***");

    Rational_Box box2(gr2);

    ok = (box2 == known_box);

    print_constraints(box2, "*** box2 ***");
    print_congruences(gr2, "*** gr2 after ***");
    print_constraints(known_box, "*** known_box ***");
  }

  return ok;
}

// An empty grid defined by congruences.
bool
test11() {
  Variable A(0);

  Grid gr1(3);
  gr1.add_congruence((A %= 0) / 2);
  gr1.add_congruence((A %= 1) / 2);

  print_congruences(gr1, "*** gr1 before ***");

  Rational_Box box1(gr1);

  Rational_Box known_box(3, EMPTY);

  bool ok = (box1 == known_box);

  print_constraints(box1, "*** box1 ***");
  print_congruences(gr1, "*** gr1 after ***");
  print_constraints(known_box, "*** known_box ***");

  if (ok) {
    Grid gr2(box1);

    print_congruences(gr2, "*** gr2 before ***");

    Rational_Box box2(gr2);

    ok = (box2 == known_box);

    print_constraints(box2, "*** box2 ***");
    print_congruences(gr2, "*** gr2 after ***");
    print_constraints(known_box, "*** known_box ***");
  }

  return ok;
}

// Simple grid where all the points have the same value in one of the
// dimensions (B).
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(C));
  gr.add_grid_generator(grid_point(D));

  print_generators(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(4);
  known_box.add_constraint(B == 0);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Simple grid, with a divisor, such that the fractions for some
// intervals (B and C) will be reduced before being assigned to the
// intervals.
bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4, EMPTY);
  gr.add_grid_generator(grid_point(  A + 2*B + 4*C, 4));
  gr.add_grid_generator(grid_point(2*A + 2*B + 4*C, 4));
  gr.add_grid_generator(grid_point(  A + 2*B + 4*C + D, 4));

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(4, UNIVERSE);
  known_box.add_constraint(2*B == 1);
  known_box.add_constraint(C == 1);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Zero dimension empty grid.
bool
test14() {
  Grid gr(0, EMPTY);

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(0, EMPTY);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Zero dimension universe grid.
bool
test15() {
  Grid gr(0, UNIVERSE);

  print_congruences(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(0, UNIVERSE);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

  return ok;
}

// Simple grid, with a divisor and 2 points.
bool
test16() {
  Variable A(0);

  Grid gr(1, EMPTY);
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(A, 2));
  print_generators(gr, "*** gr before ***");

  Rational_Box box(gr);

  Rational_Box known_box(1, UNIVERSE);

  bool ok = (box == known_box);

  print_constraints(box, "*** box ***");
  print_generators(gr, "*** gr after ***");
  print_congruences(gr, "*** gr after ***");
  print_constraints(known_box, "*** known_box ***");

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
