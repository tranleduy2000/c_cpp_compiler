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
--suppressions=$(srcdir)/valgrind_suppressions

else !VALGRIND_TESTS_ENABLED

CHECKER =

endif !VALGRIND_TESTS_ENABLED

TESTS_ENVIRONMENT = \
	$(LIBTOOL) --mode=execute $(PPL_DLOPEN) $(CHECKER)

interface_generator_files = \
ppl_interface_generator_ocaml_test_ml.m4 \
ppl_interface_generator_ocaml_test_ml_code.m4

interface_generator_dependencies = \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_ocaml_procedure_generators.m4 \
$(interface_generator_files) \
ppl_ocaml_tests_common

# Even if the OCaml sources are not really needed, they are built by m4,
# we need to include them to invoke the Automake OCaml build system.
EXTRA_DIST = \
$(interface_generator_files) \
ppl_ocaml_tests_common

OCAMLC_ENV = OCAMLRUNPARAM='l=1M'

OCAMLC_COMPILE_FLAGS = \
-I @mlgmp_dir@ -I .. -ccopt -g -ccopt -Wno-write-strings

OCAMLC_LINK_FLAGS = \
-cclib -lmlgmp -cclib -lmpfr \
`echo " $(extra_libraries) " | $(SED) -e "s/ -R[^ ]*//g" -e "s/ -/ -cclib -/g"`

OCAMLOPT_ENV = $(OCAMLC_ENV)
OCAMLOPT_COMPILE_FLAGS = $(OCAMLC_COMPILE_FLAGS)
OCAMLOPT_LINK_FLAGS = $(OCAMLC_LINK_FLAGS)

if ENABLE_SHARED

BUILT_PPL_FLAGS = -cclib $(top_builddir)/src/.libs/libppl$(SHREXT)

else !ENABLE_SHARED

BUILT_PPL_FLAGS = -cclib $(top_builddir)/src/.libs/libppl.$(LIBEXT)

endif !ENABLE_SHARED

PPL_DLOPEN = -dlopen ../../../src/libppl.la

.ml.cmo:
	$(OCAMLC_ENV) $(OCAMLC) -o $@ -c $(OCAMLC_COMPILE_FLAGS) $<

.ml.cmx:
	$(OCAMLOPT_ENV) $(OCAMLOPT) -o $@ -c $(OCAMLOPT_COMPILE_FLAGS) $<

if HAVE_OCAMLC

OCAMLC_TESTS = \
test1 \
ppl_ocaml_generated_test

endif HAVE_OCAMLC

if HAVE_OCAMLOPT

OCAMLOPT_TESTS = \
test1_opt \
ppl_ocaml_generated_test_opt

endif HAVE_OCAMLOPT

TESTS = $(OCAMLC_TESTS) $(OCAMLOPT_TESTS)

check_PROGRAMS = $(TESTS)

test1_SOURCES = test1.ml

test1$(EXEEXT): test1.cmo ../ppl_ocaml.cma
	$(OCAMLC_ENV) $(OCAMLC) -o $@ \
		-cc "$(CXX)" $(OCAMLC_COMPILE_FLAGS) \
		$(BUILT_PPL_FLAGS) $(OCAMLC_LINK_FLAGS) \
		ppl_ocaml.cma $<

test1_opt_SOURCES = test1.ml

test1_opt$(EXEEXT): test1.cmx ../ppl_ocaml.cmxa
	$(OCAMLOPT_ENV) $(OCAMLOPT) -o $@ \
		-cc "$(CXX)" $(OCAMLOPT_COMPILE_FLAGS) \
		$(BUILT_PPL_FLAGS) $(OCAMLOPT_LINK_FLAGS) \
		ppl_ocaml.cmxa $<


nodist_ppl_ocaml_generated_test_SOURCES = ppl_ocaml_generated_test.ml

nodist_ppl_ocaml_generated_test_opt_SOURCES = ppl_ocaml_generated_test.ml

ppl_ocaml_generated_test.ml: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. \
		-I$(srcdir) -I$(srcdir)/.. -I$(srcdir)/../.. \
		$(srcdir)/ppl_interface_generator_ocaml_test_ml.m4 \
			> $@

ppl_ocaml_generated_test$(EXEEXT): \
ppl_ocaml_generated_test.cmo ../ppl_ocaml.cma
	$(OCAMLC_ENV) $(OCAMLC) -o $@ \
		-cc "$(CXX)" $(OCAMLC_COMPILE_FLAGS) \
		$(BUILT_PPL_FLAGS) $(OCAMLC_LINK_FLAGS) \
		ppl_ocaml.cma $<

ppl_ocaml_generated_test_opt$(EXEEXT): \
ppl_ocaml_generated_test.cmx ../ppl_ocaml.cmxa
	$(OCAMLOPT_ENV) $(OCAMLOPT) -o $@ \
		-cc "$(CXX)" $(OCAMLOPT_COMPILE_FLAGS) \
		$(BUILT_PPL_FLAGS) $(OCAMLOPT_LINK_FLAGS) \
		ppl_ocaml.cmxa $<

MOSTLYCLEANFILES = \
test1.cmi \
test1.cmo \
test1.cmx \
test1.o \
test1 \
test1_opt \
ppl_ocaml_generated_test.cmi \
ppl_ocaml_generated_test.cmo \
ppl_ocaml_generated_test.cmx \
ppl_ocaml_generated_test.o \
ppl_ocaml_generated_test \
ppl_ocaml_generated_test_opt

CLEANFILES = ppl_ocaml_generated_test.ml

../ppl_ocaml.cma:
	$(MAKE) $(AM_MAKEFLAGS) -C .. ppl_ocaml.cma

../ppl_ocaml.cmxa:
	$(MAKE) $(AM_MAKEFLAGS) -C .. ppl_ocaml.cmxa

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

