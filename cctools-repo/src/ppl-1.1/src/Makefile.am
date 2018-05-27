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

# Libtool -version-info for libppl.la.
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
#	    0.3			    0:0:0
#	    0.4			    1:0:1
#	    0.5			    2:0:0
#	    0.6			    3:0:0
#	    0.7			    4:0:0
#	    0.8			    5:0:0
#	    0.9			    6:0:0
#	    0.10		    7:0:0
#	    0.10.1		    8:0:1
#	    0.10.2		    8:0:1
#	    0.11		    9:0:0
#	    0.12		   10:0:0
#	    0.12.1		   11:0:0
#	    1.0			   12:0:0
#	    1.1			   13:0:0

LIBPPL_LT_CURRENT =  13
LIBPPL_LT_REVISION =  0
LIBPPL_LT_AGE =       0

AM_CPPFLAGS = \
-I$(top_builddir) \
-I$(top_srcdir)/src \
@extra_includes@

lib_LTLIBRARIES = libppl.la

bin_PROGRAMS = ppl-config

ppl_config_SOURCES = \
BUGS.hh \
BUGS.cc \
COPYING.hh \
COPYING.cc \
CREDITS.hh \
CREDITS.cc

nodist_ppl_config_SOURCES = \
ppl-config.cc \
ppl.hh

ppl_config_LDADD = \
@extra_libraries@ \
libppl.la

if USE_NATIVE_INTEGERS

COEFFICIENT_TYPES_INCLUDE_FILES =

COEFFICIENT_DEFS_INCLUDE_FILES =

endif USE_NATIVE_INTEGERS

if USE_CHECKED_INTEGERS

COEFFICIENT_TYPES_INCLUDE_FILES =

COEFFICIENT_DEFS_INCLUDE_FILES =

endif USE_CHECKED_INTEGERS

if USE_GMP_INTEGERS

COEFFICIENT_TYPES_INCLUDE_FILES = \
  GMP_Integer_types.hh

COEFFICIENT_DEFS_INCLUDE_FILES = \
  GMP_Integer_defs.hh \
  GMP_Integer_inlines.hh

endif USE_GMP_INTEGERS

# The ordering of the following list *matters*!
INCLUDE_FILES = \
namespaces.hh \
compiler.hh \
assert.hh \
meta_programming.hh \
Slow_Copy.hh \
Has_Assign_Or_Swap.hh \
assign_or_swap.hh \
mp_std_bits_defs.hh \
mp_std_bits_inlines.hh \
Concrete_Expression_types.hh \
Concrete_Expression_defs.hh \
Concrete_Expression_inlines.hh \
Temp_defs.hh \
Temp_inlines.hh \
Temp_templates.hh \
Coefficient_traits_template.hh \
Checked_Number_types.hh \
termination_types.hh \
$(COEFFICIENT_TYPES_INCLUDE_FILES) \
Coefficient_types.hh \
stdiobuf_types.hh \
c_streambuf_types.hh \
globals_types.hh \
iterator_to_const_types.hh \
distances_types.hh \
Interval_Info_types.hh \
Interval_types.hh \
Box_types.hh \
Constraint_types.hh \
Generator_types.hh \
Grid_Generator_types.hh \
Congruence_types.hh \
Init_types.hh \
Dense_Row_types.hh \
CO_Tree_types.hh \
Sparse_Row_types.hh \
Matrix_types.hh \
Swapping_Vector_types.hh \
Variable_types.hh \
Variables_Set_types.hh \
Linear_Expression_types.hh \
Expression_Adapter_types.hh \
Expression_Hide_Last_types.hh \
Expression_Hide_Inhomo_types.hh \
Linear_Expression_Impl_types.hh \
Linear_Expression_Interface_types.hh \
Linear_System_types.hh \
Linear_Form_types.hh \
Bit_Row_types.hh \
Bit_Matrix_types.hh \
Constraint_System_types.hh \
Generator_System_types.hh \
Grid_Generator_System_types.hh \
Congruence_System_types.hh \
Scalar_Products_types.hh \
PIP_Tree_types.hh \
PIP_Problem_types.hh \
Poly_Con_Relation_types.hh \
Poly_Gen_Relation_types.hh \
BHRZ03_Certificate_types.hh \
H79_Certificate_types.hh \
Grid_Certificate_types.hh \
Partial_Function_types.hh \
Polyhedron_types.hh \
C_Polyhedron_types.hh \
NNC_Polyhedron_types.hh \
Grid_types.hh \
Ptr_Iterator_types.hh \
DB_Row_types.hh \
DB_Matrix_types.hh \
BD_Shape_types.hh \
OR_Matrix_types.hh \
Octagonal_Shape_types.hh \
fpu_types.hh \
Float_defs.hh \
Float_inlines.hh \
Float_templates.hh \
C_Integer.hh \
fpu_defs.hh \
fpu-c99_inlines.hh \
fpu-ia32_inlines.hh \
fpu-none_inlines.hh \
fpu-sparc_inlines.hh \
Rounding_Dir_defs.hh \
Rounding_Dir_inlines.hh \
Result_defs.hh \
Result_inlines.hh \
Numeric_Format_defs.hh \
checked_defs.hh \
checked_inlines.hh \
checked_int_inlines.hh \
checked_float_inlines.hh \
checked_mpz_inlines.hh \
checked_mpq_inlines.hh \
checked_ext_inlines.hh \
Checked_Number_defs.hh \
Checked_Number_inlines.hh \
Checked_Number_templates.hh \
WRD_coefficient_types_defs.hh \
WRD_coefficient_types_inlines.hh \
checked_numeric_limits.hh \
$(COEFFICIENT_DEFS_INCLUDE_FILES) \
Coefficient_defs.hh \
Coefficient_inlines.hh \
stdiobuf_defs.hh \
stdiobuf_inlines.hh \
c_streambuf_defs.hh \
c_streambuf_inlines.hh \
globals_defs.hh \
globals_inlines.hh \
math_utilities_defs.hh \
math_utilities_inlines.hh \
iterator_to_const_defs.hh \
iterator_to_const_inlines.hh \
distances_defs.hh \
distances_inlines.hh \
Boundary_defs.hh \
intervals_defs.hh \
Interval_Info_defs.hh \
Interval_Info_inlines.hh \
Interval_defs.hh \
Interval_inlines.hh \
Interval_templates.hh \
Box_defs.hh \
Box_Status_inlines.hh \
Box_inlines.hh \
Box_templates.hh \
Integer_Interval.hh \
Rational_Interval.hh \
Rational_Box.hh \
Variable_defs.hh \
Variable_inlines.hh \
Variables_Set_defs.hh \
Variables_Set_inlines.hh \
Init_defs.hh \
Init_inlines.hh \
initializer.hh \
Topology_types.hh \
Dense_Row_defs.hh \
Dense_Row_inlines.hh \
Dense_Row_templates.hh \
CO_Tree_defs.hh \
CO_Tree_inlines.hh \
CO_Tree_templates.hh \
Sparse_Row_defs.hh \
Sparse_Row_inlines.hh \
Sparse_Row_templates.hh \
Bit_Row_defs.hh \
Bit_Row_inlines.hh \
Linear_System_defs.hh \
Linear_System_inlines.hh \
Linear_System_templates.hh \
Bit_Matrix_defs.hh \
Bit_Matrix_inlines.hh \
Constraint_System_defs.hh \
Generator_System_defs.hh \
Grid_Generator_System_defs.hh \
Congruence_System_defs.hh \
Linear_Expression_defs.hh \
Linear_Expression_inlines.hh \
Expression_Adapter_defs.hh \
Expression_Adapter_inlines.hh \
Expression_Hide_Last_defs.hh \
Expression_Hide_Last_inlines.hh \
Expression_Hide_Inhomo_defs.hh \
Expression_Hide_Inhomo_inlines.hh \
Linear_Expression_Impl_defs.hh \
Linear_Expression_Impl_inlines.hh \
Linear_Expression_Impl_templates.hh \
Linear_Expression_Interface_defs.hh \
Linear_Form_defs.hh \
Linear_Form_inlines.hh \
Linear_Form_templates.hh \
linearize.hh \
Constraint_defs.hh \
Constraint_inlines.hh \
Constraint_System_inlines.hh \
Generator_defs.hh \
Generator_inlines.hh \
Grid_Generator_defs.hh \
Grid_Generator_inlines.hh \
Congruence_defs.hh \
Congruence_inlines.hh \
Generator_System_inlines.hh \
Grid_Generator_System_inlines.hh \
Congruence_System_inlines.hh \
Scalar_Products_defs.hh \
Scalar_Products_inlines.hh \
Matrix_defs.hh \
Matrix_inlines.hh \
Matrix_templates.hh \
Swapping_Vector_defs.hh \
Swapping_Vector_inlines.hh \
MIP_Problem_types.hh \
MIP_Problem_defs.hh \
MIP_Problem_inlines.hh \
MIP_Problem_templates.hh \
PIP_Tree_types.hh \
PIP_Tree_defs.hh \
PIP_Tree_inlines.hh \
PIP_Problem_types.hh \
PIP_Problem_defs.hh \
PIP_Problem_inlines.hh \
PIP_Problem_templates.hh \
Poly_Con_Relation_defs.hh \
Poly_Con_Relation_inlines.hh \
Poly_Gen_Relation_defs.hh \
Poly_Gen_Relation_inlines.hh \
BHRZ03_Certificate_defs.hh \
BHRZ03_Certificate_inlines.hh \
H79_Certificate_defs.hh \
H79_Certificate_inlines.hh \
Grid_Certificate_defs.hh \
Grid_Certificate_inlines.hh \
Partial_Function_defs.hh \
Partial_Function_inlines.hh \
Polyhedron_defs.hh \
Ph_Status_inlines.hh \
Polyhedron_inlines.hh \
Polyhedron_chdims_templates.hh \
Polyhedron_conversion_templates.hh \
Polyhedron_minimize_templates.hh \
Polyhedron_simplify_templates.hh \
Polyhedron_templates.hh \
Grid_defs.hh \
Grid_Status_inlines.hh \
Grid_inlines.hh \
Grid_templates.hh \
C_Polyhedron_defs.hh \
C_Polyhedron_inlines.hh \
NNC_Polyhedron_defs.hh \
NNC_Polyhedron_inlines.hh \
Widening_Function_types.hh \
Widening_Function_defs.hh \
Widening_Function_inlines.hh \
Ptr_Iterator_defs.hh \
Ptr_Iterator_inlines.hh \
DB_Row_defs.hh \
DB_Row_inlines.hh \
DB_Row_templates.hh \
DB_Matrix_defs.hh \
DB_Matrix_inlines.hh \
DB_Matrix_templates.hh \
BD_Shape_defs.hh \
BDS_Status_inlines.hh \
BD_Shape_inlines.hh \
BD_Shape_templates.hh \
OR_Matrix_defs.hh \
OR_Matrix_inlines.hh \
OR_Matrix_templates.hh \
Octagonal_Shape_defs.hh \
Og_Status_inlines.hh \
Octagonal_Shape_inlines.hh \
Octagonal_Shape_templates.hh \
Determinate_types.hh \
Determinate_defs.hh \
Determinate_inlines.hh \
Powerset_types.hh \
Powerset_defs.hh \
Powerset_inlines.hh \
Powerset_templates.hh \
Pointset_Powerset_types.hh \
Pointset_Powerset_defs.hh \
Pointset_Powerset_inlines.hh \
Pointset_Powerset_templates.hh \
Partially_Reduced_Product_types.hh \
Partially_Reduced_Product_defs.hh \
Partially_Reduced_Product_inlines.hh \
Partially_Reduced_Product_templates.hh \
Weight_Profiler_defs.hh \
max_space_dimension.hh \
algorithms.hh \
termination_defs.hh \
termination_templates.hh \
wrap_assign.hh \
wrap_string.hh \
Floating_Point_Expression_types.hh \
Floating_Point_Expression_defs.hh \
Floating_Point_Expression_inlines.hh \
Floating_Point_Expression_templates.hh \
Cast_Floating_Point_Expression_types.hh \
Cast_Floating_Point_Expression_defs.hh \
Cast_Floating_Point_Expression_inlines.hh \
Cast_Floating_Point_Expression_templates.hh \
Constant_Floating_Point_Expression_types.hh \
Constant_Floating_Point_Expression_defs.hh \
Constant_Floating_Point_Expression_inlines.hh \
Variable_Floating_Point_Expression_types.hh \
Variable_Floating_Point_Expression_defs.hh \
Variable_Floating_Point_Expression_inlines.hh \
Sum_Floating_Point_Expression_types.hh \
Sum_Floating_Point_Expression_defs.hh \
Sum_Floating_Point_Expression_inlines.hh \
Sum_Floating_Point_Expression_templates.hh \
Difference_Floating_Point_Expression_types.hh \
Difference_Floating_Point_Expression_defs.hh \
Difference_Floating_Point_Expression_inlines.hh \
Difference_Floating_Point_Expression_templates.hh \
Multiplication_Floating_Point_Expression_types.hh \
Multiplication_Floating_Point_Expression_defs.hh \
Multiplication_Floating_Point_Expression_inlines.hh \
Multiplication_Floating_Point_Expression_templates.hh \
Division_Floating_Point_Expression_types.hh \
Division_Floating_Point_Expression_defs.hh \
Division_Floating_Point_Expression_inlines.hh \
Division_Floating_Point_Expression_templates.hh \
Opposite_Floating_Point_Expression_types.hh \
Opposite_Floating_Point_Expression_defs.hh \
Opposite_Floating_Point_Expression_inlines.hh \
Doubly_Linked_Object_types.hh \
EList_types.hh \
EList_Iterator_types.hh \
Handler_types.hh \
Pending_Element_types.hh \
Pending_List_types.hh \
Time_types.hh \
Watchdog_types.hh \
Threshold_Watcher_types.hh \
Handler_defs.hh \
Handler_inlines.hh \
Time_defs.hh \
Time_inlines.hh \
Doubly_Linked_Object_defs.hh \
Doubly_Linked_Object_inlines.hh \
EList_Iterator_defs.hh \
EList_Iterator_inlines.hh \
EList_defs.hh \
EList_inlines.hh \
Pending_Element_defs.hh \
Pending_Element_inlines.hh \
Pending_List_defs.hh \
Pending_List_inlines.hh \
Pending_List_templates.hh \
Watchdog_defs.hh \
Watchdog_inlines.hh \
Threshold_Watcher_defs.hh \
Threshold_Watcher_inlines.hh \
Threshold_Watcher_templates.hh
#Any_Pointset_types.hh \
#Any_Pointset_defs.hh \
#Any_Pointset_inlines.hh \
#Ask_Tell_types.hh \
#Ask_Tell_defs.hh \
#Ask_Tell_inlines.hh \
#Ask_Tell_templates.hh \
#Pointset_Ask_Tell_types.hh \
#Pointset_Ask_Tell_defs.hh \
#Pointset_Ask_Tell_inlines.hh \
#Pointset_Ask_Tell_templates.hh

NESTED_INCLUDE_FILES = \
Ph_Status_idefs.hh \
Grid_Status_idefs.hh \
Box_Status_idefs.hh \
BDS_Status_idefs.hh \
Og_Status_idefs.hh

libppl_la_SOURCES = \
assert.cc \
Box.cc \
checked.cc \
Checked_Number.cc \
Float.cc \
fpu-ia32.cc \
Concrete_Expression.cc \
Constraint.cc \
Constraint_System.cc \
Congruence.cc \
Congruence_System.cc \
Generator_System.cc \
Grid_Generator_System.cc \
Generator.cc \
Grid_Generator.cc \
Init.cc \
Coefficient.cc \
Linear_Expression.cc \
Linear_Expression_Impl.cc \
Linear_Expression_Interface.cc \
Linear_Form.cc \
Scalar_Products.cc \
MIP_Problem.cc \
PIP_Tree.cc \
PIP_Problem.cc \
Poly_Con_Relation.cc \
Poly_Gen_Relation.cc \
BHRZ03_Certificate.cc \
H79_Certificate.cc \
Grid_Certificate.cc \
Partial_Function.cc \
Polyhedron_nonpublic.cc \
Polyhedron_public.cc \
Polyhedron_chdims.cc \
Polyhedron_widenings.cc \
C_Polyhedron.cc \
NNC_Polyhedron.cc \
Grid_nonpublic.cc \
Grid_public.cc \
Grid_chdims.cc \
Grid_widenings.cc \
BD_Shape.cc \
Octagonal_Shape.cc \
Pointset_Powerset.cc \
CO_Tree.cc \
Sparse_Row.cc \
Dense_Row.cc \
Bit_Matrix.cc \
Bit_Row.cc \
Ph_Status.cc \
Grid_Status.cc \
Variable.cc \
Variables_Set.cc \
Grid_conversion.cc \
Grid_simplify.cc \
stdiobuf.cc \
c_streambuf.cc \
globals.cc \
mp_std_bits.cc \
Weight_Profiler.cc \
version.cc \
$(INCLUDE_FILES) \
$(NESTED_INCLUDE_FILES) \
swapping_sort_templates.hh \
termination.cc \
wrap_string.cc \
Time.cc \
Watchdog.cc \
Threshold_Watcher.cc
#Pointset_Ask_Tell.cc

if NO_UNDEFINED

NO_UNDEFINED_FLAG = -no-undefined

endif NO_UNDEFINED

libppl_la_LIBADD = @extra_libraries@

libppl_la_LDFLAGS = \
$(NO_UNDEFINED_FLAG) \
-version-info $(LIBPPL_LT_CURRENT):$(LIBPPL_LT_REVISION):$(LIBPPL_LT_AGE)

if BUILD_CXX_INTERFACE

include_HEADERS = ppl.hh

else !BUILD_CXX_INTERFACE

noinst_HEADERS = ppl.hh

endif !BUILD_CXX_INTERFACE

nodist_noinst_HEADERS = ppl_include_files.hh

ppl_include_files.hh: $(INCLUDE_FILES) Makefile
	for file in $(INCLUDE_FILES) ; \
	do \
	  i="#include \"$$file\"" ; \
	  ( cd $(top_srcdir)/src ; $(FGREP) -q "$$i" $(INCLUDE_FILES) ) || echo "$$i" ; \
	done > $@

EXTRA_DIST = ppl_header.hh ppl-config.cc.in version.hh.in

if HAVE_PERL

ppl.hh:	$(top_builddir)/ppl-config.h $(top_builddir)/src/version.hh ppl_header.hh ppl_include_files.hh $(INCLUDE_FILES) $(NESTED_INCLUDE_FILES) Makefile $(top_builddir)/utils/build_header
	$(top_builddir)/utils/build_header \
		-I $(top_builddir) -I $(top_builddir)/src \
		$(top_srcdir)/src/ppl_header.hh >$@

BUGS.hh: $(top_builddir)/utils/text2cxxarray $(top_srcdir)/BUGS
	@echo "#ifndef PPL_BUGS_hh" >$@
	@echo "#define PPL_BUGS_hh 1" >>$@
	$(top_builddir)/utils/text2cxxarray --name=BUGS_array --decl-only \
		$(top_srcdir)/BUGS >>$@
	@echo "#endif // !defined(PPL_BUGS_hh)" >>$@
BUGS.cc: $(top_builddir)/utils/text2cxxarray $(top_srcdir)/BUGS
	$(top_builddir)/utils/text2cxxarray --name=BUGS_array \
		$(top_srcdir)/BUGS >$@

COPYING.hh: $(top_builddir)/utils/text2cxxarray $(top_srcdir)/COPYING
	@echo "#ifndef PPL_COPYING_hh" >$@
	@echo "#define PPL_COPYING_hh 1" >>$@
	$(top_builddir)/utils/text2cxxarray --name=COPYING_array --decl-only \
		$(top_srcdir)/COPYING >>$@
	@echo "#endif // !defined(PPL_COPYING_hh)" >>$@

COPYING.cc: $(top_builddir)/utils/text2cxxarray $(top_srcdir)/COPYING
	$(top_builddir)/utils/text2cxxarray --name=COPYING_array \
		$(top_srcdir)/COPYING >$@
CREDITS.hh: $(top_builddir)/utils/text2cxxarray $(top_srcdir)/CREDITS
	@echo "#ifndef PPL_CREDITS_hh" >$@
	@echo "#define PPL_CREDITS_hh 1" >>$@
	$(top_builddir)/utils/text2cxxarray --name=CREDITS_array --decl-only \
		$(top_srcdir)/CREDITS >>$@
	@echo "#endif // !defined(PPL_CREDITS_hh)" >>$@
CREDITS.cc: $(top_builddir)/utils/text2cxxarray $(top_srcdir)/CREDITS
	$(top_builddir)/utils/text2cxxarray --name=CREDITS_array \
		$(top_srcdir)/CREDITS >$@

else !HAVE_PERL

ppl.hh.sed: Makefile
	@echo "/^\/\* BEGIN ppl-config.h \*\/$$/ {"    >$@
	@echo "  r $(top_builddir)/ppl-config.h"      >>$@
	@echo "  : loop"                              >>$@
	@echo "  /\n\/\* END ppl-config.h \*\/$$/ !{" >>$@
	@echo "    N"                                 >>$@
	@echo "    b loop"                            >>$@
	@echo "  }"                                   >>$@
	@echo "  d"                                   >>$@
	@echo "}"                                     >>$@

ppl.hh:	ppl.hh.dist ppl.hh.sed
	$(SED) -f ppl.hh.sed ppl.hh.dist >$@
	rm -f ppl.hh.sed

BUGS.hh:	BUGS.hh.dist
	cp -f BUGS.hh.dist $@
BUGS.cc:	BUGS.cc.dist
	cp -f BUGS.cc.dist $@
COPYING.hh:	COPYING.hh.dist
	cp -f COPYING.hh.dist $@
COPYING.cc:	COPYING.cc.dist
	cp -f COPYING.cc.dist $@
CREDITS.hh:	CREDITS.hh.dist
	cp -f CREDITS.hh.dist $@
CREDITS.cc:	CREDITS.cc.dist
	cp -f CREDITS.cc.dist $@

endif !HAVE_PERL

if USE_PRECOMPILED_HEADERS

ppl.hh.gch: ppl.hh
	$(CXXCOMPILE) -xc++-header -o $@ ppl.hh

PRECOMPILED = ppl.hh.gch

else !USE_PRECOMPILED_HEADERS

PRECOMPILED =

endif !USE_PRECOMPILED_HEADERS

BUILT_SOURCES = \
$(include_HEADERS) \
$(nodist_noinst_HEADERS) \
$(noinst_HEADERS) \
$(PRECOMPILED) \
BUGS.hh \
BUGS.cc \
COPYING.hh \
COPYING.cc \
CREDITS.hh \
CREDITS.cc

# ppl.hh is not distributed.
# ppl.hh.dist, which is distributed, is a copy of ppl.hh;
# likewise for BUGS.cc.dist, COPYING.cc.dist and CREDITS.cc.dist.
dist-hook:
	mv -f $(distdir)/ppl.hh $(distdir)/ppl.hh.dist
	mv -f $(distdir)/BUGS.hh $(distdir)/BUGS.hh.dist
	mv -f $(distdir)/BUGS.cc $(distdir)/BUGS.cc.dist
	mv -f $(distdir)/COPYING.hh $(distdir)/COPYING.hh.dist
	mv -f $(distdir)/COPYING.cc $(distdir)/COPYING.cc.dist
	mv -f $(distdir)/CREDITS.hh $(distdir)/CREDITS.hh.dist
	mv -f $(distdir)/CREDITS.cc $(distdir)/CREDITS.cc.dist

CLEANFILES = $(BUILT_SOURCES)

$(top_builddir)/utils/build_header:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils build_header

$(top_builddir)/utils/text2cxxarray:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/utils text2cxxarray

$(top_builddir)/ppl-config.h:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir) ppl-config.h
