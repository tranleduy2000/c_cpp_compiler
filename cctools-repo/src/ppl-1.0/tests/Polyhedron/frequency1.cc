/* Test Polyhedron::frequency().
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

// Universe and empty polyhedron.
bool
test01() {
  Variable A(0);

  C_Polyhedron ph1(1);

  C_Polyhedron ph2(1, EMPTY);

  Coefficient num1;
  Coefficient den1;
  Coefficient valn1;
  Coefficient vald1;
  Coefficient num2;
  Coefficient den2;
  Coefficient valn2;
  Coefficient vald2;
  bool ok = (!ph1.frequency(A, num1, den1, valn1, vald1)
             && !ph2.frequency(A, num2, den2, valn2, vald2));
  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

// 0-dimension polyhedra.
bool
test02() {
  C_Polyhedron ph1(0);

  C_Polyhedron ph2(0, EMPTY);

  Coefficient num1;
  Coefficient den1;
  Coefficient valn1;
  Coefficient vald1;
  Coefficient num2;
  Coefficient den2;
  Coefficient valn2;
  Coefficient vald2;
  bool ok = (ph1.frequency(Linear_Expression(3), num1, den1, valn1, vald1)
             && num1 == 0 && den1 == 1 && valn1 == 3 && vald1 == 1
             && !ph2.frequency(Linear_Expression(3), num2, den2, valn2, vald2));
  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  return ok;
}

// Non-relational test.
bool
test03() {
  Variable A(0);

  C_Polyhedron ph(1);
  ph.add_constraint(A == 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (ph.frequency(Linear_Expression(A), num, den, valn, vald)
             && num == 0 && den == 1 && valn == 0 && vald == 1);
  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A >= 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (!ph.frequency(Linear_Expression(A), num, den, valn, vald));
  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(A <= 0);
  ph.add_constraint(A + B >= 5);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (!ph.frequency(Linear_Expression(B), num, den, valn, vald));
  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.add_constraint(2*A == 1);
  ph.add_constraint(B == 2);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (ph.frequency(Linear_Expression(A + B - 3), num, den, valn, vald)
             && num == 0 && den == 1 && valn == -1 && vald == 2);
  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph(2);
  ph.add_constraint(A < 1);
  ph.add_constraint(A >= 0);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (!ph.frequency(Linear_Expression(A - 3), num, den, valn, vald));
  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(2*A - 2*B == 1);
  ph.add_constraint(3*C == 2);
  ph.add_constraint(B <= 2);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (ph.frequency(Linear_Expression(A - B + C + 1),
                           num, den, valn, vald)
             && num == 0 && den == 1 && valn == 13 && vald == 6);
  print_constraints(ph, "*** ph ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(2*A - 2*B == 1);
  ph.add_constraint(2*C == 1);
  ph.add_constraint(B <= 2);

  Coefficient num;
  Coefficient den;
  Coefficient valn;
  Coefficient vald;
  bool ok = (ph.frequency(Linear_Expression(A - B + C + 1),
                           num, den, valn, vald)
             && num == 0 && den == 1 && valn == 2 && vald == 1);
  print_constraints(ph, "*** ph ***");

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
