/* Test operator==() and operator!=().
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
#define REVERSED_TEST
#include "partially_reduced_product_test.hh"

typedef NNC_Polyhedron DOMAIN1;
typedef Grid DOMAIN2;
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product Product;

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs(A + B >= 9);

  Product dp(2);
  dp.refine_with_constraints(cs);
  dp.refine_with_congruence((A %= 9) / 19);

  Product dp1 = dp;

  bool ok = (dp == dp1);

  print_congruences(dp, "*** dp1 congruences ***");
  print_constraints(dp, "*** dp1 constraints ***");

  return ok && dp.OK() && dp1.OK();
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs(A + B >= 9);

  Product dp(2);
  dp.refine_with_constraints(cs);
  dp.refine_with_congruence((A %= 9) / 19);

  Product dp1(2);

  bool ok = (dp != dp1);

  print_congruences(dp, "*** dp1 congruences ***");
  print_constraints(dp, "*** dp1 constraints ***");

  return ok && dp.OK() && dp1.OK();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
