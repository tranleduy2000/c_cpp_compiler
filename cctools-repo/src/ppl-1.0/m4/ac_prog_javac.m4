dnl A function to check whether the Java compiler supports enums.
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

AC_DEFUN([AC_JAVAC_SUPPORTS_ENUMS],[
AC_CACHE_CHECK([whether $JAVAC supports enums], ac_cv_javac_supports_enums, [
JAVA_TEST=Test.java
CLASS_TEST=Test.class
cat << \EOF > $JAVA_TEST
/* [#]line __oline__ "configure" */
public class Test {
public enum Relation_Symbol {
    /*! Less than. */
    LESS_THAN,
    /*! Less than or equal to. */
    LESS_OR_EQUAL,
    /*! Equal to. */
    EQUAL,
    /*! Greater than or equal to. */
    GREATER_OR_EQUAL,
    /*! Greater than. */
    GREATER_THAN,
 }
}
EOF
if AC_TRY_COMMAND($JAVAC $JAVACFLAGS $JAVA_TEST) >/dev/null 2>&1; then
  ac_cv_javac_supports_enums=yes
else
  echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
  cat $JAVA_TEST >&AS_MESSAGE_LOG_FD
  ac_cv_javac_supports_enums=no
fi

rm -f $JAVA_TEST $CLASS_TEST Test\$Relation_Symbol.class
])
AC_PROVIDE([$0])dnl
])

##### http://autoconf-archive.cryp.to/ac_prog_javac.html
#
# SYNOPSIS
#
#   AC_PROG_JAVAC
#
# DESCRIPTION
#
#   AC_PROG_JAVAC tests an existing Java compiler. It uses the
#   environment variable JAVAC then tests in sequence various common
#   Java compilers. For political reasons, it starts with the free
#   ones.
#
#   If you want to force a specific compiler:
#
#   - at the configure.in level, set JAVAC=yourcompiler before calling
#   AC_PROG_JAVAC
#
#   - at the configure level, setenv JAVAC
#
#   You can use the JAVAC variable in your Makefile.in, with @JAVAC@.
#
#   *Warning*: its success or failure can depend on a proper setting of
#   the CLASSPATH env. variable.
#
#   TODO: allow to exclude compilers (rationale: most Java programs
#   cannot compile with some compilers like guavac).
#
#   Note: This is part of the set of autoconf M4 macros for Java
#   programs. It is VERY IMPORTANT that you download the whole set,
#   some macros depend on other. Unfortunately, the autoconf archive
#   does not support the concept of set of macros, so I had to break it
#   for submission. The general documentation, as well as the sample
#   configure.in, is included in the AC_PROG_JAVA macro.
#
# LAST MODIFICATION
#
#   2006-11-07
#
# COPYLEFT
#
#   Copyright (c) 2000      Stephane Bortzmeyer <bortzmeyer@pasteur.fr>
#   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
#   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#   General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
#   02111-1307, USA.
#
#   As a special exception, the respective Autoconf Macro's copyright
#   owner gives unlimited permission to copy, distribute and modify the
#   configure scripts that are the output of Autoconf when processing
#   the Macro. You need not follow the terms of the GNU General Public
#   License when using or distributing such scripts, even though
#   portions of the text of the Macro appear in them. The GNU General
#   Public License (GPL) does govern all other use of the material that
#   constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the
#   Autoconf Macro released by the Autoconf Macro Archive. When you
#   make and distribute a modified version of the Autoconf Macro, you
#   may extend this special exception to the GPL to apply to your
#   modified version as well.
AC_DEFUN([AC_PROG_JAVAC],[
AC_REQUIRE([AC_EXEEXT])dnl
if test "x$JAVAPREFIX" = x
then
        test "x$JAVAC" = x && AC_CHECK_PROGS(JAVAC, javac$EXEEXT, no)
else
        test "x$JAVAC" = x && AC_PATH_PROGS(JAVAC, javac$EXEEXT, no, $JAVAPREFIX)
fi
if test ! x$JAVAC = "xno"
then
 	AC_JAVAC_SUPPORTS_ENUMS
fi
AC_PROVIDE([$0])dnl
])
