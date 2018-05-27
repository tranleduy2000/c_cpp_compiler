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

CIAO_CFLI_FILES = ciao_cfli.hh

EXTRA_DIST = \
ppl_interface_generator_ciao_pl.m4 \
$(CIAO_CFLI_FILES) \
ciao_clpq.pl \
ciao_clpq2.pl \
ciao_pl_check.pl \
ppl_interface_generator_ciao_prolog_generated_test_pl.m4 \
README.ciao

if ENABLE_SHARED

AM_CPPFLAGS = \
-I$(top_srcdir)/interfaces \
-I$(top_srcdir)/interfaces/Prolog \
-I$(top_buildir)/interfaces/Prolog \
-I$(top_builddir)/src \
$(CIAO_PROLOG_INCLUDE_OPTIONS) \
@extra_includes@

interface_generator_dependencies = \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_prolog_procedure_generators.m4 \
../ppl_interface_generator_prolog_systems.m4 \
ppl_interface_generator_ciao_pl.m4 \
ppl_interface_generator_ciao_prolog_generated_test_pl.m4

pkglib_LTLIBRARIES = libppl_ciao.la

libppl_ciao_la_SOURCES = \
ppl_prolog_sysdep.hh \
ciao_efli.hh \
ciao_efli.cc

nodist_libppl_ciao_la_SOURCES = \
../ppl_prolog_common.cc

nodist_EXTRA_libppl_ciao_la_SOURCES = \
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

libppl_ciao_la_LIBADD = \
@required_instantiations_prolog_cxx_objects@ \
$(top_builddir)/src/libppl.la \
@extra_libraries@

libppl_ciao_la_DEPENDENCIES = \
@required_instantiations_prolog_cxx_objects@

libppl_ciao_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-module \
-avoid-version

pkgdata_DATA = ppl_ciao.po

ppl_ciao.pl: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_ciao_pl.m4 > $@

ciao_prolog_generated_test.pl: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_ciao_prolog_generated_test_pl.m4 > $@

ppl_ciao.po: ppl_ciao.pl
	ciaoc -c ppl_ciao.pl

# All the invocations of `ciaoc' to build `ciao_pl_check', `ciao_clpq',
# `ciao_clpq2' and 'ciao_prolog_generated_test' will try to build
# identical glue files (e.g., `ppl_ciao_LINUXi86.so').
# On parallel builds, this may cause errors.
# This is why we make `ciao_clpq' and `ciao_clpq2' depend on `ciao_pl_check':
# building the latter will build the glue files once and for all;  after
# that, `ciao_clpq' and `ciao_clpq2' can be built in parallel.
ciao_pl_check: .libs/libppl_ciao.so ppl_ciao.po ciao_pl_check.pl ../tests/pl_check.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  cp -f $(srcdir)/ciao_pl_check.pl .; \
	fi
	cp -f $(srcdir)/../tests/pl_check.pl .
	ciaoc -o $@ ciao_pl_check.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  rm -f ciao_pl_check.pl; \
	fi
	rm -f pl_check.pl

ciao_clpq: .libs/libppl_ciao.so ppl_ciao.po ciao_clpq.pl ../tests/clpq.pl | ciao_pl_check
	if [ $(abs_srcdir) != `pwd` ]; then \
	  cp -f $(srcdir)/ciao_clpq.pl .; \
	fi
	cp -f $(srcdir)/../tests/clpq.pl .
	ciaoc -o $@ ciao_clpq.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  rm -f ciao_clpq.pl; \
	fi
	rm -f clpq.pl

ciao_clpq2: .libs/libppl_ciao.so ppl_ciao.po ciao_clpq.pl ../tests/clpq2.pl | ciao_pl_check
	if [ $(abs_srcdir) != `pwd` ]; then \
	  cp -f $(srcdir)/ciao_clpq2.pl .; \
	fi
	cp -f $(srcdir)/../tests/clpq2.pl .
	ciaoc -o $@ ciao_clpq2.pl
	if [ $(abs_srcdir) != `pwd` ]; then \
	  rm -f ciao_clpq2.pl; \
	fi
	rm -f clpq2.pl


ciao_prolog_generated_test: .libs/libppl_ciao.so ppl_ciao.po ciao_prolog_generated_test.pl ../tests/ppl_prolog_generated_test_main.pl | ciao_pl_check
	cp -f $(srcdir)/../tests/ppl_prolog_generated_test_common.pl .
	cp -f ../tests/ppl_prolog_generated_test_main.pl .
	ciaoc -o $@ ciao_prolog_generated_test.pl
	rm -f ppl_prolog_generated_test_common.pl
	rm -f ppl_prolog_generated_test_main.pl

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

check-local: ppl_prolog_generated_test

pl_check_test: ciao_pl_check$(EXEEXT) .libs/libppl_ciao.so $(EXPECTED_PCHK)
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_ciao.la \
		./ciao_pl_check \
			 >obtained_pchk
	diff -u --ignore-all-space $(EXPECTED_PCHK) obtained_pchk

$(CLPQ_TESTS):
	cp -f $(srcdir)/../tests/$@ .

clpq_test: ciao_clpq$(EXEEXT) .libs/libppl_ciao.so ../tests/script_clpq $(EXPECTED_CLPQ) $(CLPQ_TESTS)
	GLOBALSTKSIZE=1000000 \
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_ciao.la \
		./ciao_clpq \
			<$(srcdir)/../tests/script_clpq >obtained_clpq
	diff -u --ignore-all-space $(EXPECTED_CLPQ) obtained_clpq

clpq2_test: ciao_clpq2$(EXEEXT) .libs/libppl_ciao.so ../tests/script_clpq2 $(EXPECTED_CLPQ2) $(CLPQ_TESTS)
	GLOBALSTKSIZE=100000 \
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_ciao.la \
		./ciao_clpq2 \
			<$(srcdir)/../tests/script_clpq2 >obtained_clpq2
	diff -u --ignore-all-space $(EXPECTED_CLPQ2) obtained_clpq2

ppl_prolog_generated_test: ciao_prolog_generated_test$(EXEEXT) .libs/libppl_ciao.so ../tests/expected_pgt
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		-dlopen libppl_ciao.la \
		./ciao_prolog_generated_test \
			 >obtained_pgt
	diff -u --ignore-all-space $(srcdir)/../tests/expected_pgt obtained_pgt

endif ENABLE_SHARED

CLEANFILES = \
ppl_ciao.pl

MOSTLYCLEANFILES = \
@required_instantiations_prolog_cxx_objects@ \
*.itf *.po *.c *.so \
ciao_clpq ciao_clpq2 ciao_pl_check ciao_prolog_generated_test \
obtained_clpq obtained_clpq2 obtained_pchk obtained_pgt \
$(CLPQ_TESTS)

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

# A selection of these files, which depends on the configure options,
# is built by the `ppl_prolog_domains.cc.stamp' in parent's directory
# makefile.
$(nodist_EXTRA_libppl_ciao_la_SOURCES):
../tests/ppl_prolog_generated_test_main.pl:

# FIXME: the dependencies from .libs/libppl_ciao.so in this Makefile.am
# are probably all wrong.
.libs/libppl_ciao.so: libppl_ciao.la
