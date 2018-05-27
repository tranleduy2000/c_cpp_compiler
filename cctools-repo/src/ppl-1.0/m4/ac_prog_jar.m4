##### http://autoconf-archive.cryp.to/ac_prog_jar.html
#
# SYNOPSIS
#
#   AC_PROG_JAR
#
# DESCRIPTION
#
#   AC_PROG_JAR tests for an existing jar program. It uses the
#   environment variable JAR then tests in sequence various common jar
#   programs.
#
#   If you want to force a specific compiler:
#
#   - at the configure.in level, set JAR=yourcompiler before calling
#   AC_PROG_JAR
#
#   - at the configure level, setenv JAR
#
#   You can use the JAR variable in your Makefile.in, with @JAR@.
#
#   Note: This macro depends on the autoconf M4 macros for Java
#   programs. It is VERY IMPORTANT that you download that whole set,
#   some macros depend on other. Unfortunately, the autoconf archive
#   does not support the concept of set of macros, so I had to break it
#   for submission.
#
#   The general documentation of those macros, as well as the sample
#   configure.in, is included in the AC_PROG_JAVA macro.
#
# LAST MODIFICATION
#
#   2006-11-06
#
# COPYLEFT
#
#   Copyright (C) 2000      Egon Willighagen <e.willighagen@science.ru.nl>
#   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
#   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AC_PROG_JAR],[
AC_REQUIRE([AC_EXEEXT])dnl
if test "x$JAVAPREFIX" = x; then
        test "x$JAR" = x && AC_CHECK_PROGS(JAR, jar$EXEEXT, no)
else
        test "x$JAR" = x && AC_PATH_PROGS(JAR, jar, no, $JAVAPREFIX)
fi
test "x$JAR" = xno
AC_PROVIDE([$0])dnl
])
