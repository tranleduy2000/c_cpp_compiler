/* Test
   Product<NNC_Polyhedron, Grid>::add_space_dimensions_and_embed() and
   Product<NNC_Polyhedron, Grid>::add_space_dimensions_and_project() and
   Product<NNC_Polyhedron, Grid>::remove_space_dimensions() and
   Product<NNC_Polyhedron, Grid>::remove_higher_space_dimensions() and
   Product<NNC_Polyhedron, Grid>::fold_space_dimensions() and
   Product<NNC_Polyhedron, Grid>::expand_space_dimension() and
   Product<NNC_Polyhedron, Grid>::map_space_dimensions().
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

// add_space_dimensions_and_embed()
bool
test01() {
  Variable A(0);
  Variable B(1);

  Product prp1(2);
  prp1.refine_with_constraint(A >= 0);
  prp1.refine_with_congruence((A %= 0) / 2);

  prp1.add_space_dimensions_and_embed(3);

  Product known_prp(5);
  known_prp.refine_with_congruence((A %= 0) / 2);
  known_prp.refine_with_constraint(A >= 0);

  bool ok = (prp1 == known_prp);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");

  return ok;
}

// add_space_dimensions_and_project()
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp1(2);
  prp1.refine_with_constraint(A >= 0);
  prp1.refine_with_congruence((A %= 0) / 2);

  prp1.add_space_dimensions_and_project(1);

  Product known_prp(3);
  known_prp.refine_with_congruence((A %= 0) / 2);
  known_prp.refine_with_constraint(A >= 0);
  known_prp.refine_with_constraint(C == 0);

  bool ok = (prp1 == known_prp);

  print_congruences(prp1, "*** prp1 congruences ***");
  print_constraints(prp1, "*** prp1 constraints ***");

  return ok;
}

// remove_space_dimensions()
bool
test03() {
  Variable A(0);
  Variable C(2);
  Variable D(3);

  Product prp(4);
  prp.refine_with_constraint(A >= 0);
  prp.refine_with_congruence((A %= 0) / 2);
  prp.refine_with_congruence((A - C %= 0) / 2);

  Variables_Set vars;
  vars.insert(C);
  vars.insert(D);

  prp.remove_space_dimensions(vars);

  Product known_prp(2);
  known_prp.refine_with_constraint(A >= 0);
  known_prp.refine_with_congruence((A %= 0) / 2);

  bool ok = (prp == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp ***");

  return ok;
}

// remove_higher_space_dimensions()
bool
test04() {
  Variable A(0);
  Variable C(2);
  Variable D(3);

  Product prp(4);
  prp.refine_with_constraint(A >= 0);
  prp.refine_with_congruence((A %= 0) / 2);
  prp.refine_with_congruence((A - C %= 0) / 2);

  prp.remove_higher_space_dimensions(2);

  Product known_prp(2);
  known_prp.refine_with_constraint(A >= 0);
  known_prp.refine_with_congruence((A %= 0) / 2);

  bool ok = (prp == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// map_space_dimensions()
bool
test05() {
  Variable A(0);
  Variable B(1);

  Product prp(2);
  prp.refine_with_constraint(A >= 0);
  prp.refine_with_congruence((A - B %= 0) / 2);

  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);

  prp.map_space_dimensions(function);

  Product known_prp(2);
  known_prp.refine_with_constraint(B >= 0);
  known_prp.refine_with_congruence((B - A %= 0) / 2);

  bool ok = (prp == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// expand_space_dimension()
bool
  test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Product prp(3);
  prp.refine_with_congruence((A + B %= 2) / 7);
  prp.refine_with_constraint(A >= 0);

  prp.expand_space_dimension(A, 1);

  Product known_prp(4);
  known_prp.refine_with_congruence((A + B %= 2) / 7);
  known_prp.refine_with_congruence((D + B %= 2) / 7);
  known_prp.refine_with_constraint(A >= 0);
  known_prp.refine_with_constraint(D >= 0);

  bool ok = (prp == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// fold_space_dimensions()
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product prp(3);
  prp.refine_with_congruence((A %= 2) / 7);
  prp.refine_with_congruence((B %= 2) / 14);
  prp.refine_with_congruence((C %= 2) / 21);
  prp.refine_with_constraint(A <= 5);
  prp.refine_with_constraint(B <= 10);
  prp.refine_with_constraint(C <= 0);
  prp.refine_with_constraint(C >= 0);

  Variables_Set to_fold;
  to_fold.insert(A);
  to_fold.insert(C);

  prp.fold_space_dimensions(to_fold, B);

  Product known_prp(1);
  known_prp.refine_with_congruence((A %= 2) / 7);
  known_prp.refine_with_constraint(A <= 10);

  bool ok = (prp == known_prp);

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// max_space_dimension().
bool
test08() {
  dimension_type msd1 = NNC_Polyhedron::max_space_dimension();
  nout << "NNCPolyhedron max space dimension = "
       << msd1 << endl;
  dimension_type msd2 = Grid::max_space_dimension();
  nout << "Grid max space dimension = "
       << msd2 << endl;
  dimension_type msd = Product::max_space_dimension();
  bool ok = (msd <= msd1 && msd <= msd2);

  nout << "Product max space dimension = "
       << msd << endl << endl;

  return ok;
}

// Attempt to construct a product with too many dimensions.
bool
test09() {
  try {
    // This is an invalid use of the constructor of a product:
    // it is illegal to (try to) build a product with a dimension
    // greater than max_space_dimension().
    Product pg(Product::max_space_dimension() + 2);

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
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
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN
