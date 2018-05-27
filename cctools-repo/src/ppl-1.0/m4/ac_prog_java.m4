#### http://autoconf-archive.cryp.to/ac_prog_java_works.html
#
# SYNOPSIS
#
#   AC_PROG_JAVA_WORKS
#
# DESCRIPTION
#
#   Internal use ONLY.
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
#   2006-11-15
#
# COPYLEFT
#
#   Copyright (C) 2000      Stephane Bortzmeyer <bortzmeyer@pasteur.fr>
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
AC_DEFUN([AC_PROG_JAVA_WORKS], [
AC_CHECK_PROG(uudecode, uudecode$EXEEXT, yes)
if test x$uudecode = xyes; then
AC_CACHE_CHECK([if uudecode can decode base 64 file], ac_cv_prog_uudecode_base64, [
dnl /**
dnl  * Test.java: used to test if java compiler works.
dnl  */
dnl public class Test
dnl {
dnl
dnl public static void
dnl main( String[] argv )
dnl {
dnl     System.exit (0);
dnl }
dnl
dnl }
cat << \EOF > Test.uue
begin-base64 644 Test.class
yv66vgADAC0AFQcAAgEABFRlc3QHAAQBABBqYXZhL2xhbmcvT2JqZWN0AQAE
bWFpbgEAFihbTGphdmEvbGFuZy9TdHJpbmc7KVYBAARDb2RlAQAPTGluZU51
bWJlclRhYmxlDAAKAAsBAARleGl0AQAEKEkpVgoADQAJBwAOAQAQamF2YS9s
YW5nL1N5c3RlbQEABjxpbml0PgEAAygpVgwADwAQCgADABEBAApTb3VyY2VG
aWxlAQAJVGVzdC5qYXZhACEAAQADAAAAAAACAAkABQAGAAEABwAAACEAAQAB
AAAABQO4AAyxAAAAAQAIAAAACgACAAAACgAEAAsAAQAPABAAAQAHAAAAIQAB
AAEAAAAFKrcAErEAAAABAAgAAAAKAAIAAAAEAAQABAABABMAAAACABQ=
====
EOF
if uudecode$EXEEXT Test.uue; then
        ac_cv_prog_uudecode_base64=yes
else
        echo "configure: __oline__: uudecode had trouble decoding base 64 file 'Test.uue'" >&AS_MESSAGE_LOG_FD
        echo "configure: failed file was:" >&AS_MESSAGE_LOG_FD
        cat Test.uue >&AS_MESSAGE_LOG_FD
        ac_cv_prog_uudecode_base64=no
fi
rm -f Test.uue])
fi
if test x$ac_cv_prog_uudecode_base64 != xyes; then
        rm -f Test.class
        if test x$ac_cv_javac_supports_enums = x; then
                AC_PROG_JAVAC
        fi
fi
AC_CACHE_CHECK(if $JAVA works, ac_cv_prog_java_works, [
JAVA_TEST=Test.java
CLASS_TEST=Test.class
TEST=Test
changequote(, )dnl
cat << \EOF > $JAVA_TEST
/* [#]line __oline__ "configure" */
public class Test {
public static void main (String args[]) {
        System.exit (0);
} }
EOF
changequote([, ])dnl
if test x$ac_cv_prog_uudecode_base64 != xyes; then
        if AC_TRY_COMMAND($JAVAC $JAVACFLAGS $JAVA_TEST) && test -s $CLASS_TEST; then
                :
        else
          echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
          cat $JAVA_TEST >&AS_MESSAGE_LOG_FD
          AC_MSG_WARN(The Java compiler $JAVAC failed (see config.log, check the CLASSPATH?))
        fi
fi
# if we don't have a Java compiler installed, it's useless to check if Java
# works beacause a working javac is needed.
if test x$ac_cv_javac_supports_enums = xno; then
 ac_cv_prog_java_works=skipping
else
if AC_TRY_COMMAND($JAVA $JAVAFLAGS $TEST) >/dev/null 2>&1; then
  ac_cv_prog_java_works=yes
else
  echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
  cat $JAVA_TEST >&AS_MESSAGE_LOG_FD
  AC_MSG_WARN(The Java VM $JAVA failed (see config.log, check the CLASSPATH?))
  fi
fi
rm -fr $JAVA_TEST $CLASS_TEST Test.uue
])
AC_PROVIDE([$0])dnl
]
)


##### http://autoconf-archive.cryp.to/ac_prog_java.html
#
# SYNOPSIS
#
#   AC_PROG_JAVA
#
# DESCRIPTION
#
#   Here is a summary of the main macros:
#
#   AC_PROG_JAVAC: finds a Java compiler.
#
#   AC_PROG_JAVA: finds a Java virtual machine.
#
#   AC_CHECK_CLASS: finds if we have the given class (beware of
#   CLASSPATH!).
#
#   AC_CHECK_RQRD_CLASS: finds if we have the given class and stops
#   otherwise.
#
#   AC_TRY_COMPILE_JAVA: attempt to compile user given source.
#
#   AC_TRY_RUN_JAVA: attempt to compile and run user given source.
#
#   AC_JAVA_OPTIONS: adds Java configure options.
#
#   AC_PROG_JAVA tests an existing Java virtual machine. It uses the
#   environment variable JAVA then tests in sequence various common
#   Java virtual machines. For political reasons, it starts with the
#   free ones. You *must* call [AC_PROG_JAVAC] before.
#
#   If you want to force a specific VM:
#
#   - at the configure.in level, set JAVA=yourvm before calling
#   AC_PROG_JAVA
#
#     (but after AC_INIT)
#
#   - at the configure level, setenv JAVA
#
#   You can use the JAVA variable in your Makefile.in, with @JAVA@.
#
#   *Warning*: its success or failure can depend on a proper setting of
#   the CLASSPATH env. variable.
#
#   TODO: allow to exclude virtual machines (rationale: most Java
#   programs cannot run with some VM like kaffe).
#
#   Note: This is part of the set of autoconf M4 macros for Java
#   programs. It is VERY IMPORTANT that you download the whole set,
#   some macros depend on other. Unfortunately, the autoconf archive
#   does not support the concept of set of macros, so I had to break it
#   for submission.
#
#   A Web page, with a link to the latest CVS snapshot is at
#   <http://www.internatif.org/bortzmeyer/autoconf-Java/>.
#
#   This is a sample configure.in Process this file with autoconf to
#   produce a configure script.
#
#      AC_INIT(UnTag.java)
#
#      dnl Checks for programs.
#      AC_CHECK_CLASSPATH
#      AC_PROG_JAVAC
#      AC_PROG_JAVA
#
#      dnl Checks for classes
#      AC_CHECK_RQRD_CLASS(org.xml.sax.Parser)
#      AC_CHECK_RQRD_CLASS(com.jclark.xml.sax.Driver)
#
#      AC_OUTPUT(Makefile)
#
# LAST MODIFICATION
#
#   2006-11-06
#
# COPYLEFT
#
#   Copyright (c) 2000      Stephane Bortzmeyer <bortzmeyer@pasteur.fr>
#   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
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


AC_DEFUN([AC_PROG_JAVA],[
AC_REQUIRE([AC_EXEEXT])dnl
if test x$JAVAPREFIX = x; then
        test x$JAVA = x && AC_CHECK_PROGS(JAVA, java$EXEEXT, no)
else
        test x$JAVA = x && AC_PATH_PROGS(JAVA, java$EXEEXT, no, $JAVAPREFIX)
fi
if test ! x$JAVA = "xno"
then
 	AC_PROG_JAVA_WORKS
fi
AC_PROVIDE([$0])dnl
])
