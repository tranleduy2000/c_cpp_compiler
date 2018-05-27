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

EXTRA_DIST = \
clpq.pl script_clpq \
clpq2.pl script_clpq2 script_clpq2_int8 \
$(CLPQ_TESTS) \
pl_check.pl \
ppl_prolog_generated_test_common.pl \
ppl_interface_generator_prolog_generated_test_pl.m4 \
ppl_interface_generator_prolog_generated_test_pl_code.m4 \
expected_clpq_mpz expected_clpq2_mpz expected_pchk_mpz \
expected_clpq_int8 expected_clpq2_int8 expected_pchk_int8 \
expected_clpq_int16 expected_clpq2_int16 expected_pchk_int16 \
expected_clpq_int32 expected_clpq2_int32 expected_pchk_int32 \
expected_clpq_int64 expected_clpq2_int64 expected_pchk_int64 \
expected_clpq_mpz_a expected_clpq2_mpz_a expected_pchk_mpz_a \
expected_clpq_int8_a expected_clpq2_int8_a expected_pchk_int8_a \
expected_clpq_int16_a expected_clpq2_int16_a expected_pchk_int16_a \
expected_clpq_int32_a expected_clpq2_int32_a expected_pchk_int32_a \
expected_clpq_int64_a expected_clpq2_int64_a expected_pchk_int64_a \
expected_pgt

check-local: ppl_prolog_generated_test.stamp

interface_generator_dependencies = \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_prolog_systems.m4 \
../ppl_interface_generator_prolog_procedure_generators.m4 \
ppl_interface_generator_prolog_generated_test_pl.m4 \
ppl_interface_generator_prolog_generated_test_pl_code.m4

ppl_prolog_generated_test.stamp: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. \
		-I$(srcdir) -I$(srcdir)/.. -I$(srcdir)/../.. \
		$(srcdir)/ppl_interface_generator_prolog_generated_test_pl.m4 \
			> ppl_prolog_generated_test_blob
	$(top_srcdir)/utils/cm_cleaner.sh ./ppl_prolog_generated_test_blob
	$(top_srcdir)/utils/cm_splitter.sh ./ppl_prolog_generated_test_blob
	rm -f ppl_prolog_generated_test_blob
	echo timestamp >$@

CLEANFILES = \
@required_instantiations_prolog_generated_test_sources@ \
ppl_prolog_generated_test.stamp \
ppl_prolog_generated_test_main.pl

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4
