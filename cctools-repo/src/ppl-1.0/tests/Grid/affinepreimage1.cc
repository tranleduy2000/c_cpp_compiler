/* Test Grid::affine_preimage().
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

// The first twelve tests mirror those in affineimage1.cc.

// Grid defined by generators.
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point(2*B - 2*C));

  print_generators(gr, "*** gr ***");

  gr.affine_preimage(B, A + 2, 1);

  Grid known_gr(3);
  known_gr.add_congruence((C == -2) / 0);
  known_gr.add_congruence((A ==  0) / 0);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_preimage(B, A + 2, 1) ***");

  return ok;
}

// Grid defined by congruences.
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3);
  gr.add_congruence((A %= 1) / 6);
  gr.add_congruence((3*A - 6*B %= 3) / 0);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, A + B + 1);

  Grid known_gr(3);
  known_gr.add_congruence((A - B %= 0) / 0);
  known_gr.add_congruence((A %= 0) / 3);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, A + B + 1) ***");

  return ok;
}

// Denominator.
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point(2*A + 2*B));
  gr.add_grid_generator(grid_line(C));

  print_generators(gr, "*** gr ***");

  gr.affine_preimage(A, A + 1, 2);

  Grid known_gr(3);
  known_gr.add_congruence((A %= 3) / 0);
  known_gr.add_congruence((B %= 2) / 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, A + 1, 2) ***");

  return ok;
}

// Invertible transformation with denominator, modulus and up-to-date
// congruences.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr(3);
  gr.add_congruence((B %= 2) / 0);
  gr.add_congruence((3*A %= 4) / 5);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, A + 1, 3);

  Grid known_gr(3);
  known_gr.add_congruence((A %= 3) / 5);
  known_gr.add_congruence((B %= 2) / 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, A + 1, 3) ***");

  return ok;
}

// Simple invertible transformation with denominator and modulus.
// Congruences are out-of-date.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point(4*A + 6*B, 3));
  gr.add_grid_generator(grid_point(9*A + 6*B, 3));
  gr.add_grid_generator(grid_line(C));

  print_generators(gr, "*** gr ***");

  gr.affine_preimage(A, A + 1, 3);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(3*A + 2*B));
  known_gr.add_grid_generator(grid_point(8*A + 2*B));
  known_gr.add_grid_generator(grid_line(C));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_preimage(A, A + 1, 3) ***");

  return ok;
}

// Simple invertible transformation with denominator and modulus.
// Congruences are up-to-date.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr = Grid(3);
  gr.add_congruence((3*A %= 4) / 5);
  gr.add_congruence((B %= 2) / 0);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, A + 1, 3);

  Grid known_gr(3, EMPTY);
  known_gr.add_grid_generator(grid_point(3*A + 2*B));
  known_gr.add_grid_generator(grid_point(8*A + 2*B));
  known_gr.add_grid_generator(grid_line(C));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, A + 1, 3) ***");

  return ok;
}

// Invertible transformation which changes the modulus.
// Congruences are out-of-date.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(4*A));
  gr.add_grid_generator(grid_point(7*A));
  gr.add_grid_generator(grid_point(6*A + B));

  print_generators(gr, "*** gr ***");

  gr.affine_preimage(A, 3*A + 2*B + 4);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_point(B));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_preimage(A, 3*A + 2*B + 4) ***");

  return ok;
}

// Invertible transformation which changes the modulus.
// Congruences are up-to-date.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr = Grid(2);
  gr.add_congruence((A - 2*B %= 4) / 3);
  gr.add_congruence(B %= 0);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, 3*A + 2*B + 4);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));
  known_gr.add_grid_generator(grid_point(B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, 3*A + 2*B + 4) ***");

  return ok;
}

// One dimension.
// Congruences are out-of-date.
bool
test09() {
  Variable A(0);

  Grid gr(1, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(2*A));

  print_generators(gr, "*** gr ***");

  gr.affine_preimage(A, 2*A);

  Grid known_gr(1, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_preimage(A, 2*A) ***");

  return ok;
}

// One dimension.
// Congruences are up-to-date.
bool
test10() {
  Variable A(0);

  Grid gr = Grid(1);
  gr.add_congruence((A %= 0) / 2);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, 2*A);

  Grid known_gr(1, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(A));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, 2*A) ***");

  return ok;
}

// The first affine_image example described at anchor
// grid_affine_transformation in definitions.dox.
// Congruences are out-of-date.
bool
test11() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(7*A + 3*B));
  gr.add_grid_generator(grid_point(10*A));

  print_generators(gr, "*** gr ***");

  gr.affine_preimage(A, 3*A + 2*B + 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(3*A));
  known_gr.add_grid_generator(grid_point(3*B));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr.affine_preimage(A, 3*A + 2*B + 1) ***");

  return ok;
}

// The first affine_image example described at anchor
// grid_affine_transformation in definitions.dox.
// Congruences are up-to-date.
bool
test12() {
  Variable A(0);
  Variable B(1);

  Grid gr = Grid(2);

  gr.add_congruence((3*A - 6*B %= 3) / 9);
  gr.add_congruence((A - 2*B %= 1) / 9);
  gr.add_congruence((B %= 0) / 3);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, 3*A + 2*B + 1);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_point(3*A));
  known_gr.add_grid_generator(grid_point(3*B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, 3*A + 2*B + 1) ***");

  return ok;
}

// The second affine_image example described at anchor
// grid_affine_transformation in definitions.dox.
// Congruences are out-of-date.
bool
test13() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(3*A + 3*B));

  print_generators(gr, "*** gr ***");

  gr.affine_preimage(A, B);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_line(A));
  known_gr.add_grid_generator(grid_point(3*B));

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr ***");

  return ok;
}

// The second affine_image example described at anchor
// grid_affine_transformation in definitions.dox.
// Congruences are up-to-date.
bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr = Grid(2);
  gr.add_congruence((A - B == 0) / 0);
  gr.add_congruence((A %= 0) / 3);

  print_congruences(gr, "*** gr ***");

  gr.affine_preimage(A, B);

  Grid known_gr(2, EMPTY);
  known_gr.add_grid_generator(grid_point());
  known_gr.add_grid_generator(grid_line(A));
  known_gr.add_grid_generator(grid_point(3*B));

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.affine_preimage(A, B) ***");

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
END_MAIN
