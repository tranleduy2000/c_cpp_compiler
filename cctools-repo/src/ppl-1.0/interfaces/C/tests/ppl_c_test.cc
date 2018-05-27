/* Implementation of utility functions used in test programs
   of the C interface.
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

#include "ppl-config.h"

// The following inclusion of <cstdio> prior to "ppl_c_test.h" is to
// work around a bug whereby some versions of <stdio.h> do not obey
// clause D.5.2 of the C++ standard: "Each C header, whose name has
// the form name.h, behaves as if each name placed in the Standard
// library namespace by the corresponding cname header is also placed
// within the namespace scope of the namespace std and is followed by
// an explicit using-declaration (7.3.3)."  By including <cstdio>
// prior to "ppl_c_test.h" we ensure std::FILE is defined at the time
// when "ppl_c.h" will include <gmp.h>.
#include <cstdio>

#include "ppl_c_test.h"
#include <cstdlib>

namespace {

static int
check_noisy_variable(const char* environment_variable) {
#if PPL_HAVE_DECL_GETENV
  return getenv(environment_variable) != 0;
#else
#if NOISY
  if (strcmp(environment_variable, "PPL_NOISY_TESTS") == 0)
    return 1;
#endif
#if VERY_NOISY
  if (strcmp(environment_variable, "PPL_VERY_NOISY_TESTS") == 0)
    return 1;
#endif
  return 0;
#endif
}

} // namespace

int
check_noisy() {
  return check_noisy_variable("PPL_NOISY_TESTS");
}

int
check_very_noisy() {
  return check_noisy_variable("PPL_VERY_NOISY_TESTS");
}

