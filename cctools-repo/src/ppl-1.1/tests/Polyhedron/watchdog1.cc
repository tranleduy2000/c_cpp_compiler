/* Test the timeout facility of the library.
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
#include "timings.hh"

namespace {

void
compute_open_hypercube_generators(dimension_type dimension) {
  NNC_Polyhedron hypercube(dimension);
  for (dimension_type i = 0; i < dimension; ++i) {
    Variable x(i);
    hypercube.add_constraint(x > 0);
    hypercube.add_constraint(x < 1);
  }
  (void) hypercube.generators();
}

class Timeout : virtual public std::exception,
                public Parma_Polyhedra_Library::Throwable {
public:
  const char* what() const throw() {
    return "timeout in watchdog1.cc";
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

bool
timed_compute_open_hypercube_generators(dimension_type dimension, int csecs) {
  try {
    Parma_Polyhedra_Library::Watchdog
      w(csecs, abandon_expensive_computations, t);

    start_clock();

    compute_open_hypercube_generators(dimension);
    abandon_expensive_computations = 0;
    return true;
  }
  catch (const Timeout& e) {
    abandon_expensive_computations = 0;
    nout << e.what() << " after ";
    print_clock(nout);
    nout << " s" << endl;
    return false;
  }
#if !PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED
  // If Watchdog objects are not supported, an std::logic_error exception
  // will be thrown: this is normal.
  catch (const std::logic_error& e) {
    nout << "std::logic_error exception caught: \n" << e.what() << std::endl;
    exit(0);
  }
#endif // !PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED
  catch (const std::exception& e) {
    nout << "unexpected std::exception caught: \n" << e.what() << endl;
    exit(1);
  }
  catch (...) {
    nout << "unexpected unknown exception caught" << endl;
    exit(1);
  }
  // Should never get here.
  return false;
}

} // namespace

#define INIT_TIME 20

int
main() TRY {
  set_handlers();

  // Find a dimension that cannot be computed with a INIT_TIME timeout.
  dimension_type dimension = 0;
  do {
    ++dimension;
    nout << "Trying dimension " << dimension << endl;
  } while (timed_compute_open_hypercube_generators(dimension, INIT_TIME));

  // Now find an upper bound to the time necessary to compute it.
  int upper_bound = INIT_TIME;
  do {
    upper_bound *= 2;
    nout << "Trying upper bound " << upper_bound << endl;
  } while (!timed_compute_open_hypercube_generators(dimension, upper_bound));

  // Search the "exact" time.
  int lower_bound = upper_bound/2;
  do {
    int test_time = (lower_bound+upper_bound)/2;
    nout << "Probing " << test_time << endl;
    if (timed_compute_open_hypercube_generators(dimension, test_time))
      upper_bound = test_time;
    else
      lower_bound = test_time;
  } while (upper_bound-lower_bound > 4);

  nout << "Estimated time for dimension " << dimension
       << ": " << (lower_bound+upper_bound)/2 << " 100th of sec" << endl;

  return 0;
}
CATCH
