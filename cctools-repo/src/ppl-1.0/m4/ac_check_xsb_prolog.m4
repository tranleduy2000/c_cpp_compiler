dnl A function to check for the existence and usability of XSB Prolog.
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

AC_DEFUN([AC_CHECK_XSB_PROLOG],
[
AC_PATH_PROG(xsb_prolog, xsb)
if test x$xsb_prolog != x
then
  dnl The foreign language interface include file is `cinterf.h',
  dnl but this resides in the emulator directory and is not installed
  dnl in any standard place.
  xsb_emu_dir=`xsb --nobanner --quietload --noprompt \
                -e "write('emudir='), \
                  xsb_configuration(emudir, X), write(X), nl, halt." \
                    2>/dev/null | sed "s/^emudir=//g"`
  xsb_config_dir=`xsb --nobanner --quietload --noprompt \
                   -e "write('config_dir='), \
                     xsb_configuration(config_dir,X), write(X), nl, halt." \
                       2>/dev/null | sed "s/^config_dir=//g"`
  XSB_PROLOG_INCLUDE_OPTIONS="-I${xsb_emu_dir} -I${xsb_config_dir}"
  AC_SUBST(XSB_PROLOG_INCLUDE_OPTIONS)
fi

if test x$xsb_prolog != x
then
  have_xsb_prolog=yes
else
  have_xsb_prolog=no
fi
])
