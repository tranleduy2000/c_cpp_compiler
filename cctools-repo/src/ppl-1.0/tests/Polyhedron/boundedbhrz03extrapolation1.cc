/* Test Polyhedron::bounded_BHRZ03_extrapolation_assign().
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

#define PROPAGATION_STEPS 1
#define NUMBER_OF_TOKENS 1

namespace {

void
aux_test01(std::ostream& s, const Variable v) {
  s << char('i' + v.id());
}

bool
test01() {
  Variable::output_function_type* old_output_function
    = Variable::get_output_function();

  try {
    Variable::set_output_function(aux_test01);
    Variable i(0);
    Variable j(1);
    Variable k(2);

    C_Polyhedron ph(3);
    ph.add_constraint(i == 1);
    ph.add_constraint(j == 1);
    ph.add_constraint(0 <= k);
    ph.add_constraint(k <= 1);

    C_Polyhedron old_ph(3, EMPTY);

    // Propagation.
    for (int steps = 0; steps < PROPAGATION_STEPS; ++steps) {
      old_ph = ph;

      ph.affine_image(i, i+1);
      ph.affine_image(j, j+k);
      ph.affine_image(k, k-1);

      old_ph.upper_bound_assign(ph);

      ph = old_ph;
    }

    // Widening.
    Constraint_System up_to_constraints = ph.constraints();

    print_constraints(up_to_constraints, "*** up_to_constraints ***");

    unsigned tokens = NUMBER_OF_TOKENS;

    for (int step = 1; ; ++step) {

      nout << "\nAt step " << step << endl;
      print_constraints(ph);

      old_ph = ph;

      ph.affine_image(i, i+1);
      ph.affine_image(j, j+k);
      ph.affine_image(k, k-1);

      ph.upper_bound_assign(old_ph);
      if (old_ph.contains(ph))
        break;

      // Notice that neither the H79 nor the BHRZ03 limited
      // extrapolations (used as widenings here) allow to obtain the
      // desired postfixpoint for this example.  Both the H79 and the
      // BHRZ03 bounded extrapolation operators do achieve this result.
      ph.bounded_H79_extrapolation_assign(old_ph, up_to_constraints, &tokens);
    }

    C_Polyhedron known_result(3);
    known_result.add_constraint(-i - k >= -2);
    known_result.add_constraint(i - j + k >= 0);
    known_result.add_constraint(i - j + 2*k >= -1);
    known_result.add_constraint(i + k >= 1);
    known_result.add_constraint(i >= 1);
    known_result.add_constraint(i - j >= 0);

    bool ok = (ph == known_result);

    print_constraints(ph, "\nPostfixpoint");

    Variable::set_output_function(old_output_function);
    return ok;
  }
  catch (...) {
    Variable::set_output_function(old_output_function);
    throw;
  }
}

bool test02() {
  Variable x(0);

  C_Polyhedron P1(1);
  P1.add_constraint(1 <= x);
  P1.add_constraint(x <= 2);

  C_Polyhedron P2(1);
  P2.add_constraint(0 <= x);
  P2.add_constraint(x <= 3);

  Constraint_System cs;

  print_constraints(P1, "*** P1 ***");
  print_constraints(P2, "*** P2 ***");
  print_constraints(cs, "*** cs ***");

  P2.bounded_BHRZ03_extrapolation_assign(P1, cs);

  C_Polyhedron known_result(1);
  known_result.add_constraint(0 <= x);

  bool ok = (P2 == known_result);

  print_constraints(P2,
                    "*** P2.bounded_BHRZ03_extrapolation_assign(P1, cs) ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8A(test01);
  DO_TEST(test02);
END_MAIN
