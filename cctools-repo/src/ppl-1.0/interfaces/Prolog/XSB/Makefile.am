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

XSB_CFLI_FILES = xsb_cfli.hh

EXTRA_DIST = \
ppl_interface_generator_xsb_cc.m4 \
ppl_interface_generator_xsb_hh.m4 \
ppl_interface_generator_xsb_H.m4 \
$(XSB_CFLI_FILES) \
xsb_clpq.P \
xsb_clpq2.P \
xsb_pl_check.P \
ppl_interface_generator_xsb_prolog_generated_test_P.m4

interface_generator_dependencies = \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_prolog_procedure_generators.m4 \
../ppl_interface_generator_prolog_systems.m4 \
ppl_interface_generator_xsb_prolog_generated_test_P.m4

ppl_xsb.H: $(interface_generator_dependencies) ppl_interface_generator_xsb_H.m4
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_xsb_H.m4 > $@

ppl_xsb.hh: $(interface_generator_dependencies) ppl_interface_generator_xsb_hh.m4
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_xsb_hh.m4 > $@

ppl_xsb.cc: $(interface_generator_dependencies) ppl_interface_generator_xsb_cc.m4
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_xsb_cc.m4 > $@

xsb_prolog_generated_test.P: $(interface_generator_dependencies) ppl_interface_generator_xsb_prolog_generated_test_P.m4
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_xsb_prolog_generated_test_P.m4 > $@

AM_CPPFLAGS = \
-I$(top_srcdir)/interfaces \
-I$(top_srcdir)/interfaces/Prolog \
-I$(top_buildir)/interfaces/Prolog \
-I$(top_builddir)/src \
$(XSB_PROLOG_INCLUDE_OPTIONS) \
@extra_includes@

noinst_LTLIBRARIES = libppl_xsb.la

libppl_xsb_la_SOURCES = \
ppl_prolog_sysdep.hh \
xsb_efli.hh \
xsb_efli.cc

nodist_libppl_xsb_la_SOURCES = \
ppl_xsb.hh \
../ppl_prolog_common.cc

BUILT_SOURCES = \
ppl_xsb.hh

nodist_EXTRA_libppl_xsb_la_SOURCES = \
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

libppl_xsb_la_LIBADD = \
@required_instantiations_prolog_cxx_objects@ \
$(top_builddir)/src/libppl.la \
@extra_libraries@

libppl_xsb_la_DEPENDENCIES = \
@required_instantiations_prolog_cxx_objects@

libppl_xsb_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-module \
-avoid-version

pkgdata_DATA = ppl_xsb.xwam

ppl_xsb.xwam: ppl_xsb.H ppl_xsb.cc libppl_xsb.la
	cp -f ppl_xsb.cc ppl_xsb.c
	echo "consult(ppl_xsb," \
	  "[cc('$(CXX)'), cc_opts('$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) " \
	    "$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -fPIC " \
	    "-L.libs -lppl_xsb" \
	    "-L../../../src/.libs')])." \
		| $(LIBTOOL) --mode=execute \
			-dlopen ../../../src/libppl.la \
			xsb
	rm -f ppl_xsb.c

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

$(CLPQ_TESTS):
	cp -f $(srcdir)/../tests/$@ .

check-local: libppl_xsb.la xsb_clpq.P ../tests/clpq.pl ../tests/script_clpq xsb_clpq2.P ../tests/clpq2.pl ../tests/script_clpq2 pl_check_test ppl_prolog_generated_test $(CLPQ_TESTS)
	if [ $(abs_srcdir) != `pwd` ]; then \
	  cp -f $(srcdir)/xsb_clpq.P . ; \
	fi
	cp -f $(srcdir)/../tests/clpq.pl .
	rm -f xsb_clpq.xwam
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		xsb xsb_clpq \
			<$(srcdir)/../tests/script_clpq >obtained_clpq
	rm -f clpq.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  rm -f xsb_clpq.P; \
	fi
	diff -u --ignore-all-space $(EXPECTED_CLPQ) obtained_clpq
	if [ $(abs_srcdir) != `pwd` ]; then \
	  cp -f $(srcdir)/xsb_clpq2.P . ; \
	fi
	cp -f $(srcdir)/../tests/clpq2.pl .
	rm -f xsb_clpq2.xwam
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		xsb xsb_clpq2 \
			<$(srcdir)/../tests/script_clpq2 >obtained_clpq2
	rm -f clpq2.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  rm -f xsb_clpq2.P; \
	fi
	diff -u --ignore-all-space $(EXPECTED_CLPQ2) obtained_clpq2

pl_check_test: libppl_xsb.la ppl_xsb.xwam xsb_pl_check.P ../tests/pl_check.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  cp -f $(srcdir)/xsb_pl_check.P . ; \
	fi
	cp -f $(srcdir)/../tests/pl_check.pl .
	rm -f xsb_pl_check.xwam
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		xsb xsb_pl_check \
			>obtained_pchk
	rm -f pl_check.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  rm -f xsb_pl_check.P; \
	fi
	diff -u --ignore-all-space $(EXPECTED_PCHK) obtained_pchk

ppl_prolog_generated_test: libppl_xsb.la ppl_xsb.xwam xsb_prolog_generated_test.P ../tests/ppl_prolog_generated_test_main.pl ../tests/expected_pgt
	cp -f $(srcdir)/../tests/ppl_prolog_generated_test_common.pl .
	cp -f ../tests/ppl_prolog_generated_test_main.pl .
	rm -f xsb_prolog_generated_test.xwam
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		xsb xsb_prolog_generated_test \
			>obtained_pgt
	rm -f ppl_prolog_generated_test_common.pl
	rm -f ppl_prolog_generated_test_main.pl
	diff -u --ignore-all-space $(srcdir)/../tests/expected_pgt obtained_pgt


CLEANFILES = \
ppl_xsb.H \
ppl_xsb.hh \
ppl_xsb.cc \
xsb_prolog_generated_test.P

MOSTLYCLEANFILES = \
@required_instantiations_prolog_cxx_objects@ \
ppl_xsb.xwam ppl_xsb.so \
xsb_efli.lo xsb_efli.o \
xsb_clpq.xwam xsb_clpq2.xwam xsb_pl_check.xwam xsb_prolog_generated_test.xwam \
$(CLPQ_TESTS) obtained_clpq obtained_clpq2 obtained_pchk obtained_pgt

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

# A selection of these files, which depends on the configure options,
# is built by the `ppl_prolog_domains.cc.stamp' in parent's directory
# makefile.
$(nodist_EXTRA_libppl_xsb_la_SOURCES):
../tests/ppl_prolog_generated_test_main.pl:
