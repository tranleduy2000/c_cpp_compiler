# Autoconf source file for the Parma Polyhedra Library.
# Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
# Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)
#
# This file is part of the Parma Polyhedra Library (PPL).
#
# The PPL is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 3 of the License, or (at your
# option) any later version.
#
# The PPL is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
#
# For the most up-to-date information see the Parma Polyhedra Library
# site: http://bugseng.com/products/ppl/ .

# Process this file with Autoconf to produce a configure script.

# Every other copy of the package version number gets its value from here.
AC_INIT([the Parma Polyhedra Library], [1.1], [ppl-devel@cs.unipr.it], [ppl])

# Minimum Autoconf version required.
AC_PREREQ(2.61)

# Make sure the sources are there.
AC_CONFIG_SRCDIR(src/Polyhedron_public.cc)

# Specify the location of additional local Autoconf macros.
AC_CONFIG_MACRO_DIR([m4])

# Determine the system type and set output variables to the names of
# the canonical system types.
AC_CANONICAL_HOST

# Use Automake.
AM_INIT_AUTOMAKE([foreign no-define dist-bzip2 dist-zip dist-xz tar-ustar silent-rules 1.11])

AC_SUBST(VERSION)

# Version number machinery.
changequote(<<, >>)dnl
if test -n "`expr $VERSION : '\([0-9]*\)\.[0-9]*\.[0-9]*'`"
then
  PPL_VERSION_MAJOR=`expr $VERSION : '\([0-9]*\)\.[0-9]*\.[0-9]*'`
  PPL_VERSION_MINOR=`expr $VERSION : '[0-9]*\.\([0-9]*\)\.[0-9]*'`
  PPL_VERSION_REVISION=`expr $VERSION : '[0-9]*\.[0-9]*\.\([0-9]*\)'`
  PPL_VERSION_BETA=`expr $VERSION : '[0-9]*\.[0-9]*\.[0-9]*pre\([0-9]*\)'`
else
  PPL_VERSION_MAJOR=`expr $VERSION : '\([0-9]*\)\.[0-9]*'`
  PPL_VERSION_MINOR=`expr $VERSION : '[0-9]*\.\([0-9]*\)'`
  PPL_VERSION_REVISION=0
  PPL_VERSION_BETA=`expr $VERSION : '[0-9]*\.[0-9]*pre\([0-9]*\)'`
fi
if test -z "$PPL_VERSION_BETA"
then
  PPL_VERSION_BETA=0
fi
changequote([, ])dnl
AC_SUBST(PPL_VERSION_MAJOR)
AC_SUBST(PPL_VERSION_MINOR)
AC_SUBST(PPL_VERSION_REVISION)
AC_SUBST(PPL_VERSION_BETA)

AC_DEFINE_UNQUOTED(PPL_CONFIGURE_OPTIONS, "$ac_configure_args", [This contains the options with which `configure' was invoked.])
PPL_CONFIGURE_OPTIONS="\"$ac_configure_args\""
AC_SUBST(CONFIGURE_OPTIONS)

# Generate a configuration header file.
AC_CONFIG_HEADER([config.h])

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

AH_TOP([
/* BEGIN ppl-config.h */

#ifndef PPL_ppl_config_h
#define PPL_ppl_config_h 1

/* Unique (nonzero) code for the IEEE 754 Single Precision
   floating point format.  */
#define PPL_FLOAT_IEEE754_SINGLE 1

/* Unique (nonzero) code for the IEEE 754 Double Precision
   floating point format.  */
#define PPL_FLOAT_IEEE754_DOUBLE 2

/* Unique (nonzero) code for the IEEE 754 Quad Precision
   floating point format.  */
#define PPL_FLOAT_IEEE754_QUAD 3

/* Unique (nonzero) code for the Intel Double-Extended
   floating point format.  */
#define PPL_FLOAT_INTEL_DOUBLE_EXTENDED 4
])

AH_BOTTOM([
#if defined(PPL_NDEBUG) && !defined(NDEBUG)
# define NDEBUG PPL_NDEBUG
#endif

/* In order for the definition of `int64_t' to be seen by Comeau C/C++,
   we must make sure <stdint.h> is included before <sys/types.hh> is
   (even indirectly) included.  Moreover we need to define
   __STDC_LIMIT_MACROS before the first inclusion of <stdint.h>
   in order to have the macros defined also in C++.  */

#ifdef PPL_HAVE_STDINT_H
# ifndef __STDC_LIMIT_MACROS
#  define __STDC_LIMIT_MACROS 1
# endif
# include <stdint.h>
#endif

#ifdef PPL_HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#define PPL_U(x) x

#endif /* !defined(PPL_ppl_config_h) */

/* END ppl-config.h */
])


# C compiler.

AC_ARG_WITH(cc,
  AS_HELP_STRING([--with-cc=XXX], [use XXX as the C compiler]),
  CC=$with_cc)

# C++ compiler.

AC_ARG_WITH(cxx,
  AS_HELP_STRING([--with-cxx=XXX], [use XXX as the C++ compiler]),
  CXX=$with_cxx)

# Checks for programs.

# Note that AC_PROG_CC must precede the first use of $GCC and AC_PROG_CXX
# must precede the first use of $GXX.  Note also that we do not allow
# AC_PROG_CC and AC_PROG_CXX to affect CFLAGS and CXXFLAGS.
save_CFLAGS="$CFLAGS"
AC_PROG_CC
CFLAGS="$save_CFLAGS"

AM_CONDITIONAL(GCC, test x"$GCC" = xyes)

# The Intel C compiler masquerades as gcc, but we want to know.
if test x"$GCC" = xyes
then
  AC_MSG_CHECKING([whether we are actually using the Intel C compiler])
  AC_LANG_PUSH(C)
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifndef __INTEL_COMPILER
choke me
#endif
]])],
  AC_MSG_RESULT(yes)
  ICC=yes,
  AC_MSG_RESULT(no)
  ICC=no,
  AC_MSG_RESULT(no)
  ICC=no)
  AC_LANG_POP(C)
fi

AM_CONDITIONAL(ICC, test x"$ICC" = xyes)

# The clang compiler masquerades as gcc, but we want to know.
if test x"$GCC" = xyes
then
  AC_MSG_CHECKING([whether we are actually using clang])
  AC_LANG_PUSH(C)
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifndef __clang__
choke me
#endif
]])],
  AC_MSG_RESULT(yes)
  CLANG=yes,
  AC_MSG_RESULT(no)
  CLANG=no,
  AC_MSG_RESULT(no)
  CLANG=no)
  AC_LANG_POP(C)
fi

# The llvm-gcc compiler masquerades as gcc, but we want to know.
if test x"$GCC" = xyes
then
  AC_MSG_CHECKING([whether we are actually using llvm-gcc])
  AC_LANG_PUSH(C)
  save_CFLAGS="$CFLAGS"
  CFLAGS="-fplugin-arg-dragonegg-emit-ir -S"
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
int main() {
  return 0;
}
]])],
  AC_MSG_RESULT(yes)
  LLVM_GCC=yes,
  AC_MSG_RESULT(no)
  LLVM_GCC=no,
  AC_MSG_RESULT(no)
  LLVM_GCC=no)
  CFLAGS="$save_CFLAGS"
  AC_LANG_POP(C)
fi

save_CXXFLAGS="$CXXFLAGS"
AC_PROG_CXX
CXXFLAGS="$save_CXXFLAGS"

# The Intel C++ compiler masquerades as g++, but we want to know.
if test x"$GXX" = xyes
then
  AC_MSG_CHECKING([whether we are actually using the Intel C++ compiler])
  AC_LANG_PUSH(C++)
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifndef __INTEL_COMPILER
choke me
#endif
]])],
  AC_MSG_RESULT(yes)
  ICPC=yes,
  AC_MSG_RESULT(no)
  ICPC=no,
  AC_MSG_RESULT(no)
  ICPC=no)
  AC_LANG_POP(C++)
fi

# The clang++ compiler masquerades as g++, but we want to know.
if test x"$GXX" = xyes
then
  AC_MSG_CHECKING([whether we are actually using clang++])
  AC_LANG_PUSH(C++)
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifndef __clang__
choke me
#endif
]])],
  AC_MSG_RESULT(yes)
  CLANGXX=yes,
  AC_MSG_RESULT(no)
  CLANGXX=no,
  AC_MSG_RESULT(no)
  CLANGXX=no)
  AC_LANG_POP(C++)
fi

# The llvm-++ compiler masquerades as g++, but we want to know.
if test x"$GXX" = xyes
then
  AC_MSG_CHECKING([whether we are actually using llvm-g++])
  AC_LANG_PUSH(C++)
  save_CXXFLAGS="$CXXFLAGS"
  CXXFLAGS="-fplugin-arg-dragonegg-emit-ir -S"
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
int main() {
  return 0;
}
]])],
  AC_MSG_RESULT(yes)
  LLVM_GXX=yes,
  AC_MSG_RESULT(no)
  LLVM_GXX=no,
  AC_MSG_RESULT(no)
  LLVM_GXX=no)
  CXXFLAGS="$save_CXXFLAGS"
  AC_LANG_POP(C++)
fi


AC_PROG_FGREP
AC_PROG_EGREP
AC_PROG_SED
AC_PROG_CXXCPP
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# Compilation flags.

COMP_FLAGS=""
OPT_FLAGS=""

enableval=yes
AC_MSG_CHECKING([whether to compile with debug info])
AC_ARG_ENABLE(debugging,
  AS_HELP_STRING([--enable-debugging], [compile with debugging information]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  OPT_FLAGS="-g"
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-debugging, needs yes or no])
  ;;
esac
AM_CONDITIONAL(DEBUGGING_ENABLED, test x"$enableval" = xyes)

enableval=no
AC_MSG_CHECKING([whether to compile for profiling])
AC_ARG_ENABLE(profiling,
  AS_HELP_STRING([--enable-profiling], [compile for profiling]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  OPT_FLAGS="-g"
  COMP_FLAGS="$COMP_FLAGS -pg -DPPL_PROFILING=1"
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-profiling, needs yes or no])
  ;;
esac

enableval=no
AC_MSG_CHECKING([whether to compile for test coverage])
AC_ARG_ENABLE(coverage,
  AS_HELP_STRING([--enable-coverage], [compile for test coverage]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  OPT_FLAGS="-g"
  COMP_FLAGS="$COMP_FLAGS -fprofile-arcs -ftest-coverage"
  extra_libraries="${extra_libraries} -lgcov"
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-coverage, needs yes or no])
  ;;
esac

enableval=no
AC_MSG_CHECKING([whether to enable checking of run-time assertions])
AC_ARG_ENABLE(assertions,
  AS_HELP_STRING([--enable-assertions], [check run-time assertions]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-assertions, needs yes or no])
  ;;
esac
enable_assertions=${enableval}

enableval=no
AC_MSG_CHECKING([whether to enable even more run-time assertions])
AC_ARG_ENABLE(more-assertions,
  AS_HELP_STRING([--enable-more-assertions],
                 [break the ABI to check even more run-time assertions]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-more-assertions, needs yes or no])
  ;;
esac
enable_more_assertions=${enableval}

if test x"$enable_more_assertions" = xyes
then
  enable_assertions=yes
  AC_DEFINE(PPL_ABI_BREAKING_EXTRA_DEBUG, 1, [ABI-breaking extra assertions are enabled when this is defined.])
  AC_DEFINE(_GLIBCXX_DEBUG, 1, [When defined and libstdc++ is used, it is used in debug mode.])
  AC_DEFINE(_GLIBCXX_DEBUG_PEDANTIC, 1, [When defined and libstdc++ is used, it is used in pedantic debug mode.])
fi

if test x"$enable_assertions" = xno
then
  AC_DEFINE(PPL_NDEBUG, 1, [Assertions are disabled when this is defined.])
  debug_flag="-DNDEBUG=1"
fi

AM_CONDITIONAL(ASSERTIONS_ENABLED, test x"$enable_assertions" = xyes)


arch=no
enableval=standard
AC_MSG_CHECKING([whether to enable optimizations])
AC_ARG_ENABLE(optimization,
  AS_HELP_STRING([--enable-optimization@<:@=LEVEL@:>@],
                 [enable compiler optimizations]))
case "${enableval}" in
sspeed)
  AC_MSG_RESULT(sspeed)
  OPT_FLAGS="$OPT_FLAGS -O3 -fomit-frame-pointer"
  arch=yes
  ;;
speed)
  AC_MSG_RESULT(speed)
  OPT_FLAGS="$OPT_FLAGS -O3"
  arch=yes
  ;;
size)
  AC_MSG_RESULT(size)
  OPT_FLAGS="$OPT_FLAGS -Os"
  arch=yes
  ;;
standard | yes)
  AC_MSG_RESULT(standard)
  OPT_FLAGS="$OPT_FLAGS -O2"
  ;;
mild)
  AC_MSG_RESULT(mild)
  OPT_FLAGS="$OPT_FLAGS -O1"
  ;;
zero)
  AC_MSG_RESULT(zero)
  OPT_FLAGS="$OPT_FLAGS -O0"
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-optimization, needs sspeed, speed, size, standard, mild, zero, yes or no])
  ;;
esac

enableval=$arch
AC_MSG_CHECKING([for which architecture to optimize])
AC_ARG_ENABLE(arch,
  AS_HELP_STRING([--enable-arch@<:@=ARCH@:>@],
                 [optimize for architecture ARCH]))
case "${enableval}" in
yes)
  m=`uname -m`
  case $m in
  i?86 | k6 | athlon)
    AC_MSG_RESULT($m)
    OPT_FLAGS="$OPT_FLAGS -march=$m"
    ;;
  *)
    AC_MSG_RESULT(default)
    ;;
  esac
  ;;
no)
  AC_MSG_RESULT(default)
  ;;
*)
  AC_MSG_RESULT($enableval)
  OPT_FLAGS="$OPT_FLAGS -march=$enableval"
  ;;
esac

enableval=default
use_fpmath=yes
fpmath_may_use_387=yes
fpmath_may_use_sse=yes
AC_MSG_CHECKING([whether to use (a specific) floating point arithmetic])
AC_ARG_ENABLE(fpmath,
  AS_HELP_STRING([--enable-fpmath=INSTRUCTION_SET],
                 [select floating point arithmetic]))
case "${enableval}" in
sse)
  AC_MSG_RESULT(sse)
  OPT_FLAGS="$OPT_FLAGS -msse -mfpmath=sse"
  # The SSE instruction set only supports single precision arithmetic:
  # double and extended precision arithmetic is still done using 387.
  ;;
sse2)
  AC_MSG_RESULT(sse2)
  OPT_FLAGS="$OPT_FLAGS -msse2 -mfpmath=sse"
  # SSE2 still does not support extended precision arithmetic.
  ;;
387)
  AC_MSG_RESULT(387)
  OPT_FLAGS="$OPT_FLAGS -mno-sse -mno-sse2 -mfpmath=387"
  # Note that the -mno-sse* and -mfpmath options are only guaranteed
  # to work with GCC.
  if test x"$GCC" = xyes
  then
    fpmath_may_use_sse=no
  fi
  ;;
sse+387)
  AC_MSG_RESULT(sse+387)
  OPT_FLAGS="$OPT_FLAGS -msse -mfpmath=sse,387"
  ;;
sse2+387)
  AC_MSG_RESULT(sse2+387)
  OPT_FLAGS="$OPT_FLAGS -msse2 -mfpmath=sse,387"
  ;;
default)
  AC_MSG_RESULT(default)
  ;;
no)
  AC_MSG_RESULT(no)
  use_fpmath=no
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-fpmath, needs sse, sse2, 387, sse+387, sse2+387, default or no])
  ;;
esac

if test x"$fpmath_may_use_387" = xyes
then
  AC_DEFINE(PPL_FPMATH_MAY_USE_387, 1, [Defined if floating point arithmetic may use the 387 unit.])
fi

if test x"$fpmath_may_use_sse" = xyes
then
  AC_DEFINE(PPL_FPMATH_MAY_USE_SSE, 1, [Defined if floating point arithmetic may use the SSE instruction set.])
fi

# Disable all transformations and optimizations that assume default
# floating point rounding behavior.
if test x"$GXX" = xyes
then
  if test x"$ICPC" = xyes
  then
    OPT_FLAGS="$OPT_FLAGS -fp-model strict -fp-speculation off"
  else
    # -frounding-math is not yet supported in clang++ or llvm-g++.
    if test x"$CLANGXX" = xyes || x"$LLVM_GXX" = xyes
    then
      if test x"$use_fpmath" = xyes
      then
        AC_MSG_WARN([CANNOT RELY ON DIRECTED ROUNDING:
*** DISABLED ALL ABSTRACTIONS BASED ON MACHINE FLOATING-POINT NUMBERS.
*** A compiler has been detected that does not provide support for
*** -frounding-math (or any other option with the same semantics).
*** Hence, we cannot rely on floating-point computations to happen
*** in agreement with the rounding direction(s) used by the PPL.])
        use_fpmath=no
      fi
    else
      OPT_FLAGS="$OPT_FLAGS -frounding-math"
    fi
  fi
fi

enableval=no
AC_MSG_CHECKING([whether to use precompiled headers])
AC_ARG_ENABLE(pch,
  AS_HELP_STRING([--enable-pch],
                 [use precompiled headers, if available]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  COMP_FLAGS="$COMP_FLAGS -Winvalid-pch"
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-pch, needs yes or no])
  ;;
esac
AM_CONDITIONAL(USE_PRECOMPILED_HEADERS, test x"$enableval" = xyes)

CFLAGS="$COMP_FLAGS $OPT_FLAGS $CFLAGS"
CXXFLAGS="$COMP_FLAGS $OPT_FLAGS $CXXFLAGS"

# System-dependent adjustments.
cygwin=no
mingw=no
darwin=no
solaris=no
x86_64=no
no_undefined=no

case "${host_cpu}" in
alpha*)
  if test x"$GCC" = xyes
  then
    CFLAGS="$CFLAGS -mfp-rounding-mode=d -mieee-with-inexact"
    CXXFLAGS="$CXXFLAGS -mfp-rounding-mode=d -mieee-with-inexact"
  else
    CFLAGS="$CFLAGS -fprm d -ieee_with_inexact"
    CXXFLAGS="$CXXFLAGS -fprm d -ieee_with_inexact"
  fi
  ;;
x86_64*)
  x86_64=yes
  ;;
*)
  ;;
esac

case "${host_os}" in
cygwin*)
  cygwin=yes
  no_undefined=yes
  shared_library_path_env_var=PATH
  ;;
darwin*)
  darwin=yes
  shared_library_path_env_var=DYLD_LIBRARY_PATH
  ;;
hpux*)
  hpux=yes
  shared_library_path_env_var=SHLIB_PATH
  ;;
mingw*)
  mingw=yes
  no_undefined=yes
  shared_library_path_env_var=PATH
  ;;
solaris*)
  solaris=yes
  shared_library_path_env_var=LD_LIBRARY_PATH
  ;;
*)
  shared_library_path_env_var=LD_LIBRARY_PATH
  ;;
esac

AM_CONDITIONAL(HOST_OS_CYGWIN, test x$cygwin = xyes)
AM_CONDITIONAL(HOST_OS_DARWIN, test x$darwin = xyes)
AM_CONDITIONAL(HOST_OS_HPUX, test x$hpux = xyes)
AM_CONDITIONAL(HOST_OS_MINGW, test x$mingw = xyes)
AM_CONDITIONAL(HOST_OS_SOLARIS, test x$solaris = xyes)

AM_CONDITIONAL(HOST_CPU_X86_64, test x$x86_64 = xyes)

AM_CONDITIONAL(NO_UNDEFINED, test x$no_undefined = xyes)


enableval=mpz
AC_MSG_CHECKING([the type of integral values to use as coefficients])
AC_ARG_ENABLE(coefficients,
  AS_HELP_STRING([--enable-coefficients=TYPE],
                 [select the type of the coefficients]))
case "${enableval}" in
native-int8)
  AC_MSG_RESULT([native 8 bits integers])
  coefficient_kind=native
  coefficient_bits=8
  coefficient_mnemonic=nint8
  ;;
native-int16)
  AC_MSG_RESULT([native 16 bits integers])
  coefficient_kind=native
  coefficient_bits=16
  coefficient_mnemonic=nint16
  ;;
native-int32)
  AC_MSG_RESULT([native 32 bits integers])
  coefficient_kind=native
  coefficient_bits=32
  coefficient_mnemonic=nint32
  ;;
native-int64)
  AC_MSG_RESULT([native 64 bits integers])
  coefficient_kind=native
  coefficient_bits=64
  coefficient_mnemonic=nint64
  ;;
checked-int8)
  AC_MSG_RESULT([checked 8 bits integers])
  coefficient_kind=checked
  coefficient_bits=8
  coefficient_mnemonic=int8
  ;;
checked-int16)
  AC_MSG_RESULT([checked 16 bits integers])
  coefficient_kind=checked
  coefficient_bits=16
  coefficient_mnemonic=int16
  ;;
checked-int32)
  AC_MSG_RESULT([checked 32 bits integers])
  coefficient_kind=checked
  coefficient_bits=32
  coefficient_mnemonic=int32
  ;;
checked-int64)
  AC_MSG_RESULT([checked 64 bits integers])
  coefficient_kind=checked
  coefficient_bits=64
  coefficient_mnemonic=int64
  ;;
mpz)
  AC_MSG_RESULT([GMP mpz])
  coefficient_kind=unbounded
  coefficient_bits=0
  coefficient_mnemonic=mpz
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-coefficients, checked-int32, checked-int64, checked-int16, checked-int8, mpz, native-int32, native-int64, native-int16 or native-int8])
  ;;
esac

AM_CONDITIONAL(USE_NATIVE_INTEGERS, test x"$coefficient_kind" = xnative)
AM_CONDITIONAL(USE_CHECKED_INTEGERS, test x"$coefficient_kind" = xchecked)
AM_CONDITIONAL(USE_GMP_INTEGERS, test x"$coefficient_kind" = xunbounded)
AM_CONDITIONAL(USE_INT8, test x"$coefficient_bits" = x8)
AM_CONDITIONAL(USE_INT16, test x"$coefficient_bits" = x16)
AM_CONDITIONAL(USE_INT32, test x"$coefficient_bits" = x32)
AM_CONDITIONAL(USE_INT64, test x"$coefficient_bits" = x64)


if test x"$coefficient_kind" = xnative
then
  coefficient_type="Parma_Polyhedra_Library::Checked_Number<int${coefficient_bits}_t, Checked_Number_Transparent_Policy<int${coefficient_bits}_t> >"
  AC_DEFINE_UNQUOTED(PPL_COEFFICIENT_TYPE, $coefficient_type, [The integral type used to represent coefficients.])
  AC_DEFINE_UNQUOTED(PPL_COEFFICIENT_BITS, $coefficient_bits, [The number of bits of coefficients; 0 if unbounded.])
  AC_DEFINE(PPL_NATIVE_INTEGERS, 1, [Defined if the integral type to be used for coefficients is a native one.])
elif test x"$coefficient_kind" = xchecked
then
  coefficient_type="Parma_Polyhedra_Library::Checked_Number<int${coefficient_bits}_t, Bounded_Integer_Coefficient_Policy>"
  AC_DEFINE_UNQUOTED(PPL_COEFFICIENT_TYPE, $coefficient_type)
  AC_DEFINE_UNQUOTED(PPL_COEFFICIENT_BITS, $coefficient_bits)
  AC_DEFINE(PPL_CHECKED_INTEGERS, 1, [Defined if the integral type to be used for coefficients is a checked one.])
elif test x"$coefficient_kind" = xunbounded
then
  AC_DEFINE(PPL_COEFFICIENT_TYPE, mpz_class)
  AC_DEFINE(PPL_COEFFICIENT_BITS, 0)
  AC_DEFINE(PPL_GMP_INTEGERS, 1, [Defined if the integral type to be used for coefficients is GMP's one.])
fi

# Allow additions to C compilation flags.
AC_ARG_WITH(cflags,
  AS_HELP_STRING([--with-cflags=XXX],
                 [add XXX to the options for the C compiler]),
  CFLAGS="$CFLAGS $with_cflags")

# Allow additions to C++ compilation flags.
AC_ARG_WITH(cxxflags,
  AS_HELP_STRING([--with-cxxflags=XXX],
                 [add XXX to the options for the C++ compiler]),
  CXXFLAGS="$CXXFLAGS $with_cxxflags")

# Checks for C typedefs, structures, compiler and architecture characteristics.
AC_LANG(C)
AC_C_CONST
AC_C_INLINE
AC_C_BIGENDIAN
AC_C_TYPEOF

# Checks for C type sizes.
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(size_t)
AC_CHECK_SIZEOF(float)
AC_CHECK_SIZEOF(double)
AC_CHECK_SIZEOF(long double)
# The following are required to validate an horrible kludge working
# around an horrible bug in <gprolog.h> (see
# http://www.cs.unipr.it/pipermail/ppl-devel/2008-August/012277.html).
AC_CHECK_SIZEOF([int*])
AC_CHECK_SIZEOF([fp], [], [typedef int *(*fp)();])

# Check for Perl.
AC_PATH_PROG([PERL], perl, no)
AC_SUBST([PERL])
AM_CONDITIONAL(HAVE_PERL, test x"$PERL" != xno)

# Use C++ for the remaining checks.
AC_LANG(C++)

# The PPL uses <cmath> for sqrt and other functions in several places.
AC_SEARCH_LIBS([sqrt], [m])

if test x"$use_fpmath" = xyes
then
  # Check for the possibility to control the FPU.
  AC_CHECK_FPU_CONTROL
else
  ac_cv_can_control_fpu=0
fi
AM_CONDITIONAL(CAN_CONTROL_FPU, test $ac_cv_can_control_fpu = 1)
AC_DEFINE_UNQUOTED(PPL_CAN_CONTROL_FPU, $ac_cv_can_control_fpu,
    [Not zero if the FPU can be controlled.])

# Check whether for the C++ compiler the plain char type is signed.
AC_CXX_PLAIN_CHAR_IS_SIGNED

# Check whether the C++ compiler provides proper long doubles.
AC_CXX_PROVIDES_PROPER_LONG_DOUBLE

# Detect the binary format used by C++ floats.
AC_CXX_FLOAT_BINARY_FORMAT

# Detect the binary format used by C++ doubles.
AC_CXX_DOUBLE_BINARY_FORMAT

# Detect the binary format used by C++ long doubles.
AC_CXX_LONG_DOUBLE_BINARY_FORMAT

# Test if some floating-point type is supported.
if test $ac_supported_float = 1 \
|| test $ac_supported_double = 1 \
|| test $ac_supported_long_double = 1
then
  supported_some_floating_point_type=yes
fi
AM_CONDITIONAL(SUPPORTED_SOME_FLOATING_POINT_TYPE,
               test x$supported_some_floating_point_type = xyes)

# Check whether the C++ compiler supports zero-length arrays.
AC_CXX_SUPPORTS_ZERO_LENGTH_ARRAYS

# Check whether the IEEE inexact flag is supported in C++.
AC_CXX_SUPPORTS_IEEE_INEXACT_FLAG

# Check whether the C++ compiler supports __attribute__ ((weak)).
AC_CXX_SUPPORTS_ATTRIBUTE_WEAK

# Checks for header files.
AC_CHECK_HEADERS([fenv.h ieeefp.h getopt.h signal.h string.h strings.h sys/resource.h sys/time.h sys/types.h unistd.h])

# Checks for the availability of C library functions in C++.
AC_CHECK_DECLS([ffs],
               ,
               ,
               [
#if defined(HAVE_STRINGS_H)
# include <strings.h>
#elif defined(HAVE_STRING_H)
# include <string.h>
#endif
               ])
AC_CHECK_DECLS([getenv,strtof,strtod,strtold,strtoll,strtoull], , , [#include <cstdlib>])
AC_CHECK_DECLS([fma,fmaf,fmal,rintf,rintl], , , [#include <cmath>])

# Checks for the availability of "fast" integral types.
AC_CHECK_TYPES([int_fast16_t,int_fast32_t,int_fast64_t,uint_fast16_t,uint_fast32_t,uint_fast64_t])

# Make sure uintptr_t is defined to an unsigned integer type wide enough
# to hold a pointer, if such a type exists.
AC_TYPE_UINTPTR_T


# Checks on the GMP library.

AC_CHECK_GMP
if test x"$have_gmp" = xno
then
  AC_MSG_ERROR([Cannot find GMP version 4.1.3 or higher.
GMP is the GNU Multi-Precision library:
see http://www.swox.com/gmp/ for more information.
When compiling the GMP library, do not forget to enable the C++ interface:
add --enable-cxx to the configuration options.])
else
  if test x"$have_gmpxx" = xno
  then
    AC_MSG_ERROR([GMP compiled without enabling the C++ interface.
GMP is the GNU Multi-Precision library:
see http://www.swox.com/gmp/ for more information.
When compiling the GMP library, do not forget to enable the C++ interface:
add --enable-cxx to the configuration options.])
  fi
fi

AC_CHECK_MEMBERS([__mpz_struct._mp_alloc,
                  __mpz_struct._mp_size,
                  __mpz_struct._mp_d],
                 ,
                 gmp_has_changes=yes,
                 [#include <gmp.h>])

if test x"$gmp_has_changed" = xyes
then
  AC_MSG_ERROR([GMP HAS CHANGED:
*** The PPL exploits some implementation details of GMP that were current
*** until (at least) version 4.1.4 of GMP.  You seem to be using a version
*** where these details have changed.
*** Please report this to ppl-devel@cs.unipr.it.])
fi

extra_includes="${extra_includes}${extra_includes:+ }${gmp_include_options}"
extra_libraries="${extra_libraries}${extra_libraries:+ }${gmp_library_options}"

enableval=yes
AC_MSG_CHECKING([whether to build the ppl_lcdd program])
AC_ARG_ENABLE(ppl_lcdd,
  AS_HELP_STRING([--enable-ppl_lcdd], [build the ppl_lcdd program]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-ppl_lcdd, needs yes or no])
  ;;
esac
build_ppl_lcdd=$enableval
AM_CONDITIONAL(BUILD_PPL_LCDD, test x"$build_ppl_lcdd" = xyes)

enableval=yes
AC_MSG_CHECKING([whether to build the ppl_lpsol program])
AC_ARG_ENABLE(ppl_lpsol,
  AS_HELP_STRING([--enable-ppl_lpsol], [build the ppl_lpsol program]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-ppl_lpsol, needs yes or no])
  ;;
esac
build_ppl_lpsol=$enableval
AM_CONDITIONAL(BUILD_PPL_LPSOL, test x"$build_ppl_lpsol" = xyes)

enableval=yes
AC_MSG_CHECKING([whether to build the ppl_pips program])
AC_ARG_ENABLE(ppl_pips,
  AS_HELP_STRING([--enable-ppl_pips], [build the ppl_pips program]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-ppl_pips, needs yes or no])
  ;;
esac
build_ppl_pips=$enableval
AM_CONDITIONAL(BUILD_PPL_PIPS, test x"$build_ppl_pips" = xyes)

enableval=yes
AC_MSG_CHECKING([whether to build the PPL documentation])
AC_ARG_ENABLE(documentation,
  AS_HELP_STRING([--enable-documentation], [build the PPL documentation]))
case "${enableval}" in
yes)
  AC_MSG_RESULT(yes)
  ;;
no)
  AC_MSG_RESULT(no)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-documentation, needs yes or no])
  ;;
esac
build_documentation=$enableval
AM_CONDITIONAL(BUILD_DOCUMENTATION, test x"$build_documentation" = xyes)

# Define lists of available interfaces: lowercase and blank-separated.
non_prolog_interfaces="cxx c ocaml java"
prolog_interfaces="ciao_prolog gnu_prolog sicstus_prolog swi_prolog xsb_prolog yap_prolog"
available_interfaces="${non_prolog_interfaces} ${prolog_interfaces}"

# This is the list of interfaces that are enabled by default.
default_interfaces=${non_prolog_interfaces}

enableval=not_specified
AC_MSG_CHECKING([which interfaces are enabled])
AC_ARG_ENABLE(interfaces,
  AS_HELP_STRING([--enable-interfaces=INTERFACES],
                 [enable some or all the library interfaces]))
case x"${enableval}" in
xyes | x)
  AC_MSG_ERROR([--enable-interfaces needs at least one argument chosen among ${available_interfaces}, none and all])
  ;;
xnone | xno)
  for interface in ${available_interfaces}
  do
    eval ${interface}_interface_enabled=no
  done
  AC_MSG_RESULT(none)
  ;;
xall)
  for interface in ${available_interfaces}
  do
    eval ${interface}_interface_enabled=yes
  done
  AC_MSG_RESULT([${available_interfaces}])
  ;;
*)
  if test x"${enableval}" = xnot_specified
  then
    enableval=${default_interfaces}
  fi
  # Make the list blank-separated and lowercase; turn "c++" into "cxx".
  required_interfaces=`echo "${enableval}" | sed -e 's/-/_/g' -e 's/[[  ,]][[   ,]]*/ /g' -e 's/c++/cxx/g' | tr '[[:upper:]]' '[[:lower:]]'`
  # Check that the specified interfaces exist.
  for interface in ${required_interfaces}
  do
    case " ${available_interfaces} " in
    *\ ${interface}\ *)
      ;;
    *)
      AC_MSG_ERROR([unknown interface ${interface}: must be one of ${available_interfaces}])
      ;;
    esac
  done
  # Initialize the enabled variable for each interface.
  for interface in ${available_interfaces}
  do
    case " ${required_interfaces} " in
    *\ ${interface}\ *)
      eval ${interface}_interface_enabled=yes
      enabled_interfaces="${enabled_interfaces}${enabled_interfaces:+ }${interface}"
      ;;
    *)
      eval ${interface}_interface_enabled=no
      ;;
    esac
  done
  AC_MSG_RESULT([${enabled_interfaces}])
  ;;
esac

if test x${cxx_interface_enabled} = xyes
then
  build_cxx_interface=yes
fi
AM_CONDITIONAL(BUILD_CXX_INTERFACE, test x$build_cxx_interface = xyes)

if test x${c_interface_enabled} = xyes
then
  build_c_interface=yes
fi
AM_CONDITIONAL(BUILD_C_INTERFACE, test x$build_c_interface = xyes)

# Checks for systems for which the interface is enabled.

# Allow to specify the Java SDK installation directory.
AC_ARG_WITH(java,
  AS_HELP_STRING([--with-java=DIR], [use the Java SDK installed in DIR]),
  java_dir=$with_java)

case "${host_os}" in
# On Darwin the JDK, if present, is installed in /Library/Java/Home .
darwin*)
  if test "x$java_dir" = x
  then
    java_dir="/Library/Java/Home"
  fi
  ;;
# On modern Fedora systems the JDK is usually in /usr/lib/jvm/java .
linux*)
  if test "x$java_dir" = x
  then
    java_dir="/usr/lib/jvm/java"
  fi
  ;;
*)
  ;;
esac

if test "x$java_dir" != x
then
  JAVAPREFIX="${java_dir}/bin"
fi

# Checks for Java.
if test x${java_interface_enabled} = xyes
then
  # Check for Java programs. Order here is important: check first
  # for the compiler.
  AC_PROG_JAVAC
  AC_PROG_JAVA
  AC_PROG_JAR
  AC_PROG_JAVAH

  ac_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $JNIFLAGS"
  AC_LANG_PUSH(C++)
  AC_MSG_CHECKING([whether jlong can contain data pointers])
  AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <jni.h>

    int
    main() {
      if (sizeof(jlong) >= sizeof(void*))
        return 0;
      else
        return 1;
    }
  ]])],
  ac_jlong_can_contain_pointers=yes
  AC_MSG_RESULT(yes),
  ac_jlong_can_contain_pointers=no
  AC_MSG_RESULT(no),
  ac_jlong_can_contain_pointers=no
  AC_MSG_RESULT([assuming it cannot]))
  AC_LANG_POP(C++)
  CPPFLAGS="$ac_save_CPPFLAGS"
fi

# In order to build the Java interface, it must be enabled and all the
# Java tools must be available.
if test x${java_interface_enabled} = xyes \
&& test "x$JAVA" != xno \
&& test "x$ac_cv_javac_supports_enums" != xno \
&& test "x$JAR" != xno \
&& test "x$JAVAH" != xno \
&& test x${ac_jlong_can_contain_pointers} = xyes
then
  build_java_interface=yes
fi
AM_CONDITIONAL(BUILD_JAVA_INTERFACE, test x$build_java_interface = xyes)


# Allow to specify the ML GMP installation directory.
AC_ARG_WITH(mlgmp,
  AS_HELP_STRING([--with-mlgmp=DIR],
                 [use the ML GMP package installed in DIR]),
  mlgmp_dir=$with_mlgmp,
  mlgmp_dir=+gmp)

# Checks for OCaml.
if test x${ocaml_interface_enabled} = xyes
then
  # Detect which tools of the OCaml toolchain are available.
  AC_PROG_OCAML
  if test x"$OCAMLC" != xno
  then
    # Check for the bytecode version of ML GMP.
    AC_MSG_CHECKING([for ML GMP bytecode module gmp.cma])
    if ( test ${mlgmp_dir} = +gmp && test -f ${OCAMLLIB}/gmp/gmp.cma ) \
      || test -f ${mlgmp_dir}/gmp.cma;
    then
      AC_MSG_RESULT(yes)
      ocamlc_gmp="yes"
    else
      AC_MSG_RESULT(no)
      ocamlc_gmp="no"
    fi
    if test x"$OCAMLOPT" != xno
    then
      ocamlopt_root=`${OCAMLOPT} -where`
      # Check for the native version of ML GMP.
      AC_MSG_CHECKING([for ML GMP native module gmp.cmxa])
      if ( test ${mlgmp_dir} = +gmp && test -f ${ocamlopt_root}/gmp/gmp.cmxa ) \
        || test -f ${mlgmp_dir}/gmp.cmxa;
      then
        AC_MSG_RESULT(yes)
        ocamlopt_gmp="yes"
      else
        AC_MSG_RESULT(no)
        ocamlopt_gmp="no"
      fi
    fi
  fi
fi
AM_CONDITIONAL(HAVE_OCAMLC, test x$ocamlc_gmp = xyes)
AM_CONDITIONAL(HAVE_OCAMLOPT, test x$ocamlopt_gmp = xyes)

if test x${ocaml_interface_enabled} = xyes \
&& (test x$ocamlc_gmp = xyes || test x$ocamlopt_gmp = xyes)
then
  build_ocaml_interface=yes
fi
AM_CONDITIONAL(BUILD_OCAML_INTERFACE, test x$build_ocaml_interface = xyes)

# Checks for Ciao Prolog.
if test x${ciao_prolog_interface_enabled} = xyes
then
  AC_CHECK_CIAO_PROLOG
fi

if test x${ciao_prolog_interface_enabled} = xyes \
&& test x$have_ciao_prolog = xyes
then
  build_ciao_prolog_interface=yes
fi
AM_CONDITIONAL(BUILD_CIAO_PROLOG_INTERFACE,
               test x$build_ciao_prolog_interface = xyes)

# Checks for GNU Prolog.
if test x${gnu_prolog_interface_enabled} = xyes
then
  AC_CHECK_PROG(GNU_PROLOG, gprolog, gprolog)
  if test x"$GNU_PROLOG" = xgprolog
  then
    AC_CHECK_HEADER(gprolog.h,
                    [],
                    GNU_PROLOG="")
  fi
fi

if test x${gnu_prolog_interface_enabled} = xyes \
&& test x$GNU_PROLOG = xgprolog
then
  build_gnu_prolog_interface=yes
fi
AM_CONDITIONAL(BUILD_GNU_PROLOG_INTERFACE,
               test x$build_gnu_prolog_interface = xyes)

# Checks for SICStus.
if test x${sicstus_prolog_interface_enabled} = xyes
then
  AC_CHECK_SICSTUS_PROLOG
fi

if test x${sicstus_prolog_interface_enabled} = xyes \
&& test x$have_sicstus_prolog = xyes
then
  build_sicstus_prolog_interface=yes
fi
AM_CONDITIONAL(BUILD_SICSTUS_PROLOG_INTERFACE,
               test x$build_sicstus_prolog_interface = xyes)

# Checks for SWI-Prolog.
if test x${swi_prolog_interface_enabled} = xyes
then
  AC_CHECK_SWI_PROLOG
fi

# Under Cygwin, SWI-Prolog does not work with foreign code:
# see http://www.cs.unipr.it/pipermail/ppl-devel/2006-January/007838.html
if test x${swi_prolog_interface_enabled} = xyes \
&& test x$have_swi_prolog = xyes \
&& test x"${host_os}" != xcygwin
then
  build_swi_prolog_interface=yes
fi
AM_CONDITIONAL(BUILD_SWI_PROLOG_INTERFACE,
               test x$build_swi_prolog_interface = xyes)

# Checks for XSB.
if test x${xsb_prolog_interface_enabled} = xyes
then
  AC_CHECK_XSB_PROLOG
fi

if test x${xsb_prolog_interface_enabled} = xyes \
&& test x$have_xsb_prolog = xyes
then
  build_xsb_prolog_interface=yes
fi
AM_CONDITIONAL(BUILD_XSB_PROLOG_INTERFACE,
               test x$build_xsb_prolog_interface = xyes)

# Checks for YAP.
if test x${yap_prolog_interface_enabled} = xyes
then
  AC_CHECK_YAP_PROLOG
fi

if test x${yap_prolog_interface_enabled} = xyes \
&& test x$have_yap_prolog = xyes
then
  build_yap_prolog_interface=yes
fi
AM_CONDITIONAL(BUILD_YAP_PROLOG_INTERFACE,
               test x$build_yap_prolog_interface = xyes)

# Test if some Prolog interfaces have to be built.
if test x$build_ciao_prolog_interface = xyes \
|| test x$build_gnu_prolog_interface = xyes \
|| test x$build_sicstus_prolog_interface = xyes \
|| test x$build_swi_prolog_interface = xyes \
|| test x$build_xsb_prolog_interface = xyes \
|| test x$build_yap_prolog_interface = xyes
then
  build_some_prolog_interfaces=yes
fi
AM_CONDITIONAL(BUILD_SOME_PROLOG_INTERFACES,
               test x$build_some_prolog_interfaces = xyes)

if test x$build_c_interface = xyes \
|| test x$build_java_interface = xyes \
|| test x$build_ocaml_interface = xyes \
|| test x$build_some_prolog_interfaces = xyes
then
  # Look for a recent enough version of GNU M4.
  AC_PROG_GNU_M4
fi

# Define the list of instantiations that are enabled by default.
# Different instantiations are separated by a '@' character.
di="        Polyhedron @ Grid"
di="${di} @ Rational_Box"
di="${di} @ BD_Shape<mpz_class> @ BD_Shape<mpq_class>"
di="${di} @ Octagonal_Shape<mpz_class> @ Octagonal_Shape<mpq_class>"
di="${di} @ Constraints_Product<C_Polyhedron, Grid>"
di="${di} @ Pointset_Powerset<C_Polyhedron>"
di="${di} @ Pointset_Powerset<NNC_Polyhedron>"
if test $ac_supported_double = 1
then
  di="${di} @ Double_Box @ BD_Shape<double> @ Octagonal_Shape<double>"
fi
default_instantiations="${di}"

# Define the list of all instantiations.
# Different instantiations are separated by a '@' character.
ai="        Int8_Box @ Int16_Box @ Int32_Box @ Int64_Box"
ai="${ai} @ Uint8_Box @ Uint16_Box @ Uint32_Box @ Uint64_Box"
ai="${ai} @ Z_Box @ Rational_Box"
ai="${ai} @ BD_Shape<int8_t> @ BD_Shape<int16_t>"
ai="${ai} @ BD_Shape<int32_t> @ BD_Shape<int64_t>"
ai="${ai} @ BD_Shape<mpz_class> @ BD_Shape<mpq_class>"
ai="${ai} @ Octagonal_Shape<int8_t> @ Octagonal_Shape<int16_t>"
ai="${ai} @ Octagonal_Shape<int32_t> @ Octagonal_Shape<int64_t>"
ai="${ai} @ Octagonal_Shape<mpz_class> @ Octagonal_Shape<mpq_class>"
ai="${ai} @ Polyhedron @ Grid"
ai="${ai} @ Pointset_Powerset<Int8_Box> @ Pointset_Powerset<Int16_Box>"
ai="${ai} @ Pointset_Powerset<Int32_Box> @ Pointset_Powerset<Int64_Box>"
ai="${ai} @ Pointset_Powerset<Uint8_Box> @ Pointset_Powerset<Uint16_Box>"
ai="${ai} @ Pointset_Powerset<Uint32_Box> @ Pointset_Powerset<Uint64_Box>"
ai="${ai} @ Pointset_Powerset<Rational_Box> @ Pointset_Powerset<Z_Box>"
ai="${ai} @ Pointset_Powerset<BD_Shape<int8_t> >"
ai="${ai} @ Pointset_Powerset<BD_Shape<int16_t> >"
ai="${ai} @ Pointset_Powerset<BD_Shape<int32_t> >"
ai="${ai} @ Pointset_Powerset<BD_Shape<int64_t> >"
ai="${ai} @ Pointset_Powerset<BD_Shape<mpq_class> >"
ai="${ai} @ Pointset_Powerset<BD_Shape<mpz_class> >"
ai="${ai} @ Pointset_Powerset<Octagonal_Shape<int8_t> >"
ai="${ai} @ Pointset_Powerset<Octagonal_Shape<int16_t> >"
ai="${ai} @ Pointset_Powerset<Octagonal_Shape<int32_t> >"
ai="${ai} @ Pointset_Powerset<Octagonal_Shape<int64_t> >"
ai="${ai} @ Pointset_Powerset<Octagonal_Shape<mpq_class> >"
ai="${ai} @ Pointset_Powerset<Octagonal_Shape<mpz_class> >"
ai="${ai} @ Pointset_Powerset<C_Polyhedron>"
ai="${ai} @ Pointset_Powerset<NNC_Polyhedron>"
ai="${ai} @ Pointset_Powerset<Grid>"
ai="${ai} @ Constraints_Product<C_Polyhedron, Grid>"
ai="${ai} @ Constraints_Product<NNC_Polyhedron, Grid>"
ai="${ai} @ Constraints_Product<Grid, C_Polyhedron>"
ai="${ai} @ Constraints_Product<Grid, NNC_Polyhedron>"
if test $ac_supported_float = 1
then
  ai="${ai} @ Float_Box @ BD_Shape<float>"
  ai="${ai} @ Octagonal_Shape<float>"
  ai="${ai} @ Pointset_Powerset<Float_Box>"
  ai="${ai} @ Pointset_Powerset<BD_Shape<float> >"
  ai="${ai} @ Pointset_Powerset<Octagonal_Shape<float> >"
fi
if test $ac_supported_double = 1
then
  ai="${ai} @ Double_Box @ BD_Shape<double>"
  ai="${ai} @ Octagonal_Shape<double>"
  ai="${ai} @ Pointset_Powerset<Double_Box>"
  ai="${ai} @ Pointset_Powerset<BD_Shape<double> >"
  ai="${ai} @ Pointset_Powerset<Octagonal_Shape<double> >"
fi
if test $ac_supported_long_double = 1
then
  ai="${ai} @ Long_Double_Box @ BD_Shape<long double>"
  ai="${ai} @ Octagonal_Shape<long double>"
  ai="${ai} @ Pointset_Powerset<Long_Double_Box>"
  ai="${ai} @ Pointset_Powerset<BD_Shape<long double> >"
  ai="${ai} @ Pointset_Powerset<Octagonal_Shape<long double> >"
fi
available_instantiations=${ai}

enableval=not_specified
AC_MSG_CHECKING([which instantiations are enabled])
AC_ARG_ENABLE(instantiations,
  AS_HELP_STRING([--enable-instantiations=INSTANTIATIONS],
                 [enable instantiations for the non-C++ interfaces]))
case x"${enableval}" in
xyes | x)
  ai_one_per_line=`echo "${available_instantiations}" | sed -e 's/ *@ */\n/g' -e 's/ *\([[A-Z]]\)/\1/g'`
  AC_MSG_ERROR([
--enable-instantiations needs at least one argument chosen among none,
all, and an @-separated list of instantiation specifiers included
in the following list:
${ai_one_per_line}])
  ;;
xno | xnone)
  enableval=
  AC_MSG_RESULT(none)
  ;;
*)
  if test x"${enableval}" = xnot_specified
  then
    enableval=${default_instantiations}
  elif test x"${enableval}" = xall
  then
    enableval=${available_instantiations}
  fi
  # Avoid extra blanks.
  required_instantiations=`echo "${enableval}" | sed -e 's/[[    ]][[    ]]*/ /g' -e 's/[[ ]]*\([[@<>,]]\)/\1/g' -e 's/\([[@<>,]]\)[[ ]]*/\1/g' -e 's/>>/> >/g' -e 's/^[[ ]]//g' -e 's/[[ ]]$//g'`
  # Check the specified instantiations for validity.
  ac_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS -I${srcdir} ${gmp_include_options}"
  old_IFS="${IFS}"
  IFS=@
  set -- ${required_instantiations}
  IFS="${old_IFS}"
  for instantiation
  do
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include "instchk.hh"

using namespace Parma_Polyhedra_Library;

bool valid = ${instantiation}::valid_instantiation();
]])],
      valid_instantiation=yes,
      valid_instantiation=no)
    if test x${valid_instantiation} != xyes
    then
      AC_MSG_ERROR([invalid instantiation ${instantiation}])
    fi
  done
  CPPFLAGS="$ac_save_CPPFLAGS"
  required_instantiations_canonical_names=`echo "${required_instantiations}" | sed -e 's/> //g' -e 's/>//g' -e 's/[[< ,]]/_/g'`

  required_instantiations_c_source_names=`echo "${required_instantiations_canonical_names}" | sed -e 's/@Pointset_Powerset_Polyhedron/@Pointset_Powerset_C_Polyhedron@Pointset_Powerset_NNC_Polyhedron/g' -e 's/^Pointset_Powerset_Polyhedron/Pointset_Powerset_C_Polyhedron@Pointset_Powerset_NNC_Polyhedron/g'`
  required_instantiations_c_cxx_objects=`echo "${required_instantiations_c_source_names}" | sed -e 's/@/.lo /g' -e 's/$/.lo/g' | sed -e 's/\([[^. ]]*\)\.lo/ppl_c_\1.lo/g'`
  required_instantiations_c_cxx_sources=`echo "${required_instantiations_c_cxx_objects}" | sed -e 's/\([[^. ]]*\)\.lo/\1.cc/g'`
  required_instantiations_c_cxx_headers=`echo "${required_instantiations_c_cxx_sources}" | sed -e 's/\.cc/.hh/g'`

  required_instantiations_prolog_source_names=`echo "${required_instantiations_canonical_names}" | sed -e 's/@Pointset_Powerset_Polyhedron/@Pointset_Powerset_C_Polyhedron@Pointset_Powerset_NNC_Polyhedron/g' -e 's/^Pointset_Powerset_Polyhedron/Pointset_Powerset_C_Polyhedron@Pointset_Powerset_NNC_Polyhedron/g'`
  required_instantiations_prolog_cxx_objects=`echo "${required_instantiations_prolog_source_names}" | sed -e 's/@/.lo /g' -e 's/$/.lo/g' | sed -e 's/\([[^. ]]*\)\.lo/ppl_prolog_\1.lo/g'`
  required_instantiations_prolog_cxx_sources=`echo "${required_instantiations_prolog_cxx_objects}" | sed -e 's/\([[^. ]]*\)\.lo/\1.cc/g'`
  required_instantiations_prolog_cxx_headers=`echo "${required_instantiations_prolog_cxx_sources}" | sed -e 's/\.cc/.hh/g'`
  required_instantiations_prolog_generated_test_sources=`echo "${required_instantiations_prolog_source_names}" | sed -e 's/@/.pl /g' -e 's/$/.pl/g' | sed -e 's/\([[^. ]]*\)\.pl/ppl_prolog_generated_test_\1.pl/g'`

  required_instantiations_java_source_names=`echo "${required_instantiations_canonical_names}" | sed -e 's/^Polyhedron/Polyhedron@C_Polyhedron@NNC_Polyhedron/g' -e 's/@Polyhedron/@Polyhedron@C_Polyhedron@NNC_Polyhedron/g' -e 's/@Pointset_Powerset_Polyhedron/@Pointset_Powerset_C_Polyhedron@Pointset_Powerset_NNC_Polyhedron/g' -e 's/^Pointset_Powerset_Polyhedron/Pointset_Powerset_C_Polyhedron@Pointset_Powerset_NNC_Polyhedron/g' | sed -e 's/Pointset_Powerset_[[a-zA-Z0-9_]]*/&@&_Iterator/g'`
  required_instantiations_java_sources=`echo "${required_instantiations_java_source_names}" | sed -e 's/@/.java /g' -e 's/$/.java/g'`
  required_instantiations_java_classes=`echo "${required_instantiations_java_sources}" | sed -e 's/\.java/.class/g'`
  required_instantiations_java_cxx_headers=`echo "${required_instantiations_java_sources}" | sed -e 's/\([[^. ]]*\)\.java/parma_polyhedra_library.\1/g'`
  required_instantiations_java_cxx_headers_sources=`echo "${required_instantiations_java_sources}" | sed -e 's/\([[^. ]]*\)\.java/parma_polyhedra_library_\1.h/g'`
  required_instantiations_java_cxx_objects=`echo "${required_instantiations_canonical_names}" | sed -e 's/@/.lo /g' -e 's/$/.lo/g' | sed -e 's/\([[^. ]]*\)\.lo/ppl_java_\1.lo/g'`
  required_instantiations_java_cxx_sources=`echo "${required_instantiations_java_cxx_objects}" | sed -e 's/\([[^. ]]*\)\.lo/\1.cc/g'`

  required_instantiations_ocaml_source_names=`echo "${required_instantiations_canonical_names}" | sed -e 's/@Pointset_Powerset_Polyhedron/@Pointset_Powerset_C_Polyhedron@Pointset_Powerset_NNC_Polyhedron/g' -e 's/^Pointset_Powerset_Polyhedron/Pointset_Powerset_C_Polyhedron@Pointset_Powerset_NNC_Polyhedron/g'`
  required_instantiations_ocaml_cxx_objects=`echo "${required_instantiations_ocaml_source_names}" | sed -e 's/@/.o /g' -e 's/$/.o/g' | sed -e 's/\([[^. ]]*\)\.o/ppl_ocaml_\1.o/g'`
  required_instantiations_ocaml_cxx_sources=`echo "${required_instantiations_ocaml_cxx_objects}" | sed -e 's/\([[^. ]]*\)\.o/\1.cc/g'`
  required_instantiations_ocaml_cxx_headers=`echo "${required_instantiations_ocaml_cxx_sources}" | sed -e 's/\.cc/.hh/g'`

  AC_MSG_RESULT([${required_instantiations}])
  ;;
esac

enableval=no
AC_MSG_CHECKING([whether tests should be run under Valgrind])
AC_ARG_ENABLE(valgrind-tests,
  AS_HELP_STRING([--enable-valgrind-tests],
                 [run library tests under Valgrind]))
case "${enableval}" in
yes)
  AC_CHECK_PROG(VALGRIND, valgrind, valgrind)
  if test x"$VALGRIND" = xvalgrind
  then
    AC_MSG_RESULT(yes)
    enable_valgrind_tests=yes
  else
    AC_MSG_ERROR([bad value ${enableval} for --enable-valgrind-tests, cannot find Valgrind])
  fi
  ;;
no)
  AC_MSG_RESULT(no)
  enable_valgrind_tests=no
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-valgrind-tests, needs yes or no])
  ;;
esac
AM_CONDITIONAL(VALGRIND_TESTS_ENABLED, test x"$enable_valgrind_tests" = xyes)

enableval=quick
AC_MSG_CHECKING([whether `make check' does a thorough or quick check])
AC_ARG_ENABLE(check,
  AS_HELP_STRING([--enable-check=KIND],
                 [select thorough or quick `make check']))
case "${enableval}" in
thorough)
  AC_MSG_RESULT(thorough)
  ;;
quick)
  AC_MSG_RESULT(quick)
  ;;
*)
  AC_MSG_ERROR([bad value ${enableval} for --enable-check, needs thorough or quick])
  ;;
esac
AM_CONDITIONAL(THOROUGH_MAKE_CHECK, test x"$enableval" = xthorough)

# Libtool.
LT_PREREQ([1.5.24])
LT_INIT([dlopen,win32-dll])
AM_CONDITIONAL(ENABLE_SHARED, test x"$enable_shared" = xyes)
AM_CONDITIONAL(ENABLE_STATIC, test x"$enable_static" = xyes)

# Checks for md5sum.
AC_CHECK_PROG(MD5SUM, md5sum, md5sum)
AM_CONDITIONAL(HAVE_MD5SUM, test x$MD5SUM = xmd5sum)
if test x$MD5SUM = xmd5sum
then
  AC_TEXT_MD5SUM
fi

# Check for the GLPK library.
AC_LANG_PUSH(C)
AC_CHECK_HEADERS([glpk.h glpk/glpk.h], have_glpk=yes)
AM_CONDITIONAL(HAVE_GLPK, test x$have_glpk = xyes)
if test x$have_glpk = xyes
then
  ac_save_LIBS="$LIBS"
  LIBS="$LIBS -lglpk"
  AC_CHECK_DECL(lib_set_print_hook,
                AC_DEFINE(PPL_GLPK_HAS_LIB_SET_PRINT_HOOK,
                          1,
                          [Defined if GLPK provides lib_set_print_hook().]),
                ,
                [
#if defined(HAVE_GLPK_GLPK_H)
#include <glpk/glpk.h>
#elif defined(HAVE_GLPK_H)
#include <glpk.h>
#endif
                ])
  AC_CHECK_FUNC(_glp_lib_print_hook,
                AC_DEFINE(PPL_GLPK_HAS__GLP_LIB_PRINT_HOOK,
                          1,
                          [Defined if GLPK provides _glp_lib_print_hook().]))
  AC_CHECK_DECL(glp_term_out,
                AC_DEFINE(PPL_GLPK_HAS_GLP_TERM_OUT,
                          1,
                          [Defined if GLPK provides glp_term_out().]),
                ,
                [
#if defined(HAVE_GLPK_GLPK_H)
#include <glpk/glpk.h>
#elif defined(HAVE_GLPK_H)
#include <glpk.h>
#endif
                ])
  AC_CHECK_DECL(glp_term_hook,
                AC_DEFINE(PPL_GLPK_HAS_GLP_TERM_HOOK,
                          1,
                          [Defined if GLPK provides glp_term_hook().]),
                ,
                [
#if defined(HAVE_GLPK_GLPK_H)
#include <glpk/glpk.h>
#elif defined(HAVE_GLPK_H)
#include <glpk.h>
#endif
                ])
  LIBS="$ac_save_LIBS"
fi
AC_LANG_POP(C)

# Checks for header declarations.
AC_CHECK_DECLS([RLIMIT_DATA, RLIMIT_RSS, RLIMIT_VMEM, RLIMIT_AS],
               ,
               ,
               [
#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif
               ])

AC_CHECK_DECLS([getrusage, setrlimit],
               ,
               ,
               [
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif
])

AC_CHECK_DECLS([setitimer], [], [], [[
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
]])

AC_CHECK_DECLS([sigaction], , , [#include <csignal>])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_TYPES([timeval])
AC_CHECK_TYPES([siginfo_t], [], [], [[
#include <signal.h>
]])

# Check whether we can limit memory in C and C++ using setrlimit().
AC_CXX_SUPPORTS_LIMITING_MEMORY

# Checks for library functions.
# Nothing for the time being.

# If we are using GCC or the Intel C/C++ compiler we want to compile
# with lots of warnings enabled.
if test x"$GCC" = xyes
then
  if test x"$ICC" = xyes
  then
    CFLAGS="$CFLAGS -w2 -wd161,177,193,279,383,444,981,1098,1188,1418,1419,1572"
  else
    CFLAGS="$CFLAGS -W -Wall"
  fi
fi
if test x"$GXX" = xyes
then
  if test x"$ICPC" = xyes
  then
    CXXFLAGS="$CXXFLAGS -w2 -wd161,177,193,279,383,444,981,1098,1188,1418,1419,1572"
  else
    CXXFLAGS="$CXXFLAGS -W -Wall"
  fi
fi


AC_SUBST(LIBEXT, [$libext])
AC_SUBST(SHREXT, [$shrext_cmds])
AC_SUBST(extra_includes)
AC_SUBST(extra_libraries)
AC_SUBST(debug_flag)
AC_SUBST(coefficient_mnemonic)
AC_SUBST(required_instantiations)
AC_SUBST(required_instantiations_canonical_names)
AC_SUBST(required_instantiations_c_cxx_headers)
AC_SUBST(required_instantiations_c_cxx_sources)
AC_SUBST(required_instantiations_c_cxx_objects)
AC_SUBST(required_instantiations_prolog_cxx_headers)
AC_SUBST(required_instantiations_prolog_cxx_sources)
AC_SUBST(required_instantiations_prolog_cxx_objects)
AC_SUBST(required_instantiations_prolog_generated_test_sources)
AC_SUBST(required_instantiations_java_sources)
AC_SUBST(required_instantiations_java_classes)
AC_SUBST(required_instantiations_java_cxx_headers)
# FIXME: rename headers_sources into sources.
AC_SUBST(required_instantiations_java_cxx_headers_sources)
AC_SUBST(required_instantiations_java_cxx_sources)
AC_SUBST(required_instantiations_java_cxx_objects)
AC_SUBST(required_instantiations_ocaml_cxx_headers)
AC_SUBST(required_instantiations_ocaml_cxx_sources)
AC_SUBST(required_instantiations_ocaml_cxx_objects)
AC_SUBST(mlgmp_dir)

AC_CONFIG_FILES(Makefile
                ppl.lsm
                src/Makefile
                src/version.hh
                src/ppl-config.cc
                tests/Makefile
                tests/BD_Shape/Makefile
                tests/Box/Makefile
                tests/Concrete_Expression/Makefile
                tests/CO_Tree/Makefile
                tests/Grid/Makefile
                tests/MIP_Problem/Makefile
                tests/Octagonal_Shape/Makefile
                tests/Partially_Reduced_Product/Makefile
                tests/PIP_Problem/Makefile
                tests/Polyhedron/Makefile
                tests/Powerset/Makefile
                tests/Watchdog/Makefile
                utils/Makefile
                m4/Makefile
                demos/Makefile
                demos/ppl_lcdd/Makefile
                demos/ppl_lcdd/examples/Makefile
                demos/ppl_lpsol/Makefile
                demos/ppl_lpsol/examples/Makefile
                demos/ppl_pips/Makefile
                demos/ppl_pips/examples/Makefile
                doc/Makefile
                doc/user.doxyconf-latex
                doc/devref.doxyconf-latex
                doc/user.doxyconf-html
                doc/devref.doxyconf-html
                doc/user-language-interface.doxyconf
                doc/devref-language-interface.doxyconf
                interfaces/Makefile
                interfaces/C/Makefile
                interfaces/C/ppl_c_version.h
                interfaces/C/tests/Makefile
                interfaces/Java/Makefile
                interfaces/Java/jni/Makefile
                interfaces/Java/tests/Makefile
                interfaces/Java/parma_polyhedra_library/Makefile
                interfaces/OCaml/Makefile
                interfaces/OCaml/tests/Makefile
                interfaces/Prolog/Makefile
                interfaces/Prolog/Ciao/Makefile
                interfaces/Prolog/GNU/Makefile
                interfaces/Prolog/SICStus/Makefile
                interfaces/Prolog/SWI/Makefile
                interfaces/Prolog/XSB/Makefile
                interfaces/Prolog/YAP/Makefile
                interfaces/Prolog/tests/Makefile
                )
#               tests/Ask_Tell/Makefile
AC_OUTPUT

if test x"$gmp_supports_exceptions" = xno
then
  AC_MSG_WARN([CANNOT PROPAGATE EXCEPTIONS BACK FROM GMP:
*** MEMORY EXHAUSTION MAY RESULT IN ABRUPT TERMINATION.
*** This is OK, if you do not plan to use the bounded memory capabilities
*** offered by the PPL.  Otherwise, if you are using GCC or the Intel C/C++
*** compiler, please make sure you use a version of GMP compiled with the
*** `-fexceptions' compiler option.
*** To build such a version, you can configure GMP as follows:
*** CPPFLAGS=-fexceptions ./configure --enable-cxx --prefix=/usr/local])
fi

if test x"$PERL" = xno
then
  AC_MSG_WARN([PERL UNAVAILABLE:
***  CANNOT REBUILD SOME FILES IF SOURCES ARE CHANGED.
*** Perl could not be found on your system.  This is OK, if you only plan
*** to build the PPL without modifying the sources and without trying to
*** build a distribution out of a CVS working copy.  If this is not the case,
*** then you need to add Perl to your development environment:
*** see http://www.perl.org/ for more information.])
fi

if test x"$coefficient_kind" = xnative
then
  AC_MSG_WARN([USING NATIVE INTEGERS IS EXTREMELY DANGEROUS:
*** CONSIDER ANY RESULT YOU WILL GET FROM THE LIBRARY AS PSEUDO-RANDOM!!!
*** You have requested to use a native integral type to represent
*** coefficients.  This means that arithmetic overflows will be possible
*** and will not be detected.  The consequence of that is that nothing
*** can be said about the behavior of the library:  whatever the
*** documentation says, it does not apply to the version of the library
*** you are about to build.  Just one arithmetic overflow is enough
*** to invalidate any possible significance of the results.
*** Please, do not assume that your computation will not overflow because
*** only small coefficients are involved.  Do not do it because
*** (1) coefficients may grow very rapidly depending on the polyhedra
***     computations being performed;
*** (2) the library may do internal polyhedra computations that you
***     cannot easily anticipate;
*** (3) the library has never had, among its design goals, the one of
***     trying to keep the coefficients small.
***
*** Please, consider using a _checked_ native integral type instead.])
fi
