/* Test Polyhedron::BHRZ03_widening_assign().
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

bool
test01() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(B >= 0);
  ph1.add_constraint(A + B > 0);
  ph1.add_constraint(A - B < 1);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(B >= 0);
  ph2.add_constraint(A > 0);
  ph2.add_constraint(A < 1);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.BHRZ03_widening_assign(ph2);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after BHRZ03_widening_assign ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System gs1;
  gs1.insert(point(A));
  gs1.insert(closure_point());
  gs1.insert(ray(A));
  gs1.insert(ray(B));
  gs1.insert(ray(A + B + 2*C));
  NNC_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point(A));
  gs2.insert(closure_point());
  gs2.insert(ray(A));
  gs2.insert(ray(B));
  gs2.insert(ray(A + B + C));
  NNC_Polyhedron ph2(gs2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.BHRZ03_widening_assign(ph2);

  NNC_Polyhedron known_result(3);
  known_result.add_constraint(A > 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after BHRZ03_widening_assign ***");

  return ok;
}

Generator_System
aux1_test03(const Coefficient& half_side) {
  // Square.
  Variable A(0);
  Variable B(1);
  Generator_System gs;
  gs.insert(point(half_side*A + half_side*B));
  gs.insert(point(half_side*A - half_side*B));
  gs.insert(point(-half_side*A - half_side*B));
  gs.insert(point(-half_side*A + half_side*B));
  return gs;
}

Generator_System
aux2_test03(const Coefficient& half_diagonal) {
  // Rhombus.
  Variable A(0);
  Variable B(1);
  Generator_System gs;
  gs.insert(point(half_diagonal*A));
  gs.insert(point(half_diagonal*B));
  gs.insert(point(-half_diagonal*A));
  gs.insert(point(-half_diagonal*B));
  return gs;
}

C_Polyhedron
aux3_test03(unsigned n) {

  Coefficient half_diagonal = 2;
  for (unsigned i = n / 8; i-- > 0; ) {
    half_diagonal *= 2;
  }
  Coefficient half_side = half_diagonal;

  Generator_System gs;
  if (n % 8 < 4) {
    half_side /= 2;
    gs = aux1_test03(half_side);
    Generator_System gs2 = aux2_test03(half_diagonal);
    Generator_System::const_iterator gi = gs2.begin();
    for (int i = n % 8; i-- > 0; )
      gs.insert(*gi++);
  }
  else {
    gs = aux2_test03(half_diagonal);
    Generator_System gs2 = aux1_test03(half_side);
    Generator_System::const_iterator gi = gs2.begin();
    for (int i = n % 8 - 4; i-- > 0; )
      gs.insert(*gi++);
  }
  C_Polyhedron ph = C_Polyhedron(gs);

  return ph;
}

bool
test03() {
  // Chain condition for widenings:
  // for each increasing chain of descriptions p_0, p_1, ..., p_i, ...,
  // the sequence q_0, q_1, ..., q_i, ... defined by q_0 = p_0 and
  // for each i >= 1, q_i = q_{i-1} \nabla p_i is ultimately stationary.

  // Initialization: set q_0.
  C_Polyhedron q_i_minus_1 = aux3_test03(0);

  for (unsigned i = 1; i <= 100; ++i) {
    print_generators(q_i_minus_1.generators(),
		     "*** Result of the previous iteration ***");

    C_Polyhedron p_i = aux3_test03(i);
    print_generators(p_i.generators(), "*** New stuff ***");

    C_Polyhedron q_i = q_i_minus_1;
    q_i.upper_bound_assign(p_i);
    print_generators(q_i.generators(),
		     "*** Poly-hull of previous with new ***");

    q_i.BHRZ03_widening_assign(q_i_minus_1);
    print_generators(q_i.generators(),
		     "*** Result of widening poly-hull with new ***");

    if (q_i == q_i_minus_1) {
      C_Polyhedron known_result(2);

      bool ok = (q_i == known_result);

      print_constraints(q_i, "*** The constraints of the fix point ***");
      print_generators(q_i, "*** The generators of the fix point ***");

      return ok;
    }
    q_i_minus_1 = q_i;
  }
  return false;
}

Generator_System
aux1_test04() {
  Variable A(0);
  Variable B(1);
  Generator_System new_gs;
  new_gs.insert(point(A));
  new_gs.insert(point(-B));
  new_gs.insert(point(-A));
  new_gs.insert(point(B));
  return new_gs;
}

Generator
aux2_test04(const Generator& p1, const Generator& p2, unsigned magic_number) {
  // Splitting segment.
  const Coefficient& d1 = p1.divisor();
  const Coefficient& d2 = p2.divisor();
  Linear_Expression expr = d2 * Linear_Expression(p1);
  expr += d1 * Linear_Expression(p2);
  // The divisor for the average is 2 * d1 * d2.
  // by carefully taking a smaller divisor, we obtain a point
  // that won't be redundant in the polyhedron.
  // NOTE: I am not *sure* this dirty kludge of using such
  // a magic number will always succeed.
  return point((magic_number+1)*expr, magic_number*2*d1*d2);
}

Generator_System
aux3_test04(const Generator_System& gs, unsigned magic_number) {
  // Double generators.
  Generator_System new_gs;
  Generator_System::const_iterator i = gs.begin();
  Generator_System::const_iterator gs_end = gs.end();
  while (true) {
    const Generator& g = *i;
    new_gs.insert(g);
    ++i;
    if (i != gs_end)
      new_gs.insert(aux2_test04(g, *i, magic_number));
    else {
      // Split the last segment.
      Generator_System::const_iterator gs_begin = gs.begin();
      new_gs.insert(aux2_test04(g, *gs_begin, magic_number));
      break;
    }
  }
  return new_gs;
}

C_Polyhedron
aux4_test04(unsigned n) {

  unsigned needed_vertices = n + 4;

  unsigned magic_number = 1;
  unsigned magic_factor = 4;
  Generator_System gs = aux1_test04();
  unsigned gs_vertices = 4;

  while (gs_vertices * 2 <= needed_vertices) {
    magic_number *= magic_factor;
    gs = aux3_test04(gs, magic_number);
    gs_vertices *= 2;
  }

  if (gs_vertices < needed_vertices) {
    magic_number *= magic_factor;
    Generator_System gs2 = aux3_test04(gs, magic_number);
    Generator_System::const_iterator gs2_i = gs2.begin();
    for ( ; gs_vertices < needed_vertices; ++gs_vertices) {
      // Skip the even indexed vertices of gs2.
      ++gs2_i;
      // Add the odd indexed vertices of gs2.
      gs.insert(*gs2_i++);
    }
  }

  C_Polyhedron ph = C_Polyhedron(gs);
  return ph;
}

bool
test04() {
  // Chain condition for widenings:
  // for each increasing chain of descriptions p_0, p_1, ..., p_i, ...,
  // the sequence q_0, q_1, ..., q_i, ... defined by q_0 = p_0 and
  // for each i >= 1, q_i = q_{i-1} \nabla p_i is ultimately stationary.

  // Initialization: set q_0.
  C_Polyhedron q_i_minus_1 = aux4_test04(0);

  for (unsigned i = 1; i <= 100; ++i) {
    print_generators(q_i_minus_1.generators(),
		     "*** Result of the previous iteration ***");

    C_Polyhedron p_i = aux4_test04(i);
    print_generators(p_i.generators(), "*** New stuff ***");

    C_Polyhedron q_i = q_i_minus_1;
    q_i.upper_bound_assign(p_i);
    print_generators(q_i.generators(),
		     "*** Poly-hull of previous with new ***");

    q_i.BHRZ03_widening_assign(q_i_minus_1);
    print_generators(q_i.generators(),
		     "*** Result of widening poly-hull with new ***");

    if (q_i == q_i_minus_1) {
      C_Polyhedron known_result(2);

      bool ok = (q_i == known_result);

      print_constraints(q_i, "*** The constraints of the fix point ***");
      print_generators(q_i, "*** The generators of the fix point ***");

      return ok;
    }
    q_i_minus_1 = q_i;
  }
  return false;
}

Generator_System
aux1_test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System new_gs;
  new_gs.insert(ray(A + C));
  new_gs.insert(ray(-B + C));
  new_gs.insert(ray(-A + C));
  new_gs.insert(ray(B + C));
  return new_gs;
}

Generator
aux2_test05(const Generator& r1, const Generator& r2, unsigned magic_number) {
  // Splitting facet.
  //  Variable A(0);
  //  Variable B(1);
  Variable C(2);

  Linear_Expression expr;
  expr += Linear_Expression(r1);
  expr += Linear_Expression(r2);
  // NOTE: I am not *sure* this dirty kludge of using such
  // a magic number will always succeed.
  expr *= magic_number + 1;
  expr -= C;
  return ray(expr);
}

Generator_System
aux3_test05(const Generator_System& gs, unsigned magic_number) {
  // Double generators.
  Generator_System new_gs;
  Generator_System::const_iterator i = gs.begin();
  Generator_System::const_iterator gs_end = gs.end();
  while (true) {
    const Generator& g = *i;
    new_gs.insert(g);
    ++i;
    if (i != gs_end)
      new_gs.insert(aux2_test05(g, *i, magic_number));
    else {
      // Split the last facet.
      Generator_System::const_iterator gs_begin = gs.begin();
      new_gs.insert(aux2_test05(g, *gs_begin, magic_number));
      break;
    }
  }
  return new_gs;
}

C_Polyhedron
aux4_test05(unsigned n) {
  unsigned needed_facets = n + 4;
  unsigned magic_number = 1;
  unsigned magic_factor = 4;
  Generator_System gs = aux1_test05();
  unsigned gs_facets = 4;

  while (gs_facets * 2 <= needed_facets) {
    magic_number *= magic_factor;
    gs = aux3_test05(gs, magic_number);
    gs_facets *= 2;
  }

  if (gs_facets < needed_facets) {
    magic_number *= magic_factor;
    Generator_System gs2 = aux3_test05(gs, magic_number);
    Generator_System::const_iterator gs2_i = gs2.begin();
    for ( ; gs_facets < needed_facets; ++gs_facets) {
      // Skip the even indexed facets of gs2.
      ++gs2_i;
      // Add the odd indexed facets of gs2.
      gs.insert(*gs2_i++);
    }
  }
  gs.insert(point());
  C_Polyhedron ph = C_Polyhedron(gs);
  return ph;
}

bool
test05() {
  // Chain condition for widenings:
  // for each increasing chain of descriptions p_0, p_1, ..., p_i, ...,
  // the sequence q_0, q_1, ..., q_i, ... defined by q_0 = p_0 and
  // for each i >= 1, q_i = q_{i-1} \nabla p_i is ultimately stationary.
  //  Variable A(0);
  Variable B(1);
  Variable C(2);

  // Initialization: set q_0.
  C_Polyhedron q_i_minus_1 = aux4_test05(0);

  for (unsigned i = 1; i <= 100; ++i) {
    print_generators(q_i_minus_1, "*** Result of the previous iteration ***");

    C_Polyhedron p_i = aux4_test05(i);
    print_generators(p_i, "*** New stuff ***");

    C_Polyhedron q_i = q_i_minus_1;
    q_i.upper_bound_assign(p_i);
    print_generators(q_i, "*** Poly-hull of previous with new ***");

    q_i.BHRZ03_widening_assign(q_i_minus_1);
    print_generators(q_i, "*** Result of widening poly-hull with new ***");

    if (q_i == q_i_minus_1) {
      C_Polyhedron known_result(3, UNIVERSE);
      known_result.add_constraint(-B + C >= 0);

      bool ok = (q_i == known_result);

      print_constraints(q_i, "*** The constraints of the fix point ***");
      print_generators(q_i, "*** The generators of the fix point ***");

      return ok;
    }
    q_i_minus_1 = q_i;
  }
  return false;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(point(A + 2*B));
  gs1.insert(ray(A));
  gs1.insert(ray(2*A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(point(A + 2*B));
  gs2.insert(ray(A));
  gs2.insert(ray(A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(2*A- B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(point(A + 3*B));
  gs1.insert(ray(A));
  gs1.insert(ray(2*A - B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(point(A + 3*B));
  gs2.insert(ray(A + B));
  gs2.insert(ray(A - B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(3*A - B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(point(6*A - B));
  gs1.insert(point(6*B));
  gs1.insert(point(A + 10*B));
  gs1.insert(ray(A + 2*B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(point(6*A - B));
  gs2.insert(point(6*B));
  gs2.insert(point(A + 10*B));
  gs2.insert(ray(A + B));
  gs2.insert(ray(A + 3*B));
  gs2.insert(point(-4*A + 3*B, 13));
  gs2.insert(point(-2*A + B, 8));
  gs2.insert(point(-A + 12*B, 4));

  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_constraints(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  // This is the result of applying H79.
  Generator_System gs_known_result;
  gs_known_result.insert(point(-36*A + 6*B, 25));
  gs_known_result.insert(ray(A + 4*B));
  gs_known_result.insert(ray(6*A - B));

  C_Polyhedron known_result(gs_known_result);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");
  print_constraints(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(closure_point());
  gs1.insert(closure_point(A + B));
  gs1.insert(point(2*A + B, 2));
  gs1.insert(ray(A));
  NNC_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(closure_point());
  gs2.insert(closure_point(A + B));
  gs2.insert(closure_point(B, 2));
  gs2.insert(point(2*A + B, 2));
  gs2.insert(ray(A));
  NNC_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(B > 0);
  known_result.add_constraint(B < 1);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** after  ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(B));
  gs.insert(point(A + 2*B));
  gs.insert(point(A + B));
  NNC_Polyhedron ph(gs);

  gs.clear();
  gs.insert(point());
  gs.insert(point(B));
  gs.insert(point(A + 2*B));
  gs.insert(closure_point(A));
  NNC_Polyhedron ph1(gs);

  print_constraints(ph, "*** ph ***");
  print_constraints(ph1, "*** ph1 ***");

  ph1.BHRZ03_widening_assign(ph);

  gs.clear();
  gs.insert(point());
  gs.insert(point(B));
  gs.insert(point(A + 2*B));
  gs.insert(ray(-B));
  NNC_Polyhedron known_result(gs);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** ph1.BHRZ03_widening_assin(ph) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST_F8(test04);
  DO_TEST_F8(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST_F8(test08);
  DO_TEST(test09);
  DO_TEST(test10);
END_MAIN
