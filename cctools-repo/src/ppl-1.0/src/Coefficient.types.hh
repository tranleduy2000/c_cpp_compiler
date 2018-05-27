/* Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

This file is free software; as a special exception the author gives
unlimited permission to copy and/or distribute it, with or without
modifications, as long as this notice is preserved.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. */

#ifndef PPL_Coefficient_types_hh
#define PPL_Coefficient_types_hh 1

#include "Checked_Number.defs.hh"

#ifdef PPL_GMP_INTEGERS
#include "GMP_Integer.types.hh"
#endif

#if defined(PPL_CHECKED_INTEGERS) || defined(PPL_NATIVE_INTEGERS)

namespace Parma_Polyhedra_Library {

//! A policy for checked bounded integer coefficients.
/*! \ingroup PPL_CXX_interface */
struct Bounded_Integer_Coefficient_Policy {
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

  //! Do not handle not-a-number special value.
  const_bool_nodef(has_nan, false);

  //! Do not handle infinity special values.
  const_bool_nodef(has_infinity, false);

  /*! \brief
    The checked number can always be safely converted to the
    underlying type \p T and vice-versa.
  */
  const_bool_nodef(convertible, true);

  //! Do not honor requests to check for FPU inexact results.
  const_bool_nodef(fpu_check_inexact, false);

  //! Do not make extra checks to detect FPU NaN results.
  const_bool_nodef(fpu_check_nan_result, true);

  /*! \brief
    For constructors, by default use the same rounding used by
    underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_CONSTRUCTOR = ROUND_NATIVE;

  /*! \brief
    For overloaded operators (operator+(), operator-(), ...), by
    default use the same rounding used by the underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_OPERATOR = ROUND_NATIVE;

  /*! \brief
    For input functions, by default use the same rounding used by
    the underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_INPUT = ROUND_NATIVE;

  /*! \brief
    For output functions, by default use the same rounding used by
    the underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_OUTPUT = ROUND_NATIVE;

  /*! \brief
    For all other functions, by default use the same rounding used by
    the underlying type.
  */
  static const Rounding_Dir ROUND_DEFAULT_FUNCTION = ROUND_NATIVE;

  /*! \brief
    Handles \p r: called by all constructors, operators and functions that
    do not return a Result value.
  */
  static void handle_result(Result r);
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Coefficient traits specialization for 8 bits checked integers.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Policy>
struct Coefficient_traits_template<Checked_Number<int8_t, Policy> > {
  //! The type used for references to const 8 bit checked integers.
  typedef Checked_Number<int8_t, Policy> const_reference;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Coefficient traits specialization for 16 bits checked integers.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Policy>
struct Coefficient_traits_template<Checked_Number<int16_t, Policy> > {
  //! The type used for references to const 16 bit checked integers.
  typedef Checked_Number<int16_t, Policy> const_reference;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Coefficient traits specialization for 32 bits checked integers.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Policy>
struct Coefficient_traits_template<Checked_Number<int32_t, Policy> > {
  //! The type used for references to const 32 bit checked integers.
  typedef Checked_Number<int32_t, Policy> const_reference;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Coefficient traits specialization for 64 bits checked integers.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Policy>
struct Coefficient_traits_template<Checked_Number<int64_t, Policy> > {
  //! The type used for references to const 64 bit checked integers.
  typedef const Checked_Number<int64_t, Policy>& const_reference;
};

} // namespace Parma_Polyhedra_Library

#endif // defined(PPL_CHECKED_INTEGERS) || defined(PPL_NATIVE_INTEGERS)

namespace Parma_Polyhedra_Library {

//! An alias for easily naming the type of PPL coefficients.
/*! \ingroup PPL_CXX_interface
  Objects of type Coefficient are used to implement the integral valued
  coefficients occurring in linear expressions, constraints, generators,
  intervals, bounding boxes and so on.  Depending on the chosen
  configuration options (see file <CODE>README.configure</CODE>),
  a Coefficient may actually be:
    - The GMP_Integer type, which in turn is an alias for the
      <CODE>mpz_class</CODE> type implemented by the C++ interface
      of the GMP library (this is the default configuration).
    - An instance of the Checked_Number class template: with the policy
      Bounded_Integer_Coefficient_Policy, this implements overflow
      detection on top of a native integral type (available template
      instances include checked integers having 8, 16, 32 or 64 bits);
      with the Checked_Number_Transparent_Policy, this is a wrapper
      for native integral types with no overflow detection
      (available template instances are as above).
*/
typedef PPL_COEFFICIENT_TYPE Coefficient;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! An alias for easily naming the coefficient traits.
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
typedef Coefficient_traits_template<Coefficient> Coefficient_traits;

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Coefficient_types_hh)
