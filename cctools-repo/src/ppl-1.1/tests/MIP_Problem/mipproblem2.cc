/* Test the MIP_Problem class with instances that require a watchdog timer.
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
#include <limits>

namespace {

class Timeout : virtual public std::exception,
                public Parma_Polyhedra_Library::Throwable {
public:
  const char* what() const throw() {
    return "Timeout in refine1.cc";
  }

  void throw_me() const {
    throw *this;
  }

  int priority() const {
    return 0;
  }

  Timeout() {
  }

  ~Timeout() throw() {
  }
};

Timeout t;

bool test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Linear_Expression cost(10*A + 3*B);
  Constraint_System cs;
  cs.insert(A + B >= 0);
  cs.insert(B >= 0);
  cs.insert(B == 3);
  cs.insert(2*C + 2*D == 9);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MINIMIZATION);
  Coefficient num_kr = -21;
  Coefficient den_kr = 1;
  Coefficient num;
  Coefficient den;
  Generator pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  Generator pg_kr = point(-6*A + 6*B + 9*D, 2);
  if (pg != pg_kr)
    return false;

  // Set Variable A to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(A));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set Variable B to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(B));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set Variable C to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(C));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set Variable D to be constrained to have an integer value.
  // This will cause branch-and-bound not to terminate any longer.
  mip.add_to_integer_space_dimensions(Variables_Set(D));

  try {
    // Set a 2 seconds timeout.
    Parma_Polyhedra_Library::Watchdog
      w(200, abandon_expensive_computations, t);

    pg = mip.optimizing_point();

    // We should never get here.
    abandon_expensive_computations = 0;
    nout << "unexpected termination" << endl;
    return false;
  }
  catch (const Timeout&) {
    abandon_expensive_computations = 0;
    nout << "timeout, as expected" << endl;
    return true;
  }
#if !PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED
  // If Watchdog objects are not supported, an std::logic_error exception
  // will be thrown: this is normal.
  catch (const std::logic_error& e) {
    nout << "std::logic_error exception caught: \n" << e.what() << std::endl;
    exit(0);
  }
#endif // !PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED
  catch (const std::overflow_error& e) {
    abandon_expensive_computations = 0;
    if (std::numeric_limits<Coefficient>::is_integer
        && std::numeric_limits<Coefficient>::is_bounded
        && std::numeric_limits<Coefficient>::radix == 2
        && std::numeric_limits<Coefficient>::digits == 7) {
      // Overflow is OK with 8-bit coefficients.
      nout << "arithmetic overflow (" << e.what() << "),"
        " possible with 8-bit coefficients" << endl;
      return true;
    }
    else
      // Overflow errors should be propagated in all other cases.
      throw;
  }
  catch (...) {
    abandon_expensive_computations = 0;
    nout << "unexpected exception" << endl;
    return false;
  }
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
