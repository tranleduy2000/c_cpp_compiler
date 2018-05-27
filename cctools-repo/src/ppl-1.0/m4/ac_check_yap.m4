dnl A function to check for the existence and usability of YAP Prolog.
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

AC_DEFUN([AC_CHECK_YAP_PROLOG],
[
dnl By default, YAP-Prolog is installed as `yap'.
AC_PATH_PROG(yap_prolog, yap)
if test x$yap_prolog != x
then
  dnl Additional version checks could be inserted here, if necessary.

  YAP_PROLOG_INCLUDE_OPTIONS=""
  YAP_PROLOG_LD_OPTIONS="-lYap"
  ac_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $YAP_PROLOG_INCLUDE_OPTIONS"
  AC_LANG_PUSH(C++)
  # Check for YAP's foreign language interface header file.
  AC_CHECK_HEADER(Yap/c_interface.h,
                  [],
                  yap_prolog="")
  AC_LANG_POP(C++)
  CPPFLAGS="$ac_save_CPPFLAGS"
  YAP_PROLOG="$yap_prolog"
  AC_SUBST(YAP_PROLOG)
  AC_SUBST(YAP_PROLOG_INCLUDE_OPTIONS)
  AC_SUBST(YAP_PROLOG_LD_OPTIONS)
fi

if test x$yap_prolog != x
then
  have_yap_prolog=yes
else
  have_yap_prolog=no
fi
])
