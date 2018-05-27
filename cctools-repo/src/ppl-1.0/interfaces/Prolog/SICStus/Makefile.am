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

SICSTUS_CFLI_FILES = sicstus_cfli.h sicstus_cfli.ic sicstus_cfli.cc

EXTRA_DIST = \
ppl_interface_generator_sicstus_sd_cc.m4 \
$(SICSTUS_CFLI_FILES) \
ppl_sicstus.pl \
sp_clpq.pl \
sp_pl_check.pl \
sp_prolog_generated_test.pl

AM_CPPFLAGS = \
-I$(top_srcdir)/interfaces \
-I$(top_srcdir)/interfaces/Prolog \
-I$(top_buildir)/interfaces/Prolog \
-I$(top_builddir)/src \
$(SICSTUS_PROLOG_INCLUDE_OPTIONS) \
@extra_includes@

interface_generator_dependencies = \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_prolog_procedure_generators.m4 \
../ppl_interface_generator_prolog_systems.m4 \
ppl_interface_generator_sicstus_sd_cc.m4

ppl_sicstus_sd.cc: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. -I$(srcdir)/.. -I$(srcdir)/../.. \
           $(srcdir)/ppl_interface_generator_sicstus_sd_cc.m4 > $@

bin_PROGRAMS = ppl_sicstus
ppl_sicstus_SOURCES =


if ENABLE_SHARED

pkgdata_DATA = ppl_sicstus.pl ppl_sicstus.s.o

ppl_sicstus.so: ppl_sicstus.pl $(top_builddir)/src/libppl.la libppl_sicstus.la
	$(LIBTOOL) --mode=link --tag=CXX \
		splfr --cflag="$(AM_CPPFLAGS)" -o $@ \
			$(srcdir)/ppl_sicstus.pl \
			libppl_sicstus.la \
			@extra_libraries@

#			@required_instantiations_prolog_cxx_objects@

install-data-local: ppl_sicstus.so
	$(LIBTOOL) --mode=install $(INSTALL) $(INSTALL_STRIP_FLAG) \
		$< $(DESTDIR)$(pkglibdir)/$<

else !ENABLE_SHARED

pkgdata_DATA = ppl_sicstus.pl

endif !ENABLE_SHARED

noinst_LTLIBRARIES = libppl_sicstus.la

libppl_sicstus_la_SOURCES = \
ppl_prolog_sysdep.hh \
sicstus_efli.hh \
sicstus_efli.cc

nodist_libppl_sicstus_la_SOURCES = \
ppl_sicstus_sd.cc \
../ppl_prolog_common.cc

nodist_EXTRA_libppl_sicstus_la_SOURCES = \
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

libppl_sicstus_la_LIBADD = \
@required_instantiations_prolog_cxx_objects@ \
$(top_builddir)/src/libppl.la \
@extra_libraries@

libppl_sicstus_la_DEPENDENCIES = \
@required_instantiations_prolog_cxx_objects@

libppl_sicstus_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-module \
-avoid-version

ppl_sicstus$(EXEEXT): ppl_sicstus.s.o $(top_builddir)/src/libppl.la
	$(LIBTOOL) --mode=link --tag=CXX \
		spld --main=prolog \
			--cflag=`echo '$(AM_CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)' | tr " " ","` \
			--resources=ppl_sicstus.s.o -o $@ \
			$(top_builddir)/src/libppl.la \
			@extra_libraries@

if ENABLE_STATIC

ppl_sicstus.s.o: ppl_sicstus.pl libppl_sicstus.la
	splfr --static --cflag="$(AM_CPPFLAGS)" -o $@ \
		$(srcdir)/ppl_sicstus.pl \
		ppl_sicstus_sd.o \
		sicstus_efli.o \
		ppl_prolog_common.o \
		`echo @required_instantiations_prolog_cxx_objects@ | $(SED) -e "s/\.lo/.o/g"`

else !ENABLE_STATIC

ppl_sicstus.s.o: ppl_sicstus.pl libppl_sicstus.la
	splfr --cflag="$(AM_CPPFLAGS)" -o $@ \
		$(srcdir)/ppl_sicstus.pl \
		.libs/ppl_sicstus_sd.o \
		.libs/sicstus_efli.o \
		.libs/ppl_prolog_common.o \
		`echo @required_instantiations_prolog_cxx_objects@ | $(SED) -e "s/\([^. ]*\)\.lo/.libs\/\1.o/g"`

endif !ENABLE_STATIC

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

pl_check_test: ppl_sicstus ppl_sicstus.pl sp_pl_check.pl ../tests/pl_check.pl $(EXPECTED_PCHK)
	cp -f $(srcdir)/../tests/pl_check.pl .
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		./ppl_sicstus -l $(srcdir)/sp_pl_check.pl \
        --goal "main."  \
			>obtained_pchk
	rm -f pl_check.pl
	diff -u --ignore-all-space $(EXPECTED_PCHK) obtained_pchk

$(CLPQ_TESTS):
	cp -f $(srcdir)/../tests/$@ .

# FIXME: the last prerequisite is to get around a bug of `spld': see
# http://www.cs.unipr.it/pipermail/ppl-devel/2006-January/007610.html
clpq_test: ppl_sicstus ppl_sicstus.pl sp_clpq.pl ../tests/clpq.pl ../tests/script_clpq $(EXPECTED_CLPQ) $(CLPQ_TESTS) | pl_check_test
	$(LIBTOOL) --mode=execute \
	  -dlopen ../../../src/libppl.la \
	  ./ppl_sicstus -l $(srcdir)/sp_clpq.pl \
	    --goal "main('$(top_srcdir)/interfaces/Prolog/tests/clpq')." \
	      <$(srcdir)/../tests/script_clpq >obtained_clpq
	diff -u --ignore-all-space $(EXPECTED_CLPQ) obtained_clpq

# FIXME: the last prerequisite is to get around a bug of `spld': see
# http://www.cs.unipr.it/pipermail/ppl-devel/2006-January/007610.html
clpq2_test: ppl_sicstus ppl_sicstus.pl sp_clpq.pl ../tests/clpq2.pl ../tests/script_clpq2 $(EXPECTED_CLPQ2) $(CLPQ_TESTS) | clpq_test
	$(LIBTOOL) --mode=execute \
          -dlopen ../../../src/libppl.la \
	  ./ppl_sicstus -l $(srcdir)/sp_clpq.pl \
	    --goal "main('$(top_srcdir)/interfaces/Prolog/tests/clpq2')." \
	      <$(srcdir)/../tests/script_clpq2 >obtained_clpq2
	diff -u --ignore-all-space $(EXPECTED_CLPQ2) obtained_clpq2

ppl_prolog_generated_test: ppl_sicstus ppl_sicstus.pl sp_prolog_generated_test.pl ../tests/ppl_prolog_generated_test_main.pl ../tests/expected_pgt
	cp -f $(srcdir)/../tests/ppl_prolog_generated_test_common.pl .
	cp -f ../tests/ppl_prolog_generated_test_main.pl .
	$(LIBTOOL) --mode=execute \
		-dlopen ../../../src/libppl.la \
		./ppl_sicstus -l $(srcdir)/sp_prolog_generated_test.pl \
        --goal "main."  \
			>obtained_pgt
	rm -f ppl_prolog_generated_test_common.pl
	rm -f ppl_prolog_generated_test_main.pl
	diff -u --ignore-all-space $(srcdir)/../tests/expected_pgt obtained_pgt

CLEANFILES = \
ppl_sicstus_sd.cc

MOSTLYCLEANFILES = \
@required_instantiations_prolog_cxx_objects@ \
ppl_sicstus.s.o ppl_sicstus.so \
$(CLPQ_TESTS) \
obtained_clpq obtained_clpq2 obtained_pchk obtained_pgt

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

# A selection of these files, which depends on the configure options,
# is built by the `ppl_prolog_domains.cc.stamp' in parent's directory
# makefile.
$(nodist_EXTRA_libppl_sicstus_la_SOURCES):
../tests/ppl_prolog_generated_test_main.pl:

# FIXME: check whether the dependencies from `libppl.la' in this Makefile.am
# are actually required.
$(top_builddir)/src/libppl.la:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/src libppl.la
