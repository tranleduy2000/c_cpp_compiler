/* Test Octagonal_Shape<T>::minimized_constraints().
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

  TOctagonal_Shape oct(2);
  oct.add_constraint(A >= 1);
  oct.add_constraint(B >= 0);
  oct.add_constraint(A - B >= -2);
  oct.add_constraint(A >= -3);
  oct.add_constraint(A <= 1);

  print_constraints(oct, "*** oct ***");

  const Constraint_System cs = oct.minimized_constraints();

  using namespace IO_Operators;
  nout << "*** oct.minimized_constraints() ***" << endl;

  dimension_type num_constraints = 0;
  for (Constraint_System::const_iterator i = cs.begin(),
         iend = cs.end(); i != iend; ++i) {
    nout << *i << endl;
    ++num_constraints;
  }

  nout << "num_constraints == " << num_constraints << endl;

  C_Polyhedron ph_oct(cs);
  C_Polyhedron known_result(2);
  known_result.add_constraint(A == 1);
  known_result.add_constraint(B >= 0);
  known_result.add_constraint(B <= 3);

  bool ok = (num_constraints == 3 && known_result == ph_oct);

  return ok;
}

bool
test02() {
  Variable A(0);
  Variable B(1);

  TOctagonal_Shape oct(2);
  oct.add_constraint(A == 2);
  oct.add_constraint(B == 3);
  oct.add_constraint(A - B == -1);
  oct.add_constraint(A + B == 5);

  print_constraints(oct, "*** oct ***");

  const Constraint_System cs = oct.minimized_constraints();

  using namespace IO_Operators;
  nout << "*** oct.minimized_constraints() ***" << endl;

  dimension_type num_constraints = 0;
  for (Constraint_System::const_iterator i = cs.begin(),
         iend = cs.end(); i != iend; ++i) {
    nout << *i << endl;
    ++num_constraints;
  }

  nout << "num_constraints == " << num_constraints << endl;

  C_Polyhedron ph_oct(cs);
  C_Polyhedron known_result(2);
  known_result.add_constraint(A >= 2);
  known_result.add_constraint(A - B <= -1);
  known_result.add_constraint(B <= 3);

  bool ok = (num_constraints == 3 && known_result == ph_oct);

  return ok;
}

bool
test03() {
  TOctagonal_Shape oc(0, UNIVERSE);

  print_constraints(oc, "*** oc ***");

  const Constraint_System cs = oc.minimized_constraints();

  using namespace IO_Operators;
  nout << "*** oc.minimized_constraints() ***" << endl;

  dimension_type num_constraints = 0;
  for (Constraint_System::const_iterator i = cs.begin(),
         iend = cs.end(); i != iend; ++i) {
    nout << *i << endl;
    ++num_constraints;
  }

  nout << "num_constraints == " << num_constraints << endl;

  return num_constraints == 0;
}


} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
