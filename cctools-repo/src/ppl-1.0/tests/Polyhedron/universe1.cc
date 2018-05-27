/* Test Polyhedron::is_universe().
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
  C_Polyhedron ph(3);
  print_constraints(ph, "*** ph ***");

  bool universe = ph.is_universe();

  nout << "*** ph.is_universe() ***"
       << endl
       << (universe ? "true" : "false")
       << endl;

  return universe;
}

bool
test02() {
  Variable x(0);
  Variable z(2);

  C_Polyhedron ph(3);
  ph.add_constraint(x >= z);
  ph.add_constraint(z == 3);

  print_constraints(ph, "*** ph ***");

  bool universe = ph.is_universe();

  nout << "*** ph.is_universe() ***"
       << endl
       << (universe ? "true" : "false")
       << endl;

  return !universe;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(2);
  ph.add_constraint(y == 0);
  ph.add_constraint(x >= 1);
  ph.add_constraint(x <= 3);

  Constraint_System cs = ph.constraints();
  print_constraints(ph, "*** ph ***");

  bool universe = ph.is_universe();

  nout << "*** ph.is_universe() ***"
       << endl
       << (universe ? "true" : "false")
       << endl;

  return !universe;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron ph(3);
  ph.add_constraint(x >= 1);
  ph.add_constraint(y >= 1);
  ph.add_constraint(x <= 4);
  ph.add_constraint(y <= 4);

  Constraint_System cs = ph.constraints();
  print_constraints(ph, "*** ph ***");

  bool universe = ph.is_universe();

  nout << "*** ph.is_universe() ***"
       << endl
       << (universe ? "true" : "false")
       << endl;

  return !universe;
}

bool
test05() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  C_Polyhedron ph(3);
  ph.add_constraint(x - y - z >= 0);

  print_constraints(ph, "*** ph ***");

  bool universe = ph.is_universe();

  nout << "*** ph.is_universe() ***"
       << endl
       << (universe ? "true" : "false")
       << endl;

  return !universe;
}

bool
test06() {
  Variable x(0);
  Variable y(1);

  Generator_System gs;
  gs.insert(point());
  gs.insert(ray(-x));
  gs.insert(ray(x + y));
  gs.insert(ray(x - y));

  C_Polyhedron ph(gs);
  print_generators(ph, "*** ph ***");

  bool universe = ph.is_universe();

  nout << "*** ph.is_universe() ***"
       << endl
       << (universe ? "true" : "false")
       << endl;

  return universe;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
END_MAIN
