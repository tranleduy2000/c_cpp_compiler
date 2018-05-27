/* Test Grid::relation_with(const Constraint&).
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

// A point and an inequality.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(A - B));
  print_generators(gr, "*** gr ***");

  bool ok = gr.relation_with(A - B >= 1) == Poly_Con_Relation::is_included();

  return ok;
}

// A line.
bool
test02() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_line(A));
  print_generators(gr, "*** gr ***");

  using namespace Parma_Polyhedra_Library::IO_Operators;

  bool ok = (gr.relation_with(A + 0*B == 0)
	     == Poly_Con_Relation::strictly_intersects()
	     && gr.relation_with(B > -2)
	     == Poly_Con_Relation::is_included()
 	     && gr.relation_with(B > 2)
 	     == Poly_Con_Relation::is_disjoint()
 	     && gr.relation_with(B >= 2)
 	     == Poly_Con_Relation::is_disjoint()
 	     && gr.relation_with(B < -2)
 	     == Poly_Con_Relation::is_disjoint()
 	     && gr.relation_with(B <= -2)
 	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(B == -2)
	     == Poly_Con_Relation::is_disjoint());

  return ok;
}

// Inclusion of a point grid.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(A + B));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(B == 1)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::saturates()));

  return ok;
}

// Empty grid.

bool
test04() {
  Variable B(1);

  Grid gr(2, EMPTY);
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(B == 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates())
	     && gr.relation_with(B > 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates()));

  return ok;
}

// Zero dimension universe grid.
bool
test05() {
  Grid gr;
  print_generators(gr, "*** gr ***");

  bool ok = (// False.
	     gr.relation_with(Linear_Expression(1) == 0)
	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(Linear_Expression(0) > 0)
	     == (Poly_Con_Relation::saturates()
		 && Poly_Con_Relation::is_disjoint())
	     // True.
	     && gr.relation_with(Linear_Expression(1) == 1)
	     == (Poly_Con_Relation::saturates()
		 && Poly_Con_Relation::is_included())
	     && gr.relation_with(Linear_Expression(0) >= 0)
	     == (Poly_Con_Relation::saturates()
		 && Poly_Con_Relation::is_included())
	     // False.
	     && gr.relation_with(Linear_Expression(1) < 0)
	     == Poly_Con_Relation::is_disjoint()
	     // True.
	     && gr.relation_with(Linear_Expression(1) >= 0)
	     == Poly_Con_Relation::is_included());

  return ok;
}

// A disjoint grid.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(2*A + 5*B));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(5*A - 2*B == 1)
	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(5*A - 2*B > 1)
	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(5*A - 2*B >= 1)
	     == Poly_Con_Relation::is_disjoint());

  return ok;
}

// Point with a divisor that is greater than zero.
bool
test07() {
  Variable A(0);

  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point(A, 2));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(A == 3)
	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(2*A == 1)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::saturates())
	     && gr.relation_with(2*A < 1)
	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(2*A >= 1)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::saturates()));

  return ok;
}

// Grid with a divisor that is greater than zero: separate spaces.
bool
test08() {
  Variable A(0);

  Grid gr(1, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(parameter(A, 5));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(10*A == 1)
	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(Linear_Expression(10) > 10)
	     == Poly_Con_Relation::is_disjoint());

  return ok;
}

// Grid with a divisor that is greater than zero: inclusion.
bool
test09() {
  Variable A(0);

  Grid gr(1, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(parameter(A, 5));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(Linear_Expression(10) == 10)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::saturates())
	     && gr.relation_with(Linear_Expression(10) >= 10)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::saturates()));

  return ok;
}

// Grid with a divisor that is greater than zero: strict intersection.
bool
test10() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(parameter(A, 5));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(A + B == 0)
	     == Poly_Con_Relation::strictly_intersects()
	     && gr.relation_with(A + B >= 0)
	     == Poly_Con_Relation::strictly_intersects());

  return ok;
}

// Space dimension exception.
bool
test11() {
  Variable A(0);
  Variable B(1);

  Grid gr(1);
  print_generators(gr, "*** gr ***");

  try {
    gr.relation_with(A + B >= 0);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Empty grid, where updating finds the grid empty.
bool
test12() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_constraint(A == 1);
  gr.add_constraint(A == 2);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(B == 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates())
	     && gr.relation_with(B >= 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates()));

  return ok;
}

// Generators that require the relation_with(cg) GCD calculation.
bool
test13() {
  Variable A(0);

  Grid gr(1, EMPTY);
  gr.add_grid_generator(grid_point(A));
  gr.add_grid_generator(grid_point(3*A));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(A == 0)
	     == Poly_Con_Relation::is_disjoint());

  return ok;
}

// Strict intersection, where generators require the relation_with(cg)
// GCD calculation.
bool
test14() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(3*A));
  gr.add_grid_generator(grid_point(6*A));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(A + B == 0)
	     == Poly_Con_Relation::strictly_intersects());

  return ok;
}

// Strict intersection, where generators require the relation_with(cg)
// GCD calculation, with a parameter.
bool
test15() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(3*A));
  gr.add_grid_generator(parameter(3*A));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(A + B == 0)
	     == Poly_Con_Relation::strictly_intersects());

  return ok;
}

// generators are out of date and the grid is empty.
bool
test16() {
  Variable A(0);
  Variable B(1);

  Grid gr(2);
  gr.add_congruence((A %= 0) / 2);
  gr.add_congruence((A %= 1) / 2);
  print_congruences(gr, "*** gr ***");

  bool ok = (gr.relation_with(A + B == 8)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates())
	     && gr.relation_with(A + B > 8)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates()));

  return ok;
}

// A simple example.
bool
test17() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point(B));
  gr.add_grid_generator(parameter(A));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(A > 1)
	     == Poly_Con_Relation::strictly_intersects()
	     && gr.relation_with(A >= 1)
	     == Poly_Con_Relation::strictly_intersects()
	     && gr.relation_with(A < 1)
	     == Poly_Con_Relation::strictly_intersects()
	     && gr.relation_with(A <= 1)
	     == Poly_Con_Relation::strictly_intersects());

  return ok;
}

// A grid in 3D.
bool
test18() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  // Regularly spaced parallel lines along a slanted plane in 3D.
  Grid gr(3, EMPTY);
  gr.add_grid_generator(grid_point(A + B + C));
  gr.add_grid_generator(grid_line(A - 2*B + 3*C));
  gr.add_grid_generator(parameter(A - B, 3));
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(A + B + C == 0)
	     == Poly_Con_Relation::strictly_intersects()
	     && gr.relation_with(A + B == 0)
	     == Poly_Con_Relation::strictly_intersects()
	     && gr.relation_with(A == 0)
	     == Poly_Con_Relation::strictly_intersects()
	     && gr.relation_with(Linear_Expression(0) == 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::saturates()));

  return ok;
}

// Zero dimension empty grid.
bool
test19() {
  Grid gr(0, EMPTY);
  print_generators(gr, "*** gr ***");

  bool ok = (// False.
	     gr.relation_with(Linear_Expression(1) == 0)
	     == (Poly_Con_Relation::is_disjoint()
		 && Poly_Con_Relation::is_included()
	         && Poly_Con_Relation::saturates())
	     // True.
	     && gr.relation_with(Linear_Expression(1) == 1)
	     == (Poly_Con_Relation::is_disjoint()
		 && Poly_Con_Relation::is_included()
	         && Poly_Con_Relation::saturates())
	     // False.
	     && gr.relation_with(Linear_Expression(1) < 0)
	     == (Poly_Con_Relation::is_disjoint()
		 && Poly_Con_Relation::is_included()
	         && Poly_Con_Relation::saturates())
	     // True.
	     && gr.relation_with(Linear_Expression(1) >= 0)
	     == (Poly_Con_Relation::is_disjoint()
		 && Poly_Con_Relation::is_included()
	         && Poly_Con_Relation::saturates()));

  return ok;
}

// A variant of test06.
bool
test20() {
  Variable A(0);
  Variable B(1);

  Grid gr(2, EMPTY);
  gr.add_grid_generator(grid_point());
  gr.add_grid_generator(grid_point(2*A + 5*B));
  // Force minimization.
  (void) gr.minimized_grid_generators();
  print_generators(gr, "*** gr ***");

  bool ok = (gr.relation_with(5*A - 2*B == 1)
	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(5*A - 2*B > 1)
	     == Poly_Con_Relation::is_disjoint()
	     && gr.relation_with(5*A - 2*B >= 1)
	     == Poly_Con_Relation::is_disjoint());

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
