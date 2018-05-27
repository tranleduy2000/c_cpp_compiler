/* Test that the right exceptions are thrown in case of incorrect uses.
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
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x - y > 0);
  cs.insert(x >= 0);

  try {
    // This is an invalid use of the constructor of a polyhedron:
    // it is illegal to build a closed polyhedron starting from
    // a system of constraints that contains strict inequalities.
    C_Polyhedron ph(cs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Generator_System gs;
  gs.insert(closure_point(2*x + y));
  gs.insert(point(x + y));
  gs.insert(ray(x));
  gs.insert(ray(y));

  try {
    // This is an invalid use of the constructor of a polyhedron:
    // it is illegal to build a closed polyhedron starting from
    // a system of generators that contains closure points.
    C_Polyhedron ph(gs);
    exit(1);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);

  try {
    // This is an invalid use of the function add_constraint(c): it is
    // illegal to insert a strict inequality into a system of
    // constraints of a closed polyhedron.
    ph.add_constraint(x - y > 0);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test04() {
  C_Polyhedron ph(3, EMPTY);

  try {
    // This is an incorrect use of the function add_generator(g): it
    // is illegal to insert a closure-point into a system of
    // generators of a closed polyhedron.
    ph.add_generator(closure_point(Linear_Expression(2)));
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);
  ph.add_constraint(x >= 2);
  ph.add_constraint(y >= 2);

  Constraint_System cs;
  cs.insert(x == y);
  cs.insert(x < 5);

  try {
    // This is an incorrect use of the function
    // add_constraints(cs): it is illegal to add a system of
    // constraints that contains strict inequalities to a closed polyhedron.
    ph.add_constraints(cs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);
  Constraint_System cs;
  cs.insert(x > 3);
  cs.insert(x > y);

  try {
    // This is an invalid use of the function add_constraints(cs):
    // it is illegal to add a system of constraints that contains
    // strict inequalities to a closed polyhedron.
    ph.add_constraints(cs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
}
  catch (...) {
  }
  return false;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);

  Constraint_System cs;
  cs.insert(x > 2);
  cs.insert(x == y);
  NNC_Polyhedron qh(cs);

  try {
    // This is an incorrect use of the method concatenate_assign(): it
    // is illegal to apply this method to a closed polyhedron with a
    // NNC Polyhedron.
    ph.concatenate_assign(qh);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);
  Generator_System gs;
  gs.insert(point());
  gs.insert(closure_point(-x));
  gs.insert(ray(x));
  gs.insert(ray(y));

  try {
    // This is an incorrect use of the function
    // add_generators(gs): it is illegal to add a
    // system of generators that contains closure-points to a closed
    // polyhedron.
    ph.add_generators(gs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);
  Generator_System gs;
  gs.insert(point(x));
  gs.insert(closure_point());
  gs.insert(ray(x+y));

  try {
    // This is an incorrect use of the function add_generators(gs): it is
    // illegal to add a system of generators that contains closure-points
    // to a closed polyhedron.
    ph.add_generators(gs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 1);
  ph.add_constraint(B >= 1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::generalized_affine_image(v, expr, d):
    // `GREATER_THAN' is an illegal relation for necessarily closed
    // polyhedron.
    ph.generalized_affine_image(A + B, GREATER_THAN, A - B);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 2);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(A - B > 0);
  ph2.add_constraint(A >= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  try {
    // This is an invalid use of the function
    // `intersection_assign': it is illegal to apply
    // to a closed polyhedron and a non-closed polyhedron.
    ph1.intersection_assign(ph2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test12() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(B >= 2);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(A - B > 0);
  ph2.add_constraint(B >= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  try {
    // This is an invalid use of the function
    // `intersection_assign': it is illegal to apply this function
    // to a closed polyhedron and a non-closed polyhedron.
    ph1.intersection_assign(ph2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(point(3*B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point(2*A));
  gs2.insert(closure_point());
  gs2.insert(closure_point(3*A));
  NNC_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

   try {
    // This is an invalid use of the function
    // `upper_bound_assign': it is illegal to apply this function
    // to a closed polyhedron and a non-closed polyhedron.
    ph1.upper_bound_assign(ph2);
   }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
   return false;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A <= 4);
  ph1.add_constraint(B >= 0);
  ph1.add_constraint(B <= 4);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(A >= 2);
  ph2.add_constraint(A <= 6);
  ph2.add_constraint(B >= 0);
  ph2.add_constraint(B <= 4);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  try {
    // This is an invalid use of the function
    // `difference_assign': it is illegal to apply this function
    // to a closed polyhedron and a non-closed polyhedron.
    ph1.difference_assign(ph2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test16() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A <= 2);
  ph1.add_constraint(A - B >= 0);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(A >= 0);
  ph2.add_constraint(A <= 4);
  ph2.add_constraint(A - B >= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  try {
    // This is an invalid use of the function
    // `H79_widening_assign': it is illegal to apply this function
    // to a closed polyhedron and a non-closed polyhedron.
    ph2.H79_widening_assign(ph1);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test17() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A <= 2);
  ph1.add_constraint(A - B >= 0);

  C_Polyhedron ph2(2);
  ph2.add_constraint(A >= 0);
  ph2.add_constraint(A <= 4);
  ph2.add_constraint(A - B >= 0);

  Constraint_System cs;
  cs.insert(A <= 8);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(cs, "*** cs ***");

  try {
    // This is an invalid use of the function
    // `limited_H79_widening_assign': it is illegal to
    // apply this function to a closed polyhedron and
    // a non-closed polyhedron.
    ph2.limited_H79_extrapolation_assign(ph1, cs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A <= 2);
  ph1.add_constraint(A - B >= 0);

  C_Polyhedron ph2(2);
  ph2.add_constraint(A >= 0);
  ph2.add_constraint(A <= 4);
  ph2.add_constraint(A - B >= 0);

  Constraint_System cs;
  cs.insert(A < 8);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(cs, "*** cs ***");

  try {
    // This is an invalid use of the function
    // `limited_H79_extrapolation_assign': it is illegal to
    // apply this function to two closed polyhedra and
    // to a non-closed system of constraints.
    ph2.limited_H79_extrapolation_assign(ph1, cs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
}
  catch (...) {
  }
  return false;
}

bool
test19() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(B >= 0);
  ph1.add_constraint(A + B <= 1);

  Generator_System gs;
  gs.insert(point(2*A + 2*B));
  gs.insert(ray(A + B));
  NNC_Polyhedron ph2(gs);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

 try {
    // This is an invalid use of the function
    // `time_elapse_assign': it is illegal to
    // apply this function to a closed polyhedron and
    // a non-closed polyhedron.
    ph1.time_elapse_assign(ph2);
 }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
 }
  catch (...) {
  }
 return false;
}

bool
test20() {
  Variable A(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(A >= 5);

  NNC_Polyhedron ph2(1);
  ph2.add_constraint(A > 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  try {
    // This is an invalid use of the Polyhedron::contains(): it is
    // illegal to apply this method to a closed polyhedron and a
    // non-closed polyhedron.
    ph1.contains(ph2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test21() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A - B > 0);

  const Constraint_System cs = ph1.constraints();

  print_constraints(cs, "*** cs ***");

  try {
    // This is an incorrect use of the function
    // C_Polyhedron::C_Polyhedron(cs): it is illegal to build a
    // closed polyhedron starting from a system of constraints
    // that contains strict inequalities.
    C_Polyhedron ph2(cs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test22() {
  Variable A(0);

  Generator_System gs1;
  gs1.insert(point(3*A));
  gs1.insert(closure_point(2*A));
  gs1.insert(ray(A));

  NNC_Polyhedron ph1(gs1);

  const Generator_System gs2 = ph1.generators();

  try {
    // This is an incorrect use of the function
    // `C_Polyhedron(const Generator_System)': it is illegal to build
    // a closed polyhedron starting from a constant non-closed
    // system of generators.
    C_Polyhedron ph2(gs2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test23() {
  NNC_Polyhedron ph(0, EMPTY);

  try {
    // This is an incorrect use of the function
    // `add_generator(g)': it is illegal add a closure point
    // to a zero-dimensional and empty non-closed polyhedron.
    ph.add_generator(closure_point());
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test24() {
  NNC_Polyhedron ph(0, EMPTY);

  print_constraints(ph, "*** ph ***");

  Generator_System gs;
  gs.insert(closure_point());

  try {
    // This is an invalid used of the function
    // `add_generators(gs)': it is illegal to
    // add a system of generators that does not contain points
    // to an empty zero-dimensional polyhedron.
    ph.add_generators(gs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test25() {
  NNC_Polyhedron ph(0, EMPTY);

  print_constraints(ph, "*** ph ***");

  Generator_System gs;
  gs.insert(closure_point());

  try {
    // This is an invalid used of the function
    // `add_generators(gs)': it is illegal to
    // add a system of generators that does not contain points
    // to an empty zero-dimensional polyhedron.
    ph.add_generators(gs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test26() {
  Variable A(0);

  NNC_Polyhedron ph1(1);
  ph1.add_constraint(A > 5);

  C_Polyhedron ph2(1);
  ph2.add_constraint(A >= 0);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  try {
    // This is an invalid use of Polyhedron::contains():
    // it is illegal to apply this method to a
    // closed polyhedron and a non-closed polyhedron.
    ph2.contains(ph1);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test27() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A <= 2);
  ph1.add_constraint(A - B >= 0);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(A >= 0);
  ph2.add_constraint(A <= 4);
  ph2.add_constraint(A - B >= 0);

  Constraint_System cs;
  cs.insert(A < 8);

  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");
  print_constraints(cs, "*** cs ***");

  try {
    // This is an invalid use of the function
    // `limited_H79_extrapolation_assign': it is illegal to
    // apply this function to a non-closed polyhedron,
    // a non-closed polyhedron and a system of
    // constraints that contains strict inequalities.
    ph2.limited_H79_extrapolation_assign(ph1, cs);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test28() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A <= 2);
  ph1.add_constraint(B >= 0);
  ph1.add_constraint(A - B >= 0);

  NNC_Polyhedron ph2(2);
  ph2.add_constraint(A < 5);
  ph2.add_constraint(B >= 0);
  ph2.add_constraint(A - B >= 0);

  try {
    // This is an invalid use of the function
    // `BHRZ03_widening_assign': it is illegal to
    // apply this function to a non-closed polyhedron and
    // a non-closed polyhedron.
    ph2.BHRZ03_widening_assign(ph1);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test29() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  try {
    // This is an incorrect use of function
    // Generator::closure_point(e, d):
    // it is illegal to use a denominator
    // equal to zero.
    gs.insert(closure_point(A + 2*B, 0));
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test30() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2);
  ph1.add_constraint(A < 2);
  ph1.add_constraint(B > 0);

  C_Polyhedron ph2;

  try {
    // This is an invalid use of the function
    // `Polyhedron::swap(Polyhedron&)': it is illegal
    // to apply this function to a closed and a
    // not necessarily closed polyhedron.
    swap(ph1, ph2);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test31() {
  Variable A(0);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::generalized_affine_image(v, r, expr,d ):
    // `GREATER_THAN' is an illegal relation for necessarily closed
    // polyhedron.
    ph.generalized_affine_image(A, GREATER_THAN, A + 1);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
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
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
  DO_TEST(test20);
  DO_TEST(test21);
  DO_TEST(test22);
  DO_TEST(test23);
  DO_TEST(test24);
  DO_TEST(test25);
  DO_TEST(test26);
  DO_TEST(test27);
  DO_TEST(test28);
  DO_TEST(test29);
  DO_TEST(test30);
  DO_TEST(test31);
END_MAIN
