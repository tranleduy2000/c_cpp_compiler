/* IA-32 floating point unit inline related functions.
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

#ifndef PPL_fpu_ia32_inlines_hh
#define PPL_fpu_ia32_inlines_hh 1

#include <csetjmp>
#include <csignal>

#define FPU_INVALID       0x01
#define FPU_DIVBYZERO     0x04
#define FPU_OVERFLOW      0x08
#define FPU_UNDERFLOW     0x10
#define FPU_INEXACT       0x20

#define FPU_ALL_EXCEPT \
  (FPU_INEXACT | FPU_DIVBYZERO | FPU_UNDERFLOW | FPU_OVERFLOW | FPU_INVALID)

#define PPL_FPU_TONEAREST     0
#define PPL_FPU_DOWNWARD      0x400
#define PPL_FPU_UPWARD        0x800
#define PPL_FPU_TOWARDZERO    0xc00

#define FPU_ROUNDING_MASK 0xc00

#define SSE_INEXACT       0x20

#define PPL_FPU_CONTROL_DEFAULT_BASE 0x37f
#define PPL_SSE_CONTROL_DEFAULT_BASE 0x1f80

// This MUST be congruent with the definition of ROUND_DIRECT
#define PPL_FPU_CONTROL_DEFAULT \
  (PPL_FPU_CONTROL_DEFAULT_BASE | PPL_FPU_UPWARD)
#define PPL_SSE_CONTROL_DEFAULT \
  (PPL_SSE_CONTROL_DEFAULT_BASE | (PPL_FPU_UPWARD << 3))

namespace Parma_Polyhedra_Library {

typedef struct {
  unsigned short control_word;
  unsigned short unused1;
  unsigned short status_word;
  unsigned short unused2;
  unsigned short tags;
  unsigned short unused3;
  unsigned int eip;
  unsigned short cs_selector;
  unsigned int opcode:11;
  unsigned int unused4:5;
  unsigned int data_offset;
  unsigned short data_selector;
  unsigned short unused5;
} ia32_fenv_t;

inline int
fpu_get_control() {
  unsigned short cw;
  __asm__ __volatile__ ("fnstcw %0" : "=m" (*&cw) : : "memory");
  return cw;
}

inline void
fpu_set_control(int c) {
  unsigned short cw = static_cast<unsigned short>(c);
  __asm__ __volatile__ ("fldcw %0" : : "m" (*&cw) : "memory");
}

inline int
fpu_get_status() {
  unsigned short sw;
  __asm__ __volatile__ ("fnstsw %0" : "=a" (sw) : : "memory");
  return sw;
}

inline void
fpu_clear_status(unsigned short bits) {
  /* There is no fldsw instruction */
  ia32_fenv_t env;
  __asm__ __volatile__ ("fnstenv %0" : "=m" (env));
  env.status_word = static_cast<unsigned short>(env.status_word & ~bits);
  __asm__ __volatile__ ("fldenv %0" : : "m" (env) : "memory");
}

inline void
fpu_clear_exceptions() {
  __asm__ __volatile__ ("fnclex" : /* No outputs.  */ : : "memory");
}

#ifdef PPL_FPMATH_MAY_USE_SSE
inline void
sse_set_control(unsigned int cw) {
  __asm__ __volatile__ ("ldmxcsr %0" : : "m" (*&cw) : "memory");
}

inline unsigned int
sse_get_control() {
  unsigned int cw;
  __asm__ __volatile__ ("stmxcsr %0" : "=m" (*&cw) : : "memory");
  return cw;
}
#endif

inline void
fpu_initialize_control_functions() {
#ifdef PPL_FPMATH_MAY_USE_SSE
  extern void detect_sse_unit();
  detect_sse_unit();
#endif
}

inline fpu_rounding_direction_type
fpu_get_rounding_direction() {
  return static_cast<fpu_rounding_direction_type>(fpu_get_control() & FPU_ROUNDING_MASK);
}

inline void
fpu_set_rounding_direction(fpu_rounding_direction_type dir) {
#ifdef PPL_FPMATH_MAY_USE_387
  fpu_set_control(PPL_FPU_CONTROL_DEFAULT_BASE | dir);
#endif
#ifdef PPL_FPMATH_MAY_USE_SSE
  extern bool have_sse_unit;
  if (have_sse_unit)
    sse_set_control(PPL_SSE_CONTROL_DEFAULT_BASE | (dir << 3));
#endif
}

inline fpu_rounding_control_word_type
fpu_save_rounding_direction(fpu_rounding_direction_type dir) {
#ifdef PPL_FPMATH_MAY_USE_387
  fpu_set_control(PPL_FPU_CONTROL_DEFAULT_BASE | dir);
#endif
#ifdef PPL_FPMATH_MAY_USE_SSE
  extern bool have_sse_unit;
  if (have_sse_unit)
    sse_set_control(PPL_SSE_CONTROL_DEFAULT_BASE | (dir << 3));
#endif
  return static_cast<fpu_rounding_control_word_type>(0);
}

inline void
fpu_reset_inexact() {
#ifdef PPL_FPMATH_MAY_USE_387
  fpu_clear_exceptions();
#endif
#ifdef PPL_FPMATH_MAY_USE_SSE
  // NOTE: on entry to this function the current rounding mode
  // has to be the default one.
  extern bool have_sse_unit;
  if (have_sse_unit)
    sse_set_control(PPL_SSE_CONTROL_DEFAULT);
#endif
}

inline void
fpu_restore_rounding_direction(fpu_rounding_control_word_type) {
#ifdef PPL_FPMATH_MAY_USE_387
  fpu_set_control(PPL_FPU_CONTROL_DEFAULT);
#endif
#ifdef PPL_FPMATH_MAY_USE_SSE
  extern bool have_sse_unit;
  if (have_sse_unit)
    sse_set_control(PPL_SSE_CONTROL_DEFAULT);
#endif
}

inline int
fpu_check_inexact() {
#ifdef PPL_FPMATH_MAY_USE_387
  if (fpu_get_status() & FPU_INEXACT)
    return 1;
#endif
#ifdef PPL_FPMATH_MAY_USE_SSE
  extern bool have_sse_unit;
  if (have_sse_unit && (sse_get_control() & SSE_INEXACT))
    return 1;
#endif
  return 0;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_fpu_ia32_inlines_hh)
