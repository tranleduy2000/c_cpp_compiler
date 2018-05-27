/* Test BD_Shape::affine_image().
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
  Variable z(2);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 1);
  bds.add_constraint(y <= 2);
  bds.add_constraint(z >= 3);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, -x);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x >= -1);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(z >= 3);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, -x) ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(3);
  bds.add_constraint(x <= 1);
  bds.add_constraint(y <= 2);
  bds.add_constraint(z >= 3);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, -z);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(x <= -3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(z >= 3);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, -z) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= 1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, -y + 1);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 0);
  known_result.add_constraint(x >= -1);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, -y + 1) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= 1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, -2*y + 1, -2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*x - 2*y == -1);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, -2*y + 1, -2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= 1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, -2*y + 1, 2);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*x <= -1);
  known_result.add_constraint(2*x >= -3);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, -2*y + 1, 2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(x >= 0);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= -1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, 2*x + y + 1);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 5);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);
  known_result.add_constraint(x - y <= 3);
  known_result.add_constraint(x - y >= 1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, 2*x + y + 1) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(x >= 0);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= -1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, -2*x + y + 1);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 3);
  known_result.add_constraint(x >= -2);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);
  known_result.add_constraint(x - y <= 1);
  known_result.add_constraint(x - y >= -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.affine_image(x, -2*x + y + 1) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(x >= 0);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= -1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, 2*x - 3*y + 1, 5);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(5*x <= 6);
  known_result.add_constraint(x >= -1);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);

  bool ok = check_result(bds, known_result, "9.54e-8", "6.75e-8", "4.77e-8");

  print_constraints(bds, "*** bds.affine_image(x, 2*x - 3*y + 1, 5) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(x >= 0);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= -1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, -2*x - 3*y + 1, 5);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(5*x <= 4);
  known_result.add_constraint(5*x >= -7);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);

  bool ok = check_result(bds, known_result, "2.15e-7", "1.36e-7", "9.54e-8");

  print_constraints(bds, "*** bds.affine_image(x, -2*x - 3*y + 1, 5) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);

  TBD_Shape bds(2);
  bds.add_constraint(x <= 1);
  bds.add_constraint(x >= 0);
  bds.add_constraint(y <= 2);
  bds.add_constraint(y >= -1);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, 2*x - 3*y + 1, -5);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(x <= 1);
  known_result.add_constraint(5*x >= -6);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(y >= -1);
  known_result.add_constraint(5*x - 5*y <= 1);
  known_result.add_constraint(5*x - 5*y >= -7);

  bool ok = check_result(bds, known_result, "1.91e-7", "1.17e-7", "9.54e-8");

  print_constraints(bds, "*** bds.affine_image(x, 2*x - 3*y + 1, -5) ***");

  return ok;
}

bool
test11() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(3);
  bds.add_constraint(y >= 0);
  bds.add_constraint(y <= 2);
  bds.add_constraint(z <= 3);

  print_constraints(bds, "*** bds ***");

  bds.affine_image(x, y + 5*z, 3);

  BD_Shape<mpq_class> known_result(3);
  known_result.add_constraint(3*x <= 17);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 2);
  known_result.add_constraint(z <= 3);
  known_result.add_constraint(x - y <= 5);
  known_result.add_constraint(3*x - 3*z <= 8);

  bool ok = check_result(bds, known_result, "1.12e-6", "6.56e-7", "4.77e-7");

  print_constraints(bds, "*** bds.affine_image(x, y + 5*z, 3) ***");

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
  DO_TEST(test10);
  DO_TEST(test11);
END_MAIN
