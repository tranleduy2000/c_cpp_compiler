dnl A function to check for the possibility to control the FPU.
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

AC_DEFUN([AC_CHECK_FPU_CONTROL],
[
AC_LANG_PUSH(C++)
AC_CHECK_HEADERS([fenv.h ieeefp.h])
AC_MSG_CHECKING([if it is possible to control the FPU])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#if i386

int
main() {
  return 0;
}

#elif defined(HAVE_FENV_H)

# include <fenv.h>

# if !defined(FE_UPWARD) || !defined(FE_DOWNWARD)

  choke me

# elif defined(__arm__) \
  && (!defined(PPL_ARM_CAN_CONTROL_FPU) || !PPL_ARM_CAN_CONTROL_FPU)

  choke me

#else

     float  nf1 =  -3, pf1 = 3,  f2 =  5;
     double nd1 =  -7, pd1 = 7,  d2 = 11;
long double nl1 = -13, pl1 = 13, l2 = 17;

      float nf[2], pf[2];
     double nd[2], pd[2];
long double nl[2], pl[2];

int
ppl_check_function() {
  int r = 0;
  if (nf[0] == nf[1] || pf[0] == pf[1] || -nf[0] != pf[1] || -nf[1] != pf[0])
    r |= 1;
  if (nd[0] == nd[1] || pd[0] == pd[1] || -nd[0] != pd[1] || -nd[1] != pd[0])
    r |= 2;
  if (nl[0] == nl[1] || pl[0] == pl[1] || -nl[0] != pl[1] || -nl[1] != pl[0])
    r |= 4;
  return r;
}

int
ppl_setround_function(int rounding_mode) {
  return fesetround(rounding_mode);
}

int (* volatile ppl_check_function_p)() = ppl_check_function;
int (* volatile ppl_setround_function_p)(int) = ppl_setround_function;

int
main() {
  if ((*ppl_setround_function_p)(FE_DOWNWARD) != 0)
    return 255;

  nf[0] = nf1 / f2;
  nd[0] = nd1 / d2;
  nl[0] = nl1 / l2;
  pf[0] = pf1 / f2;
  pd[0] = pd1 / d2;
  pl[0] = pl1 / l2;

  if ((*ppl_setround_function_p)(FE_UPWARD) != 0)
    return 255;

  nf[1] = nf1 / f2;
  nd[1] = nd1 / d2;
  nl[1] = nl1 / l2;
  pf[1] = pf1 / f2;
  pd[1] = pd1 / d2;
  pl[1] = pl1 / l2;

  return (*ppl_check_function_p)();
}

# endif

#elif sparc && defined(HAVE_IEEEFP_H)

int
main() {
  return 0;
}

#else

  choke me

#endif
]])],
  AC_MSG_RESULT(yes)
  ac_cv_can_control_fpu=1,
  AC_MSG_RESULT(no)
  ac_cv_can_control_fpu=0,
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#if i386 || (sparc && defined(HAVE_IEEEFP_H))

int
main() {
  return 0;
}

#else

  choke me

#endif
  ]])],
    AC_MSG_RESULT(yes)
    ac_cv_can_control_fpu=1,
    AC_MSG_RESULT(no)
    ac_cv_can_control_fpu=0
  )
)
AC_LANG_POP(C++)
])
