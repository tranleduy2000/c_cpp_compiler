/* Coefficient class implementation (non-inline functions).
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
#include "Coefficient.defs.hh"

namespace Parma_Polyhedra_Library {

#if defined(PPL_CHECKED_INTEGERS) || defined(PPL_NATIVE_INTEGERS)
void
Coefficient_constants_initialize() {
}

void
Coefficient_constants_finalize() {
}
#endif

#ifdef PPL_GMP_INTEGERS
const Coefficient* Coefficient_zero_p = 0;
const Coefficient* Coefficient_one_p = 0;

void
Coefficient_constants_initialize() {
  PPL_ASSERT(Coefficient_zero_p == 0);
  Coefficient_zero_p = new Coefficient(0);

  PPL_ASSERT(Coefficient_one_p == 0);
  Coefficient_one_p = new Coefficient(1);
}

void
Coefficient_constants_finalize() {
  PPL_ASSERT(Coefficient_zero_p != 0);
  delete Coefficient_zero_p;
  Coefficient_zero_p = 0;

  PPL_ASSERT(Coefficient_one_p != 0);
  delete Coefficient_one_p;
  Coefficient_one_p = 0;
}
#endif

} // namespace Parma_Polyhedra_Library
