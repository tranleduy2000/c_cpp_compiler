/* Init class declaration.
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

#ifndef PPL_Init_defs_hh
#define PPL_Init_defs_hh 1

#include "Init_types.hh"
#include "fpu_types.hh"

namespace Parma_Polyhedra_Library {

/*! \brief
  Sets the FPU rounding mode so that the PPL abstractions based on
  floating point numbers work correctly.

  This is performed automatically at initialization-time.  Calling
  this function is needed only if restore_pre_PPL_rounding() has been
  previously called.
*/
void set_rounding_for_PPL();

/*! \brief
  Sets the FPU rounding mode as it was before initialization of the PPL.

  This is important if the application uses floating-point computations
  outside the PPL.  It is crucial when the application uses functions
  from a mathematical library that are not guaranteed to work correctly
  under all rounding modes.

  After calling this function it is absolutely necessary to call
  set_rounding_for_PPL() before using any PPL abstractions based on
  floating point numbers.
  This is performed automatically at finalization-time.
*/
void restore_pre_PPL_rounding();

} // namespace Parma_Polyhedra_Library

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Class for initialization and finalization.
/*! \ingroup PPL_CXX_interface
  <EM>Nifty Counter</EM> initialization class,
  ensuring that the library is initialized only once
  and before its first use.
  A count of the number of translation units using the library
  is maintained. A static object of Init type will be declared
  by each translation unit using the library.  As a result,
  only one of them will initialize and properly finalize
  the library.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
class Parma_Polyhedra_Library::Init {
public:
  //! Initializes the PPL.
  Init();

  //! Finalizes the PPL.
  ~Init();

private:
  /*! \brief
    Default precision parameter used for irrational calculations.

    The default is chosen to have a precision greater than most
    precise IEC 559 floating point (112 bits of mantissa).
  */
  static const unsigned DEFAULT_IRRATIONAL_PRECISION = 128U;

  //! Count the number of objects created.
  static unsigned int count;
  static fpu_rounding_direction_type old_rounding_direction;

  friend void set_rounding_for_PPL();
  friend void restore_pre_PPL_rounding();
};

#include "Init_inlines.hh"


#endif // !defined(PPL_Init_defs_hh)
