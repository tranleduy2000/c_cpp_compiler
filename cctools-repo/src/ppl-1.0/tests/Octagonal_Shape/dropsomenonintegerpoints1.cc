/* Test Octagonal_Shape::drop_some_non_integer_points().
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

  Octagonal_Shape<mpq_class> os(2);

  os.add_constraint(2*A >= 1);
  os.add_constraint(2*B >= -1);
  os.add_constraint(2*A - 2*B >= 1);
  os.add_constraint(4*A + 4*B >= 1);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points();

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(A - B >= 1);
  known_result.add_constraint(A + B >= 1);

  bool ok = (os == known_result);

  print_constraints(os, "*** after os.drop_some_non_integer_points() ***");

  return ok;
}

bool
test02() {
  Octagonal_Shape<mpq_class> os1;
  Octagonal_Shape<mpq_class> os2(0, EMPTY);
  Octagonal_Shape<mpq_class> os3(2, EMPTY);
  Octagonal_Shape<mpq_class> os4(2);

  print_constraints(os1, "*** os1 ***");
  print_constraints(os2, "*** os2 ***");

  Octagonal_Shape<mpq_class> known_result1(os1);
  Octagonal_Shape<mpq_class> known_result2(os2);
  Octagonal_Shape<mpq_class> known_result3(os3);
  Octagonal_Shape<mpq_class> known_result4(os4);

  os1.drop_some_non_integer_points();
  os2.drop_some_non_integer_points();
  os3.drop_some_non_integer_points();
  os4.drop_some_non_integer_points();

  bool ok = (os1 == known_result1 && os2 == known_result2
             && os3 == known_result3 && os4 == known_result4);

  print_constraints(os1, "*** after os1.drop_some_non_integer_points() ***");
  print_constraints(os2, "*** after os2.drop_some_non_integer_points() ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Octagonal_Shape<mpq_class> os(2);
  os.add_constraint(A - B == 0);
  os.add_constraint(A >= 0);

  Octagonal_Shape<mpq_class> known_result(os);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points();

  bool ok = (os == known_result);

  print_constraints(os, "*** after os.drop_some_non_integer_points() ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Octagonal_Shape<mpq_class> os(2);
  os.add_constraint(A >= 0);
  os.add_constraint(A == B);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points();

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A == B);

  bool ok = (os == known_result);

  print_constraints(os, "*** after os.drop_some_non_integer_points() ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Octagonal_Shape<mpq_class> os(2);
  os.add_constraint(2*A == 1);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points();

  Octagonal_Shape<mpq_class> known_result(2, EMPTY);

  bool ok = (os == known_result);

  print_constraints(os, "*** after os.drop_some_non_integer_points() ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Octagonal_Shape<mpq_class> os(2);
  os.add_constraint(2*A <= 1);
  os.add_constraint(2*B <= -1);
  os.add_constraint(4*A - 4*B <= 7);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points();

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(B <= -1);
  known_result.add_constraint(A - B <= 1);

  bool ok = (os == known_result);

  print_constraints(os, "*** after os.drop_some_non_integer_points() ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_A;
  varset_A.insert(A);

  Octagonal_Shape<mpq_class> os(2);

  os.add_constraint(2*A >= 1);
  os.add_constraint(2*B >= -1);
  os.add_constraint(2*A - 2*B >= 1);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points(varset_A);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(2*B >= -1);
  known_result.add_constraint(2*A - 2*B >= 1);

  bool ok = (os == known_result);

  print_constraints(os,
                    "*** after os.drop_some_non_integer_points(varset_A) ***");

  return ok;
}

bool
test08() {
  Variable A(0);
  Variables_Set varset_A;
  varset_A.insert(A);

  Octagonal_Shape<mpq_class> os1(2, EMPTY);
  Octagonal_Shape<mpq_class> os2(2);

  print_constraints(os1, "*** os1 ***");
  print_constraints(os2, "*** os2 ***");

  Octagonal_Shape<mpq_class> known_result1(os1);
  Octagonal_Shape<mpq_class> known_result2(os2);

  os1.drop_some_non_integer_points(varset_A);
  os2.drop_some_non_integer_points(varset_A);

  bool ok = (os1 == known_result1 && os2 == known_result2);

  print_constraints(os1,
                    "*** after os1.drop_some_non_integer_points(varset_A) ***");
  print_constraints(os2,
                    "*** after os2.drop_some_non_integer_points(varset_A) ***");

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_A;
  varset_A.insert(A);

  Octagonal_Shape<mpq_class> os(2);
  os.add_constraint(2*A - 2*B == 1);
  os.add_constraint(A >= 0);

  Octagonal_Shape<mpq_class> known_result(os);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points(varset_A);

  bool ok = (os == known_result);

  print_constraints(os,
                    "*** after os.drop_some_non_integer_points(varset_A) ***");

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_A;
  varset_A.insert(A);

  Octagonal_Shape<mpq_class> os(2);
  os.add_constraint(A >= 0);
  os.add_constraint(A == B);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points(varset_A);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A >= 0);
  known_result.add_constraint(A == B);

  bool ok = (os == known_result);

  print_constraints(os,
                    "*** after os.drop_some_non_integer_points(varset_A) ***");

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_B;
  varset_B.insert(B);

  Octagonal_Shape<mpq_class> os(2);
  os.add_constraint(2*A == 1);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points(varset_B);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(2*A == 1);

  bool ok = (os == known_result);

  print_constraints(os,
                    "*** after os.drop_some_non_integer_points(varset_B) ***");

  return ok;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variables_Set varset_A;
  varset_A.insert(A);

  Octagonal_Shape<mpq_class> os(2);
  os.add_constraint(2*A <= 1);
  os.add_constraint(2*B <= -1);
  os.add_constraint(4*A - 4*B <= 7);

  print_constraints(os, "*** os ***");

  os.drop_some_non_integer_points(varset_A);

  Octagonal_Shape<mpq_class> known_result(2);
  known_result.add_constraint(A <= 0);
  known_result.add_constraint(2*B <= -1);
  known_result.add_constraint(4*A - 4*B <= 7);

  bool ok = (os == known_result);

  print_constraints(os,
                    "*** after os.drop_some_non_integer_points(varset_A) ***");

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
