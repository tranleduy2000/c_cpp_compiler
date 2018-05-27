/* Some simple tests for the basic functionality of PPL::Watchdog.
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

volatile bool interrupt = false;

void
do_interrupt() {
  interrupt = true;
}

bool
test01() {
  const int csecs = 10;
  try {
    bool ok;
    {
      Watchdog w(csecs, do_interrupt);
      nout << ((float) csecs)/100.0 << " seconds watchdog" << endl;

      nout << "starting iteration... " << std::flush;
      for (unsigned long i = 0; i < 1000000000; ++i) {
        if (interrupt) {
          nout << "interrupted" << endl;
          ok = true;
          goto done;
        }
      }
      nout << "not interrupted" << endl;
      ok = false;
    done:
      ;
    }
    interrupt = false;
    return ok;
  }
#if !PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED
  catch (const std::logic_error& e) {
    nout << "std::logic_error caught (" << e.what() << ")" << endl;
    return true;
  }
#endif // !PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED
  catch (...) {
    return false;
  }
  // Should never get here.
  return false;
}


} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
