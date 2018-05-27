/* Test Product<NNC_Polyhedron, Grid>::maximize() and
   Product<NNC_Polyhedron, Grid>::minimize()
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
#include "partially_reduced_product_test.hh"

typedef NNC_Polyhedron DOMAIN1;
typedef Grid DOMAIN2;
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product Product;

namespace {

// Empty. maximize() and minimize()
bool
test01() {
  Product prp(7, EMPTY);

  Coefficient extr_n;
  Coefficient extr_d;
  bool dummy;

  bool ok = (!prp.maximize(Linear_Expression(0), extr_n, extr_d, dummy));

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Zero dimension empty.
bool
test02() {
  Product prp(0, EMPTY);

  Coefficient extr_n;
  Coefficient extr_d;
  bool dummy;
  Generator pnt(point());

  bool ok = !prp.maximize(Linear_Expression(0), extr_n, extr_d, dummy)
    && !prp.maximize(Linear_Expression(0), extr_n, extr_d, dummy, pnt);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Zero dimension universe.
bool
test03() {
  Product prp(0);

  Coefficient extr_n, extr_d;
  bool dummy;
  Generator pnt(point());

  bool ok = prp.maximize(Linear_Expression(0), extr_n, extr_d, dummy)
    && prp.maximize(Linear_Expression(0), extr_n, extr_d, dummy, pnt);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Point.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Product prp(2);
  prp.refine_with_constraint(A == 1);
  prp.refine_with_constraint(3*B == 2);

  Linear_Expression le = A + B;

  Coefficient max_n;
  Coefficient max_d;
  Coefficient min_n;
  Coefficient min_d;
  Generator pnt_max(point());
  Generator pnt_min(point());
  Generator known_pnt(point(3*A + 2*B, 3));
  bool max;
  bool min;

  bool ok = prp.maximize(le, max_n, max_d, max)
    && prp.minimize(le, min_n, min_d, min)
    && prp.maximize(le, max_n, max_d, max, pnt_max)
    && prp.minimize(le, min_n, min_d, min, pnt_min);

  ok = ok
    && max && min && max_n == 5 && max_d == 3 && min_n == 5 && min_d == 3
    && pnt_max == known_pnt && pnt_min == known_pnt;

  print_generator(pnt_max, "*** max point ***");
  print_generator(pnt_min, "*** min point ***");

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// only one component is bounded.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Product prp(2);
  prp.refine_with_constraint(A - B >= 1);
  prp.refine_with_constraint(A - B <= 1);
  prp.refine_with_congruence(3*B %= 2);

  Linear_Expression le = A - B;

  Coefficient max_n;
  Coefficient max_d;
  Coefficient min_n;
  Coefficient min_d;
  Generator pnt_max(point());
  Generator pnt_min(point());
  bool max;
  bool min;

  bool ok = prp.maximize(le, max_n, max_d, max)
    && prp.minimize(le, min_n, min_d, min)
    && prp.maximize(le, max_n, max_d, max, pnt_max)
    && prp.minimize(le, min_n, min_d, min, pnt_min);

  ok = ok
    && max && min && max_n == 1 && max_d == 1 && min_n == 1 && min_d == 1
    && pnt_max == pnt_min;

  print_generator(pnt_max, "*** maximum point ***");

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// Line and neither component is bounded.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Product prp(2);
  prp.refine_with_constraint(B == 1);

  Linear_Expression le = 2*A - B;

  Coefficient max_n;
  Coefficient max_d;
  Coefficient min_n;
  Coefficient min_d;
  Generator pnt_max(point());
  Generator pnt_min(point());
  bool max;
  bool min;

  bool ok = !prp.maximize(le, max_n, max_d, max)
    && !prp.minimize(le, min_n, min_d, min)
    && !prp.maximize(le, max_n, max_d, max, pnt_max)
    && !prp.minimize(le, min_n, min_d, min, pnt_min);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// only one component is strictly bounded.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Product prp(2);

  prp.refine_with_constraint(A - B > 0);
  prp.refine_with_constraint(A - B < 1);
  prp.refine_with_congruence(3*B %= 2);

  Linear_Expression le = A - B;

  Coefficient max_n;
  Coefficient max_d;
  Coefficient min_n;
  Coefficient min_d;
  Generator pnt_max(point());
  Generator pnt_min(point());

  bool max;
  bool min;

  bool ok = prp.maximize(le, max_n, max_d, max)
    && prp.minimize(le, min_n, min_d, min)
    && prp.maximize(le, max_n, max_d, max, pnt_max)
    && prp.minimize(le, min_n, min_d, min, pnt_min);

  ok = ok && !max && !min;

  ok = ok && max_n == 1 && max_d == 1 && min_n == 0 && min_d == 1;

  print_generator(pnt_max, "*** maximum point ***");

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

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
END_MAIN
