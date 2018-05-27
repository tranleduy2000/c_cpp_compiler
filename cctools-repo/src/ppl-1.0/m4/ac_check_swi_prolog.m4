dnl A function to check for the existence and usability of SWI-Prolog.
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

AC_DEFUN([AC_CHECK_SWI_PROLOG],
[
dnl By default, old versions of SWI-Prolog were installed as `pl',
dnl though some administrators called it `swipl' or `swi-prolog'.
dnl In particular, on Mac OS X `pl' is the name of another program.
dnl On Windows, the console version of SWI-Prolog was called `plcon'.
dnl Since SWI-Prolog 5.9.9, by default and on all systems, the
dnl interpreter is called `swipl' and `plld' is called `swipl-ld'

AC_PATH_PROG(swi_prolog, swipl$EXEEXT)
if test -z $swi_prolog
then
  AC_PATH_PROG(swi_prolog, swi-prolog$EXEEXT)
  if test -z $swi_prolog
  then
    AC_PATH_PROG(swi_prolog, pl$EXEEXT)
    if test -z $swi_prolog
    then
      AC_PATH_PROG(swi_prolog, plcon$EXEEXT)
    fi
  fi
fi
if test x$swi_prolog != x
then
  swi_prolog_base=`$swi_prolog -dump-runtime-variables | grep PLBASE= | sed 's/PLBASE="\([[a-z]]:\)\{0,1\}\(.*\)";.*/\2/'`
  swi_prolog_version=`$swi_prolog -dump-runtime-variables | grep PLVERSION= | sed 's/PLVERSION="\(.*\)";.*/\1/'`
  swi_prolog_arch=`$swi_prolog -dump-runtime-variables | grep PLARCH= | sed 's/PLARCH="\(.*\)";.*/\1/'`
  swi_prolog_libs=`$swi_prolog -dump-runtime-variables | grep PLLIBS= | sed 's/PLLIBS="\(.*\)";.*/\1/'`
  swi_prolog_lib=`$swi_prolog -dump-runtime-variables | grep PLLIB= | sed 's/PLLIB="\(.*\)";.*/\1/'`

  dnl Additional version checks could be inserted here, if necessary.

  AC_PATH_PROG(swi_prolog_ld, swipl-ld$EXEEXT)
  if test -z $swi_prolog_ld
  then
    AC_PATH_PROG(swi_prolog_ld, plld$EXEEXT)
  fi

  # In Fedora, SWI-Prolog.h is installed only in /usr/include/pl, which,
  # IMHO, is a bug (https://bugzilla.redhat.com/show_bug.cgi?id=471071).
  SWI_PROLOG_INCLUDE_OPTIONS="-I${swi_prolog_base}/include -I/usr/include/pl"
  # If SWI-Prolog was configured with `--disable-libdirversion', then
  # the files are not in the `${swi_prolog_arch}' subdirectory.  Since
  # currently there is no way to know that, we look in both places.
  SWI_PROLOG_LD_OPTIONS="-L${swi_prolog_base}/lib/${swi_prolog_arch} -L${swi_prolog_base}/lib ${swi_prolog_lib} ${swi_prolog_libs}"
  ac_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $SWI_PROLOG_INCLUDE_OPTIONS"
  AC_LANG_PUSH(C++)
  # We require SWI-Prolog 5.6.0 or later.
  AC_CHECK_HEADER(SWI-Prolog.h,
                  AC_MSG_CHECKING([for SWI-Prolog version 5.6.0 or later])
                  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <SWI-Prolog.h>

int
main() {
#if !defined(PLVERSION) || PLVERSION < 50600
  choke me
#endif

  ;
  return 0;
}
                    ]])],
                    AC_MSG_RESULT(yes),
                    swi_prolog=""
                    AC_MSG_RESULT(no)
                  ),
                  swi_prolog="")
  AC_LANG_POP(C++)
  CPPFLAGS="$ac_save_CPPFLAGS"
  SWI_PROLOG="$swi_prolog"
  SWI_PROLOG_LD="$swi_prolog_ld"
  AC_SUBST(SWI_PROLOG)
  AC_SUBST(SWI_PROLOG_LD)
  AC_SUBST(SWI_PROLOG_INCLUDE_OPTIONS)
  AC_SUBST(SWI_PROLOG_LD_OPTIONS)
fi

if test x$swi_prolog != x
then
  have_swi_prolog=yes
else
  have_swi_prolog=no
fi
])
