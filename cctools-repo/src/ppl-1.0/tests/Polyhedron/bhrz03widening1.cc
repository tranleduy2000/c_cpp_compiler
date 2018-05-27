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
  Variable C(2);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(point(A));
  gs1.insert(point(B));
  gs1.insert(point(A + B));
  gs1.insert(point(C));
  gs1.insert(point(A + C));
  gs1.insert(point(B + C));
  gs1.insert(point(A + B + C));
  C_Polyhedron ph1(gs1);

  C_Polyhedron ph1_copy(ph1);

  Generator_System gs2;
  gs2.insert(point(0*C));
  gs2.insert(point(A));
  gs2.insert(point(B));
  gs2.insert(point(A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.BHRZ03_widening_assign(ph2);

  bool ok = (ph1 == ph1_copy);

  print_constraints(ph1, "*** after BHRZ03_widening_assign ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);

  C_Polyhedron ph1_copy(ph1);

  C_Polyhedron ph2(2);
  ph2.add_constraint(A >= 0);
  ph2.add_constraint(B >= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.BHRZ03_widening_assign(ph2);

  bool ok = (ph1 == ph1_copy);

  print_constraints(ph1, "*** after BHRZ03_widening_assign ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(point(2*A));
  gs1.insert(point(2*B));

  C_Polyhedron ph1(gs1);
  C_Polyhedron ph1_copy(ph1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(point(A));
  gs2.insert(point(B));
  gs2.insert(point(A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph1.BHRZ03_widening_assign(ph2);

  bool ok = (ph1 == ph1_copy);

  print_constraints(ph1, "*** after BHRZ03_widening_assign ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point(2*A));
  gs1.insert(closure_point(A+B));
  gs1.insert(closure_point(3*A+B));
  NNC_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point(2*A));
  gs2.insert(closure_point(B));
  gs2.insert(closure_point(4*A+B));
  NNC_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B < 1);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** after BHRZ03_widening_assign ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A - B >= 0);
  ph1.add_constraint(A + B <= 2);
  ph1.add_constraint(B >= 0);

  C_Polyhedron ph2(2);
  ph2.add_constraint(2*A - B >= 0);
  ph2.add_constraint(B >= 0);
  ph2.add_constraint(2*A + B <= 4);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(2*A + B >= 0);
  known_result.add_constraint(2*A - B <= 4);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** after BHRZ03_widening_assign ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A - B <= 0);

  C_Polyhedron ph2(2);
  ph2.add_constraint(A >= 0);
  ph2.add_constraint(2*A - B <= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.BHRZ03_widening_assign(ph2);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A >= 0);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after BHRZ03_widening_assign ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(A));
  gs1.insert(ray(B));
  gs1.insert(ray(A + 4*B + 2*C));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(A));
  gs2.insert(ray(B));
  gs2.insert(ray(A + 2*B + 4*C));
  C_Polyhedron ph2(gs2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(3);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** after BHRZ03_widening_assign ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(line(A));
  gs1.insert(ray(B));
  gs1.insert(ray(A + B + C));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(line(A));
  gs2.insert(ray(B));
  gs2.insert(ray(A + B + 2*C));
  C_Polyhedron ph2(gs2);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(3);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 0);

  bool ok = (ph2 == known_result);

  print_constraints(ph2, "*** after BHRZ03_widening_assign ***");

  return ok;
}

const Generator_System&
aux1_test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  static Generator_System gs;
  static bool initialized = false;
  if (!initialized) {
    gs.insert(point());
    gs.insert(ray(C));
    gs.insert(ray(D));
    gs.insert(ray(E));
    gs.insert(ray(A + D));
    gs.insert(ray(A + B + E));
    initialized = true;
  }
  return gs;
}

C_Polyhedron
aux2_test09(unsigned n) {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  C_Polyhedron ph(aux1_test09());
  n += 2;
  ph.add_generator(ray(A + (n-1)*B + E));
  if (n % 2 == 0) {
    // Even.
    unsigned m = n / 2;
    ph.add_generator(ray(m*B + E));
    ph.add_generator(ray(A + (m-1)*B + D));
  }
  else {
    // Odd.
    ph.add_generator(ray(n*B + 2*E));
    ph.add_generator(ray(2*A + (n-2)*B + 2*D));
  }
  return ph;
}

bool
test09() {
  // Chain condition for widenings:
  // for each increasing chain of descriptions p_0, p_1, ..., p_i, ...,
  // the sequence q_0, q_1, ..., q_i, ... defined by q_0 = p_0 and
  // for each i >= 1, q_i = q_{i-1} \nabla p_i is ultimately stationary.
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  // Initialization: set q_0.
  C_Polyhedron q_i_minus_1 = aux2_test09(0);

  for (unsigned i = 1; i <= 100; ++i) {
    print_generators(q_i_minus_1.generators(),
		     "*** Result of the previous iteration ***");

    C_Polyhedron p_i = aux2_test09(i);
    print_generators(p_i.generators(), "*** New stuff ***");

    C_Polyhedron q_i = q_i_minus_1;
    q_i.upper_bound_assign(p_i);
    print_generators(q_i.generators(),
		     "*** Poly-hull of previous with new ***");

    q_i.BHRZ03_widening_assign(q_i_minus_1);
    print_generators(q_i.generators(),
		     "*** Result of widening poly-hull with new ***");

    if (q_i == q_i_minus_1) {

      C_Polyhedron known_result(5);
      known_result.add_constraint(A >= 0);
      known_result.add_constraint(B >= 0);
      known_result.add_constraint(C >= 0);
      known_result.add_constraint(D >= 0);
      known_result.add_constraint(E >= 0);
      known_result.add_constraint(-A + B + D >= 0);

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
test10() {
  C_Polyhedron ph1;
  C_Polyhedron ph2(0, EMPTY);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.BHRZ03_widening_assign(ph2);

  C_Polyhedron known_result;

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.H79_widening_assign(ph2) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(B >= 0);

  C_Polyhedron ph2(2);
  ph2.add_constraint(A >= 2);
  ph2.add_constraint(A <= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  C_Polyhedron known_result(ph1);

  ph1.BHRZ03_widening_assign(ph2);

  bool ok = (ph1 == known_result);

  print_constraints(ph1, "*** after ph1.BHRZ03_widening_assign(ph2) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(A));
  gs1.insert(ray(A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(A));
  gs2.insert(ray(A + 2*B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-A));
  gs1.insert(ray(-A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(-A));
  gs2.insert(ray(-A + 2*B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-A));
  gs1.insert(ray(-A - B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(-A));
  gs2.insert(ray(-A - 2*B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B <= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(A));
  gs1.insert(ray(A - B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(A));
  gs2.insert(ray(A - 2*B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(B));
  gs1.insert(ray(A + 2*B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(B));
  gs2.insert(ray(A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(B));
  gs1.insert(ray(-A + 2*B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(B));
  gs2.insert(ray(-A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-B));
  gs1.insert(ray(-A - 2*B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(-B));
  gs2.insert(ray(-A - B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B <= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test19() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-B));
  gs1.insert(ray(A - 2*B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(-B));
  gs2.insert(ray(A - B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B <= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test20() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(A));
  gs1.insert(ray(A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(A));
  gs2.insert(ray(-A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

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
