/* Test construction of product from another product.
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
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Direct_Product DProduct;
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product CProduct;

namespace {

// Copy constructor.
bool
test01() {
  Variable A(0);
  Variable C(2);

  DProduct dp1(3);
  dp1.refine_with_constraint(A - C == 0);

  DProduct dp(dp1);

  bool ok = (dp == dp1);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok && dp.OK();
}

bool
test02() {
  Variable A(0);
  Variable C(2);

  DProduct dp(3);
  dp.refine_with_constraint(A - C == 0);

  CProduct cp(dp);

  CProduct known_result(3);
  known_result.refine_with_constraint(A - C == 0);
  bool ok = (cp == known_result);

  print_congruences(cp, "*** cp congruences ***");
  print_constraints(cp, "*** cp constraints ***");

  return ok && dp.OK();
}

typedef Domain_Product<TBox, Grid>::Constraints_Product TBox_Grid;
typedef Domain_Product<Grid, TBox>::Constraints_Product Grid_TBox;
typedef Domain_Product<NNC_Polyhedron, Grid>::Constraints_Product NNCPoly_Grid;

// Copy constructor.
bool
test03() {
  Variable A(0);

  const Constraint_System cs(A >= 0);

  NNCPoly_Grid src(1);
  src.refine_with_constraints(cs);

  NNCPoly_Grid prp(src, POLYNOMIAL_COMPLEXITY);

  NNCPoly_Grid prp1(src);

  NNCPoly_Grid known_prp(1);
  known_prp.refine_with_constraint(A >= 0);

  bool ok = (prp == known_prp && prp1 == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");

  return ok && prp.OK();
}

// Constructing an NNCPoly_Grid from a TBox_Grid.
bool
test04() {
  Variable A(0);

  const Constraint_System cs(A >= 0);
  const Congruence_System cgs(A %= 0);

  TBox_Grid src(1);
  src.refine_with_constraints(cs);
  src.refine_with_congruences(cgs);

  NNCPoly_Grid prp(src, POLYNOMIAL_COMPLEXITY);

  NNCPoly_Grid prp1(src);

  NNCPoly_Grid known_prp(1);
  known_prp.refine_with_constraint(A >= 0);
  known_prp.refine_with_congruence(A %= 0);

  bool ok = (prp == known_prp && prp1 == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");

  return ok && prp.OK();
}

// Constructing an NNCPoly_Grid from a Grid_TBox.
bool
test05() {
  Variable A(0);

  const Constraint_System cs(A >= 0);
  const Congruence_System cgs(A %= 0);

  Grid_TBox src(1);
  src.refine_with_constraints(cs);
  src.refine_with_congruences(cgs);

  NNCPoly_Grid prp(src);

  NNCPoly_Grid prp1(src);

  NNCPoly_Grid known_prp(1);
  known_prp.refine_with_constraint(A >= 0);
  known_prp.refine_with_congruence(A %= 0);

  bool ok = (prp == known_prp && prp1 == known_prp);

  print_congruences(src, "*** src congruences ***");
  print_constraints(src, "*** src constraints ***");
  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");
  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");
  print_congruences(known_prp, "*** known_prp congruences ***");
  print_constraints(known_prp, "*** known_prp constraints ***");

  return ok && prp.OK();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
