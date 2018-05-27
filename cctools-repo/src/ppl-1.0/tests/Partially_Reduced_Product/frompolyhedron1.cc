/* Test construction of product from a polyhedron.
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

// Product(c_polyhedron).
bool
test01() {
  Variable A(0);

  const Constraint_System cs(A == 0);

  C_Polyhedron ph(cs);

  Product dp(ph);

  Product known_result(1);
  known_result.refine_with_congruence((A %= 0) / 0);

  bool ok = (dp == known_result);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok && dp.OK();
}

// Product(nnc_polyhedron).
bool
test02() {
  Variable A(0);

  const Constraint_System cs(A > 0);

  NNC_Polyhedron ph(cs);

  Product dp(ph);

  Product known_result(1);
  known_result.refine_with_constraint(A > 0);

  bool ok = (dp == known_result);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok && dp.OK();
}

typedef Domain_Product<TBox, Grid>::Constraints_Product TBox_Grid;

// TBox_Grid(nnc_polyhedron, POLYNOMIAL_COMPLEXITY).
bool
test03() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.refine_with_constraint(3*x + y >= 2);
  ph.refine_with_constraint(x <= 4);
  ph.refine_with_constraint(y <= 4);

  TBox_Grid pprp(ph, POLYNOMIAL_COMPLEXITY);

  TBox_Grid nprp(ph);

  TBox_Grid known_prp(2);
  known_prp.refine_with_constraint(3*x >= -2);
  known_prp.refine_with_constraint(x <= 4);
  known_prp.refine_with_constraint(y >= -10);
  known_prp.refine_with_constraint(y <= 4);

  bool ok = (nprp == known_prp && pprp == known_prp);

  print_constraints(ph, "*** ph ***");
  print_constraints(nprp, "*** nprp ***");
  print_congruences(nprp, "*** nprp ***");
  print_constraints(pprp, "*** pprp ***");
  print_congruences(pprp, "*** pprp ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
