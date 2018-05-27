/* Test Polyhedron::maximize(const Linear_Expression&, ...)
   and Polyhedron::minimize(const Linear_Expression&, ...).
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

  C_Polyhedron ph(2);
  ph.add_constraint(-2*x1-x2 >= -5);
  ph.add_constraint(4*x1-4*x2 >= -5);
  ph.add_constraint(x1 >= 0);
  ph.add_constraint(x2 >= 0);

  print_constraints(ph, "*** ph ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  bool ok = ph.maximize(x1-2*x2, num, den, included, g)
    && num == 5 && den == 2 && included
    && g.is_point()
    && g.coefficient(x1) == 5 && g.coefficient(x2) == 0
    && g.divisor() == 2;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  ok = ph.minimize(x1-2*x2, num, den, included, g)
    && num == -15 && den == 4 && included
    && g.is_point()
    && g.coefficient(x1) == 5 && g.coefficient(x2) == 10
    && g.divisor() == 4;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

bool
test02() {
  Variable x1(0);
  Variable x2(1);
  Variable x3(2);

  C_Polyhedron ph(3);
  ph.add_constraint(-x1-x2-x3 >= -100);
  ph.add_constraint(-10*x1-4*x2-5*x3 >= -600);
  ph.add_constraint(-x1-x2-3*x3 >= -150);
  ph.add_constraint(x1 >= 0);
  ph.add_constraint(x2 >= 0);
  ph.add_constraint(x3 >= 0);

  print_constraints(ph, "*** ph ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  bool ok = ph.maximize(-10*x1-6*x2-4*x3+4, num, den, included, g)
    && num == 4 && den == 1 && included
    && g.is_point()
    && g.coefficient(x1) == 0
    && g.coefficient(x2) == 0
    && g.coefficient(x3) == 0
    && g.divisor() == 1;

  nout << (included ? "maximum" : "supremum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  if (!ok)
    return false;

  ok = ph.minimize(-10*x1-6*x2-4*x3+4, num, den, included, g)
    && num == -2188 && den == 3 && included
    && g.is_point()
    && g.coefficient(x1) == 100
    && g.coefficient(x2) == 200
    && g.coefficient(x3) == 0
    && g.divisor() == 3;

  nout << (included ? "minimum" : "infimum") << " = " << num;
  if (den != 1)
    nout << "/" << den;
  nout << " @ ";
  print_generator(g);
  nout << endl;

  return ok;
}

bool
test03() {
  C_Polyhedron ph(0);

  print_constraints(ph, "*** ph ***");

  Coefficient num;
  Coefficient den;
  bool included;
  Generator g(point());
  Linear_Expression LE;
  bool ok = ph.maximize(LE, num, den, included, g)
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

  ok = ph.minimize(LE, num, den, included, g)
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
  C_Polyhedron ph(2, EMPTY);

  print_constraints(ph, "*** ph ***");

  Coefficient num = 0;
  Coefficient den = 0;
  bool included = false;
  Generator g(point());
  Linear_Expression LE;
  bool ok = !ph.maximize(LE, num, den, included, g)
    && num == 0 && den == 0 && !included
    && g.is_point()
    && g.divisor() == 1;
  ok = ok && !ph.minimize(LE, num, den, included, g)
    && num == 0 && den == 0 && !included
    && g.is_point()
    && g.divisor() == 1;

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST_F8(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
