/* Test Box::affine_dimension().
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

bool
test01() {
  Variable x(0);
  Variable y(1);

  TBox box(3);

  box.add_constraint(x <= 2);
  box.add_constraint(y <= 2);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension of `box' is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 3);

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBox box(5);

  box.add_constraint(A <= 5);
  box.add_constraint(C <= 2);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 5);

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  TBox box(5);

  box.add_constraint(A == 5);
  box.add_constraint(C <= 2);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 4);

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A <= 3);
  box.add_constraint(-B <= 2);
  box.add_constraint(B <= -2);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 1);

  return ok;
}

bool
test05() {
  TBox box(2, EMPTY);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 0);

  return ok;
}

bool
test06() {
  TBox box(2);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 2);

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  TBox box(4);
  box.add_constraint(A <= 1);
  box.add_constraint(A >= 1);
  box.add_constraint(B == 0);
  box.add_constraint(C == 7);
  box.add_constraint(D <= 9);
  box.add_constraint(-D <= -9);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 0);

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable D(3);
  Variable E(4);

  TBox box(5);
  box.add_constraint(A == 1);
  box.add_constraint(E == 1);
  box.add_constraint(B <= 7);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 3);

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A == 0);
  box.add_constraint(B == 2);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

  bool ok = (affine_dim == 0);

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TBox box(7, EMPTY);

  print_constraints(box, "*** box ***");

  dimension_type affine_dim = box.affine_dimension();

  nout << endl
       << "The affine dimension is "
       << affine_dim
       << endl;

   bool ok = (affine_dim == 0);

  return ok;
}

bool
test11() {
  TBox box(0, UNIVERSE);

  const dimension_type affine_dim = box.affine_dimension();

  print_constraints(box, "*** box ***");

  return affine_dim == 0;
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
  DO_TEST(test11);
END_MAIN


