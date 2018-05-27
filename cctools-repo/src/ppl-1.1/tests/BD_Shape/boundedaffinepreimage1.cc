/* Test BD_Shape::bounded_affine_preimage().
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");

  bds.bounded_affine_preimage(A, Linear_Expression(7), Linear_Expression(3));

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 7, 3) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(B <= 4);

  bds.bounded_affine_preimage(A, Linear_Expression(3), Linear_Expression(7));

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 3, 7) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bds.bounded_affine_preimage(A, Linear_Expression(3), Linear_Expression(7),
                              -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 3, 7, -1) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bds.bounded_affine_preimage(A, Linear_Expression(7),
                              Linear_Expression(3), -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 7, 3, -1) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");


  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(B - A <= 3);

  bds.bounded_affine_preimage(A, Linear_Expression(3), A+2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 3, A+2) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= -1);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(B <= 4);

  bds.bounded_affine_preimage(A, Linear_Expression(3), -A+2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 3, -A+2) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 1);
  known_result.add_constraint(B <= 4);

  bds.bounded_affine_preimage(A, Linear_Expression(3), B+5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 3, B+5) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(2*B <= 8);

  bds.bounded_affine_preimage(A, Linear_Expression(3), B-5, -1);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 3, B-5, -1) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");;

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(4*A >= -5);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);
  known_result.add_constraint(4*(B - A) <= 21);

  bds.bounded_affine_preimage(A, Linear_Expression(2), 4*A + 3*B - 5);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 2, 4*A + 3*B - 5) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TBD_Shape bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);
  bds.add_constraint(B >= 0);
  bds.add_constraint(B <= 4);
  bds.add_constraint(A - B <= 2);
  bds.add_constraint(A - B >= -2);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(4*A <= 5);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 4);

  bds.bounded_affine_preimage(A, Linear_Expression(2), 4*A + 3*B - 5, -2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, 2, "
                    "4*A + 3*B - 5, -2) ***");

  return ok;
}

bool
test11() {
  Variable A(0);

  TBD_Shape bds(1);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A <= 4);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(1);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A <= 4);

  bds.bounded_affine_preimage(A, A, A);

  bool ok = check_result(bds, known_result);

  print_constraints(bds,
                    "*** bds.bounded_affine_preimage(A, A, A) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable C(2);

  BD_Shape<mpz_class> bds(3);
  bds.add_constraint(C == 5);

  print_constraints(bds, "*** bds ***");

  bds.bounded_affine_preimage(C, A, A, 5);

  BD_Shape<mpz_class> known_result(3);
  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds.bounded_affine_preimage(C, A, A, 5) ***");

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
  DO_TEST(test12);
END_MAIN
