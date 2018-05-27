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

check_PROGRAMS = \
addconstraints1 \
addspacedims1 \
affinedimension1 \
affineimage1 affineimage2 \
affinepreimage1 \
ascii_dump_load1 \
bgp99extrapolation1 \
bhmz05widening1 \
bhz03widening1 \
bounded1 \
bounds1 \
boundedaffineimage1 \
boundedaffinepreimage1 \
cc76narrowing1 \
cc76extrapolation1 \
closure1 \
concatenate1 \
congruences1 \
constrains1 \
constraints1 \
contains1 \
containsintegerpoint1 \
difference1 \
discrete1 \
disjoint1 \
dropsomenonintegerpoints1 \
empty1 \
equality1 \
expandspacedim1 \
foldspacedims1 \
frequency1 \
frombdshape1 \
frombox1 \
fromgensys1 \
fromgrid1 \
fromoctagonalshape1 \
frompolyhedron1 \
fromspacedim1 \
generalizedaffineimage1 generalizedaffineimage2 \
generalizedaffinepreimage1 generalizedaffinepreimage2 \
generalizedaffinepreimage3 \
geomcovers1 \
h79widening1 \
integerupperboundifexact1 \
intersection1 \
limitedbhmz05extrapolation1 \
limitedcc76extrapolation1 \
limitedh79extrapolation1 \
mapspacedims1 \
max_min1 max_min2 \
maxspacedim1 \
membytes1 \
minconstraints1 \
relations1 relations2 relations3 relations4 \
removespacedims1 \
simplifyusingcontext1 \
timeelapse1 \
unconstrain1 \
universe1 \
upperbound1 \
upperboundifexact1 \
wrap1 \
writebdshape1

# This will be overridden by the `run_tests' script.
TEST_CPPFLAGS=-DBD_SHAPE_INSTANCE=mpq_class

dist_check_SCRIPTS = run_tests

TESTS = run_tests.stamp

.PHONY: run_tests.stamp
run_tests.stamp: run_tests
	+MAKE="$(MAKE)" $(srcdir)/run_tests
	echo "true" >run_tests.stamp
	chmod +x run_tests.stamp

XFAIL_TESTS =

if SUPPORTED_FLOAT
FLOAT_INSTANCE=float
endif
if SUPPORTED_DOUBLE
DOUBLE_INSTANCE=double
endif
if SUPPORTED_LONG_DOUBLE
LONG_DOUBLE_INSTANCE=long_double
endif

# NOTE: mpq_class must be the first in the list.
INSTANCES = \
mpq_class \
$(FLOAT_INSTANCE) \
$(DOUBLE_INSTANCE) \
$(LONG_DOUBLE_INSTANCE) \
mpz_class \
int8_t \
int16_t \
int32_t \
int64_t

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
affineimage2_SOURCES = affineimage2.cc

affinepreimage1_SOURCES = affinepreimage1.cc

ascii_dump_load1_SOURCES = ascii_dump_load1.cc

bgp99extrapolation1_SOURCES = bgp99extrapolation1.cc

bhmz05widening1_SOURCES = bhmz05widening1.cc

bhz03widening1_SOURCES = bhz03widening1.cc

bounded1_SOURCES = bounded1.cc

bounds1_SOURCES = bounds1.cc

boundedaffineimage1_SOURCES = boundedaffineimage1.cc

boundedaffinepreimage1_SOURCES = boundedaffinepreimage1.cc

cc76narrowing1_SOURCES = cc76narrowing1.cc

cc76extrapolation1_SOURCES = cc76extrapolation1.cc

closure1_SOURCES = closure1.cc

concatenate1_SOURCES = concatenate1.cc

congruences1_SOURCES = congruences1.cc

constrains1_SOURCES = constrains1.cc

constraints1_SOURCES = constraints1.cc

contains1_SOURCES = contains1.cc

containsintegerpoint1_SOURCES = containsintegerpoint1.cc

difference1_SOURCES = difference1.cc

discrete1_SOURCES = discrete1.cc

disjoint1_SOURCES = disjoint1.cc

dropsomenonintegerpoints1_SOURCES = dropsomenonintegerpoints1.cc

empty1_SOURCES = empty1.cc

equality1_SOURCES = equality1.cc

expandspacedim1_SOURCES = expandspacedim1.cc

foldspacedims1_SOURCES = foldspacedims1.cc

frequency1_SOURCES = frequency1.cc

frombdshape1_SOURCES = frombdshape1.cc

frombox1_SOURCES = frombox1.cc

fromgensys1_SOURCES = fromgensys1.cc

fromgrid1_SOURCES = fromgrid1.cc

fromoctagonalshape1_SOURCES = fromoctagonalshape1.cc

frompolyhedron1_SOURCES = frompolyhedron1.cc

fromspacedim1_SOURCES = fromspacedim1.cc

generalizedaffineimage1_SOURCES = generalizedaffineimage1.cc
generalizedaffineimage2_SOURCES = generalizedaffineimage2.cc

generalizedaffinepreimage1_SOURCES = generalizedaffinepreimage1.cc
generalizedaffinepreimage2_SOURCES = generalizedaffinepreimage2.cc
generalizedaffinepreimage3_SOURCES = generalizedaffinepreimage3.cc

geomcovers1_SOURCES = geomcovers1.cc

h79widening1_SOURCES = h79widening1.cc

integerupperboundifexact1_SOURCES = integerupperboundifexact1.cc

intersection1_SOURCES = intersection1.cc

limitedbhmz05extrapolation1_SOURCES = limitedbhmz05extrapolation1.cc

limitedcc76extrapolation1_SOURCES = limitedcc76extrapolation1.cc

limitedh79extrapolation1_SOURCES = limitedh79extrapolation1.cc

mapspacedims1_SOURCES = mapspacedims1.cc

max_min1_SOURCES = max_min1.cc
max_min2_SOURCES = max_min2.cc

maxspacedim1_SOURCES = maxspacedim1.cc

membytes1_SOURCES = membytes1.cc

minconstraints1_SOURCES = minconstraints1.cc

relations1_SOURCES = relations1.cc
relations2_SOURCES = relations2.cc
relations3_SOURCES = relations3.cc
relations4_SOURCES = relations4.cc

removespacedims1_SOURCES = removespacedims1.cc

simplifyusingcontext1_SOURCES = simplifyusingcontext1.cc

timeelapse1_SOURCES = timeelapse1.cc

unconstrain1_SOURCES = unconstrain1.cc

universe1_SOURCES = universe1.cc

upperbound1_SOURCES = upperbound1.cc

upperboundifexact1_SOURCES = upperboundifexact1.cc

wrap1_SOURCES = wrap1.cc

writebdshape1_SOURCES = writebdshape1.cc


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

