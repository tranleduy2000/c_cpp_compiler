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
addcongruence1 \
addcongruences1 \
addconstraint1 \
addconstraints1 \
addgenerator1 \
addgenerators1 \
addspacedims1 \
affinedim1 \
affineimage1 affineimage2 \
affinepreimage1 affinepreimage2 \
approximatepartition1 \
asciidumpload1 asciidumpload2 asciidumpload3 asciidumpload4 asciidumpload5 \
asciidumpload6 \
bhz03widening1 \
bounded1 \
bounds1 \
boundedaffineimage1 \
boundedaffinepreimage1 \
certificate1 \
concatenate1 \
congruence1 \
congruences1 \
congruences2 \
constraints1 \
contains1 \
containsintegerpoint1 \
discrete1 \
disjoint1 \
dropnonintegerpoints1 \
equals1 \
expandspacedim1 \
foldspacedims1 \
frequency1 \
frombdshape1 \
frombox1 \
fromgrid1 \
fromoctagonalshape1 \
frompolyhedron1 \
generalizedaffineimage1 generalizedaffineimage2 generalizedaffineimage3 \
generalizedaffinepreimage1 generalizedaffinepreimage2 \
generalizedaffinepreimage3 \
generator1 \
generators1 generators2 \
grid1 grid2 grid3 \
griddifference1 \
intersection1 \
isempty1 \
isuniverse1 \
limitedextrapolation1 limitedextrapolation2 limitedextrapolation3 \
mapspacedims1 \
maxmin1 \
membytes1 \
mingenerators1 \
mincongruences1 \
outputoperator1 outputoperator2 outputoperator3 \
partition1 \
powersetdifference1 \
powersetgeometricallycovers1 \
powersetgeometricallyequals1 \
refinewithcongruences1 \
refinewithconstraints1 \
relations1 relations2 relations3 \
removespacedims1 removespacedims2 \
simplifyusingcontext1 \
timeelapse1 \
topclosed1 \
topclosure1 \
unconstrain1 \
upperbound1 upperbound2 \
widening1 widening2  widening3 \
wrap1 \
writecongruencesystem

XFAIL_TESTS =

#
# Sources for the tests
#

addcongruence1_SOURCES = addcongruence1.cc

addcongruences1_SOURCES = addcongruences1.cc

addconstraint1_SOURCES = addconstraint1.cc

addconstraints1_SOURCES = addconstraints1.cc

addgenerator1_SOURCES = addgenerator1.cc

addgenerators1_SOURCES = addgenerators1.cc

addspacedims1_SOURCES = addspacedims1.cc

affinedim1_SOURCES = affinedim1.cc

affineimage1_SOURCES = affineimage1.cc
affineimage2_SOURCES = affineimage2.cc

affinepreimage1_SOURCES = affinepreimage1.cc
affinepreimage2_SOURCES = affinepreimage2.cc

approximatepartition1_SOURCES = approximatepartition1.cc

asciidumpload1_SOURCES = asciidumpload1.cc
asciidumpload2_SOURCES = asciidumpload2.cc
asciidumpload3_SOURCES = asciidumpload3.cc
asciidumpload4_SOURCES = asciidumpload4.cc
asciidumpload5_SOURCES = asciidumpload5.cc
asciidumpload6_SOURCES = asciidumpload6.cc

bhz03widening1_SOURCES = bhz03widening1.cc

bounded1_SOURCES = bounded1.cc

bounds1_SOURCES = bounds1.cc

boundedaffineimage1_SOURCES = boundedaffineimage1.cc

boundedaffinepreimage1_SOURCES = boundedaffinepreimage1.cc

certificate1_SOURCES = certificate1.cc

concatenate1_SOURCES = concatenate1.cc

congruence1_SOURCES = congruence1.cc

congruences1_SOURCES = congruences1.cc
congruences2_SOURCES = congruences2.cc

constraints1_SOURCES = constraints1.cc

contains1_SOURCES = contains1.cc

containsintegerpoint1_SOURCES = containsintegerpoint1.cc

discrete1_SOURCES = discrete1.cc

disjoint1_SOURCES = disjoint1.cc

dropnonintegerpoints1_SOURCES = dropnonintegerpoints1.cc

equals1_SOURCES = equals1.cc

expandspacedim1_SOURCES = expandspacedim1.cc

frequency1_SOURCES = frequency1.cc

frombdshape1_SOURCES = frombdshape1.cc

frombox1_SOURCES = frombox1.cc

fromgrid1_SOURCES = fromgrid1.cc

fromoctagonalshape1_SOURCES = fromoctagonalshape1.cc

frompolyhedron1_SOURCES = frompolyhedron1.cc

foldspacedims1_SOURCES = foldspacedims1.cc

generalizedaffineimage1_SOURCES = generalizedaffineimage1.cc
generalizedaffineimage2_SOURCES = generalizedaffineimage2.cc
generalizedaffineimage3_SOURCES = generalizedaffineimage3.cc

generalizedaffinepreimage1_SOURCES = generalizedaffinepreimage1.cc
generalizedaffinepreimage2_SOURCES = generalizedaffinepreimage2.cc
generalizedaffinepreimage3_SOURCES = generalizedaffinepreimage3.cc

generator1_SOURCES = generator1.cc

generators1_SOURCES = generators1.cc
generators2_SOURCES = generators2.cc

grid1_SOURCES = grid1.cc
grid2_SOURCES = grid2.cc
grid3_SOURCES = grid3.cc

griddifference1_SOURCES = griddifference1.cc

intersection1_SOURCES = intersection1.cc

isempty1_SOURCES = isempty1.cc

isuniverse1_SOURCES = isuniverse1.cc

limitedextrapolation1_SOURCES = limitedextrapolation1.cc
limitedextrapolation2_SOURCES = limitedextrapolation2.cc
limitedextrapolation3_SOURCES = limitedextrapolation3.cc

mapspacedims1_SOURCES = mapspacedims1.cc

maxmin1_SOURCES = maxmin1.cc

membytes1_SOURCES = membytes1.cc

mincongruences1_SOURCES = mincongruences1.cc

mingenerators1_SOURCES = mingenerators1.cc

outputoperator1_SOURCES = outputoperator1.cc
outputoperator2_SOURCES = outputoperator2.cc
outputoperator3_SOURCES = outputoperator3.cc

partition1_SOURCES = partition1.cc

powersetdifference1_SOURCES = powersetdifference1.cc
powersetgeometricallycovers1_SOURCES = powersetgeometricallycovers1.cc
powersetgeometricallyequals1_SOURCES = powersetgeometricallyequals1.cc

refinewithcongruences1_SOURCES = refinewithcongruences1.cc
refinewithconstraints1_SOURCES = refinewithconstraints1.cc

relations1_SOURCES = relations1.cc
relations2_SOURCES = relations2.cc
relations3_SOURCES = relations3.cc

removespacedims1_SOURCES = removespacedims1.cc
removespacedims2_SOURCES = removespacedims2.cc

simplifyusingcontext1_SOURCES = simplifyusingcontext1.cc

timeelapse1_SOURCES = timeelapse1.cc

topclosed1_SOURCES = topclosed1.cc

topclosure1_SOURCES = topclosure1.cc

unconstrain1_SOURCES = unconstrain1.cc

upperbound1_SOURCES = upperbound1.cc
upperbound2_SOURCES = upperbound2.cc

widening1_SOURCES = widening1.cc
widening2_SOURCES = widening2.cc
widening3_SOURCES = widening3.cc

writecongruencesystem_SOURCES = writecongruencesystem.cc

wrap1_SOURCES = wrap1.cc

check_PROGRAMS = $(TESTS)

MOSTLYCLEANFILES = \
ascii_dump_load1.dat \
ascii_dump_load2.dat \
ascii_dump_load3.dat \
ascii_dump_load4.dat \
ascii_dump_load5.dat \
ascii_dump_load6.dat \
ascii_dump_load7.dat

$(top_builddir)/utils/libppl_utils.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils libppl_utils.a

$(top_builddir)/tests/libppl_tests.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/tests libppl_tests.a

$(top_builddir)/src/libppl.la:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/src libppl.la
