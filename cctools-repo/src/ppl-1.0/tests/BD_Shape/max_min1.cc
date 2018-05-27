/* Test BD_Shape::maximize(const Linear_Expression&, ...)
   and BD_Shape::minimize(const Linear_Expression&, ...).
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
  Variable x1(0);
  Variable x2(1);

  TBD_Shape bds(2);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  bool ok = !bds.maximize(x1-2*x2, num, den, included, g);

  if (!ok)
    return false;

  ok = !bds.minimize(x1-2*x2, num, den, included, g);

  return ok;
}

bool
test02() {
  Variable x1(0);
  Variable x2(1);

  TBD_Shape bds(2, EMPTY);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  bool ok = !bds.maximize(-10*x1-6*x2+4, num, den, included, g);

  if (!ok)
    return false;

  ok = !bds.minimize(-10*x1-6*x2+4, num, den, included, g);

  return ok;
}

bool
test03() {

  TBD_Shape bds(0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE;
  bool ok = bds.maximize(LE, num, den, included, g)
    && num == 0 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(LE, num, den, included, g)
    && num == 0 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);
  bds.add_constraint(B >= 1);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A + B);
  bool ok = !bds.maximize(LE, num, den, included, g);

  if (!ok)
    return false;

  ok = bds.minimize(LE, num, den, included, g)
    && num == 2 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A <= 0);
  bds.add_constraint(B >= 0);
  bds.add_constraint(A - B <= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A - B);
  bool ok = bds.maximize(LE, num, den, included, g)
    && num == 0 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  ok = !bds.minimize(LE, num, den, included, g);

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBD_Shape bds(3);
  bds.add_constraint(A <= 0);
  bds.add_constraint(B - C <= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A + B - C);
  bool ok = bds.maximize(LE, num, den, included, g)
    && num == 0 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  ok = !bds.minimize(LE, num, den, included, g);

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds(5);
  bds.add_constraint(A <= 0);
  bds.add_constraint(B - C <= 0);
  bds.add_constraint(E - D <= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A + B - C + 2*E - 2*D);
  bool ok = bds.maximize(LE, num, den, included, g)
    && num == 0 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  ok = !bds.minimize(LE, num, den, included, g);

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds(5);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B - C >= 0);
  bds.add_constraint(E - D >= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A + B - C + 2*E - 2*D);
  bool ok = !bds.maximize(LE, num, den, included, g);

  if (!ok)
    return false;

  ok = bds.minimize(LE, num, den, included, g)
    && num == 0 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds(5);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B - C >= 0);
  bds.add_constraint(B - C <= -1);
  bds.add_constraint(E - D >= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A + B - C + 2*E - 2*D);
  bool ok = !bds.maximize(LE, num, den, included, g)
    && ! bds.minimize(LE, num, den, included, g);

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBD_Shape bds(5);
  bds.add_constraint(A >= 0);
  bds.add_constraint(B - C >= 0);
  bds.add_constraint(E - D >= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(Linear_Expression(3));
  bool ok = bds.maximize(LE, num, den, included, g)
    && num == 3 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(LE, num, den, included, g)
    && num == 3 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(2);
  bds.add_constraint(x >= y);

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(z);

  try {
    // This is an incorrect use of the method
    // BD_Shape::minimize(LE, num, den, included, g): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the space dimension of the BDS.
    bds.minimize(LE, num, den, included, g);
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
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(2);
  bds.add_constraint(x >= y);

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(z);

  try {
    // This is an incorrect use of the method
    // BD_Shape::maximize(LE, num, den, included, g): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the space dimension of the BDS.
    bds.maximize(LE, num, den, included, g);
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
test13() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);
  bds.add_constraint(A <= 5);
  bds.add_constraint(B <= 3);
  bds.add_constraint(B >= -5);
  bds.add_constraint(A - B <= 6);
  bds.add_constraint(B - A <= 2);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = bds.maximize(A-2, num, den, included)
    && num == 3 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(A-2, num, den, included)
    && num == -1 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);
  bds.add_constraint(A <= 5);
  bds.add_constraint(B <= 3);
  bds.add_constraint(B >= -5);
  bds.add_constraint(A - B <= 6);
  bds.add_constraint(B - A <= 2);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = bds.maximize(A+2, num, den, included)
    && num == 7 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(A+2, num, den, included)
    && num == 3 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 100);
  bds.add_constraint(B <= 55);
  bds.add_constraint(B >= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = bds.maximize(-A+2, num, den, included)
    && num == 2 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(-A+2, num, den, included)
    && num == -98 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test16() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 100);
  bds.add_constraint(B <= 55);
  bds.add_constraint(B >= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = bds.maximize(-A-2, num, den, included)
    && num == -2 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(-A-2, num, den, included)
    && num == -102 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test17() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);
  bds.add_constraint(A <= 5);
  bds.add_constraint(B <= 3);
  bds.add_constraint(B >= -5);
  bds.add_constraint(A - B <= 6);
  bds.add_constraint(B - A <= 2);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = bds.maximize(A - B - 2, num, den, included)
    && num == 4 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(A - B - 2, num, den, included)
    && num == -4 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 1);
  bds.add_constraint(A <= 5);
  bds.add_constraint(B <= 3);
  bds.add_constraint(B >= -5);
  bds.add_constraint(A - B <= 6);
  bds.add_constraint(B - A <= 2);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = bds.maximize(A - B + 2, num, den, included)
    && num == 8 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(A - B + 2, num, den, included)
    && num == 0 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test19() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 100);
  bds.add_constraint(B <= 55);
  bds.add_constraint(B >= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = bds.maximize(-A + B + 2, num, den, included)
    && num == 57 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(-A + B + 2, num, den, included)
    && num == -98 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test20() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 100);
  bds.add_constraint(B <= 55);
  bds.add_constraint(B >= 0);

  print_constraints(bds, "*** bds ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = bds.maximize(-A + B - 2, num, den, included)
    && num == 53 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = bds.minimize(-A + B - 2, num, den, included)
    && num == -102 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

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
  DO_TEST(test16);;
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
  DO_TEST(test20);
END_MAIN
