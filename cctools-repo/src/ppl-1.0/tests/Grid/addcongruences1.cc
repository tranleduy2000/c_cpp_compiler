/* Test methods which can add multiple congruences to a grid.
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

using namespace Parma_Polyhedra_Library::IO_Operators;

namespace {

// add_congruences
bool
test01() {

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 2);
  cgs.insert((B == 0) / 2);

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.add_congruences(cgs);

  Grid known_gr(2);
  known_gr.add_congruence((A %= 0) / 2);
  known_gr.add_congruence((B == 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_congruences(cgs) ***");

  return ok;
}

// add_recycled_congruences
bool
test02() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A + B %= 0) / 2);

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.add_recycled_congruences(cgs);

  Grid known_gr(2);
  known_gr.add_congruence((A + B %= 0) / 2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_recycled_congruences(cgs) ***");

  return ok;
}

// add_recycled_congruences(cgs) -- space dimension exception
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + B %= 0) / 2);

  Grid gr(1);

  try {
    gr.add_recycled_congruences(cgs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// add_recycled_congruences, empty grid.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A + B %= 0) / 2);

  Grid gr(2, EMPTY);

  print_congruences(gr, "*** gr ***");

  gr.add_recycled_congruences(cgs);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** add_recycled_congruences(cgs) ***");

  return ok;
}

// add_recycled_congruences, add system of single trivial
// congruence to zero dim grid.
bool
test05() {

  Grid gr(0);

  print_congruences(gr, "*** gr ***");

  Grid known_gr = gr;

  Congruence_System cgs;
  cgs.insert(Congruence::zero_dim_integrality());

  gr.add_recycled_congruences(cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.add_recycled_congruences(cgs) ***");

  return ok;
}

// add_congruences(cgs) -- space dimension exception
bool
test06() {
  Variable B(1);

  Congruence_System cgs;
  cgs.insert(B == 0);

  Grid gr(1);

  try {
    gr.add_congruences(cgs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// add_recycled_congruences, add to empty grid.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);

  print_congruences(gr, "*** gr ***");

  Grid known_gr = gr;

  Congruence_System cgs;
  cgs.insert(A + B == 0);

  gr.add_recycled_congruences(cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.add_recycled_congruences(cgs) ***");

  return ok;
}

// add_recycled_congruences, add empty system to grid
// with minimized generators and up to date congruences.
bool
test08() {
  Variable A(0);

  Grid gr(2);

  // Ensure both systems are up to date with only generators minimal.
  gr.affine_image(A, 1*A);
  gr.minimized_grid_generators();

  print_congruences(gr, "*** gr ***");

  Congruence_System cgs;

  gr.add_recycled_congruences(cgs);

  Grid known_gr(2);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.add_recycled_congruences(cgs) ***");

  return ok;
}

// add_recycled_congruences, add empty system to grid
// with up to date congruences and generators.
bool
test09() {
  Variable A(0);

  Grid gr(2);

  // Ensure both systems are just up to date.
  gr.affine_image(A, 1*A);

  print_congruences(gr, "*** gr ***");

  Congruence_System cgs;

  gr.add_recycled_congruences(cgs);

  Grid known_gr(2);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.add_recycled_congruences(cgs) ***");

  return ok;
}

// add_recycled_congruences, add congruence in 1D to
// universe grid in 2D.
bool
test10() {
  Variable A(0);
  Variable B(1);

  Grid gr1(2);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 7);

  gr1.add_recycled_congruences(cgs);
  print_generators(gr1, "*** gr1 ***");
  print_congruences(gr1, "*** gr1 ***");

  Grid gr2(2);
  gr2.add_congruence((A %= 0) / 7);
  print_generators(gr2, "*** gr2 ***");
  print_congruences(gr2, "*** gr2 ***");

  Grid known_gr = gr2;

  bool ok = (gr1 == known_gr);

  print_congruences(gr1, "*** gr1.add_recycled_congruences(cgs) ***");

  return ok;
}

// add_recycled_congruences, add empty system to grid
// with minimized congruences and up to date generators.
bool
test11() {
  Variable A(0);

  Grid gr(2);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(parameter(3*A));

  // Ensure both systems are up to date with only congruences minimal.
  gr.affine_image(A, 1*A);
  gr.minimized_congruences();

  print_congruences(gr, "*** gr ***");

  Congruence_System cgs;

  gr.add_recycled_congruences(cgs);

  Grid known_gr(2);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.add_recycled_congruences(cgs) ***");

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
END_MAIN
