/* Test Grid::fold_space_dimensions().
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

// Universe grid.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid gr(3);
  print_congruences(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(A);

  gr.fold_space_dimensions(to_fold, B);

  Grid known_gr(2);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, B) ***");

  return ok;
}

// Empty grid.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr(3, EMPTY);
  print_congruences(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(A);

  gr.fold_space_dimensions(to_fold, B);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, B) ***");

  return ok;
}

// Trivial fold.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence(A %= 0);
  gr.add_congruence((A + B + C %= 2) / 3);
  print_congruences(gr, "*** gr ***");

  Grid known_gr = gr;

  Variables_Set to_fold;

  gr.fold_space_dimensions(to_fold, B);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, B) ***");

  return ok;
}

// Simple fold from congruences.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence(A %= 1);
  gr.add_congruence((B %= 1) / 3);
  print_congruences(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(A);

  gr.fold_space_dimensions(to_fold, B);

  Grid known_gr(1);
  known_gr.add_congruence(A %= 1);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, B) ***");

  return ok;
}

// Simple fold from generators.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point(A + 2*B + 2*C));
  gr.add_grid_generator(grid_point(A + 2*B + 4*C));
  gr.add_grid_generator(grid_point(A + 6*B + 2*C));
  print_generators(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(C);

  gr.fold_space_dimensions(to_fold, B);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point(A + 2*B));
  known_gr.add_grid_generator(grid_point(A + 4*B));

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, B) ***");

  return ok;
}

// Folding several dimensions into a higher dimension.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((A %= 2) / 3);
  gr.add_congruence((B %= 8) / 9);
  gr.add_congruence((C == 17) / 0);
  print_congruences(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(A);
  to_fold.insert(B);

  gr.fold_space_dimensions(to_fold, C);

  Grid known_gr(1);
  known_gr.add_congruence((A %= 2) / 3);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, C) ***");

  return ok;
}

// Folding dimensions into a lower dimension.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4);
  gr.add_congruence((A - B %= 3) / 4);
  gr.add_congruence((C %= 5) / 9);
  gr.add_congruence((D %= 2) / 6);
  print_congruences(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(C);
  to_fold.insert(D);

  gr.fold_space_dimensions(to_fold, A);

  Grid known_gr(2);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, A) ***");

  return ok;
}

// Folding dimensions into an intermediate dimension.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4);
  gr.add_congruence((A %= 0) / 2);
  gr.add_congruence((B %= 0) / 9);
  gr.add_congruence((C %= 0) / 6);
  gr.add_congruence((D %= 0) / 12);
  print_congruences(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(B);
  to_fold.insert(D);

  gr.fold_space_dimensions(to_fold, C);

  Grid known_gr(2);
  known_gr.add_congruence((A %= 0) / 2);
  known_gr.add_congruence((B %= 0) / 3);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, C) ***");

  return ok;
}

// Test folding dimensions of a relational grid into an intermediate
// dimension.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid gr(4);
  gr.add_congruence((A - B %= 0) / 9);
  gr.add_congruence((C %= 0) / 6);
  gr.add_congruence((D %= 0) / 12);
  print_congruences(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(B);
  to_fold.insert(D);

  gr.fold_space_dimensions(to_fold, C);

  Grid known_gr(2);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, C) ***");

  return ok;
}

// One dimension.
bool
test10() {
  Variable A(0);

  Grid gr(1);
  gr.add_congruence((A %= 3) / 7);
  print_congruences(gr, "*** gr ***");

  Grid known_gr = gr;

  Variables_Set to_fold;

  gr.fold_space_dimensions(to_fold, A);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, A) ***");

  return ok;
}

// Test folding dimensions of a relational grid into an intermediate
// dimension, where the resulting grid is smaller than the universe.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_constraint(A - B == 0);
  gr.add_congruence(A %= 0);
  gr.add_constraint(C == 0);
  print_congruences(gr, "*** gr ***");

  Variables_Set to_fold;
  to_fold.insert(A);

  gr.fold_space_dimensions(to_fold, C);

  Grid known_gr(2);
  known_gr.add_congruence(A %= 0);
  known_gr.add_congruence(B %= 0);

  bool ok = (gr == known_gr);
  print_congruences(gr, "*** gr.fold_space_dimensions(to_fold, C) ***");

  return ok;
}

// Second parameter of greater dimension than grid.
bool
test12() {
  Variable A(0);
  Variable B(1);

  Grid gr(1, EMPTY);

  Variables_Set vars;
  vars.insert(A);

  try {
    gr.fold_space_dimensions(vars, B);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Highest variable in set of greater dimension than grid.
bool
test13() {
  Variable B(1);

  Grid gr(3, EMPTY);

  Variables_Set vars;
  vars.insert(B);

  try {
    gr.fold_space_dimensions(vars, B);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Dimension of highest variable in set greater than dimension of
// grid.
bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr(1, EMPTY);

  Variables_Set vars;
  vars.insert(B);

  try {
    gr.fold_space_dimensions(vars, A);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
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
END_MAIN
