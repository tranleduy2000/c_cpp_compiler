/* Test Polyhedron::add_generator().
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

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(A));
  gs.insert(line(B));
  NNC_Polyhedron ph1(gs);

  print_generators(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, EMPTY);
  ph2.add_generator(point(-A));

  print_generators(ph2, "*** ph2 ***");

  const Generator_System& gs1 = ph1.minimized_generators();
  for (Generator_System::const_iterator i = gs1.begin(),
	 gs1_end = gs1.end(); i != gs1_end; ++i)
    ph2.add_generator(*i);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= -1);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2add_generator(*i) ***");

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(point(A + B));
  NNC_Polyhedron ph1(gs);

  print_generators(ph1, "*** ph1 ***");

  C_Polyhedron ph2(2, EMPTY);

  print_generators(ph2, "*** ph2 ***");

  const Generator_System& gs1 = ph1.minimized_generators();
  for (Generator_System::const_iterator i = gs1.begin(),
	 gs1_end = gs1.end(); i != gs1_end; ++i)
    ph2.add_generator(*i);

  C_Polyhedron known_result(2);
  known_result.add_constraint(A == 1);
  known_result.add_constraint(B == 1);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** after ph2.add_generator(*i) ***");

  return ok;
}

bool
test03() {
  Variable A(0);
  Variable B(1);

  Generator_System gs;
  gs.insert(closure_point(3*A, 2));
  gs.insert(point(7*A, 4));
  gs.insert(ray(A - B));

  print_generators(gs, "*** gs ***");

  C_Polyhedron ph(2, EMPTY);

  for (Generator_System::const_iterator i = gs.begin(),
	 gs_end = gs.end(); i != gs_end; ++i)
    if (!(*i).is_closure_point())
      ph.add_generator(*i);

  Generator_System gs_known;
  gs_known.insert(point(7*A + 0*B, 4));
  gs_known.insert(ray(A - B));
  C_Polyhedron known_result(gs_known);

  bool ok = (ph == known_result);

  print_generators(gs, "*** gs ***");
  print_generators(ph, "*** ph ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
