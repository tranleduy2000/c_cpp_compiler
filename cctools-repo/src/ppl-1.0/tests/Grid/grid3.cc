/* Test construction of grids from constraints.
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

// Grid(cs)
bool
test01() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(B == 0);

  Grid gr(cs);

  Grid known_gr(2);
  known_gr.add_constraint(B == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(cs) ***");

  return ok;
}


// Building from an inequality exception.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;

  cs.insert(A >= B);

  try {
    Grid gr(cs);
   }
  catch (const std::invalid_argument& e) {
    nout << "cs contains an inequality constraint: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Grid(cs), cs empty and resulting grid universe.
bool
test03() {

  Constraint_System cs;
  print_constraints(cs, "*** cs ***");

  Grid gr(cs);

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(cs) ***");

  return ok;
}

// Grid(const cs)
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(2*B == A);
  cs.insert(0*C == 0);

  const Constraint_System ccs = cs;

  Grid gr(ccs);

  Grid known_gr(3);
  known_gr.add_constraint(2*B == A);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ccs) ***");

  return ok;
}

// Grid(const cs), resulting grid empty.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(2*B == A);
  cs.insert(2*B == 0);
  cs.insert(A == 1);
  cs.insert(C == 4);

  const Congruence_System cgs(cs);

  Grid gr(cgs);

  Grid known_gr(3, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(cgs) ***");

  return ok;
}

// Space dimension exception.
bool
test06() {
  try {
    Grid gr(Constraint_System::max_space_dimension() + 1);
  }
  catch (const std::length_error& e) {
    nout << "length_error: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Assignment of universe grid, zero dimensions.
bool
test07() {
  Grid gr(0, EMPTY);

  gr = Grid(0);

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr ***");

  return ok;
}

// Space dimension exception.
bool
test08() {
  try {
    Grid gr(Grid::max_space_dimension() + 1);
  }
  catch (const std::length_error& e) {
    nout << "length_error: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Create grid from empty zero dimension constraint system.
bool
test09() {
  Constraint_System cs(Constraint::zero_dim_false());

  Grid gr(cs);

  print_congruences(gr, "*** gr(cs) ***");

  Grid known_gr(0, EMPTY);

  print_congruences(known_gr, "*** known_gr ***");

  bool ok = (gr == known_gr);

  return ok;
}

// Create grid from universe zero dimension constraint system.
bool
test10() {
  Constraint_System cs(Constraint::zero_dim_positivity());

  Grid gr(cs);

  print_congruences(gr, "*** gr(cs) ***");

  Grid known_gr(0);

  print_congruences(known_gr, "*** known_gr ***");

  bool ok = (gr == known_gr);

  return ok;
}

// Create grid from const empty zero dimension constraint system.
bool
test11() {
  const Constraint_System cs(Constraint::zero_dim_false());

  Grid gr(cs);

  print_congruences(gr, "*** gr(cs) ***");

  Grid known_gr(0, EMPTY);

  print_congruences(known_gr, "*** known_gr ***");

  bool ok = (gr == known_gr);

  return ok;
}

// Create grid from const universe zero dimension constraint system.
bool
test12() {
  const Constraint_System cs(Constraint::zero_dim_positivity());

  Grid gr(cs);

  print_congruences(gr, "*** gr(cs) ***");

  Grid known_gr(0);

  print_congruences(known_gr, "*** known_gr ***");

  bool ok = (gr == known_gr);

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
END_MAIN
