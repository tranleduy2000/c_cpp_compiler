/* Coefficient class declaration.
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

#ifndef PPL_Coefficient_defs_hh
#define PPL_Coefficient_defs_hh 1

#include "Coefficient.types.hh"
#include <iosfwd>

#if defined(PPL_CHECKED_INTEGERS) || defined(PPL_NATIVE_INTEGERS)
#include "Checked_Number.defs.hh"
#include "checked_int.inlines.hh"
#endif

#ifdef PPL_GMP_INTEGERS
#include "GMP_Integer.defs.hh"
#endif

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Initializes the Coefficient constants.
#endif
void Coefficient_constants_initialize();

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Finalizes the Coefficient constants.
#endif
void Coefficient_constants_finalize();

//! Returns a const reference to a Coefficient with value 0.
Coefficient_traits::const_reference Coefficient_zero();

//! Returns a const reference to a Coefficient with value 1.
Coefficient_traits::const_reference Coefficient_one();

} // namespace Parma_Polyhedra_Library

#include "Coefficient.inlines.hh"

#endif // !defined(PPL_Coefficient_defs_hh)
