/* Test the PIP_Problem class.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);

  PIP_Problem pip;
  pip.add_space_dimensions_and_embed(2, 0);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);

  pip.add_constraints(cs);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (!ok)
    return ok;

  {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
    nout << endl;
  }

  pip.add_space_dimensions_and_embed(0, 2);
  pip.add_constraint(j <= m);
  pip.add_constraint(i <= n);

  ok &= (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }
  return ok;
}

bool
test02() {
  // NOTE_ changing the order of insertion of variables.
  Variable i(0);
  Variable n(1);
  Variable j(2);
  Variable m(3);

  PIP_Problem pip;
  pip.add_space_dimensions_and_embed(1, 1);

  Constraint_System cs;
  pip.add_constraint(i <= n);

  pip.add_constraints(cs);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (!ok)
    return ok;

  {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
    nout << endl;
  }

  pip.add_space_dimensions_and_embed(1, 1);
  pip.add_constraint(3*j >= -2*i+8);
  pip.add_constraint(j <= 4*i - 4);
  pip.add_constraint(j <= m);

  ok &= (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }
  return ok;
}

bool
test03() {
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);

  PIP_Problem pip;
  pip.add_space_dimensions_and_embed(2, 2);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  pip.add_constraint(j <= m);
  pip.add_constraint(i <= n);

  pip.add_constraints(cs);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (!ok)
    return ok;

  {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
    nout << endl;
  }

  Variable k(4);
  Variable p(5);
  pip.add_space_dimensions_and_embed(1, 1);
  pip.add_constraint(k <= i + p);
  pip.add_constraint(p <= n + 2);

  ok &= (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }
  return ok;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A == B);
  cs.insert(A == -1);

  PIP_Problem pip(2);
  pip.add_constraints(cs);

  bool ok = !pip.is_satisfiable();

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8(test02);
  DO_TEST_F8(test03);
  DO_TEST(test04);
END_MAIN
