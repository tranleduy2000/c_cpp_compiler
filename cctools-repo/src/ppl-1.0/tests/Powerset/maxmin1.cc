/* Test Pointset_Powerset<PH>::maximize().
        Pointset_Powerset<PH>::minimize().
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

// Powerset of C polyhedra: maximize().
bool
test01() {
  Pointset_Powerset<C_Polyhedron> ps(0, EMPTY);
  Coefficient num = 0;
  Coefficient den = 0;
  bool included = false;
  Generator g(point());
  Linear_Expression LE;
  bool ok = !ps.maximize(LE, num, den, included)
    && num == 0 && den == 0 && !included;
  ok = ok && !ps.maximize(LE, num, den, included, g)
    && num == 0 && den == 0 && !included
    && g.is_point()
    && g.divisor() == 1;

  if (!ok)
    return false;

  ps.add_disjunct(C_Polyhedron(0));
  bool ok1 = ps.maximize(LE, num, den, included)
    && num == 0 && den == 1 && included;
  ok1 = ok1 && ps.maximize(LE, num, den, included, g)
    && num == 0 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok1;
}

// Powerset of NNC polyhedra: maximize().
bool
test02() {
  Variable x(0);
  Constraint_System cs;
  Linear_Expression LE = x;

  Coefficient max_n = 0;
  Coefficient max_d = 0;
  bool max_included = false;
  Generator max_g(point());
  Pointset_Powerset<NNC_Polyhedron> ps(1, EMPTY);

  cs.clear();
  cs.insert(x >= 3);
  cs.insert(x < 14);
  ps.add_disjunct(NNC_Polyhedron(cs));

  bool ok = ps.maximize(LE, max_n, max_d, max_included)
    && max_n == 14 && max_d == 1 && !max_included;
  ok = ok && ps.maximize(LE, max_n, max_d, max_included, max_g)
    && max_n == 14 && max_d == 1 && !max_included
    && max_g.is_closure_point()
    && max_g.divisor() == 1;

  nout << max_n << "/" << max_d;
  nout << " @ ";
  print_generator(max_g);
  nout << endl;

  if (!ok)
    return false;

  cs.clear();
  cs.insert(x >= 3);
  cs.insert(x <= 14);
  ps.add_disjunct(NNC_Polyhedron(cs));

  ok = ps.maximize(LE, max_n, max_d, max_included)
    && max_n == 14 && max_d == 1 && max_included;
  ok = ok && ps.maximize(LE, max_n, max_d, max_included, max_g)
    && max_n == 14 && max_d == 1 && max_included
    && max_g.is_point()
    && max_g.divisor() == 1;

  nout << max_n << "/" << max_d;
  nout << " @ ";
  print_generator(max_g);
  nout << endl;

  return ok;
}

// Powerset of C polyhedra: maximize().
bool
test03() {
  Variable x(0);
  Variable y(1);
  Constraint_System cs;
  Linear_Expression LE = 9*x + y;

  Coefficient num = 0;
  Coefficient den = 0;
  bool included = false;
  Generator g(point());
  Pointset_Powerset<C_Polyhedron> ps(2, EMPTY);

  cs.clear();
  cs.insert(x >= 3);
  cs.insert(3*x <= 14);
  cs.insert(y >= 5);
  cs.insert(11*y <= 87);
  ps.add_disjunct(C_Polyhedron(cs));

  bool ok = ps.maximize(LE, num, den, included)
    && num == 549 && den == 11 && included;
  ok = ok && ps.maximize(LE, num, den, included, g)
    && num == 549 && den == 11 && included
    && g.is_point()
    && g.divisor() == 33;

  nout << num << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  cs.clear();
  cs.insert(x - 3*y >= 5);
  cs.insert(x <= 28);
  cs.insert(y >= 5);
  cs.insert(4*y <= 85);
  ps.add_disjunct(C_Polyhedron(cs));

  ok = ps.maximize(LE, num, den, included)
    && num == 779 && den == 3 && included;
  ok = ok && ps.maximize(LE, num, den, included, g)
    && num == 779 && den == 3 && included
    && g.is_point()
    && g.divisor() == 3;

  nout << num << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

// Powerset of C polyhedra: maximize().
bool
test04() {
  Variable x(0);
  Linear_Expression LE = x;

  Coefficient num = 0;
  Coefficient den = 0;
  bool included = false;
  Generator g(point());
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  bool ok = !ps.maximize(LE, num, den, included)
    && num == 0 && den == 0 && !included;
  ok = ok && !ps.maximize(LE, num, den, included, g)
    && num == 0 && den == 0 && !included
    && g.is_point()
    && g.divisor() == 1;

  if (!ok)
    return false;

  ps.add_disjunct(C_Polyhedron(1));
  ok = !ps.maximize(LE, num, den, included, g)
    && num == 0 && den == 0 && !included
    && g.is_point()
    && g.divisor() == 1;

  return ok;
}

// Powerset of C polyhedra: minimize().
bool
test05() {
  Linear_Expression LE;
  Coefficient num = 0;
  Coefficient den = 0;
  bool included = false;
  Generator g(point());
  Pointset_Powerset<C_Polyhedron> ps(0, EMPTY);

  bool ok = !ps.minimize(LE, num, den, included)
    && num == 0 && den == 0 && !included;
  ok = ok && !ps.minimize(LE, num, den, included, g)
    && num == 0 && den == 0 && !included
    && g.is_point()
    && g.divisor() == 1;

  if (!ok)
    return false;

  ps.add_disjunct(C_Polyhedron(0));
  ok = ps.minimize(LE, num, den, included)
    && num == 0 && den == 1 && included;
  ok = ok && ps.minimize(LE, num, den, included, g)
    && num == 0 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << (included ? "minimum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

// Powerset of NNC polyhedra: minimize().
bool
test06() {
  Variable x(0);
  Constraint_System cs;
  Linear_Expression LE = x;

  Coefficient min_n = 0;
  Coefficient min_d = 0;
  bool min_included = false;
  Generator min_g(point());

  Pointset_Powerset<NNC_Polyhedron> ps(1, EMPTY);

  cs.clear();
  cs.insert(2*x > 3);
  cs.insert(x < 14);
  ps.add_disjunct(NNC_Polyhedron(cs));

  bool ok = ps.minimize(LE, min_n, min_d, min_included)
    && min_n == 3 && min_d == 2 && !min_included;
  ok = ok && ps.minimize(LE, min_n, min_d, min_included, min_g)
    && min_n == 3 && min_d == 2 && !min_included
    && min_g.is_closure_point()
    && min_g.divisor() == 2;

  nout << min_n << "/" << min_d;
  nout << " @ ";
  print_generator(min_g);
  nout << endl;

  if (!ok)
    return false;

  cs.clear();
  cs.insert(2*x >= 3);
  cs.insert(x < 14);
  ps.add_disjunct(NNC_Polyhedron(cs));

  ok = ps.minimize(LE, min_n, min_d, min_included)
    && min_n == 3 && min_d == 2 && min_included;
  ok = ok && ps.minimize(LE, min_n, min_d, min_included, min_g)
    && min_n == 3 && min_d == 2 && min_included
    && min_g.is_point()
    && min_g.divisor() == 2;

  nout << min_n << "/" << min_d;
  nout << " @ ";
  print_generator(min_g);
  nout << endl;

  return ok;
}

// Powerset of C polyhedra: minimize().
bool
test07() {
  Variable x(0);
  Variable y(1);
  Constraint_System cs;
  Linear_Expression LE = x + y;

  Coefficient num = 0;
  Coefficient den = 0;
  bool included = false;
  Generator g(point());
  Pointset_Powerset<C_Polyhedron> ps(2, EMPTY);

  cs.clear();
  cs.insert(x >= 3);
  cs.insert(x <= 4);
  cs.insert(y >= 5);
  cs.insert(y <= 8);
  ps.add_disjunct(C_Polyhedron(cs));

  bool ok = ps.minimize(LE, num, den, included)
    && num == 8 && den == 1 && included;
  ok = ok && ps.minimize(LE, num, den, included, g)
    && num == 8 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << num << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  cs.clear();
  cs.insert(x - y >= 1);
  cs.insert(x <= 8);
  cs.insert(y >= 2);
  cs.insert(y <= 10);
  ps.add_disjunct(C_Polyhedron(cs));

  ok = ps.minimize(LE, num, den, included)
    && num == 5 && den == 1 && included;
  ok = ok && ps.minimize(LE, num, den, included, g)
    && num == 5 && den == 1 && included
    && g.is_point()
    && g.divisor() == 1;

  nout << num << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

// Powerset of C polyhedra: minimize().
bool
test08() {
  Variable x(0);
  Linear_Expression LE = x;

  Coefficient num = 0;
  Coefficient den = 0;
  bool included = false;
  Generator g(point());
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  bool ok = !ps.minimize(LE, num, den, included)
    && num == 0 && den == 0 && !included;
  ok = ok && !ps.minimize(LE, num, den, included, g)
    && num == 0 && den == 0 && !included
    && g.is_point()
    && g.divisor() == 1;

  if (!ok)
    return false;

  ps.add_disjunct(C_Polyhedron(1));
  ok = !ps.minimize(LE, num, den, included, g)
    && num == 0 && den == 0 && !included
    && g.is_point()
    && g.divisor() == 1;

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST_F8(test02);
  DO_TEST_F8(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST_F8(test06);
  DO_TEST(test07);
  DO_TEST(test08);
END_MAIN
