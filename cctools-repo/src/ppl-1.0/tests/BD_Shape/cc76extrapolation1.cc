/* Test BD_Shape::CC76_extrapolation_assign().
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

  TBD_Shape bds1(3);
  bds1.add_constraint(x <= 1);
  bds1.add_constraint(x - y <= 2);
  bds1.add_constraint(y - x <= 7);

  TBD_Shape bds2(3);
  bds2.add_constraint(x - y <= 2);
  bds2.add_constraint(-x <= 3);
  bds2.add_constraint(x <= 0);
  bds2.add_constraint(y - x <= 2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.CC76_extrapolation_assign(bds2);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(x - y <= 2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.CC76_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds1(4);
  bds1.add_constraint(A >= 0);
  bds1.add_constraint(B >= 0);
  bds1.add_constraint(B <= 25);
  bds1.add_constraint(C >= 0);
  bds1.add_constraint(C <= 29);
  bds1.add_constraint(D >= 0);
  bds1.add_constraint(D <= 27);
  bds1.add_constraint(B - A <= 25);
  bds1.add_constraint(C - A <= 29);
  bds1.add_constraint(D - A <= 27);
  bds1.add_constraint(B - C <= 2);
  bds1.add_constraint(C - B <= 6);
  bds1.add_constraint(B - D <= 2);
  bds1.add_constraint(D - B <= 4);
  bds1.add_constraint(C - D <= 4);
  bds1.add_constraint(D - C <= 4);

  TBD_Shape bds2(4);
  bds2.add_constraint(A >= 0);
  bds2.add_constraint(B >= 0);
  bds2.add_constraint(B <= 26);
  bds2.add_constraint(C >= 3);
  bds2.add_constraint(C <= 29);
  bds2.add_constraint(D >= 2);
  bds2.add_constraint(D <= 28);
  bds2.add_constraint(B - A <= 26);
  bds2.add_constraint(C - A <= 29);
  bds2.add_constraint(D - A <= 28);
  bds2.add_constraint(B - C <= 0);
  bds2.add_constraint(C - B <= 6);
  bds2.add_constraint(B - D == 2);
  bds2.add_constraint(C - D <= 4);
  bds2.add_constraint(D - C <= 2);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B >= 0);
  cs.insert(C >= 0);
  cs.insert(D >= 0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");
  print_constraints(cs, "*** cs ***");

  bds1.upper_bound_assign(bds2);

  bds1.limited_CC76_extrapolation_assign(bds2, cs);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 26);
  known_result.add_constraint(C >= 0);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(B - A <= 26);
  known_result.add_constraint(B - C <= 2);
  known_result.add_constraint(B - D <= 2);
  known_result.add_constraint(C - D <= 4);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "bds1.limited_CC76_extrapolation_assign(bds2, cs) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBD_Shape bds1(4);
  bds1.add_constraint(A >= 0);
  bds1.add_constraint(B >= 0);
  bds1.add_constraint(B <= 39);
  bds1.add_constraint(C >= 0);
  bds1.add_constraint(C <= 40);
  bds1.add_constraint(D >= 0);
  bds1.add_constraint(D <= 40);
  bds1.add_constraint(B - A <= 39);
  bds1.add_constraint(C - A <= 40);
  bds1.add_constraint(D - A <= 40);
  bds1.add_constraint(B - C <= 0);
  bds1.add_constraint(C - B <= 1);
  bds1.add_constraint(B - D <= 0);
  bds1.add_constraint(D - B <= 2);
  bds1.add_constraint(C - D <= 0);
  bds1.add_constraint(D - C <= 1);

  TBD_Shape bds2(4);
  bds2.add_constraint(A >= 0);
  bds2.add_constraint(B >= 0);
  bds2.add_constraint(B <= 38);
  bds2.add_constraint(C >= 0);
  bds2.add_constraint(C <= 39);
  bds2.add_constraint(D >= 0);
  bds2.add_constraint(D <= 40);
  bds2.add_constraint(B - A <= 38);
  bds2.add_constraint(C - A <= 39);
  bds2.add_constraint(D - A <= 40);
  bds2.add_constraint(B - C <= 0);
  bds2.add_constraint(C - B <= 1);
  bds2.add_constraint(B - D <= 0);
  bds2.add_constraint(D - B <= 2);
  bds2.add_constraint(C - D <= 0);
  bds2.add_constraint(D - C <= 1);

  // Force the closure of `bds1'.
  (void) (bds1 == bds2);

  BD_Shape<mpq_class> known_widening(4);
  known_widening.add_constraint(A >= 0);
  known_widening.add_constraint(B >= 0);
  known_widening.add_constraint(C >= 0);
  known_widening.add_constraint(D >= 0);
  known_widening.add_constraint(D <= 40);
  known_widening.add_constraint(D - A <= 40);
  known_widening.add_constraint(B - C <= 0);
  known_widening.add_constraint(C - B <= 1);
  known_widening.add_constraint(B - D <= 0);
  known_widening.add_constraint(D - B <= 2);
  known_widening.add_constraint(C - D <= 0);
  known_widening.add_constraint(D - C <= 1);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.CC76_extrapolation_assign(bds2);

  print_constraints(bds1, "*** bds1.CC76_extrapolation_assign(bds2) ***");

  // Force the closure of `bds1'.
  (void) (bds1 == bds2);

  bool ok = (BD_Shape<mpq_class>(bds1) == known_widening);

  print_constraints(bds1, "*** bds1.closure_assign() ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 0);
  bds1.add_constraint(B >= 0);
  bds1.add_constraint(B <= 2);
  bds1.add_constraint(B - A <= 2);

  TBD_Shape bds2(2);
  bds2.add_constraint(A <= 0);
  bds2.add_constraint(B >= 0);
  bds2.add_constraint(B <= 1);
  bds2.add_constraint(B - A <= 1);

  BD_Shape<mpq_class> known_result(bds1);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.CC76_extrapolation_assign(bds2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.CC76_extrapolation_assign(bds2) ***");

  return ok;
}

TBD_Shape
aux_test05(int i, Variable a, Variable b, Variable c) {
  TBD_Shape bds(3);
  if (i == 0) {
    bds.add_constraint(0 <= a-b);
    bds.add_constraint(     a-b <= 0);
    bds.add_constraint(-1 <= b-c);
    bds.add_constraint(      b-c <= 1);
  }
  else {
    bds.add_constraint(-i <= a-b);
    bds.add_constraint(      a-b <= i);
    bds.add_constraint(-1 <= b-c);
    bds.add_constraint(      b-c <= 1);
    bds.add_constraint(-i <= a-c);
    bds.add_constraint(      a-c <= i);
  }

  using namespace IO_Operators;
  nout << "*** n_" << i << " ***" << endl
       << bds << endl;

  // Force closure.
  (void) (bds == bds);
  return bds;
}

bool
test05() {
  Variable a(0);
  Variable b(1);
  Variable c(2);
  unsigned i = 0;
  TBD_Shape m_i = aux_test05(i, a, b, c);
  TBD_Shape m_i_next;
  while (i < 100) {

    using namespace IO_Operators;
    nout << "*** m_" << i << " ***" << endl
         << m_i << endl;

    m_i_next = aux_test05(++i, a, b, c);
    TBD_Shape::coefficient_type_base* no_stop_points = 0;
    m_i_next.CC76_extrapolation_assign(m_i, no_stop_points, no_stop_points);
    m_i_next.upper_bound_assign(m_i);
    // Force closure.
    (void) (m_i_next == m_i_next);
    if (m_i == m_i_next) {

      nout << "*** m_" << i << " (fixpoint) ***" << endl
           << m_i << endl;

      return false;
    }
    m_i = m_i_next;
  }
  return true;
}

bool
test06() {
  TBD_Shape bds1(0);

  TBD_Shape bds2(0, EMPTY);

  BD_Shape<mpq_class> known_result(bds1);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.CC76_extrapolation_assign(bds2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.CC76_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test07() {
  TBD_Shape bds1(2, EMPTY);
  TBD_Shape bds2(2, EMPTY);
  BD_Shape<mpq_class> known_result(bds2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds2.CC76_extrapolation_assign(bds1);

  bool ok = check_result(bds2, known_result);

  print_constraints(bds2,
                    "*** bds2.CC76_extrapolation_assign(bds1) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(A - B >= 2);
  bds1.add_constraint(B - C >= -1);
  bds1.add_constraint(C - A >= -3);

  TBD_Shape bds2(3);
  bds2.add_constraint(A - B >= 2);
  bds2.add_constraint(B - C >= 1);
  bds2.add_constraint(C - A >= 3);

  BD_Shape<mpq_class> known_result(bds1);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.CC76_extrapolation_assign(bds2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.CC76_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds1(2);
  bds1.add_constraint(A <= 2);

  TBD_Shape bds2(2);
  bds2.add_constraint(A <= 1);
  bds2.add_constraint(B == -1);

  BD_Shape<mpq_class> known_result(bds1);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.CC76_extrapolation_assign(bds2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.CC76_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(C <= 4);
  bds1.add_constraint(B >= 2);

  TBD_Shape bds2(3);
  bds2.add_constraint(C == 3);
  bds2.add_constraint(A - C >= 0);
  bds2.add_constraint(B - A >= 1);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(B >= 2);
  known_result.add_constraint(C - B <= 2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.CC76_extrapolation_assign(bds2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1,
                    "*** bds1.CC76_extrapolation_assign(bds2) ***");

  return ok;
}

bool
test11() {
  TBD_Shape bds1(1);
  TBD_Shape bds2(2);

  try {
    // This is an invalid use of the method
    // BD_Shape::CC76_extrapolation_assign(bds): it is
    // illegal to apply this method to two polyhedra that are not
    // dimension-compatible.
    bds2.CC76_extrapolation_assign(bds1);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test12() {
  TBD_Shape bds1(5);
  TBD_Shape bds2(10);

  try {
    // This is an invalid use of the method
    // BD_Shape::CC76_widening_assign(bds2): it is illegal to apply
    // this method to two polyhedra that are not dimensional
    // compatible.
    bds2.CC76_extrapolation_assign(bds1);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << endl;
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
END_MAIN
