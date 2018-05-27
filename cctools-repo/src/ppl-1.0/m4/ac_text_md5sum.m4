dnl A function to set the command for computing the MD5 checksum of text files.
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

AC_DEFUN([AC_TEXT_MD5SUM],
[
AC_MSG_CHECKING([for the text md5sum command])
if echo a | (md5sum -t) >/dev/null 2>&1
then
  ac_cv_prog_text_md5sum='md5sum -t'
else
  ac_cv_prog_text_md5sum='md5sum'
fi
AC_MSG_RESULT($ac_cv_prog_text_md5sum)
TEXT_MD5SUM=$ac_cv_prog_text_md5sum
AC_SUBST([TEXT_MD5SUM])
])


