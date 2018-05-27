/* Declaration of Rounding_Dir and related functions.
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

#ifndef PPL_Rounding_defs_hh
#define PPL_Rounding_defs_hh 1

#include "Result.defs.hh"
#include "fpu.defs.hh"

namespace Parma_Polyhedra_Library {

//! Rounding directions for arithmetic computations.
/*! \ingroup PPL_CXX_interface */
enum Rounding_Dir {
  /*! \hideinitializer
    Round toward \f$-\infty\f$.
  */
  ROUND_DOWN = 0U,

  /*! \hideinitializer
    Round toward \f$+\infty\f$.
  */
  ROUND_UP = 1U,

  /*! \hideinitializer
    Rounding is delegated to lower level. Result info is evaluated lazily.
  */
  ROUND_IGNORE = 6U,
  ROUND_NATIVE = ROUND_IGNORE,

  /*! \hideinitializer
    Rounding is not needed: client code must ensure that the operation
    result is exact and representable in the destination type.
    Result info is evaluated lazily.
  */
  ROUND_NOT_NEEDED = 7U,

  ROUND_DIRECT = ROUND_UP,
  ROUND_INVERSE = ROUND_DOWN,

  ROUND_DIR_MASK = 7U,

  /*! \hideinitializer
    The client code is willing to pay an extra price to know the exact
    relation between the exact result and the computed one.
   */
  ROUND_STRICT_RELATION = 8U,

  ROUND_CHECK = ROUND_DIRECT | ROUND_STRICT_RELATION
};

//! \name Functions Inspecting and/or Combining Rounding_Dir Values
//@{

/*! \ingroup PPL_CXX_interface */
Rounding_Dir operator&(Rounding_Dir x, Rounding_Dir y);

/*! \ingroup PPL_CXX_interface */
Rounding_Dir operator|(Rounding_Dir x, Rounding_Dir y);

/*! \brief \ingroup PPL_CXX_interface
  Returns the inverse rounding mode of \p dir,
  <CODE>ROUND_IGNORE</CODE> being the inverse of itself.
*/
Rounding_Dir inverse(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
Rounding_Dir round_dir(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
bool round_down(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
bool round_up(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
bool round_ignore(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
bool round_not_needed(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
bool round_not_requested(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
bool round_direct(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
bool round_inverse(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
bool round_strict_relation(Rounding_Dir dir);

/*! \ingroup PPL_CXX_interface */
fpu_rounding_direction_type round_fpu_dir(Rounding_Dir dir);

//@} // Functions Inspecting and/or Combining Rounding_Dir Values

} // namespace Parma_Polyhedra_Library

#include "Rounding_Dir.inlines.hh"

#endif // !defined(PPL_Float_defs_hh)

