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

if BUILD_C_INTERFACE
C_SUBDIR = C
endif BUILD_C_INTERFACE

if BUILD_JAVA_INTERFACE
JAVA_SUBDIR = Java
endif BUILD_JAVA_INTERFACE

if BUILD_OCAML_INTERFACE
OCAML_SUBDIR = OCaml
endif BUILD_OCAML_INTERFACE

if BUILD_SOME_PROLOG_INTERFACES
PROLOG_SUBDIR = Prolog
endif BUILD_SOME_PROLOG_INTERFACES

SUBDIRS = . $(C_SUBDIR) $(JAVA_SUBDIR) $(OCAML_SUBDIR) $(PROLOG_SUBDIR)

REQUIRED_INSTANTIATIONS = \
@required_instantiations@

REQUIRED_INSTANTIATIONS_CANONICAL_NAMES = \
@required_instantiations_canonical_names@

BUILT_SOURCES = ppl_interface_instantiations.m4

ppl_interface_instantiations.m4: Makefile
	echo "m4_define(\`m4_interface_classes_names', \`$(REQUIRED_INSTANTIATIONS_CANONICAL_NAMES)')" > $@
	echo "m4_define(\`m4_cplusplus_classes_names', \`$(REQUIRED_INSTANTIATIONS)')" >> $@

noinst_HEADERS = \
interfaced_boxes.hh \
marked_pointers.hh

EXTRA_DIST = \
ppl_interface_generator_common.m4 \
ppl_interface_generator_common_dat.m4 \
ppl_interface_generator_common_procedure_generators.m4 \
ppl_interface_generator_copyright

CLEANFILES = \
ppl_interface_instantiations.m4
