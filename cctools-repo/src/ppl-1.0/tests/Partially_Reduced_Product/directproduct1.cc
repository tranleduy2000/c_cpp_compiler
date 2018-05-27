/* Test Direct_Product<NNC_Polyhedron, Grid>.
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

using namespace Parma_Polyhedra_Library::IO_Operators;
#include "partially_reduced_product_test.hh"

typedef NNC_Polyhedron DOMAIN1;
typedef Grid DOMAIN2;
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Direct_Product Product;

namespace {

// Universe product in 0 dimensions
bool
test01() {
  Product dp1;

  Product dp2(0, UNIVERSE);

  bool ok = (dp1 == dp2);

  print_congruences(dp1, "*** dp1 congruences ***");
  print_constraints(dp1, "*** dp1 constraints ***");
  print_congruences(dp2, "*** dp2 congruences ***");
  print_constraints(dp2, "*** dp2 constraints ***");

  return ok && dp1.OK() && dp2.OK();
}

// Empty product(dims, type)
bool
test02() {
  Product dp(3, EMPTY);

  bool ok = (dp.is_empty());

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok && dp.OK();
}

// Check equalities are not propagated between components
bool
test03() {
  Variable A(0);

  Product dp(3);

  dp.refine_with_constraint(A >= 7);
  dp.refine_with_constraint(A <= 7);

  DOMAIN1 known_result_d1(1);
  known_result_d1.add_constraint(A == 7);
  DOMAIN2 known_result_d2(1);

  bool ok = ((!dp.domain1().is_universe() && dp.domain2().is_universe())
             || (!dp.domain2().is_universe() && dp.domain1().is_universe()));

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok && dp.OK();
}

// Check emptiness is not propagated between components
bool
test04() {
  Variable A(0);

  Product dp(3);

  dp.refine_with_constraint(A >= 7);
  dp.refine_with_constraint(A <= 5);

  DOMAIN1 known_result_d1(1, EMPTY);
  DOMAIN2 known_result_d2(1);

  bool ok = ((dp.domain1().is_empty() && dp.domain2().is_universe())
             || (dp.domain2().is_empty() && dp.domain1().is_universe()));

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok && dp.OK();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
