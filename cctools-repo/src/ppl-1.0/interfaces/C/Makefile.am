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

SUBDIRS = . tests

interface_generator_files = \
ppl_interface_generator_c_procedure_generators.m4 \
ppl_interface_generator_c_h.m4 \
ppl_interface_generator_c_h_code.m4 \
ppl_interface_generator_c_cc_files.m4 \
ppl_interface_generator_c_hh_files.m4 \
ppl_interface_generator_c_cc_code.m4

EXTRA_DIST = \
$(interface_generator_files) \
ppl_c_header.h \
C_interface.dox

# Libtool -version-info for libppl_c.la.
#
#  1. Start with version information of `0:0:0' for each Libtool library.
#
#  2. Update the version information only immediately before a public
#     release of your software.  More frequent updates are unnecessary,
#     and only guarantee that the current interface number gets larger
#     faster.
#
#  3. If the library source code has changed at all since the last
#     update, then increment REVISION (`C:R:A' becomes `C:r+1:A').
#
#  4. If any interfaces have been added, removed, or changed since the
#     last update, increment CURRENT, and set REVISION to 0.
#
#  5. If any interfaces have been added since the last public release,
#     then increment AGE.
#
#  6. If any interfaces have been removed since the last public release,
#     then set AGE to 0.
#
#	PPL release		-version-info
#	    0.1			    -----
#	    0.2			    -----
#	    0.3			    -----
#	    0.4			    0:0:0
#	    0.5			    1:0:1
#	    0.6			    1:0:1
#	    0.6.1		    1:0:1
#	    0.7			    1:0:1
#	    0.8			    1:0:1
#	    0.9			    1:0:1
#	    0.10		    2:0:0
#	    0.10.1		    2:0:0 (was not updated by mistake)
#	    0.10.2		    3:0:1
#	    0.11		    4:0:0

LIBPPL_C_LT_CURRENT =  4
LIBPPL_C_LT_REVISION = 0
LIBPPL_C_LT_AGE =      0


if HAVE_PERL

ppl_c.h: ppl_c_header.h ppl_c_version.h ppl_c_domains.h Makefile $(top_builddir)/utils/build_header
	$(top_builddir)/utils/build_header \
		-I $(top_builddir)/interfaces/C -I $(top_srcdir)/src \
		$(top_srcdir)/interfaces/C/ppl_c_header.h >$@

else !HAVE_PERL

ppl_c.h:	ppl_c.h.dist
	cp -f $< $@

endif !HAVE_PERL

BUILT_SOURCES = \
ppl_c.h \
ppl_c_implementation_domains.hh \
ppl_c_implementation_domains.cc.stamp

# ppl_c.h is not distributed.
# ppl_c.h.dist, which is distributed, is a copy of ppl_c.h.
dist-hook:
	mv -f $(distdir)/ppl_c.h $(distdir)/ppl_c.h.dist

interface_generator_dependencies = \
$(interface_generator_files) \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../ppl_interface_generator_common.m4 \
../ppl_interface_generator_common_dat.m4 \
../ppl_interface_generator_copyright \
../ppl_interface_generator_common_procedure_generators.m4

ppl_c_domains.h: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I.. -I$(srcdir) -I$(srcdir)/.. \
           $(srcdir)/ppl_interface_generator_c_h.m4 > $@

ppl_c_implementation_domains.cc.stamp: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I.. -I$(srcdir) -I$(srcdir)/.. \
           $(srcdir)/ppl_interface_generator_c_cc_files.m4 \
                > ppl_c_cc_blob
	$(top_srcdir)/utils/cm_cleaner.sh ./ppl_c_cc_blob
	$(top_srcdir)/utils/cm_splitter.sh ./ppl_c_cc_blob
	rm -f ppl_c_cc_blob
	echo timestamp >$@

ppl_c_implementation_domains.hh: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I.. -I$(srcdir) -I$(srcdir)/.. \
           $(srcdir)/ppl_interface_generator_c_hh_files.m4 \
                > ppl_c_hh_blob
	$(top_srcdir)/utils/cm_cleaner.sh ./ppl_c_hh_blob
	$(top_srcdir)/utils/cm_splitter.sh ./ppl_c_hh_blob
	rm -f ppl_c_hh_blob

AM_CPPFLAGS = \
-I$(top_srcdir)/interfaces \
-I$(top_builddir)/interfaces/C \
-I$(top_builddir)/src \
@extra_includes@

lib_LTLIBRARIES = libppl_c.la

libppl_c_la_SOURCES = \
ppl_c_implementation_common.defs.hh \
ppl_c_implementation_common.inlines.hh \
ppl_c_implementation_common.cc

nodist_libppl_c_la_SOURCES =

nodist_EXTRA_libppl_c_la_SOURCES = \
ppl_c_Int8_Box.cc \
ppl_c_Int16_Box.cc \
ppl_c_Int32_Box.cc \
ppl_c_Int64_Box.cc \
ppl_c_Uint8_Box.cc \
ppl_c_Uint16_Box.cc \
ppl_c_Uint32_Box.cc \
ppl_c_Uint64_Box.cc \
ppl_c_Float_Box.cc \
ppl_c_Double_Box.cc \
ppl_c_Long_Double_Box.cc \
ppl_c_Rational_Box.cc \
ppl_c_Z_Box.cc \
ppl_c_BD_Shape_int8_t.cc \
ppl_c_BD_Shape_int16_t.cc \
ppl_c_BD_Shape_int32_t.cc \
ppl_c_BD_Shape_int64_t.cc \
ppl_c_BD_Shape_float.cc \
ppl_c_BD_Shape_double.cc \
ppl_c_BD_Shape_long_double.cc \
ppl_c_BD_Shape_mpq_class.cc \
ppl_c_BD_Shape_mpz_class.cc \
ppl_c_Octagonal_Shape_int8_t.cc \
ppl_c_Octagonal_Shape_int16_t.cc \
ppl_c_Octagonal_Shape_int32_t.cc \
ppl_c_Octagonal_Shape_int64_t.cc \
ppl_c_Octagonal_Shape_float.cc \
ppl_c_Octagonal_Shape_double.cc \
ppl_c_Octagonal_Shape_long_double.cc \
ppl_c_Octagonal_Shape_mpq_class.cc \
ppl_c_Octagonal_Shape_mpz_class.cc \
ppl_c_Polyhedron.cc \
ppl_c_Grid.cc \
ppl_c_Pointset_Powerset_Int8_Box.cc \
ppl_c_Pointset_Powerset_Int16_Box.cc \
ppl_c_Pointset_Powerset_Int32_Box.cc \
ppl_c_Pointset_Powerset_Int64_Box.cc \
ppl_c_Pointset_Powerset_Uint8_Box.cc \
ppl_c_Pointset_Powerset_Uint16_Box.cc \
ppl_c_Pointset_Powerset_Uint32_Box.cc \
ppl_c_Pointset_Powerset_Uint64_Box.cc \
ppl_c_Pointset_Powerset_Float_Box.cc \
ppl_c_Pointset_Powerset_Double_Box.cc \
ppl_c_Pointset_Powerset_Long_Double_Box.cc \
ppl_c_Pointset_Powerset_Rational_Box.cc \
ppl_c_Pointset_Powerset_Z_Box.cc \
ppl_c_Pointset_Powerset_BD_Shape_int8_t.cc \
ppl_c_Pointset_Powerset_BD_Shape_int16_t.cc \
ppl_c_Pointset_Powerset_BD_Shape_int32_t.cc \
ppl_c_Pointset_Powerset_BD_Shape_int64_t.cc \
ppl_c_Pointset_Powerset_BD_Shape_float.cc \
ppl_c_Pointset_Powerset_BD_Shape_double.cc \
ppl_c_Pointset_Powerset_BD_Shape_long_double.cc \
ppl_c_Pointset_Powerset_BD_Shape_mpq_class.cc \
ppl_c_Pointset_Powerset_BD_Shape_mpz_class.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_int8_t.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_int16_t.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_int32_t.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_int64_t.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_float.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_double.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_long_double.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_mpq_class.cc \
ppl_c_Pointset_Powerset_Octagonal_Shape_mpz_class.cc \
ppl_c_Pointset_Powerset_C_Polyhedron.cc \
ppl_c_Pointset_Powerset_NNC_Polyhedron.cc \
ppl_c_Pointset_Powerset_Grid.cc \
ppl_c_Constraints_Product_C_Polyhedron_Grid.cc \
ppl_c_Constraints_Product_NNC_Polyhedron_Grid.cc \
ppl_c_Constraints_Product_Grid_C_Polyhedron.cc \
ppl_c_Constraints_Product_Grid_NNC_Polyhedron.cc

if NO_UNDEFINED

NO_UNDEFINED_FLAG = -no-undefined

endif NO_UNDEFINED

libppl_c_la_LIBADD = \
@required_instantiations_c_cxx_objects@ \
$(top_builddir)/src/libppl.la \
@extra_libraries@

libppl_c_la_DEPENDENCIES = \
@required_instantiations_c_cxx_objects@

libppl_c_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-version-info $(LIBPPL_C_LT_CURRENT):$(LIBPPL_C_LT_REVISION):$(LIBPPL_C_LT_AGE)

include_HEADERS = ppl_c.h

nodist_noinst_HEADERS = \
ppl_c_domains.h \
ppl_c_version.h

MOSTLYCLEANFILES = \
@required_instantiations_c_cxx_objects@

CLEANFILES = \
@required_instantiations_c_cxx_sources@ \
@required_instantiations_c_cxx_headers@ \
ppl_c.h \
ppl_c_domains.h \
ppl_c_implementation_domains.hh \
ppl_c_implementation_domains.cc.stamp

DISTCLEANFILES = \
ppl_c_version.h

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

$(top_builddir)/utils/build_header:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils build_header

# A selection of these files, which depends on the configure options,
# is built by the `ppl_c_implementation_domains.cc.stamp' target.
$(nodist_EXTRA_libppl_c_la_SOURCES):
