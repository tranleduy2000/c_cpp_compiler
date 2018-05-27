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

if VALGRIND_TESTS_ENABLED

CHECKER = \
$(VALGRIND) --tool=memcheck -q --trace-children=yes --leak-check=yes \
--num-callers=40 --leak-resolution=high \
--suppressions=$(top_srcdir)/tests/valgrind_suppressions

else !VALGRIND_TESTS_ENABLED

CHECKER =

endif !VALGRIND_TESTS_ENABLED

AM_CPPFLAGS = \
-I$(top_builddir) \
-I$(top_builddir)/src \
-I$(top_srcdir)/utils \
@extra_includes@

bin_PROGRAMS = ppl_pips
ppl_pips_SOURCES = ppl_pips.cc
ppl_pips_LDADD = \
$(top_builddir)/src/libppl.la \
$(top_builddir)/utils/libppl_utils.a \
@extra_libraries@

dist_man_MANS = \
ppl_pips.1

piplib_pips.o: $(srcdir)/ppl_pips.cc $(top_builddir)/utils/libppl_utils.a
	$(CXXCOMPILE) -c -o $@ -DUSE_PIPLIB $(srcdir)/ppl_pips.cc

piplib_pips: piplib_pips.o
	$(CXXLINK) $< -lpip -lgmpxx -lgmp $(top_builddir)/utils/libppl_utils.a

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

TOUGH_EXAMPLES_IF_ASSERTIONS_ENABLED =

EXPECTED = expected_$(COEFFICIENT_MNEMONIC)_a

else !ASSERTIONS_ENABLED

EXPECTED = expected_$(COEFFICIENT_MNEMONIC)

endif !ASSERTIONS_ENABLED

TOUGH_EXAMPLES = \
$(TOUGH_EXAMPLES_IF_ASSERTIONS_ENABLED)

check-local: ppl_pips$(EXEEXT)
	echo $(TOUGH_EXAMPLES) >tough_examples
	echo $(ECHO_N) "$(ECHO_C)" >obtained
	export LC_ALL=C; \
	for path in $(srcdir)/examples/*.dat; \
	do \
	  file=`basename $$path`; \
	  $(FGREP) $$file tough_examples >/dev/null 2>&1 && continue; \
	  echo $(ECHO_N) $$file " $(ECHO_C)" >>obtained; \
	  echo ppl_pips$(EXEEXT) -p $$path; \
	  $(CHECKER) ./ppl_pips$(EXEEXT) -p $$path \
            | tr -d '\015' | $(TEXT_MD5SUM) >>obtained; \
	done
	for path in $(srcdir)/examples/*.pip; \
	do \
	  file=`basename $$path`; \
	  $(FGREP) $$file tough_examples >/dev/null 2>&1 && continue; \
	  echo $(ECHO_N) $$file " $(ECHO_C)" >>obtained; \
	  echo ppl_pips$(EXEEXT) -P $$path; \
	   $(CHECKER) ./ppl_pips$(EXEEXT) -P $$path \
             | tr -d '\015' | $(TEXT_MD5SUM) >>obtained; \
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
