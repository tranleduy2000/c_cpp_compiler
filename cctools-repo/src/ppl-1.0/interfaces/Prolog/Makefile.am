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

if BUILD_CIAO_PROLOG_INTERFACE
CIAO_PROLOG_SUBDIR = Ciao
endif BUILD_CIAO_PROLOG_INTERFACE

if BUILD_GNU_PROLOG_INTERFACE
GNU_PROLOG_SUBDIR = GNU
endif BUILD_GNU_PROLOG_INTERFACE

if BUILD_SICSTUS_PROLOG_INTERFACE
SICSTUS_PROLOG_SUBDIR = SICStus
endif BUILD_SICSTUS_PROLOG_INTERFACE

if BUILD_SWI_PROLOG_INTERFACE
SWI_PROLOG_SUBDIR = SWI
endif BUILD_SWI_PROLOG_INTERFACE

if BUILD_XSB_PROLOG_INTERFACE
XSB_PROLOG_SUBDIR = XSB
endif BUILD_XSB_PROLOG_INTERFACE

if BUILD_YAP_PROLOG_INTERFACE
YAP_PROLOG_SUBDIR = YAP
endif BUILD_YAP_PROLOG_INTERFACE

SUBDIRS = \
. tests \
$(CIAO_PROLOG_SUBDIR) \
$(GNU_PROLOG_SUBDIR) \
$(SICSTUS_PROLOG_SUBDIR) \
$(SWI_PROLOG_SUBDIR) \
$(XSB_PROLOG_SUBDIR) \
$(YAP_PROLOG_SUBDIR)

interface_generator_files = \
ppl_interface_generator_prolog_procedure_generators.m4 \
ppl_interface_generator_prolog_hh_files.m4 \
ppl_interface_generator_prolog_hh_code.m4 \
ppl_interface_generator_prolog_cc_files.m4 \
ppl_interface_generator_prolog_cc_code.m4 \
ppl_interface_generator_prolog_systems.m4

interface_generator_dox_files = \
ppl_interface_generator_prolog_dox.m4 \
ppl_interface_generator_prolog_dox_code.m4


EXTRA_DIST = \
$(interface_generator_files) \
ppl_prolog_common.defs.hh \
ppl_prolog_common.inlines.hh \
ppl_prolog_common.cc \
Prolog_configured_interface.dox \
$(interface_generator_dox_files) \
Prolog_interface.dox \
Prolog_interface_sysindep.dox \
Prolog_interface_sysindep_domains.dox \
Prolog_interface_compilation.dox \
Prolog_interface_sysdep.dox


interface_generator_dox_dependencies = \
../ppl_interface_generator_common.m4 \
../ppl_interface_generator_common_dat.m4 \
../ppl_interface_generator_copyright \
../ppl_interface_generator_common_procedure_generators.m4 \
ppl_interface_generator_prolog_procedure_generators.m4 \
ppl_interface_generator_prolog_dox.m4 \
ppl_interface_generator_prolog_dox_code.m4

Prolog_configured_domains.dox: \
$(interface_generator_dox_dependencies) Makefile
	$(M4) --prefix-builtin -I.. -I$(srcdir) -I$(srcdir)/.. \
           $(srcdir)/ppl_interface_generator_prolog_dox.m4 \
                > Prolog_configured_domains.dox

BUILT_SOURCES = \
ppl_prolog_domains.hh \
ppl_prolog_domains.cc.stamp

nodist_noinst_DATA = Prolog_configured_domains.dox

interface_generator_dependencies = \
$(interface_generator_files) \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../ppl_interface_generator_common.m4 \
../ppl_interface_generator_common_dat.m4 \
../ppl_interface_generator_copyright \
../ppl_interface_generator_common_procedure_generators.m4


ppl_prolog_domains.hh: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I.. -I$(srcdir) -I$(srcdir)/.. \
           $(srcdir)/ppl_interface_generator_prolog_hh_files.m4 \
                > ppl_prolog_hh_blob
	$(top_srcdir)/utils/cm_cleaner.sh ./ppl_prolog_hh_blob
	$(top_srcdir)/utils/cm_splitter.sh ./ppl_prolog_hh_blob
	rm -f ppl_prolog_hh_blob

ppl_prolog_domains.cc.stamp: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I.. -I$(srcdir) -I$(srcdir)/.. \
           $(srcdir)/ppl_interface_generator_prolog_cc_files.m4 \
                > ppl_prolog_cc_blob
	$(top_srcdir)/utils/cm_cleaner.sh ./ppl_prolog_cc_blob
	$(top_srcdir)/utils/cm_splitter.sh ./ppl_prolog_cc_blob
	rm -f ppl_prolog_cc_blob
	echo timestamp >$@

CLEANFILES = \
@required_instantiations_prolog_cxx_headers@ \
@required_instantiations_prolog_cxx_sources@ \
ppl_prolog_domains.cc.stamp \
ppl_prolog_domains.hh \
Prolog_configured_domains.dox

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4
