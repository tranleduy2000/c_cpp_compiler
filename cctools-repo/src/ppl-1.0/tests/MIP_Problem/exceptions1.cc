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
  Variable A(0);
  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(A > -6);
  MIP_Problem mip(cs.space_dimension());

  try {
    // This tries to build an invalid MIP_Problem object: the feasible
    // region can not be defined using strict inequalities.
    mip.add_constraints(cs);
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
  Variable A(0);
  MIP_Problem mip;

  try {
    // This tries to build an invalid MIP_Problem object: the space dimension
    // of the objective function can not be greater than the space dimension
    // of the feasible region.
    mip.set_objective_function(A);
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
test03() {
  Variable A(0);
  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(A <= 0);
  MIP_Problem mip(cs.space_dimension(), cs, A, MAXIMIZATION);

  try {
    // We cannot extract a feasible point from an unsatisfiable MIP_Problem.
    Generator fp = mip.feasible_point();
  }
  catch (std::domain_error& e) {
    nout << "domain_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test04() {
  Variable A(0);
  Constraint_System cs;
  cs.insert(A >= 6);
  MIP_Problem mip(cs.space_dimension(), cs, A, MAXIMIZATION);

  try {
    // We cannot extract an optimizing point from an unbounded MIP_Problem.
    Generator fp = mip.optimizing_point();
  }
  catch (std::domain_error& e) {
    nout << "domain_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
  }
  return false;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs;
  cs.insert(A >= 6);
  MIP_Problem mip(cs.space_dimension(), cs, A, MAXIMIZATION);
  Generator p = point(A + B);
  Coefficient num;
  Coefficient den;

  try {
    // This tries to evaluate the objective function on a space-dimension
    // incompatible generator.
    mip.evaluate_objective_function(p, num, den);
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
  Variable A(0);
  Constraint_System cs;
  cs.insert(A >= 6);
  MIP_Problem mip(cs.space_dimension(), cs, A, MAXIMIZATION);
  Generator r = ray(A);
  Coefficient num;
  Coefficient den;

  try {
    // This tries to evaluate the objective function on a ray.
    mip.evaluate_objective_function(r, num, den);
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
  try {
    // This tries to overflow the maximum space dimension.
    MIP_Problem mip(MIP_Problem::max_space_dimension() + 1);
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
test08() {
  MIP_Problem mip(1);
  try {
    // This tries to overflow the maximum space dimension.
    mip.add_space_dimensions_and_embed(MIP_Problem::max_space_dimension());
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
test09() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(A <= 0);
  dimension_type cs_space_dimension = cs.space_dimension();

  Linear_Expression cost(A + B);

  try {
    // This tries to make the cost function incompatible with the MIP_Problem
    // space dimension.
    MIP_Problem mip(cs_space_dimension, cs, cost, MAXIMIZATION);
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
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(A <= 0);

  Linear_Expression cost(A + B);

  try {
    // This tries to overflow the maximum space dimension.
    MIP_Problem mip(MIP_Problem::max_space_dimension() + 1,
		    cs, cost, MAXIMIZATION);
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
test11() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(A < 0);
  dimension_type cs_space_dimension = cs.space_dimension();

  Linear_Expression cost(A + B);

  try {
    // This tries to build an MIP_Problem with strict inequalities.
    MIP_Problem mip(cs_space_dimension, cs, cost, MAXIMIZATION);
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
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(B <= 0);
  dimension_type cs_space_dimension = cs.space_dimension();

  Linear_Expression cost(A + B);

  MIP_Problem mip(cs_space_dimension, cs, cost, MAXIMIZATION);
  try {
    // This tries to add Constraint that exceeds the MIP_Problem
    // space dimension.
    mip.add_constraint(C >= 0);
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
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(B <= 0);
  dimension_type cs_space_dimension = cs.space_dimension();

  Linear_Expression cost(A + B);

  MIP_Problem mip(cs_space_dimension, cs, cost, MAXIMIZATION);
  try {
    // This tries to add a strict inequality.
    mip.add_constraint(B > 0);
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
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(B <= 0);
  dimension_type cs_space_dimension = cs.space_dimension();

  Linear_Expression cost(A + B);

  Constraint_System incompatible_cs;
  incompatible_cs.insert(C >= 6);
  incompatible_cs.insert(D <= 0);

  MIP_Problem mip(cs_space_dimension, cs, cost, MAXIMIZATION);
  try {
    // Adds a Constraint_System that exceeds the space dimension of the
    // MIP_Problem.
    mip.add_constraints(incompatible_cs);
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
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(B <= 0);
  dimension_type cs_space_dimension = cs.space_dimension();

  Linear_Expression cost(A + B);

  Constraint_System incompatible_cs;
  incompatible_cs.insert(A >= 10);
  incompatible_cs.insert(B < 22 );

  MIP_Problem mip(cs_space_dimension, cs, cost, MAXIMIZATION);
  try {
    // This tries to add Constraint_System that contains a strict inequality.
    mip.add_constraints(incompatible_cs);
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
test16() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(B <= 0);

  Linear_Expression cost(A + B);

  try {
    // This tries to overflow the maximum space dimension.
    MIP_Problem mip(MIP_Problem::max_space_dimension() + 1,
		    cs.begin(), cs.end(),
		    A + B, MAXIMIZATION);
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
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(B <= 0);

  Linear_Expression cost(A + B);

  try {
    // This tries to let exceed the objective function space dimension.
    MIP_Problem mip(cs.space_dimension(),
		    cs.begin(), cs.end(),
		    A + B + C, MAXIMIZATION);
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
test18() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(B < 0);

  Linear_Expression cost(A + B);

  try {
    // This tries to build an MIP_Problem with strict inequalities..
    MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(),
		    A + B, MAXIMIZATION);
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
test19() {
  Variable A(0);
  Variable B(1);

  Constraint_System cs;
  cs.insert(A >= 6);
  cs.insert(B <= 0);

  Linear_Expression cost(A + B);

  try {
    // This tries to build an MIP_Problem with a wrong space dimension.
    MIP_Problem mip(cs.space_dimension() - 1, cs.begin(), cs.end(),
		    A + B, MAXIMIZATION);
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
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
  DO_TEST(test18);
  DO_TEST(test19);
END_MAIN
