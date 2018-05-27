# Automake source file for the Parma Polyhedra Library.
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
-I$(top_srcdir)/src \
-I$(top_builddir)/src \
-I$(top_srcdir)/utils \
-I$(top_srcdir)/tests \
@extra_includes@ \
@debug_flag@

LDADD = \
$(top_builddir)/utils/libppl_utils.a \
$(top_builddir)/tests/libppl_tests.a \
$(top_builddir)/src/libppl.la \
@extra_libraries@

ORIGINAL_TESTS = \
addcongruence1 \
addcongruences1 \
addconstraint1 \
addconstraints1 addconstraints2 \
addgenerator1 addgenerator2 \
addgenerators1 addgenerators2 \
addspacedims1 addspacedims2 \
affineimage1 affineimage2 \
affinepreimage1 \
affinetrans \
append1 append2 \
ascii_dump_load1 ascii_dump_load2 ascii_dump_load3 \
bgp99extrapolation1 bgp99extrapolation2 \
bhrz03widening1 bhrz03widening2 bhrz03widening3 \
bhz03widening1 \
bounded1 \
boundedaffineimage1 \
boundedaffinepreimage1 \
boundedbhrz03extrapolation1 \
boundedh79extrapolation1 \
bounds1 \
cnncconversion1 \
concatenate1 \
congruences1 \
constrains1 \
constraints1 \
contains1 contains2 \
containsintegerpoint1 \
disjoint1 disjoint2 \
dropsomenonintegerpoints1 dropsomenonintegerpoints2 \
dualhypercubes \
empty1 \
equals1 \
exceptions1 exceptions2  exceptions3 \
expandspacedim1 expandspacedim2 \
foldspacedims1 foldspacedims2 \
frequency1 \
frombdshape1 \
frombox1 frombox2 \
fromgrid1 \
fromoctagonalshape1 \
generalizedaffineimage1 generalizedaffineimage2 \
generalizedaffinepreimage1 generalizedaffinepreimage2 \
generators1 \
geomcovers1 \
h79widening1 h79widening2 \
hybrid \
intersection1 \
limitedbhrz03extrapolation1 \
limitedh79extrapolation1 \
linearexpression1 \
linearpartition1 \
linearsystem1 \
mapspacedims1 \
matrix1 \
max_min1 \
maxspacedim1 \
mc91 \
membytes1 \
memory2 \
minconstraints1 minconstraints2 \
mingenerators1 mingenerators2 \
nncminimize1 nncminimize2 \
nncpostimeelapse1 \
numberinput1 \
onepoint \
permute \
polydifference1 polydifference2 \
polyhull1 polyhull2 \
polyhullifexact1 polyhullifexact2 \
randphull1 \
refinewithcongruence1 \
refinewithcongruences1 \
refinewithconstraint1 \
refinewithconstraints1 \
relations1 relations2 relations3 \
removespacedims1 removespacedims2 \
simplifyusingcontext1 \
smm1 \
sparserow1 \
termination1 termination2 \
timeelapse1 timeelapse2 \
topclosed1 \
topclosure1 \
unconstrain1 \
universe1 universe2 \
variablesset1 \
watchdog1 \
weightwatch1 \
wrap1 wrap2 \
writeconsys1 \
writegensys1 \
writepolyhedron1 writepolyhedron2 \
writerelation1 \
writevariable1

DERIVED_TESTS = \
nnc_addcongruence1 \
nnc_addcongruences1 \
nnc_addconstraint1 \
nnc_addconstraints1 \
nnc_addgenerator1 \
nnc_addgenerators1 \
nnc_addspacedims1 \
nnc_affineimage1 \
nnc_affinepreimage1 \
nnc_affinetrans \
nnc_append1 nnc_append2 \
nnc_ascii_dump_load1 nnc_ascii_dump_load2 \
nnc_bgp99extrapolation1 \
nnc_bhrz03widening1 nnc_bhrz03widening2 \
nnc_bounded1 \
nnc_boundedaffineimage1 \
nnc_boundedaffinepreimage1 \
nnc_boundedbhrz03extrapolation1 \
nnc_boundedh79extrapolation1 \
nnc_bounds1 \
nnc_concatenate1 \
nnc_congruences1 \
nnc_constrains1 \
nnc_constraints1 \
nnc_contains1 \
nnc_containsintegerpoint1 \
nnc_disjoint1 \
nnc_dropsomenonintegerpoints1 \
nnc_empty1 \
nnc_equals1 \
nnc_exceptions1 nnc_exceptions3 \
nnc_expandspacedim1 \
nnc_foldspacedims1 \
nnc_frombdshape1 \
nnc_frombox1 \
nnc_fromgrid1 \
nnc_fromoctagonalshape1 \
nnc_generalizedaffineimage1 \
nnc_generalizedaffinepreimage1 \
nnc_generators1 \
nnc_geomcovers1 \
nnc_h79widening1 \
nnc_intersection1 \
nnc_simplifyusingcontext1 \
nnc_limitedbhrz03extrapolation1 \
nnc_limitedh79extrapolation1 \
nnc_linearpartition1 \
nnc_mapspacedims1 \
nnc_max_min1 \
nnc_mc91 \
nnc_membytes1 \
nnc_minconstraints1 \
nnc_mingenerators1 \
nnc_onepoint \
nnc_permute \
nnc_polydifference1 \
nnc_polyhull1 \
nnc_polyhullifexact1 \
nnc_randphull1 \
nnc_relations1 nnc_relations2 \
nnc_removespacedims1 \
nnc_smm1 \
nnc_timeelapse1 \
nnc_unconstrain1 \
nnc_universe1 \
nnc_wrap1 nnc_wrap2 \
nnc_writepolyhedron1

#
# Sources for the tests
#

addcongruence1_SOURCES = addcongruence1.cc

addcongruences1_SOURCES = addcongruences1.cc

addconstraint1_SOURCES = addconstraint1.cc

addconstraints1_SOURCES = addconstraints1.cc
addconstraints2_SOURCES = addconstraints2.cc

addgenerator1_SOURCES = addgenerator1.cc
addgenerator2_SOURCES = addgenerator2.cc

addgenerators1_SOURCES = addgenerators1.cc
addgenerators2_SOURCES = addgenerators2.cc

addspacedims1_SOURCES = addspacedims1.cc
addspacedims2_SOURCES = addspacedims2.cc

affineimage1_SOURCES = affineimage1.cc
affineimage2_SOURCES = affineimage2.cc

affinepreimage1_SOURCES = affinepreimage1.cc

affinetrans_SOURCES = affinetrans.cc

append1_SOURCES = append1.cc
append2_SOURCES = append2.cc

ascii_dump_load1_SOURCES = ascii_dump_load1.cc
ascii_dump_load2_SOURCES = ascii_dump_load2.cc
ascii_dump_load3_SOURCES = ascii_dump_load3.cc

bgp99extrapolation1_SOURCES = bgp99extrapolation1.cc
bgp99extrapolation2_SOURCES = bgp99extrapolation2.cc

bhrz03widening1_SOURCES = bhrz03widening1.cc
bhrz03widening2_SOURCES = bhrz03widening2.cc
bhrz03widening3_SOURCES = bhrz03widening3.cc

bhz03widening1_SOURCES = bhz03widening1.cc

bounded1_SOURCES = bounded1.cc

boundedaffineimage1_SOURCES = boundedaffineimage1.cc

boundedaffinepreimage1_SOURCES = boundedaffinepreimage1.cc

boundedbhrz03extrapolation1_SOURCES = boundedbhrz03extrapolation1.cc

boundedh79extrapolation1_SOURCES = boundedh79extrapolation1.cc

bounds1_SOURCES = bounds1.cc

cnncconversion1_SOURCES = cnncconversion1.cc

concatenate1_SOURCES = concatenate1.cc

congruences1_SOURCES = congruences1.cc

constrains1_SOURCES = constrains1.cc

constraints1_SOURCES = constraints1.cc

contains1_SOURCES = contains1.cc
contains2_SOURCES = contains2.cc

containsintegerpoint1_SOURCES = containsintegerpoint1.cc

disjoint1_SOURCES = disjoint1.cc
disjoint2_SOURCES = disjoint2.cc

dropsomenonintegerpoints1_SOURCES = dropsomenonintegerpoints1.cc
dropsomenonintegerpoints2_SOURCES = dropsomenonintegerpoints2.cc

dualhypercubes_SOURCES = dualhypercubes.cc

empty1_SOURCES = empty1.cc

equals1_SOURCES = equals1.cc

exceptions1_SOURCES = exceptions1.cc
exceptions2_SOURCES = exceptions2.cc
exceptions3_SOURCES = exceptions3.cc

expandspacedim1_SOURCES = expandspacedim1.cc
expandspacedim2_SOURCES = expandspacedim2.cc

foldspacedims1_SOURCES = foldspacedims1.cc
foldspacedims2_SOURCES = foldspacedims2.cc

frequency1_SOURCES = frequency1.cc

frombdshape1_SOURCES = frombdshape1.cc

frombox1_SOURCES = frombox1.cc
frombox2_SOURCES = frombox2.cc

fromgrid1_SOURCES = fromgrid1.cc

fromoctagonalshape1_SOURCES = fromoctagonalshape1.cc

generalizedaffineimage1_SOURCES = generalizedaffineimage1.cc
generalizedaffineimage2_SOURCES = generalizedaffineimage2.cc

generalizedaffinepreimage1_SOURCES = generalizedaffinepreimage1.cc
generalizedaffinepreimage2_SOURCES = generalizedaffinepreimage2.cc

generators1_SOURCES = generators1.cc

geomcovers1_SOURCES = geomcovers1.cc

h79widening1_SOURCES = h79widening1.cc
h79widening2_SOURCES = h79widening2.cc

hybrid_SOURCES = hybrid.cc

intersection1_SOURCES = intersection1.cc

simplifyusingcontext1_SOURCES = simplifyusingcontext1.cc

limitedbhrz03extrapolation1_SOURCES = limitedbhrz03extrapolation1.cc

limitedh79extrapolation1_SOURCES = limitedh79extrapolation1.cc

linearpartition1_SOURCES = linearpartition1.cc

linearexpression1_SOURCES = linearexpression1.cc

linearsystem1_SOURCES = linearsystem1.cc

mapspacedims1_SOURCES = mapspacedims1.cc

matrix1_SOURCES = matrix1.cc

max_min1_SOURCES = max_min1.cc

maxspacedim1_SOURCES = maxspacedim1.cc

mc91_SOURCES = mc91.cc

membytes1_SOURCES = membytes1.cc

memory2_SOURCES = memory2.cc

minconstraints1_SOURCES = minconstraints1.cc
minconstraints2_SOURCES = minconstraints2.cc

mingenerators1_SOURCES = mingenerators1.cc
mingenerators2_SOURCES = mingenerators2.cc

numberinput1_SOURCES = numberinput1.cc

nncminimize1_SOURCES = nncminimize1.cc
nncminimize2_SOURCES = nncminimize2.cc

nncpostimeelapse1_SOURCES = nncpostimeelapse1.cc

onepoint_SOURCES = onepoint.cc

permute_SOURCES = permute.cc

polydifference1_SOURCES = polydifference1.cc
polydifference2_SOURCES = polydifference2.cc

polyhull1_SOURCES = polyhull1.cc
polyhull2_SOURCES = polyhull2.cc

polyhullifexact1_SOURCES = polyhullifexact1.cc
polyhullifexact2_SOURCES = polyhullifexact2.cc

randphull1_SOURCES = randphull1.cc

refinewithconstraint1_SOURCES = refinewithconstraint1.cc
refinewithconstraints1_SOURCES = refinewithconstraints1.cc
refinewithcongruence1_SOURCES = refinewithcongruence1.cc
refinewithcongruences1_SOURCES = refinewithcongruences1.cc

relations1_SOURCES = relations1.cc
relations2_SOURCES = relations2.cc
relations3_SOURCES = relations3.cc

removespacedims1_SOURCES = removespacedims1.cc
removespacedims2_SOURCES = removespacedims2.cc

smm1_SOURCES = smm1.cc

sparserow1_SOURCES = sparserow1.cc

termination1_SOURCES = termination1.cc
termination2_SOURCES = termination2.cc

timeelapse1_SOURCES = timeelapse1.cc
timeelapse2_SOURCES = timeelapse2.cc

topclosed1_SOURCES = topclosed1.cc

topclosure1_SOURCES = topclosure1.cc

unconstrain1_SOURCES = unconstrain1.cc

universe1_SOURCES = universe1.cc
universe2_SOURCES = universe2.cc

variablesset1_SOURCES = variablesset1.cc

watchdog1_SOURCES = watchdog1.cc

weightwatch1_SOURCES = weightwatch1.cc

wrap1_SOURCES = wrap1.cc
wrap2_SOURCES = wrap2.cc

writeconsys1_SOURCES = writeconsys1.cc

writegensys1_SOURCES = writegensys1.cc

writepolyhedron1_SOURCES = writepolyhedron1.cc
writepolyhedron2_SOURCES = writepolyhedron2.cc

writerelation1_SOURCES = writerelation1.cc

writevariable1_SOURCES = writevariable1.cc

#
# Sources and compilation flags for the derived check programs
#

DERIVED_CXXFLAGS = -DDERIVED_TEST $(AM_CXXFLAGS)

nnc_addcongruence1_SOURCES = addcongruence1.cc
nnc_addcongruence1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_addcongruences1_SOURCES = addcongruences1.cc
nnc_addcongruences1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_addconstraint1_SOURCES = addconstraint1.cc
nnc_addconstraint1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_addconstraints1_SOURCES = addconstraints1.cc
nnc_addconstraints1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_addgenerator1_SOURCES = addgenerator1.cc
nnc_addgenerator1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_addgenerators1_SOURCES = addgenerators1.cc
nnc_addgenerators1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_addspacedims1_SOURCES = addspacedims1.cc
nnc_addspacedims1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_affineimage1_SOURCES = affineimage1.cc
nnc_affineimage1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_affinepreimage1_SOURCES = affinepreimage1.cc
nnc_affinepreimage1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_affinetrans_SOURCES = affinetrans.cc
nnc_affinetrans_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_append1_SOURCES = append1.cc
nnc_append1_CXXFLAGS = $(DERIVED_CXXFLAGS)
nnc_append2_SOURCES = append2.cc
nnc_append2_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_ascii_dump_load1_SOURCES = ascii_dump_load1.cc
nnc_ascii_dump_load1_CXXFLAGS = $(DERIVED_CXXFLAGS)
nnc_ascii_dump_load2_SOURCES = ascii_dump_load2.cc
nnc_ascii_dump_load2_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_bgp99extrapolation1_SOURCES = bgp99extrapolation1.cc
nnc_bgp99extrapolation1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_bhrz03widening1_SOURCES = bhrz03widening1.cc
nnc_bhrz03widening1_CXXFLAGS = $(DERIVED_CXXFLAGS)
nnc_bhrz03widening2_SOURCES = bhrz03widening2.cc
nnc_bhrz03widening2_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_bounded1_SOURCES = bounded1.cc
nnc_bounded1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_boundedaffineimage1_SOURCES = boundedaffineimage1.cc
nnc_boundedaffineimage1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_boundedaffinepreimage1_SOURCES = boundedaffinepreimage1.cc
nnc_boundedaffinepreimage1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_boundedbhrz03extrapolation1_SOURCES = boundedbhrz03extrapolation1.cc
nnc_boundedvhrz03extrapolation1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_boundedh79extrapolation1_SOURCES = boundedh79extrapolation1.cc
nnc_boundedh79extrapolation1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_bounds1_SOURCES = bounds1.cc
nnc_bounds1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_concatenate1_SOURCES = concatenate1.cc
nnc_concatenate1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_congruences1_SOURCES = congruences1.cc
nnc_congruences1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_constrains1_SOURCES = constrains1.cc
nnc_constrains1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_constraints1_SOURCES = constraints1.cc
nnc_constraints1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_contains1_SOURCES = contains1.cc
nnc_contains1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_containsintegerpoint1_SOURCES = containsintegerpoint1.cc
nnc_containsintegerpoint1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_disjoint1_SOURCES = disjoint1.cc
nnc_disjoint1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_dropsomenonintegerpoints1_SOURCES = dropsomenonintegerpoints1.cc
nnc_dropsomenonintegerpoints1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_empty1_SOURCES = empty1.cc
nnc_empty1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_equals1_SOURCES = equals1.cc
nnc_equals1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_exceptions1_SOURCES = exceptions1.cc
nnc_exceptions1_CXXFLAGS = $(DERIVED_CXXFLAGS)
nnc_exceptions3_SOURCES = exceptions3.cc
nnc_exceptions3_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_expandspacedim1_SOURCES = expandspacedim1.cc
nnc_expandspacedim1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_foldspacedims1_SOURCES = foldspacedims1.cc
nnc_foldspacedims1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_frombdshape1_SOURCES = frombdshape1.cc
nnc_frombdshape1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_frombox1_SOURCES = frombox1.cc
nnc_frombox1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_fromgrid1_SOURCES = fromgrid1.cc
nnc_fromgrid1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_fromoctagonalshape1_SOURCES = fromoctagonalshape1.cc
nnc_fromoctagonalshape1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_generalizedaffineimage1_SOURCES = generalizedaffineimage1.cc
nnc_generalizedaffineimage1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_generalizedaffinepreimage1_SOURCES = generalizedaffinepreimage1.cc
nnc_generalizedaffinepreimage1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_generators1_SOURCES = generators1.cc
nnc_generators1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_geomcovers1_SOURCES = geomcovers1.cc
nnc_geomcovers1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_h79widening1_SOURCES = h79widening1.cc
nnc_h79widening1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_intersection1_SOURCES = intersection1.cc
nnc_intersection1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_simplifyusingcontext1_SOURCES = simplifyusingcontext1.cc
nnc_simplifyusingcontext1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_limitedbhrz03extrapolation1_SOURCES = limitedbhrz03extrapolation1.cc
nnc_limitedbhrz03extrapolation1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_limitedh79extrapolation1_SOURCES = limitedh79extrapolation1.cc
nnc_limitedh79extrapolation1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_linearpartition1_SOURCES = linearpartition1.cc
nnc_linearpartition1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_mapspacedims1_SOURCES = mapspacedims1.cc
nnc_mapspacedims1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_max_min1_SOURCES = max_min1.cc
nnc_max_min1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_mc91_SOURCES = mc91.cc
nnc_mc91_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_membytes1_SOURCES = membytes1.cc
nnc_membytes1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_minconstraints1_SOURCES = minconstraints1.cc
nnc_minconstraints1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_mingenerators1_SOURCES = mingenerators1.cc
nnc_mingenerators1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_onepoint_SOURCES = onepoint.cc
nnc_onepoint_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_permute_SOURCES = permute.cc
nnc_permute_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_polydifference1_SOURCES = polydifference1.cc
nnc_polydifference1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_polyhull1_SOURCES = polyhull1.cc
nnc_polyhull1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_polyhullifexact1_SOURCES = polyhullifexact1.cc
nnc_polyhullifexact1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_randphull1_SOURCES = randphull1.cc
nnc_randphull1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_relations1_SOURCES = relations1.cc
nnc_relations1_CXXFLAGS = $(DERIVED_CXXFLAGS)
nnc_relations2_SOURCES = relations2.cc
nnc_relations2_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_removespacedims1_SOURCES = removespacedims1.cc
nnc_removespacedims1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_smm1_SOURCES = smm1.cc
nnc_smm1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_timeelapse1_SOURCES = timeelapse1.cc
nnc_timeelapse1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_unconstrain1_SOURCES = unconstrain1.cc
nnc_unconstrain1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_universe1_SOURCES = universe1.cc
nnc_universe1_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_wrap1_SOURCES = wrap1.cc
nnc_wrap1_CXXFLAGS = $(DERIVED_CXXFLAGS)
nnc_wrap2_SOURCES = wrap2.cc
nnc_wrap2_CXXFLAGS = $(DERIVED_CXXFLAGS)

nnc_writepolyhedron1_SOURCES = writepolyhedron1.cc
nnc_writepolyhedron1_CXXFLAGS = $(DERIVED_CXXFLAGS)

#
# Sources for the special tests.
#

memory1_SRCS = memory1.cc


if !VALGRIND_TESTS_ENABLED
# The `memory1' test is currently disabled.  The reason is that there
# is currently no reliable way to make it work, at least on Linux.
# The reason is that memory is limited using RLIMIT_AS, which limits
# brk, mmap and the stack: when we run out of stack, then of course
# we crash.  So RLIMIT_AS cannot be used.  In principle, we could use
# RLIMIT_DATA, which limits brk only.  However, glibc uses mmap when
# it cannot obtain memory from brk (and perhaps in other cases as well),
# but mmap is not limited by RLIMIT_DATA.  In summary, RLIMIT_AS limits
# too much (in particular the stack, that should not be limited);
# RLIMIT_DATA limits too little (it does not limit mmap, as we would like
# it to).

#VALGRIND_BRITTLE_TESTS = memory1

#memory1_SOURCES = $(memory1_SRCS)

endif !VALGRIND_TESTS_ENABLED


TESTS = \
$(ORIGINAL_TESTS) \
$(DERIVED_TESTS) \
$(VALGRIND_BRITTLE_TESTS)

XFAIL_TESTS =

check_PROGRAMS = $(TESTS) $(BUGS)

EXTRA_DIST = $(memory1_SRCS)

BUGS =

MOSTLYCLEANFILES = \
ascii_dump_load1.dat \
ascii_dump_load2.dat \
ascii_dump_load3.dat \
densematrix1.dat \
linearsystem1.dat \
writegensys1.dat \
writepolyhedron1.dat \
writepolyhedron2.dat \
writevariable1.dat

$(top_builddir)/utils/libppl_utils.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils libppl_utils.a

$(top_builddir)/tests/libppl_tests.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/tests libppl_tests.a

$(top_builddir)/src/libppl.la:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/src libppl.la
