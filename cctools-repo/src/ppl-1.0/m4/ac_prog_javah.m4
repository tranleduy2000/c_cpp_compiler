##### http://autoconf-archive.cryp.to/ac_prog_javah.html
#
# SYNOPSIS
#
#   AC_PROG_JAVAH
#
# DESCRIPTION
#
#   AC_PROG_JAVAH tests the availability of the javah header generator
#   and looks for the jni.h header file. If available, JAVAH is set to
#   the full path of javah and CPPFLAGS is updated accordingly.
#
# LAST MODIFICATION
#
#   2006-12-20
#
# COPYLEFT
#
#   Copyright (C) 2002      Luc Maisonobe <luc@spaceroots.org>
#   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
#   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AC_PROG_JAVAH],[
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_LANG_PUSH(C++)dnl
if test "x$JAVAPREFIX" = x; then
        AC_CHECK_PROGS(JAVAH,javah, no)
else
	AC_PATH_PROGS(JAVAH,javah, no, $JAVAPREFIX)
fi

if test x"`eval 'echo $ac_cv_path_JAVAH'`" != xno ; then
  AC_PREPROC_IFELSE([AC_LANG_SOURCE([[#include <jni.h>]]) ] ,,[
    ac_save_CPPFLAGS="$CPPFLAGS"
changequote(, )dnl
    ac_dir=`echo $ac_cv_path_JAVAH | sed 's,\(.*\)/[^/]*/[^/]*$,\1/include,'`
    ac_machdep=`echo $build_os | sed 's,[-0-9].*,,' | sed 's,cygwin,win32,'`
changequote([, ])dnl
    JNIFLAGS="-I$ac_dir -I$ac_dir/$ac_machdep"
    CPPFLAGS="$ac_save_CPPFLAGS $JNIFLAGS"
    AC_SUBST(JNIFLAGS)
    AC_PREPROC_IFELSE([AC_LANG_SOURCE([[#include <jni.h>]])],
               CPPFLAGS="$ac_save_CPPFLAGS",
               AC_MSG_WARN([unable to include <jni.h>])
	       JAVAH=no)
    CPPFLAGS="$ac_save_CPPFLAGS"])
fi
AC_LANG_POP(C++)
])
