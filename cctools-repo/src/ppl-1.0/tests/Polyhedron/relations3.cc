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

  NNC_Polyhedron ph(2);
  ph.add_constraint(A - B > 0);
  ph.add_constraint(B >= 0);

  Poly_Gen_Relation rel1 = ph.relation_with(point(B));
  Poly_Gen_Relation rel2 = ph.relation_with(point(-B));

  print_generators(ph, "*** ph ***");
  nout << "ph.relation_with(point(B)) == " << rel1 << endl;
  nout << "ph.relation_with(point(-B)) == " << rel2 << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();
  return rel1 == known_result && rel2 == known_result;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph1(2, EMPTY);
  ph1.add_generator(point(A + B));

  Poly_Con_Relation rel = ph1.relation_with(A - B == 0);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::saturates()
    && Poly_Con_Relation::is_included();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(A - B == 0) = " << rel << endl;

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  gs.insert(line(B));

  NNC_Polyhedron ph1(gs);

  Poly_Con_Relation rel = ph1.relation_with(A >= 1);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(A >= 1) = " << rel << endl;

  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(line(A));
  gs.insert(ray(B));
  gs.insert(point());
  NNC_Polyhedron ph1(gs);

  Poly_Con_Relation rel = ph1.relation_with(A > 1);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(A > 1) = " << rel << endl;

  return ok;
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(line(B));
  gs.insert(point());
  gs.insert(ray(A));
  NNC_Polyhedron ph(gs);
  Generator_System gs1;
  for (Generator_System::const_iterator i = ph.generators().begin(),
	 gs_end = ph.generators().end(); i != gs_end; ++i)
    if (!(*i).is_closure_point())
      gs.insert(*i);
  C_Polyhedron ph1(gs);

  Poly_Con_Relation rel = ph1.relation_with(B >= 1);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(B >= 1) = " << rel << endl;

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(A));
  NNC_Polyhedron ph(gs);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A == 1);
  ph1.add_constraint(B == 1);
  Generator_System gs1;
  for (Generator_System::const_iterator i = ph.generators().begin(),
	 gs_end = ph.generators().end(); i != gs_end; ++i)
    if (!(*i).is_closure_point())
      gs1.insert(*i);
  ph1.add_generators(gs1);

  Poly_Con_Relation rel = ph1.relation_with(B == 1);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(B == 1) = " << rel << endl;

  return ok;
}

bool
test07() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(ray(B));
  gs.insert(point(-A));
  C_Polyhedron ph1(gs);
  ph1.generators();

  Poly_Con_Relation rel = ph1.relation_with(B <= 0);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(B <= 0) = " << rel << endl;

  return ok;
}

bool
test08() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(A + B));
  gs.insert(point(-A + B));
  C_Polyhedron ph1(gs);
  ph1.generators();

  Poly_Con_Relation rel = ph1.relation_with(A >= 0);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(A >= 0) = " << rel << endl;

  return ok;
}

bool
test09() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(-A));
  gs.insert(ray(-B));
  gs.insert(ray(A + B));
  NNC_Polyhedron ph1(gs);
  ph1.generators();

  Poly_Con_Relation rel = ph1.relation_with(B < 0);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::strictly_intersects();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(B < 0) = " << rel << endl;

  return ok;
}

bool
test10() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(ray(A));
  gs.insert(ray(A + B));
  gs.insert(point(-B));
  NNC_Polyhedron ph1(gs);
  ph1.generators();

  Poly_Con_Relation rel = ph1.relation_with(A < 0);
  Poly_Con_Relation  known_rel = Poly_Con_Relation::is_disjoint();

  bool ok = (rel == known_rel);

  print_generators(ph1, "*** ph1 ***");
  nout << "ph1.relation_with(A < 0) = " << rel << endl;

  return ok;
}

bool
test11() {
  Variable A(0);
  Variable B(1);

  NNC_Polyhedron ph(2);
  ph.add_constraint(A == 0);
  ph.add_constraint(B == 0);

  Poly_Gen_Relation rel = ph.relation_with(closure_point(A));

  print_constraints(ph, "*** ph ***");
  nout << "ph.relation_with(line(A)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::nothing();
  return rel == known_result;
}

bool
test12() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  C_Polyhedron ph(3);
  ph.add_constraint(A >= B);
  ph.add_generator(point());
  ph.generators();
  ph.add_constraint(A <= B-1);

  Poly_Gen_Relation rel = ph.relation_with(line(C));

  print_constraints(ph, "*** ph ***");
  nout << "ph.relation_with(line(C)) == " << rel << endl;

  Poly_Gen_Relation known_result = Poly_Gen_Relation::subsumes();
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
END_MAIN
