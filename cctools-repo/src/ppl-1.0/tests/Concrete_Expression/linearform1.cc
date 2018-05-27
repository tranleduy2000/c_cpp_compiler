/* Testing Linear_Form on floating point intervals.
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
#include <limits>

namespace {

using namespace Parma_Polyhedra_Library::IO_Operators;

// Test operator-=(FP_Linear_Form& f1, const FP_Linear_Form& f2):
// in this case the dimension of f2 is strictly greater than
// the dimension of f1.
bool
test01() {
  Variable A(0);
  Variable B(1);

  FP_Linear_Form f;
  FP_Linear_Form f1 = A;
  FP_Linear_Form f2 = B;

  f1 += f.coefficient(A);
  f1 -= f2;

  FP_Linear_Form known_result = A - FP_Linear_Form(B);

  bool ok = (f1 == known_result);

  nout << "*** known_result ***" << endl
       << known_result << endl;

  return ok;
}

bool
test02() {
  Variable A(15);
  Variable B(0);

  FP_Linear_Form f1 = A;
  FP_Linear_Form f2 = B;

  FP_Linear_Form known_result1 = f1 + f2;

  bool ok1 = (FP_Linear_Form(A) + B == known_result1)
    && (B + FP_Linear_Form(A) == known_result1)
    && (A + FP_Linear_Form(B) == known_result1)
    && (FP_Linear_Form(B) + A == known_result1)
    && (FP_Linear_Form(B) + FP_Linear_Form(A) == known_result1);

  nout << "*** known_result1 ***" << endl
       << known_result1 << endl;

  FP_Linear_Form known_result2 = f1 + f1;

  bool ok2 = (FP_Linear_Form(A) + A == known_result2)
    && (A + FP_Linear_Form(A) == known_result2)
    && (A + FP_Linear_Form(A) == known_result2)
    && (FP_Linear_Form(A) + A == known_result2)
    && (FP_Linear_Form(A) + FP_Linear_Form(A) == known_result2);

  nout << "*** known_result2 ***" << endl
       << known_result2 << endl;

  return ok1 && ok2;
}

bool
test03() {
  Variable A(15);
  Variable B(10);

  FP_Linear_Form f1 = A;
  FP_Linear_Form f2 = B;

  FP_Linear_Form known_result1 = f1 - f2;

  bool ok1 = (FP_Linear_Form(A) - B == known_result1)
    && (A - FP_Linear_Form(B) == known_result1)
    && (FP_Linear_Form(A) - FP_Linear_Form(B) == known_result1);

  nout << "*** known_result1 ***" << endl
       << known_result1 << endl;

  FP_Linear_Form known_result2 = f2 - f1;

  bool ok2 = (FP_Linear_Form(B) - A == known_result2)
    && (B - FP_Linear_Form(A) == known_result2)
    && (FP_Linear_Form(B) - FP_Linear_Form(A) == known_result2);

  nout << "*** known_result2 ***" << endl
       << known_result2 << endl;

  FP_Linear_Form known_result3 = f1 - f1;

  bool ok3 = (FP_Linear_Form(A) - A == known_result3)
    && (A - FP_Linear_Form(A) == known_result3)
    && (FP_Linear_Form(A) - FP_Linear_Form(A) == known_result3);

  nout << "*** known_result3 ***" << endl
       << known_result3 << endl;

  return ok1 && ok2 && ok3;
}

// Test operator+=(FP_Linear_Form& f1, const FP_Linear_Form& f2):
// in this case the dimension of f2 is strictly greater than
// the dimension of f1.
bool
test04() {
  Variable A(0);
  Variable B(1);

  FP_Linear_Form f1 = A;
  FP_Linear_Form f2 = B;
  f1 += f2;

  FP_Linear_Form known_result = FP_Linear_Form(A) + B;

  bool ok = (f1 == known_result);

  nout << "*** known_result ***" << endl
       << known_result << endl;

  return ok;
}

// Test operator+=(FP_Linear_Form& f, Variable v):
// in this case the dimension of v is strictly greater than
// the dimension of f.
bool
test05() {
  Variable A(0);
  Variable B(1);

  FP_Linear_Form f = A;
  FP_Interval x(FP_Interval::boundary_type(2));
  x /= FP_Interval(FP_Interval::boundary_type(3));
  f *= x;

  FP_Linear_Form known_result = f + B;

  f += B;

  bool ok = (f == known_result);

  nout << "*** known_result ***" << endl
       << known_result << endl;

  return ok;
}

bool
test06() {
  Variable A(0);
  Variable B(1);
  Variable C(16);
  Variable D(120);

  FP_Linear_Form f = A + 2*B + 16*C + 120*D;

  FP_Linear_Form known_result = A;
  known_result += FP_Interval(2) * FP_Linear_Form(B);
  known_result += FP_Interval(16) * FP_Linear_Form(C);
  known_result += FP_Interval(120) * FP_Linear_Form(D);

  bool ok = (f == known_result);

  nout << "*** known_result ***" << endl
       << known_result << endl;

  return ok;
}

// Tests operator==(const Linear_Form<C>& x, const Linear_Form<C>& y).
bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  FP_Linear_Form f1 = A;
  FP_Linear_Form f2;
  bool known_result = false;

  bool result1 = (f1 == f2);

  f2 += FP_Interval(FP_Interval::boundary_type(1));
  bool result2 = (f1 == f2);

  bool result3 = (f2 == f1);

  f1 += FP_Interval(FP_Interval::boundary_type(1));
  bool result4 = (f2 == f1);

  nout << "*** known_result ***" << endl
       << known_result << endl;

  return (known_result == (result1 || result2 || result3 || result4));
}


// Tests overflows of space dimension.
bool
test08() {
  FP_Linear_Form f;
  Variable A(f.max_space_dimension());

  bool ok1 = false;
  try {
    f = FP_Linear_Form(A);
  }
  catch(std::length_error e) {
      nout << "Overflow in Linear_Form(const Variable v)." << endl;
      ok1 = true;
  }

  bool ok2 = false;
  try {
    f += A;
  }
  catch(std::length_error e) {
      nout << "Overflow in operator+=(Linear_Form<C>& f, const Variable v)."
           << endl;
      ok2 = true;
  }

  bool ok3 = false;
  try {
    f -= A;
  }
  catch(std::length_error e) {
      nout << "Overflow in operator-=(Linear_Form<C>& f, const Variable v)."
           << endl;
      Variable B(1);
      Variable C(2);
      FP_Linear_Form g(B);
      g -= C;
      ok3 = true;
  }

  FP_Linear_Form g;
  bool ok4 = false;
  try {
    g = f - A;
  }
  catch(std::length_error e) {
      nout << "Overflow in operator-(Linear_Form<C>& f, const Variable v)."
           << endl;
      ok4 = true;
  }

  bool ok5 = false;
  try {
    g = A - f;
  }
  catch(std::length_error e) {
      nout << "Overflow in operator-(const Variable v, Linear_Form<C>& f)."
           << endl;
      ok5 = true;
  }

  bool ok6 = false;
  try {
    g = A + f;
  }
  catch(std::length_error e) {
      nout << "Overflow in operator+(const Variable v, Linear_Form<C>& f)."
           << endl;
      ok6 = true;
  }

  return ok1 && ok2 && ok3 && ok4 && ok5 && ok6;
}

bool
test09() {
  Variable A(0);
  FP_Linear_Form f;
  bool ok1 = !f.overflows();
  f += A;
  bool ok2 = !f.overflows();
  FP_Interval max(std::numeric_limits<ANALYZER_FP_FORMAT>::max());
  f *= max;
  f *= max;
  bool ok3 = f.overflows();
  return ok1 && ok2 && ok3;
}

bool
test10() {
  Variable x5(5);
  Variable x2(2);
  FP_Interval x5_coefficient;
  x5_coefficient.lower() = FP_Interval::boundary_type(2);
  x5_coefficient.upper() = FP_Interval::boundary_type(3);
  FP_Interval inhomogeneous_term;
  inhomogeneous_term.lower() = FP_Interval::boundary_type(4);
  inhomogeneous_term.upper() = FP_Interval::boundary_type(8);
  FP_Linear_Form lf(x2);
  lf = -lf;
  lf += FP_Linear_Form(x2);
  FP_Linear_Form lx5(x5);
  lx5 *= x5_coefficient;
  lf += lx5;
  return lf.space_dimension() == 6;
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
