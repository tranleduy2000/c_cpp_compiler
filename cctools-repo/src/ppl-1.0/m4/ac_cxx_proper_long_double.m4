dnl A function to check whether the C++ compiler provides long double
dnl numbers that have bigger range or precision than double.
dnl Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
dnl Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
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
dnl
AC_DEFUN([AC_CXX_PROVIDES_PROPER_LONG_DOUBLE],
[
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_LIBS="$LIBS"
AC_LANG_PUSH(C++)

AC_MSG_CHECKING([whether the C++ compiler provides proper long doubles])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <cfloat>

long double f = 0.0;

int main() {
  if ((LDBL_MAX <= DBL_MAX) && (DBL_EPSILON <= LDBL_EPSILON)
      && (LDBL_MAX_EXP <= DBL_MAX_EXP) && (LDBL_MANT_DIG <= DBL_MANT_DIG))
    return 1;
  else
    return 0;
}
]])],
  AC_MSG_RESULT(yes)
  ac_cxx_provides_proper_long_double=yes,
  AC_MSG_RESULT(no)
  ac_cxx_provides_proper_long_double=no,
  AC_MSG_RESULT([assuming not])
  ac_cxx_provides_proper_long_double=no)

if test x"$ac_cxx_provides_proper_long_double" = xyes
then
  value=1
else
  value=0
fi
AC_DEFINE_UNQUOTED(PPL_CXX_PROVIDES_PROPER_LONG_DOUBLE, $value,
  [Not zero if the C++ compiler provides long double numbers that have bigger range or precision than double.])

AC_LANG_POP(C++)
CPPFLAGS="$ac_save_CPPFLAGS"
LIBS="$ac_save_LIBS"
])
