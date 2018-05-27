/* Test Product<>::is_topologically_closed().
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
typedef Domain_Product<DOMAIN1x, DOMAIN2x>::Constraints_Product Product;

namespace {

// is_topologically_closed() where the NNC Polyhedron is topologically
// open.
bool
test01() {
  Variable A(0);

  Product prp(3);
  prp.refine_with_constraint(A < 3);
  prp.refine_with_congruence((A %= 0) / 3);

  bool ok = !prp.is_topologically_closed();

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// is_topologically_closed() where the Polyhedron is topologically
// closed.
bool
test02() {
  Variable A(0);

  Product prp(3);
  prp.refine_with_constraint(A <= 3);
  prp.refine_with_congruence((A %= 0) / 3);

  bool ok = prp.is_topologically_closed();

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

// is_topologically_closed() where the Polyhedron is topologically
// open and the intersection is closed.
bool
test03() {
  Variable A(0);

  Product prp(3);
  prp.refine_with_congruence((A %= 0) / 4);
  prp.refine_with_constraint(A < 3);

  bool ok = !prp.is_topologically_closed();

  print_congruences(prp, "*** prp congruences ***");
  print_constraints(prp, "*** prp constraints ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
