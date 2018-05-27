/* Coefficient types of weakly-relational domains: declarations.
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

#ifndef PPL_WRD_coefficient_types_defs_hh
#define PPL_WRD_coefficient_types_defs_hh 1

#include "meta_programming.hh"

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief \ingroup PPL_CXX_interface
  The production policy for checked numbers used in weakly-relational
  domains.
 */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct WRD_Extended_Number_Policy {
  //! Check for overflowed result.
  const_bool_nodef(check_overflow, true);

  //! Do not check for attempts to add infinities with different sign.
  const_bool_nodef(check_inf_add_inf, false);

  //! Do not check for attempts to subtract infinities with same sign.
  const_bool_nodef(check_inf_sub_inf, false);

  //! Do not check for attempts to multiply infinities by zero.
  const_bool_nodef(check_inf_mul_zero, false);

  //! Do not check for attempts to divide by zero.
  const_bool_nodef(check_div_zero, false);

  //! Do not check for attempts to divide infinities.
  const_bool_nodef(check_inf_div_inf, false);

  //! Do not check for attempts to compute remainder of infinities.
  const_bool_nodef(check_inf_mod, false);

  //! Do not checks for attempts to take the square root of a negative number.
  const_bool_nodef(check_sqrt_neg, false);

  //! Handle not-a-number special value.
  const_bool_nodef(has_nan, true);

  //! Handle infinity special values.
  const_bool_nodef(has_infinity, true);

  // `convertible' is intentionally not defined: the compile time
  // error on conversions is the expected behavior.

  //! Honor requests to check for FPU inexact results.
  const_bool_nodef(fpu_check_inexact, true);

  //! Do not make extra checks to detect FPU NaN results.
  const_bool_nodef(fpu_check_nan_result, false);

  // ROUND_DEFAULT_CONSTRUCTOR is intentionally not defined.
  // ROUND_DEFAULT_OPERATOR is intentionally not defined.
  // ROUND_DEFAULT_FUNCTION is intentionally not defined.
  // ROUND_DEFAULT_INPUT is intentionally not defined.
  // ROUND_DEFAULT_OUTPUT is intentionally not defined.

  /*! \brief
    Handles \p r: called by all constructors, operators and functions that
    do not return a Result value.
  */
  static void handle_result(Result r);
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief \ingroup PPL_CXX_interface
  The debugging policy for checked numbers used in weakly-relational
  domains.
 */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct Debug_WRD_Extended_Number_Policy {
  //! Check for overflowed result.
  const_bool_nodef(check_overflow, true);

  //! Check for attempts to add infinities with different sign.
  const_bool_nodef(check_inf_add_inf, true);

  //! Check for attempts to subtract infinities with same sign.
  const_bool_nodef(check_inf_sub_inf, true);

  //! Check for attempts to multiply infinities by zero.
  const_bool_nodef(check_inf_mul_zero, true);

  //! Check for attempts to divide by zero.
  const_bool_nodef(check_div_zero, true);

  //! Check for attempts to divide infinities.
  const_bool_nodef(check_inf_div_inf, true);

  //! Check for attempts to compute remainder of infinities.
  const_bool_nodef(check_inf_mod, true);

  //! Checks for attempts to take the square root of a negative number.
  const_bool_nodef(check_sqrt_neg, true);

  //! Handle not-a-number special value.
  const_bool_nodef(has_nan, true);

  //! Handle infinity special values.
  const_bool_nodef(has_infinity, true);

  // `convertible' is intentionally not defined: the compile time
  // error on conversions is the expected behavior.

  //! Honor requests to check for FPU inexact results.
  const_bool_nodef(fpu_check_inexact, true);

  //! Make extra checks to detect FPU NaN results.
  const_bool_nodef(fpu_check_nan_result, true);

  // ROUND_DEFAULT_CONSTRUCTOR is intentionally not defined.
  // ROUND_DEFAULT_OPERATOR is intentionally not defined.
  // ROUND_DEFAULT_FUNCTION is intentionally not defined.
  // ROUND_DEFAULT_INPUT is intentionally not defined.
  // ROUND_DEFAULT_OUTPUT is intentionally not defined.

  /*! \brief
    Handles \p r: called by all constructors, operators and functions that
    do not return a Result value.
  */
  static void handle_result(Result r);
};

} // namespace Parma_Polyhedra_Library

#include "WRD_coefficient_types.inlines.hh"

#endif // !defined(PPL_WRD_coefficient_types_defs_hh)
