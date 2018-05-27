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

SUBDIRS = examples

EXTRA_DIST = \
ppl_lpsol.c \
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

dist_man_MANS = \
ppl_lpsol.1

if BUILD_C_INTERFACE
if HAVE_GLPK

AM_CPPFLAGS = \
-I$(top_builddir)/interfaces/C \
@extra_includes@

bin_PROGRAMS = ppl_lpsol

# dummy.cc serves the only purpose to force linking using the C++ compiler.
ppl_lpsol_SOURCES = \
ppl_lpsol.c \
dummy.cc

if GCC

if ICC

# We cannot use -std=c89 because of the definition of sigaction in <signal.h>.
ppl_lpsol_CFLAGS = -std=gnu89

else !ICC

# We cannot use -std=c89 because of the definition of sigaction in <signal.h>.
ppl_lpsol_CFLAGS = -pedantic -std=gnu89

endif !ICC

endif GCC

ppl_lpsol_LDADD = \
libglpk_set_d_eps.a \
-lglpk \
$(top_builddir)/interfaces/C/libppl_c.la \
$(top_builddir)/src/libppl.la \
@extra_libraries@

noinst_LIBRARIES = libglpk_set_d_eps.a
libglpk_set_d_eps_a_SOURCES = \
glpk_set_d_eps.c

COEFFICIENT_MNEMONIC = @coefficient_mnemonic@

if ASSERTIONS_ENABLED

TOUGH_EXAMPLES_FOR_ENUM_IF_ASSERTIONS_ENABLED = \
afiro.mps

TOUGH_EXAMPLES_FOR_SIMPLEX_IF_ASSERTIONS_ENABLED = \
opt1217.mps \
rout.mps

TOUGH_EXAMPLES_FOR_BB_SATISFIABILTY_IF_ASSERTIONS_ENABLED = \
mas74.mps \
mas76.mps \
modglob.mps \
noswot.mps \
opt1217.mps \
pk1.mps \
rout.mps

EXPECTED = expected_$(COEFFICIENT_MNEMONIC)_a

CHECK_OPTION =

else !ASSERTIONS_ENABLED

EXPECTED = expected_$(COEFFICIENT_MNEMONIC)

CHECK_OPTION = -c

endif !ASSERTIONS_ENABLED

TOUGH_EXAMPLES_FOR_ENUM = \
adlittle.mps \
blend.mps \
boeing1.mps \
boeing2.mps \
egout.mps \
kb2.mps \
lseu.mps \
markshare1.mps \
markshare2.mps \
mas74.mps \
mas76.mps \
modglob.mps \
noswot.mps \
opt1217.mps \
p0033.mps \
pk1.mps \
rout.mps \
sc105.mps \
sc50a.mps \
sc50b.mps \
ship08l.mps \
$(TOUGH_EXAMPLES_FOR_ENUM_IF_ASSERTIONS_ENABLED)

TOUGH_EXAMPLES_FOR_SIMPLEX = \
ship08l.mps\
$(TOUGH_EXAMPLES_FOR_SIMPLEX_IF_ASSERTIONS_ENABLED)

TOUGH_EXAMPLES_FOR_BB_SATISFIABILITY = \
$(TOUGH_EXAMPLES_FOR_BB_SATISFIABILTY_IF_ASSERTIONS_ENABLED)

TOUGH_EXAMPLES_FOR_BB_OPTIMALITY = \
egout.mps \
lseu.mps \
markshare1.mps \
markshare2.mps \
mas74.mps \
mas76.mps \
modglob.mps \
noswot.mps \
opt1217.mps \
pk1.mps \
rout.mps

LP_PROBLEMS = \
adlittle.mps \
afiro.mps \
bgprtr.mps \
blend.mps \
boeing1.mps \
boeing2.mps \
kb2.mps \
sample.mps \
sc105.mps \
sc50a.mps \
sc50b.mps \
ship08l.mps

DO_NOT_OUTPUT_OPTIMUM_LOCATION_PROBLEMS = \
adlittle.mps \
boeing1.mps \
boeing2.mps \
rout.mps

INDIVIDUALLY_CHECKED_PROBLEMS = \
boeing1.mps \
boeing2.mps \
ex12.mps \
mas74.mps \
modglob.mps

if HOST_OS_CYGWIN

DIFF_COMMAND = \
$(EGREP) "^Optimum value: " $(srcdir)/$(EXPECTED) >expected_optima \
&& $(EGREP) "^Optimum value: " obtained >obtained_optima \
&& diff -u expected_optima obtained_optima

else !HOST_OS_CYGWIN

if HOST_OS_DARWIN

DIFF_COMMAND = \
$(EGREP) "^Optimum value: " $(srcdir)/$(EXPECTED) >expected_optima \
&& $(EGREP) "^Optimum value: " obtained >obtained_optima \
&& diff -u expected_optima obtained_optima

else !HOST_OS_DARWIN

if HOST_OS_SOLARIS

DIFF_COMMAND = \
$(EGREP) "^Optimum value: " $(srcdir)/$(EXPECTED) \
| $(SED) -e "s/^Optimum value: \(-*[0-9\.][0-9\.][0-9\.][0-9\.][0-9\.][0-9\.][0-9\.][0-9\.][0-9\.]\)[0-9]*$/Optimum value: \1/g" >expected_optima \
&& $(EGREP) "^Optimum value: " obtained \
| $(SED) -e "s/^Optimum value: \(-*[0-9\.][0-9\.][0-9\.][0-9\.][0-9\.][0-9\.][0-9\\.][0-9\.][0-9\.]\)[0-9]*$/Optimum value: \1/g" >obtained_optima \
&& diff -u expected_optima obtained_optima

else !HOST_OS_SOLARIS

if HOST_OS_MINGW

DIFF_COMMAND = \
tr -d '\015' <obtained \
| sed -e "s/ppl_lpsol.exe/ppl_lpsol/g" -e "s/1e+012/1e+12/g" \
| diff $(srcdir)/$(EXPECTED) -

else !HOST_OS_MINGW

DIFF_COMMAND = diff -u $(srcdir)/$(EXPECTED) obtained

endif !HOST_OS_MINGW

endif !HOST_OS_SOLARIS

endif !HOST_OS_DARWIN

endif !HOST_OS_CYGWIN

# NOTE: use exact steepest-edge pricing (-p1) instead float-based one.
# This has an efficiency penalty, but it guarantees deterministic
# results across different architectures.
SIMPLEX = ./ppl_lpsol$(EXEEXT) -s -p1 $(CHECK_OPTION) -oobtained
SIMPLEX_NOCHECK = ./ppl_lpsol$(EXEEXT) -s -p1 -oobtained
ECHO_SIMPLEX = echo "***" $(SIMPLEX)
ECHO_SIMPLEX_NOCHECK = echo "***" $(SIMPLEX_NOCHECK)

check-local: ppl_lpsol$(EXEEXT)
	echo $(ECHO_N) "$(ECHO_C)" >obtained
	echo $(LP_PROBLEMS) >lp_problems
	echo $(INDIVIDUALLY_CHECKED_PROBLEMS) >individually_checked_problems
	export LC_ALL=C; \
	echo $(TOUGH_EXAMPLES_FOR_BB_SATISFIABILITY) >tough_examples
	-for path in $(srcdir)/examples/*.mps; \
	do \
	  file=`basename $$path`; \
	  $(FGREP) $$file lp_problems >/dev/null 2>&1 && continue; \
	  $(FGREP) $$file individually_checked_problems >/dev/null 2>&1 && continue; \
	  $(FGREP) $$file tough_examples >/dev/null 2>&1 && continue; \
	  $(ECHO_SIMPLEX) "-n $$m $$file" >>obtained; \
	  $(SIMPLEX) -n $$path >/dev/null; \
	done
	echo $(TOUGH_EXAMPLES_FOR_BB_OPTIMALITY) >tough_examples
	-for path in $(srcdir)/examples/*.mps; \
	do \
	  file=`basename $$path`; \
	  $(FGREP) $$file lp_problems >/dev/null 2>&1 && continue; \
	  $(FGREP) $$file individually_checked_problems >/dev/null 2>&1 && continue; \
	  $(FGREP) $$file tough_examples >/dev/null 2>&1 && continue; \
	  for m in -m -M; \
	  do \
	    $(ECHO_SIMPLEX) "$$m $$file" >>obtained; \
	    $(SIMPLEX) $$m $$path >/dev/null; \
	  done \
	done
	echo $(TOUGH_EXAMPLES_FOR_SIMPLEX) >tough_examples
	echo $(DO_NOT_OUTPUT_OPTIMUM_LOCATION_PROBLEMS) >do_not_output_location
	-for path in $(srcdir)/examples/*.mps; \
	do \
	 file=`basename $$path`; \
	 $(FGREP) $$file individually_checked_problems >/dev/null 2>&1 && continue; \
	 $(FGREP) $$file tough_examples >/dev/null 2>&1 && continue; \
	 verbosity=''; \
	 $(FGREP) $$file do_not_output_location >/dev/null 2>&1 && verbosity='-v 2'; \
	 for m in -m -M; \
	 do \
	    $(ECHO_SIMPLEX) "-r $$m $$file" >>obtained; \
	    $(SIMPLEX) -r $$verbosity $$m $$path >/dev/null; \
	  done \
	done
	echo $(TOUGH_EXAMPLES_FOR_ENUM) >tough_examples
	-for path in $(srcdir)/examples/*.mps; \
	do \
	  file=`basename $$path`; \
	  $(FGREP) $$file individually_checked_problems >/dev/null 2>&1 && continue; \
	  $(FGREP) $$file tough_examples >/dev/null 2>&1 && continue; \
	  echo "*** ppl_lpsol -e -r $$m -oobtained $$file" >>obtained; \
	  ./ppl_lpsol$(EXEEXT) -e -r $(CHECK_OPTION) $$m -oobtained $$path >/dev/null; \
	done
	$(ECHO_SIMPLEX_NOCHECK) "-r -m -c0.00000000518 boeing1.mps" >>obtained
	-$(SIMPLEX_NOCHECK) -r -m -c0.00000000518 $(srcdir)/examples/boeing1.mps >/dev/null
	$(ECHO_SIMPLEX_NOCHECK) "-r -M -c0.0000000105 boeing1.mps" >>obtained
	-$(SIMPLEX_NOCHECK) -r -M -c0.0000000105 $(srcdir)/examples/boeing1.mps $$m >/dev/null
	$(ECHO_SIMPLEX) "-n boeing1.mps" >>obtained
	-$(SIMPLEX) -n $(srcdir)/examples/boeing1.mps >/dev/null
	$(ECHO_SIMPLEX_NOCHECK) "-r -m -c0.00000000863 boeing2.mps" >>obtained
	-$(SIMPLEX_NOCHECK) -r -m -c0.00000000863 $(srcdir)/examples/boeing2.mps >/dev/null
	$(ECHO_SIMPLEX_NOCHECK) "-r -M -c0.00000000213 boeing2.mps" >>obtained
	-$(SIMPLEX_NOCHECK) -r -M -c0.00000000213 $(srcdir)/examples/boeing2.mps $$m >/dev/null
	$(ECHO_SIMPLEX) "-n boeing2.mps" >>obtained
	-$(SIMPLEX) -n $(srcdir)/examples/boeing2.mps >/dev/null

	$(ECHO_SIMPLEX) "-r -m mas74.mps" >>obtained
	-$(SIMPLEX) -r -m $(srcdir)/examples/mas74.mps >/dev/null
	$(ECHO_SIMPLEX_NOCHECK) "-r -M -c0.000123 mas74.mps" >>obtained
	-$(SIMPLEX_NOCHECK) -r -M -c0.000123 $(srcdir)/examples/mas74.mps $$m >/dev/null
	$(ECHO_SIMPLEX) "-n mas74.mps" >>obtained
	-$(SIMPLEX) -n $(srcdir)/examples/mas74.mps >/dev/null
	$(ECHO_SIMPLEX_NOCHECK) "-r -m -c0.0000273 modglob.mps" >>obtained
	-$(SIMPLEX_NOCHECK) -r -m -c0.0000273 $(srcdir)/examples/modglob.mps >/dev/null
	$(ECHO_SIMPLEX_NOCHECK) "-r -M -c0.0290 modglob.mps" >>obtained
	-$(SIMPLEX_NOCHECK) -r -M -c0.0290 $(srcdir)/examples/modglob.mps >/dev/null
	$(ECHO_SIMPLEX) "-n modglob.mps" >>obtained
	-$(SIMPLEX) -n $(srcdir)/examples/modglob.mps >/dev/null
	$(DIFF_COMMAND)

endif HAVE_GLPK
endif BUILD_C_INTERFACE

MOSTLYCLEANFILES = \
individually_checked_problems \
do_not_output_location \
expected_optima \
lp_problems \
obtained \
obtained_optima \
tough_examples

$(top_builddir)/interfaces/C/libppl_c.la:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces/C libppl_c.la
