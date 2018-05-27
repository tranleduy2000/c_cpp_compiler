/* Test Octagonal_Shape::integer_upper_bound_assign_if_exact().
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
  Octagonal_Shape<mpz_class> oct_empty(0, EMPTY);
  Octagonal_Shape<mpz_class> oct_universe(0, UNIVERSE);

  // Testing all combinations for 0-dim polyhedra.
  bool ok = true;
  Octagonal_Shape<mpz_class> oct;

  // empty, empty
  oct = oct_empty;
  ok &= oct.integer_upper_bound_assign_if_exact(oct_empty);
  ok &= (oct == oct_empty);
  print_constraints(oct, "*** empty union empty ***");

  // empty, universe
  oct = oct_empty;
  ok &= oct.integer_upper_bound_assign_if_exact(oct_universe);
  ok &= (oct == oct_universe);
  print_constraints(oct, "*** empty union universe ***");

  // universe, empty
  oct = oct_universe;
  ok &= oct.integer_upper_bound_assign_if_exact(oct_empty);
  ok &= (oct == oct_universe);
  print_constraints(oct, "*** universe union empty ***");

  // universe, universe
  oct = oct_universe;
  ok &= oct.integer_upper_bound_assign_if_exact(oct_universe);
  ok &= (oct == oct_universe);
  print_constraints(oct, "*** universe union universe ***");

  return ok;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x >= -2);
  oct1.add_constraint(x <= -1);
  oct1.add_constraint(y >= 0);
  oct1.add_constraint(y <= 2);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(2, UNIVERSE);
  oct2.add_constraint(x >= 1);
  oct2.add_constraint(x <= 2);
  oct2.add_constraint(y >= 0);
  oct2.add_constraint(y <= 2);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(oct1);

  bool ok = !oct1.integer_upper_bound_assign_if_exact(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x >= -2);
  oct1.add_constraint(x <= 0);
  oct1.add_constraint(y >= 0);
  oct1.add_constraint(y <= 2);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(2, UNIVERSE);
  oct2.add_constraint(x >= 0);
  oct2.add_constraint(x <= 2);
  oct2.add_constraint(y >= 0);
  oct2.add_constraint(y <= 2);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(2, UNIVERSE);
  known_result.add_constraint(x >= -2);
  known_result.add_constraint(x <= 2);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 2);

  bool ok = oct1.integer_upper_bound_assign_if_exact(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x == 0);
  oct1.add_constraint(y == 0);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(2, UNIVERSE);
  oct2.add_constraint(x >= 0);
  oct2.add_constraint(x <= 2);
  oct2.add_constraint(y >= -2);
  oct2.add_constraint(y <= 2);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(oct2);

  bool ok = oct1.integer_upper_bound_assign_if_exact(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test05() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x >= 0);
  oct1.add_constraint(y == 0);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(2, UNIVERSE);
  oct2.add_constraint(x >= 0);
  oct2.add_constraint(y >= 2);
  oct2.add_constraint(y <= 4);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(oct1);

  bool ok = !oct1.integer_upper_bound_assign_if_exact(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x == y);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(2, UNIVERSE);
  oct2.add_constraint(x == 0);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(oct1);

  bool ok = !oct1.integer_upper_bound_assign_if_exact(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x >= y);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(2, UNIVERSE);
  oct2.add_constraint(x >= 0);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(oct1);

  bool ok = !oct1.integer_upper_bound_assign_if_exact(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x >= y);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(2, UNIVERSE);
  oct2.add_constraint(x <= y);

  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(2, UNIVERSE);

  bool ok = oct1.integer_upper_bound_assign_if_exact(oct2);
  ok &= (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test09() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Octagonal_Shape<mpz_class> oct(3, UNIVERSE);
  oct.add_constraint(x >= 0);
  oct.add_constraint(x <= 2);
  oct.add_constraint(y >= 0);
  oct.add_constraint(y <= 4);
  oct.add_constraint(z >= 0);
  oct.add_constraint(z <= 4);
  oct.add_constraint(x - y <= 2);
  oct.add_constraint(z - y <= 2);

  Octagonal_Shape<mpz_class> oct1(oct);
  oct1.add_constraint(z <= 3);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(oct);
  oct2.add_constraint(x - y <= 1);

  Octagonal_Shape<mpz_class> known_result(oct);

  bool ok = oct1.integer_upper_bound_assign_if_exact(oct2);

  ok &= (oct1 == oct);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test10() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  Octagonal_Shape<mpz_class> oct(4, UNIVERSE);
  oct.add_constraint(x - y <= 4);
  oct.add_constraint(z - w <= 4);
  oct.add_constraint(x - w <= 5);
  oct.add_constraint(z <= 0);
  oct.add_constraint(z - y <= 1);
  oct.add_constraint(y + w >= -1);

  Octagonal_Shape<mpz_class> oct1(oct);
  oct1.add_constraint(x - y <= 2);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpz_class> oct2(oct);
  oct2.add_constraint(z - w <= 2);

  Octagonal_Shape<mpz_class> known_result(oct);

  bool ok = oct1.integer_upper_bound_assign_if_exact(oct2);
  ok &= (oct1 == oct);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

bool
test11() {
  //  const dimension_type dim = 200;
  const dimension_type dim = 50;
  Constraint_System cs;
  for (dimension_type i = 1; i < dim; ++i) {
    Variable x(i);
    cs.insert(x >= 0);
    cs.insert(x <= 4);
  }

  Variable x(0);

  Octagonal_Shape<mpz_class> hypercube1(cs);
  hypercube1.add_constraint(x >= 0);
  hypercube1.add_constraint(x <= 4);

  Octagonal_Shape<mpz_class> hypercube2(cs);
  hypercube2.add_constraint(x >= 2);
  hypercube2.add_constraint(x <= 6);

  Octagonal_Shape<mpz_class> known_result(cs);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 6);

  bool ok = hypercube1.integer_upper_bound_assign_if_exact(hypercube2);

  ok &= (hypercube1 == known_result);

  print_constraints(hypercube1, "*** hyp1 ***");
  print_constraints(hypercube2, "*** hyp2 ***");

  return ok;
}

bool
test12() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x >= 0);
  oct1.add_constraint(x <= 3);
  oct1.add_constraint(y >= 0);
  oct1.add_constraint(y <= 2);
  oct1.add_constraint(x - y <= 2);

  Octagonal_Shape<mpz_class> oct2(2, UNIVERSE);
  oct2.add_constraint(x >= 3);
  oct2.add_constraint(x <= 6);
  oct2.add_constraint(y >= 0);
  oct2.add_constraint(y <= 2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(2, UNIVERSE);
  known_result.add_constraint(x >= 0);
  known_result.add_constraint(x <= 6);
  known_result.add_constraint(y >= 0);
  known_result.add_constraint(y <= 2);

  bool ok = oct1.integer_upper_bound_assign_if_exact(oct2)
    && (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

  return ok;
}

/*
  The following test shows that method
  Octagonal_Shape<T>::integer_upper_bound_assign_if_exact
  is not an upper bound operator (on the domain of rational shapes).
*/
bool
test13() {
  Variable x(0);
  Variable y(1);

  Octagonal_Shape<mpz_class> oct1(2, UNIVERSE);
  oct1.add_constraint(x + y <= 2);
  oct1.add_constraint(x - y <= 2);
  oct1.add_constraint(x + y >= 1);
  oct1.add_constraint(x - y >= 1);

  Octagonal_Shape<mpz_class> oct2(oct1);
  oct2.affine_image(x, x + 2);

  print_constraints(oct1, "*** oct1 ***");
  print_constraints(oct2, "*** oct2 ***");

  Octagonal_Shape<mpz_class> known_result(2, UNIVERSE);
  known_result.add_constraint(x >= 1);
  known_result.add_constraint(x <= 4);
  known_result.add_constraint(y == 0);

  bool ok = oct1.integer_upper_bound_assign_if_exact(oct2)
    && (oct1 == known_result);

  print_constraints(oct1,
                    "*** oct1.integer_upper_bound_assign_if_exact(oct2) ***");

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
  DO_TEST(test13);
END_MAIN
