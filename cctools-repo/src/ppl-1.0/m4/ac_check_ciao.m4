dnl A function to check for the existence and usability of Ciao-Prolog.
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

AC_DEFUN([AC_CHECK_CIAO_PROLOG],
[
AC_PATH_PROG(ciao_prolog, ciao)
if test x$ciao_prolog != x
then
  # Create a temporary directory $tmp in $TMPDIR (default /tmp).
  # Use mktemp if possible; otherwise fall back on mkdir,
  # with $RANDOM to make collisions less likely.
  : ${TMPDIR=/tmp}
  {
    tmp=`
      (umask 077 && mktemp -d "$TMPDIR/ppl_ciao_XXXXXX") 2>/dev/null
    ` &&
    test -n "$tmp" && test -d "$tmp"
  } || {
    tmp=$TMPDIR/ppl_ciao_$$-$RANDOM
    (umask 077 && mkdir "$tmp")
  } || exit $?
  cat >$tmp/print_include_dir.pl <<_CIAO_PRINT_INCLUDE_DIR_EOF
main(_) :-
    ciaolibdir(Lib_Dir),
    get_os(Os),
    get_arch(Arch),
    format("~p/include/~p~p", [[Lib_Dir, Os, Arch]]).
_CIAO_PRINT_INCLUDE_DIR_EOF
  ciao_prolog_include_dir=`$ciao_prolog $tmp/print_include_dir.pl`
  rm -rf $tmp

  CIAO_PROLOG_INCLUDE_OPTIONS="-I${ciao_prolog_include_dir}"

  ac_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $CIAO_PROLOG_INCLUDE_OPTIONS"
  AC_LANG_PUSH(C++)
  AC_CHECK_HEADER(ciao_prolog.h,
                  AC_MSG_CHECKING([for Ciao Prolog version x.y.z or later])
                  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <ciao_prolog.h>

int
main() {
  return 0;
}
                    ]])],
                    AC_MSG_RESULT(yes),
                    ciao_prolog=""
                    AC_MSG_RESULT(no)
                  ),
                  ciao_prolog="")

  dnl Additional version checks could be inserted here, if necessary.

  AC_LANG_POP(C++)
  CPPFLAGS="$ac_save_CPPFLAGS"
  CIAO_PROLOG="$ciao_prolog"
  AC_SUBST(CIAO_PROLOG)
  AC_SUBST(CIAO_PROLOG_INCLUDE_OPTIONS)
  dnl AC_SUBST(CIAO_PROLOG_LD_OPTIONS)
fi

if test x$ciao_prolog != x
then
  have_ciao_prolog=yes
else
  have_ciao_prolog=no
fi
])
