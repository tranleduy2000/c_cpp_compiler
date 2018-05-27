/* Test the PIP_Problem class.
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
  Variable X1(0);
  Variable X2(1);
  Variable I0(2);
  Variable J0(3);
  Variable N(4);
  Variables_Set params(I0, N);

  Constraint_System cs;
  cs.insert(-X1 + N - 1 >= 0);
  cs.insert(X1 - X2 >= 0);
  cs.insert(X1 + I0 == N);
  cs.insert(X2 + J0 - N - 1 >= 0);
  cs.insert(I0 >= 1);
  cs.insert(N >= 1);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test02() {
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  //cs.insert(j >= 0);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);

    // Programmatically check the expected solution.
    const PIP_Decision_Node* root = solution->as_decision();
    if (root == 0)
      return false;
    {
      // Check the root node.
      if (root->art_parameter_count() != 0)
        return false;
      const Constraint_System& cs = root->constraints();
      if (std::distance(cs.begin(), cs.end()) != 1)
        return false;
      const Constraint& c = *cs.begin();
      if (!(c.is_inequality()
            && c.coefficient(n) == 1
            && c.coefficient(m) == 0
            && c.inhomogeneous_term() == -2))
        return false;
    }
    if (root->child_node(true) == 0 || root->child_node(false) != 0)
      return false;
    const PIP_Decision_Node* t_child = root->child_node(true)->as_decision();
    if (t_child == 0)
      return false;
    {
      // Check t_child node context.
      if (t_child->art_parameter_count() != 0)
        return false;
      const Constraint_System& cs = t_child->constraints();
      if (std::distance(cs.begin(), cs.end()) != 1)
        return false;
      const Constraint& c = *cs.begin();
      if (!(c.is_inequality()
            && c.coefficient(n) == 0
            && c.coefficient(m) == 1
            && c.inhomogeneous_term() == -2))
        return false;
      // Dummy print of (non-root) tree node to increase code coverage.
      using namespace IO_Operators;
      nout << "\nPrinting the root's true child subtree:\n";
      nout << (*t_child) << endl;
    }
    if (t_child->child_node(true) == 0 || t_child->child_node(false) == 0)
      return false;
    const PIP_Solution_Node* t_t_child
      = t_child->child_node(true)->as_solution();
    if (t_t_child == 0)
      return false;
    {
      // Check t_t_child node.
      if (t_t_child->art_parameter_count() != 0)
        return false;
      const Constraint_System& cs = t_t_child->constraints();
      if (std::distance(cs.begin(), cs.end()) != 0)
        return false;
      const Linear_Expression& v_i = t_t_child->parametric_values(i);
      if (!(v_i.coefficient(n) == 0
            && v_i.coefficient(m) == 0
            && v_i.inhomogeneous_term() == 2))
        return false;
      const Linear_Expression& v_j = t_t_child->parametric_values(j);
      if (!(v_j.coefficient(n) == 0
            && v_j.coefficient(m) == 0
            && v_j.inhomogeneous_term() == 2))
        return false;
    }
    const PIP_Solution_Node* t_f_child
      = t_child->child_node(false)->as_solution();
    if (t_f_child == 0)
      return false;
    {
      // Check t_f_child node.
      // Check artificial parameter.
      if (t_f_child->art_parameter_count() != 1)
        return false;
      const PIP_Tree_Node::Artificial_Parameter& ap
        = *(t_f_child->art_parameter_begin());
      if (!(ap.coefficient(n) == 0
            && ap.coefficient(m) == 1
            && ap.denominator() == 2))
        return false;
      // Check context.
      const Constraint_System& cs = t_f_child->constraints();
      if (std::distance(cs.begin(), cs.end()) != 1)
        return false;
      const Constraint& c = *cs.begin();
      if (!(c.is_inequality()
            && c.coefficient(n) == 2
            && c.coefficient(m) == 3
            && c.inhomogeneous_term() == -8))
        return false;
      // Check parametric values.
      Variable art_p(4);
      const Linear_Expression& v_i = t_f_child->parametric_values(i);
      if (!(v_i.coefficient(n) == 0
            && v_i.coefficient(m) == -1
            && v_i.coefficient(art_p) == -1
            && v_i.inhomogeneous_term() == 4))
        return false;
      const Linear_Expression& v_j = t_f_child->parametric_values(j);
      if (!(v_j.coefficient(n) == 0
            && v_j.coefficient(m) == 1
            && v_j.coefficient(art_p) == 0
            && v_j.inhomogeneous_term() == 0))
        return false;
    }
  }
  return ok;
}

bool
test03() {
  Variable i(0);
  Variable j(1);
  Variable k(2);
  Variable m(3);
  Variable n(4);
  Variables_Set params(k, n);

  Constraint_System cs;
  cs.insert(i <= m);
  cs.insert(j <= n);
  cs.insert(2*i+j == 2*m+n-k);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test04() {
  Variable i(0);
  Variable j(1);
  Variable k(2);
  Variable m(3);
  Variable n(4);
  Variables_Set params(k, n);

  Constraint_System cs;
  cs.insert(i <= m);
  cs.insert(j <= n);
  cs.insert(2*i+j == 2*m+n-k);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  // Test copy constructor.
  {
    PIP_Problem pip_copy = pip;
    // Here we call the destructor of pip_copy
    // and we also destroy the (copied) solution tree of pip_copy.
    const PIP_Tree solution = pip_copy.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test05() {
  Variable i(0);
  Variable j(1);
  Variable m(2);
  Variable n(3);
  Variables_Set params(m, n);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(i <= n);
  cs.insert(j <= m);
  cs.insert(n >= 3);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test06() {
  Variable i(0);
  Variable n(1);
  Variables_Set params(n);

  Constraint_System cs;
  cs.insert(4*i + 2*n == 1);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == UNFEASIBLE_PIP_PROBLEM);
  if (ok)
    pip.print_solution(nout);

  return ok;
}

bool
test07() {
  Variable i(0);
  Variable j(1);
  Variable m(2);
  Variable n(3);
  Variables_Set params(m, n);

  PIP_Problem pip(4);
  pip.add_to_parameter_space_dimensions(params);

  pip.add_constraint(3*j >= -2*i+8);
  pip.add_constraint(j <= 4*i - 4);
  pip.add_constraint(i <= n);
  pip.add_constraint(n >= 3);
  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

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
test08() {
  Variable i(0);
  Variable j(1);
  Variable m(2);
  Variable n(3);
  Variables_Set params(m, n);

  PIP_Problem pip(4);
  pip.add_to_parameter_space_dimensions(params);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(i <= n);
  cs.insert(n >= 3);
  cs.insert(j <= m);

  pip.add_constraints(cs);

  pip.set_control_parameter(PIP_Problem::PIVOT_ROW_STRATEGY_MAX_COLUMN);

  bool ok = pip.is_satisfiable();
  if (ok) {
    const PIP_Tree solution = pip.optimizing_solution();
    ok &= solution->OK();
    pip.print_solution(nout);
    pip.clear();
  }

  return ok;
}

bool
test09() {
  // Same problem as test02, but using CUTTING_STRATEGY_DEEPEST.
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  //cs.insert(j >= 0);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);
  pip.set_control_parameter(PIP_Problem::CUTTING_STRATEGY_DEEPEST);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test10() {
  // Same problem as test02, but using CUTTING_STRATEGY_ALL.
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  //cs.insert(j >= 0);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);
  pip.set_control_parameter(PIP_Problem::CUTTING_STRATEGY_ALL);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test11() {
  // 0-dimension trivial PIP problem.
  PIP_Problem pip;

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }
  return ok;
}

bool
test12() {
  // Trivial PIP problem, but with 4 parameters.
  PIP_Problem pip;
  pip.add_space_dimensions_and_embed(0, 4);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }
  return ok;
}

bool
test13() {
  // Trivial PIP problem with 4 variables.
  PIP_Problem pip;
  pip.add_space_dimensions_and_embed(4, 0);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }
  return ok;
}

bool
test14() {
  // Trivial PIP problem with 4 variables and 4 parameters.
  PIP_Problem pip;
  pip.add_space_dimensions_and_embed(4, 4);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }
  return ok;
}

bool
test15() {
  PIP_Problem pip;
  // Adding trivial satisfiable constraint.
  pip.add_constraint(Linear_Expression(5) == 5);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }
  return ok;
}

bool
test16() {
  PIP_Problem pip;
  // Adding trivial unsatisfiable constraint.
  pip.add_constraint(Linear_Expression(0) == 1);
  bool ok = (pip.solve() == UNFEASIBLE_PIP_PROBLEM);
  if (pip.solution() != 0)
    pip.print_solution(nout);
  return ok;
}

bool
test17() {
  PIP_Problem pip;
  pip.add_space_dimensions_and_embed(0, 1);
  // Adding unsatisfiable context constraints.
  Variable n(0);
  pip.add_constraint(n <= 5);
  pip.add_constraint(n >= 10);
  bool ok = (pip.solve() == UNFEASIBLE_PIP_PROBLEM);
  if (pip.solution() != 0)
    pip.print_solution(nout);
  return ok;
}

bool
test18() {
  PIP_Problem pip;
  pip.add_space_dimensions_and_embed(0, 2);
  // Adding unsatisfiable context constraints.
  Variable n(0);
  Variable m(1);
  pip.add_constraint(n == 2);
  pip.add_constraint(m == 2);
  pip.add_constraint(n + m == 3);
  bool ok = (pip.solve() == UNFEASIBLE_PIP_PROBLEM);
  if (pip.solution() != 0)
    pip.print_solution(nout);
  return ok;
}

bool
test19() {
  // Same problem as test02, but incrementally adding a parameter constraint
  // making the problem unfeasible.
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  pip.add_constraint(n <= 1);
  ok &= (pip.solve() == UNFEASIBLE_PIP_PROBLEM);

  return ok;
}

bool
test20() {
  // Same problem as test02, but incrementally adding a parameter constraint
  // making the solution tree simpler.
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  pip.add_constraint(7*m >= 12);
  ok &= (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test21() {
  // Same problem as test02, but incrementally adding a parameter constraint
  // making the solution tree simpler.
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  pip.add_constraint(7*m < 12);
  ok &= (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test22() {
  // Same problem as test02, but incrementally adding two parameter constraints
  // making the problem infeasible.
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  pip.add_constraint(7*m < 12);
  pip.add_constraint(2*n + 3*m < 8);
  ok &= (pip.solve() == UNFEASIBLE_PIP_PROBLEM);

  return ok;
}

bool
test23() {
  // Same problem as test02, but incrementally adding a parameter constraint
  // removing one level in the decision tree.
  Variable i(0);
  Variable j(1);
  Variable n(2);
  Variable m(3);
  Variables_Set params(n, m);

  Constraint_System cs;
  cs.insert(3*j >= -2*i+8);
  cs.insert(j <= 4*i - 4);
  cs.insert(j <= m);
  cs.insert(i <= n);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  pip.add_constraint(7*n >= 10);
  ok &= (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test24() {
  // Minimization problem, using the big parameter trick to allow the
  // variables to be negative.
  Variable x(0);
  Variable y(1);
  Variable p(2);
  Variable M(3);
  Variables_Set params(p, M);

  Constraint_System cs;
  cs.insert(y - M >= -2*x + 2*M - 4);     // y >= -2*x - 4
  cs.insert(2*y - 2*M <= x - M + 2*p);    // 2*y <= x + 2*p

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);
  pip.set_big_parameter_dimension(3);     // M is the big parameter

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test25() {
  // Lexicographical maximization, using variable substitution.
  Variable x(0);
  Variable y(1);
  Variable p(2);
  Variable M(3);
  Variables_Set params(p, M);

  Constraint_System cs;
  cs.insert(M - y >= 2*M - 2*x - 4);      // y >= 2*x - 4
  cs.insert(M - y <= -M + x + p);         // y <= -x + p

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);
  pip.set_big_parameter_dimension(3);     // M is the big parameter

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

bool
test26() {
  // Problem generating a solution node with two context constraints.
  Variable x(0);
  Variable y(1);
  Variable p(2);
  Variable q(3);
  Variables_Set params(p, q);

  Constraint_System cs;
  cs.insert(x + p <= 2);
  cs.insert(y + q <= 1);

  PIP_Problem pip(cs.space_dimension(), cs.begin(), cs.end(), params);

  bool ok = (pip.solve() == OPTIMIZED_PIP_PROBLEM);
  if (ok) {
    const PIP_Tree solution = pip.solution();
    ok &= solution->OK();
    pip.print_solution(nout);
  }

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST_F8(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST_F8(test05);
  DO_TEST(test06);
  DO_TEST_F8(test07);
  DO_TEST_F8(test08);
  DO_TEST_F8(test09);
  DO_TEST_F8(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST_F8(test19);
  DO_TEST_F8(test20);
  DO_TEST_F8(test21);
  DO_TEST_F8(test22);
  DO_TEST_F8(test23);
  DO_TEST(test24);
  DO_TEST(test25);
  DO_TEST(test26);
END_MAIN
