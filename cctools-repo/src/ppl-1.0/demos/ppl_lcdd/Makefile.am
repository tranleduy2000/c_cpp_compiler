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

SUBDIRS = examples

AM_CPPFLAGS = \
-I$(top_builddir) \
-I$(top_builddir)/src \
-I$(top_srcdir)/utils \
@extra_includes@

bin_PROGRAMS = ppl_lcdd
ppl_lcdd_SOURCES = ppl_lcdd.cc
ppl_lcdd_LDADD = \
$(top_builddir)/src/libppl.la \
$(top_builddir)/utils/libppl_utils.a \
@extra_libraries@

dist_man_MANS = \
ppl_lcdd.1

polka_lcdd.o: $(srcdir)/ppl_lcdd.cc $(top_builddir)/utils/libppl_utils.a
	$(CXXCOMPILE) -c -o $@ -DUSE_POLKA $(srcdir)/ppl_lcdd.cc

polka_lcdd: polka_lcdd.o
	$(CXXLINK) $< -lpolkag -lgmpxx -lgmp $(top_builddir)/utils/libppl_utils.a


polylib_lcdd.o:	$(srcdir)/ppl_lcdd.cc
	$(CXXCOMPILE) -c -o $@ -DUSE_POLYLIB $(srcdir)/ppl_lcdd.cc

polylib_lcdd: polylib_lcdd.o $(top_builddir)/utils/libppl_utils.a
	$(CXXLINK) $< -lpolylibgmp -lgmpxx -lgmp $(top_builddir)/utils/libppl_utils.a

EXTRA_DIST = \
expected_mpz \
expected_mpz_a \
expected_int8 \
expected_int8_a \
expected_int16 \
expected_int16_a \
expected_int32 \
expected_int32_a \
expected_int64 \
expected_int64_a

if HAVE_MD5SUM

COEFFICIENT_MNEMONIC = @coefficient_mnemonic@

if ASSERTIONS_ENABLED

TOUGH_EXAMPLES_IF_ASSERTIONS_ENABLED = \
ccc6.ext \
ccp6.ext \
cut32_16.ext \
cyclic16-10.ext \
reg600-5_m.ext \
cyclic17_8.ine \
in6.ine \
in7.ine \
kq20_11_m.ine \
mit31-20.ine \
sampleh8.ine \
trunc10.ine

EXPECTED = expected_$(COEFFICIENT_MNEMONIC)_a

else !ASSERTIONS_ENABLED

EXPECTED = expected_$(COEFFICIENT_MNEMONIC)

endif !ASSERTIONS_ENABLED

TOUGH_EXAMPLES = \
cp7.ext \
cyclic25_13.ext \
prodst62.ext \
cp6.ine \
mit.ine \
mit288-281.ine \
mit41-16.ine \
mit708-9.ine \
mit71-61.ine \
mit90-86.ine \
$(TOUGH_EXAMPLES_IF_ASSERTIONS_ENABLED)

check-local: ppl_lcdd$(EXEEXT)
	echo $(TOUGH_EXAMPLES) >tough_examples
	echo $(ECHO_N) "$(ECHO_C)" >obtained
	export LC_ALL=C; \
	for path in $(srcdir)/examples/*.ext $(srcdir)/examples/*.ine; \
	do \
	  file=`basename $$path`; \
	  $(FGREP) $$file tough_examples >/dev/null 2>&1 && continue; \
	  echo $(ECHO_N) $$file " $(ECHO_C)" >>obtained; \
	  echo ppl_lcdd$(EXEEXT) $$path; \
	  ./ppl_lcdd$(EXEEXT) $$path | tr -d '\015' | $(TEXT_MD5SUM) \
            >>obtained; \
	done
	diff obtained $(srcdir)/$(EXPECTED)

MOSTLYCLEANFILES = \
tough_examples \
obtained

endif HAVE_MD5SUM

$(top_builddir)/utils/libppl_utils.a:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils libppl_utils.a

$(top_builddir)/src/libppl.la:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/src libppl.la
