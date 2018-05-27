/* Test Product<NNC_Polyhedron, Grid>::time_elapse_assign().
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

// time_elapse_assign(y)
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

  Product prp2(3);
  prp2.refine_with_constraint(7*C == 4);
  prp2.refine_with_constraint(7*B == -1);
  prp2.refine_with_constraint(7*A == 3);

  prp1.time_elapse_assign(prp2);

  Product known_prp(3);
  known_prp.refine_with_constraint(5*A - 13*B - 7*C == 0);
  known_prp.refine_with_constraint(3*A + C >= 0);
  known_prp.refine_with_constraint(A + B >= 3);
  known_prp.refine_with_constraint(4*A - 3*C >= 13);
  known_prp.refine_with_congruence((65*A - B %= 0) / 7);
  known_prp.refine_with_congruence(21*A %= 0);
  known_prp.refine_with_constraint(A >= 0);

  bool ok = (prp1 == known_prp);

  print_congruences(prp1, "*** prp1.time_elapse_assign(prp1) congruences ***");
  print_constraints(prp1, "*** prp1.time_elapse_assign(prp1) constraints ***");
  print_congruences(prp2, "*** prp2.time_elapse_assign(prp2) congruences ***");
  print_constraints(prp2, "*** prp2.time_elapse_assign(prp2) constraints ***");

  return ok;
}

// time_elapse_assign(y) where the initial products are not reduced
// and the second product has non-intersecting single point components.
bool
test02() {
  Variable A(0);


  Product prp1(1);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");

  Grid gr(1);
  gr.refine_with_congruence((A %= 0)/ 2);

  Product prp2(gr);

  prp2.refine_with_constraint(A >= 1);
  prp2.refine_with_constraint(A <= 1);

  prp1.time_elapse_assign(prp2);

  Product known_prp(1, EMPTY);

  bool cons_ok = prp1.OK() && (prp1 == known_prp);

  print_congruences(prp1,
                    "*** prp1.time_elapse_assign(prp2) congruences ***");
  print_constraints(prp1,
                    "*** prp1.time_elapse_assign(prp2) constraints ***");

  print_congruences(prp2, "*** prp2 congruences ***");
  print_constraints(prp2, "*** prp2 constraints ***");

  return cons_ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST(test02);
END_MAIN
