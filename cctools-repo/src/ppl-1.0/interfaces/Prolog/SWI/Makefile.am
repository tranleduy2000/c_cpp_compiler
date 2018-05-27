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
$(VALGRIND) --tool=memcheck -q --trace-children=yes --leak-check=yes \
--num-callers=40 --leak-resolution=high \
--suppressions=$(top_srcdir)/tests/valgrind_suppressions

else !VALGRIND_TESTS_ENABLED

CHECKER =

endif !VALGRIND_TESTS_ENABLED

SWI_CFLI_FILES = swi_cfli.hh

EXTRA_DIST = \
ppl_interface_generator_swiprolog_cc.m4 \
$(SWI_CFLI_FILES) \
ppl_swiprolog.pl \
pl_clpq.cc \
pl_clpq.pl \
swi_pl_check.pl \
swi_prolog_generated_test.pl \
README.swiprolog

AM_CPPFLAGS = \
-I$(top_srcdir)/interfaces \
-I$(top_srcdir)/interfaces/Prolog \
-I$(top_buildir)/interfaces/Prolog \
-I$(top_builddir)/src \
$(SWI_PROLOG_INCLUDE_OPTIONS) \
@extra_includes@

interface_generator_dependencies = \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_prolog_procedure_generators.m4 \
../ppl_interface_generator_prolog_systems.m4 \
ppl_interface_generator_swiprolog_cc.m4

ppl_swiprolog.cc: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_swiprolog_cc.m4 > $@

pkglib_LTLIBRARIES = libppl_swiprolog.la

libppl_swiprolog_la_SOURCES = \
ppl_prolog_sysdep.hh \
swi_efli.hh \
swi_efli.cc

nodist_libppl_swiprolog_la_SOURCES = \
ppl_swiprolog.cc \
../ppl_prolog_common.cc

nodist_EXTRA_libppl_swiprolog_la_SOURCES = \
../ppl_prolog_Int8_Box.cc \
../ppl_prolog_Int16_Box.cc \
../ppl_prolog_Int32_Box.cc \
../ppl_prolog_Int64_Box.cc \
../ppl_prolog_Uint8_Box.cc \
../ppl_prolog_Uint16_Box.cc \
../ppl_prolog_Uint32_Box.cc \
../ppl_prolog_Uint64_Box.cc \
../ppl_prolog_Float_Box.cc \
../ppl_prolog_Double_Box.cc \
../ppl_prolog_Long_Double_Box.cc \
../ppl_prolog_Rational_Box.cc \
../ppl_prolog_Z_Box.cc \
../ppl_prolog_BD_Shape_int8_t.cc \
../ppl_prolog_BD_Shape_int16_t.cc \
../ppl_prolog_BD_Shape_int32_t.cc \
../ppl_prolog_BD_Shape_int64_t.cc \
../ppl_prolog_BD_Shape_float.cc \
../ppl_prolog_BD_Shape_double.cc \
../ppl_prolog_BD_Shape_long_double.cc \
../ppl_prolog_BD_Shape_mpq_class.cc \
../ppl_prolog_BD_Shape_mpz_class.cc \
../ppl_prolog_Octagonal_Shape_int8_t.cc \
../ppl_prolog_Octagonal_Shape_int16_t.cc \
../ppl_prolog_Octagonal_Shape_int32_t.cc \
../ppl_prolog_Octagonal_Shape_int64_t.cc \
../ppl_prolog_Octagonal_Shape_float.cc \
../ppl_prolog_Octagonal_Shape_double.cc \
../ppl_prolog_Octagonal_Shape_long_double.cc \
../ppl_prolog_Octagonal_Shape_mpq_class.cc \
../ppl_prolog_Octagonal_Shape_mpz_class.cc \
../ppl_prolog_Polyhedron.cc \
../ppl_prolog_Grid.cc \
../ppl_prolog_Pointset_Powerset_Int8_Box.cc \
../ppl_prolog_Pointset_Powerset_Int16_Box.cc \
../ppl_prolog_Pointset_Powerset_Int32_Box.cc \
../ppl_prolog_Pointset_Powerset_Int64_Box.cc \
../ppl_prolog_Pointset_Powerset_Uint8_Box.cc \
../ppl_prolog_Pointset_Powerset_Uint16_Box.cc \
../ppl_prolog_Pointset_Powerset_Uint32_Box.cc \
../ppl_prolog_Pointset_Powerset_Uint64_Box.cc \
../ppl_prolog_Pointset_Powerset_Float_Box.cc \
../ppl_prolog_Pointset_Powerset_Double_Box.cc \
../ppl_prolog_Pointset_Powerset_Long_Double_Box.cc \
../ppl_prolog_Pointset_Powerset_Rational_Box.cc \
../ppl_prolog_Pointset_Powerset_Z_Box.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_int8_t.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_int16_t.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_int32_t.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_int64_t.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_float.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_double.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_long_double.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_mpq_class.cc \
../ppl_prolog_Pointset_Powerset_BD_Shape_mpz_class.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_int8_t.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_int16_t.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_int32_t.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_int64_t.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_float.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_double.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_long_double.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_mpq_class.cc \
../ppl_prolog_Pointset_Powerset_Octagonal_Shape_mpz_class.cc \
../ppl_prolog_Pointset_Powerset_C_Polyhedron.cc \
../ppl_prolog_Pointset_Powerset_NNC_Polyhedron.cc \
../ppl_prolog_Pointset_Powerset_Grid.cc \
../ppl_prolog_Constraints_Product_C_Polyhedron_Grid.cc \
../ppl_prolog_Constraints_Product_NNC_Polyhedron_Grid.cc \
../ppl_prolog_Constraints_Product_Grid_C_Polyhedron.cc \
../ppl_prolog_Constraints_Product_Grid_NNC_Polyhedron.cc \
../ppl_prolog_Shape_Preserving_Product_C_Polyhedron_Grid.cc \
../ppl_prolog_Shape_Preserving_Product_NNC_Polyhedron_Grid.cc \
../ppl_prolog_Shape_Preserving_Product_Grid_C_Polyhedron.cc \
../ppl_prolog_Shape_Preserving_Product_Grid_NNC_Polyhedron.cc

if NO_UNDEFINED

NO_UNDEFINED_FLAG = -no-undefined

endif NO_UNDEFINED

libppl_swiprolog_la_LIBADD = \
@required_instantiations_prolog_cxx_objects@ \
$(top_builddir)/src/libppl.la \
@extra_libraries@ \
$(SWI_PROLOG_LD_OPTIONS)

libppl_swiprolog_la_DEPENDENCIES = \
@required_instantiations_prolog_cxx_objects@

libppl_swiprolog_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-module \
-avoid-version

pkgdata_DATA = ppl_swiprolog.pl

CLPQ_TESTS_NONSTRICT = \
ack.clpq \
ackn.clpq \
fib.clpq \
mc91.clpq \
smm.clpq \
sumto.clpq \
tak.clpq

CLPQ_TESTS_STRICT = \
schedule.clpq

CLPQ_TESTS = $(CLPQ_TESTS_NONSTRICT) $(CLPQ_TESTS_STRICT)

check_PROGRAMS = ppl_pl pl_clpq pl_clpq2
pl_clpq_SOURCES = pl_clpq.cc
pl_clpq2_SOURCES = pl_clpq.cc

if HOST_OS_CYGWIN

# Under Cygwin, $(SWI_PROLOG_LD) gets the link options wrong:
# -lpl (PLLIB) comes before -lppl_swiprolog, with the result that all
# the symbols of the SWI-Prolog foreign interface are undefined.
# Repeating the link options works around this problem.
EXTRA_LINK_OPTIONS = $(SWI_PROLOG_LD_OPTIONS)

endif HOST_OS_CYGWIN

# Note: since `libppl_swiprolog' is a pkglib, we prefer statically
# linking it into `ppl_pl'.  Otherwise `ppl_pl' would not work unless
# the user instructs the dynamic linker to look into the pkglib
# directory.
if ENABLE_STATIC

LIBPPL_SWIPROLOG_LINK_OPTIONS = .libs/libppl_swiprolog.a

else !ENABLE_STATIC

LIBPPL_SWIPROLOG_LINK_OPTIONS = -L.libs -lppl_swiprolog

endif !ENABLE_STATIC

# Apart from the internal testing purposes, `ppl_pl' only makes
# sense when shared libraries are disabled.
if !ENABLE_SHARED
bin_PROGRAMS = ppl_pl
endif !ENABLE_SHARED

ppl_pl_SOURCES = ppl_pl.cc


ppl_pl$(EXEEXT): libppl_swiprolog.la ppl_pl.o
	$(SWI_PROLOG_LD) -pl $(SWI_PROLOG) -cc "$(CC)" -c++ "$(CXX)" -ld "$(CXX)" \
		-ld-options`echo '' $(AM_CXXFLAGS) $(CXXFLAGS) | tr " " "/"` \
		-o $@ $(LIBPPL_SWIPROLOG_LINK_OPTIONS) ppl_pl.o \
		-L$(top_builddir)/src/.libs \
		-lppl @extra_libraries@ $(EXTRA_LINK_OPTIONS)

pl_clpq$(EXEEXT): libppl_swiprolog.la pl_clpq.o pl_clpq.pl ../tests/clpq.pl
	$(SWI_PROLOG_LD) -pl $(SWI_PROLOG) -cc $(CC) -c++ $(CXX) -ld $(CXX) \
		-ld-options`echo '' $(AM_CXXFLAGS) $(CXXFLAGS) | tr " " "/"` \
		-o $@ $(LIBPPL_SWIPROLOG_LINK_OPTIONS) pl_clpq.o \
                $(srcdir)/pl_clpq.pl $(srcdir)/../tests/clpq.pl \
		-L$(top_builddir)/src/.libs \
		-lppl @extra_libraries@ $(EXTRA_LINK_OPTIONS)


pl_clpq2$(EXEEXT): libppl_swiprolog.la pl_clpq.o pl_clpq.pl ../tests/clpq2.pl
	$(SWI_PROLOG_LD) -pl $(SWI_PROLOG) -cc $(CC) -c++ $(CXX) -ld $(CXX) \
		-ld-options`echo '' $(AM_CXXFLAGS) $(CXXFLAGS) | tr " " "/"` \
		-o $@ $(LIBPPL_SWIPROLOG_LINK_OPTIONS) pl_clpq.o \
                $(srcdir)/pl_clpq.pl $(srcdir)/../tests/clpq2.pl \
		-L$(top_builddir)/src/.libs \
		-lppl @extra_libraries@ $(EXTRA_LINK_OPTIONS)

COEFFICIENT_MNEMONIC = @coefficient_mnemonic@

if ASSERTIONS_ENABLED

EXPECTED_CLPQ = $(srcdir)/../tests/expected_clpq_$(COEFFICIENT_MNEMONIC)_a
EXPECTED_CLPQ2 = $(srcdir)/../tests/expected_clpq2_$(COEFFICIENT_MNEMONIC)_a
EXPECTED_PCHK = $(srcdir)/../tests/expected_pchk_$(COEFFICIENT_MNEMONIC)_a

else !ASSERTIONS_ENABLED

EXPECTED_CLPQ = $(srcdir)/../tests/expected_clpq_$(COEFFICIENT_MNEMONIC)
EXPECTED_CLPQ2 = $(srcdir)/../tests/expected_clpq2_$(COEFFICIENT_MNEMONIC)
EXPECTED_PCHK = $(srcdir)/../tests/expected_pchk_$(COEFFICIENT_MNEMONIC)

endif !ASSERTIONS_ENABLED

check-local: pl_check_test clpq_test clpq2_test ppl_prolog_generated_test

pl_check_test: swi_pl_check.pl ../tests/pl_check.pl $(EXPECTED_PCHK)
	cp -f $(srcdir)/../tests/pl_check.pl .
	echo "ensure_loaded('$(srcdir)/swi_pl_check'). main." > script_pchk
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_swiprolog.la \
                $(CHECKER) ./ppl_pl < script_pchk
	rm -f pl_check.pl
	diff -u --ignore-all-space $(EXPECTED_PCHK) obtained_pchk

$(CLPQ_TESTS):
	cp -f $(srcdir)/../tests/$@ .

clpq_test: pl_clpq ../tests/script_clpq $(EXPECTED_CLPQ) $(CLPQ_TESTS)
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_swiprolog.la \
		$(CHECKER) ./pl_clpq <$(srcdir)/../tests/script_clpq >obtained_clpq
	diff -u --ignore-all-space $(EXPECTED_CLPQ) obtained_clpq

clpq2_test: pl_clpq2 ../tests/script_clpq2 $(EXPECTED_CLPQ2) $(CLPQ_TESTS)
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_swiprolog.la \
		$(CHECKER) ./pl_clpq2 <$(srcdir)/../tests/script_clpq2 >obtained_clpq2
	diff -u --ignore-all-space $(EXPECTED_CLPQ2) obtained_clpq2

ppl_prolog_generated_test: swi_prolog_generated_test.pl ../tests/ppl_prolog_generated_test_main.pl ../tests/ppl_prolog_generated_test_common.pl ../tests/expected_pgt
	cp -f $(srcdir)/../tests/ppl_prolog_generated_test_common.pl .
	cp -f ../tests/ppl_prolog_generated_test_main.pl .
	echo "ensure_loaded('$(srcdir)/swi_prolog_generated_test'). main." > script_pgt
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_swiprolog.la \
                $(CHECKER) ./ppl_pl <script_pgt
	rm -f ppl_prolog_generated_test_common.pl
	rm -f ppl_prolog_generated_test_main.pl
	diff -u --ignore-all-space $(srcdir)/../tests/expected_pgt obtained_pgt

CLEANFILES = \
ppl_swiprolog.cc

MOSTLYCLEANFILES = \
@required_instantiations_prolog_cxx_objects@ \
pl_clpq pl_clpq2 \
obtained_clpq obtained_clpq2 obtained_pchk obtained_pgt \
$(CLPQ_TESTS) \
script_pchk script_pgt

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

# A selection of these files, which depends on the configure options,
# is built by the `ppl_prolog_domains.cc.stamp' in parent's directory
# makefile.
$(nodist_EXTRA_libppl_swiprolog_la_SOURCES):
../tests/ppl_prolog_generated_test_main.pl:
