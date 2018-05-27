/* Test Pointset_Powerset<PH>::Pointset_Powerset(Constraint_System).
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

namespace {

// Construct powerset of polyhedra from a 0 dimension empty constraint system.
bool
test01() {
  Constraint_System cs = Constraint_System::zero_dim_empty();
  Pointset_Powerset<C_Polyhedron> ps(cs);
  return ps.OK() && ps.is_empty() && ps.space_dimension() == 0;
}

// Construct powerset of polyhedra from a non-empty constraint system.
bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Constraint_System cs;

  cs.insert(x + y > 0);
  cs.insert(x - 2*y <= 2);
  cs.insert(z == 2);
  Pointset_Powerset<NNC_Polyhedron> ps(cs);

  return ps.OK() && ps.space_dimension() == 3;
}

// Construct powerset of bd shapes from a 0 dimension empty constraint system.
bool
test03() {
  Constraint_System cs = Constraint_System::zero_dim_empty();
  Pointset_Powerset<TBD_Shape> ps(cs);
  return ps.OK() && ps.is_empty() && ps.space_dimension() == 0;
}

// Construct powerset of bd shapes from non-empty constraint system.
bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Constraint_System cs;

  cs.insert(x - y <= 2);
  cs.insert(z == 2);
  Pointset_Powerset<TBD_Shape> ps(cs);

  return ps.OK() && ps.space_dimension() == 3;
}

// Construct powerset of octagonal shapes from a 0 dimension empty constraint system.
bool
test05() {
  Constraint_System cs = Constraint_System::zero_dim_empty();
  Pointset_Powerset<TOctagonal_Shape> ps(cs);
  return ps.OK() && ps.is_empty() && ps.space_dimension() == 0;
}

// Construct powerset of octagonal shapes from non-empty constraint system.
bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Constraint_System cs;

  cs.insert(x + y >= -2);
  cs.insert(x - y <= 2);
  cs.insert(z == 2);
  Pointset_Powerset<TOctagonal_Shape> ps(cs);

  return ps.OK() && ps.space_dimension() == 3;
}

// Construct powerset of boxes from a 0 dimension empty constraint system.
bool
test07() {
  Constraint_System cs = Constraint_System::zero_dim_empty();
  Pointset_Powerset<TBox> ps(cs);

  print_constraints(ps, "*** ps ***");

  return ps.OK() && ps.is_empty() && ps.space_dimension() == 0;
}

// Construct powerset of boxes from a non-empty constraint system.
bool
test08() {
  Variable x(0);
  Constraint_System cs;

  cs.insert(x >= 0);
  cs.insert(x <= 2);
  Pointset_Powerset<TBox> ps(cs);

  return ps.OK() && ps.space_dimension() == 1;
}

// Construct powerset of grids from a 0 dimension empty constraint system.
bool
test09() {
  Constraint_System cs = Constraint_System::zero_dim_empty();
  Pointset_Powerset<Grid> ps(cs);

  print_constraints(ps, "*** ps ***");

  return ps.OK() && ps.is_empty() && ps.space_dimension() == 0;
}

// Construct powerset of grids from non-empty constraint system.
bool
test10() {
  Variable x(0);
  Constraint_System cs;

  cs.insert(x == 0);
  Pointset_Powerset<Grid> ps(cs);

  return ps.OK() && ps.space_dimension() == 1;
}

// Construct powerset of products from a 0-dim inconsistent constraint system.
  typedef Domain_Product<NNC_Polyhedron, Grid>::Constraints_Product CProduct;
bool
test11() {
  Constraint_System cs = Constraint_System::zero_dim_empty();
  Pointset_Powerset<CProduct> pscp(cs);
  return pscp.OK();
}

// Construct powerset of products from a 1-dim constraint system.
bool
test12() {
  Variable A(0);

  CProduct cp(1);
  cp.add_constraint(A == 0);
  Pointset_Powerset<CProduct> pscp(1);
  pscp.add_disjunct(cp);
  return pscp.OK();
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
  DO_TEST(test10);
END_MAIN
