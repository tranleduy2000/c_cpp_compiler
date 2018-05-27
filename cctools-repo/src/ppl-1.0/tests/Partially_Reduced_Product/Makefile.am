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

noinst_HEADERS = \
partially_reduced_product_test.hh

AM_CPPFLAGS = \
-I$(top_builddir)/src \
-I$(top_srcdir)/src -I$(top_srcdir)/tests \
-I$(top_srcdir)/tests/Partially_Reduced_Product \
-I$(top_srcdir)/utils \
@extra_includes@ \
@debug_flag@

LDADD = \
$(top_builddir)/utils/libppl_utils.a \
$(top_builddir)/tests/libppl_tests.a \
$(top_builddir)/src/libppl.la \
@extra_libraries@

ORIGINAL_TESTS = \
addcongruences1 \
affineimage1 \
asciidumpload1 \
bounded1 \
boundedaffineimage1 \
bounds1 \
concatenate1 \
congruences1 \
congruencesproduct1 \
constraints1 \
constraintsproduct1 \
contains1 \
difference1 \
dimension1 \
directproduct1 \
discrete1 \
disjoint1 \
dropsomenonintegerpoints1 \
equals1 \
frombdshape1 \
frombox1 \
fromgrid1 \
fromoctagonalshape1 \
frompolyhedron1 \
fromproduct1 \
generalizedaffineimage1 \
intersection1 \
isempty1 \
isuniverse1 \
maxmin1 \
refinewithcongruences1 \
refinewithconstraints1 \
relations1 \
shapepreservingproduct1 \
smashproduct1 \
spacedims1 \
timeelapse1 \
topclosed1 \
topclosure1 \
upperbound1

REVERSED_TESTS = \
reverse_constraints1 \
reverse_congruences1 \
reverse_directproduct1 \
reverse_constraintsproduct1 \
reverse_congruencesproduct1 \
reverse_shapepreservingproduct1

TESTS = \
$(ORIGINAL_TESTS) \
$(REVERSED_TESTS)

XFAIL_TESTS =

#
# Sources for the tests
#

addcongruences1_SOURCES = addcongruences1.cc

affineimage1_SOURCES = affineimage1.cc

asciidumpload1_SOURCES = asciidumpload1.cc

bounded1_SOURCES = bounded1.cc

boundedaffineimage1_SOURCES = boundedaffineimage1.cc

bounds1_SOURCES = bounds1.cc

concatenate1_SOURCES = concatenate1.cc

congruences1_SOURCES = congruences1.cc

congruencesproduct1_SOURCES = congruencesproduct1.cc

constraintsproduct1_SOURCES = constraintsproduct1.cc

constraints1_SOURCES = constraints1.cc

contains1_SOURCES = contains1.cc

difference1_SOURCES = difference1.cc

dimension1_SOURCES = dimension1.cc

directproduct1_SOURCES = directproduct1.cc

discrete1_SOURCES = discrete1.cc

disjoint1_SOURCES = disjoint1.cc

dropsomenonintegerpoints1_SOURCES = dropsomenonintegerpoints1.cc

equals1_SOURCES = equals1.cc

frombdshape1_SOURCES = frombdshape1.cc

frombox1_SOURCES = frombox1.cc

fromgrid1_SOURCES = fromgrid1.cc

fromoctagonalshape1_SOURCES = fromoctagonalshape1.cc

frompolyhedron1_SOURCES = frompolyhedron1.cc

fromproduct1_SOURCES = fromproduct1.cc

generalizedaffineimage1_SOURCES = generalizedaffineimage1.cc

intersection1_SOURCES = intersection1.cc

isempty1_SOURCES = isempty1.cc

isuniverse1_SOURCES = isuniverse1.cc

maxmin1_SOURCES = maxmin1.cc

refinewithcongruences1_SOURCES = refinewithcongruences1.cc
refinewithconstraints1_SOURCES = refinewithconstraints1.cc

relations1_SOURCES = relations1.cc

shapepreservingproduct1_SOURCES = shapepreservingproduct1.cc

smashproduct1_SOURCES = smashproduct1.cc

spacedims1_SOURCES = spacedims1.cc

timeelapse1_SOURCES = timeelapse1.cc

topclosed1_SOURCES = topclosed1.cc

topclosure1_SOURCES = topclosure1.cc

upperbound1_SOURCES = upperbound1.cc

DERIVED_CXXFLAGS = -DREVERSED_TESTS $(AM_CXXFLAGS)

reverse_congruences1_SOURCES = congruences1.cc
reverse_congruences1_CXXFLAGS = $(DERIVED_CXXFLAGS)

reverse_constraints1_SOURCES = constraints1.cc
reverse_constraints1_CXXFLAGS = $(DERIVED_CXXFLAGS)

reverse_directproduct1_SOURCES = directproduct1.cc
reverse_directproduct1_CXXFLAGS = $(DERIVED_CXXFLAGS)

reverse_constraintsproduct1_SOURCES = constraintsproduct1.cc
reverse_constraintsproduct1_CXXFLAGS = $(DERIVED_CXXFLAGS)

reverse_congruencesproduct1_SOURCES = congruencesproduct1.cc
reverse_congruencesproduct1_CXXFLAGS = $(DERIVED_CXXFLAGS)

reverse_shapepreservingproduct1_SOURCES = shapepreservingproduct1.cc
reverse_shapepreservingproduct1_CXXFLAGS = $(DERIVED_CXXFLAGS)

check_PROGRAMS = $(TESTS)

MOSTLYCLEANFILES = \
ascii_dump_load1.dat

$(top_builddir)/utils/libppl_utils.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils libppl_utils.a

$(top_builddir)/tests/libppl_tests.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/tests libppl_tests.a

$(top_builddir)/src/libppl.la:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/src libppl.la
