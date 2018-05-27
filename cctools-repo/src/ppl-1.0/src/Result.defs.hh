/* Result enum and supporting function declarations.
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

#ifndef PPL_Result_defs_hh
#define PPL_Result_defs_hh 1

namespace Parma_Polyhedra_Library {

enum Result_Class {
  //! \hideinitializer Representable number result class.
  VC_NORMAL = 0U << 4,

  //! \hideinitializer Negative infinity result class.
  VC_MINUS_INFINITY = 1U << 4,

  //! \hideinitializer Positive infinity result class.
  VC_PLUS_INFINITY = 2U << 4,

  //! \hideinitializer Not a number result class.
  VC_NAN = 3U << 4,

  VC_MASK = VC_NAN
};

// This must be kept in sync with Relation_Symbol
enum Result_Relation {
  //! \hideinitializer No values satisfies the relation.
  VR_EMPTY = 0U,

  //! \hideinitializer Equal. This need to be accompanied by a value.
  VR_EQ = 1U,

  //! \hideinitializer Less than. This need to be accompanied by a value.
  VR_LT = 2U,

  //! \hideinitializer Greater than. This need to be accompanied by a value.
  VR_GT = 4U,

  //! \hideinitializer Not equal. This need to be accompanied by a value.
  VR_NE = VR_LT | VR_GT,

  //! \hideinitializer Less or equal. This need to be accompanied by a value.
  VR_LE = VR_EQ | VR_LT,

  //! \hideinitializer Greater or equal. This need to be accompanied by a value.
  VR_GE = VR_EQ | VR_GT,

  //! \hideinitializer All values satisfy the relation.
  VR_LGE = VR_LT | VR_EQ | VR_GT,

  VR_MASK = VR_LGE
};

//! Possible outcomes of a checked arithmetic computation.
/*! \ingroup PPL_CXX_interface */
enum Result {
  //! \hideinitializer The exact result is not comparable.
  V_EMPTY = VR_EMPTY,

  //! \hideinitializer The computed result is exact.
  V_EQ = static_cast<unsigned>(VR_EQ),

  //! \hideinitializer The computed result is inexact and rounded up.
  V_LT = static_cast<unsigned>(VR_LT),

  //! \hideinitializer The computed result is inexact and rounded down.
  V_GT = static_cast<unsigned>(VR_GT),

  //! \hideinitializer The computed result is inexact.
  V_NE = VR_NE,

  //! \hideinitializer The computed result may be inexact and rounded up.
  V_LE = VR_LE,

  //! \hideinitializer The computed result may be inexact and rounded down.
  V_GE = VR_GE,

  //! \hideinitializer The computed result may be inexact.
  V_LGE = VR_LGE,

  //! \hideinitializer The exact result is a number out of finite bounds.
  V_OVERFLOW = 1U << 6,

  //! \hideinitializer A negative integer overflow occurred (rounding up).
  V_LT_INF = V_LT | V_OVERFLOW,

  //! \hideinitializer A positive integer overflow occurred (rounding down).
  V_GT_SUP = V_GT | V_OVERFLOW,

  //! \hideinitializer A positive integer overflow occurred (rounding up).
  V_LT_PLUS_INFINITY = V_LT | static_cast<unsigned>(VC_PLUS_INFINITY),

  //! \hideinitializer A negative integer overflow occurred (rounding down).
  V_GT_MINUS_INFINITY = V_GT | static_cast<unsigned>(VC_MINUS_INFINITY),

  //! \hideinitializer Negative infinity result.
  V_EQ_MINUS_INFINITY = V_EQ | static_cast<unsigned>(VC_MINUS_INFINITY),

  //! \hideinitializer Positive infinity result.
  V_EQ_PLUS_INFINITY = V_EQ | static_cast<unsigned>(VC_PLUS_INFINITY),

  //! \hideinitializer Not a number result.
  V_NAN = static_cast<unsigned>(VC_NAN),

  //! \hideinitializer Converting from unknown string.
  V_CVT_STR_UNK = V_NAN | (1U << 8),

  //! \hideinitializer Dividing by zero.
  V_DIV_ZERO = V_NAN | (2U << 8),

  //! \hideinitializer Adding two infinities having opposite signs.
  V_INF_ADD_INF = V_NAN | (3U << 8),

  //! \hideinitializer Dividing two infinities.
  V_INF_DIV_INF = V_NAN | (4U << 8),

  //! \hideinitializer Taking the modulus of an infinity.
  V_INF_MOD = V_NAN | (5U << 8),

  //! \hideinitializer Multiplying an infinity by zero.
  V_INF_MUL_ZERO = V_NAN | (6U << 8),

  //! \hideinitializer Subtracting two infinities having the same sign.
  V_INF_SUB_INF = V_NAN | (7U << 8),

  //! \hideinitializer Computing a remainder modulo zero.
  V_MOD_ZERO = V_NAN | (8U << 8),

  //! \hideinitializer Taking the square root of a negative number.
  V_SQRT_NEG = V_NAN | (9U << 8),

  //! \hideinitializer Unknown result due to intermediate negative overflow.
  V_UNKNOWN_NEG_OVERFLOW = V_NAN | (10U << 8),

  //! \hideinitializer Unknown result due to intermediate positive overflow.
  V_UNKNOWN_POS_OVERFLOW = V_NAN | (11U << 8),

  //! \hideinitializer The computed result is not representable.
  V_UNREPRESENTABLE = 1U << 7

};

//! \name Functions Inspecting and/or Combining Result Values
//@{

/*! \ingroup PPL_CXX_interface */
Result operator&(Result x, Result y);

/*! \ingroup PPL_CXX_interface */
Result operator|(Result x, Result y);

/*! \ingroup PPL_CXX_interface */
Result operator-(Result x, Result y);

/*! \brief \ingroup PPL_CXX_interface
  Extracts the value class part of \p r (representable number,
  unrepresentable minus/plus infinity or nan).
*/
Result_Class result_class(Result r);

/*! \brief \ingroup PPL_CXX_interface
  Extracts the relation part of \p r.
*/
Result_Relation result_relation(Result r);

/*! \ingroup PPL_CXX_interface */
Result result_relation_class(Result r);

//@} // Functions Inspecting and/or Combining Result Values

} // namespace Parma_Polyhedra_Library

#include "Result.inlines.hh"

#endif // !defined(PPL_Result_defs_hh)
