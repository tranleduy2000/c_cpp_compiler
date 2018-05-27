/* Test construction of grids from polyhedron.
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

// Grid(ph) - non-empty polyhedron
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(B == 0);
  cs.insert(A >= 0);
  cs.insert(C > 0);

  NNC_Polyhedron ph(cs);
  Grid gr(ph);

  Grid known_gr(3);
  known_gr.add_constraint(B == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");

  return ok;
}

// Grid(ph) - empty polyhedron
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(B == 0);
  cs.insert(A >= 0);
  cs.insert(B >= 1);
  cs.insert(C > 0);

  NNC_Polyhedron ph(cs);
  Grid gr(ph);

  Grid known_gr(3, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");

  return ok;
}

// Grid(ph) - zero dimension universe polyhedron
bool
test03() {

  NNC_Polyhedron ph(0);
  Grid gr(ph);

  Grid known_gr(0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");

  return ok;
}

// Grid(ph) - zero dimension empty polyhedron
bool
test04() {

  NNC_Polyhedron ph(0, EMPTY);
  Grid gr(ph);

  Grid known_gr(0, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");

  return ok;
}

// Grid(ph) - non-empty polyhedron constructed from generators
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System cs;
  cs.insert(point(A + B, 3));
  cs.insert(ray(A - C));
  cs.insert(point());

  C_Polyhedron ph(cs);
  Grid gr(ph);

  Grid known_gr(3);
  known_gr.add_constraint(A - B + C == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");
  print_generators(gr, "*** gr(ph) ***");

  return ok;
}

/* Grid(ph) - non-empty and non-universe grid built from
   C_polyhedron constructed from generators */
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System cs;
  cs.insert(point(A + B, 3));
  cs.insert(line(A - C));
  cs.insert(point(3 * C, 2));

  C_Polyhedron ph(cs);

  Grid gr(ph);

  print_constraints(ph, "*** ph ***");

  Grid known_gr(3);
  known_gr.add_constraint(2*A + 7*B + 2*C == 3);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");
  print_generators(gr, "*** gr(ph) ***");

  print_congruences(known_gr, "*** known_gr(ph) ***");
  print_generators(known_gr, "*** known_gr(ph) ***");

  return ok;
}

/* Grid(ph) - universe grid built from
   non-universe C_polyhedron constructed from generators */
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System cs;
  cs.insert(point(A + B, 3));
  cs.insert(point(3 * A, 2));
  cs.insert(point(B, 7));
  cs.insert(point(5 * C));

  C_Polyhedron ph(cs);

  Grid gr(ph);

  print_constraints(ph, "*** ph ***");

  Grid known_gr(3);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");
  print_generators(gr, "*** gr(ph) ***");

  return ok;
}

// Grid(ph) - universe polyhedron
bool
test08() {

  NNC_Polyhedron ph(5);
  Grid gr(ph);

  Grid known_gr(5);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");

  return ok;
}

/* Grid(ph) - non-empty and non-universe grid built from
   C_polyhedron constructed from generators; The complexity
   limit allows the detection of implicit equalities from
   any generator system*/
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System cs;
  cs.insert(point(A + B));
  cs.insert(line(A - C));
  cs.insert(point(3 * C));

  C_Polyhedron ph(cs);

  Grid gr(ph, POLYNOMIAL_COMPLEXITY);

  print_constraints(ph, "*** ph ***");

  Grid known_gr(3);
  known_gr.add_constraint(A + 2*B + C == 3);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");
  print_generators(gr, "*** gr(ph) ***");

  print_congruences(known_gr, "*** known_gr(ph) ***");
  print_generators(known_gr, "*** known_gr(ph) ***");

  return ok;
}

/* Grid(ph) - non-empty and non-universe grid built from
   C_polyhedron constructed from constraints; The complexity
   is unlimited so it is able to detect the implicit equality */
bool
test10() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(B >= 0);
  cs.insert(B <= 0);
  cs.insert(A >= 0);

  C_Polyhedron ph(cs);
  Grid gr(ph, ANY_COMPLEXITY);

  Grid known_gr(2);
  known_gr.add_constraint(B == 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");

  return ok;
}

/* Grid(ph) - non-empty and non-universe grid built from
   C_polyhedron constructed from constraints; The complexity
   is limited to be polynomial so it is unable to detect the
   implicit equality */
bool
test11() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(B >= 0);
  cs.insert(B <= 0);
  cs.insert(A >= 0);

  C_Polyhedron ph(cs);
  Grid gr(ph, POLYNOMIAL_COMPLEXITY);

  Grid known_gr(2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");

  return ok;
}

/* Grid(ph) - non-empty and non-universe grid built from
   C_polyhedron constructed from constraints; The complexity
   is limited to that of simplex so it is unable to detect the
   implicit equality */
bool
test12() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(B >= 0);
  cs.insert(B <= 0);
  cs.insert(A >= 0);

  C_Polyhedron ph(cs);
  Grid gr(ph, SIMPLEX_COMPLEXITY);

  Grid known_gr(2);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr(ph) ***");

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
  DO_TEST_F8(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
END_MAIN
