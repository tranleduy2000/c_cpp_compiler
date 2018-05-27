dnl A function to check for the existence and usability of SICStus Prolog.
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

AC_DEFUN([AC_CHECK_SICSTUS_PROLOG],
[
AC_PATH_PROG(sicstus_prolog, sicstus)
if test x$sicstus_prolog != x
then
  sicstus_prolog_app_dir=`$sicstus_prolog -f --goal "use_module(library(system)), environ('SP_APP_DIR', X), write(X), nl, halt."`
  sicstus_prolog_base=`dirname $sicstus_prolog_app_dir`
  SICSTUS_PROLOG_INCLUDE_OPTIONS="-I${sicstus_prolog_base}/include"
  ac_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $SICSTUS_PROLOG_INCLUDE_OPTIONS"
  AC_LANG_PUSH(C++)
  # We require SICStus Prolog 3.9.1 or later.
  AC_CHECK_HEADER(sicstus/sicstus.h,
                  AC_MSG_CHECKING([for SICStus version 3.9.1 or later])
                  AC_EGREP_CPP(yes,
                    [
                      #include <sicstus/sicstus.h>
                      #if SICSTUS_MAJOR_VERSION > 3 || (SICSTUS_MAJOR_VERSION == 3 && SICSTUS_MINOR_VERSION > 9) || (SICSTUS_MAJOR_VERSION == 3 && SICSTUS_MINOR_VERSION == 9 && SICSTUS_REVISION_VERSION >= 1)
                      yes
                      #endif
                    ],
                    AC_MSG_RESULT(yes),
                    sicstus_prolog=""
                    AC_MSG_RESULT(no)
                  ),
                  sicstus_prolog="")
  AC_LANG_POP(C++)
  CPPFLAGS="$ac_save_CPPFLAGS"
  AC_SUBST(SICSTUS_PROLOG_INCLUDE_OPTIONS)
fi

if test x$sicstus_prolog != x
then
  have_sicstus_prolog=yes
else
  have_sicstus_prolog=no
fi
])
