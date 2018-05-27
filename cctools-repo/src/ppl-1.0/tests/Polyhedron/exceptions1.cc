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
  Variable z(2);

  Generator_System gs;
  try {
    // This is an incorrect use of the function Generator::point(expr, d):
    // it is illegal to build a point with the denominator
    // equal to zero.
    gs.insert(point(x + y + z, 0));
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
test02() {
  Variable x(0);
  Variable y(1);

  Generator_System gs;
  gs.insert(point(x + y));
  gs.insert(ray(x + 0*y));
  gs.insert(ray(0*x + y));
  C_Polyhedron ph(gs);
  Linear_Expression coeff1 = x + y + 1;
  try {
    // This is an incorrect use of function
    // C_Polyhedron::affine_image(v, expr,d): it is illegal applying
    // the function with a linear expression with the denominator equal to
    // zero.
    Coefficient d = 0;
    ph.affine_image(x, coeff1, d);
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

  C_Polyhedron ph1;
  Generator_System gs;
  gs.insert(point(x + y));
  C_Polyhedron ph2(gs);
  try {
    // This is an incorrect use of function
    // C_Polyhedron::upper_bound_assign(p): it is illegal to use
    // it with two polyhedra of different dimensions.
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
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Generator_System gs;
  gs.insert(line(x + y + z));

  try {
    // This is an incorrect use of the function
    // C_Polyhedron::C_Polyhedron(gs): it is illegal to build a
    // polyhedron starting from a system of generators that does not
    // contain a point.
    C_Polyhedron ph(gs);
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
  Variable z(2);

  Generator_System gs;
  gs.insert(point(0*x + 1*y +2*z));
  C_Polyhedron ph(gs);

  Variables_Set to_be_removed;
  to_be_removed.insert(z);

  ph.remove_space_dimensions(to_be_removed);

  try {
    to_be_removed.insert(x);
    // This is an incorrect use use of function
    // C_Polyhedron::remove_space_dimensions(to_be_remove).
    // Here the set `to_be_removed' still contains variable `z'.
    // This variable is now beyond the space dimension,
    // so that a dimension-incompatibility exception is obtained.
    ph.remove_space_dimensions(to_be_removed);
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

  C_Polyhedron ph(1);
  ph.add_constraint(x >= 1);

  try {
    // This is an invalid used of the function
    // C_Polyhedron::affine_image(v, expr, d): it is illegal to
    // apply this function to a variable that is not in the space of
    // the polyhedron.
    ph.affine_image(y, x + 1);
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
  Variable z(2);

  C_Polyhedron ph(2);
  ph.add_constraint(x >= 1);
  ph.add_constraint(y >= 1);

  try {
    // This is an invalid used of the function
    // C_Polyhedron::affine_image(v, expr, d): it is illegal to
    // use a variable in the expression that does not appear in the
    // space of the polyhedron.
    ph.affine_image(y, x + z + 1);
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

  C_Polyhedron ph(2);
  ph.add_constraint(x >= y);
  Linear_Expression coeff = x + y + 1;
  try {
    // This is an incorrect use of the function
    // C_Polyhedron::affine_preimage(v, expr, d): it is illegal
    // to apply to a polyhedron an expression with the denominator
    // equal to zero.
    Coefficient d = 0;
    ph.affine_preimage(x, coeff, d);
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
  Variable z(2);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(x + y));
  gs.insert(ray(x));

  C_Polyhedron ph(gs);
  try {
    // This is an invalid used of the function
    // C_Polyhedron::affine_image(v, expr, d): it is illegal apply
    // the transformation to a variable that is not in the space
    // of the polyhedron.
    ph.affine_preimage(z, x + 1);
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
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Generator_System gs;
  gs.insert(point());
  gs.insert(point(x));
  gs.insert(line(x + y));

  C_Polyhedron ph(gs);
  try {
    // This is an invalid used of the function
    // C_Polyhedron::affine_preimage(v, expr, d): it is illegal to
    // apply to a polyhedron an expression that contains a variable that
    // is not in the space of the polyhedron.
    ph.affine_preimage(y, x + z + 1);
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
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(2);
  ph1.add_constraint(x >= y);

  C_Polyhedron ph2(3);

  try {
    // This is an invalid use of method
    // C_Polyhedron::intersection_assign(ph2): it is illegal
    // to apply this function to two polyhedra of different dimensions.
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
  C_Polyhedron ph1(7);

  C_Polyhedron ph2(15);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::intersection_assign(ph2): it is illegal to apply
    // this function to two polyhedra of different dimensions.
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
test13() {
  Variable w(4);

  C_Polyhedron ph(2, EMPTY);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::add_generators(gs): it is illegal
    // to add a system of generator that is not dimension-compatible
    // with the polyhedron.
    Generator_System gs;
    gs.insert(point(w));
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
test14() {
  C_Polyhedron ph(5);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::remove_higher_space_dimensions(n): it is illegal to
    // erase a variable that is not in the space of the polyhedron.
    ph.remove_higher_space_dimensions(7);
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
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::add_constraints(cs): it is illegal to
    // add a system of constraints that is not dimensional incompatible
    // with the polyhedron.
    Constraint_System cs;
    cs.insert(x - y >= 0);
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
test16() {
  Variable y(1);

  C_Polyhedron ph(1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::add_constraint(c): it is illegal to insert a
    // constraints that contains a variable that is not in the space
    // of the polyhedron.
    ph.add_constraint(y >= 0);
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
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::add_constraints(cs): it is illegal to add a system
    // of constraints that is dimensional incompatible with the
    // polyhedron.
    Constraint_System cs;
    cs.insert(x - y == 0);
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
test18() {
  Variable x(0);
  Variable y(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(x));

  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point(x));
  gs2.insert(ray(x + y));

  C_Polyhedron ph2(gs2);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::upper_bound_assign(ph2): it is illegal to apply
    // this function to two polyhedra with different dimensions.
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
test19() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(1, EMPTY);

  try {
    // This is an invalid use of the function C_Polyhedron::add_generator(g):
    // it is illegal to insert a generator that is dimensional
    // incompatible with the polyhedron.
    ph.add_generator(point(x + y));
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
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(1, EMPTY);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::add_generators(gs): it is illegal to a system of
    // generators that is dimensional incompatible with the
    // polyhedron.
    Generator_System gs;
    gs.insert(point());
    gs.insert(line(x + y));
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
test21() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Generator_System gs;
  gs.insert(ray(x + y));
  gs.insert(point());

  C_Polyhedron ph(gs);
  try {
    // This is an invalid use of the function C_Polyhedron::relation_with(c):
    // it is illegal to use a constraints that is dimensional
    // incompatible with the polyhedron.
    Constraint c(z >= 0);
    ph.relation_with(c);
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
  Variable z(2);

  C_Polyhedron ph(2);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::relation_with(g): it is illegal to apply this
    // function to a generator that is not dimension-compatible with
    // the polyhedron.
    Generator g(point(z));
    ph.relation_with(g);
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
  C_Polyhedron ph1(5);
  C_Polyhedron ph2(10);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::H79_widening_assign(ph2): it is illegal to apply
    // this function to two polyhedra that are not dimensional
    // compatible.
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
test24() {
  Variable y(1);

  C_Polyhedron ph1(1);
  C_Polyhedron ph2(2);

  Constraint_System cs;
  cs.insert(y <= 9);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::limited_H79_extrapolation_assign(ph2, cs): it is
    // illegal to apply this function to two polyhedra that are not
    // dimension-compatible.
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
test25() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  C_Polyhedron ph1(2);
  ph1.add_constraint(x - y >= 0);
  ph1.add_constraint(x >= 0);
  ph1.add_constraint(x <= 2);

  C_Polyhedron ph2(2);
  ph2.add_constraint(x - y >= 0);
  ph2.add_constraint(x >= 0);
  ph2.add_constraint(x <= 5);

  Constraint_System cs;
  cs.insert(z <= 5);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::limited_H79_extrapolation_assign(ph, cs): it is
    // illegal to apply this function to a system of constraints that
    // is not dimension-compatible with the two polyhedra.
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
test26() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1(3);
  ph1.add_constraint(x - y >= 0);

  C_Polyhedron ph2(2);
  ph2.add_constraint(x - y == 0);

  try {
    // This is an invalid use of Polyhedron::contains(): it is
    // illegal to apply this method to two polyhedra that are not
    // dimension-compatible.
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
test27() {
  Variable x(0);

  C_Polyhedron ph(2, EMPTY);

  try {
    // This is an invalid use of method
    // C_Polyhedron::add_generator(g): it is illegal to insert a
    // generator that is not dimension-compatible with the
    // polyhedron.
    Generator g(ray(x));
    ph.add_generator(g);
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
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3, EMPTY);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::add_generators(gs): it is illegal to add a system
    // of generators with no points to an empty polyhedron.
    Generator_System gs;
    gs.insert(ray(x + y));
    gs.insert(ray(x - y));
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
test29() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2, EMPTY);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::add_generators(gs): it is illegal
    // to apply this function with a system of generators with no
    // points to an empty polyhedron.
    Generator_System gs;
    gs.insert(line(x));
    gs.insert(line(y));
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
test30() {

  C_Polyhedron ph1(3);
  C_Polyhedron ph2(5);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::difference_assign(ph2): it is impossible to apply
    // this function to two polyhedra of different dimensions.
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
test31() {
  C_Polyhedron ph1(3);
  C_Polyhedron ph2(8);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::time_elapse_assign(p): it is illegal to use
    // it with two polyhedra of different dimensions.
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
test32() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(ray(A));
  gs1.insert(line(B));

  const Generator_System gs2 = gs1;

  print_generators(gs2, "*** gs2 ***");

  try {
    // This is an incorrect use of the function
    // `C_Polyhedron::C_Polyhedron(gs)': it is illegal to build a
    // closed polyhedron starting from a constant system of
    // generators that does not contain points.
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
test33() {
  Variable A(0);

  C_Polyhedron ph1(2, EMPTY);

  print_generators(ph1, "*** ph1 ***");

  try {
    // This is an incorrect use of the function
    // `add_generator(g)': it is illegal to add a
    // ray to an empty polyhedron.
    ph1.add_generator(ray(A));
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
test34() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 3);
  ph.add_constraint(A <= 5);

  try {
    // This is an invalid used of the function
    // `C_Polyhedron::bounds_from_above(v, expr, d)': it is illegal to
    // use a variable in the expression that does not appear in the
    // space of the polyhedron.
    ph.bounds_from_above(A + B);
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
test35() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);

  print_constraints(ph, "*** ph ***");

  Generator_System gs;
  gs.insert(line(A));
  gs.insert(ray(B));

  try {
    // This is an invalid used of the function
    // `add_generators(gs)': it is illegal to
    // add a system of generators that does not contain points
    // to an empty polyhedron.
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
test36() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);

  print_constraints(ph, "*** ph ***");

  Generator_System gs;
  gs.insert(ray(A));
  gs.insert(ray(B));

  try {
    // This is an invalid used of the function
    // `add_generators(gs)': it is illegal to
    // add a system of generators that does not contain points
    // to an empty polyhedron.
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
test37() {
  C_Polyhedron ph1(5);
  C_Polyhedron ph2(10);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::BHRZ03_widening_assign(ph2): it is illegal to apply
    // this function to two polyhedra that are not dimensional
    // compatible.
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
test38() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A - B >= 0);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::generalized_affine_image(v, r, expr, d): it is illegal
    // applying the function with a linear expression with the denominator
    // equal to zero.
    Coefficient d = 0;
    ph.generalized_affine_image(B, GREATER_OR_EQUAL, B + 2, d);
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
test39() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 0);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::generalized_affine_image(v, r, expr, d): it is illegal to
    // use a variable in the expression that does not appear in the polyhedron.
    ph.generalized_affine_image(A, GREATER_OR_EQUAL, B);
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
test40() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 1);

  try {
    // This is an invalid used of the function
    // C_Polyhedron::generalized_affine_image(v, r, expr, d): it is illegal to
    // apply this function to a variable that is not in the space of
    // the polyhedron.
    ph.generalized_affine_image(B, LESS_OR_EQUAL, A + 1);
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
test41() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::generalized_affine_image(lhs, r, rhs):
    // it is illegal to use a variable in the `rhs' expression that
    // does not appear in the polyhedron.
    ph.generalized_affine_image(A + B, GREATER_OR_EQUAL, B + C);
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
test42() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 1);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::generalized_affine_image(lhs, r, rhs):
    // it is illegal to use a variable in the `lhs' expression that
    // does not appear in the polyhedron.
    ph.generalized_affine_image(B + C, LESS_OR_EQUAL, A + 1);
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
test43() {
  Generator_System gs;
  Linear_Expression e;
  try {
    // This is an incorrect use of function
    // Generator::ray(e):
    // the origin can not be a ray.
    gs.insert(ray(e));
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
test44() {
  Generator_System gs;
  Linear_Expression e;
  try {
    // This is an incorrect use of function
    // Generator::line(e):
    // the origin can not be a line.
    gs.insert(line(e));
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
test45() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator g = point(3*A - 2*B);
  try {
    // This is an incorrect use of function
    // Generator::coefficient(v):
    // it is impossible to compute the coefficient
    // of a variable that is not in the space of the
    // generator.
    g.coefficient(C);
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
test46() {
  Variable A(0);
  Variable B(1);

  Generator g = line(3*A - 2*B);
  try {
    // This is an incorrect use of method Generator::divisor(): it is
    // illegal to ask for the divisor of a line.
    g.divisor();
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
test47() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint c(2*A - 3*B <= 2);
  try {
    // This is an incorrect use of function
    // Constraint::coefficient(v):
    // it is impossible to compute the coefficient
    // of a variable that is not in the space of the
    // constraint.
    c.coefficient(C);
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
test48() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A - B >= 0);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::bounded_affine_image(v, lb_expr, ub_expr, d):
    // any call with a denominator equal to zero is illegal.
    Coefficient d = 0;
    ph.bounded_affine_image(B, A - 7, B + 2, d);
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
test49() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 0);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to use a variable in the lower bounding expression
    // that does not appear in the polyhedron.
    ph.bounded_affine_image(A, B, A + 7);
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
test50() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 0);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to use a variable in the upper bounding expression
    // that does not appear in the polyhedron.
    ph.bounded_affine_image(A, A + 7, B);
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
test51() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 1);

  try {
    // This is an incorrect use of function
    // C_Polyhedron::bounded_affine_image(v, lb_expr, ub_expr, d):
    // it is illegal to bound a variable not occurring in the
    // vector space embedding the polyhedron.
    ph.bounded_affine_image(B, A - 7, 2*A - 2);
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
test52() {
  Variable A(0);

  C_Polyhedron ph(1);
  ph.add_constraint(A >= 1);

  Pointset_Powerset<C_Polyhedron> ps(2, EMPTY);

  try {
    // This is an incorrect use of function
    // Pointset_Powerset::add_disjunct(ph): the powerset and
    // the added disjunct should have the same space dimension.
    ps.add_disjunct(ph);
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
test53() {
  Variable y(1);

  C_Polyhedron ph(1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::refine_with_constraint(c): it is illegal to insert a
    // constraint that contains a variable that is not in the space
    // of the polyhedron.
    ph.refine_with_constraint(y >= 0);
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
test54() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::refine_with_constraints(cs): it is illegal to
    // refine with a system of constraints that is dimensionally
    // incompatible with the polyhedron.
    Constraint_System cs;
    cs.insert(x - y == 0);
    ph.refine_with_constraints(cs);
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
test55() {
  Variable y(1);

  C_Polyhedron ph(1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::refine_with_congruence(cg): it is illegal to insert a
    // congruence that contains a variable that is not in the space
    // of the polyhedron.
    ph.refine_with_congruence(y %= 0);
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
test56() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(1);

  try {
    // This is an invalid use of the function
    // C_Polyhedron::refine_with_congruences(cgs): it is illegal to
    // refine with a system of congruences that is dimensionally
    // incompatible with the polyhedron.
    Congruence_System cgs;
    cgs.insert(x - y == 0);
    ph.refine_with_congruences(cgs);
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
  DO_TEST(test13);
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
  DO_TEST(test32);
  DO_TEST(test33);
  DO_TEST(test34);
  DO_TEST(test35);
  DO_TEST(test36);
  DO_TEST(test37);
  DO_TEST(test38);
  DO_TEST(test39);
  DO_TEST(test40);
  DO_TEST(test41);
  DO_TEST(test42);
  DO_TEST(test43);
  DO_TEST(test44);
  DO_TEST(test45);
  DO_TEST(test46);
  DO_TEST(test47);
  DO_TEST(test48);
  DO_TEST(test49);
  DO_TEST(test50);
  DO_TEST(test51);
  DO_TEST(test52);
  DO_TEST(test53);
  DO_TEST(test54);
  DO_TEST(test55);
  DO_TEST(test56);
END_MAIN
