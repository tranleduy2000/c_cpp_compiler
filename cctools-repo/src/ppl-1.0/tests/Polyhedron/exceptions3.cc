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
  try {
    // This is an invalid use of the constructor of a Variable:
    // it is illegal to (try to) build a variable with an id()
    // greater than or equal to Variable::max_space_dimension().
    Variable v(Variable::max_space_dimension());

    // This is only to avoid a compiler warning.
    (void) v.id();

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

bool
test02() {
  try {
    Variable v(Linear_Expression::max_space_dimension());
    // This is an invalid use of the constructor of a Linear_Expression:
    // it is illegal to (try to) build a linear expression with a dimensions
    // greater than Linear_Expression::max_space_dimension().
    Linear_Expression e(v);

   // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

bool
test03() {
  try {
    Variable v(Linear_Expression::max_space_dimension());
    // This is an invalid use of the constructor of a Linear_Expression:
    // it is illegal to (try to) build a linear expression with a dimensions
    // greater than Linear_Expression::max_space_dimension().
    Linear_Expression e;
    e += v;

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
   }
  return false;
}

bool
test04() {
  try {
    Variable v(Linear_Expression::max_space_dimension());
    // This is an invalid use of the constructor of a Linear_Expression:
    // it is illegal to (try to) build a linear expression with a dimensions
    // greater than Linear_Expression::max_space_dimension().
    Linear_Expression e;
    e -= v;

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
   }
  return false;
}

bool
test05() {
  try {
    // This is an invalid use of the constructor of a polyhedron:
    // it is illegal to (try to) build a polyhedron with a dimensions
    // greater than C_Polyhedron::max_space_dimension().
    C_Polyhedron ph(C_Polyhedron::max_space_dimension() + 1);

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

bool
test06() {
  try {
    C_Polyhedron ph(1);
    // This is an invalid use of the method for adding dimensions:
    // it is illegal to (try to) embed a polyhedron into a vector space
    // whose dimension is greater than C_Polyhedron::max_space_dimension().
    ph.add_space_dimensions_and_embed(C_Polyhedron::max_space_dimension());

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

bool
test07() {
  try {
    C_Polyhedron ph(1);
    // This is an invalid use of the method for adding dimensions:
    // it is illegal to (try to) project a polyhedron into a vector space
    // whose dimension is greater than C_Polyhedron::max_space_dimension().
    ph.add_space_dimensions_and_project(C_Polyhedron::max_space_dimension());

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

bool
test08() {
  try {
    C_Polyhedron ph(C_Polyhedron::max_space_dimension(), EMPTY);
    // This is an invalid use of the method for concatenating polyhedra:
    // it is illegal to (try to) concatenate polyhedra if the resulting
    // vector space will have a dimension greater than
    // C_Polyhedron::max_space_dimension().
    ph.concatenate_assign(C_Polyhedron(1));

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
  }
  return false;
}

bool
test09() {
  try {
    C_Polyhedron ph(1);
    // This is an invalid use of the method for expanding space dimensions:
    // it is illegal to (try to) expand a space dimension if the resulting
    // vector space will have a dimension greater than
    // C_Polyhedron::max_space_dimension().
    ph.expand_space_dimension(Variable(0),
			      C_Polyhedron::max_space_dimension());

    // It is an error if the exception is not thrown.
  }
  catch (std::length_error& e) {
    nout << "length_error: " << e.what() << endl << endl;
    return true;
  }
  catch (...) {
    // It is an error if the wrong exception is thrown.
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
END_MAIN
