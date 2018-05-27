/* IEC 559 floating point format related functions:
   non-inline template functions.
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

#ifndef PPL_Float_templates_hh
#define PPL_Float_templates_hh 1

#include "Variable_defs.hh"
#include "Linear_Form_defs.hh"
#include <cmath>

namespace Parma_Polyhedra_Library {

template <typename FP_Interval_Type>
const FP_Interval_Type& compute_absolute_error(
                        const Floating_Point_Format analyzed_format) {
  typedef typename FP_Interval_Type::boundary_type analyzer_format;

  // FIXME: check if initializing caches with EMPTY is better.
  static const FP_Interval_Type ZERO_INTERVAL = FP_Interval_Type(0);
  // Cached results for each different analyzed format.
  static FP_Interval_Type ieee754_half_result = ZERO_INTERVAL;
  static FP_Interval_Type ieee754_single_result = ZERO_INTERVAL;
  static FP_Interval_Type ieee754_double_result = ZERO_INTERVAL;
  static FP_Interval_Type ibm_single_result = ZERO_INTERVAL;
  static FP_Interval_Type ieee754_quad_result = ZERO_INTERVAL;
  static FP_Interval_Type intel_double_extended_result = ZERO_INTERVAL;

  FP_Interval_Type* to_compute = NULL;
  // Get the necessary information on the analyzed's format.
  unsigned int f_base;
  int f_exponent_bias;
  unsigned int f_mantissa_bits;
  switch (analyzed_format) {
    case IEEE754_HALF:
      if (ieee754_half_result != ZERO_INTERVAL)
        return ieee754_half_result;

      to_compute = &ieee754_half_result;
      f_base = float_ieee754_half::BASE;
      f_exponent_bias = float_ieee754_half::EXPONENT_BIAS;
      f_mantissa_bits = float_ieee754_half::MANTISSA_BITS;
      break;
    case IEEE754_SINGLE:
      if (ieee754_single_result != ZERO_INTERVAL)
        return ieee754_single_result;

      to_compute = &ieee754_single_result;
      f_base = float_ieee754_single::BASE;
      f_exponent_bias = float_ieee754_single::EXPONENT_BIAS;
      f_mantissa_bits = float_ieee754_single::MANTISSA_BITS;
      break;
    case IEEE754_DOUBLE:
      if (ieee754_double_result != ZERO_INTERVAL)
        return ieee754_double_result;

      to_compute = &ieee754_double_result;
      f_base = float_ieee754_double::BASE;
      f_exponent_bias = float_ieee754_double::EXPONENT_BIAS;
      f_mantissa_bits = float_ieee754_double::MANTISSA_BITS;
      break;
    case IBM_SINGLE:
      if (ibm_single_result != ZERO_INTERVAL)
        return ibm_single_result;

      to_compute = &ibm_single_result;
      f_base = float_ibm_single::BASE;
      f_exponent_bias = float_ibm_single::EXPONENT_BIAS;
      f_mantissa_bits = float_ibm_single::MANTISSA_BITS;
      break;
    case IEEE754_QUAD:
      if (ieee754_quad_result != ZERO_INTERVAL)
        return ieee754_quad_result;

      to_compute = &ieee754_quad_result;
      f_base = float_ieee754_quad::BASE;
      f_exponent_bias = float_ieee754_quad::EXPONENT_BIAS;
      f_mantissa_bits = float_ieee754_quad::MANTISSA_BITS;
      break;
    case INTEL_DOUBLE_EXTENDED:
      if (intel_double_extended_result != ZERO_INTERVAL)
        return intel_double_extended_result;

      to_compute = &intel_double_extended_result;
      f_base = float_intel_double_extended::BASE;
      f_exponent_bias = float_intel_double_extended::EXPONENT_BIAS;
      f_mantissa_bits = float_intel_double_extended::MANTISSA_BITS;
      break;
    default:
      PPL_UNREACHABLE;
      break;
  }

  PPL_ASSERT(to_compute != NULL);

  // We assume that f_base is a power of 2.
  analyzer_format omega;
  int power = static_cast<int>(msb_position(f_base))
    * ((1 - f_exponent_bias) - static_cast<int>(f_mantissa_bits));
  omega = std::max(static_cast<analyzer_format>(ldexp(1.0, power)),
                   std::numeric_limits<analyzer_format>::denorm_min());

  to_compute->build(i_constraint(GREATER_OR_EQUAL, -omega),
                    i_constraint(LESS_OR_EQUAL, omega));
  return *to_compute;
}

template <typename FP_Interval_Type>
void
discard_occurrences(std::map<dimension_type,
                             Linear_Form<FP_Interval_Type> >& lf_store,
                    Variable var) {
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;
  typedef typename std::map<dimension_type, FP_Linear_Form>::iterator Iter;
  for (Iter i = lf_store.begin(); i != lf_store.end(); ) {
    if((i->second).coefficient(var) != 0)
      i = lf_store.erase(i);
    else
      ++i;
  }
}

/* FIXME: improve efficiency by adding the list of potentially conflicting
   variables as an argument. */
template <typename FP_Interval_Type>
void upper_bound_assign(std::map<dimension_type,
                                 Linear_Form<FP_Interval_Type> >& ls1,
                        const std::map<dimension_type,
                                       Linear_Form<FP_Interval_Type> >& ls2) {
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;
  typedef typename std::map<dimension_type, FP_Linear_Form>::iterator Iter;
  typedef typename std::map<dimension_type,
                            FP_Linear_Form>::const_iterator Const_Iter;

  Const_Iter i2_end = ls2.end();
  for (Iter i1 = ls1.begin(), i1_end = ls1.end(); i1 != i1_end; ) {
    Const_Iter i2 = ls2.find(i1->first);
    if ((i2 == i2_end) || (i1->second != i2->second))
      i1 = ls1.erase(i1);
    else
      ++i1;
  }
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Float_templates_hh)
