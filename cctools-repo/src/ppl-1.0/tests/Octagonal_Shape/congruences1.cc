/* Test Octagonal_Shape::congruences(): we compute the system of
   congruences of an octagon  that is defined by a system of
   congruences that contains only a trivially false congruence.
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

  TOctagonal_Shape oct1(2);
  oct1.add_congruence((0*A %= 1) / 0);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Constraint_System cs = oct1.minimized_constraints();

  print_constraints(cs, "*** cs ***");

  Congruence_System cgs = oct1.congruences();
  TOctagonal_Shape oct2(cgs);

  print_constraints(oct2, "*** oct2 ***");

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test02() {
  TOctagonal_Shape oct1(3, EMPTY);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Congruence_System cgs = oct1.congruences();
  TOctagonal_Shape oct2(cgs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct1(3);
  oct1.add_congruence((x %= 1) / 0);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Congruence_System cgs = oct1.congruences();
  TOctagonal_Shape oct2(3);
  oct2.add_congruences(cgs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

bool
test04() {
  TOctagonal_Shape oct1(0, EMPTY);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Congruence_System cgs = oct1.congruences();
  print_constraints(oct1, "*** oct1 ***");
  TOctagonal_Shape oct2(cgs.space_dimension(), EMPTY);


  print_constraints(oct2, "*** oct2 ***");

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test05() {
  TOctagonal_Shape oct1(0);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Congruence_System cgs = oct1.congruences();
  TOctagonal_Shape oct2(cgs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TOctagonal_Shape oct1(3);
  oct1.refine_with_congruence((x %= 1) / 0);
  oct1.refine_with_congruence((y %= 3) / 2);
  oct1.refine_with_congruence((y + z %= 0) / 3);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Congruence_System cgs = oct1.congruences();
  TOctagonal_Shape oct2(3);
  oct2.refine_with_congruences(cgs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(oct2, "*** oct2 ***");

  return ok;
}

bool
test07() {
  Variable x(0);
  Variable y(1);

  TOctagonal_Shape oct1(2);
  oct1.refine_with_congruence((x + y %= 3) / 0);

  print_constraints(oct1, "*** oct1 ***");

  Octagonal_Shape<mpq_class> known_result(oct1);

  Congruence_System cgs = oct1.congruences();

  TOctagonal_Shape oct2(2);
  oct2.refine_with_congruences(cgs);

  bool ok = (Octagonal_Shape<mpq_class>(oct2) == known_result);

  print_constraints(oct2, "*** oct2 ***");

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
END_MAIN

