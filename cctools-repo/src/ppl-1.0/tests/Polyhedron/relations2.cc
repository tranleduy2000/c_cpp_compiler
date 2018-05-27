/* Test Polyhedron::relation_with(c) and Polyhedron::relation_with(g).
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

using namespace IO_Operators;

namespace {

bool
test01() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(line(A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(ray(A));
  gs2.insert(point(B));
  gs2.insert(point(-B));
  C_Polyhedron ph2(gs2);

  Poly_Con_Relation rel1 = ph1.relation_with(A == 0);
  Poly_Con_Relation rel2 = ph2.relation_with(A == 0);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");
  nout << "ph1.relation_with(A == 0) == " << rel1 << endl;
  nout << "ph2.relation_with(A == 0) == " << rel2 << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();
  return rel1 == known_result && rel2 == known_result;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(A));
  gs.insert(line(B));
  C_Polyhedron ph(gs);

  Poly_Con_Relation rel = ph.relation_with(B > 0);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(B > 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();
  return rel == known_result;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 2);
  ph.add_constraint(B == 0);

  Poly_Gen_Relation rel = ph.relation_with(ray(A + B));

  Poly_Gen_Relation known_rel = Poly_Gen_Relation::nothing();

  bool ok = (rel == known_rel);

  print_constraints(ph, "*** ph ***");
  nout << "ph.relation_with(ray(A + B)) == " << rel << endl;

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point());
  ph.constraints();
  ph.add_generator(ray(A));
  ph.add_generator(ray(B));

  Poly_Con_Relation rel = ph.relation_with(A == 0);

  Poly_Con_Relation known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  print_constraints(ph, "*** ph ***");
  nout << "ph.relation_with(A == 0) == " << rel << endl;

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Generator_System gs1;
  gs1.insert(point());
  gs1.insert(line(A + B));
  C_Polyhedron ph1(gs1);

  Generator_System gs2;
  gs2.insert(ray(A));
  gs2.insert(point(B));
  gs2.insert(point(-B));
  C_Polyhedron ph2(gs2);

  Poly_Con_Relation rel1 = ph1.relation_with((A %= 0) / 5);
  Poly_Con_Relation rel2 = ph2.relation_with((A %= 0) / 5);

  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");
  nout << "ph1.relation_with((A %= 0) / 5) == " << rel1 << endl;
  nout << "ph2.relation_with((A %= 0) / 5) == " << rel2 << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::strictly_intersects();
  return rel1 == known_result && rel2 == known_result;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(A));
  C_Polyhedron ph(gs);

  Poly_Con_Relation rel = ph.relation_with(A %= 0);

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(A %= 0) == " << rel << endl;

  Poly_Con_Relation known_result = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();
  return rel == known_result;
}

bool
test07() {
  Variable A(0);

  C_Polyhedron ph(2);
  ph.generators();
  ph.add_constraint(A >= 2);
  ph.add_constraint(A <= 4);

  Poly_Con_Relation rel = ph.relation_with((A %= 1) / 4);

  Poly_Con_Relation known_rel = Poly_Con_Relation::is_disjoint();

  bool ok = (rel == known_rel);

  print_constraints(ph, "*** ph ***");
  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with((A %= 1) / 4) == " << rel << endl;

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  C_Polyhedron ph(2, EMPTY);
  ph.add_generator(point(2*A, 3));
//  ph.add_generator(point(-A));
  ph.add_generator(ray(B));

  Poly_Con_Relation rel1 = ph.relation_with(A %= 0);

  Poly_Con_Relation rel2 = ph.relation_with((A %= 1) / 5);

  Poly_Con_Relation known_rel = Poly_Con_Relation::is_disjoint();

  bool ok = (rel1 == known_rel && rel2 == known_rel);

  print_constraints(ph, "*** ph ***");
  nout << "ph.relation_with(A %= 0) == " << rel1 << endl;
  nout << "ph.relation_with((A %= 1) / 5) == " << rel2 << endl;

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
