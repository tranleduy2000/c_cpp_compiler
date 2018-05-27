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
-I$(top_builddir)/src \
-I$(top_srcdir)/src -I$(top_srcdir)/tests \
-I$(top_srcdir)/utils \
@extra_includes@ \
@debug_flag@

LDADD = \
$(top_builddir)/utils/libppl_utils.a \
$(top_builddir)/tests/libppl_tests.a \
$(top_builddir)/src/libppl.la \
@extra_libraries@

TESTS = \
addcongruences1 \
addconstraints1 \
affinedimension1 \
affineimage1 \
affinepreimage1 \
bounded1 \
bounds1 \
closed1 \
closure1 \
collapse1 \
concatenate1 \
contains1 \
containsintegerpoint1 \
difference1 \
discrete1 \
disjoint1 \
disjunct1 \
empty1 \
entails1 \
equals1 \
frombdshape1 \
frombox1 \
fromcongruences1 \
fromconstraints1 \
fromgrid1 \
fromoctagonalshape1 \
frompolyhedron1 \
fromspacedimension1 \
intersection1 \
maxmin1 \
meet1 \
membytes1 \
powerset1 \
reduce1 \
refinewith1 \
relationwith1 \
simplifyusingcontext1 \
size1 \
spacedims1 \
universe1 \
upperbound1

XFAIL_TESTS =

#
# Sources for the tests
#
addcongruences1_SOURCES = addcongruences1.cc

addconstraints1_SOURCES = addconstraints1.cc

affinedimension1_SOURCES = affinedimension1.cc

affineimage1_SOURCES = affineimage1.cc

affinepreimage1_SOURCES = affinepreimage1.cc

bounded1_SOURCES = bounded1.cc

bounds1_SOURCES = bounds1.cc

closed1_SOURCES = closed1.cc

closure1_SOURCES = closure1.cc

collapse1_SOURCES = collapse1.cc

concatenate1_SOURCES = concatenate1.cc

contains1_SOURCES = contains1.cc

containsintegerpoint1_SOURCES = containsintegerpoint1.cc

difference1_SOURCES = difference1.cc

discrete1_SOURCES = discrete1.cc

disjoint1_SOURCES = disjoint1.cc

disjunct1_SOURCES = disjunct1.cc

empty1_SOURCES = empty1.cc

entails1_SOURCES = entails1.cc

equals1_SOURCES = equals1.cc

frombdshape1_SOURCES = frombdshape1.cc

frombox1_SOURCES = frombox1.cc

fromcongruences1_SOURCES = fromcongruences1.cc

fromconstraints1_SOURCES = fromconstraints1.cc

fromgrid1_SOURCES = fromgrid1.cc

fromoctagonalshape1_SOURCES = fromoctagonalshape1.cc

frompolyhedron1_SOURCES = frompolyhedron1.cc

fromspacedimension1_SOURCES = fromspacedimension1.cc

intersection1_SOURCES = intersection1.cc

maxmin1_SOURCES = maxmin1.cc

meet1_SOURCES = meet1.cc

membytes1_SOURCES = membytes1.cc

powerset1_SOURCES = powerset1.cc

reduce1_SOURCES = reduce1.cc

refinewith1_SOURCES = refinewith1.cc

relationwith1_SOURCES = relationwith1.cc

simplifyusingcontext1_SOURCES = simplifyusingcontext1.cc

size1_SOURCES = size1.cc

spacedims1_SOURCES = spacedims1.cc

universe1_SOURCES = universe1.cc

upperbound1_SOURCES = upperbound1.cc

check_PROGRAMS = $(TESTS)

$(top_builddir)/utils/libppl_utils.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils libppl_utils.a

$(top_builddir)/tests/libppl_tests.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/tests libppl_tests.a

$(top_builddir)/src/libppl.la:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/src libppl.la
