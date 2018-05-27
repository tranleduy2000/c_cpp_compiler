/* Test Product<NNC_Polyhedron, Grid>::drop_some_non_integer_points().
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

// drop_some_non_integer_points(ANY_COMPLEXITY)
bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp1(3);
  prp1.refine_with_constraint(A >= 0);
  prp1.refine_with_constraint(B >= 0);
  prp1.refine_with_constraint(A + B >= 3);
  prp1.refine_with_constraint(2*A - B == 0);
  prp1.refine_with_constraint(3*A + C == 0);
  prp1.refine_with_congruence(3*A %= 0);

  prp1.drop_some_non_integer_points(ANY_COMPLEXITY);

  Product known_prp(3);
  known_prp.refine_with_constraint(3*A + C == 0);
  known_prp.refine_with_constraint(2*A - B == 0);
  known_prp.refine_with_constraint(A >= 1);
  known_prp.refine_with_congruence(A %= 0);

  bool ok = (prp1 == known_prp);

  print_congruences(prp1, "*** prp1.time_elapse_assign(prp1) congruences ***");
  print_constraints(prp1, "*** prp1.time_elapse_assign(prp1) constraints ***");

  return ok;
}

// drop_some_non_integer_points(ANY_COMPLEXITY)
// where the initial products are not reduced
// and the second product has non-intersecting single point components.
bool
test02() {
  Variable A(0);


  Product prp1(1);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");

  prp1.drop_some_non_integer_points(ANY_COMPLEXITY);

  Product known_prp(1);
  known_prp.refine_with_congruence(A %= 0);

  bool ok = prp1.OK() && (prp1 == known_prp);

  print_congruences(prp1,
                    "*** prp1.drop_some_non_integer_points(ANY_COMPLEXITY) congruences ***");
  print_constraints(prp1,
                    "*** prp1.drop_some_non_integer_points(ANY_COMPLEXITY) constraints ***");

  return ok;
}

// drop_some_non_integer_points(vars, ANY_COMPLEXITY)
bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Variables_Set vars;
  vars.insert(A);

  Product prp1(3);
  prp1.refine_with_constraint(A >= 0);
  prp1.refine_with_constraint(B >= 0);
  prp1.refine_with_constraint(A + B >= 3);
  prp1.refine_with_constraint(2*A - B == 0);
  prp1.refine_with_constraint(3*A + C == 0);
  prp1.refine_with_congruence(3*A %= 0);

  prp1.drop_some_non_integer_points(vars, ANY_COMPLEXITY);

  Product known_prp(3);
  known_prp.refine_with_constraint(3*A + C == 0);
  known_prp.refine_with_constraint(2*A - B == 0);
  known_prp.refine_with_constraint(A >= 1);
  known_prp.refine_with_congruence(A %= 0);

  bool ok = (prp1 == known_prp);

  print_congruences(prp1, "*** prp1.time_elapse_assign(prp1) congruences ***");
  print_constraints(prp1, "*** prp1.time_elapse_assign(prp1) constraints ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
