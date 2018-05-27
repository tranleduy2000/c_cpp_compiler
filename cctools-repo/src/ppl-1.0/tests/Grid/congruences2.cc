/* Test Congruence_System::satisfies_all_congruences().
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

using namespace Parma_Polyhedra_Library::IO_Operators;

namespace {

class Test_Congruence_System : public Congruence_System {
public:
  Test_Congruence_System() : Congruence_System() {}
  Test_Congruence_System(Congruence_System cgs) : Congruence_System(cgs) {}
  Test_Congruence_System(Congruence cg) : Congruence_System(cg) {}
  bool
  satisfies_all_congruences(const Grid_Generator& g) const {
    return Congruence_System::satisfies_all_congruences(g);
  }
};

// Public constructors.
bool
test01() {
  Test_Congruence_System cgs0;

  print_congruences(cgs0, "*** cgs0 ***");

  Test_Congruence_System cgs1(cgs0);

  bool ok = (cgs1 == cgs0);

  print_congruences(cgs1, "*** cgs1(cgs0) ***");

  if (ok) {
    Variable A(0);
    Variable B(1);

    Test_Congruence_System cgs2((A - 3*B %= 5) / 2);

    ok &= !(cgs2 == cgs0) && !(cgs2 == cgs1);

    print_congruences(cgs2, "*** cgs2((A - 3*B %= 5) / 2) ***");
  };

  return ok;
}

// operator=
bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence_System cgs0;

  Test_Congruence_System cgs1((A - 3*B + 21*C %= 5) / 2);

  cgs0 = cgs1;

  bool ok = (cgs0 == cgs1);

  print_congruences(cgs0, "*** cgs0 ***");

  return ok;
}

// space_dimension
bool
test03() {
  Variable A(3);
  Variable B(7);
  Variable C(4);

  Test_Congruence_System cgs0((A - 3*B + 21*C %= 55) / 21);

  bool ok = (cgs0.space_dimension() == 8);

  print_congruences(cgs0, "*** cgs0((A - 3*B + 21*C %= 55) / 21) ***");

  return ok;
}

// clear
bool
test04() {
  Variable A(0);
  Variable B(1);

  Test_Congruence_System cgs0((A - 3*B %= 5) / 7);

  print_congruences(cgs0, "*** cgs0 ***");

  cgs0.clear();

  bool ok = (cgs0.space_dimension() == 0);

  print_congruences(cgs0, "*** cgs0.clear() ***");

  return ok;
}

// insert
bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Test_Congruence_System cgs0((A - 3*B + C - 18*D %= 5) / 7);
  cgs0.insert((A %= 0) / 3);
  cgs0.insert((A + B %= 3) / 5);

  dimension_type tem = cgs0.num_proper_congruences();
  bool ok = (tem == 3);

  print_congruences(cgs0, "*** cgs0.insert ***");

  return ok;
}

// num_[non_]equalities
bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence_System cgs0((-A - B + 13*C %= 6) / 7);
  cgs0.insert((A %= 0) / 0);
  cgs0.insert((A + B %= 3) / 0);

  dimension_type neqs = cgs0.num_equalities();
  dimension_type npcgs = cgs0.num_proper_congruences();
  bool ok = (neqs == 2 && npcgs == 1);

  print_congruences(cgs0, "*** cgs0.insert ***");

  return ok;
}

// insert, including a row with all terms zero and an equality.
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Test_Congruence_System cgs0(0*A + 0*B %= -1);
  cgs0.insert((A + 0*B %= 1) / 0);
  cgs0.insert(0*A + 1*B %= 1);

  dimension_type neqs = cgs0.num_equalities();
  dimension_type npcgs = cgs0.num_proper_congruences();
  bool ok = (neqs == 1 && npcgs == 2);

  print_congruences(cgs0, "*** cgs0.insert ***");

  return ok;
}

/* If EXPECTED is true:
     If G satisfies CGS, then return true, else print an error message
     and return false.

   And if EXPECTED is false:
     If G satisfies CGS, then print an error message and return false,
     else return true.
*/
bool
satisfies(const Grid_Generator& g,
	  const Test_Congruence_System& cgs,
	  bool expected = true) {
  if (cgs.satisfies_all_congruences(g) == expected)
    return true;

  nout << g << " should";
  expected || nout << "fail to";
  nout << " satisfy " << cgs << "." << endl
       << "ASCII dump of " << g << ":" << endl;
  g.ascii_dump(nout);
  nout << "ASCII dump of " << cgs << ":" << endl;
  cgs.ascii_dump(nout);

  return false;
}

inline bool
fails_to_satisfy(const Grid_Generator& g,
		 const Congruence_System& cgs) {
  return satisfies(g, cgs, false);
}

// Divisor of 1.
bool
test08() {
  Variable A(0);
  Variable B(1);

  Test_Congruence_System cgs0((A - B %= 0) / 7);
  print_congruences(cgs0, "*** cgs0((A - B %= 0) / 7) ***");

  // Points.

  bool ok = (satisfies(grid_point(), cgs0));

  ok &= (satisfies(grid_point(A + B), cgs0));

  ok &= (fails_to_satisfy(grid_point(A + 2*B), cgs0));

  ok &= (fails_to_satisfy(grid_point(5*A + 2*B), cgs0));

  ok &= (satisfies(grid_point(5*A - 2*B), cgs0));

  // Parameters.

  ok &= (satisfies(parameter(3*A + 3*B), cgs0));

  ok &= (satisfies(parameter(0*A + 14*B), cgs0));

  ok &= (satisfies(parameter(-A + 13*B), cgs0));

  // Lines.

  ok &= (satisfies(grid_line(13*A + 13*B), cgs0));

  ok &= (fails_to_satisfy(grid_line(18*A + 14*B), cgs0));

  ok &= (fails_to_satisfy(grid_line(14*A - 21*B), cgs0));

  cgs0.insert((A %= 0) / 2);
  ok &= (fails_to_satisfy(grid_line(3*A), cgs0));
  print_congruences(cgs0,
		    "*** cgs0.clear(); cgs0.insert((A %= 0) / 2) ***");

  return ok;
}

// Divisors greater than 1.
bool
test09() {
  Variable A(0);
  Variable B(1);

  Test_Congruence_System cgs0((A - B %= 0) / 7);
  print_congruences(cgs0,
		    "*** cgs0((A - B %= 0) / 7) ***");

  // Points.

  bool ok = (satisfies(grid_point(A + B, 3), cgs0));

  ok &= (fails_to_satisfy(grid_point(A + 2*B, 3), cgs0));

  ok &= (fails_to_satisfy(grid_point(5*A + 2*B, 5), cgs0));

  ok &= (fails_to_satisfy(grid_point(5*A - 2*B, 7), cgs0));

  // Parameters.

  ok &= (satisfies(parameter(3*A + 3*B, 4), cgs0));

  ok &= (fails_to_satisfy(parameter(A + 14*B, 5), cgs0));

  ok &= (satisfies(parameter(-A + 13*B, 2), cgs0));

  // Lines.

  ok &= (satisfies(grid_line(13*A + 13*B), cgs0));

  ok &= (fails_to_satisfy(grid_line(18*A + 14*B), cgs0));

  ok &= (fails_to_satisfy(grid_line(14*A - 21*B), cgs0));

  cgs0.clear();
  cgs0.insert((A %= 0) / 2);
  ok &= (fails_to_satisfy(grid_line(3*A), cgs0));
  print_congruences(cgs0,
		    "*** cgs0.clear(); cgs0.insert((A %= 0) / 2) ***");

  return ok;
}

// clear empty congruence system
bool
test10() {
  Variable A(0);
  Variable B(1);

  Test_Congruence_System cgs0;

  print_congruences(cgs0, "*** cgs0 ***");

  cgs0.clear();

  bool ok = (cgs0.space_dimension() == 0);

  print_congruences(cgs0, "*** cgs0.clear() ***");

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
  DO_TEST(test09);
  DO_TEST(test10);
END_MAIN
