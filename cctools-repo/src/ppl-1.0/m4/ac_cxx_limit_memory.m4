dnl A function to check whether it is possible to limit the memory
dnl available to processes using setrlimit().
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

AC_DEFUN([AC_CXX_SUPPORTS_LIMITING_MEMORY],
[
ac_save_CPPFLAGS="$CPPFLAGS"
ac_save_LIBS="$LIBS"
AC_LANG_PUSH(C++)

AC_MSG_CHECKING([whether we can limit memory in C++ using setrlimit()])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#if !HAVE_DECL_SETRLIMIT
# error "cannot limit memory without setrlimit()"
#endif

#include <stdexcept>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
// This should be included after <time.h> and <sys/time.h> so as to make
// sure we have the definitions for, e.g., ru_utime.
# include <sys/resource.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#define LIMIT(WHAT) \
  do {									\
    if (getrlimit(WHAT, &t) != 0) 					\
      return false;                                                     \
    t.rlim_cur = bytes;							\
    if (setrlimit(WHAT, &t) != 0)					\
      return false;                                                     \
  } while (0)

bool
limit_memory(unsigned long bytes) {
  struct rlimit t;
#if HAVE_DECL_RLIMIT_DATA
  // Limit heap size.
  LIMIT(RLIMIT_DATA);
#endif
#if HAVE_DECL_RLIMIT_RSS
  // Limit resident set size.
  LIMIT(RLIMIT_RSS);
#endif
#if HAVE_DECL_RLIMIT_VMEM
  // Limit mapped memory (brk + mmap).
  LIMIT(RLIMIT_VMEM);
#endif
#if HAVE_DECL_RLIMIT_AS
  // Limit virtual memory.
  LIMIT(RLIMIT_AS);
#endif
  return true;
}

int
main() try {
  if (!limit_memory(10000))
    return 1;
  (void) new char[20000];
  return 1;
 }
 catch (std::bad_alloc) {
   return 0;
 }
 catch (...) {
   return 1;
 }
]])],
  AC_MSG_RESULT(yes)
  ac_cxx_supports_limiting_memory=yes,
  AC_MSG_RESULT(no)
  ac_cxx_supports_limiting_memory=no,
  AC_MSG_RESULT([assuming not])
  ac_cxx_supports_limiting_memory=no)

if test x"$ac_cxx_supports_limiting_memory" = xyes
then
  value=1
else
  value=0
fi
AC_DEFINE_UNQUOTED(PPL_CXX_SUPPORTS_LIMITING_MEMORY, $value,
  [Not zero if it is possible to limit memory using setrlimit().])

AC_LANG_POP(C++)
CPPFLAGS="$ac_save_CPPFLAGS"
LIBS="$ac_save_LIBS"
])
