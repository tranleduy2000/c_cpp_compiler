dnl A function to detect the binary format used by C++ floats.
dnl Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
dnl Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)
dnl
dnl This file is part of the Parma Polyhedra Library (PPL).
dnl
dnl The PPL is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by the
dnl Free Software Foundation; either version 3 of the License, or (at your
dnl option) any later version.
dnl
dnl The PPL is distributed in the hope that it will be useful, but WITHOUT
dnl ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
dnl FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
dnl for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software Foundation,
dnl Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
dnl
dnl For the most up-to-date information see the Parma Polyhedra Library
dnl site: http://bugseng.com/products/ppl/ .

AC_DEFUN([AC_CXX_FLOAT_BINARY_FORMAT],
[
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_LIBS="$LIBS"
AC_LANG_PUSH(C++)

AC_MSG_CHECKING([the binary format of C++ floats])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <limits>
#ifdef HAVE_STDINT_H
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS 1
#endif
#include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#if SIZEOF_FLOAT == 4

inline float
convert(uint32_t x) {
  union {
    float value;
    uint32_t word;
  } u;

  u.word = x;
  return u.value;
}

int
main() {
  if (std::numeric_limits<float>::is_iec559
      && (   convert(0xaaacccaaU)
          == -3.069535185924732179074680971098132431507110595703125e-13
          && convert(0xcccaaaccU)
          == -106255968
          && convert(0x00000001U)
          == 1.40129846432481707092372958328991613128026194187651577175706828388979108268586060148663818836212158203125e-45
          && convert(0x80000001U)
          == -1.40129846432481707092372958328991613128026194187651577175706828388979108268586060148663818836212158203125e-45))
    return 0;
  else
    return 1;
}

#else // SIZEOF_FLOAT != 4

int
main() {
  return 1;
}

#endif // SIZEOF_FLOAT != 4
]])],
  AC_DEFINE(PPL_CXX_FLOAT_BINARY_FORMAT, PPL_FLOAT_IEEE754_SINGLE,
    [The binary format of C++ floats, if supported; undefined otherwise.])
  ac_cxx_float_binary_format="IEEE754 Single Precision",
  ac_cxx_float_binary_format=unknown,
  ac_cxx_float_binary_format=unknown)

AC_MSG_RESULT($ac_cxx_float_binary_format)

if test x"$ac_cxx_float_binary_format" = x"unknown" || test $ac_cv_can_control_fpu = 0
then
  ac_supported_float=0
else
  ac_supported_float=1
fi
AM_CONDITIONAL(SUPPORTED_FLOAT, test $ac_supported_float = 1)
AC_DEFINE_UNQUOTED(PPL_SUPPORTED_FLOAT, $ac_supported_float,
  [Not zero if floats are supported.])

AC_LANG_POP(C++)
CPPFLAGS="$ac_save_CPPFLAGS"
LIBS="$ac_save_LIBS"
])
