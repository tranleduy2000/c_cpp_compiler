/* Test Octagonal_Shape::relation_with(g): we verify that a generator
   is not subsumed by an empty octagon.
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

  TOctagonal_Shape oct(2, EMPTY);

  print_constraints(oct, "*** oct ***");

  Generator g = point(x);

  print_generator(g, "*** g ***");

  Poly_Gen_Relation rel = oct.relation_with(g);

  using namespace IO_Operators;
  nout << "oct.relation_with(v(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test02() {
  TOctagonal_Shape oct;
  print_constraints(oct, "*** oct ***");

  Generator g = point();

  print_generator(g, "*** g ***");

  Poly_Gen_Relation rel = oct.relation_with(g);

  using namespace IO_Operators;
  nout << "oct.relation_with(v()) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::subsumes();

  return rel == known_result;
}

bool
test03() {
  Variable A(0);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A == 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(point(2*A));

  using namespace IO_Operators;
  nout << "oct.relation_with(point(2*A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A + B >= 0);
  oct.add_constraint(B >= 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel1 = oct.relation_with(point(B));

  using namespace IO_Operators;
  nout << "oct.relation_with(point(B)) == " << rel1 << endl;

  Poly_Gen_Relation rel2 = oct.relation_with(point(-B));

  nout << "oct.relation_with(point(-B)) == " << rel2 << endl;

  Poly_Gen_Relation known_result1 = Poly_Gen_Relation::subsumes();
  Poly_Gen_Relation known_result2 = Poly_Gen_Relation::nothing();

  return rel1 == known_result1 && rel2 == known_result2;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(B == 1);
  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(ray(-A));

  using namespace IO_Operators;
  nout << "oct.relation_with(ray(-A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 0);
  oct.add_constraint(B == 1);
  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(ray(-A));

  using namespace IO_Operators;
  nout << "oct.relation_with(ray(-A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::subsumes();

  return rel == known_result;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 0);
  oct.add_constraint(B >= -1);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(line(A));

  using namespace IO_Operators;
  nout << "oct.relation_with(line(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A == 0);
  oct.add_constraint(B >= -1);
  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(line(-A));

  using namespace IO_Operators;
  nout << "oct.relation_with(line(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct  (2);
  oct.add_constraint(A <= 0);
  oct.add_constraint(B == 2);

  print_constraints(oct , "*** oct       ***");

  Poly_Gen_Relation rel = oct.relation_with(closure_point(A));

  using namespace IO_Operators;
  nout << "oct.relation_with(line(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 2);
  oct.add_constraint(A + B <= 3);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(ray(A + B));

  using namespace IO_Operators;
  nout << "oct.relation_with(ray(A + B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 2);
  oct.add_constraint(A + B <= 3);
  oct.add_constraint(A - B >= 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(ray(-2*B));

  using namespace IO_Operators;
  nout << "oct.relation_with(ray(-2*B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::subsumes();

  return rel == known_result;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  TOctagonal_Shape oc(2);
  oc.add_constraint(A >= 1);

  try {
    // This is an incorrect use of method
    // Octagon::relation_with(g):
    // it is illegal to use a generator that is
    // dimensional incompatible with the OS.
    Poly_Gen_Relation rel = oc.relation_with(ray(C));
    (void) rel;
  }
  catch (std::invalid_argument& e) {
    nout << "std::invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
    return false;
  }
  return false;
}

bool
test13() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A <= 2);
  oct.add_constraint(A + B <= 3);
  oct.add_constraint(A - B >= 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(line(-2*B));

  using namespace IO_Operators;
  nout << "oct.relation_with(line(-2*B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test14() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A + B == 3);
  oct.add_constraint(A - B == 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(line(-2*B));

  using namespace IO_Operators;
  nout << "oct.relation_with(line(-2*B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test15() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A - B == 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(line(2*B));

  using namespace IO_Operators;
  nout << "oct.relation_with(line(2*B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test16() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A - B <= 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(point(A + 2*B));

  using namespace IO_Operators;
  nout << "oct.relation_with(point(A + 2*B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::subsumes();

  return rel == known_result;
}

bool
test17() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A <= 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(point(A + 2*B));

  using namespace IO_Operators;
  nout << "oct.relation_with(point(A + 2*B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test18() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B - A <= 0);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(line(B - 3*A + 5));

  using namespace IO_Operators;
  nout << "oct.relation_with(line(B - 3*A + 5)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test19() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(B <= 2);

  print_constraints(oct, "*** oct ***");

  Poly_Gen_Relation rel = oct.relation_with(line(2*B));

  using namespace IO_Operators;
  nout << "oct.relation_with(line(-2*B)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test20() {
  // A 1D empty shape that is not in minimal form and the point is the origin.
  Variable A(0);

  TOctagonal_Shape oct(1);
  oct.add_constraint(A <= 0);
  oct.add_constraint(A >= 1);

  Generator g = point();
  Poly_Gen_Relation rel = oct.relation_with(g);

  print_constraints(oct, "*** oct ***");
  print_generator(g, "*** g ***");
  using namespace IO_Operators;
  nout << "oct.relation_with(v()) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
}

bool
test21() {
  // A single point does not subsume another (different) point.
  Variable A(0);

  TOctagonal_Shape oct(1);
  oct.add_constraint(A == 1);

  Generator g = point();
  Poly_Gen_Relation rel = oct.relation_with(g);

  print_constraints(oct, "*** oct ***");
  print_generator(g, "*** g ***");
  using namespace IO_Operators;
  nout << "oct.relation_with(v()) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();

  return rel == known_result;
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
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
  DO_TEST(test20);
  DO_TEST(test21);
END_MAIN
