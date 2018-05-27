/* C99 Floating point unit related functions.
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

#ifndef PPL_fpu_c99_inlines_hh
#define PPL_fpu_c99_inlines_hh 1

#ifdef PPL_HAVE_FENV_H
#include <fenv.h>
#include <stdexcept>

#ifdef FE_TONEAREST
#define PPL_FPU_TONEAREST FE_TONEAREST
#endif
#ifdef FE_UPWARD
#define PPL_FPU_UPWARD FE_UPWARD
#endif
#ifdef FE_DOWNWARD
#define PPL_FPU_DOWNWARD FE_DOWNWARD
#endif
#ifdef FE_TOWARDZERO
#define PPL_FPU_TOWARDZERO FE_TOWARDZERO
#endif

namespace Parma_Polyhedra_Library {

inline void
fpu_initialize_control_functions() {
  int old = fegetround();
  if (fesetround(PPL_FPU_DOWNWARD) != 0
      || fesetround(PPL_FPU_UPWARD) != 0
      || fesetround(old) != 0)
    throw std::logic_error("PPL configuration error:"
			   " PPL_CAN_CONTROL_FPU evaluates to true,"
			   " but fesetround() returns nonzero.");
}

inline fpu_rounding_direction_type
fpu_get_rounding_direction() {
  return static_cast<fpu_rounding_direction_type>(fegetround());
}

inline void
fpu_set_rounding_direction(fpu_rounding_direction_type dir) {
  fesetround(dir);
}

inline fpu_rounding_control_word_type
fpu_save_rounding_direction(fpu_rounding_direction_type dir) {
  fpu_rounding_control_word_type old
    = static_cast<fpu_rounding_control_word_type>(fegetround());
  fesetround(dir);
  return old;
}

inline void
fpu_reset_inexact() {
#if PPL_CXX_SUPPORTS_IEEE_INEXACT_FLAG
  feclearexcept(FE_INEXACT);
#endif
}

inline void
fpu_restore_rounding_direction(fpu_rounding_control_word_type w) {
  fesetround(w);
}

inline int
fpu_check_inexact() {
#if PPL_CXX_SUPPORTS_IEEE_INEXACT_FLAG
  return fetestexcept(FE_INEXACT) != 0 ? 1 : 0;
#else
  return -1;
#endif
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_HAVE_FENV_H)

#endif // !defined(PPL_fpu_c99_inlines_hh)
