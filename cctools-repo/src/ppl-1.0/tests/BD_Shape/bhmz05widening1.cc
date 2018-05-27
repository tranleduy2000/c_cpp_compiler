/* Test BD_Shape::BHMZ05_widening_assign().
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

  TBD_Shape bds1(2);
  bds1.add_constraint(x <= 1);
  bds1.add_constraint(x - y <= 2);
  bds1.add_constraint(y - x <= 7);

  TBD_Shape bds2(2);
  bds2.add_constraint(x - y <= 2);
  bds2.add_constraint(-x <= 3);
  bds2.add_constraint(x <= 0);
  bds2.add_constraint(y - x <= 2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.BHMZ05_widening_assign(bds2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x - y <= 2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.BHMZ05_widening_assign(bds2) ***");

  return ok;
}

bool
test02() {
  TBD_Shape bds1;
  TBD_Shape bds2(0, EMPTY);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.BHMZ05_widening_assign(bds2);

  BD_Shape<mpq_class> known_result;

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** after bds1.BHMZ05_widening_assign(bds2) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds1(5);
  bds1.add_constraint(A >= 0);
  bds1.add_constraint(B >= 0);
  bds1.add_constraint(C >= 1);
  bds1.add_constraint(D >= 0);
  bds1.add_constraint(E >= 0);
  bds1.add_constraint(C - D <= 76);
  bds1.add_constraint(C - E <= 76);
  bds1.add_constraint(E - D == 0);

  TBD_Shape bds2(5);
  bds2.add_constraint(A >= 0);
  bds2.add_constraint(B >= 0);
  bds2.add_constraint(C >= 1);
  bds2.add_constraint(D >= 0);
  bds2.add_constraint(E >= 0);
  bds2.add_constraint(C - D <= 75);
  bds2.add_constraint(C - E <= 75);
  bds2.add_constraint(E - D == 0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.BHMZ05_widening_assign(bds2);

  BD_Shape<mpq_class> known_result(5);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(C >= 1);
  known_result.add_constraint(D >= 0);
  known_result.add_constraint(E - D == 0);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.BHMZ05_widening_assign(bds2) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(A - B <= 1);

  TBD_Shape bds2(3);
  bds2.add_constraint(A - B <= 1);
  bds2.add_constraint(A - C <= 1);
  bds2.add_constraint(C - B <= 0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.BHMZ05_widening_assign(bds2);

  BD_Shape<mpq_class> known_result(3);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.BHMZ05_widening_assign(bds2) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(A - B <= 1);
  bds1.add_constraint(A - B >= 2);

  TBD_Shape bds2(3);
  bds2.add_constraint(A - B <= 1);
  bds2.add_constraint(A - B >= 2);
  bds2.add_constraint(A - C <= 1);
  bds2.add_constraint(C - B <= 0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  bds1.BHMZ05_widening_assign(bds2);

  BD_Shape<mpq_class> known_result(3, EMPTY);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.BHMZ05_widening_assign(bds2) ***");

  return ok;
}

TBD_Shape
aux_test06(int i, Variable a, Variable b, Variable c) {
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
test06() {
  Variable a(0);
  Variable b(1);
  Variable c(2);
  unsigned i = 0;
  TBD_Shape m_i = aux_test06(i, a, b, c);
  TBD_Shape m_i_next;
  while (i < 100) {

    using namespace IO_Operators;
    nout << "*** m_" << i << " ***" << endl
         << m_i << endl;

    m_i_next = aux_test06(++i, a, b, c);
    m_i_next.upper_bound_assign(m_i);
    m_i_next.BHMZ05_widening_assign(m_i);
    // Force closure.
    (void) (m_i_next == m_i_next);
    if (m_i == m_i_next) {
      BD_Shape<mpq_class> known_result(3);
      known_result.add_constraint(-1 <= b-c);
      known_result.add_constraint(      b-c <= 1);

      bool ok = (BD_Shape<mpq_class>(m_i) == known_result);

      nout << "*** m_" << i << " (fixpoint) ***" << endl
           << m_i << endl;
      print_constraints(known_result, "*** known_result ***");

      return ok;
    }
    m_i = m_i_next;
  }

  return false;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds1(3);
  bds1.add_constraint(A - B <= 1);

  TBD_Shape bds2(3);
  bds2.add_constraint(A - B <= 1);
  bds2.add_constraint(A - B >= 2);
  bds2.add_constraint(A - C <= 1);
  bds2.add_constraint(C - B <= 0);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  BD_Shape<mpq_class> known_result(bds1);

  bds1.BHMZ05_widening_assign(bds2);

  bool ok = check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1.BHMZ05_widening_assign(bds2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs1;
  cs1.insert(x >= 0);
  cs1.insert(x <= 1);
  cs1.insert(y == 0);

  TBD_Shape bds1(cs1);

  Constraint_System cs2;
  cs2.insert(x <= 1);
  cs2.insert(y >= 0);
  cs2.insert(y - x <= 0);

  TBD_Shape bds2(cs2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  BD_Shape<mpq_class> known_result(bds2);

  bds2.BHMZ05_widening_assign(bds1);

  bool ok = check_result(bds2, known_result);

  print_constraints(bds2,
                    "*** bds2.BHMZ05_widening_assign(bds1) ***");

  return ok;
}

bool
test09() {
  TBD_Shape bds1(5);
  TBD_Shape bds2(10);

  try {
    // This is an invalid use of the method
    // BD_Shape::BHMZ05_widening_assign(bds1): it is illegal to apply
    // the method to two polyhedra that are not dimensional
    // compatible.
    bds2.BHMZ05_widening_assign(bds1);
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
END_MAIN
