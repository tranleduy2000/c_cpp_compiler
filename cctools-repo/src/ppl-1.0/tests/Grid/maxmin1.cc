/* Test Grid::maximize(const Linear_Expression&, ...)
   and Grid::minimize(const Linear_Expression&, ...).
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
#include <string>

using namespace Parma_Polyhedra_Library::IO_Operators;

namespace {

bool
check_both(Grid& gr, const Linear_Expression& le,
	   const std::string grid_name) {
  Generator ext_pnt(point());
  bool dummy;
  Coefficient ext_n, ext_d;

  bool ok = (!gr.maximize(le, ext_n, ext_d, dummy, ext_pnt)
	     && !gr.minimize(le, ext_n, ext_d, dummy, ext_pnt));

  if (!ok)
    nout << grid_name << " bounded expr" << endl;

  return ok;
}

bool
check_minimize(Grid& gr, const Linear_Expression& le,
	       Coefficient_traits::const_reference expected_n,
	       Coefficient_traits::const_reference expected_d,
	       const Generator& expected_pnt,
	       const std::string grid_name) {
  Generator inf_pnt(point());

  bool dummy;
  Coefficient inf_n, inf_d;

  bool ok = (gr.minimize(le, inf_n, inf_d, dummy, inf_pnt)
	     && inf_n == expected_n
	     && inf_d == expected_d
	     && inf_pnt == expected_pnt);

  if (!ok)
    nout << "grid name " << grid_name << " min point " << inf_pnt
	 << " (expected " << expected_pnt << ")" << endl;

  return ok;
}

bool
check_maximize(Grid& gr, const Linear_Expression& le,
	       Coefficient_traits::const_reference expected_n,
	       Coefficient_traits::const_reference expected_d,
	       const Generator& expected_pnt,
	       const std::string grid_name) {
  Generator sup_pnt(point());

  bool dummy;
  Coefficient sup_n, sup_d;

  bool ok = (gr.maximize(le, sup_n, sup_d, dummy, sup_pnt)
	     && sup_n == expected_n
	     && sup_d == expected_d
	     && sup_pnt == expected_pnt);

  if (!ok)
    nout << "grid name " << grid_name << " max point " << sup_pnt
	 << " (expected " << expected_pnt << ")" << endl;

  return ok;
}

// Empty.
bool
test01() {
  Grid gr(7, EMPTY);

  Coefficient extr_n, extr_d;
  bool dummy;
  Generator pnt(point());

  bool ok = (!gr.maximize(Linear_Expression(0), extr_n, extr_d, dummy, pnt)
	     && !gr.minimize(Linear_Expression(0), extr_n, extr_d, dummy, pnt));
  return ok;
}

// Zero dimension empty.
bool
test02() {
  Grid gr(0, EMPTY);
  print_congruences(gr, "*** gr ***");

  Linear_Expression le = Linear_Expression::zero();

  return check_both(gr, le, "gr");
}

// Zero dimension universe.
bool
test03() {
  Grid gr(0);
  print_congruences(gr, "*** gr ***");

  Linear_Expression le = Linear_Expression::zero();

  Generator exp_pnt(point());

  bool ok = check_maximize(gr, le, 0, 1, exp_pnt, "gr")
    && check_minimize(gr, le, 0, 1, exp_pnt, "gr");

  return ok;
}

// Point.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point(3*A + 2*B, 3));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point(3*A + 2*B, 3));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(A == 1);
  gr_cgs_needs_min.add_constraint(3*B == 2);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = A + B;

  Generator exp_pnt(point(3*A + 2*B, 3));

  bool ok = check_maximize(gr_gs_min, le, 5, 3, exp_pnt, "gr_gs_min")
    && check_minimize(gr_gs_min, le, 5, 3, exp_pnt, "gr_gs_min");

  ok &= check_maximize(
        gr_gs_needs_min, le, 5, 3, exp_pnt, "gr_gs_needs_min")
    && check_minimize(gr_gs_needs_min, le, 5, 3, exp_pnt, "gr_gs_needs_min");

  ok &= check_maximize(
        gr_cgs_needs_min, le, 5, 3, exp_pnt, "gr_cgs_needs_min")
    && check_minimize(gr_cgs_needs_min, le, 5, 3, exp_pnt, "gr_cgs_needs_min");

  return ok;
}

// Rectilinear line.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(B));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(B));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(A == 0);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A - B;

  bool ok = check_both(gr_gs_min, le, "gr_gs_min")
    && check_both(gr_gs_needs_min, le, "gr_gs_needs_min")
    && check_both(gr_cgs_needs_min, le, "gr_cgs_needs_min");

  return ok;
}

// Line.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(2*A + B));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(2*A + B));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(A - 2*B == 0);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A + B;

  bool ok = check_both(gr_gs_min, le, "gr_gs_min")
    && check_both(gr_gs_needs_min, le, "gr_gs_needs_min")
    && check_both(gr_cgs_needs_min, le, "gr_cgs_needs_min");

  return ok;
}

// A line along the equality `expr == 0'.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(A + 2*B));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(A + 2*B));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(2*A - B == 0);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A - B;

  Generator exp_pnt(point(0*B));

  bool ok = check_maximize(gr_gs_min, le, 0, 1, exp_pnt, "gr_gs_min")
    && check_minimize(gr_gs_min, le, 0, 1, exp_pnt, "gr_gs_min");

  ok &= check_maximize(gr_gs_needs_min, le, 0, 1, exp_pnt, "gr_gs_needs_min")
    && check_minimize(gr_gs_needs_min, le, 0, 1, exp_pnt, "gr_gs_needs_min");

  ok &= check_maximize(
          gr_cgs_needs_min, le, 0, 1, exp_pnt, "gr_cgs_needs_min")
    && check_minimize(
          gr_cgs_needs_min, le, 0, 1, exp_pnt, "gr_cgs_needs_min");

  return ok;
}

// A parameter along the equality `expr == 0'.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_point(A + 2*B));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_point(A + 2*B));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(2);
  gr_cgs_needs_min.add_constraint(2*A - B == 0);
  gr_cgs_needs_min.add_congruence((B %= 0) / 2);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A - B;

  Generator exp_pnt(point(0*B));

  bool ok = check_maximize(gr_gs_min, le, 0, 1, exp_pnt, "gr_gs_min")
    && check_minimize(gr_gs_min, le, 0, 1, exp_pnt, "gr_gs_min");

  ok &= check_maximize(gr_gs_needs_min, le, 0, 1, exp_pnt, "gr_gs_needs_min")
    && check_minimize(gr_gs_needs_min, le, 0, 1, exp_pnt, "gr_gs_needs_min");

  ok &= check_maximize(
          gr_cgs_needs_min, le, 0, 1, exp_pnt, "gr_cgs_needs_min")
    && check_minimize(
          gr_cgs_needs_min, le, 0, 1, exp_pnt, "gr_cgs_needs_min");

  return ok;
}

// Two lines which combine to cover any (affine) line defined by expr.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Grid gr_gs_min(2, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(A));
  gr_gs_min.add_grid_generator(grid_line(B));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(2, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(A));
  gr_gs_needs_min.add_grid_generator(grid_line(B));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(2);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = A - B;

  bool ok = check_both(gr_gs_min, le, "gr_gs_min")
    && check_both(gr_gs_needs_min, le, "gr_gs_needs_min")
    && check_both(gr_cgs_needs_min, le, "gr_cgs_needs_min");

  return ok;
}

// In three dimensions, lines and parameters which combine to include
// expr.
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr_gs_min(3, EMPTY);
  gr_gs_min.add_grid_generator(grid_point());
  gr_gs_min.add_grid_generator(grid_line(A));
  gr_gs_min.add_grid_generator(grid_point(B + C));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(3, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point());
  gr_gs_needs_min.add_grid_generator(grid_line(A));
  gr_gs_needs_min.add_grid_generator(grid_point(B + C));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(3);
  gr_cgs_needs_min.add_constraint(B - C == 0);
  gr_cgs_needs_min.add_congruence(B %= 0);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A + B - C;

  bool ok = check_both(gr_gs_min, le, "gr_gs_min")
    && check_both(gr_gs_needs_min, le, "gr_gs_needs_min")
    && check_both(gr_cgs_needs_min, le, "gr_cgs_needs_min");

  return ok;
}

// Grid which bounds a 3D expr.
bool
test11() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Grid gr_gs_min(3, EMPTY);
  gr_gs_min.add_grid_generator(grid_point(A));
  gr_gs_min.add_grid_generator(grid_line(3*B + C));
  gr_gs_min.add_grid_generator(grid_line(A - 2*B));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(3, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point(A));
  gr_gs_needs_min.add_grid_generator(grid_line(3*B + C));
  gr_gs_needs_min.add_grid_generator(grid_line(A - 2*B));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(3);
  gr_cgs_needs_min.add_constraint(2*A + B - 3*C - 2 == 0);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = 2*A + B - 3*C;

  Generator exp_pnt1(point(2*B + 0*C));

  bool ok = check_maximize(gr_gs_min, le, 2, 1, exp_pnt1, "gr_gs_min")
    && check_minimize(gr_gs_min, le, 2, 1, exp_pnt1, "gr_gs_min");

  ok &= check_maximize(gr_gs_needs_min, le, 2, 1, exp_pnt1, "gr_gs_needs_min")
    &&check_minimize(gr_gs_needs_min, le, 2, 1, exp_pnt1, "gr_gs_needs_min");

  Generator exp_pnt2(point(-2*C, 3));

  ok &= check_maximize(
          gr_cgs_needs_min, le, 2, 1, exp_pnt2, "gr_cgs_needs_min")
    && check_minimize(
          gr_cgs_needs_min, le, 2, 1, exp_pnt2, "gr_cgs_needs_min");

  return ok;
}

// Point in 6D.
bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);

  Grid gr_gs_min(6, EMPTY);
  gr_gs_min.add_grid_generator(grid_point(7*A - 11*B + 19*F));
  print_generators(gr_gs_min, "*** gr_gs_min ***");

  Grid gr_gs_needs_min(6, EMPTY);
  gr_gs_needs_min.add_grid_generator(grid_point(7*A - 11*B + 19*F));
  print_generators(gr_gs_needs_min, "*** gr_gs_needs_min ***");

  Grid gr_cgs_needs_min(6);
  gr_cgs_needs_min.add_constraint(A == 7);
  gr_cgs_needs_min.add_constraint(B == -11);
  gr_cgs_needs_min.add_constraint(C == 0);
  gr_cgs_needs_min.add_constraint(D == 0);
  gr_cgs_needs_min.add_constraint(E == 0);
  gr_cgs_needs_min.add_constraint(F == 19);
  print_congruences(gr_cgs_needs_min, "*** gr_cgs_needs_min ***");

  // Grids gr_gs_min, gr_gs_needs_min and gr_cgs_needs_min are the
  // same grids.

  Linear_Expression le = A + 2*B + 3*C + 4*D + 6*F;

  Generator exp_pnt(point(7*A - 11*B + 19*F));

  bool ok = check_maximize(gr_gs_min, le, 99, 1, exp_pnt, "gr_gs_min")
    && check_minimize(gr_gs_min, le, 99, 1, exp_pnt, "gr_gs_min");

  ok &= check_maximize(gr_gs_needs_min, le, 99, 1, exp_pnt, "gr_gs_needs_min")
    &&check_minimize(gr_gs_needs_min, le, 99, 1, exp_pnt, "gr_gs_needs_min");

  ok &= check_maximize(
          gr_cgs_needs_min, le, 99, 1, exp_pnt, "gr_cgs_needs_min")
    && check_minimize(
          gr_cgs_needs_min, le, 99, 1, exp_pnt, "gr_cgs_needs_min");

  return ok;
}

// Unbounded grid in 1D where the generator system is up-to-date but
// not minimized.
bool
test13() {
  Variable A(0);

  Grid gr(1, EMPTY);
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(A, 2));
  print_generators(gr, "*** gr generators before ***");

  bool ok = check_both(gr, A, "gr");
  print_generators(gr, "*** gr generators after ***");

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
END_MAIN
