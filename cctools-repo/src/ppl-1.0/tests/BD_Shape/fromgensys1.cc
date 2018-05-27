/* Test BD_Shape::BD_Shape(const Generator_System&).
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
  Generator_System gs;
  TBD_Shape bds(gs);

  BD_Shape<mpq_class> known_result(0, EMPTY);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds ***");

  return ok;
}

bool
test02() {
  Variable V(10);

  Generator_System gs;
  gs.insert(closure_point(V));

  try {
    // It is illegal to build a BD_Shape starting from a non-empty
    // generator system having no points.
    TBD_Shape bds(gs);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test03() {
  Variable V(10);

  Generator_System gs;
  gs.insert(ray(V));

  try {
    // It is illegal to build a BD_Shape starting from a non-empty
    // generator system having no points.
    TBD_Shape bds(gs);
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Generator_System gs;
  gs.insert(ray(A + B));
  gs.insert(point(1*A + 2*B + 3*C + 4*D));
  gs.insert(point(2*A + 3*B + 4*C + 5*D));
  TBD_Shape bds(gs);

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(A >= 1);
  known_result.add_constraint(B >= 2);
  known_result.add_constraint(C >= 3);
  known_result.add_constraint(C <= 4);
  known_result.add_constraint(D >= 4);
  known_result.add_constraint(D <= 5);
  known_result.add_constraint(A == B-1);
  known_result.add_constraint(C == D-1);
  known_result.add_constraint(C <= A+2);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds ***");

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  C_Polyhedron ph(4);
  ph.add_constraint(A >= B);
  ph.add_constraint(B >= 2*C);
  ph.add_constraint(C >= 3*D);
  ph.add_constraint(D >= 4);
  ph.add_constraint(A-D <= 50);

  TBD_Shape bds(ph.generators());

  BD_Shape<mpq_class> known_result(4);
  known_result.add_constraint(C <= 30);
  known_result.add_constraint(D >= 4);
  known_result.add_constraint(D <= 10);
  known_result.add_constraint(B - A <= 0);
  known_result.add_constraint(A - D <= 50);
  known_result.add_constraint(B - C >= 12);
  known_result.add_constraint(C - D <= 23);
  known_result.add_constraint(C - D >= 8);

  bool ok = check_result(bds, known_result);

  print_constraints(bds, "*** bds ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
