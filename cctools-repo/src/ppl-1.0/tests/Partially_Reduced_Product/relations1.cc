/* Test relation_with().
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

// relation_with a generator
bool
test01() {
  Variable A(0);
  Variable B(1);

  Generator pnt(point(A + B));

  Product dp(2);

  bool ok = Poly_Gen_Relation::subsumes() == dp.relation_with(pnt);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// relation_with a constraint
bool
test02() {
  Variable A(0);
  Variable B(1);

  Constraint c(A == 2);

  Product dp(2);

  bool ok = Poly_Con_Relation::nothing() == dp.relation_with(c);

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// Empty product; relation_with a constraint.
bool
test03() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.refine_with_constraint(A == 1);
  dp.refine_with_congruence((A %= 2) / 0);

  bool ok = (dp.relation_with(B == 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates())
	     && dp.relation_with(B >= 0)
	     == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates()));

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// A product in 3D; relation_with a constraint.
bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.refine_with_constraint(3*A + 3*B + C == 7);
  dp.refine_with_constraint(3*A - C >= 2);
  dp.refine_with_congruence(6*A + 3*B %= 0);

  Poly_Con_Relation rel1 = dp.domain1().relation_with(2*A + B >= 3);
  Poly_Con_Relation rel2 = dp.domain2().relation_with(2*A + B >= 3);
  bool okdp1 = (rel1 == Poly_Con_Relation::strictly_intersects());
  bool okdp2 = (rel2 == Poly_Con_Relation::is_included());

  if (!okdp1 || !okdp2) {
    nout << "dp.domain1().relation_with(2*A + B >= 3) == " << rel1 << endl;
    nout << "dp.domain2().relation_with(2*A + B >= 3) == " << rel2 << endl;
    print_constraints(dp, "*** dp constraints ***");
    print_congruences(dp, "*** dp congruences ***");
    return false;
  }

  Poly_Con_Relation rel3 = dp.relation_with(A + B + C == 0);
  Poly_Con_Relation rel4 = dp.relation_with(A + B == 0);
  Poly_Con_Relation rel5 = dp.relation_with(A == 0);
  Poly_Con_Relation rel6 = dp.relation_with(Linear_Expression(0) == 0);
  Poly_Con_Relation rel7 = dp.relation_with(2*A + B >= 3);
  Poly_Con_Relation rel8 = dp.relation_with(3*A + 3*B + C >= 7);
  bool ok = (rel3 == Poly_Con_Relation::nothing()
	     && rel4 == Poly_Con_Relation::nothing()
	     && rel5 == Poly_Con_Relation::nothing()
	     && rel6 == (Poly_Con_Relation::is_included()
			 && Poly_Con_Relation::saturates())
	     && rel7 == Poly_Con_Relation::is_included()
	     && rel8 == (Poly_Con_Relation::is_included()
			 && Poly_Con_Relation::saturates()));

  nout << "dp.relation_with(A + B + C %= 0) == " << rel3 << endl;
  nout << "dp.relation_with(A + B %= 0) == " << rel4 << endl;
  nout << "dp.relation_with(A %= 0) == " << rel5 << endl;
  nout << "dp.relation_with(Linear_Expression(0) %= 0) == " << rel6 << endl;
  nout << "dp.relation_with(2*A + B %= 3) == " << rel7 << endl;
  nout << "dp.relation_with(3*A + 3*B + C %= 7) == " << rel8 << endl;
  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");
  return ok;
}

// A product where the components strictly intersect the constraint.
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.refine_with_constraint(C == 0);
  dp.refine_with_congruence(6*A + 3*B %= 0);

  Poly_Con_Relation rel1 = dp.domain1().relation_with(2*A + B >= 3);
  Poly_Con_Relation rel2 = dp.domain2().relation_with(2*A + B >= 3);
  bool okdp1 = (rel1 == Poly_Con_Relation::strictly_intersects());
  bool okdp2 = (rel2 == Poly_Con_Relation::strictly_intersects());

  if (!okdp1 || !okdp2) {
    nout << "dp.domain1().relation_with(2*A + B >= 3) == " << rel1 << endl;
    nout << "dp.domain2().relation_with(2*A + B >= 3) == " << rel2 << endl;
    print_constraints(dp, "*** dp constraints ***");
    print_congruences(dp, "*** dp congruences ***");
    return false;
  }

  Poly_Con_Relation rel3 = dp.relation_with(2*A + B >= 3);
  bool ok = (rel3 == Poly_Con_Relation::nothing());

  nout << "dp.relation_with(2*A + B >= 3) == " << rel3 << endl;
  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// relation_with a congruence
bool
test06() {
  Variable A(0);
  Variable B(1);

  Congruence cg(A %= 2);

  Product dp(2);

  Poly_Con_Relation rel = dp.relation_with(cg);
  bool ok = Poly_Con_Relation::nothing() == rel;

  nout << "dp.relation_with(A %= 2) == " << rel << endl;
  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// Empty product; relation_with a congruence.
bool
test07() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.refine_with_constraint(A == 1);
  dp.refine_with_congruence((A %= 2) / 0);

  Poly_Con_Relation rel = dp.relation_with(B %= 0);
  bool ok = (rel == (Poly_Con_Relation::is_included()
		 && Poly_Con_Relation::is_disjoint()
	         && Poly_Con_Relation::saturates()));

  nout << "dp.relation_with(B %= 0) == " << rel << endl;
  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// A product in 3D; relation_with a congruence.
bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.refine_with_constraint(3*A + 3*B + C == 7);
  dp.refine_with_constraint(3*A - C >= 2);
  dp.refine_with_congruence((2*A + B %= 0) / 2);

  Poly_Con_Relation rel1 = dp.domain1().relation_with(2*A + B %= 3);
  Poly_Con_Relation rel2 = dp.domain2().relation_with(2*A + B %= 3);
  bool okdp1 = (rel1 == Poly_Con_Relation::is_included());
  bool okdp2 = (rel2 == Poly_Con_Relation::strictly_intersects());

  if (!okdp2 || !okdp1) {
    nout << "dp.domain1().relation_with(2*A + B %= 3) == " << rel1 << endl;
    nout << "dp.domain2().relation_with(2*A + B %= 3) == " << rel2 << endl;
    print_constraints(dp, "*** dp constraints ***");
    print_congruences(dp, "*** dp congruences ***");
    return false;
  }

  Poly_Con_Relation rel3 = dp.relation_with(A + B + C %= 0);
  Poly_Con_Relation rel4 = dp.relation_with(A + B %= 0);
  Poly_Con_Relation rel5 = dp.relation_with(A %= 0);
  Poly_Con_Relation rel6 = dp.relation_with(Linear_Expression(0) %= 0);
  Poly_Con_Relation rel7 = dp.relation_with(2*A + B %= 3);
  Poly_Con_Relation rel8 = dp.relation_with(3*A + 3*B + C %= 7);
  bool ok = (rel3 == Poly_Con_Relation::nothing()
	     && rel4 == Poly_Con_Relation::nothing()
	     && rel5 == Poly_Con_Relation::nothing()
	     && rel6 == (Poly_Con_Relation::is_included()
			 && Poly_Con_Relation::saturates())
	     && rel7 == Poly_Con_Relation::is_included()
	     && rel8 == (Poly_Con_Relation::is_included()
			 && Poly_Con_Relation::saturates()));

  nout << "dp.relation_with(A + B + C %= 0) == " << rel3 << endl;
  nout << "dp.relation_with(A + B %= 0) == " << rel4 << endl;
  nout << "dp.relation_with(A %= 0) == " << rel5 << endl;
  nout << "dp.relation_with(Linear_Expression(0) %= 0) == " << rel6 << endl;
  nout << "dp.relation_with(2*A + B %= 3) == " << rel7 << endl;
  nout << "dp.relation_with(3*A + 3*B + C %= 7) == " << rel8 << endl;
  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");

  return ok;
}

// A product where the components strictly intersect the congruence.
bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.refine_with_constraint(C == 0);
  dp.refine_with_congruence(6*A + 3*B %= 0);

  bool okdp1 = (dp.domain1().relation_with(2*A + B %= 3)
		== Poly_Con_Relation::strictly_intersects());

  bool okdp2 = (dp.domain2().relation_with(2*A + B %= 3)
	       == Poly_Con_Relation::strictly_intersects());

  Poly_Con_Relation rel = dp.relation_with(2*A + B %= 3);
  bool ok = (okdp1 && okdp2
	     && rel == Poly_Con_Relation::nothing());

  print_constraints(dp, "*** dp constraints ***");
  print_congruences(dp, "*** dp congruences ***");
  nout << "dp.relation_with(2*A + B %= 3) == " << rel << endl;

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
  DO_TEST(test08);
  DO_TEST(test09);
END_MAIN
