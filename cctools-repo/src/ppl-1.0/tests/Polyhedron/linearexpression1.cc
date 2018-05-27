/* Testing Linear_Expression.
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

using namespace Parma_Polyhedra_Library::IO_Operators;

#define EQUIVALENT(le1, le2) (((le1) == 0) == ((le2) == 0))

// Test operator-=(Linear_Expression& e1, const Linear_Expression& e2):
// in this case the dimension of e2 is strictly greater than
// the dimension of e1.
bool
test01() {
  Variable A(0);
  Variable B(1);

  Linear_Expression e1 = A;
  Linear_Expression e2 = B;
  e1 -= e2;

  Linear_Expression known_result = A - B;

  bool ok = EQUIVALENT(e1, known_result);

  nout << "*** known_result ***" << endl
       << known_result << endl;

  return ok;
}

bool
test02() {
  Variable A(15);
  Variable B(0);

  Linear_Expression e1 = A;
  Linear_Expression e2 = B;

  Linear_Expression known_result1 = e1 + e2;

  bool ok1 = EQUIVALENT(A + B, known_result1)
    && EQUIVALENT(B + A, known_result1)
    && EQUIVALENT(Linear_Expression(A) + B, known_result1)
    && EQUIVALENT(B + Linear_Expression(A), known_result1)
    && EQUIVALENT(A + Linear_Expression(B), known_result1)
    && EQUIVALENT(Linear_Expression(B) + A, known_result1)
    && EQUIVALENT(Linear_Expression(B) + Linear_Expression(A), known_result1);

  nout << "*** known_result1 ***" << endl
       << known_result1 << endl;

  Linear_Expression known_result2 = e1 + e1;

  bool ok2 = EQUIVALENT(A + A, known_result2)
    && EQUIVALENT(A + A, known_result2)
    && EQUIVALENT(Linear_Expression(A) + A, known_result2)
    && EQUIVALENT(A + Linear_Expression(A), known_result2)
    && EQUIVALENT(A + Linear_Expression(A), known_result2)
    && EQUIVALENT(Linear_Expression(A) + A, known_result2)
    && EQUIVALENT(Linear_Expression(A) + Linear_Expression(A), known_result2);

  nout << "*** known_result2 ***" << endl
       << known_result2 << endl;

  return ok1 && ok2;
}

bool
test03() {
  Variable A(15);
  Variable B(10);

  Linear_Expression e1 = A;
  Linear_Expression e2 = B;

  Linear_Expression known_result1 = e1 - e2;

  bool ok1 = EQUIVALENT(A - B, known_result1)
    && EQUIVALENT(Linear_Expression(A) - B, known_result1)
    && EQUIVALENT(A - Linear_Expression(B), known_result1)
    && EQUIVALENT(Linear_Expression(A) - Linear_Expression(B), known_result1);

  nout << "*** known_result1 ***" << endl
       << known_result1 << endl;

  Linear_Expression known_result2 = e2 - e1;

  bool ok2 = EQUIVALENT(B - A, known_result2)
    && EQUIVALENT(Linear_Expression(B) - A, known_result2)
    && EQUIVALENT(B - Linear_Expression(A), known_result2)
    && EQUIVALENT(Linear_Expression(B) - Linear_Expression(A), known_result2);

  nout << "*** known_result2 ***" << endl
       << known_result2 << endl;

  Linear_Expression known_result3 = e1 - e1;

  bool ok3 = EQUIVALENT(A - A, known_result3)
    && EQUIVALENT(Linear_Expression(A) - A, known_result3)
    && EQUIVALENT(A - Linear_Expression(A), known_result3)
    && EQUIVALENT(Linear_Expression(A) - Linear_Expression(A), known_result3);

  nout << "*** known_result3 ***" << endl
       << known_result3 << endl;

  return ok1 && ok2 && ok3;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
END_MAIN
