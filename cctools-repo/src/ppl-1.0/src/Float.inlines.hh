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

#ifndef PPL_Float_inlines_hh
#define PPL_Float_inlines_hh 1

#include <climits>
#include "Variable.defs.hh"
#include "Linear_Form.defs.hh"

namespace Parma_Polyhedra_Library {

inline int
float_ieee754_half::inf_sign() const {
  if (word == NEG_INF)
    return -1;
  if (word == POS_INF)
    return 1;
  return 0;
}

inline bool
float_ieee754_half::is_nan() const {
  return (word & ~SGN_MASK) > POS_INF;
}

inline int
float_ieee754_half::zero_sign() const {
  if (word == NEG_ZERO)
    return -1;
  if (word == POS_ZERO)
    return 1;
  return 0;
}

inline void
float_ieee754_half::negate() {
  word ^= SGN_MASK;
}

inline bool
float_ieee754_half::sign_bit() const {
  return (word & SGN_MASK) != 0;
}

inline void
float_ieee754_half::dec() {
  --word;
}

inline void
float_ieee754_half::inc() {
  ++word;
}

inline void
float_ieee754_half::set_max(bool negative) {
  word = 0x7bffU;
  if (negative)
    word |= SGN_MASK;
}

inline void
float_ieee754_half::build(bool negative, mpz_t mantissa, int exponent) {
  word = static_cast<uint16_t>(mpz_get_ui(mantissa)
                               & ((1UL << MANTISSA_BITS) - 1));
  if (negative)
    word |= SGN_MASK;
  int exponent_repr = exponent + EXPONENT_BIAS;
  PPL_ASSERT(exponent_repr >= 0 && exponent_repr < (1 << EXPONENT_BITS));
  word |= static_cast<uint16_t>(exponent_repr) << MANTISSA_BITS;
}

inline int
float_ieee754_single::inf_sign() const {
  if (word == NEG_INF)
    return -1;
  if (word == POS_INF)
    return 1;
  return 0;
}

inline bool
float_ieee754_single::is_nan() const {
  return (word & ~SGN_MASK) > POS_INF;
}

inline int
float_ieee754_single::zero_sign() const {
  if (word == NEG_ZERO)
    return -1;
  if (word == POS_ZERO)
    return 1;
  return 0;
}

inline void
float_ieee754_single::negate() {
  word ^= SGN_MASK;
}

inline bool
float_ieee754_single::sign_bit() const {
  return (word & SGN_MASK) != 0;
}

inline void
float_ieee754_single::dec() {
  --word;
}

inline void
float_ieee754_single::inc() {
  ++word;
}

inline void
float_ieee754_single::set_max(bool negative) {
  word = 0x7f7fffffU;
  if (negative)
    word |= SGN_MASK;
}

inline void
float_ieee754_single::build(bool negative, mpz_t mantissa, int exponent) {
  word = static_cast<uint32_t>(mpz_get_ui(mantissa)
                               & ((1UL << MANTISSA_BITS) - 1));
  if (negative)
    word |= SGN_MASK;
  int exponent_repr = exponent + EXPONENT_BIAS;
  PPL_ASSERT(exponent_repr >= 0 && exponent_repr < (1 << EXPONENT_BITS));
  word |= static_cast<uint32_t>(exponent_repr) << MANTISSA_BITS;
}

inline int
float_ieee754_double::inf_sign() const {
  if (lsp != LSP_INF)
    return 0;
  if (msp == MSP_NEG_INF)
    return -1;
  if (msp == MSP_POS_INF)
    return 1;
  return 0;
}

inline bool
float_ieee754_double::is_nan() const {
  uint32_t a = msp & ~MSP_SGN_MASK;
  return a > MSP_POS_INF || (a == MSP_POS_INF && lsp != LSP_INF);
}

inline int
float_ieee754_double::zero_sign() const {
  if (lsp != LSP_ZERO)
    return 0;
  if (msp == MSP_NEG_ZERO)
    return -1;
  if (msp == MSP_POS_ZERO)
    return 1;
  return 0;
}

inline void
float_ieee754_double::negate() {
  msp ^= MSP_SGN_MASK;
}

inline bool
float_ieee754_double::sign_bit() const {
  return (msp & MSP_SGN_MASK) != 0;
}

inline void
float_ieee754_double::dec() {
  if (lsp == 0) {
    --msp;
    lsp = LSP_MAX;
  }
  else
    --lsp;
}

inline void
float_ieee754_double::inc() {
  if (lsp == LSP_MAX) {
    ++msp;
    lsp = 0;
  }
  else
    ++lsp;
}

inline void
float_ieee754_double::set_max(bool negative) {
  msp = 0x7fefffffU;
  lsp = 0xffffffffU;
  if (negative)
    msp |= MSP_SGN_MASK;
}

inline void
float_ieee754_double::build(bool negative, mpz_t mantissa, int exponent) {
  unsigned long m;
#if ULONG_MAX == 0xffffffffUL
  lsp = mpz_get_ui(mantissa);
  mpz_tdiv_q_2exp(mantissa, mantissa, 32);
  m = mpz_get_ui(mantissa);
#else
  m = mpz_get_ui(mantissa);
  lsp = static_cast<uint32_t>(m & LSP_MAX);
  m >>= 32;
#endif
  msp = static_cast<uint32_t>(m & ((1UL << (MANTISSA_BITS - 32)) - 1));
  if (negative)
    msp |= MSP_SGN_MASK;
  int exponent_repr = exponent + EXPONENT_BIAS;
  PPL_ASSERT(exponent_repr >= 0 && exponent_repr < (1 << EXPONENT_BITS));
  msp |= static_cast<uint32_t>(exponent_repr) << (MANTISSA_BITS - 32);
}

inline int
float_ibm_single::inf_sign() const {
  if (word == NEG_INF)
    return -1;
  if (word == POS_INF)
    return 1;
  return 0;
}

inline bool
float_ibm_single::is_nan() const {
  return (word & ~SGN_MASK) > POS_INF;
}

inline int
float_ibm_single::zero_sign() const {
  if (word == NEG_ZERO)
    return -1;
  if (word == POS_ZERO)
    return 1;
  return 0;
}

inline void
float_ibm_single::negate() {
  word ^= SGN_MASK;
}

inline bool
float_ibm_single::sign_bit() const {
  return (word & SGN_MASK) != 0;
}

inline void
float_ibm_single::dec() {
  --word;
}

inline void
float_ibm_single::inc() {
  ++word;
}

inline void
float_ibm_single::set_max(bool negative) {
  word = 0x7f000000U;
  if (negative)
    word |= SGN_MASK;
}

inline void
float_ibm_single::build(bool negative, mpz_t mantissa, int exponent) {
  word = static_cast<uint32_t>(mpz_get_ui(mantissa)
                               & ((1UL << MANTISSA_BITS) - 1));
  if (negative)
    word |= SGN_MASK;
  int exponent_repr = exponent + EXPONENT_BIAS;
  PPL_ASSERT(exponent_repr >= 0 && exponent_repr < (1 << EXPONENT_BITS));
  word |= static_cast<uint32_t>(exponent_repr) << MANTISSA_BITS;
}

inline int
float_intel_double_extended::inf_sign() const {
  if (lsp != LSP_INF)
    return 0;
  uint32_t a = msp & MSP_NEG_INF;
  if (a == MSP_NEG_INF)
    return -1;
  if (a == MSP_POS_INF)
    return 1;
  return 0;
}

inline bool
float_intel_double_extended::is_nan() const {
  return (msp & MSP_POS_INF) == MSP_POS_INF
    && lsp != LSP_INF;
}

inline int
float_intel_double_extended::zero_sign() const {
  if (lsp != LSP_ZERO)
    return 0;
  uint32_t a = msp & MSP_NEG_INF;
  if (a == MSP_NEG_ZERO)
    return -1;
  if (a == MSP_POS_ZERO)
    return 1;
  return 0;
}

inline void
float_intel_double_extended::negate() {
  msp ^= MSP_SGN_MASK;
}

inline bool
float_intel_double_extended::sign_bit() const {
  return (msp & MSP_SGN_MASK) != 0;
}

inline void
float_intel_double_extended::dec() {
  if ((lsp & LSP_DMAX) == 0) {
    --msp;
    lsp = ((msp & MSP_NEG_INF) == 0) ? LSP_DMAX : LSP_NMAX;
  }
  else
    --lsp;
}

inline void
float_intel_double_extended::inc() {
  if ((lsp & LSP_DMAX) == LSP_DMAX) {
    ++msp;
    lsp = LSP_DMAX + 1;
  }
  else
    ++lsp;
}

inline void
float_intel_double_extended::set_max(bool negative) {
  msp = 0x00007ffeU;
  lsp = static_cast<uint64_t>(0xffffffffffffffffULL);
  if (negative)
    msp |= MSP_SGN_MASK;
}

inline void
float_intel_double_extended::build(bool negative,
				   mpz_t mantissa, int exponent) {
#if ULONG_MAX == 0xffffffffUL
  mpz_export(&lsp, 0, -1, 8, 0, 0, mantissa);
#else
  lsp = mpz_get_ui(mantissa);
#endif
  msp = (negative ? MSP_SGN_MASK : 0);
  int exponent_repr = exponent + EXPONENT_BIAS;
  PPL_ASSERT(exponent_repr >= 0 && exponent_repr < (1 << EXPONENT_BITS));
  msp |= static_cast<uint32_t>(exponent_repr);
}

inline int
float_ieee754_quad::inf_sign() const {
  if (lsp != LSP_INF)
    return 0;
  if (msp == MSP_NEG_INF)
    return -1;
  if (msp == MSP_POS_INF)
    return 1;
  return 0;
}

inline bool
float_ieee754_quad::is_nan() const {
  return (msp & ~MSP_SGN_MASK) == MSP_POS_INF
    && lsp != LSP_INF;
}

inline int
float_ieee754_quad::zero_sign() const {
  if (lsp != LSP_ZERO)
    return 0;
  if (msp == MSP_NEG_ZERO)
    return -1;
  if (msp == MSP_POS_ZERO)
    return 1;
  return 0;
}

inline void
float_ieee754_quad::negate() {
  msp ^= MSP_SGN_MASK;
}

inline bool
float_ieee754_quad::sign_bit() const {
  return (msp & MSP_SGN_MASK) != 0;
}

inline void
float_ieee754_quad::dec() {
  if (lsp == 0) {
    --msp;
    lsp = LSP_MAX;
  }
  else
    --lsp;
}

inline void
float_ieee754_quad::inc() {
  if (lsp == LSP_MAX) {
    ++msp;
    lsp = 0;
  }
  else
    ++lsp;
}

inline void
float_ieee754_quad::set_max(bool negative) {
  msp = static_cast<uint64_t>(0x7ffeffffffffffffULL);
  lsp = static_cast<uint64_t>(0xffffffffffffffffULL);
  if (negative)
    msp |= MSP_SGN_MASK;
}

inline void
float_ieee754_quad::build(bool negative, mpz_t mantissa, int exponent) {
  uint64_t parts[2];
  mpz_export(parts, 0, -1, 8, 0, 0, mantissa);
  lsp = parts[0];
  msp = parts[1];
  msp &= ((static_cast<uint64_t>(1) << (MANTISSA_BITS - 64)) - 1);
  if (negative)
    msp |= MSP_SGN_MASK;
  int exponent_repr = exponent + EXPONENT_BIAS;
  PPL_ASSERT(exponent_repr >= 0 && exponent_repr < (1 << EXPONENT_BITS));
  msp |= static_cast<uint64_t>(exponent_repr) << (MANTISSA_BITS - 64);
}

inline bool
is_less_precise_than(Floating_Point_Format f1, Floating_Point_Format f2) {
  return f1 < f2;
}

inline unsigned int
msb_position(unsigned long long v) {
  return static_cast<unsigned int>(sizeof_to_bits(sizeof(v))) - 1U - clz(v);
}

template <typename FP_Interval_Type>
inline void
affine_form_image(std::map<dimension_type,
                           Linear_Form<FP_Interval_Type> >& lf_store,
                  const Variable var,
                  const Linear_Form<FP_Interval_Type>& lf) {
  // Assign the new linear form for var.
  lf_store[var.id()] = lf;
  // Now invalidate all linear forms in which var occurs.
  discard_occurrences(lf_store, var);
}

#if PPL_SUPPORTED_FLOAT
inline
Float<float>::Float() {
}

inline
Float<float>::Float(float v) {
  u.number = v;
}

inline float
Float<float>::value() {
  return u.number;
}
#endif

#if PPL_SUPPORTED_DOUBLE
inline
Float<double>::Float() {
}

inline
Float<double>::Float(double v) {
  u.number = v;
}

inline double
Float<double>::value() {
  return u.number;
}
#endif

#if PPL_SUPPORTED_LONG_DOUBLE
inline
Float<long double>::Float() {
}

inline
Float<long double>::Float(long double v) {
  u.number = v;
}

inline long double
Float<long double>::value() {
  return u.number;
}
#endif

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Float_inlines_hh)
