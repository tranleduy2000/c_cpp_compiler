/* Test BD_Shape::congruences().
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

  TBD_Shape bds1(2);
  bds1.add_congruence((0*A %= 1) / 0);

  print_constraints(bds1, "*** bds1 ***");

  BD_Shape<mpq_class> known_result(bds1);

  Constraint_System cs = bds1.minimized_constraints();

  print_constraints(cs, "*** cs ***");

  Congruence_System cgs = bds1.congruences();
  TBD_Shape bds2(cgs);

  print_constraints(bds2, "*** bds2 ***");

  bool ok = check_result(bds2, known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test02() {
  TBD_Shape bds1(3, EMPTY);

  print_constraints(bds1, "*** bds1 ***");

  BD_Shape<mpq_class> known_result(bds1);

  Congruence_System cgs = bds1.congruences();
  TBD_Shape bds2(cgs);

  bool ok = check_result(bds2, known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds1(3);
  bds1.add_congruence((x %= 1) / 0);

  print_constraints(bds1, "*** bds1 ***");

  BD_Shape<mpq_class> known_result(bds1);

  Congruence_System cgs = bds1.congruences();
  TBD_Shape bds2(3);
  bds2.add_congruences(cgs);

  bool ok = check_result(bds2, known_result);

  print_constraints(bds2, "*** bds2 ***");

  return ok;
}

bool
test04() {
  TBD_Shape bds1(0, EMPTY);

  print_constraints(bds1, "*** bds1 ***");

  BD_Shape<mpq_class> known_result(bds1);

  Congruence_System cgs = bds1.congruences();
  print_constraints(bds1, "*** bds1 ***");
  TBD_Shape bds2(cgs.space_dimension(), EMPTY);


  print_constraints(bds2, "*** bds2 ***");

  bool ok = check_result(bds2, known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test05() {
  TBD_Shape bds1(0);

  print_constraints(bds1, "*** bds1 ***");

  BD_Shape<mpq_class> known_result(bds1);

  Congruence_System cgs = bds1.congruences();
  TBD_Shape bds2(cgs);

  bool ok = check_result(bds2, known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test06() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds(3);
  bds.add_congruence((x %= 1) / 0);
  bds.add_congruence((x %= 2) / 0);

  print_constraints(bds, "*** bds ***");

  BD_Shape<mpq_class> known_result(bds.space_dimension(), EMPTY);

  Congruence_System cgs = bds.congruences();
  TBD_Shape bds1(3);
  bds1.add_congruences(bds.congruences());

  bool ok = check_result(bds, bds1) && check_result(bds1, known_result);

  print_constraints(bds1, "*** bds1 ***");

  return ok;
}

bool
test07() {
  TBD_Shape bds1(0, EMPTY);

  print_constraints(bds1, "*** bds1 ***");

  BD_Shape<mpq_class> known_result(bds1);

  Congruence_System cgs = bds1.congruences();
  TBD_Shape bds2(cgs.space_dimension(), EMPTY);
  bds2.add_congruences(cgs);
  bool b = !bds2.is_empty();

  bool ok = !b && check_result(bds2, known_result);

  print_congruences(cgs, "*** cgs ***");

  return ok;
}

bool
test08() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  TBD_Shape bds1(3);
  bds1.refine_with_congruence((x %= 1) / 0);
  bds1.refine_with_congruence((y %= 3) / 2);
  bds1.refine_with_congruence((y - z %= 0) / 3);

  print_constraints(bds1, "*** bds1 ***");

  BD_Shape<mpq_class> known_result(bds1);

  Congruence_System cgs = bds1.congruences();
  TBD_Shape bds2(3);
  bds2.add_congruences(cgs);

  bool ok = check_result(bds2, known_result);

  print_constraints(bds2, "*** bds2 ***");

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
END_MAIN

