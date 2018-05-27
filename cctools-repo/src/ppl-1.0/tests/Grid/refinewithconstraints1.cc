/* Test refine_with_constraint() and refine_with_constraints().
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

// refine_with_constraint()
bool
test01() {

  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((B %= 2) / 0);

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.refine_with_constraint(A >= 2);
  gr.refine_with_constraint(B == 2);

  Grid known_gr(cgs);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.refine_with_constraint() ***");

  return ok;
}

// refine_with_constraints()
bool
test02() {

  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 3);
  cs.insert(2*B == 3);

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.refine_with_constraints(cs);

  Grid known_gr(2);
  known_gr.add_congruence((2*B %= 3) / 0);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.refine_with_constraints(cs) ***");

  return ok;
}

// Space dimension exception.
bool
test03() {
  Variable A(0);
  Variable C(2);

  Grid gr(2);

  try {
    gr.refine_with_constraint(A + C >= 0);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// Space dimension exception.
bool
test04() {
  Variable A(0);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A + C >= 0);

  Grid gr(2);

  try {
    gr.refine_with_constraints(cs);
  }
  catch (const std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

// refine_with_constraint()
bool
test05() {

  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((B %= 1) / 2);

  Grid gr(cgs);

  print_congruences(gr, "*** gr ***");

  gr.refine_with_constraint(B == 2);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.refine_with_constraint() ***");

  return ok;
}

// refine_with_constraints()
bool
test06() {

  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(0*A >= 3);

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.refine_with_constraints(cs);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.refine_with_constraints(cs) ***");

  return ok;
}

// refine_with_constraints()
bool
test07() {

  Variable A(0);
  Variable B(1);

  Grid gr(2);

  print_congruences(gr, "*** gr ***");

  gr.refine_with_constraint(Linear_Expression(1) <= 0);

  Grid known_gr(2, EMPTY);

  bool ok = (gr == known_gr);

  print_congruences(gr, "*** gr.refine_with_constraints(cs) ***");

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
END_MAIN
