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

JAVAROOT = .

interface_generator_files = \
ppl_interface_generator_java_test_java.m4 \
ppl_interface_generator_java_test_java_code.m4

interface_generator_dependencies = \
$(interface_generator_files) \
$(top_builddir)/interfaces/ppl_interface_instantiations.m4 \
../../ppl_interface_generator_common.m4 \
../../ppl_interface_generator_common_dat.m4 \
../../ppl_interface_generator_copyright \
../../ppl_interface_generator_common_procedure_generators.m4 \
../ppl_interface_generator_java_procedure_generators.m4 \
ppl_java_tests_common

# Even if the Java sources are not really needed, they are built by m4,
# we need to include them to invoke the Automake Java build system.
EXTRA_DIST = \
$(interface_generator_files) \
Test_Executor.java \
C_Polyhedron_test1.java \
NNC_Polyhedron_test1.java \
MIP_Problem_test1.java \
PIP_Problem_test1.java \
Parma_Polyhedra_Library_test1.java \
Parma_Polyhedra_Library_test2.java \
PPL_Test.java \
ppl_java_tests_common

if ENABLE_SHARED

check_JAVA = \
ppl_java_generated_tests.java \
Test_Executor.java \
C_Polyhedron_test1.java \
NNC_Polyhedron_test1.java \
MIP_Problem_test1.java \
PIP_Problem_test1.java \
Parma_Polyhedra_Library_test1.java \
Parma_Polyhedra_Library_test2.java \
PPL_Test.java

CLASSPATH = ../ppl_java.jar:.
JAVACFLAGS = -classpath $(CLASSPATH)

PPL_DLOPEN = -dlopen ../../../src/libppl.la

java_test_environment = \
$(LIBTOOL) --mode=execute $(PPL_DLOPEN) $(PWL_DLOPEN) \
	$(JAVA) -classpath .:../ppl_java.jar \
		-Djava.library.path=../jni/.libs

ppl_java_generated_tests.java: $(interface_generator_dependencies)
	$(M4) --prefix-builtin -I../.. \
		-I$(srcdir) -I$(srcdir)/.. -I$(srcdir)/../.. \
		$(srcdir)/ppl_interface_generator_java_test_java.m4 \
			> $@

check-local: ppl_java_generated_tests.java
	$(java_test_environment) ppl_java_generated_tests
	$(java_test_environment) C_Polyhedron_test1
	$(java_test_environment) NNC_Polyhedron_test1
	$(java_test_environment) MIP_Problem_test1
	$(java_test_environment) PIP_Problem_test1
	$(java_test_environment) Parma_Polyhedra_Library_test1
	$(java_test_environment) Parma_Polyhedra_Library_test2

CLEANFILES = \
ppl_java_generated_tests.java

endif ENABLE_SHARED

$(top_builddir)/interfaces/ppl_interface_instantiations.m4:
	$(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/interfaces ppl_interface_instantiations.m4
