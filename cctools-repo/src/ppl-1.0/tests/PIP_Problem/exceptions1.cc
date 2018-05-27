/* Test that the right exceptions are thrown in case of incorrect uses.
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
  Constraint_System cs;
  Variable x(0);
  Variables_Set params(x);

  try {
    // This is an incorrect use of the constructor:
    // the parameters in `params' should be space dimensions
    // that are valid for the PIP_Problem we are going to build.
    PIP_Problem pip(0, cs.begin(), cs.end(), params);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test02() {
  Constraint_System cs;

  try {
    // This is an incorrect use of the constructor:
    // invalid space dimension required.
    PIP_Problem pip(1 + PIP_Problem::max_space_dimension(),
                    cs.begin(), cs.end(), Variables_Set());
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test03() {
  try {
    // This is an incorrect use of the constructor:
    // invalid space dimension required.
    PIP_Problem pip(1 + PIP_Problem::max_space_dimension());
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test04() {
  PIP_Problem pip;
  try {
    // Adding too many space dimensions.
    pip.add_space_dimensions_and_embed(1, PIP_Problem::max_space_dimension());
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test05() {
  Variable X(0);
  Variable Y(1);
  Variable Z(2);
  Constraint_System cs;
  cs.insert(X == 0);
  cs.insert(Y == 1);
  cs.insert(Z <= 2);

  try {
    // This is an incorrect use of the constructor:
    // the space dimensions of the constraints should not be greater
    // than the space dimension of the PIP problem.
    PIP_Problem pip(2, cs.begin(), cs.end(), Variables_Set());
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test06() {
  PIP_Problem pip;
  try {
    // Adding invalid parameter space dimensions.
    Variable m(4);
    pip.add_to_parameter_space_dimensions(Variables_Set(m));
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test07() {
  PIP_Problem pip;
  try {
    // Adding space dimension incompatible constraint.
    Variable x(2);
    pip.add_constraint(x >= 0);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test08() {
  PIP_Problem pip;
  try {
    // Setting an invalid control parameter value.
    pip.set_control_parameter(PIP_Problem::CONTROL_PARAMETER_VALUE_SIZE);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test09() {
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
  (void) pip.is_satisfiable();
  // Adding 2 additional space dimensions.
  pip.add_space_dimensions_and_embed(2, 0);

  try {
    // Trying to mark a problem variable as a parameter.
    pip.add_to_parameter_space_dimensions(Variables_Set(X1));
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test10() {
  PIP_Problem pip;

  try {
    // Trying to set an invalid big parameter dimension.
    pip.set_big_parameter_dimension(1);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test11() {
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
  (void) pip.is_satisfiable();
  // Adding 2 additional space dimensions.
  pip.add_space_dimensions_and_embed(2, 0);

  try {
    // Trying to set an invalid big parameter dimension.
    pip.set_big_parameter_dimension(3);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test12() {
  typedef PIP_Tree_Node::Artificial_Parameter Art_Param;
  Variable A(0);

  try {
    // Trying to set an invalid (zero) denominator.
    Art_Param ap(3*A + 8, 0);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test13() {
  PIP_Problem pip;
  try {
    // Printing the solution before trying to solve the problem.
    pip.print_solution(nout);
  }
  catch (std::logic_error& e) {
    nout << "logic_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test14() {
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
  (void) pip.solve();

  const PIP_Decision_Node* root = pip.solution()->as_decision();
  const PIP_Decision_Node* t_child = root->child_node(true)->as_decision();
  const PIP_Solution_Node* t_t_child = t_child->child_node(true)->as_solution();

  try {
    // It is illegal to ask for the parametric value of a parameter.
    (void) t_t_child->parametric_values(n);
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test15() {
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
  (void) pip.solve();

  const PIP_Decision_Node* root = pip.solution()->as_decision();
  const PIP_Decision_Node* t_child = root->child_node(true)->as_decision();
  const PIP_Solution_Node* t_t_child = t_child->child_node(true)->as_solution();

  try {
    // It is illegal to ask for the parametric value of a variable
    // having space dimension greater than that o fthe problem.
    (void) t_t_child->parametric_values(Variable(4));
  }
  catch (std::invalid_argument& e) {
    nout << "invalid_argument: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
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
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST_F8(test14);
  DO_TEST_F8(test15);
END_MAIN
