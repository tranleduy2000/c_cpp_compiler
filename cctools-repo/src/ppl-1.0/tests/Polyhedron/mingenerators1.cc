/* Test Polyhedron::minimized_generators().
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
  gs1.insert(point(B));
  gs1.insert(line(A));
  gs1.insert(ray(B));
  gs1.insert(ray(A + B));
  gs1.insert(ray(-A + B));

  C_Polyhedron ph1(gs1);

  const Generator_System gs2 = ph1.minimized_generators();

  print_generators(gs2, "*** gs2 ***");

  C_Polyhedron ph2(gs2);
  Generator_System known_gs;
  known_gs.insert(point());
  known_gs.insert(line(A));
  known_gs.insert(ray(B));
  C_Polyhedron known_result(known_gs);

  bool ok = (ph2 == known_result);

  print_generators(ph2, "*** ph2 ***");

  return ok;
}

bool
test02() {
  C_Polyhedron ph1(2, EMPTY);

  print_generators(ph1, "*** ph1 ***");

  C_Polyhedron known_result = ph1;

  Generator_System gs = ph1.minimized_generators();

  C_Polyhedron ph2(2, EMPTY);
  ph2.add_generators(gs);

  bool ok = (ph2 == known_result);

  print_generators(gs, "*** gs ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
