/* IEC 559 floating point format related functions.
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

#ifndef PPL_Float_defs_hh
#define PPL_Float_defs_hh 1

#include "globals.types.hh"
#include "meta_programming.hh"
#include "compiler.hh"
#include "assert.hh"
#include "Concrete_Expression.types.hh"
#include "Variable.types.hh"
#include "Linear_Form.types.hh"
#include <set>
#include <cmath>
#include <map>
#include <gmp.h>

#ifdef NAN
#define PPL_NAN NAN
#else
#define PPL_NAN (HUGE_VAL - HUGE_VAL)
#endif

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct float_ieee754_half {
  uint16_t word;
  static const uint16_t SGN_MASK = 0x8000U;
  static const uint16_t EXP_MASK = 0xfc00U;
  static const uint16_t POS_INF = 0xfc00U;
  static const uint16_t NEG_INF = 0x7c00U;
  static const uint16_t POS_ZERO = 0x0000U;
  static const uint16_t NEG_ZERO = 0x8000U;
  static const unsigned int BASE = 2;
  static const unsigned int EXPONENT_BITS = 5;
  static const unsigned int MANTISSA_BITS = 10;
  static const int EXPONENT_MAX = (1 << (EXPONENT_BITS - 1)) - 1;
  static const int EXPONENT_BIAS = EXPONENT_MAX;
  static const int EXPONENT_MIN = -EXPONENT_MAX + 1;
  static const int EXPONENT_MIN_DENORM = EXPONENT_MIN
					- static_cast<int>(MANTISSA_BITS);
  static const Floating_Point_Format floating_point_format = IEEE754_HALF;
  int inf_sign() const;
  bool is_nan() const;
  int zero_sign() const;
  bool sign_bit() const;
  void negate();
  void dec();
  void inc();
  void set_max(bool negative);
  void build(bool negative, mpz_t mantissa, int exponent);

};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct float_ieee754_single {
  uint32_t word;
  static const uint32_t SGN_MASK = 0x80000000U;
  static const uint32_t EXP_MASK = 0x7f800000U;
  static const uint32_t POS_INF = 0x7f800000U;
  static const uint32_t NEG_INF = 0xff800000U;
  static const uint32_t POS_ZERO = 0x00000000U;
  static const uint32_t NEG_ZERO = 0x80000000U;
  static const unsigned int BASE = 2;
  static const unsigned int EXPONENT_BITS = 8;
  static const unsigned int MANTISSA_BITS = 23;
  static const int EXPONENT_MAX = (1 << (EXPONENT_BITS - 1)) - 1;
  static const int EXPONENT_BIAS = EXPONENT_MAX;
  static const int EXPONENT_MIN = -EXPONENT_MAX + 1;
  static const int EXPONENT_MIN_DENORM = EXPONENT_MIN
					- static_cast<int>(MANTISSA_BITS);
  static const Floating_Point_Format floating_point_format = IEEE754_SINGLE;
  int inf_sign() const;
  bool is_nan() const;
  int zero_sign() const;
  bool sign_bit() const;
  void negate();
  void dec();
  void inc();
  void set_max(bool negative);
  void build(bool negative, mpz_t mantissa, int exponent);
};

#ifdef WORDS_BIGENDIAN
#ifndef PPL_WORDS_BIGENDIAN
#define PPL_WORDS_BIGENDIAN
#endif
#endif

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct float_ieee754_double {
#ifdef PPL_WORDS_BIGENDIAN
  uint32_t msp;
  uint32_t lsp;
#else
  uint32_t lsp;
  uint32_t msp;
#endif
  static const uint32_t MSP_SGN_MASK = 0x80000000U;
  static const uint32_t MSP_POS_INF = 0x7ff00000U;
  static const uint32_t MSP_NEG_INF = 0xfff00000U;
  static const uint32_t MSP_POS_ZERO = 0x00000000U;
  static const uint32_t MSP_NEG_ZERO = 0x80000000U;
  static const uint32_t LSP_INF = 0;
  static const uint32_t LSP_ZERO = 0;
  static const uint32_t LSP_MAX = 0xffffffffU;
  static const unsigned int BASE = 2;
  static const unsigned int EXPONENT_BITS = 11;
  static const unsigned int MANTISSA_BITS = 52;
  static const int EXPONENT_MAX = (1 << (EXPONENT_BITS - 1)) - 1;
  static const int EXPONENT_BIAS = EXPONENT_MAX;
  static const int EXPONENT_MIN = -EXPONENT_MAX + 1;
  static const int EXPONENT_MIN_DENORM = EXPONENT_MIN
					- static_cast<int>(MANTISSA_BITS);
  static const Floating_Point_Format floating_point_format = IEEE754_DOUBLE;
  int inf_sign() const;
  bool is_nan() const;
  int zero_sign() const;
  bool sign_bit() const;
  void negate();
  void dec();
  void inc();
  void set_max(bool negative);
  void build(bool negative, mpz_t mantissa, int exponent);
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct float_ibm_single {
  uint32_t word;
  static const uint32_t SGN_MASK = 0x80000000U;
  static const uint32_t EXP_MASK = 0x7f000000U;
  static const uint32_t POS_INF = 0x7f000000U;
  static const uint32_t NEG_INF = 0xff000000U;
  static const uint32_t POS_ZERO = 0x00000000U;
  static const uint32_t NEG_ZERO = 0x80000000U;
  static const unsigned int BASE = 16;
  static const unsigned int EXPONENT_BITS = 7;
  static const unsigned int MANTISSA_BITS = 24;
  static const int EXPONENT_BIAS = 64;
  static const int EXPONENT_MAX = (1 << (EXPONENT_BITS - 1)) - 1;
  static const int EXPONENT_MIN = -EXPONENT_MAX + 1;
  static const int EXPONENT_MIN_DENORM = EXPONENT_MIN
					- static_cast<int>(MANTISSA_BITS);
  static const Floating_Point_Format floating_point_format = IBM_SINGLE;
  int inf_sign() const;
  bool is_nan() const;
  int zero_sign() const;
  bool sign_bit() const;
  void negate();
  void dec();
  void inc();
  void set_max(bool negative);
  void build(bool negative, mpz_t mantissa, int exponent);
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct float_ibm_double {
  static const unsigned int BASE = 16;
  static const unsigned int EXPONENT_BITS = 7;
  static const unsigned int MANTISSA_BITS = 56;
  static const int EXPONENT_BIAS = 64;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct float_intel_double_extended {
#ifdef PPL_WORDS_BIGENDIAN
  uint32_t msp;
  uint64_t lsp;
#else
  uint64_t lsp;
  uint32_t msp;
#endif
  static const uint32_t MSP_SGN_MASK = 0x00008000U;
  static const uint32_t MSP_POS_INF = 0x00007fffU;
  static const uint32_t MSP_NEG_INF = 0x0000ffffU;
  static const uint32_t MSP_POS_ZERO = 0x00000000U;
  static const uint32_t MSP_NEG_ZERO = 0x00008000U;
  static const uint64_t LSP_INF = static_cast<uint64_t>(0x8000000000000000ULL);
  static const uint64_t LSP_ZERO = 0;
  static const uint64_t LSP_DMAX = static_cast<uint64_t>(0x7fffffffffffffffULL);
  static const uint64_t LSP_NMAX = static_cast<uint64_t>(0xffffffffffffffffULL);
  static const unsigned int BASE = 2;
  static const unsigned int EXPONENT_BITS = 15;
  static const unsigned int MANTISSA_BITS = 63;
  static const int EXPONENT_MAX = (1 << (EXPONENT_BITS - 1)) - 1;
  static const int EXPONENT_BIAS = EXPONENT_MAX;
  static const int EXPONENT_MIN = -EXPONENT_MAX + 1;
  static const int EXPONENT_MIN_DENORM = EXPONENT_MIN
					- static_cast<int>(MANTISSA_BITS);
  static const Floating_Point_Format floating_point_format =
                                     INTEL_DOUBLE_EXTENDED;
  int inf_sign() const;
  bool is_nan() const;
  int zero_sign() const;
  bool sign_bit() const;
  void negate();
  void dec();
  void inc();
  void set_max(bool negative);
  void build(bool negative, mpz_t mantissa, int exponent);
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
struct float_ieee754_quad {
#ifdef PPL_WORDS_BIGENDIAN
  uint64_t msp;
  uint64_t lsp;
#else
  uint64_t lsp;
  uint64_t msp;
#endif
  static const uint64_t MSP_SGN_MASK = static_cast<uint64_t>(0x8000000000000000ULL);
  static const uint64_t MSP_POS_INF = static_cast<uint64_t>(0x7fff000000000000ULL);
  static const uint64_t MSP_NEG_INF = static_cast<uint64_t>(0xffff000000000000ULL);
  static const uint64_t MSP_POS_ZERO = static_cast<uint64_t>(0x0000000000000000ULL);
  static const uint64_t MSP_NEG_ZERO = static_cast<uint64_t>(0x8000000000000000ULL);
  static const uint64_t LSP_INF = 0;
  static const uint64_t LSP_ZERO = 0;
  static const uint64_t LSP_MAX = static_cast<uint64_t>(0xffffffffffffffffULL);
  static const unsigned int BASE = 2;
  static const unsigned int EXPONENT_BITS = 15;
  static const unsigned int MANTISSA_BITS = 112;
  static const int EXPONENT_MAX = (1 << (EXPONENT_BITS - 1)) - 1;
  static const int EXPONENT_BIAS = EXPONENT_MAX;
  static const int EXPONENT_MIN = -EXPONENT_MAX + 1;
  static const int EXPONENT_MIN_DENORM = EXPONENT_MIN
					- static_cast<int>(MANTISSA_BITS);
  int inf_sign() const;
  bool is_nan() const;
  int zero_sign() const;
  bool sign_bit() const;
  void negate();
  void dec();
  void inc();
  void set_max(bool negative);
  void build(bool negative, mpz_t mantissa, int exponent);
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Float : public False { };

#if PPL_SUPPORTED_FLOAT
template <>
class Float<float> : public True {
public:
#if PPL_CXX_FLOAT_BINARY_FORMAT == PPL_FLOAT_IEEE754_HALF
  typedef float_ieee754_half Binary;
#elif PPL_CXX_FLOAT_BINARY_FORMAT == PPL_FLOAT_IEEE754_SINGLE
  typedef float_ieee754_single Binary;
#elif PPL_CXX_FLOAT_BINARY_FORMAT == PPL_FLOAT_IEEE754_DOUBLE
  typedef float_ieee754_double Binary;
#elif PPL_CXX_FLOAT_BINARY_FORMAT == PPL_FLOAT_IBM_SINGLE
  typedef float_ibm_single Binary;
#elif PPL_CXX_FLOAT_BINARY_FORMAT == PPL_FLOAT_IEEE754_QUAD
  typedef float_ieee754_quad Binary;
#elif PPL_CXX_FLOAT_BINARY_FORMAT == PPL_FLOAT_INTEL_DOUBLE_EXTENDED
  typedef float_intel_double_extended Binary;
#else
#error "Invalid value for PPL_CXX_FLOAT_BINARY_FORMAT"
#endif
  union {
    float number;
    Binary binary;
  } u;
  Float();
  Float(float v);
  float value();
};
#endif

#if PPL_SUPPORTED_DOUBLE
template <>
class Float<double> : public True {
public:
#if PPL_CXX_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IEEE754_HALF
  typedef float_ieee754_half Binary;
#elif PPL_CXX_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IEEE754_SINGLE
  typedef float_ieee754_single Binary;
#elif PPL_CXX_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IEEE754_DOUBLE
  typedef float_ieee754_double Binary;
#elif PPL_CXX_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IBM_SINGLE
  typedef float_ibm_single Binary;
#elif PPL_CXX_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IEEE754_QUAD
  typedef float_ieee754_quad Binary;
#elif PPL_CXX_DOUBLE_BINARY_FORMAT == PPL_FLOAT_INTEL_DOUBLE_EXTENDED
  typedef float_intel_double_extended Binary;
#else
#error "Invalid value for PPL_CXX_DOUBLE_BINARY_FORMAT"
#endif
  union {
    double number;
    Binary binary;
  } u;
  Float();
  Float(double v);
  double value();
};
#endif

#if PPL_SUPPORTED_LONG_DOUBLE
template <>
class Float<long double> : public True {
public:
#if PPL_CXX_LONG_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IEEE754_HALF
  typedef float_ieee754_half Binary;
#elif PPL_CXX_LONG_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IEEE754_SINGLE
  typedef float_ieee754_single Binary;
#elif PPL_CXX_LONG_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IEEE754_DOUBLE
  typedef float_ieee754_double Binary;
#elif PPL_CXX_LONG_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IBM_SINGLE
  typedef float_ibm_single Binary;
#elif PPL_CXX_LONG_DOUBLE_BINARY_FORMAT == PPL_FLOAT_IEEE754_QUAD
  typedef float_ieee754_quad Binary;
#elif PPL_CXX_LONG_DOUBLE_BINARY_FORMAT == PPL_FLOAT_INTEL_DOUBLE_EXTENDED
  typedef float_intel_double_extended Binary;
#else
#error "Invalid value for PPL_CXX_LONG_DOUBLE_BINARY_FORMAT"
#endif
  union {
    long double number;
    Binary binary;
  } u;
  Float();
  Float(long double v);
  long double value();
};
#endif

// FIXME: is this the right place for this function?
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  If \p v is nonzero, returns the position of the most significant bit
  in \p a.

  The behavior is undefined if \p v is zero.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
unsigned int msb_position(unsigned long long v);

/*! \brief
  An abstract class to be implemented by an external analyzer such
  as ECLAIR in order to provide to the PPL the necessary information
  for performing the analysis of floating point computations.

  \par Template type parameters

  - The class template parameter \p Target specifies the implementation
  of Concrete_Expression to be used.
  - The class template parameter \p FP_Interval_Type represents the type
  of the intervals used in the abstract domain. The interval bounds
  should have a floating point type.
*/
template <typename Target, typename FP_Interval_Type>
class FP_Oracle {
public:
  /*
    FIXME: the const qualifiers on expressions may raise CLANG
    compatibility issues. It may be necessary to omit them.
  */

  /*! \brief
    Asks the external analyzer for an interval that correctly
    approximates the floating point entity referenced by \p dim.
    Result is stored into \p result.

    \return <CODE>true</CODE> if the analyzer was able to find a correct
    approximation, <CODE>false</CODE> otherwise.
  */
  virtual bool get_interval(dimension_type dim, FP_Interval_Type& result) const
    = 0;

  /*! \brief
    Asks the external analyzer for an interval that correctly
    approximates the value of floating point constant \p expr.
    Result is stored into \p result.

    \return <CODE>true</CODE> if the analyzer was able to find a correct
    approximation, <CODE>false</CODE> otherwise.
  */
  virtual bool get_fp_constant_value(
               const Floating_Point_Constant<Target>& expr,
                     FP_Interval_Type& result) const = 0;

  /*! \brief
    Asks the external analyzer for an interval that correctly approximates
    the value of \p expr, which must be of integer type.
    Result is stored into \p result.

    \return <CODE>true</CODE> if the analyzer was able to find a correct
    approximation, <CODE>false</CODE> otherwise.
  */
  virtual bool get_integer_expr_value(const Concrete_Expression<Target>& expr,
                                      FP_Interval_Type& result) const = 0;

  /*! \brief
    Asks the external analyzer for the possible space dimensions that
    are associated to the approximable reference \p expr.
    Result is stored into \p result.

    \return <CODE>true</CODE> if the analyzer was able to return
    the (possibly empty!) set, <CODE>false</CODE> otherwise.

    The resulting set MUST NOT contain <CODE>not_a_dimension()</CODE>.
  */
  virtual bool get_associated_dimensions(
	  const Approximable_Reference<Target>& expr,
          std::set<dimension_type>& result) const = 0;

};

/* FIXME: some of the following  documentation should probably be
   under PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS */

/*! \brief \relates Float
  Returns <CODE>true</CODE> if and only if there is some floating point
  number that is representable by \p f2 but not by \p f1.
*/
bool is_less_precise_than(Floating_Point_Format f1, Floating_Point_Format f2);

/*! \brief \relates Float
  Computes the absolute error of floating point computations.

  \par Template type parameters

  - The class template parameter \p FP_Interval_Type represents the type
  of the intervals used in the abstract domain. The interval bounds
  should have a floating point type.

  \param analyzed_format The floating point format used by the analyzed
  program.

  \return The interval \f$[-\omega, \omega]\f$ where \f$\omega\f$ is the
  smallest non-zero positive number in the less precise floating point
  format between the analyzer format and the analyzed format.
*/
template <typename FP_Interval_Type>
const FP_Interval_Type&
compute_absolute_error(Floating_Point_Format analyzed_format);

/*! \brief \relates Linear_Form
  Discards all linear forms containing variable \p var from the
  linear form abstract store \p lf_store.
*/
template <typename FP_Interval_Type>
void
discard_occurrences(std::map<dimension_type,
                             Linear_Form<FP_Interval_Type> >& lf_store,
                    Variable var);

/*! \brief \relates Linear_Form
  Assigns the linear form \p lf to \p var in the linear form abstract
  store \p lf_store, then discards all occurrences of \p var from it.
*/
template <typename FP_Interval_Type>
void
affine_form_image(std::map<dimension_type,
                           Linear_Form<FP_Interval_Type> >& lf_store,
                  Variable var,
                  const Linear_Form<FP_Interval_Type>& lf);

/*! \brief \relates Linear_Form
  Discards from \p ls1 all linear forms but those that are associated
  to the same variable in \p ls2.
*/
template <typename FP_Interval_Type>
void
upper_bound_assign(std::map<dimension_type,
                            Linear_Form<FP_Interval_Type> >& ls1,
                   const std::map<dimension_type,
                                  Linear_Form<FP_Interval_Type> >& ls2);

} // namespace Parma_Polyhedra_Library

#include "Float.inlines.hh"
#include "Float.templates.hh"

#endif // !defined(PPL_Float_defs_hh)
