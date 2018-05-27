/* Test Polyhedron::BHRZ03_widening_assign().
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

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(B));
  gs1.insert(ray(-A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(B));
  gs2.insert(ray(-A - B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A <= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-A));
  gs1.insert(ray(-A - B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(-A));
  gs2.insert(ray(A - B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B <= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-B));
  gs1.insert(ray(A - B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(-B));
  gs2.insert(ray(A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-A));
  gs1.insert(ray(-A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(-A));
  gs2.insert(ray(A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(-B));
  gs1.insert(ray(-A - B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(-B));
  gs2.insert(ray(-A + B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A <= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(A));
  gs1.insert(ray(A - B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(A));
  gs2.insert(ray(-A - B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(B <= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(ray(B));
  gs1.insert(ray(A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(point());
  gs2.insert(ray(B));
  gs2.insert(ray(A - B));
  C_Polyhedron ph2(gs2);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");

  ph2.BHRZ03_widening_assign(ph1);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 0);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.BHRZ03_widening_assign(ph1) ***");

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
