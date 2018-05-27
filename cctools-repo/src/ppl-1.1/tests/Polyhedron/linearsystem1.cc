/* Test some functionality of class Linear_System.
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
#include "files.hh"
#include <fstream>

namespace {

bool
test01() {
  const char* data_file = "linearsystem1.dat";

  Variable A(0);
  Variable B(1);

#if 0
  // Using this seed and the checked 8-bit integer coefficients,
  // the random number generator produces the minimum value during
  // the following computations.
  unsigned long problematic_seed = 1141853716;
  Random_Number_Generator rng(problematic_seed);
#else
  Random_Number_Generator rng;
#endif

#define ROWS 7
#define COLS 3

  Linear_System<Constraint> ls1(NOT_NECESSARILY_CLOSED, DENSE);
  for (dimension_type rowi = 0; rowi < ROWS; ++rowi) {
    Linear_Expression e;
    e.set_space_dimension(COLS - 1);
    for (dimension_type col = 0; col < COLS; ++col) {
      Coefficient c;
      rng.get(c, 0);
      // The following workaround is to avoid trivial positive overflows
      // when using bounded coefficients.
      if (std::numeric_limits<Coefficient>::is_bounded
          && c == std::numeric_limits<Coefficient>::min())
        // Here the randomly generated coefficients is equal to the
        // allowed minimum value for a signed integer data type that
        // might adopt the 2's complement representation
        // (e.g., -128 for 8 bit signed integers).
        // Thus, it would cause a positive overflow during the normalization
        // of the Linear_Row, because the GCD computation will try to negate
        // such a coefficient.
        // To avoid the problem, we simply increment the coefficient.
        ++c;
      
      if (col == 0)
        e += c;
      else
        add_mul_assign(e, c, Variable(col - 1));
    }

    ls1.insert(e > 0);

    using std::fstream;
    using std::ios_base;
    fstream f;
    open(f, data_file, ios_base::out);
    ls1.ascii_dump(f);
    close(f);

    open(f, data_file, ios_base::in);
    Linear_System<Constraint> ls2(NECESSARILY_CLOSED, DENSE);
    ls2.ascii_load(f);
    close(f);

    if (ls1 == ls2)
      continue;

    nout << "Linear_System<Constraint>::ascii_dump/load test failed." << endl
         << "m1.ascii_dump() gives" << endl;
    ls1.ascii_dump(nout);
    nout << "m2.ascii_dump() gives" << endl;
    ls2.ascii_dump(nout);

    return false;
  }
  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
