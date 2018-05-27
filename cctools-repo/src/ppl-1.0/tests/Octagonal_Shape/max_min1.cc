/* Test Octagonal_Shape::maximize(const Linear_Expression&, ...)
   and Octagonal_Shape::minimize(const Linear_Expression&, ...).
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

  TOctagonal_Shape oct(2);
  oct.add_constraint(-x1-x2 >= -5);
  oct.add_constraint(x1-x2 >= -5);
  oct.add_constraint(x1 >= 0);
  oct.add_constraint(x2 >= 0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = oct.maximize(x1-2*x2, num, den, included)
    && num == 5 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = oct.minimize(x1-2*x2, num, den, included)
    && num == -10 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test02() {
  Variable x1(0);
  Variable x2(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(-x1-x2 >= -5);
  oct.add_constraint(x1-x2 >= -5);
  oct.add_constraint(x1 >= 0);
  oct.add_constraint(x2 >= 0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = oct.maximize(x1-x2, num, den, included)
    && num == 5 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = oct.minimize(x1-x2, num, den, included)
    && num == -5 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test03() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(-x1 - x2 >= -10);
  oct.add_constraint(-10*x1 - 10*x3 >= -60);
  oct.add_constraint(x1 + x3 >= -150);
  oct.add_constraint(x1 >= 0);
  oct.add_constraint(x2 >= 0);
  oct.add_constraint(x3 >= 0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = oct.maximize(-10*x1-6*x2-4*x3+4, num, den, included)
    && num == 4 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = oct.minimize(-10*x1-6*x2-4*x3+4, num, den, included)
    && num == -80 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test04() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(-x1 - x2 >= -10);
  oct.add_constraint(-10*x1 - 10*x3 >= -60);
  oct.add_constraint(x1 + x3 >= -150);
  oct.add_constraint(x1 >= 0);
  oct.add_constraint(x2 >= 0);
  oct.add_constraint(x3 >= 0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = oct.maximize(x2-x3+5, num, den, included)
    && num == 15 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = oct.minimize(x2-x3+5, num, den, included)
    && num == -1 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test05() {
  Variable x1(0);
  Variable x2(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x1 - x2 <= 6);
  oct.add_constraint(x1 + x2 <= 4);
  oct.add_constraint(x1 >= 1);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = oct.maximize(x1-2, num, den, included)
    && num == 3 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = oct.minimize(x1-2, num, den, included)
    && num == -1 && den == 1 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test06() {
  Variable x1(0);

  TOctagonal_Shape oct(1, EMPTY);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = !oct.maximize(x1-2, num, den, included);

  if (!ok)
    return false;

  ok = !oct.minimize(x1-2, num, den, included);

  return ok;
}

bool
test07() {

  TOctagonal_Shape oct(0, EMPTY);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = !oct.maximize(Linear_Expression(2), num, den, included);

  if (!ok)
    return false;

  ok = !oct.minimize(Linear_Expression(2), num, den, included);

  return ok;
}

bool
test08() {

  TOctagonal_Shape oct(0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = oct.maximize(Linear_Expression(2), num, den, included)
    && num == 2 && den == 1 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = oct.minimize(Linear_Expression(2), num, den, included);

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test09() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x1 <= x2 + 1);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Linear_Expression LE(x3);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::maximize(LE, num, den, included): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the space dimension of the OS.
    oct.minimize(LE, num, den, included);
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
test10() {
  TOctagonal_Shape oct(0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE;
  bool ok = oct.maximize(LE, num, den, included, g)
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

  ok = oct.minimize(LE, num, den, included, g)
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
test11() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 1);
  oct.add_constraint(B >= 1);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A + B);
  bool ok = !oct.maximize(LE, num, den, included, g);

  if (!ok)
    return false;

  ok = oct.minimize(LE, num, den, included, g)
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
test12() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 0);
  oct.add_constraint(B >= 0);
  oct.add_constraint(A - B <= 0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A - B);
  bool ok = oct.maximize(LE, num, den, included, g)
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

  ok = !oct.minimize(LE, num, den, included, g);

  return ok;
}

bool
test13() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oct(3);
  oct.add_constraint(A <= 0);
  oct.add_constraint(B - C <= 0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A + B - C);
  bool ok = oct.maximize(LE, num, den, included, g)
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

  ok = !oct.minimize(LE, num, den, included, g);

  return ok;
}

bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oct(5);
  oct.add_constraint(A <= 0);
  oct.add_constraint(B - C <= 0);
  oct.add_constraint(E - D <= 0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(A + B - C + 2*E - 2*D);
  bool ok = oct.maximize(LE, num, den, included, g)
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

  ok = !oct.minimize(LE, num, den, included, g);

  return ok;
}

bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TOctagonal_Shape oct(5);
  oct.add_constraint(A >= 0);
  oct.add_constraint(B - C >= 0);
  oct.add_constraint(E - D >= 0);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(Linear_Expression(3));
  bool ok = oct.maximize(LE, num, den, included, g)
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

  ok = oct.minimize(LE, num, den, included, g)
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
test16() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct(2);
  oct.add_constraint(x >= y);

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE(z);

  try {
    // This is an incorrect use of the method
    // Octagonal_Shape::minimize(LE, num, den, included, g): it is illegal
    // to apply it to an expression whose space dimension is
    // greater than the space dimension of the OS.
    oct.minimize(LE, num, den, included, g);
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
test17() {
  Variable A(0);

  Octagonal_Shape<mpq_class> oct(1);
  oct.add_constraint(5*A <= 2);
  oct.add_constraint(5*A >= 1);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = oct.maximize(3*A, num, den, included)
    && num == 6 && den == 5 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = oct.minimize(3*A, num, den, included)
    && num == 3 && den == 5 && included;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  return ok;
}

bool
test18() {
  Variable A(0);

  Octagonal_Shape<mpq_class> oct(1);
  oct.add_constraint(5*A <= 2);
  oct.add_constraint(3*A >= 1);

  print_constraints(oct, "*** oct ***");

  Coefficient num;
  Coefficient den;
  bool included;
  bool ok = oct.maximize(-7*A, num, den, included)
    && num == -7 && den == 3 && included;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << endl;

  if (!ok)
    return false;

  ok = oct.minimize(-7*A, num, den, included)
    && num == -14 && den == 5 && included;

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
  DO_TEST_F8(test03);
  DO_TEST_F8(test04);
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
END_MAIN
