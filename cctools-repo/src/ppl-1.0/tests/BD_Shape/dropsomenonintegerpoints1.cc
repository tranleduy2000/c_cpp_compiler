/* Test BD_Shape::drop_some_non_integer_points().
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
  Variable A(0);
  Variable B(1);

  BD_Shape<mpq_class> bds(2);

  bds.add_constraint(2*A >= 1);
  bds.add_constraint(2*B >= -1);
  bds.add_constraint(2*A - 2*B >= 1);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points();

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A - B >= 1);

  bool ok = (bds == known_result);

  print_constraints(bds, "*** after bds.drop_some_non_integer_points() ***");

  return ok;
}

bool
test02() {
  BD_Shape<mpq_class> bds1;
  BD_Shape<mpq_class> bds2(0, EMPTY);
  BD_Shape<mpq_class> bds3(2, EMPTY);
  BD_Shape<mpq_class> bds4(2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  BD_Shape<mpq_class> known_result1(bds1);
  BD_Shape<mpq_class> known_result2(bds2);
  BD_Shape<mpq_class> known_result3(bds3);
  BD_Shape<mpq_class> known_result4(bds4);

  bds1.drop_some_non_integer_points();
  bds2.drop_some_non_integer_points();
  bds3.drop_some_non_integer_points();
  bds4.drop_some_non_integer_points();

  bool ok = (bds1 == known_result1 && bds2 == known_result2
             && bds3 == known_result3 && bds4 == known_result4);

  print_constraints(bds1, "*** after bds1.drop_some_non_integer_points() ***");
  print_constraints(bds2, "*** after bds2.drop_some_non_integer_points() ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(A - B == 0);
  bds.add_constraint(A >= 0);

  BD_Shape<mpq_class> known_result(bds);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points();

  bool ok = (bds == known_result);

  print_constraints(bds, "*** after bds.drop_some_non_integer_points() ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A == B);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points();

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A == B);

  bool ok = (bds == known_result);

  print_constraints(bds, "*** after bds.drop_some_non_integer_points() ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(2*A == 1);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points();

  BD_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (bds == known_result);

  print_constraints(bds, "*** after bds.drop_some_non_integer_points() ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(2*A <= 1);
  bds.add_constraint(2*B <= -1);
  bds.add_constraint(4*A - 4*B <= 7);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points();

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B <= -1);
  known_result.add_constraint(A - B <= 1);

  bool ok = (bds == known_result);

  print_constraints(bds, "*** after bds.drop_some_non_integer_points() ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_A;
  varset_A.insert(A);

  BD_Shape<mpq_class> bds(2);

  bds.add_constraint(2*A >= 1);
  bds.add_constraint(2*B >= -1);
  bds.add_constraint(2*A - 2*B >= 1);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points(varset_A);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(2*B >= -1);
  known_result.add_constraint(2*A - 2*B >= 1);

  bool ok = (bds == known_result);

  print_constraints(bds,
                    "*** after bds.drop_some_non_integer_points(varset_A) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variables_Set varset_A;
  varset_A.insert(A);

  BD_Shape<mpq_class> bds1(2, EMPTY);
  BD_Shape<mpq_class> bds2(2);

  print_constraints(bds1, "*** bds1 ***");
  print_constraints(bds2, "*** bds2 ***");

  BD_Shape<mpq_class> known_result1(bds1);
  BD_Shape<mpq_class> known_result2(bds2);

  bds1.drop_some_non_integer_points(varset_A);
  bds2.drop_some_non_integer_points(varset_A);

  bool ok = (bds1 == known_result1 && bds2 == known_result2);

  print_constraints(bds1,
                    "*** after bds1.drop_some_non_integer_points(varset_A) ***");
  print_constraints(bds2,
                    "*** after bds2.drop_some_non_integer_points(varset_A) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_A;
  varset_A.insert(A);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(2*A - 2*B == 1);
  bds.add_constraint(A >= 0);

  BD_Shape<mpq_class> known_result(bds);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points(varset_A);

  bool ok = (bds == known_result);

  print_constraints(bds,
                    "*** after bds.drop_some_non_integer_points(varset_A) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_A;
  varset_A.insert(A);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(A >= 0);
  bds.add_constraint(A == B);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points(varset_A);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A == B);

  bool ok = (bds == known_result);

  print_constraints(bds,
                    "*** after bds.drop_some_non_integer_points(varset_A) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_B;
  varset_B.insert(B);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(2*A == 1);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points(varset_B);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A == 1);

  bool ok = (bds == known_result);

  print_constraints(bds,
                    "*** after bds.drop_some_non_integer_points(varset_B) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_A;
  varset_A.insert(A);

  BD_Shape<mpq_class> bds(2);
  bds.add_constraint(2*A <= 1);
  bds.add_constraint(2*B <= -1);
  bds.add_constraint(4*A - 4*B <= 7);

  print_constraints(bds, "*** bds ***");

  bds.drop_some_non_integer_points(varset_A);

  BD_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(2*B <= -1);
  known_result.add_constraint(4*A - 4*B <= 7);

  bool ok = (bds == known_result);

  print_constraints(bds,
                    "*** after bds.drop_some_non_integer_points(varset_A) ***");

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
