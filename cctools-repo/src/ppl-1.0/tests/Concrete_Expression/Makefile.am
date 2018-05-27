# Automake source file for the Parma Polyhedra Library.
# Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
# Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
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

if VALGRIND_TESTS_ENABLED

CHECKER = \
libtool --mode=execute \
$(VALGRIND) --tool=memcheck -q --trace-children=yes --leak-check=yes \
--num-callers=40 --leak-resolution=high \
--suppressions=$(top_srcdir)/tests/valgrind_suppressions

else !VALGRIND_TESTS_ENABLED

CHECKER =

endif !VALGRIND_TESTS_ENABLED

TESTS_ENVIRONMENT = $(CHECKER)

AM_CPPFLAGS = \
$(TEST_CPPFLAGS) \
-I$(top_builddir)/src \
-I$(top_srcdir)/src \
-I$(top_srcdir)/tests \
-I$(top_srcdir)/utils \
@extra_includes@ \
@debug_flag@

LDADD = \
$(top_builddir)/utils/libppl_utils.a \
$(top_builddir)/tests/libppl_tests.a \
$(top_builddir)/src/libppl.la \
@extra_libraries@

ALL_TESTS = \
C_Expr \
linearform1 \
linearize \
digitalfilters1 \
bdshape1 \
bdshape2 \
octagonalshape1 \
octagonalshape2 \
polyhedron1 \
polyhedron2

BUGS =


# There are platforms where no floating-point type is supported.
if SUPPORTED_SOME_FLOATING_POINT_TYPE

check_PROGRAMS = $(ALL_TESTS) $(BUGS)

TESTS = run_tests.stamp

endif SUPPORTED_SOME_FLOATING_POINT_TYPE


# This will be overridden by the `run_tests' script.
if SUPPORTED_FLOAT
FLOAT_FLAGS = \
-DANALYZER_FP_FORMAT=float -DANALYZED_FP_FORMAT=IEEE754_SINGLE
else
if SUPPORTED_DOUBLE
DOUBLE_FLAGS = \
-DANALYZER_FP_FORMAT=double -DANALYZED_FP_FORMAT=IEEE754_DOUBLE
else
if SUPPORTED_LONG_DOUBLE
LONG_DOUBLE_FLAGS = \
-DANALYZER_FP_FORMAT=long_double -DANALYZED_FP_FORMAT=IEEE754_QUAD
endif
endif
endif

TEST_CPPFLAGS = $(FLOAT_FLAGS) $(DOUBLE_FLAGS) $(LONG_DOUBLE_FLAGS)

dist_check_SCRIPTS = run_tests

.PHONY: run_tests.stamp
run_tests.stamp: run_tests
	+MAKE="$(MAKE)" $(srcdir)/run_tests
	echo "true" >run_tests.stamp
	chmod +x run_tests.stamp

XFAIL_TESTS =

# Do not change the ordering in what follows.
# Analyzer format should be less precise or equal than analyzed format.
if SUPPORTED_FLOAT
FLOAT_INSTANCES= \
float/IEEE754_SINGLE \
float/IEEE754_DOUBLE \
float/IEEE754_QUAD
endif
if SUPPORTED_DOUBLE
DOUBLE_INSTANCES= \
double/IEEE754_DOUBLE \
double/IEEE754_QUAD
endif
if SUPPORTED_LONG_DOUBLE
LONG_DOUBLE_INSTANCES= \
long_double/IEEE754_QUAD
endif

# Do not change the ordering in what follows.
INSTANCES = \
$(FLOAT_INSTANCES) \
$(DOUBLE_INSTANCES) \
$(LONG_DOUBLE_INSTANCES)

print_check_PROGRAMS:
	echo $(check_PROGRAMS)

print_INSTANCES:
	echo $(INSTANCES)


#
# Sources for the tests
#

C_Expr_SOURCES = C_Expr.defs.hh C_Expr.inlines.hh C_Expr.cc

bdshape1_SOURCES = bdshape1.cc

bdshape2_SOURCES = bdshape2.cc

digitalfilters1_SOURCES = digitalfilters1.cc

linearform1_SOURCES = linearform1.cc

linearize_SOURCES = linearize.cc

octagonalshape1_SOURCES = octagonalshape1.cc

octagonalshape2_SOURCES = octagonalshape2.cc

polyhedron1_SOURCES = polyhedron1.cc

polyhedron2_SOURCES = polyhedron2.cc


CLEANFILES = \
run_tests.stamp


DISTCLEANFILES = \
dirty_marker


$(top_builddir)/utils/libppl_utils.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils libppl_utils.a

$(top_builddir)/tests/libppl_tests.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/tests libppl_tests.a

$(top_builddir)/src/libppl.la:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/src libppl.la
