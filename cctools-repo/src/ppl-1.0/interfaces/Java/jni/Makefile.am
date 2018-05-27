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

interface_generator_files = \
ppl_interface_generator_java_classes_cc_files.m4 \
ppl_interface_generator_java_classes_cc_code.m4

interface_generator_dependencies = \
$(interface_generator_files) \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_java_procedure_generators.m4

EXTRA_DIST = \
$(interface_generator_files)

if HOST_OS_MINGW

JNI_DEFINE_OPTIONS = -D_JNI_IMPLEMENTATION_

endif HOST_OS_MINGW

AM_CPPFLAGS = \
-I$(top_srcdir)/interfaces \
-I$(top_builddir)/src \
-I../parma_polyhedra_library \
-I$(top_srcdir)/src \
@extra_includes@ @JNIFLAGS@ $(JNI_DEFINE_OPTIONS)

BUILT_SOURCES = \
ppl_java_implementation_domains.cc.stamp

pkglib_LTLIBRARIES = libppl_java.la

# There is no point in generating the static version.
AM_LIBTOOLFLAGS = --tag=disable-static

if NO_UNDEFINED

NO_UNDEFINED_FLAG = -no-undefined

endif NO_UNDEFINED

if HOST_OS_DARWIN

libppl_java_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-avoid-version \
-shrext .jnilib

else !HOST_OS_DARWIN

if HOST_OS_MINGW

libppl_java_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-module \
-avoid-version \
-Wl,--kill-at

else !HOST_OS_MINGW

libppl_java_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-module \
-avoid-version

endif !HOST_OS_MINGW

endif !HOST_OS_DARWIN

libppl_java_la_SOURCES = \
ppl_java_common.defs.hh \
ppl_java_common.inlines.hh \
ppl_java_common.cc \
ppl_java_globals.cc

nodist_EXTRA_libppl_java_la_SOURCES = \
ppl_java_Termination.cc \
ppl_java_Int8_Box.cc \
ppl_java_Int16_Box.cc \
ppl_java_Int32_Box.cc \
ppl_java_Int64_Box.cc \
ppl_java_Uint8_Box.cc \
ppl_java_Uint16_Box.cc \
ppl_java_Uint32_Box.cc \
ppl_java_Uint64_Box.cc \
ppl_java_Float_Box.cc \
ppl_java_Double_Box.cc \
ppl_java_Long_Double_Box.cc \
ppl_java_Rational_Box.cc \
ppl_java_Z_Box.cc \
ppl_java_BD_Shape_int8_t.cc \
ppl_java_BD_Shape_int16_t.cc \
ppl_java_BD_Shape_int32_t.cc \
ppl_java_BD_Shape_int64_t.cc \
ppl_java_BD_Shape_float.cc \
ppl_java_BD_Shape_double.cc \
ppl_java_BD_Shape_long_double.cc \
ppl_java_BD_Shape_mpq_class.cc \
ppl_java_BD_Shape_mpz_class.cc \
ppl_java_Octagonal_Shape_int8_t.cc \
ppl_java_Octagonal_Shape_int16_t.cc \
ppl_java_Octagonal_Shape_int32_t.cc \
ppl_java_Octagonal_Shape_int64_t.cc \
ppl_java_Octagonal_Shape_float.cc \
ppl_java_Octagonal_Shape_double.cc \
ppl_java_Octagonal_Shape_long_double.cc \
ppl_java_Octagonal_Shape_mpq_class.cc \
ppl_java_Octagonal_Shape_mpz_class.cc \
ppl_java_Polyhedron.cc \
ppl_java_Grid.cc \
ppl_java_Pointset_Powerset_Int8_Box.cc \
ppl_java_Pointset_Powerset_Int16_Box.cc \
ppl_java_Pointset_Powerset_Int32_Box.cc \
ppl_java_Pointset_Powerset_Int64_Box.cc \
ppl_java_Pointset_Powerset_Uint8_Box.cc \
ppl_java_Pointset_Powerset_Uint16_Box.cc \
ppl_java_Pointset_Powerset_Uint32_Box.cc \
ppl_java_Pointset_Powerset_Uint64_Box.cc \
ppl_java_Pointset_Powerset_Float_Box.cc \
ppl_java_Pointset_Powerset_Double_Box.cc \
ppl_java_Pointset_Powerset_Long_Double_Box.cc \
ppl_java_Pointset_Powerset_Rational_Box.cc \
ppl_java_Pointset_Powerset_Z_Box.cc \
ppl_java_Pointset_Powerset_BD_Shape_int8_t.cc \
ppl_java_Pointset_Powerset_BD_Shape_int16_t.cc \
ppl_java_Pointset_Powerset_BD_Shape_int32_t.cc \
ppl_java_Pointset_Powerset_BD_Shape_int64_t.cc \
ppl_java_Pointset_Powerset_BD_Shape_float.cc \
ppl_java_Pointset_Powerset_BD_Shape_double.cc \
ppl_java_Pointset_Powerset_BD_Shape_long_double.cc \
ppl_java_Pointset_Powerset_BD_Shape_mpq_class.cc \
ppl_java_Pointset_Powerset_BD_Shape_mpz_class.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_int8_t.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_int16_t.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_int32_t.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_int64_t.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_float.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_double.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_long_double.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_mpq_class.cc \
ppl_java_Pointset_Powerset_Octagonal_Shape_mpz_class.cc \
ppl_java_Pointset_Powerset_C_Polyhedron.cc \
ppl_java_Pointset_Powerset_NNC_Polyhedron.cc \
ppl_java_Pointset_Powerset_Grid.cc \
ppl_java_Constraints_Product_C_Polyhedron_Grid.cc \
ppl_java_Constraints_Product_NNC_Polyhedron_Grid.cc \
ppl_java_Constraints_Product_Grid_C_Polyhedron.cc \
ppl_java_Constraints_Product_Grid_NNC_Polyhedron.cc

libppl_java_la_LIBADD = \
ppl_java_Termination.lo \
@required_instantiations_java_cxx_objects@ \
$(top_builddir)/src/libppl.la \
@extra_libraries@

libppl_java_la_DEPENDENCIES = \
ppl_java_Termination.lo \
@required_instantiations_java_cxx_objects@

ppl_java_implementation_domains.cc.stamp: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. \
		-I$(srcdir) -I$(srcdir)/.. -I$(srcdir)/../.. \
		$(srcdir)/ppl_interface_generator_java_classes_cc_files.m4 \
                > ppl_java_cc_blob
	$(top_srcdir)/utils/cm_cleaner.sh ./ppl_java_cc_blob
	$(top_srcdir)/utils/cm_splitter.sh ./ppl_java_cc_blob
	rm -f ppl_java_cc_blob
	echo timestamp >$@

CLEANFILES = \
ppl_java_Termination.cc \
@required_instantiations_java_cxx_sources@ \
ppl_java_implementation_domains.cc.stamp

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4

# A selection of these files, which depends on the configure options,
# is built by the `ppl_java_implementation_domains.cc.stamp' target.
$(nodist_EXTRA_libppl_java_la_SOURCES):
