/* Test Box::Box(const Congruence_System&).
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

// Universe Box constructed from empty congruences
bool
test01() {
  Congruence_System cgs;
  TBox box(cgs);

  Rational_Box known_result(0);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// Box constructed from empty congruences and add_congruences()
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Congruence_System cgs;
  cgs.insert(0*D %= 0);
  TBox box(cgs);

  Rational_Box known_result(4);
  known_result.add_congruences(cgs);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// Box constructed from non-empty congruences
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((C %= 7) / 0);

  TBox box(cgs);

  Rational_Box known_result(3);
  known_result.add_constraint(C == 7);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// add_recycled_congruences()
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A %= 7) / 0);

  TBox box(3);
  box.add_recycled_congruences(cgs);
  bool ok = !box.is_empty();

  Rational_Box known_result(3);
  known_result.add_constraint(A == 7);

  ok = ok && check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// Box constructed from non-empty congruences; congruences().
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + 0*C %= 7) / 0);

  TBox box(cgs);

  TBox box1(box.congruences());

  Rational_Box known_result(3);
  known_result.add_constraint(A == 7);

  bool ok = check_result(box1, known_result);

  print_constraints(box, "*** box ***");
  print_constraints(box1, "*** box1(box.congruences()) ***");

  return ok;
}

// Box constructed from non-empty congruences.
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A %= 7) / 0);
  cgs.insert((B %= 3) / 0);
  // This inconsistent equality is not ignored when refining.
  cgs.insert((A + B %= 0) / 0);
  cgs.insert(C %= 7);

  TBox box(cgs.space_dimension(), UNIVERSE);
  box.refine_with_congruences(cgs);

  Rational_Box known_result(3, EMPTY);

  bool ok = check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// add_congruences() for inconsistent equality congruences
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A %= 7) / 0);
  cgs.insert((B %= 3) / 0);
  cgs.insert((A %= 0) / 0);

  TBox box(3);
  box.add_congruences(cgs);
  bool ok = box.is_empty();

  Rational_Box known_result(3, EMPTY);

  ok = ok && check_result(box, known_result);

  print_constraints(box, "*** box ***");

  return ok;
}

// add_congruence()
bool
test08() {
  Variable A(0);

  TBox box(1);
  box.add_congruence((A %= 7) / 0);
  bool ok = !box.is_empty();
  box.add_congruence((A %= 2) / 0);
  ok = ok && box.is_empty();

  Rational_Box known_result(1, EMPTY);

  ok = ok && check_result(box, known_result);

  print_constraints(box, "*** box ***");
  print_constraints(known_result, "*** known_result ***");

  return ok;
}

// Non-empty Box; minimized_congruences().
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A %= 7) / 0);
  cgs.insert((C %= 3) / 0);

  TBox box(cgs);
  TBox box1(box.minimized_congruences());

  Rational_Box known_result(3);
  known_result.add_constraint(A == 7);
  known_result.add_constraint(C == 3);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1(box.congruences()) ***");

  return ok;
}

// Empty Box; minimized_congruences().
bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A %= 7) / 0);
  cgs.insert((B %= 3) / 0);
  cgs.insert((A %= 0) / 0);

  TBox box(3);
  box.add_congruences(cgs);

  TBox box1(box.minimized_congruences());

  Rational_Box known_result(3, EMPTY);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1(box.congruences()) ***");

  return ok;
}

// Zero dimension universe; congruences()
bool
test11() {
  TBox box(0);
  TBox box1(box.congruences());

  Rational_Box known_result(0);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1(box.congruences()) ***");

  return ok;
}

// Zero dimension empty; congruences()
bool
test12() {
  TBox box(0, EMPTY);
  TBox box1(box.congruences());

  Rational_Box known_result(0, EMPTY);

  bool ok = check_result(box1, known_result);

  print_constraints(box1, "*** box1(box.congruences()) ***");

  return ok;
}

bool
test13() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBox box1(2);

  try {
    box1.add_congruence((x %= 0) / 0);
    // This is an invalid use of method
    // Box::add_congruence: it is illegal
    // to add a congruence with bigger dimension.
    box1.add_congruence(y - x + z %= 0);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test14() {
  Variable x(0);
  Variable y(1);

  TBox box(1);

  try {
    // This is an invalid use of the method
    // Box::add_congruences(cs): it is illegal to
    // add a system of congruences that is dimensional incompatible
    // with the box.
    Congruence_System cgs;
    cgs.insert(x - y %= 0);
    box.add_congruences(cgs);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test15() {
  Variable y(1);

  TBox box(1);

  try {
    // This is an invalid use of the method
    // Box::add_congruence(c): it is illegal to insert a
    // congruence that contains a variable that is not in the space
    // of the box.
    box.add_congruence((y %= 0) / 0);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
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
END_MAIN
