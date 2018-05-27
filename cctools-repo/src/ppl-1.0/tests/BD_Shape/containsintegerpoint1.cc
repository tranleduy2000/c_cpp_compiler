/* Test BD_Shape::contains_integer_point().
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
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y <= 2);
  cs.insert(3*y >= 1);

  BD_Shape<mpz_class> bds(3);
  bds.add_constraints(cs);

  print_constraints(bds, "*** bds ***");

  bool contains = bds.contains_integer_point();

  nout << "bds.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

bool
test02() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y <= 2);
  cs.insert(3*y >= 1);

  BD_Shape<mpq_class> bds(3);
  bds.add_constraints(cs);

  print_constraints(bds, "*** bds ***");

  bool contains = bds.contains_integer_point();

  nout << "bds.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return !contains;
}

bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y <= 2);
  cs.insert(3*y >= 1);

  TBD_Shape bds(3);
  bds.add_constraints(cs);

  print_constraints(bds, "*** bds ***");

  bool contains = bds.contains_integer_point();

  nout << "bds.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  // NOTE: results depends on whether or not the rational constraints
  // on y have been approximated as integral constraints.
  typedef TBD_Shape::coefficient_type_base T;
  return std::numeric_limits<T>::is_integer ? contains : !contains;
}

bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);

  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 1);
  cs.insert(3*y - 3*z <= 2);
  cs.insert(8*z - 8*y >= 7);

  BD_Shape<mpq_class> bds(3);
  bds.add_constraints(cs);

  print_constraints(bds, "*** bds ***");

  bool contains = bds.contains_integer_point();

  nout << "bds.contains_integer_point() == "
       << (contains ? "true" : "false") << endl;

  return contains;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
