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

ORIGINAL_TESTS = \
addconstraints1 \
addspacedims1 \
affinedimension1 \
affineimage1 \
affinepreimage1 \
ascii_dump_load1 \
bgp99extrapolation1 \
bhz03widening1 \
bounded1 \
boundedaffineimage1 \
boundedaffinepreimage1 \
cc76narrowing1 \
cc76widening \
concatenate1 \
congruences1 \
constrains1 \
constraints1 \
contains1 \
containsintegerpoint1 \
difference1 \
discrete1 \
disjoint1 \
empty1 \
equality1 \
expandspacedim1 \
foldspacedims1 \
frequency1 \
frombdshape1 \
frombox1 \
fromgensys1 \
fromgrid1 \
frompartiallyreducedproduct1 \
frompolyhedron1 \
frompolyhedron2 \
fromspacedim1 \
generalizedaffineimage1 generalizedaffineimage2 \
generalizedaffinepreimage1 \
geomcovers1 \
intersection1 \
interval1 \
limitedcc76extrapolation1 \
mapspacedims1 \
max_min1 \
maxspacedim1 \
membytes1 \
minconstraints1 \
propagateconstraints1 propagateconstraints2 \
relations1 relations2 relations3 relations4 \
refinewithconstraint1 refinewithconstraint2 \
refinewithconstraints1 \
refinewithconstraints2 \
refinewithcongruence1 \
refinewithcongruences1 \
removespacedims1 \
simplifyusingcontext1 \
timeelapse1 \
topclosed1 \
unconstrain1 \
universe1 \
upperbound1 \
upperboundifexact1 \
wrap1 \
writebox1

DERIVED_TESTS = \
nnc_frompolyhedron1

ALL_TESTS = $(ORIGINAL_TESTS) $(DERIVED_TESTS)

# This will be overridden by the `run_tests' script.
TEST_CPPFLAGS=-DBOX_INSTANCE=rt_r_oc

dist_check_SCRIPTS = run_tests

TESTS = run_tests.stamp

.PHONY: run_tests.stamp
run_tests.stamp: run_tests
	+MAKE="$(MAKE)" $(srcdir)/run_tests
	echo "true" >run_tests.stamp
	chmod +x run_tests.stamp

XFAIL_TESTS =

if SUPPORTED_FLOAT
FLOAT_INSTANCES=fl_r_oc
endif
if SUPPORTED_DOUBLE
DOUBLE_INSTANCES=db_r_oc
endif
if SUPPORTED_LONG_DOUBLE
LONG_DOUBLE_INSTANCES=ld_r_oc
endif

# NOTE: rt_r_oc must be the first in the list.
INSTANCES = \
rt_r_oc \
$(LONG_DOUBLE_INSTANCES) \
$(DOUBLE_INSTANCES) \
$(FLOAT_INSTANCES)

print_check_PROGRAMS:
	echo $(check_PROGRAMS)

print_INSTANCES:
	echo $(INSTANCES)

#
# Sources for the tests
#

addconstraints1_SOURCES = addconstraints1.cc

addspacedims1_SOURCES = addspacedims1.cc

affinedimension1_SOURCES = affinedimension1.cc

affineimage1_SOURCES = affineimage1.cc

affinepreimage1_SOURCES = affinepreimage1.cc

ascii_dump_load1_SOURCES = ascii_dump_load1.cc

bgp99extrapolation1_SOURCES = bgp99extrapolation1.cc

bhz03widening1_SOURCES = bhz03widening1.cc

bounded1_SOURCES = bounded1.cc

boundedaffineimage1_SOURCES = boundedaffineimage1.cc
boundedaffinepreimage1_SOURCES = boundedaffinepreimage1.cc

cc76narrowing1_SOURCES = cc76narrowing1.cc

cc76widening_SOURCES = cc76widening.cc

concatenate1_SOURCES = concatenate1.cc

congruences1_SOURCES = congruences1.cc

constrains1_SOURCES = constrains1.cc

constraints1_SOURCES = constraints1.cc

contains1_SOURCES = contains1.cc

containsintegerpoint1_SOURCES = containsintegerpoint1.cc

difference1_SOURCES = difference1.cc

discrete1_SOURCES = discrete1.cc

disjoint1_SOURCES = disjoint1.cc

empty1_SOURCES = empty1.cc

equality1_SOURCES = equality1.cc

expandspacedim1_SOURCES = expandspacedim1.cc

foldspacedims1_SOURCES = foldspacedims1.cc

frequency1_SOURCES = frequency1.cc

frombdshape1_SOURCES = frombdshape1.cc

frombox1_SOURCES = frombox1.cc

fromgensys1_SOURCES = fromgensys1.cc

fromgrid1_SOURCES = fromgrid1.cc

frompolyhedron1_SOURCES = frompolyhedron1.cc
frompolyhedron2_SOURCES = frompolyhedron2.cc

frompartiallyreducedproduct1_SOURCES = frompartiallyreducedproduct1.cc

fromspacedim1_SOURCES = fromspacedim1.cc

generalizedaffineimage1_SOURCES = generalizedaffineimage1.cc
generalizedaffineimage2_SOURCES = generalizedaffineimage2.cc

generalizedaffinepreimage1_SOURCES = generalizedaffinepreimage1.cc

geomcovers1_SOURCES = geomcovers1.cc

intersection1_SOURCES = intersection1.cc

interval1_SOURCES = interval1.cc

limitedcc76extrapolation1_SOURCES = limitedcc76extrapolation1.cc

mapspacedims1_SOURCES = mapspacedims1.cc

max_min1_SOURCES = max_min1.cc

maxspacedim1_SOURCES = maxspacedim1.cc

membytes1_SOURCES = membytes1.cc

minconstraints1_SOURCES = minconstraints1.cc

relations1_SOURCES = relations1.cc
relations2_SOURCES = relations2.cc
relations3_SOURCES = relations3.cc
relations4_SOURCES = relations4.cc

propagateconstraints1_SOURCES = propagateconstraints1.cc
propagateconstraints2_SOURCES = propagateconstraints2.cc

refinewithcongruence1_SOURCES = refinewithcongruence1.cc

refinewithcongruences1_SOURCES = refinewithcongruences1.cc

refinewithconstraint1_SOURCES = refinewithconstraint1.cc
refinewithconstraint2_SOURCES = refinewithconstraint2.cc

refinewithconstraints1_SOURCES = refinewithconstraints1.cc
refinewithconstraints2_SOURCES = refinewithconstraints2.cc

removespacedims1_SOURCES = removespacedims1.cc

simplifyusingcontext1_SOURCES = simplifyusingcontext1.cc

timeelapse1_SOURCES = timeelapse1.cc

topclosed1_SOURCES = topclosed1.cc

unconstrain1_SOURCES = unconstrain1.cc

universe1_SOURCES = universe1.cc

upperbound1_SOURCES = upperbound1.cc

upperboundifexact1_SOURCES = upperboundifexact1.cc

wrap1_SOURCES = wrap1.cc

writebox1_SOURCES = writebox1.cc


#
# Sources and compilation flags for the derived check programs
#

DERIVED_CXXFLAGS = -DDERIVED_TEST $(AM_CXXFLAGS)

nnc_frompolyhedron1_SOURCES = frompolyhedron1.cc
nnc_frompolyhedron1_CXXFLAGS = $(DERIVED_CXXFLAGS)

BUGS =

check_PROGRAMS = $(ALL_TESTS) $(BUGS)

EXTRA_DIST =

MOSTLYCLEANFILES = \
ascii_dump_load1.dat

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
