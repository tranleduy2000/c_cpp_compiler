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

YAP_CFLI_FILES = yap_cfli.hh

EXTRA_DIST = \
ppl_interface_generator_yap_cc.m4 \
$(YAP_CFLI_FILES) \
ppl_yap.pl \
yap_clpq.pl \
yap_clpq2.pl \
yap_pl_check.pl \
yap_prolog_generated_test.pl \
README.yap

interface_generator_dependencies = \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_prolog_procedure_generators.m4 \
../ppl_interface_generator_prolog_systems.m4 \
ppl_interface_generator_yap_cc.m4

ppl_yap.cc: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_yap_cc.m4 > $@

if ENABLE_SHARED

AM_CPPFLAGS = \
-I$(top_srcdir)/interfaces \
-I$(top_srcdir)/interfaces/Prolog \
-I$(top_buildir)/interfaces/Prolog \
-I$(top_builddir)/src \
$(YAP_PROLOG_INCLUDE_OPTIONS) \
@extra_includes@

if NO_UNDEFINED

NO_UNDEFINED_FLAG = -no-undefined

endif NO_UNDEFINED

pkglib_LTLIBRARIES = ppl_yap.la

# There is no point in generating the static version.
AM_LIBTOOLFLAGS = --tag=disable-static

ppl_yap_la_SOURCES = \
ppl_prolog_sysdep.hh \
yap_efli.hh \
yap_efli.cc

nodist_ppl_yap_la_SOURCES = \
ppl_yap.cc \
../ppl_prolog_common.cc

nodist_EXTRA_ppl_yap_la_SOURCES = \
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

ppl_yap_la_LIBADD = \
@required_instantiations_prolog_cxx_objects@ \
$(YAP_PROLOG_LD_OPTIONS) \
$(top_builddir)/src/libppl.la \
@extra_libraries@

ppl_yap_la_DEPENDENCIES = \
@required_instantiations_prolog_cxx_objects@

ppl_yap_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-module \
-avoid-version

pkgdata_DATA = ppl_yap.pl

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

pl_check_test: yap_pl_check.pl ppl_yap.pl ../tests/pl_check.pl $(EXPECTED_PCHK)
	echo "main." >script_pchk
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen ppl_yap.la \
		$(YAP_PROLOG) -l $(srcdir)/yap_pl_check.pl \
			<script_pchk >obtained_pchk
	diff -u --ignore-all-space $(EXPECTED_PCHK) obtained_pchk

$(CLPQ_TESTS):
	cp -f $(srcdir)/../tests/$@ .

clpq_test: yap_clpq.pl ppl_yap.pl ../tests/clpq.pl ../tests/script_clpq $(EXPECTED_CLPQ) $(CLPQ_TESTS)
	echo "main." >script_clpq
	cat $(srcdir)/../tests/script_clpq >>script_clpq
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen ppl_yap.la \
		$(YAP_PROLOG) -l $(srcdir)/yap_clpq.pl \
			<script_clpq >obtained_clpq
	diff -u --ignore-all-space $(EXPECTED_CLPQ) obtained_clpq

clpq2_test: yap_clpq2.pl ppl_yap.pl ../tests/clpq2.pl ../tests/script_clpq2 $(EXPECTED_CLPQ2) $(CLPQ_TESTS)
	echo "main." >script_clpq2
	cat $(srcdir)/../tests/script_clpq2 >>script_clpq2
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen ppl_yap.la \
		$(YAP_PROLOG) -l $(srcdir)/yap_clpq2.pl \
			<script_clpq2 >obtained_clpq2
	diff -u --ignore-all-space $(EXPECTED_CLPQ2) obtained_clpq2

ppl_prolog_generated_test: yap_prolog_generated_test.pl ppl_yap.pl  ../tests/expected_pgt
	cp -f $(srcdir)/../tests/ppl_prolog_generated_test_common.pl .
	if [ $(abs_srcdir) != `pwd` ]; then \
          cp -f $(srcdir)/yap_prolog_generated_test.pl . ; \
          cp -f $(srcdir)/ppl_yap.pl . ; \
	fi
	cp -f ../tests/ppl_prolog_generated_test_main.pl . ; \
	echo "main." >script_pgt
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen ppl_yap.la \
		$(YAP_PROLOG) -l yap_prolog_generated_test.pl \
			<script_pgt >obtained_pgt
	rm -f ppl_prolog_generated_test_common.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  rm -f yap_prolog_generated_test.pl; \
	  rm -f ppl_yap.pl; \
        fi
	rm -f ppl_prolog_generated_test_main.pl
	diff -u --ignore-all-space $(srcdir)/../tests/expected_pgt obtained_pgt

endif ENABLE_SHARED

CLEANFILES = \
ppl_yap.cc

MOSTLYCLEANFILES = \
@required_instantiations_prolog_cxx_objects@ \
script_clpq script_clpq2 script_pchk script_pgt \
obtained_clpq obtained_clpq2 obtained_pchk obtained_pgt \
$(CLPQ_TESTS)

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

# A selection of these files, which depends on the configure options,
# is built by the `ppl_prolog_domains.cc.stamp' in parent's directory
# makefile.
$(nodist_EXTRA_ppl_yap_la_SOURCES):
../tests/ppl_prolog_generated_test_main.pl:
