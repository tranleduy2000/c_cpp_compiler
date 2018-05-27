/* Test C_Polyhedron::C_Polyhedron(const Grid&)
   and NNC_Polyhedron::NNC_Polyhedron(const Grid&).
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

// Constructs the polyhedron { x + y == 0, y - z == 0 }
// from a grid with the same equalities but 1 extra congruence.
bool
test01() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Grid gr(3);
  gr.add_congruence((x + y %= 0) / 0);
  gr.add_congruence((y - z %= 0) / 0);
  gr.add_congruence((z %= 0) / 1);

  TOctagonal_Shape oct(gr);


  Octagonal_Shape<mpq_class> known_oct(gr.space_dimension());
  known_oct.add_constraint(x + y == 0);
  known_oct.add_constraint(y - z == 0);

  bool ok = (check_result(oct, known_oct));

  print_constraints(oct, "*** oct ***");
  print_constraints(known_oct, "*** known_oct ***");

  return ok;
}

// The grid is the xy plane.
bool
test02() {
  Grid gr(2);

  TOctagonal_Shape oct(gr);

  Octagonal_Shape<mpq_class> known_oct(gr.space_dimension());

  bool ok = (check_result(oct, known_oct));

  print_constraints(oct, "*** oct ***");
  print_constraints(known_oct, "*** known_oct ***");

  return ok;
}

// This grid is unbounded.
bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Grid gr(3);
  gr.add_congruence((x + y %= 1) / 3);
  gr.add_congruence((y - z %= 2) / 5);

  TOctagonal_Shape oct(gr);

  Octagonal_Shape<mpq_class> known_oct(gr.space_dimension());

  bool ok = (check_result(oct, known_oct));

  print_constraints(oct, "*** oct ***");
  print_constraints(known_oct, "*** known_oct ***");

  return ok;
}

// A zero-dimensional grid.
bool
test04() {
  Grid gr(0);

  TOctagonal_Shape oct(gr);

  Octagonal_Shape<mpq_class> known_oct;

  bool ok = (check_result(oct, known_oct));

  print_constraints(oct, "*** oct ***");
  print_constraints(known_oct, "*** known_oct ***");

  return ok;
}

// An empty closed grid in 2D.
bool
test05() {
  Grid gr(2, EMPTY);

  TOctagonal_Shape oct(gr);

  Octagonal_Shape<mpq_class> known_oct(2, EMPTY);

  bool ok = (check_result(oct, known_oct));

  print_constraints(oct, "*** oct ***");
  print_constraints(known_oct, "*** known_oct ***");

  return ok;
}

// A single point.
bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Grid gr(3);
  gr.add_congruence((x - y %= 1) / 0);
  gr.add_congruence((y %= 0) / 0);

  TOctagonal_Shape oct(gr);

  Octagonal_Shape<mpq_class> known_oct(gr.space_dimension());
  known_oct.add_constraint(x == 1);
  known_oct.add_constraint(y == 0);

  bool ok = (check_result(oct, known_oct));

  print_constraints(oct, "*** oct ***");
  print_constraints(known_oct, "*** known_oct ***");

  return ok;
}

// A zero-dimensional empty grid.
bool
test07() {
  Grid gr(0, EMPTY);

  TOctagonal_Shape oct(gr);

  Octagonal_Shape<mpq_class> known_oct(0, EMPTY);

  bool ok = (check_result(oct, known_oct));

  print_constraints(oct, "*** oct ***");
  print_constraints(known_oct, "*** known_oct ***");

  return ok;
}

// The grid is the xy plane.
bool
test08() {
  Grid gr(2);
  Constraint_System cs = gr.constraints();

  TOctagonal_Shape oct(cs);

  Octagonal_Shape<mpq_class> known_oct(gr.space_dimension());

  bool ok = (check_result(oct, known_oct));

  print_constraints(oct, "*** oct ***");
  print_constraints(known_oct, "*** known_oct ***");

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
END_MAIN
