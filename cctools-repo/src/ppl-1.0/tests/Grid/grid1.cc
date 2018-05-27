/* Test reduction and conversion of grids created from generators.
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

// add_grid_generator, one variable.
bool
test01() {
  Variable A(0);

  Grid_Generator_System gs;
  gs.insert(grid_point(A));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generator(grid_point(2*A));

  Congruence_System known_cgs;
  known_cgs.insert(A %= 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr,
		    "*** gr.add_grid_generator(grid_point(2*A)) ***");

  return ok;
}

// add_grid_generator, two variables.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_point(A + B));

  Grid gr(gs);
  print_generators(gr, "*** gr ***");

  Grid_Generator g(grid_point(A + 2*B));
  gr.add_grid_generator(g);

  Congruence_System known_cgs;
  known_cgs.insert( 0*A + 0*B %= -1);
  known_cgs.insert((  A + 0*B %=  1) / 0);
  known_cgs.insert( 0*A + 1*B %=  1);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generator(g) ***");

  return ok;
}

// add_grid_generators
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid_Generator_System gs;
  gs.insert(grid_line(0*A +   B));
  gs.insert(grid_point(3*A + 4*B));
  gs.insert(grid_point(9*A + 0*B));

  Grid gr(2, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Grid_Generator_System known_gs;
  known_gs.insert(grid_line(0*A +   B));
  known_gs.insert(grid_point(3*A + 4*B));
  known_gs.insert(grid_point(9*A + 0*B));

  Grid known_gr(known_gs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(4*A -   B + 0*C, 3));
  gs.insert(grid_line(2*A + 3*B + 0*C));
  gs.insert(grid_point(4*A + 0*B + 0*C, 3));
  gs.insert(grid_point(4*A -   B +   C, 3));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert(( 0*A + 0*B + 0*C %= -2) / 2);
  known_cgs.insert((-9*A + 6*B + 0*C %= 14) / 2);
  known_cgs.insert(( 0*A - 0*B + 6*C %=  0) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(-1*A + 4*B + 3*C, 2));
  gs.insert(grid_line( 3*A + 2*B - 4*C));
  gs.insert(grid_line( 0*A + 0*B - 2*C));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert(0*A + 0*B + 0*C %= -1);
  known_cgs.insert((-2*A + 3*B + 0*C %= 7) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(-1*A + 4*B +  3*C, 2));
  gs.insert(grid_point( 2*A + 6*B -    C, 2));
  gs.insert(grid_point(-1*A + 9*B +  7*C, 2));
  gs.insert(grid_line( 0*A + 0*B -  2*C));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((  0*A + 0*B + 0*C %=  15) / 15);
  known_cgs.insert((-10*A + 0*B + 0*C %=   5) / 15);
  known_cgs.insert((  4*A - 6*B + 0*C %= -14) / 15);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(-1*A + 4*B + 3*C, 2));
  gs.insert(grid_line( 2*A +   B - 2*C));
  gs.insert(grid_point(-1*A + 9*B + 7*C, 2));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert(( 0*A + 0*B + 0*C %= -10) / 10);
  known_cgs.insert((-2*A + 4*B + 0*C %=   9) / 10);
  known_cgs.insert(( 7*A - 4*B + 5*C %=  -4) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(A));
  gs.insert(grid_point(2*A));
  gs.insert(grid_point(A + B));
  gs.insert(grid_point(A + C));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert(A %= 0);
  known_cgs.insert(B %= 0);
  known_cgs.insert(C %= 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(A +   B));
  gs.insert(grid_point(A + 2*B));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((A %= 1) / 0);
  known_cgs.insert(B %= 0);
  known_cgs.insert((C %= 0) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(A +   B + 0*C));
  gs.insert(grid_point(A + 2*B + 0*C));
  gs.insert(grid_point(A +   B +   C));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((A %=  1) / 0);
  known_cgs.insert(B %= 0);
  known_cgs.insert(C %= 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(0*A + 7*B + 0*C, 3));
  gs.insert(grid_line(3*A + 2*B + 0*C));
  gs.insert(grid_line(0*A + 0*B +   C));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert(( 0*A + 0*B + 0*C %= -1) / 1);
  known_cgs.insert((-2*A + 3*B + 0*C %=  7) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(-1*A + 0*B + 3*C, 4));
  gs.insert(grid_line( 3*A + 2*B + 0*C));
  gs.insert(grid_line( 0*A + 0*B +   C));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((-4*A + 6*B + 0*C %= 1) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

// Empty grid, one dimension.
bool
test13() {
  Grid gr(1, EMPTY);

  Grid known_gr(1, EMPTY);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr(1, EMPTY) ***");

  return ok;
}

// Empty grid, many dimensions.
bool
test14() {
  Grid gr(112, EMPTY);

  Grid known_gr(112, EMPTY);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr(112, EMPTY) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point( -A + 0*B + 3*C, 4));
  gs.insert(grid_line(0*A + 2*B + 0*C));
  gs.insert(grid_line(0*A + 4*B + 0*C));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((4*A + 0*B + 0*C %= -1) / 0);
  known_cgs.insert((0*A + 0*B + 4*C %=  3) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(   A));
  gs.insert(grid_point( 2*A +    B));
  gs.insert(grid_point(12*A + 11*B));
  gs.insert(grid_point(10*A + 12*B));
  gs.insert(grid_point( 2*A + 33*B, 2));
  gs.insert(grid_point( 4*A + 35*B, 2));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((C %= 0) / 0);
  known_cgs.insert((-2*A + 2*B %= 1) / 3);
  known_cgs.insert(( 3*A %= 0) / 3);
  known_cgs.insert(( 0*A %= 3) / 3);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(4*A -   B + 0*C, 3));
  gs.insert(grid_line(2*A + 3*B));
  gs.insert(grid_point(4*A            , 3));
  gs.insert(grid_point(4*A -   B +   C, 3));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((-9*A + 6*B + 0*C %= 0) / 2);
  known_cgs.insert((             6*C %= 0) / 2);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

// Universe grid, one dimension.
bool
test18() {
  Grid gr(1);

  Grid known_gr(1);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr(1) ***");

  return ok;
}

// Universe grid, many dimensions.
bool
test19() {
  Grid gr(21);

  Grid known_gr(21);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(21) ***");

  return ok;
}

// Universe grid, zero dimensions.
bool
test20() {
  Grid gr(0);

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr(0) ***");

  return ok;
}

// Using large values for the coordinates
bool
test21() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(-9933*A + 2000*B + 3953*C, 9113));
  gs.insert(grid_point(    0*A +    0*B + 8888*C, 7302));
  gs.insert(grid_point(   29*A +   23*B + 1111*C, 1010));
  gs.insert(grid_point( 2394*A + 7273*B +    0*C,   30));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;

  // Create coefficients with string constructors as they're too big
  // for the long type.

  // 37315344498526  0  0  0  congruence, modulus = 37315344498526
  // 0  343455281759218112380  0  0  congruence, modulus = 37315344498526
  // 0  -133815138923073144612  223892066991156  0  congruence, modulus = 37315344498526
  // -22220  -31385495955559489171  93798931757298  18255  congruence, modulus = 37315344498526

  Coefficient* tem1 = new Coefficient("37315344498526");
  known_cgs.insert((     0*A +     0*B +     0*C %= -*tem1) / *tem1);

  Coefficient* tem2 = new Coefficient("343455281759218112380");
  known_cgs.insert(( *tem2*A +     0*B +     0*C %= 0) / *tem1);
  delete tem2;

  tem2 = new Coefficient("-133815138923073144612");
  Coefficient* tem3 = new Coefficient("223892066991156");
  known_cgs.insert(( *tem2*A + *tem3*B +     0*C %= 0) / *tem1);
  delete tem2; delete tem3;

  tem2 = new Coefficient("-31385495955559489171");
  tem3 = new Coefficient("93798931757298");
  known_cgs.insert(( *tem2*A + *tem3*B + 18255*C %= 22220) / *tem1);
  delete tem1; delete tem2; delete tem3;


  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  //print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

// Using large values for the coefficients.
bool
test22() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(-93*A +   0*B +  39*C, 113));
  gs.insert(grid_line( 29*A +  23*B + 111*C));
  gs.insert(grid_point(117*A + 200*B +  88*C, 33));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((       0*A +       0*B +      0*C %=  280730) / 280730);
  known_cgs.insert((  -85767*A +  108141*B +      0*C %=   70587) / 280730);
  known_cgs.insert((-2309489*A + 1557137*B + 280730*C %= 1997619) / 0);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test23() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid_Generator_System gs;
  gs.insert(grid_point( 3*A +   B + 0*C, 4));
  gs.insert(grid_point(11*A + 2*B + 0*C, 4));
  gs.insert(grid_point( 3*A + 6*B + 0*C, 4));
  gs.insert(grid_point( 3*A +   B + 2*C, 4));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert(( 0*A +  0*B +  0*C %= -40) / 40);
  known_cgs.insert((20*A +  0*B +  0*C %=  15) / 40);
  known_cgs.insert((-4*A + 32*B +  0*C %=   5) / 40);
  known_cgs.insert(( 0*A +  0*B + 80*C %=   0) / 40);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

bool
test24() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid_Generator_System gs;
  gs.insert(grid_point( 3*A +   B + 0*C, 4));
  gs.insert(grid_point(11*A + 2*B + 0*C, 4));
  gs.insert(grid_point( 3*A + 6*B + 0*C, 4));
  gs.insert(grid_point( 3*A +   B + 2*C, 4));

  Grid gr(3, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((20*A +  0*B        %= 15) / 40);
  known_cgs.insert((-4*A + 32*B        %=  5) / 40);
  known_cgs.insert((              80*C %=  0) / 40);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

// add_grid_generators, with more rows than columns.
bool
test25() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid_Generator_System gs;
  gs.insert(grid_point(3*A + 7*B - 2*C + 3*D));
  gs.insert(grid_point(0*A + 0*B +   C +   D));
  gs.insert(grid_point(3*A + 4*B + 2*C + 0*D));
  gs.insert(grid_point(3*A + 2*B +   C + 2*D));
  gs.insert(grid_point(9*A + 0*B + 4*C +   D));

  Grid gr(4, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((  9*A +   0*B +  0*C + 0*D %=   0) / 27);
  known_cgs.insert((-18*A +  27*B +  0*C + 0*D %=   0) / 27);
  known_cgs.insert((-90*A + 135*B + 27*C + 0*D %=  27) / 27);
  known_cgs.insert((-17*A +  25*B +  6*C +   D %=   7) / 27);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

// Example from Muller-Olm and Seidl SAS 2005 paper
bool
test26() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Grid_Generator_System gs;
  gs.insert(grid_point(2*A + 0*B));
  gs.insert(grid_point(30*A + 36*B));
  gs.insert(grid_point(450*A + 564*B));

  Grid gr(2, EMPTY);
  print_generators(gr, "*** gr ***");

  gr.add_grid_generators(gs);

  Congruence_System known_cgs;
  known_cgs.insert((A %= 2) / 28);
  known_cgs.insert((B %= 0) / 12);

  Grid known_gr(known_cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.add_grid_generators(gs) ***");

  return ok;
}

// A generator system with only a line.
bool
test27() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_line(0*A + 2*B + 0*C));

  try {
    Grid gr(gs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// A generator system containing a parameter.
bool
test28() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid_Generator_System gs;
  gs.insert(grid_point(0*C));
  gs.insert(grid_line(A));
  gs.insert(grid_line(B));
  gs.insert(parameter(-C));

  Grid gr(gs);

  Grid known_gr(3);
  known_gr.add_congruence(C %= 0);

  bool ok = (gr == known_gr);

  print_generators(gr, "*** gr(gs) ***");

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
  DO_TEST_F64(test21);
  DO_TEST_F32(test22);
  DO_TEST_F8(test23);
  DO_TEST_F8(test24);
  DO_TEST_F8(test25);
  DO_TEST_F8(test26);
  DO_TEST(test27);
  DO_TEST(test28);
END_MAIN
