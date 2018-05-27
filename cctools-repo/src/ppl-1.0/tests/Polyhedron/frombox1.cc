/* Test C_Polyhedron::C_Polyhedron(const Box<Interval>&)
   and NNC_Polyhedron::NNC_Polyhedron(const Box<Interval>&).
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

// Constructs the polyhedron { x >= 0, x <= 1/2, y >= 0 }
// from the corresponding box.
bool
test01() {
  Variable x(0);
  Variable y(1);

  Rational_Box box(2);
  box.add_constraint(x >= 0);
  box.add_constraint(2*x <= 1);
  box.add_constraint(y >= 0);

  C_Polyhedron ph(box);


  C_Polyhedron known_ph(box.space_dimension());
  known_ph.add_constraint(x >= 0);
  known_ph.add_constraint(2*x <= 1);
  known_ph.add_constraint(y >= 0);

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_ph, "*** known_ph ***");

  return ok;
}

// The box is the xy plane.
bool
test02() {
  Rational_Box box(2);

  C_Polyhedron ph(box);

  C_Polyhedron known_ph(box.space_dimension());

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_ph, "*** known_ph ***");

  return ok;
}

// This box is the closed +ve quadrant.
bool
test03() {
  Variable x(0);
  Variable y(1);

  Rational_Box box(2);
  box.add_constraint(x >= 0);
  box.add_constraint(y >= 0);

  C_Polyhedron ph(box);

  C_Polyhedron known_ph(box.space_dimension());
  known_ph.add_constraint(x >= 0);
  known_ph.add_constraint(y >= 0);

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_ph, "*** known_ph ***");

  return ok;
}

// A bounded box in 2D.
bool
test04() {
  Variable x(0);
  Variable y(1);


  Rational_Box box(2);
  box.add_constraint(3*x >= -2);
  box.add_constraint(x <= 4);
  box.add_constraint(y >= -10);
  box.add_constraint(3*y <= 12);

  C_Polyhedron ph(box);

  C_Polyhedron known_ph(box.space_dimension());
  known_ph.add_constraint(3*x >= -2);
  known_ph.add_constraint(x <= 4);
  known_ph.add_constraint(y <= 4);
  known_ph.add_constraint(y >= -10);

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_ph, "*** known_ph ***");

  return ok;
}

// An unbounded closed box in 4D but bounded in 2D.
bool
test05() {
  Variable x(1);
  Variable y(2);
  Variable z(3);

  Rational_Box box(4);
  box.add_constraint(3*x >= -2);
  box.add_constraint(x <= 4);
  box.add_constraint(y >= -10);
  box.add_constraint(3*y <= 12);
  box.add_constraint(3*z >= 15);

  C_Polyhedron ph(box);

  C_Polyhedron known_ph(box.space_dimension());
  known_ph.add_constraint(3*x >= -2);
  known_ph.add_constraint(x <= 4);
  known_ph.add_constraint(y <= 4);
  known_ph.add_constraint(y >= -10);
  known_ph.add_constraint(z >= 5);

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_ph, "*** known_ph ***");

  return ok;
}

// A zero-dimensional box.
bool
test06() {
  Rational_Box box(0);

  C_Polyhedron ph(box);

  C_Polyhedron known_ph;

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_ph, "*** known_ph ***");

  return ok;
}

// An empty closed box in 2D.
bool
test07() {
  Rational_Box box(2);
  box.set_empty();

  C_Polyhedron ph(box);

  C_Polyhedron known_ph(2, EMPTY);

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_ph, "*** known_ph ***");

  return ok;
}

// A single point.
bool
test08() {
  Variable x(0);
  Variable y(1);

  Rational_Box box(2);
  box.add_constraint(x == 2);
  box.add_constraint(y == 4);

  C_Polyhedron ph(box);

  C_Polyhedron known_ph(box.space_dimension());
  known_ph.add_constraint(x == 2);
  known_ph.add_constraint(y == 4);

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph ***");
  print_constraints(known_ph, "*** known_ph ***");

  return ok;
}

// A closed unit square.
bool
test09() {
  Variable x(0);
  Variable y(1);

  Rational_Box box(2);
  box.add_constraint(x >= 0);
  box.add_constraint(x <= 1);
  box.add_constraint(y >= 0);
  box.add_constraint(y <= 1);

  C_Polyhedron ph(box);

  Constraint_System known_cs;
  known_cs.insert(x >= 0);
  known_cs.insert(x <= 1);
  known_cs.insert(y >= 0);
  known_cs.insert(y <= 1);

  C_Polyhedron known_ph(known_cs);

  bool ok = (ph == known_ph);

  print_constraints(ph, "*** ph generators ***");
  print_constraints(known_ph, "*** known_ph ***");

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
END_MAIN
