/* Test Box::frequency().
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

namespace {

// Universe and empty bd shape.
bool
test01() {
  Variable A(0);

  TBox box1(1);

  TBox box2(1, EMPTY);

  Coefficient num1;
  Coefficient den1;
  Coefficient valn1;
  Coefficient vald1;
  Coefficient num2;
  Coefficient den2;
  Coefficient valn2;
  Coefficient vald2;
  bool ok = (!box1.frequency(A, num1, den1, valn1, vald1)
             && !box2.frequency(A, num2, den2, valn2, vald2));
  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  return ok;
}

// 0-dimension polyhedra.
bool
test02() {
  TBox box1(0);

  TBox box2(0, EMPTY);

  Coefficient num1;
  Coefficient den1;
  Coefficient valn1;
  Coefficient vald1;
  Coefficient num2;
  Coefficient den2;
  Coefficient valn2;
  Coefficient vald2;
  bool ok = (box1.frequency(Linear_Expression(3), num1, den1, valn1, vald1)
             && num1 == 0 && den1 == 1 && valn1 == 3 && vald1 == 1
             && !box2.frequency(Linear_Expression(3), num2, den2, valn2, vald2));
  print_constraints(box1, "*** box1 ***");
  print_constraints(box2, "*** box2 ***");

  return ok;
}

// Non-relational test.
bool
test03() {
  Variable A(0);

  TBox box(1);
  box.add_constraint(A == 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (box.frequency(Linear_Expression(A), num, den, valn, vald)
             && num == 0 && den == 1 && valn == 0 && vald == 1);
  print_constraints(box, "*** box ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A >= 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (!box.frequency(Linear_Expression(A), num, den, valn, vald));
  print_constraints(box, "*** box ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A <= 0);
  box.add_constraint(B >= 5);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (!box.frequency(Linear_Expression(B), num, den, valn, vald));
  print_constraints(box, "*** box ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(2*A == 1);
  box.add_constraint(B == 2);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (box.frequency(Linear_Expression(A + B - 3), num, den, valn, vald)
             && num == 0 && den == 1 && valn == -1 && vald == 2);
  print_constraints(box, "*** box ***");
  nout << "valn " << valn << ", vald " << vald << endl;

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TBox box(2);
  box.add_constraint(A <= 1);
  box.add_constraint(A >= 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (!box.frequency(Linear_Expression(A - B), num, den, valn, vald));
  print_constraints(box, "*** box ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TBox box(3);
  box.add_constraint(A == 1);
  box.add_constraint(2*B == -1);
  box.add_constraint(2*C == 1);
  box.add_constraint(B <= 4);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (box.frequency(Linear_Expression(A - B + C + 1),
                           num, den, valn, vald)
             && num == 0 && den == 1 && valn == 3 && vald == 1);
  print_constraints(box, "*** box ***");
  nout << "valn " << valn << ", vald " << vald << endl;

  return ok;
}

// Non-relational test of an empty box in 1-dimension.
bool
test09() {
  Variable A(0);

  TBox box(1);
  box.add_constraint(A <= 0);
  box.add_constraint(A >= 1);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (!box.frequency(Linear_Expression(A), num, den, valn, vald));
  print_constraints(box, "*** box ***");

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
