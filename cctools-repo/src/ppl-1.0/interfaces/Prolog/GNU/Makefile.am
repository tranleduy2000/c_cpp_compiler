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

GPROLOG_CFLI_FILES = gprolog_cfli.hh

EXTRA_DIST = \
ppl_interface_generator_gprolog_pl.m4 \
$(GPROLOG_CFLI_FILES) \
gp_clpq.pl \
gnu_pl_check.pl \
gnu_prolog_generated_test.pl \
README.gprolog

AM_CPPFLAGS = \
-I$(top_srcdir)/interfaces \
-I$(top_srcdir)/interfaces/Prolog \
-I$(top_buildir)/interfaces/Prolog \
-I$(top_builddir)/src \
@extra_includes@

interface_generator_dependencies = \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_prolog_procedure_generators.m4 \
../ppl_interface_generator_prolog_systems.m4 \
ppl_interface_generator_gprolog_pl.m4

pkglib_LTLIBRARIES = libppl_gprolog.la

libppl_gprolog_la_SOURCES = \
ppl_prolog_sysdep.hh \
gprolog_efli.hh \
gprolog_efli.cc

nodist_libppl_gprolog_la_SOURCES = \
../ppl_prolog_common.cc

nodist_EXTRA_libppl_gprolog_la_SOURCES = \
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

libppl_gprolog_la_LIBADD = \
@required_instantiations_prolog_cxx_objects@ \
$(top_builddir)/src/libppl.la \
@extra_libraries@

libppl_gprolog_la_DEPENDENCIES = \
@required_instantiations_prolog_cxx_objects@

libppl_gprolog_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-module \
-avoid-version

ppl_gprolog.pl: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_gprolog_pl.m4 > $@

pkgdata_DATA = ppl_gprolog.pl

bin_PROGRAMS = ppl_gprolog

ppl_gprolog_SOURCES =


# Note: since `libppl_gprolog' is a pkglib, we prefer statically
# linking it into `ppl_pl'.  Otherwise `ppl_pl' would not work unless
# the user instructs the dynamic linker to look into the pkglib
# directory.
if ENABLE_STATIC

LIBPPL_GPROLOG = .libs/libppl_gprolog.a

else !ENABLE_STATIC

LIBPPL_GPROLOG = .libs/libppl_gprolog.so

endif !ENABLE_STATIC

ppl_gprolog$(EXEEXT): libppl_gprolog.la ppl_gprolog.pl
	gplc --c-compiler $(CXX) -o $@ \
		ppl_gprolog.pl $(LIBPPL_GPROLOG) \
		-L '$(AM_CXXFLAGS) $(CXXFLAGS) -L$(top_builddir)/src/.libs -lppl -lgmpxx -lgmp'

gp_clpq: libppl_gprolog.la ppl_gprolog.pl gp_clpq.pl ../tests/clpq.pl
	gplc --c-compiler $(CXX) -o $@ \
		ppl_gprolog.pl $(srcdir)/gp_clpq.pl \
		$(srcdir)/../tests/clpq.pl $(LIBPPL_GPROLOG) \
		-L '$(AM_CXXFLAGS) $(CXXFLAGS) -L$(top_builddir)/src/.libs -lppl -lgmpxx -lgmp'

gp_clpq2: libppl_gprolog.la ppl_gprolog.pl gp_clpq.pl ../tests/clpq2.pl
	gplc --c-compiler $(CXX) -o $@ \
		ppl_gprolog.pl $(srcdir)/gp_clpq.pl \
		$(srcdir)/../tests/clpq2.pl $(LIBPPL_GPROLOG) \
		-L '$(AM_CXXFLAGS) $(CXXFLAGS) -L$(top_builddir)/src/.libs -lppl -lgmpxx -lgmp'

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

SCRIPT_CLPQ= $(srcdir)/../tests/script_clpq

if USE_INT8

SCRIPT_CLPQ2 = $(srcdir)/../tests/script_clpq2_int8

else !USE_INT8

SCRIPT_CLPQ2 = $(srcdir)/../tests/script_clpq2

endif !USE_INT8

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

pl_check_test: gnu_pl_check.pl ../tests/pl_check.pl $(EXPECTED_PCHK)
	echo "['$(srcdir)/../tests/pl_check.pl', '$(srcdir)/gnu_pl_check.pl']." \
		>script_pchk
	echo "main." >>script_pchk
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_gprolog.la \
		./ppl_gprolog <script_pchk
	diff -u --ignore-all-space $(EXPECTED_PCHK) obtained_pchk

$(CLPQ_TESTS):
	cp -f $(srcdir)/../tests/$@ .

clpq_test: gp_clpq ../tests/script_clpq $(EXPECTED_CLPQ) $(CLPQ_TESTS)
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_gprolog.la \
		./gp_clpq <$(SCRIPT_CLPQ) >obtained_clpq
	diff -u --ignore-all-space $(EXPECTED_CLPQ) obtained_clpq

clpq2_test: gp_clpq2 ../tests/script_clpq2 $(EXPECTED_CLPQ2) $(CLPQ_TESTS)
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_gprolog.la \
		./gp_clpq2 <$(SCRIPT_CLPQ2) >obtained_clpq2
	diff -u --ignore-all-space $(EXPECTED_CLPQ2) obtained_clpq2

ppl_prolog_generated_test: gnu_prolog_generated_test.pl  ../tests/ppl_prolog_generated_test_main.pl ../tests/expected_pgt
	cp -f $(srcdir)/../tests/ppl_prolog_generated_test_common.pl .
	if [ $(abs_srcdir) != `pwd` ]; then \
          cp -f $(srcdir)/gnu_prolog_generated_test.pl . ; \
	fi
	cp -f ../tests/ppl_prolog_generated_test_main.pl .
	echo "['ppl_prolog_generated_test_main.pl', 'gnu_prolog_generated_test.pl']." \
		>script_pgt
	echo "main." >>script_pgt
	GLOBALSZ=131072 $(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_gprolog.la \
		./ppl_gprolog <script_pgt
	rm -f ppl_prolog_generated_test_common.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  rm -f gnu_prolog_generated_test.pl; \
        fi
	rm -f ppl_prolog_generated_test_main.pl
	diff -u --ignore-all-space $(srcdir)/../tests/expected_pgt obtained_pgt

CLEANFILES = \
ppl_gprolog.pl

MOSTLYCLEANFILES = \
@required_instantiations_prolog_cxx_objects@ \
gp_clpq gp_clpq2 \
obtained_clpq obtained_clpq2 obtained_pchk obtained_pgt \
$(CLPQ_TESTS) \
script_pchk script_pgt

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

# A selection of these files, which depends on the configure options,
# is built by the `ppl_prolog_domains.cc.stamp' in parent's directory
# makefile.
$(nodist_EXTRA_libppl_gprolog_la_SOURCES):
../tests/ppl_prolog_generated_test_main.pl:
