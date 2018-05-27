/* Test minimization of NNC polyhedra.
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
#include <algorithm>

namespace {

bool
test01() {
  Variable x(0);
  Variable y(1);

  // Building a square.
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 6);
  cs.insert(y >= 0);
  cs.insert(y <= 6);

  NNC_Polyhedron ph(cs);

  nout << "Topologically closed square" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  // Removing all the vertices using strict inequalities.
  cs.clear();
  cs.insert(x + y > 0);
  cs.insert(x + y < 12);
  cs.insert(x - y < 6);
  cs.insert(x - y > -6);

  ph.add_constraints(cs);

  nout << "After vertices removal:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  ph.minimized_generators();

  Generator_System gs;
  gs.insert(closure_point());
  gs.insert(closure_point(6*x));
  gs.insert(closure_point(6*y));
  gs.insert(closure_point(6*x + 6*y));
  gs.insert(point(3*x));
  gs.insert(point(3*y));
  gs.insert(point(3*x + 6*y));
  gs.insert(point(6*x + 3*y));

  NNC_Polyhedron known_result(gs);

  bool ok = (ph == known_result);

  const Generator_System& min_gs = ph.minimized_generators();
  bool ok1 = (std::distance(min_gs.begin(), min_gs.end()) == 8);

  nout << "After NNC minimization:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  return ok && ok1;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  // Building a square.
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(y >= 0);
  cs.insert(y <= 1);

  NNC_Polyhedron ph(cs);

  nout << "Topologically closed square" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  // Removing all the vertices using strict inequalities.
  cs.clear();
  cs.insert(x + y > 0);
  cs.insert(x + y < 2);
  cs.insert(x - y < 1);
  cs.insert(x - y > -1);

  ph.add_constraints(cs);

  nout << "After vertices removal:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  ph.minimized_generators();

  Generator_System gs;
  gs.insert(closure_point());
  gs.insert(closure_point(x));
  gs.insert(closure_point(y));
  gs.insert(closure_point(x + y));
  gs.insert(point(x, 10));
  gs.insert(point(y, 10));
  gs.insert(point(x + 10*y, 10));
  gs.insert(point(10*x + y, 10));

  NNC_Polyhedron known_result(gs);

  bool ok = (ph == known_result);

  const Generator_System& min_gs = ph.minimized_generators();
  bool ok1 = (std::distance(min_gs.begin(), min_gs.end()) == 8);

  nout << "After NNC minimization:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");
  nout << endl;
  nout << "known result" << endl;
  print_constraints(known_result.constraints(), "*** known constraints ***");
  print_generators(known_result.generators(), "*** known generators ***");

  return ok && ok1;
}

bool
test03() {
  Variable x(0);
  Variable y(1);

  // Building a square.
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(y >= 0);
  cs.insert(y <= 1);

  NNC_Polyhedron ph(cs);

  nout << "Topologically closed square" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  // Removing all the vertices using strict inequalities.
  cs.clear();
  cs.insert(x + y > 0);
  cs.insert(x + y < 2);
  cs.insert(x - y < 1);
  cs.insert(x - y > -1);

  ph.add_constraints(cs);

  nout << "After vertices removal:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  Generator_System gs;
  gs.insert(point(x + y, 4));
  gs.insert(point(x + 5*y, 4));
  gs.insert(point(5*x + y, 4));
  gs.insert(point(5*x + 5*y, 4));

  NNC_Polyhedron ph2(gs);

  ph.intersection_assign(ph2);

  nout << "After intersection:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  ph.minimized_generators();

  gs.clear();
  gs.insert(closure_point(x + y));
  gs.insert(point(x + y, 4));
  gs.insert(point(x + 4*y, 4));
  gs.insert(point(4*x + y, 4));
  NNC_Polyhedron known_result(gs);

  bool ok = (ph == known_result);

  const Generator_System& min_gs = ph.minimized_generators();
  bool ok1 = (std::distance(min_gs.begin(), min_gs.end()) == 4);

  nout << "After NNC minimization:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  return ok && ok1;
}

bool
test04() {
  Variable x(0);
  Variable y(1);

  // Building an open square.
  Generator_System gs;
  gs.insert(closure_point());
  gs.insert(closure_point(15*x));
  gs.insert(closure_point(15*y));
  gs.insert(closure_point(15*x + 15*y));

  // All of these points, but a (any) single one of them, are redundant.
  gs.insert(point(3*x + 3*y));
  gs.insert(point(6*x + y));
  gs.insert(point(9*x + y));
  gs.insert(point(12*x + 3*y));
  gs.insert(point(3*x + 12*y));
  gs.insert(point(6*x + 14*y));
  gs.insert(point(9*x + 14*y));
  gs.insert(point(12*x + 12*y));
  gs.insert(point(x + 6*y));
  gs.insert(point(x + 9*y));
  gs.insert(point(14*x + 6*y));
  gs.insert(point(14*x + 9*y));

  NNC_Polyhedron ph(gs);

  nout << endl << "Before NNC minimization:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  ph.minimized_constraints();

  nout << endl << "After NNC minimization:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  gs.clear();
  gs.insert(closure_point());
  gs.insert(closure_point(15*x));
  gs.insert(closure_point(15*y));
  gs.insert(closure_point(15*x + 15*y));
  gs.insert(point(x + y));

  NNC_Polyhedron known_result(gs);
  bool ok = (ph == known_result);

  const Generator_System& min_gs = ph.minimized_generators();
  bool ok1 = (std::distance(min_gs.begin(), min_gs.end()) == 5);

  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  print_constraints(known_result.constraints(),
		    "*** known_result constraints ***");
  print_generators(known_result.generators(),
		   "*** known_result generators ***");
  return ok && ok1;
}

bool
test05() {
  Variable x(0);

  Constraint_System cs;
  cs.insert(x > 0);
  cs.insert(x < 2);

  NNC_Polyhedron ph1(cs);

  cs.clear();
  cs.insert(x > 2);
  cs.insert(x < 3);

  NNC_Polyhedron ph2(cs);

  ph1.upper_bound_assign(ph2);
  (void) ph1.is_empty();

  nout << "(Weakly) minimized poly hull" << endl;
  print_constraints(ph1.constraints(), "*** ph1 constraints ***");
  print_generators(ph1.generators(), "*** ph1 generators ***");

  NNC_Polyhedron copy_ph1(ph1);

  const Constraint_System& ph1_cs = ph1.constraints();
  const int num_constraints = std::distance(ph1_cs.begin(), ph1_cs.end());

  const Constraint_System& ph1_min_cs = ph1.minimized_constraints();
  const int num_minimized_constraints = std::distance(ph1_min_cs.begin(),
						      ph1_min_cs.end());

  print_constraints(ph1, "*** after ph1.minimized_constraints() ***");
  nout << "num_constraints = " << num_constraints << endl;
  nout << "num_minimized_constraints = "
       << num_minimized_constraints << endl;

  int num_points = 0;
  for (Generator_System::const_iterator i = copy_ph1.generators().begin(),
	 gs_end = copy_ph1.generators().end(); i != gs_end; ++i)
    if ((*i).is_point() || (*i).is_closure_point())
      ++num_points;

  copy_ph1.minimized_generators();

  int num_minimized_points = 0;
  for (Generator_System::const_iterator i = copy_ph1.generators().begin(),
	 gs_end = copy_ph1.generators().end(); i != gs_end; ++i)
    if ((*i).is_point() || (*i).is_closure_point())
      ++num_minimized_points;

  bool ok = (num_constraints == num_minimized_constraints + 1
	     && num_points == num_minimized_points + 1);

  print_generators(copy_ph1,
		   "*** after copy_ph1_minimized_generators() ***");
  nout << "num_points = " << num_points << endl;
  nout << "num_minimized_points = "
       << num_minimized_points << endl;

  return ok;
}

bool
test06() {
  Variable x(0);

  Constraint_System cs;
  cs.insert(x > 0);

  NNC_Polyhedron ph(cs);

  cs.clear();
  cs.insert(3*x >= 1);
  cs.insert(2*x <= 1);

  ph.add_constraints(cs);

  nout << endl << "Before NNC minimization:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  ph.minimized_constraints();

  NNC_Polyhedron known_result(1);
  known_result.add_constraint(3*x >= 1);
  known_result.add_constraint(2*x <= 1);

  bool ok = (ph == known_result);

  nout << endl << "After NNC minimization:" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8A(test01);
  DO_TEST_F8(test02);
  DO_TEST(test03);
  DO_TEST_F8(test04);
  DO_TEST(test05);
  DO_TEST(test06);
END_MAIN
