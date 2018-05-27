/* SPARC floating point unit related functions.
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

#ifndef PPL_fpu_sparc_inlines_hh
#define PPL_fpu_sparc_inlines_hh 1

#ifdef PPL_HAVE_IEEEFP_H
#include <ieeefp.h>

#define PPL_FPU_TONEAREST  ((int) FP_RN)
#define PPL_FPU_UPWARD     ((int) FP_RP)
#define PPL_FPU_DOWNWARD   ((int) FP_RM)
#define PPL_FPU_TOWARDZERO ((int) FP_RZ)

namespace Parma_Polyhedra_Library {

inline void
fpu_initialize_control_functions() {
}

inline fpu_rounding_direction_type
fpu_get_rounding_direction() {
  return static_cast<fpu_rounding_direction_type>(fpgetround());
}

inline void
fpu_set_rounding_direction(fpu_rounding_direction_type dir) {
  fpsetround((fp_rnd) dir);
}

inline fpu_rounding_control_word_type
fpu_save_rounding_direction(fpu_rounding_direction_type dir) {
  return static_cast<fpu_rounding_control_word_type>(fpsetround((fp_rnd) dir));
}

inline void
fpu_reset_inexact() {
  fp_except except = fpgetmask();
  except &= ~FP_X_IMP;
  fpsetmask(except);
}

inline void
fpu_restore_rounding_direction(fpu_rounding_control_word_type w) {
  fpsetround((fp_rnd) w);
}

inline int
fpu_check_inexact() {
  return (fpgetmask() & FP_X_IMP) ? 1 : 0;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_HAVE_IEEEFP_H)

#endif // !defined(PPL_fpu_sparc_inlines_hh)
