/* Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

This file is free software; as a special exception the author gives
unlimited permission to copy and/or distribute it, with or without
modifications, as long as this notice is preserved.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. */

#ifndef PPL_globals_types_hh
#define PPL_globals_types_hh 1

#include <cstddef>

namespace Parma_Polyhedra_Library {

//! An unsigned integral type for representing space dimensions.
/*! \ingroup PPL_CXX_interface */
typedef size_t dimension_type;

//! An unsigned integral type for representing memory size in bytes.
/*! \ingroup PPL_CXX_interface */
typedef size_t memory_size_type;

//! Kinds of degenerate abstract elements.
/*! \ingroup PPL_CXX_interface */
enum Degenerate_Element {
  //! The universe element, i.e., the whole vector space.
  UNIVERSE,
  //! The empty element, i.e., the empty set.
  EMPTY
};

//! Relation symbols.
/*! \ingroup PPL_CXX_interface */
// This must be kept in sync with Result
enum Relation_Symbol {
  //! \hideinitializer Equal to.
  EQUAL = 1U,
  //! \hideinitializer Less than.
  LESS_THAN = 2U,
  //! \hideinitializer Less than or equal to.
  LESS_OR_EQUAL = LESS_THAN | EQUAL,
  //! \hideinitializer Greater than.
  GREATER_THAN = 4U,
  //! \hideinitializer Greater than or equal to.
  GREATER_OR_EQUAL = GREATER_THAN | EQUAL,
  //! \hideinitializer Not equal to.
  NOT_EQUAL = LESS_THAN | GREATER_THAN
};

//! Complexity pseudo-classes.
/*! \ingroup PPL_CXX_interface */
enum Complexity_Class {
  //! Worst-case polynomial complexity.
  POLYNOMIAL_COMPLEXITY,
  //! Worst-case exponential complexity but typically polynomial behavior.
  SIMPLEX_COMPLEXITY,
  //! Any complexity.
  ANY_COMPLEXITY
};

//! Possible optimization modes.
/*! \ingroup PPL_CXX_interface */
enum Optimization_Mode {
  //! Minimization is requested.
  MINIMIZATION,
  //! Maximization is requested.
  MAXIMIZATION
};

/*! \ingroup PPL_CXX_interface \brief
  Widths of bounded integer types.

  See the section on
  \ref Approximating_Bounded_Integers "approximating bounded integers".
*/
enum Bounded_Integer_Type_Width {
  //! \hideinitializer 8 bits.
  BITS_8 = 8,

  //! \hideinitializer 16 bits.
  BITS_16 = 16,

  //! \hideinitializer 32 bits.
  BITS_32 = 32,

  //! \hideinitializer 64 bits.
  BITS_64 = 64,

  //! \hideinitializer 128 bits.
  BITS_128 = 128
};

/*! \ingroup PPL_CXX_interface \brief
  Representation of bounded integer types.

  See the section on
  \ref Approximating_Bounded_Integers "approximating bounded integers".
*/
enum Bounded_Integer_Type_Representation {
  //! Unsigned binary.
  UNSIGNED,

  /*! \brief
    Signed binary where negative values are represented by the two's
    complement of the absolute value.
  */
  SIGNED_2_COMPLEMENT
};

/*! \ingroup PPL_CXX_interface \brief
  Overflow behavior of bounded integer types.

  See the section on
  \ref Approximating_Bounded_Integers "approximating bounded integers".
*/
enum Bounded_Integer_Type_Overflow {
  /*! \brief
    On overflow, wrapping takes place.

    This means that, for a \f$w\f$-bit bounded integer, the computation
    happens modulo \f$2^w\f$.
  */
  OVERFLOW_WRAPS,

  /*! \brief
    On overflow, the result is undefined.

    This simply means that the result of the operation resulting in an
    overflow can take any value.

    \note
    Even though something more serious can happen in the system
    being analyzed ---due to, e.g., C's undefined behavior---, here we
    are only concerned with the results of arithmetic operations.
    It is the responsibility of the analyzer to ensure that other
    manifestations of undefined behavior are conservatively approximated.
  */
  OVERFLOW_UNDEFINED,

  /*! \brief
    Overflow is impossible.

    This is for the analysis of languages where overflow is trapped
    before it affects the state, for which, thus, any indication that
    an overflow may have affected the state is necessarily due to
    the imprecision of the analysis.
  */
  OVERFLOW_IMPOSSIBLE
};

/*! \ingroup PPL_CXX_interface \brief
  Possible representations of coefficient sequences (i.e. linear expressions
  and more complex objects containing linear expressions, e.g. Constraints,
  Generators, etc.).
*/
enum Representation {
  /*! \brief
    Dense representation: the coefficient sequence is represented as a vector
    of coefficients, including the zero coefficients.
    If there are only a few nonzero coefficients, this representation is
    faster and also uses a bit less memory.
  */
  DENSE,

  /*! \brief
    Sparse representation: only the nonzero coefficient are stored.
    If there are many nonzero coefficients, this improves memory consumption
    and run time (both because there is less data to process in O(n)
    operations and because finding zeroes/nonzeroes is much faster since
    zeroes are not stored at all, so any stored coefficient is nonzero).
  */
  SPARSE
};

/*! \ingroup PPL_CXX_interface \brief
  Floating point formats known to the library.

  The parameters of each format are defined by a specific struct
  in file Float_defs.hh.  See the section on \ref floating_point
  "Analysis of floating point computations" for more information.
*/
enum Floating_Point_Format {
  //! IEEE 754 half precision, 16 bits (5 exponent, 10 mantissa).
  IEEE754_HALF,

  //! IEEE 754 single precision, 32 bits (8 exponent, 23 mantissa).
  IEEE754_SINGLE,

  //! IEEE 754 double precision, 64 bits (11 exponent, 52 mantissa).
  IEEE754_DOUBLE,

  //! IEEE 754 quad precision, 128 bits (15 exponent, 112 mantissa).
  IEEE754_QUAD,

  //! Intel double extended precision, 80 bits (15 exponent, 64 mantissa)
  INTEL_DOUBLE_EXTENDED,

  //! IBM single precision, 32 bits (7 exponent, 24 mantissa).
  IBM_SINGLE,

  //! IBM double precision, 64 bits (7 exponent, 56 mantissa).
  IBM_DOUBLE
};

struct Weightwatch_Traits;

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_globals_types_hh)
