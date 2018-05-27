/* Test Pointset_Powerset<PH>::contains_integer_point(),
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

// Powerset of C polyhedra: contains_integer_point() returns false.
bool
test01() {
  Variable x(0);
  Variable y(1);

  Pointset_Powerset<C_Polyhedron> ps(2);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(4*y <= 3);
  cs.insert(4*y >= 1);

  ps.refine_with_constraints(cs);

  bool contains = ps.contains_integer_point();

  print_constraints(ps, "*** ps ***");
  nout << "ps.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

// Powerset of C polyhedra: contains_integer_point() returns true.
bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Pointset_Powerset<C_Polyhedron> ps(3);
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(4*z - 4*y >= 3);

  ps.refine_with_constraints(cs);

  print_constraints(ps, "*** ps ***");

  bool contains = ps.contains_integer_point();

  nout << "ps.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

// Empty 0-dimensional powerset of C polyhedra: contains_integer_point().
bool
test03() {
  Pointset_Powerset<C_Polyhedron> ps(0, EMPTY);

  print_constraints(ps, "*** ps ***");

  bool contains = ps.contains_integer_point();

  nout << "ps.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

// Universe 0-dimensional powerset of C polyhedra: contains_integer_point().
bool
test04() {
  Pointset_Powerset<C_Polyhedron> ps(0);

  bool contains = ps.contains_integer_point();

  print_constraints(ps, "*** ps ***");

  nout << "ps.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

// Empty 1-dimensional powerset of C polyhedra: contains_integer_point().
bool
test05() {
  Pointset_Powerset<C_Polyhedron> ps(1);

  ps.refine_with_constraint(Linear_Expression(0) == 1);

  bool contains = ps.contains_integer_point();

  print_constraints(ps, "*** ps ***");

  nout << "ps.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
