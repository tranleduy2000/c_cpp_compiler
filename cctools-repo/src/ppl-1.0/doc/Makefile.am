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

# All the documentation in docdir.
docdir = @docdir@
htmldir = $(docdir)
pdfdir = $(docdir)
psdir = $(docdir)
txtdir = $(docdir)

DOXYGEN = doxygen

PDF_LICENSES = \
gpl.pdf \
fdl.pdf

PS_LICENSES = \
gpl.ps.gz \
fdl.ps.gz

TXT_LICENSES = \
gpl.txt \
fdl.txt

LICENSES = \
$(PDF_LICENSES) \
$(TXT_LICENSES) \
$(PS_LICENSES)

BIBLIOGRAPHIES = \
ppl.bib \
ppl_citations.bib

##### Library Core Manuals (C++ Interface) #####

USER_CORE_HTML_MANUAL = ppl-user-@VERSION@-html.tar.gz
DEVREF_CORE_HTML_MANUAL = ppl-devref-@VERSION@-html.tar.gz

USER_CORE_PDF_MANUAL = ppl-user-@VERSION@.pdf
DEVREF_CORE_PDF_MANUAL = ppl-devref-@VERSION@.pdf

USER_CORE_PS_MANUAL = ppl-user-@VERSION@.ps.gz
DEVREF_CORE_PS_MANUAL = ppl-devref-@VERSION@.ps.gz

USER_CORE_HTML_DIR = ppl-user-@VERSION@-html
DEVREF_CORE_HTML_DIR = ppl-devref-@VERSION@-html

# NOTE: this variable is used to record Makefile dependencies.
# The file path is that of the source directory.
DOX_LICENSE_FILES = \
gpl.dox \
fdl.dox

# NOTE: this variable is used to populate Doxygen configuration tag INPUT.
# The file path is that of the build directory, hence we ca not use
# variable DOX_LICENSE_FILES defined above.
DOX_LICENSE_INPUT = \
$(srcdir)/gpl.dox \
$(srcdir)/fdl.dox

DOX_FILES = \
$(DOX_LICENSE_FILES) \
definitions.dox

USER_CORE_STUFF = \
$(DOX_FILES) \
user.tex \
ppl.sty \
../src/ppl_header.hh ../src/ppl_include_files.hh

DEVREF_CORE_STUFF = \
$(DOX_FILES) \
devref.tex \
ppl.sty \
$(PPL_SOURCE_FILES)


if BUILD_C_INTERFACE

# C Language Interface Manuals

USER_C_INTERFACE_HTML_MANUAL = \
ppl-user-c-interface-@VERSION@-html.tar.gz
DEVREF_C_INTERFACE_HTML_MANUAL = \
ppl-devref-c-interface-@VERSION@-html.tar.gz

USER_C_INTERFACE_PDF_MANUAL = \
ppl-user-c-interface-@VERSION@.pdf
DEVREF_C_INTERFACE_PDF_MANUAL = \
ppl-devref-c-interface-@VERSION@.pdf

USER_C_INTERFACE_PS_MANUAL = \
ppl-user-c-interface-@VERSION@.ps.gz
DEVREF_C_INTERFACE_PS_MANUAL = \
ppl-devref-c-interface-@VERSION@.ps.gz

USER_C_INTERFACE_HTML_DIR = \
ppl-user-c-interface-@VERSION@-html
DEVREF_C_INTERFACE_HTML_DIR = \
ppl-devref-c-interface-@VERSION@-html

COMMON_C_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
ppl.sty \
../configure.ac \
../interfaces/C/ppl_c_version.h.in \
../interfaces/C/ppl_c_header.h \
../interfaces/C/C_interface.dox

USER_C_INTERFACE_STUFF = \
$(COMMON_C_INTERFACE_STUFF) \
user-language-interface.tex

DEVREF_C_INTERFACE_STUFF = \
$(COMMON_C_INTERFACE_STUFF) \
devref-language-interface.tex \
../interfaces/C/ppl_c_implementation_common.defs.hh \
../interfaces/C/ppl_c_implementation_common.inlines.hh \
../interfaces/C/ppl_c_implementation_common.cc


# Configuration Dependent C Language Interface Manuals.

USER_CONFIGURED_C_INTERFACE_HTML_MANUAL = \
ppl-user-configured-c-interface-@VERSION@-html.tar.gz
DEVREF_CONFIGURED_C_INTERFACE_HTML_MANUAL = \
ppl-devref-configured-c-interface-@VERSION@-html.tar.gz

USER_CONFIGURED_C_INTERFACE_PDF_MANUAL = \
ppl-user-configured-c-interface-@VERSION@.pdf
DEVREF_CONFIGURED_C_INTERFACE_PDF_MANUAL = \
ppl-devref-configured-c-interface-@VERSION@.pdf

USER_CONFIGURED_C_INTERFACE_PS_MANUAL = \
ppl-user-configured-c-interface-@VERSION@.ps.gz
DEVREF_CONFIGURED_C_INTERFACE_PS_MANUAL = \
ppl-devref-configured-c-interface-@VERSION@.ps.gz

USER_CONFIGURED_C_INTERFACE_HTML_DIR = \
ppl-user-configured-c-interface-@VERSION@-html
DEVREF_CONFIGURED_C_INTERFACE_HTML_DIR = \
ppl-devref-configured-c-interface-@VERSION@-html

USER_CONFIGURED_C_INTERFACE_STUFF = \
$(COMMON_C_INTERFACE_STUFF) \
$(top_builddir)/interfaces/C/ppl_c.h

DEVREF_CONFIGURED_C_INTERFACE_STUFF = \
$(COMMON_C_INTERFACE_STUFF) \
$(top_builddir)/interfaces/C/ppl_c.h \
../interfaces/C/ppl_c_implementation_common.defs.hh \
../interfaces/C/ppl_c_implementation_common.inlines.hh \
../interfaces/C/ppl_c_implementation_common.cc

endif BUILD_C_INTERFACE


if BUILD_SOME_PROLOG_INTERFACES

# Prolog Language Interface Manuals

USER_PROLOG_INTERFACE_HTML_MANUAL = \
ppl-user-prolog-interface-@VERSION@-html.tar.gz
DEVREF_PROLOG_INTERFACE_HTML_MANUAL = \
ppl-devref-prolog-interface-@VERSION@-html.tar.gz

USER_PROLOG_INTERFACE_PDF_MANUAL = \
ppl-user-prolog-interface-@VERSION@.pdf
DEVREF_PROLOG_INTERFACE_PDF_MANUAL = \
ppl-devref-prolog-interface-@VERSION@.pdf

USER_PROLOG_INTERFACE_PS_MANUAL = \
ppl-user-prolog-interface-@VERSION@.ps.gz
DEVREF_PROLOG_INTERFACE_PS_MANUAL = \
ppl-devref-prolog-interface-@VERSION@.ps.gz

USER_PROLOG_INTERFACE_HTML_DIR = \
ppl-user-prolog-interface-@VERSION@-html
DEVREF_PROLOG_INTERFACE_HTML_DIR = \
ppl-devref-prolog-interface-@VERSION@-html


PROLOG_INTERFACE_ADDITIONAL_DOX_PAGES = \
../interfaces/Prolog/Prolog_interface_sysindep.dox \
../interfaces/Prolog/Prolog_interface_sysindep_domains.dox \
../interfaces/Prolog/Prolog_interface_compilation.dox \
../interfaces/Prolog/Prolog_interface_sysdep.dox

USER_PROLOG_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
user-language-interface.tex \
ppl.sty \
../interfaces/Prolog/Prolog_interface.dox \
$(PROLOG_INTERFACE_ADDITIONAL_DOX_PAGES)

DEVREF_PROLOG_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
devref-language-interface.tex \
ppl.sty \
../interfaces/Prolog/Prolog_interface.dox \
$(PROLOG_INTERFACE_ADDITIONAL_DOX_PAGES) \
../interfaces/Prolog/ppl_prolog_common.defs.hh \
../interfaces/Prolog/ppl_prolog_common.inlines.hh \
../interfaces/Prolog/ppl_prolog_common.cc


# Configuration Dependent Prolog Language Interface Manuals

USER_CONFIGURED_PROLOG_INTERFACE_HTML_MANUAL = \
ppl-user-configured-prolog-interface-@VERSION@-html.tar.gz
DEVREF_CONFIGURED_PROLOG_INTERFACE_HTML_MANUAL = \
ppl-devref-configured-prolog-interface-@VERSION@-html.tar.gz

USER_CONFIGURED_PROLOG_INTERFACE_PDF_MANUAL = \
ppl-user-configured-prolog-interface-@VERSION@.pdf
DEVREF_CONFIGURED_PROLOG_INTERFACE_PDF_MANUAL = \
ppl-devref-configured-prolog-interface-@VERSION@.pdf

USER_CONFIGURED_PROLOG_INTERFACE_PS_MANUAL = \
ppl-user-configured-prolog-interface-@VERSION@.ps.gz
DEVREF_CONFIGURED_PROLOG_INTERFACE_PS_MANUAL = \
ppl-devref-configured-prolog-interface-@VERSION@.ps.gz

USER_CONFIGURED_PROLOG_INTERFACE_HTML_DIR = \
ppl-user-configured-prolog-interface-@VERSION@-html
DEVREF_CONFIGURED_PROLOG_INTERFACE_HTML_DIR = \
ppl-devref-configured-prolog-interface-@VERSION@-html

USER_CONFIGURED_PROLOG_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
user-language-interface.tex \
ppl.sty \
../interfaces/Prolog/Prolog_configured_interface.dox \
$(PROLOG_INTERFACE_ADDITIONAL_DOX_PAGES)

DEVREF_CONFIGURED_PROLOG_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
devref-language-interface.tex \
ppl.sty \
../interfaces/Prolog/Prolog_configured_interface.dox \
$(PROLOG_INTERFACE_ADDITIONAL_DOX_PAGES) \
../interfaces/Prolog/ppl_prolog_common.defs.hh \
../interfaces/Prolog/ppl_prolog_common.inlines.hh \
../interfaces/Prolog/ppl_prolog_common.cc \
$(top_builddir)/interfaces/Prolog/ppl_prolog_domains.hh \
$(top_builddir)/interfaces/Prolog/ppl_prolog_domains.cc.stamp

endif BUILD_SOME_PROLOG_INTERFACES


if BUILD_OCAML_INTERFACE

# OCaml Language Interface Manuals

USER_OCAML_INTERFACE_HTML_MANUAL = \
ppl-user-ocaml-interface-@VERSION@-html.tar.gz
DEVREF_OCAML_INTERFACE_HTML_MANUAL = \
ppl-devref-ocaml-interface-@VERSION@-html.tar.gz

USER_OCAML_INTERFACE_PDF_MANUAL = \
ppl-user-ocaml-interface-@VERSION@.pdf
DEVREF_OCAML_INTERFACE_PDF_MANUAL = \
ppl-devref-ocaml-interface-@VERSION@.pdf

USER_OCAML_INTERFACE_PS_MANUAL = \
ppl-user-ocaml-interface-@VERSION@.ps.gz
DEVREF_OCAML_INTERFACE_PS_MANUAL = \
ppl-devref-ocaml-interface-@VERSION@.ps.gz

USER_OCAML_INTERFACE_HTML_DIR = \
ppl-user-ocaml-interface-@VERSION@-html
DEVREF_OCAML_INTERFACE_HTML_DIR = \
ppl-devref-ocaml-interface-@VERSION@-html

USER_OCAML_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
user-language-interface.tex \
ppl.sty \
../interfaces/OCaml/OCaml_interface.dox \
../interfaces/OCaml/ppl_ocaml_globals.mli

DEVREF_OCAML_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
devref-language-interface.tex \
ppl.sty \
../interfaces/OCaml/OCaml_interface.dox \
../interfaces/OCaml/ppl_ocaml_common.defs.hh \
../interfaces/OCaml/ppl_ocaml_common.inlines.hh \
../interfaces/OCaml/ppl_ocaml_common.cc \
../interfaces/OCaml/ppl_ocaml_globals.mli


# Configuration Dependent OCaml Language Interface Manuals.

USER_CONFIGURED_OCAML_INTERFACE_HTML_MANUAL = \
ppl-user-configured-ocaml-interface-@VERSION@-html.tar.gz
DEVREF_CONFIGURED_OCAML_INTERFACE_HTML_MANUAL = \
ppl-devref-configured-ocaml-interface-@VERSION@-html.tar.gz

USER_CONFIGURED_OCAML_INTERFACE_PDF_MANUAL = \
ppl-user-configured-ocaml-interface-@VERSION@.pdf
DEVREF_CONFIGURED_OCAML_INTERFACE_PDF_MANUAL = \
ppl-devref-configured-ocaml-interface-@VERSION@.pdf

USER_CONFIGURED_OCAML_INTERFACE_PS_MANUAL = \
ppl-user-configured-ocaml-interface-@VERSION@.ps.gz
DEVREF_CONFIGURED_OCAML_INTERFACE_PS_MANUAL = \
ppl-devref-configured-ocaml-interface-@VERSION@.ps.gz

USER_CONFIGURED_OCAML_INTERFACE_HTML_DIR = \
ppl-user-configured-ocaml-interface-@VERSION@-html
DEVREF_CONFIGURED_OCAML_INTERFACE_HTML_DIR = \
ppl-devref-configured-ocaml-interface-@VERSION@-html

USER_CONFIGURED_OCAML_INTERFACE_STUFF = \
$(USER_OCAML_INTERFACE_STUFF) \
$(top_builddir)/interfaces/OCaml/ppl_ocaml.mli

DEVREF_CONFIGURED_OCAML_INTERFACE_STUFF = \
$(DEVREF_OCAML_INTERFACE_STUFF) \
$(top_builddir)/interfaces/OCaml/ppl_ocaml.mli

endif BUILD_OCAML_INTERFACE

if BUILD_JAVA_INTERFACE

# Java Language Interface Manuals

USER_JAVA_INTERFACE_HTML_MANUAL = \
ppl-user-java-interface-@VERSION@-html.tar.gz
DEVREF_JAVA_INTERFACE_HTML_MANUAL = \
ppl-devref-java-interface-@VERSION@-html.tar.gz

USER_JAVA_INTERFACE_PDF_MANUAL = \
ppl-user-java-interface-@VERSION@.pdf
DEVREF_JAVA_INTERFACE_PDF_MANUAL = \
ppl-devref-java-interface-@VERSION@.pdf

USER_JAVA_INTERFACE_PS_MANUAL = \
ppl-user-java-interface-@VERSION@.ps.gz
DEVREF_JAVA_INTERFACE_PS_MANUAL = \
ppl-devref-java-interface-@VERSION@.ps.gz

USER_JAVA_INTERFACE_HTML_DIR = \
ppl-user-java-interface-@VERSION@-html
DEVREF_JAVA_INTERFACE_HTML_DIR = \
ppl-devref-java-interface-@VERSION@-html

JAVA_INTERFACE_JAVA_FILES = \
../interfaces/Java/parma_polyhedra_library/Fake_Class_for_Doxygen.java \
../interfaces/Java/parma_polyhedra_library/By_Reference.java \
../interfaces/Java/parma_polyhedra_library/Coefficient.java \
../interfaces/Java/parma_polyhedra_library/Complexity_Class.java \
../interfaces/Java/parma_polyhedra_library/Congruence.java \
../interfaces/Java/parma_polyhedra_library/Congruence_System.java \
../interfaces/Java/parma_polyhedra_library/Constraint.java \
../interfaces/Java/parma_polyhedra_library/Constraint_System.java \
../interfaces/Java/parma_polyhedra_library/Control_Parameter_Name.java \
../interfaces/Java/parma_polyhedra_library/Control_Parameter_Value.java \
../interfaces/Java/parma_polyhedra_library/Degenerate_Element.java \
../interfaces/Java/parma_polyhedra_library/Domain_Error_Exception.java \
../interfaces/Java/parma_polyhedra_library/Generator.java \
../interfaces/Java/parma_polyhedra_library/Generator_System.java \
../interfaces/Java/parma_polyhedra_library/Generator_Type.java \
../interfaces/Java/parma_polyhedra_library/Grid_Generator.java \
../interfaces/Java/parma_polyhedra_library/Grid_Generator_System.java \
../interfaces/Java/parma_polyhedra_library/Grid_Generator_Type.java \
../interfaces/Java/parma_polyhedra_library/Invalid_Argument_Exception.java \
../interfaces/Java/parma_polyhedra_library/Length_Error_Exception.java \
../interfaces/Java/parma_polyhedra_library/Linear_Expression_Coefficient.java \
../interfaces/Java/parma_polyhedra_library/Linear_Expression_Difference.java \
../interfaces/Java/parma_polyhedra_library/Linear_Expression.java \
../interfaces/Java/parma_polyhedra_library/Linear_Expression_Sum.java \
../interfaces/Java/parma_polyhedra_library/Linear_Expression_Times.java \
../interfaces/Java/parma_polyhedra_library/Linear_Expression_Unary_Minus.java \
../interfaces/Java/parma_polyhedra_library/Linear_Expression_Variable.java \
../interfaces/Java/parma_polyhedra_library/Logic_Error_Exception.java \
../interfaces/Java/parma_polyhedra_library/MIP_Problem.java \
../interfaces/Java/parma_polyhedra_library/MIP_Problem_Status.java \
../interfaces/Java/parma_polyhedra_library/Optimization_Mode.java \
../interfaces/Java/parma_polyhedra_library/Overflow_Error_Exception.java \
../interfaces/Java/parma_polyhedra_library/Pair.java \
../interfaces/Java/parma_polyhedra_library/Parma_Polyhedra_Library.java \
../interfaces/Java/parma_polyhedra_library/Partial_Function.java \
../interfaces/Java/parma_polyhedra_library/Poly_Con_Relation.java \
../interfaces/Java/parma_polyhedra_library/Poly_Gen_Relation.java \
../interfaces/Java/parma_polyhedra_library/PPL_Object.java \
../interfaces/Java/parma_polyhedra_library/Relation_Symbol.java \
../interfaces/Java/parma_polyhedra_library/Variable.java \
../interfaces/Java/parma_polyhedra_library/Variables_Set.java

USER_JAVA_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
user-language-interface.tex \
ppl.sty \
$(JAVA_INTERFACE_JAVA_FILES)

DEVREF_JAVA_INTERFACE_STUFF = \
$(DOX_LICENSE_FILES) \
devref-language-interface.tex \
ppl.sty \
$(JAVA_INTERFACE_JAVA_FILES) \
../interfaces/Java/jni/ppl_java_common.cc \
../interfaces/Java/jni/ppl_java_common.defs.hh \
../interfaces/Java/jni/ppl_java_common.inlines.hh \
../interfaces/Java/jni/ppl_java_globals.cc


# Configuration Dependent Java Language Interface Manuals.

USER_CONFIGURED_JAVA_INTERFACE_HTML_MANUAL = \
ppl-user-configured-java-interface-@VERSION@-html.tar.gz
DEVREF_CONFIGURED_JAVA_INTERFACE_HTML_MANUAL = \
ppl-devref-configured-java-interface-@VERSION@-html.tar.gz

USER_CONFIGURED_JAVA_INTERFACE_PDF_MANUAL = \
ppl-user-configured-java-interface-@VERSION@.pdf
DEVREF_CONFIGURED_JAVA_INTERFACE_PDF_MANUAL = \
ppl-devref-configured-java-interface-@VERSION@.pdf

USER_CONFIGURED_JAVA_INTERFACE_PS_MANUAL = \
ppl-user-configured-java-interface-@VERSION@.ps.gz
DEVREF_CONFIGURED_JAVA_INTERFACE_PS_MANUAL = \
ppl-devref-configured-java-interface-@VERSION@.ps.gz

USER_CONFIGURED_JAVA_INTERFACE_HTML_DIR = \
ppl-user-configured-java-interface-@VERSION@-html
DEVREF_CONFIGURED_JAVA_INTERFACE_HTML_DIR = \
ppl-devref-configured-java-interface-@VERSION@-html

USER_CONFIGURED_JAVA_INTERFACE_STUFF = \
$(USER_JAVA_INTERFACE_STUFF)

DEVREF_CONFIGURED_JAVA_INTERFACE_STUFF = \
$(DEVREF_JAVA_INTERFACE_STUFF)

endif BUILD_JAVA_INTERFACE

###################################################################

# Grouping of dirs and manuals.

USER_HTML_DIRS = \
$(USER_CORE_HTML_DIR) \
$(USER_C_INTERFACE_HTML_DIR) \
$(USER_PROLOG_INTERFACE_HTML_DIR) \
$(USER_OCAML_INTERFACE_HTML_DIR) \
$(USER_JAVA_INTERFACE_HTML_DIR) \
$(USER_CONFIGURED_C_INTERFACE_HTML_DIR) \
$(USER_CONFIGURED_PROLOG_INTERFACE_HTML_DIR) \
$(USER_CONFIGURED_OCAML_INTERFACE_HTML_DIR) \
$(USER_CONFIGURED_JAVA_INTERFACE_HTML_DIR)

DEVREF_HTML_DIRS = \
$(DEVREF_CORE_HTML_DIR) \
$(DEVREF_C_INTERFACE_HTML_DIR) \
$(DEVREF_PROLOG_INTERFACE_HTML_DIR) \
$(DEVREF_OCAML_INTERFACE_HTML_DIR) \
$(DEVREF_JAVA_INTERFACE_HTML_DIR) \
$(DEVREF_CONFIGURED_C_INTERFACE_HTML_DIR) \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_HTML_DIR) \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_HTML_DIR) \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_HTML_DIR)

HTML_DIRS = $(USER_HTML_DIRS) $(DEVREF_HTML_DIRS)

##### Distributed documentation #####

DIST_HTML_DOCS = \
$(USER_CORE_HTML_MANUAL) \
$(USER_C_INTERFACE_HTML_MANUAL) \
$(USER_PROLOG_INTERFACE_HTML_MANUAL) \
$(USER_OCAML_INTERFACE_HTML_MANUAL) \
$(USER_JAVA_INTERFACE_HTML_MANUAL)

DIST_PDF_DOCS = \
$(PDF_LICENSES) \
$(USER_CORE_PDF_MANUAL) \
$(USER_C_INTERFACE_PDF_MANUAL) \
$(USER_PROLOG_INTERFACE_PDF_MANUAL) \
$(USER_OCAML_INTERFACE_PDF_MANUAL) \
$(USER_JAVA_INTERFACE_PDF_MANUAL)

DIST_PS_DOCS = \
$(PS_LICENSES) \
$(USER_CORE_PS_MANUAL) \
$(USER_C_INTERFACE_PS_MANUAL) \
$(USER_PROLOG_INTERFACE_PS_MANUAL) \
$(USER_OCAML_INTERFACE_PS_MANUAL) \
$(USER_JAVA_INTERFACE_PS_MANUAL)

DIST_TXT_DOCS = \
$(TXT_LICENSES)

DIST_DOCS = \
$(DIST_HTML_DOCS) \
$(DIST_PDF_DOCS) \
$(DIST_PS_DOCS) \
$(DIST_TXT_DOCS)

NONDIST_MANUALS = \
$(USER_CONFIGURED_C_INTERFACE_HTML_MANUAL) \
$(USER_CONFIGURED_PROLOG_INTERFACE_HTML_MANUAL) \
$(USER_CONFIGURED_OCAML_INTERFACE_HTML_MANUAL) \
$(USER_CONFIGURED_JAVA_INTERFACE_HTML_MANUAL) \
$(USER_CONFIGURED_C_INTERFACE_PDF_MANUAL) \
$(USER_CONFIGURED_PROLOG_INTERFACE_PDF_MANUAL) \
$(USER_CONFIGURED_OCAML_INTERFACE_PDF_MANUAL) \
$(USER_CONFIGURED_JAVA_INTERFACE_PDF_MANUAL) \
$(USER_CONFIGURED_C_INTERFACE_PS_MANUAL) \
$(USER_CONFIGURED_PROLOG_INTERFACE_PS_MANUAL) \
$(USER_CONFIGURED_OCAML_INTERFACE_PS_MANUAL) \
$(USER_CONFIGURED_JAVA_INTERFACE_PS_MANUAL) \
$(DEVREF_CORE_HTML_MANUAL) \
$(DEVREF_C_INTERFACE_HTML_MANUAL) \
$(DEVREF_PROLOG_INTERFACE_HTML_MANUAL) \
$(DEVREF_OCAML_INTERFACE_HTML_MANUAL) \
$(DEVREF_JAVA_INTERFACE_HTML_MANUAL) \
$(DEVREF_CORE_PDF_MANUAL) \
$(DEVREF_C_INTERFACE_PDF_MANUAL) \
$(DEVREF_PROLOG_INTERFACE_PDF_MANUAL) \
$(DEVREF_OCAML_INTERFACE_PDF_MANUAL) \
$(DEVREF_JAVA_INTERFACE_PDF_MANUAL) \
$(DEVREF_CORE_PS_MANUAL) \
$(DEVREF_C_INTERFACE_PS_MANUAL) \
$(DEVREF_PROLOG_INTERFACE_PS_MANUAL) \
$(DEVREF_OCAML_INTERFACE_PS_MANUAL) \
$(DEVREF_JAVA_INTERFACE_PS_MANUAL) \
$(DEVREF_CONFIGURED_C_INTERFACE_HTML_MANUAL) \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_HTML_MANUAL) \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_HTML_MANUAL) \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_HTML_MANUAL) \
$(DEVREF_CONFIGURED_C_INTERFACE_PDF_MANUAL) \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_PDF_MANUAL) \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_PDF_MANUAL) \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_PDF_MANUAL) \
$(DEVREF_CONFIGURED_C_INTERFACE_PS_MANUAL) \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_PS_MANUAL) \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_PS_MANUAL) \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_PS_MANUAL)

dist_man_MANS = \
libppl.3 \
libppl_c.3 \
ppl-config.1

##### Build Targets #####

# Do nothing by default.
all:

user-html: \
$(USER_CORE_HTML_MANUAL) \
$(USER_C_INTERFACE_HTML_MANUAL) \
$(USER_PROLOG_INTERFACE_HTML_MANUAL) \
$(USER_OCAML_INTERFACE_HTML_MANUAL) \
$(USER_JAVA_INTERFACE_HTML_MANUAL)

user-pdf: \
$(USER_CORE_PDF_MANUAL) \
$(USER_C_INTERFACE_PDF_MANUAL) \
$(USER_PROLOG_INTERFACE_PDF_MANUAL) \
$(USER_OCAML_INTERFACE_PDF_MANUAL) \
$(USER_JAVA_INTERFACE_PDF_MANUAL)

user-ps: \
$(USER_CORE_PS_MANUAL) \
$(USER_C_INTERFACE_PS_MANUAL) \
$(USER_PROLOG_INTERFACE_PS_MANUAL) \
$(USER_OCAML_INTERFACE_PS_MANUAL) \
$(USER_JAVA_INTERFACE_PS_MANUAL)

user: user-html user-pdf user-ps

devref-html: \
$(DEVREF_CORE_HTML_MANUAL) \
$(DEVREF_C_INTERFACE_HTML_MANUAL) \
$(DEVREF_PROLOG_INTERFACE_HTML_MANUAL) \
$(DEVREF_OCAML_INTERFACE_HTML_MANUAL) \
$(DEVREF_JAVA_INTERFACE_HTML_MANUAL)

devref-pdf: \
$(DEVREF_CORE_PDF_MANUAL) \
$(DEVREF_C_INTERFACE_PDF_MANUAL) \
$(DEVREF_PROLOG_INTERFACE_PDF_MANUAL) \
$(DEVREF_OCAML_INTERFACE_PDF_MANUAL) \
$(DEVREF_JAVA_INTERFACE_PDF_MANUAL)

devref-ps: \
$(DEVREF_CORE_PS_MANUAL) \
$(DEVREF_C_INTERFACE_PS_MANUAL) \
$(DEVREF_PROLOG_INTERFACE_PS_MANUAL) \
$(DEVREF_OCAML_INTERFACE_PS_MANUAL) \
$(DEVREF_JAVA_INTERFACE_PS_MANUAL)

devref: devref-html devref-pdf devref-ps

user-configured-html: \
$(USER_CONFIGURED_C_INTERFACE_HTML_MANUAL) \
$(USER_CONFIGURED_PROLOG_INTERFACE_HTML_MANUAL) \
$(USER_CONFIGURED_OCAML_INTERFACE_HTML_MANUAL) \
$(USER_CONFIGURED_JAVA_INTERFACE_HTML_MANUAL)

user-configured-pdf: \
$(USER_CONFIGURED_C_INTERFACE_PDF_MANUAL) \
$(USER_CONFIGURED_PROLOG_INTERFACE_PDF_MANUAL) \
$(USER_CONFIGURED_OCAML_INTERFACE_PDF_MANUAL) \
$(USER_CONFIGURED_JAVA_INTERFACE_PDF_MANUAL)

user-configured-ps: \
$(USER_CONFIGURED_C_INTERFACE_PS_MANUAL) \
$(USER_CONFIGURED_PROLOG_INTERFACE_PS_MANUAL) \
$(USER_CONFIGURED_OCAML_INTERFACE_PS_MANUAL) \
$(USER_CONFIGURED_JAVA_INTERFACE_PS_MANUAL)

user-configured: \
user-configured-html \
user-configured-pdf \
user-configured-ps

devref-configured-html: \
$(DEVREF_CONFIGURED_C_INTERFACE_HTML_MANUAL) \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_HTML_MANUAL) \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_HTML_MANUAL) \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_HTML_MANUAL)

devref-configured-pdf: \
$(DEVREF_CONFIGURED_C_INTERFACE_PDF_MANUAL) \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_PDF_MANUAL) \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_PDF_MANUAL) \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_PDF_MANUAL)

devref-configured-ps: \
$(DEVREF_CONFIGURED_C_INTERFACE_PS_MANUAL) \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_PS_MANUAL) \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_PS_MANUAL) \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_PS_MANUAL)

devref-configured: \
devref-configured-html \
devref-configured-pdf \
devref-configured-ps

user-all: user user-configured
devref-all: devref devref-configured

world: user-all devref-all

# Installation: all the distributed *core* docs, as well as
# the distributed docs for all the *enabled* language interfaces.

if BUILD_C_INTERFACE

INSTALL_C_INTERFACE_HTML_DOCS = $(USER_C_INTERFACE_HTML_MANUAL)
INSTALL_C_INTERFACE_PDF_DOCS = $(USER_C_INTERFACE_PDF_MANUAL)
INSTALL_C_INTERFACE_PS_DOCS = $(USER_C_INTERFACE_PS_MANUAL)

endif BUILD_C_INTERFACE

if BUILD_JAVA_INTERFACE

INSTALL_JAVA_INTERFACE_HTML_DOCS = $(USER_JAVA_INTERFACE_HTML_MANUAL)
INSTALL_JAVA_INTERFACE_PDF_DOCS = $(USER_JAVA_INTERFACE_PDF_MANUAL)
INSTALL_JAVA_INTERFACE_PS_DOCS = $(USER_JAVA_INTERFACE_PS_MANUAL)

endif BUILD_JAVA_INTERFACE

if BUILD_OCAML_INTERFACE

INSTALL_OCAML_INTERFACE_HTML_DOCS = $(USER_OCAML_INTERFACE_HTML_MANUAL)
INSTALL_OCAML_INTERFACE_PDF_DOCS = $(USER_OCAML_INTERFACE_PDF_MANUAL)
INSTALL_OCAML_INTERFACE_PS_DOCS = $(USER_OCAML_INTERFACE_PS_MANUAL)

endif BUILD_OCAML_INTERFACE

if BUILD_SOME_PROLOG_INTERFACES

INSTALL_PROLOG_INTERFACE_HTML_DOCS = $(USER_PROLOG_INTERFACE_HTML_MANUAL)
INSTALL_PROLOG_INTERFACE_PDF_DOCS = $(USER_PROLOG_INTERFACE_PDF_MANUAL)
INSTALL_PROLOG_INTERFACE_PS_DOCS = $(USER_PROLOG_INTERFACE_PS_MANUAL)

endif BUILD_SOME_PROLOG_INTERFACES


INSTALL_HTML_DOCS = \
$(USER_CORE_HTML_MANUAL) \
$(INSTALL_C_INTERFACE_HTML_DOCS) \
$(INSTALL_JAVA_INTERFACE_HTML_DOCS) \
$(INSTALL_OCAML_INTERFACE_HTML_DOCS) \
$(INSTALL_PROLOG_INTERFACE_HTML_DOCS)

INSTALL_PDF_DOCS = \
$(PDF_LICENSES) \
$(USER_CORE_PDF_MANUAL) \
$(INSTALL_C_INTERFACE_PDF_DOCS) \
$(INSTALL_JAVA_INTERFACE_PDF_DOCS) \
$(INSTALL_OCAML_INTERFACE_PDF_DOCS) \
$(INSTALL_PROLOG_INTERFACE_PDF_DOCS)

INSTALL_PS_DOCS = \
$(PS_LICENSES) \
$(USER_CORE_PS_MANUAL) \
$(INSTALL_C_INTERFACE_PS_DOCS) \
$(INSTALL_JAVA_INTERFACE_PS_DOCS) \
$(INSTALL_OCAML_INTERFACE_PS_DOCS) \
$(INSTALL_PROLOG_INTERFACE_PS_DOCS)


install-data-local: \
install-html \
install-pdf \
install-ps \
install-txt

installdirs-local: \
installdirs-html \
installdirs-pdf \
installdirs-ps \
installdirs-txt

uninstall-local: \
uninstall-html \
uninstall-pdf \
uninstall-ps \
uninstall-txt


if BUILD_DOCUMENTATION

CONTINUE_IF_NO_DOC = true

else !BUILD_DOCUMENTATION

CONTINUE_IF_NO_DOC = continue

endif !BUILD_DOCUMENTATION


# Installation and uninstallation of HTML documentation.
install-html:
	for ball in $(INSTALL_HTML_DOCS); do \
	  test -f $$ball || test -f $(srcdir)/$$ball \
	    || $(CONTINUE_IF_NO_DOC) || $(MAKE) $$ball; \
	  dir=`basename $$ball .tar.gz`; \
	  $(mkinstalldirs) $(DESTDIR)$(htmldir)/$$dir; \
	  gunzip -c `if test -f $$ball; then echo .; else echo $(srcdir); fi`/$$ball | tar xf - ; \
	  if test -d $$dir/ocamldoc; then \
	    $(mkinstalldirs) $(DESTDIR)$(htmldir)/$$dir/ocamldoc; \
	    for file in $$dir/ocamldoc/*; do \
	      $(INSTALL_DATA) $$file \
	        $(DESTDIR)$(htmldir)/$$dir/ocamldoc/`basename $$file`; \
	    done; \
	  fi; \
	  for file in $$dir/*; do \
	    $(INSTALL_DATA) $$file $(DESTDIR)$(htmldir)/$$dir/`basename $$file`; \
	  done; \
	done

installdirs-html:
	for ball in $(INSTALL_HTML_DOCS); do \
	  dir=`basename $$ball .tar.gz`; \
	  $(mkinstalldirs) $(DESTDIR)$(htmldir)/$$dir; \
	done

uninstall-html:
	for ball in $(INSTALL_HTML_DOCS); do \
	  dir=`basename $$ball .tar.gz`; \
	  rm -rf $(DESTDIR)$(htmldir)/$$dir; \
	done

# Installation and uninstallation of PDF documentation.
install-pdf:
	$(mkinstalldirs) $(DESTDIR)$(pdfdir)
	for file in $(INSTALL_PDF_DOCS); do \
	  test -f $$file || test -f $(srcdir)/$$file \
	    || $(CONTINUE_IF_NO_DOC) || $(MAKE) $$file; \
	  $(INSTALL_DATA) `if test -f $$file; then echo .; else echo $(srcdir); fi`/$$file $(DESTDIR)$(pdfdir)/$$file; \
	done

installdirs-pdf:
	$(mkinstalldirs) $(DESTDIR)$(pdfdir)

uninstall-pdf:
	for file in $(INSTALL_PDF_DOCS); do \
	  rm -f $(DESTDIR)$(pdfdir)/$$file; \
	done

# Installation and uninstallation of PostScript documentation.
# The PostScript documentation is obtained from the PDF one;
# the dependency is needed so as to make sure the PDF documentation
# is fully built before the conversion is performed.
install-ps: install-pdf
	$(mkinstalldirs) $(DESTDIR)$(psdir)
	for file in $(INSTALL_PS_DOCS); do \
	  test -f $$file || test -f $(srcdir)/$$file \
	    || $(CONTINUE_IF_NO_DOC) || $(MAKE) $$file; \
	  $(INSTALL_DATA) `if test -f $$file; then echo .; else echo $(srcdir); fi`/$$file $(DESTDIR)$(psdir)/$$file; \
	done

installdirs-ps:
	$(mkinstalldirs) $(DESTDIR)$(psdir)

uninstall-ps:
	for file in $(INSTALL_PS_DOCS); do \
	  rm -f $(DESTDIR)$(psdir)/$$file; \
	done

# Installation and uninstallation of pure text documentation.
install-txt:
	$(mkinstalldirs) $(DESTDIR)$(txtdir)
	for file in $(srcdir)/*.txt $(srcdir)/README.doc; do \
	  $(INSTALL_DATA) $$file $(DESTDIR)$(txtdir)/`basename $$file`; \
	done
	for file in BUGS COPYING CREDITS ChangeLog NEWS README README.configure TODO ; do \
	  $(INSTALL_DATA) $(srcdir)/../$$file $(DESTDIR)$(txtdir)/$$file; \
	done

installdirs-txt:
	$(mkinstalldirs) $(DESTDIR)$(txtdir)

uninstall-txt:
	rm -f $(DESTDIR)$(txtdir)/*.txt $(DESTDIR)$(txtdir)/README.doc
	for file in BUGS COPYING CREDITS ChangeLog NEWS README README.configure TODO ; do \
	  rm -f $(DESTDIR)$(txtdir)/$$file; \
	done

PPL_SOURCE_FILES = $(wildcard $(srcdir)/../src/*.hh $(srcdir)/../src/*.cc)

LATEX_DIRS = \
user.latex-dir \
devref.latex-dir \
user-c-interface.latex-dir \
devref-c-interface.latex-dir \
user-prolog-interface.latex-dir \
devref-prolog-interface.latex-dir \
user-ocaml-interface.latex-dir \
devref-ocaml-interface.latex-dir \
user-java-interface.latex-dir \
devref-java-interface.latex-dir \
user-configured-c-interface.latex-dir \
devref-configured-c-interface.latex-dir \
user-configured-prolog-interface.latex-dir \
devref-configured-prolog-interface.latex-dir \
user-configured-ocaml-interface.latex-dir \
devref-configured-ocaml-interface.latex-dir \
user-configured-java-interface.latex-dir \
devref-configured-java-interface.latex-dir

TEX_ENV = TEXINPUTS=$(abs_srcdir):$(TEXINPUTS)

.SECONDARY: $(LATEX_DIRS)

user.latex-dir: user.doxyconf-latex $(USER_CORE_STUFF)
	rm -rf $@
	$(TEX_ENV) $(DOXYGEN) $<

devref.latex-dir: devref.doxyconf-latex $(DEVREF_CORE_STUFF)
	rm -rf $@
	$(TEX_ENV) $(DOXYGEN) $<

ppl-user-@VERSION@.pdf: user.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-@VERSION@.pdf: devref.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

$(USER_CORE_HTML_DIR): user.doxyconf-html $(USER_CORE_STUFF)
	rm -rf $@
	$(TEX_ENV) $(DOXYGEN) $<

$(DEVREF_CORE_HTML_DIR): devref.doxyconf-html $(DEVREF_CORE_STUFF)
	rm -rf $@
	$(TEX_ENV) $(DOXYGEN) $<

############################################################################

##### Configuration Independent Language Interface Targets #####

SED_HTML_OPTION = -f $(srcdir)/interfaces-html.sed
SED_LATEX_OPTION = -f $(srcdir)/interfaces-latex.sed

SED_USER_HTML_OPTION = $(SED_HTML_OPTION) \
-e 's|<PPL_SED_TAGFILES>|ppl-user-@VERSION@-html.tag=../ppl-user-@VERSION@-html|'

SED_DEVREF_HTML_OPTION = $(SED_HTML_OPTION) \
-e 's|<PPL_SED_TAGFILES>|ppl-devref-@VERSION@-html.tag=../ppl-devref-@VERSION@-html|'

SED_USER_LATEX_OPTION = \
$(SED_LATEX_OPTION) \
-e 's/<PPL_SED_TAGFILES>//'

SED_DEVREF_LATEX_OPTION = \
$(SED_LATEX_OPTION) \
-e 's/<PPL_SED_TAGFILES>//'

# C Language Interface Targets

SED_C_OPTION = \
  -e 's/<PPL_SED_CONFIGURED_MANUAL>//' \
  -e 's/<PPL_SED_LANGUAGE_NAME>/C/' \
  -e 's/<PPL_SED_INTERFACE_NAME>/c-interface/' \
  -e 's/<PPL_SED_USEPACKAGE_OCAMLDOC>//'

SED_C_INPUT_USER = \
$(DOX_LICENSE_INPUT) \
../interfaces/C/ppl_c_version.h \
$(srcdir)/../interfaces/C/ppl_c_header.h \
$(srcdir)/../interfaces/C/C_interface.dox

SED_C_INPUT_DEVREF = \
$(DOX_LICENSE_INPUT) \
../interfaces/C/ppl_c_version.h \
$(srcdir)/../interfaces/C/ppl_c_header.h \
$(srcdir)/../interfaces/C/C_interface.dox \
$(srcdir)/../interfaces/C/ppl_c_implementation_common.cc \
$(srcdir)/../interfaces/C/ppl_c_implementation_common.defs.hh \
$(srcdir)/../interfaces/C/ppl_c_implementation_common.inlines.hh

user-c-interface.latex-dir: \
user-language-interface.doxyconf \
$(USER_C_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_C_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-c-interface.tex
	$(SED) $(SED_C_OPTION) $(SED_USER_LATEX_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_C_INPUT_USER)"'|' \
	    $<  > Doxyfile.user-c-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-c-interface-latex

devref-c-interface.latex-dir: \
devref-language-interface.doxyconf \
$(DEVREF_C_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_C_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-c-interface.tex
	$(SED) $(SED_C_OPTION) $(SED_DEVREF_LATEX_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_C_INPUT_DEVREF)"'|' \
	    $<  > Doxyfile.devref-c-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-c-interface-latex

ppl-user-c-interface-@VERSION@.pdf: user-c-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-c-interface-@VERSION@.pdf: devref-c-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-user-c-interface-@VERSION@-html: \
user-language-interface.doxyconf \
$(USER_C_INTERFACE_STUFF) $(USER_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_C_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-c-interface.tex
	$(SED) $(SED_C_OPTION) $(SED_USER_HTML_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_C_INPUT_USER)"'|' \
	    $<  > Doxyfile.user-c-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-c-interface-html

ppl-devref-c-interface-@VERSION@-html: \
devref-language-interface.doxyconf \
$(DEVREF_C_INTERFACE_STUFF) $(DEVREF_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_C_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-c-interface.tex
	$(SED) $(SED_C_OPTION) $(SED_DEVREF_HTML_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_C_INPUT_DEVREF)"'|' \
	    $<  > Doxyfile.devref-c-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-c-interface-html

# (System Independent) Prolog Language Interface Targets

SED_PROLOG_OPTION = \
  -e 's/<PPL_SED_CONFIGURED_MANUAL>//' \
  -e 's/<PPL_SED_LANGUAGE_NAME>/Prolog/' \
  -e 's/<PPL_SED_INTERFACE_NAME>/prolog-interface/' \
  -e 's/<PPL_SED_USEPACKAGE_OCAMLDOC>//'

SED_PROLOG_INPUT_USER = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/Prolog/Prolog_interface.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysindep.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysindep_domains.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_compilation.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysdep.dox

SED_PROLOG_INPUT_DEVREF = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/Prolog/Prolog_interface.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysindep.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysindep_domains.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_compilation.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysdep.dox \
$(srcdir)/../interfaces/Prolog/ppl_prolog_common.defs.hh \
$(srcdir)/../interfaces/Prolog/ppl_prolog_common.inlines.hh \
$(srcdir)/../interfaces/Prolog/ppl_prolog_common.cc

user-prolog-interface.latex-dir: \
user-language-interface.doxyconf \
$(USER_PROLOG_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_PROLOG_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-prolog-interface.tex
	$(SED) $(SED_PROLOG_OPTION) $(SED_USER_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_PROLOG_INPUT_USER)"'|' \
	  $<  > Doxyfile.user-prolog-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-prolog-interface-latex

devref-prolog-interface.latex-dir: \
devref-language-interface.doxyconf \
$(DEVREF_PROLOG_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_PROLOG_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-prolog-interface.tex
	$(SED) $(SED_PROLOG_OPTION) $(SED_DEVREF_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_PROLOG_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-prolog-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-prolog-interface-latex

ppl-user-prolog-interface-@VERSION@.pdf: user-prolog-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-prolog-interface-@VERSION@.pdf: devref-prolog-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-user-prolog-interface-@VERSION@-html: \
user-language-interface.doxyconf \
$(USER_PROLOG_INTERFACE_STUFF) $(USER_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_PROLOG_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-prolog-interface.tex
	$(SED) $(SED_PROLOG_OPTION) $(SED_USER_HTML_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_PROLOG_INPUT_USER)"'|' \
	  $<  > Doxyfile.user-prolog-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-prolog-interface-html

ppl-devref-prolog-interface-@VERSION@-html: \
devref-language-interface.doxyconf \
$(DEVREF_PROLOG_INTERFACE_STUFF) $(DEVREF_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_PROLOG_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-prolog-interface.tex
	$(SED) $(SED_PROLOG_OPTION) $(SED_DEVREF_HTML_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_PROLOG_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-prolog-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-prolog-interface-html

# OCaml Language Interface Targets

SED_OCAML_OPTION = \
  -e 's/<PPL_SED_CONFIGURED_MANUAL>//' \
  -e 's/<PPL_SED_LANGUAGE_NAME>/OCaml/' \
  -e 's/<PPL_SED_INTERFACE_NAME>/ocaml-interface/' \
  -e 's/<PPL_SED_USEPACKAGE_OCAMLDOC>/\\usepackage{ocamldoc}/'

SED_OCAML_INPUT_USER = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/OCaml/OCaml_interface.dox

SED_OCAML_INPUT_DEVREF = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/OCaml/OCaml_interface.dox \
$(srcdir)/../interfaces/OCaml/ppl_ocaml_common.cc \
$(srcdir)/../interfaces/OCaml/ppl_ocaml_common.defs.hh \
$(srcdir)/../interfaces/OCaml/ppl_ocaml_common.inlines.hh

OCAMLDOC_HTML_OPTIONS = \
-I @mlgmp_dir@ -I $(top_builddir)/interfaces/OCaml -html

OCAMLDOC_LATEX_OPTIONS = \
-I @mlgmp_dir@ -I $(top_builddir)/interfaces/OCaml \
-latex -noheader -notrailer -notoc

user-ocaml-interface.latex-dir: \
user-language-interface.doxyconf \
$(USER_OCAML_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_OCAML_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-ocaml-interface.tex
	$(SED) $(SED_OCAML_OPTION) $(SED_USER_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_OCAML_INPUT_USER)"'|' \
	  $<  > Doxyfile.user-ocaml-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-ocaml-interface-latex
	$(OCAMLDOC) $(OCAMLDOC_LATEX_OPTIONS) -o $@/ocamldoc.tex \
		$(srcdir)/../interfaces/OCaml/ppl_ocaml_globals.mli

devref-ocaml-interface.latex-dir: \
devref-language-interface.doxyconf \
$(DEVREF_OCAML_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_OCAML_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-ocaml-interface.tex
	$(SED) $(SED_OCAML_OPTION) $(SED_DEVREF_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_OCAML_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-ocaml-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-ocaml-interface-latex
	$(OCAMLDOC) $(OCAMLDOC_LATEX_OPTIONS) -o $@/ocamldoc.tex \
		$(srcdir)/../interfaces/OCaml/ppl_ocaml_globals.mli

ppl-user-ocaml-interface-@VERSION@.pdf: user-ocaml-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-ocaml-interface-@VERSION@.pdf: devref-ocaml-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-user-ocaml-interface-@VERSION@-html: \
user-language-interface.doxyconf \
$(USER_OCAML_INTERFACE_STUFF) $(USER_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_OCAML_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-ocaml-interface.tex
	$(SED) $(SED_OCAML_OPTION) $(SED_USER_HTML_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_OCAML_INPUT_USER)"'|' \
	    $<  > Doxyfile.user-ocaml-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-ocaml-interface-html
	mkdir $@/ocamldoc
	$(OCAMLDOC) $(OCAMLDOC_HTML_OPTIONS) -d $@/ocamldoc \
		$(srcdir)/../interfaces/OCaml/ppl_ocaml_globals.mli

ppl-devref-ocaml-interface-@VERSION@-html: \
devref-language-interface.doxyconf \
$(DEVREF_OCAML_INTERFACE_STUFF) $(DEVREF_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_OCAML_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-ocaml-interface.tex
	$(SED) $(SED_OCAML_OPTION) $(SED_DEVREF_HTML_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_OCAML_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-ocaml-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-ocaml-interface-html
	mkdir $@/ocamldoc
	$(OCAMLDOC) $(OCAMLDOC_HTML_OPTIONS) -d $@/ocamldoc \
		$(srcdir)/../interfaces/OCaml/ppl_ocaml_globals.mli

# Java Language Interface Targets

SED_JAVA_OPTION = \
  -e 's/<PPL_SED_CONFIGURED_MANUAL>//' \
  -e 's/<PPL_SED_LANGUAGE_NAME>/Java/' \
  -e 's/<PPL_SED_INTERFACE_NAME>/java-interface/' \
  -e 's/<PPL_SED_USEPACKAGE_OCAMLDOC>//'

SED_JAVA_INPUT_USER = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/Java/parma_polyhedra_library

SED_JAVA_INPUT_DEVREF = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/Java/parma_polyhedra_library \
$(srcdir)/../interfaces/Java/jni/ppl_java_common.cc \
$(srcdir)/../interfaces/Java/jni/ppl_java_common.defs.hh \
$(srcdir)/../interfaces/Java/jni/ppl_java_common.inlines.hh \
$(srcdir)/../interfaces/Java/jni/ppl_java_globals.cc

user-java-interface.latex-dir: \
user-language-interface.doxyconf \
$(USER_JAVA_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_JAVA_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-java-interface.tex
	$(SED) $(SED_JAVA_OPTION) $(SED_USER_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_JAVA_INPUT_USER)"'|' \
	  $<  > Doxyfile.user-java-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-java-interface-latex

devref-java-interface.latex-dir: \
devref-language-interface.doxyconf \
$(DEVREF_JAVA_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_JAVA_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-java-interface.tex
	$(SED) $(SED_JAVA_OPTION) $(SED_DEVREF_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_JAVA_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-java-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-java-interface-latex

ppl-user-java-interface-@VERSION@.pdf: user-java-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-java-interface-@VERSION@.pdf: devref-java-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-user-java-interface-@VERSION@-html: \
user-language-interface.doxyconf \
$(USER_JAVA_INTERFACE_STUFF) $(USER_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_JAVA_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-java-interface.tex
	$(SED) $(SED_JAVA_OPTION) $(SED_USER_HTML_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_JAVA_INPUT_USER)"'|' \
	  $<  > Doxyfile.user-java-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-java-interface-html

ppl-devref-java-interface-@VERSION@-html: \
devref-language-interface.doxyconf \
$(DEVREF_JAVA_INTERFACE_STUFF) $(DEVREF_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_JAVA_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-java-interface.tex
	$(SED) $(SED_JAVA_OPTION) $(SED_DEVREF_HTML_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_JAVA_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-java-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-java-interface-html

############################################################################

##### Configuration Dependent Language Interface Targets #####

# Configuration-Dependent C Language Interface Targets
if BUILD_C_INTERFACE

SED_CONFIGURED_C_OPTION = \
  -e 's/<PPL_SED_CONFIGURED_MANUAL>/PPL_DOXYGEN_CONFIGURED_MANUAL/' \
  -e 's/<PPL_SED_LANGUAGE_NAME>/Configured C/' \
  -e 's/<PPL_SED_INTERFACE_NAME>/configured-c-interface/' \
  -e 's/<PPL_SED_USEPACKAGE_OCAMLDOC>//'

SED_CONFIGURED_C_INPUT_USER = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/C/C_interface.dox \
../interfaces/C/ppl_c.h

SED_CONFIGURED_C_INPUT_DEVREF = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/C/C_interface.dox \
../interfaces/C/ppl_c.h \
$(srcdir)/../interfaces/C/ppl_c_implementation_common.cc \
$(srcdir)/../interfaces/C/ppl_c_implementation_common.defs.hh \
$(srcdir)/../interfaces/C/ppl_c_implementation_common.inlines.hh
# FIXME add generated files

user-configured-c-interface.latex-dir: \
user-language-interface.doxyconf \
$(USER_CONFIGURED_C_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_C_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-configured-c-interface.tex
	$(SED) $(SED_CONFIGURED_C_OPTION) $(SED_USER_LATEX_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_C_INPUT_USER)"'|' \
	    $<  > Doxyfile.user-configured-c-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-configured-c-interface-latex

devref-configured-c-interface.latex-dir: \
devref-language-interface.doxyconf \
$(DEVREF_CONFIGURED_C_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_C_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-configured-c-interface.tex
	$(SED) $(SED_CONFIGURED_C_OPTION) $(SED_DEVREF_LATEX_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_C_INPUT_DEVREF)"'|' \
	    $<  > Doxyfile.devref-configured-c-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-configured-c-interface-latex

ppl-user-configured-c-interface-@VERSION@.pdf: \
user-configured-c-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-configured-c-interface-@VERSION@.pdf: \
devref-configured-c-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-user-configured-c-interface-@VERSION@-html: \
user-language-interface.doxyconf \
$(USER_CONFIGURED_C_INTERFACE_STUFF) $(USER_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_C_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-configured-c-interface.tex
	$(SED) $(SED_CONFIGURED_C_OPTION) $(SED_USER_HTML_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_C_INPUT_USER)"'|' \
	    $<  > Doxyfile.user-configured-c-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-configured-c-interface-html

ppl-devref-configured-c-interface-@VERSION@-html: \
devref-language-interface.doxyconf \
$(DEVREF_CONFIGURED_C_INTERFACE_STUFF) $(DEVREF_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_C_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-configured-c-interface.tex
	$(SED) $(SED_CONFIGURED_C_OPTION) $(SED_DEVREF_HTML_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_C_INPUT_DEVREF)"'|' \
	    $<  > Doxyfile.devref-configured-c-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-configured-c-interface-html

endif BUILD_C_INTERFACE

# Configuration-Dependent Prolog Language Interface Targets
if BUILD_SOME_PROLOG_INTERFACES

SED_CONFIGURED_PROLOG_OPTION = \
  -e 's/<PPL_SED_CONFIGURED_MANUAL>/PPL_DOXYGEN_CONFIGURED_MANUAL/' \
  -e 's/<PPL_SED_LANGUAGE_NAME>/Configured Prolog/' \
  -e 's/<PPL_SED_INTERFACE_NAME>/configured-prolog-interface/' \
  -e 's/<PPL_SED_USEPACKAGE_OCAMLDOC>//'

SED_CONFIGURED_PROLOG_INPUT_USER = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/Prolog/Prolog_configured_interface.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysindep.dox \
../interfaces/Prolog/Prolog_configured_domains.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_compilation.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysdep.dox

SED_CONFIGURED_PROLOG_INPUT_DEVREF = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/Prolog/Prolog_configured_interface.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysindep.dox \
../interfaces/Prolog/Prolog_configured_domains.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_compilation.dox \
$(srcdir)/../interfaces/Prolog/Prolog_interface_sysdep.dox \
$(srcdir)/../interfaces/Prolog/ppl_prolog_common.defs.hh \
$(srcdir)/../interfaces/Prolog/ppl_prolog_common.inlines.hh \
$(srcdir)/../interfaces/Prolog/ppl_prolog_common.cc
# FIXME: add appropriate generated files.

user-configured-prolog-interface.latex-dir: \
user-language-interface.doxyconf \
$(USER_CONFIGURED_PROLOG_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_PROLOG_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-configured-prolog-interface.tex
	$(SED) $(SED_CONFIGURED_PROLOG_OPTION) $(SED_USER_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_PROLOG_INPUT_USER)"'|' \
	  $<  > Doxyfile.user-configured-prolog-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-configured-prolog-interface-latex

devref-configured-prolog-interface.latex-dir: \
devref-language-interface.doxyconf \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_PROLOG_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-configured-prolog-interface.tex
	$(SED) $(SED_CONFIGURED_PROLOG_OPTION) $(SED_DEVREF_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_PROLOG_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-configured-prolog-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-configured-prolog-interface-latex

ppl-user-configured-prolog-interface-@VERSION@.pdf: \
user-configured-prolog-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-configured-prolog-interface-@VERSION@.pdf: \
devref-configured-prolog-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-user-configured-prolog-interface-@VERSION@-html: \
user-language-interface.doxyconf \
$(USER_CONFIGURED_PROLOG_INTERFACE_STUFF) $(USER_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_PROLOG_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-configured-prolog-interface.tex
	$(SED) $(SED_CONFIGURED_PROLOG_OPTION) $(SED_USER_HTML_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_PROLOG_INPUT_USER)"'|' \
	  $<  > Doxyfile.user-configured-prolog-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-configured-prolog-interface-html

ppl-devref-configured-prolog-interface-@VERSION@-html: \
devref-language-interface.doxyconf \
$(DEVREF_CONFIGURED_PROLOG_INTERFACE_STUFF) $(DEVREF_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_PROLOG_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-configured-prolog-interface.tex
	$(SED) $(SED_CONFIGURED_PROLOG_OPTION) $(SED_DEVREF_HTML_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_PROLOG_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-configured-prolog-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-configured-prolog-interface-html

endif BUILD_SOME_PROLOG_INTERFACES

# Configuration-Dependent OCaml Language Interface Targets
if BUILD_OCAML_INTERFACE

SED_CONFIGURED_OCAML_OPTION = \
  -e 's/<PPL_SED_CONFIGURED_MANUAL>/PPL_DOXYGEN_CONFIGURED_MANUAL/' \
  -e 's/<PPL_SED_LANGUAGE_NAME>/Configured OCaml/' \
  -e 's/<PPL_SED_INTERFACE_NAME>/configured-ocaml-interface/' \
  -e 's/<PPL_SED_USEPACKAGE_OCAMLDOC>/\\usepackage{ocamldoc}/'

SED_CONFIGURED_OCAML_INPUT_USER = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/OCaml/OCaml_interface.dox

SED_CONFIGURED_OCAML_INPUT_DEVREF = \
$(DOX_LICENSE_INPUT) \
$(srcdir)/../interfaces/OCaml/OCaml_interface.dox \
$(srcdir)/../interfaces/OCaml/ppl_ocaml_common.cc \
$(srcdir)/../interfaces/OCaml/ppl_ocaml_common.defs.hh \
$(srcdir)/../interfaces/OCaml/ppl_ocaml_common.inlines.hh

user-configured-ocaml-interface.latex-dir: \
user-language-interface.doxyconf \
$(USER_CONFIGURED_OCAML_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_OCAML_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-configured-ocaml-interface.tex
	$(SED) $(SED_CONFIGURED_OCAML_OPTION) $(SED_USER_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_OCAML_INPUT_USER)"'|' \
	  $<  > Doxyfile.user-configured-ocaml-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-configured-ocaml-interface-latex
	$(OCAMLDOC) $(OCAMLDOC_LATEX_OPTIONS) -o $@/ocamldoc.tex \
		../interfaces/OCaml/ppl_ocaml.mli

devref-configured-ocaml-interface.latex-dir: \
devref-language-interface.doxyconf \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_OCAML_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-configured-ocaml-interface.tex
	$(SED) $(SED_CONFIGURED_OCAML_OPTION) $(SED_DEVREF_LATEX_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_OCAML_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-configured-ocaml-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-configured-ocaml-interface-latex
	$(OCAMLDOC) $(OCAMLDOC_LATEX_OPTIONS) -o $@/ocamldoc.tex \
		../interfaces/OCaml/ppl_ocaml.mli

ppl-user-configured-ocaml-interface-@VERSION@.pdf: \
user-configured-ocaml-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-configured-ocaml-interface-@VERSION@.pdf: \
devref-configured-ocaml-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-user-configured-ocaml-interface-@VERSION@-html: \
user-language-interface.doxyconf \
$(USER_CONFIGURED_OCAML_INTERFACE_STUFF) $(USER_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_OCAML_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-configured-ocaml-interface.tex
	$(SED) $(SED_CONFIGURED_OCAML_OPTION) $(SED_USER_HTML_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_OCAML_INPUT_USER)"'|' \
	    $<  > Doxyfile.user-configured-ocaml-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-configured-ocaml-interface-html
	mkdir $@/ocamldoc
	$(OCAMLDOC) $(OCAMLDOC_HTML_OPTIONS) -d $@/ocamldoc \
		../interfaces/OCaml/ppl_ocaml.mli

ppl-devref-configured-ocaml-interface-@VERSION@-html: \
devref-language-interface.doxyconf \
$(DEVREF_CONFIGURED_OCAML_INTERFACE_STUFF) $(DEVREF_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_OCAML_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-configured-ocaml-interface.tex
	$(SED) $(SED_CONFIGURED_OCAML_OPTION) $(SED_DEVREF_HTML_OPTION) \
	  -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_OCAML_INPUT_DEVREF)"'|' \
	  $<  > Doxyfile.devref-configured-ocaml-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-configured-ocaml-interface-html
	mkdir $@/ocamldoc
	$(OCAMLDOC) $(OCAMLDOC_HTML_OPTIONS) -d $@/ocamldoc \
		../interfaces/OCaml/ppl_ocaml.mli

endif BUILD_OCAML_INTERFACE

# Configuration-Dependent Java Language Interface Targets
if BUILD_JAVA_INTERFACE

SED_CONFIGURED_JAVA_OPTION = \
  -e 's/<PPL_SED_CONFIGURED_MANUAL>/PPL_DOXYGEN_CONFIGURED_MANUAL/' \
  -e 's/<PPL_SED_LANGUAGE_NAME>/Configured Java/' \
  -e 's/<PPL_SED_INTERFACE_NAME>/configured-java-interface/' \
  -e 's/<PPL_SED_USEPACKAGE_OCAMLDOC>//'

SED_CONFIGURED_JAVA_INPUT_USER = \
$(SED_JAVA_INPUT_USER) \
../interfaces/Java/parma_polyhedra_library

SED_CONFIGURED_JAVA_INPUT_DEVREF = \
$(SED_JAVA_INPUT_DEVREF) \
../interfaces/Java/parma_polyhedra_library
# FIXME add generated files

# NOTE: as an exception to the common policy,
# for the user-configured-java case we override the setting
# for Doxygen configuration variables HIDE_UNDOC_MEMBERS (set to NO)
# and EXCLUDE (set to Fake_Class_for_Doxygen.java)

user-configured-java-interface.latex-dir: \
user-language-interface.doxyconf \
$(USER_CONFIGURED_JAVA_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_JAVA_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-configured-java-interface.tex
	$(SED) $(SED_CONFIGURED_JAVA_OPTION) $(SED_USER_LATEX_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_JAVA_INPUT_USER)"'|' \
	    $<  > Doxyfile.user-configured-java-interface-latex
	echo "HIDE_UNDOC_MEMBERS = NO" \
	    >> Doxyfile.user-configured-java-interface-latex
	echo "EXCLUDE_PATTERNS = */Fake_Class_for_Doxygen*" \
	    >> Doxyfile.user-configured-java-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-configured-java-interface-latex

devref-configured-java-interface.latex-dir: \
devref-language-interface.doxyconf \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_STUFF)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_JAVA_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-configured-java-interface.tex
	$(SED) $(SED_CONFIGURED_JAVA_OPTION) $(SED_DEVREF_LATEX_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_JAVA_INPUT_DEVREF)"'|' \
	    $<  > Doxyfile.devref-configured-java-interface-latex
	echo "EXCLUDE_PATTERNS = */Fake_Class_for_Doxygen*" \
	    >> Doxyfile.devref-configured-java-interface-latex
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-configured-java-interface-latex

ppl-user-configured-java-interface-@VERSION@.pdf: \
user-configured-java-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-devref-configured-java-interface-@VERSION@.pdf: \
devref-configured-java-interface.latex-dir
	$(TEX_ENV) make -C $< refman.pdf && mv -f $</refman.pdf $@

ppl-user-configured-java-interface-@VERSION@-html: \
user-language-interface.doxyconf \
$(USER_CONFIGURED_JAVA_INTERFACE_STUFF) $(USER_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_JAVA_OPTION) \
	    $(srcdir)/user-language-interface.tex \
	    > user-configured-java-interface.tex
	$(SED) $(SED_CONFIGURED_JAVA_OPTION) $(SED_USER_HTML_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_JAVA_INPUT_USER)"'|' \
	    $<  > Doxyfile.user-configured-java-interface-html
	echo "HIDE_UNDOC_MEMBERS = NO" \
	    >> Doxyfile.user-configured-java-interface-html
	echo "EXCLUDE_PATTERNS = */Fake_Class_for_Doxygen*" \
	    >> Doxyfile.user-configured-java-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.user-configured-java-interface-html

ppl-devref-configured-java-interface-@VERSION@-html: \
devref-language-interface.doxyconf \
$(DEVREF_CONFIGURED_JAVA_INTERFACE_STUFF) $(DEVREF_CORE_HTML_DIR)
	rm -rf $@
	$(SED) $(SED_CONFIGURED_JAVA_OPTION) \
	    $(srcdir)/devref-language-interface.tex \
	    > devref-configured-java-interface.tex
	$(SED) $(SED_CONFIGURED_JAVA_OPTION) $(SED_DEVREF_HTML_OPTION) \
	    -e 's|<PPL_SED_INPUT>|'"$(SED_CONFIGURED_JAVA_INPUT_DEVREF)"'|' \
	    $<  > Doxyfile.devref-configured-java-interface-html
	echo "EXCLUDE_PATTERNS = */Fake_Class_for_Doxygen*" \
	    >> Doxyfile.devref-configured-java-interface-html
	$(TEX_ENV) $(DOXYGEN) Doxyfile.devref-configured-java-interface-html

endif BUILD_JAVA_INTERFACE

############################################################################

# Ad-hoc rules for distributed HTML manuals.
# These follow the guidelines of Section 27.5 of the Automake manual
# (distributed files should never depend upon non-distributed built files).

$(USER_CORE_HTML_MANUAL): user.doxyconf-html $(USER_CORE_STUFF)
	$(MAKE) $(AM_MAKEFLAGS) $(USER_CORE_HTML_DIR)
	tar c $(USER_CORE_HTML_DIR) | gzip --best --force > $@

$(USER_C_INTERFACE_HTML_MANUAL): $(USER_CORE_HTML_MANUAL) \
user-language-interface.doxyconf $(USER_C_INTERFACE_STUFF)
	$(MAKE) $(AM_MAKEFLAGS) $(USER_C_INTERFACE_HTML_DIR)
	tar c $(USER_C_INTERFACE_HTML_DIR) | gzip --best --force > $@

$(USER_PROLOG_INTERFACE_HTML_MANUAL): $(USER_CORE_HTML_MANUAL) \
user-language-interface.doxyconf $(USER_PROLOG_INTERFACE_STUFF)
	$(MAKE) $(AM_MAKEFLAGS) $(USER_PROLOG_INTERFACE_HTML_DIR)
	tar c $(USER_PROLOG_INTERFACE_HTML_DIR) | gzip --best --force > $@

$(USER_OCAML_INTERFACE_HTML_MANUAL): $(USER_CORE_HTML_MANUAL) \
user-language-interface.doxyconf $(USER_OCAML_INTERFACE_STUFF)
	$(MAKE) $(AM_MAKEFLAGS) $(USER_OCAML_INTERFACE_HTML_DIR)
	tar c $(USER_OCAML_INTERFACE_HTML_DIR) | gzip --best --force > $@

$(USER_JAVA_INTERFACE_HTML_MANUAL): $(USER_CORE_HTML_MANUAL) \
user-language-interface.doxyconf $(USER_JAVA_INTERFACE_STUFF)
	$(MAKE) $(AM_MAKEFLAGS) $(USER_JAVA_INTERFACE_HTML_DIR)
	tar c $(USER_JAVA_INTERFACE_HTML_DIR) | gzip --best --force > $@

############################################################################

%.tar: %
	tar cf $@ $<

%.gz: %
	gzip --best --force $<

%.pdf: %.tex
	$(TEX_ENV) pdflatex $<

%.ps:	%.pdf
	pdftops $< $@

EXTRA_DIST= \
$(BIBLIOGRAPHIES) \
$(DIST_DOCS) \
$(DOX_FILES) \
README.doc \
bugseng_logo.pdf \
devref-language-interface.tex \
devref.tex \
fdl.tex \
gpl.tex \
interfaces-html.sed \
interfaces-latex.sed \
ppl.sty \
ppl_logo.pdf \
ppl_logo_no_ppl.pdf \
user-language-interface.tex \
user.tex

GENERATED_LATEX_HEADER_FILES = \
devref-configured-c-interface.tex \
devref-configured-java-interface.tex \
devref-configured-ocaml-interface.tex \
devref-configured-prolog-interface.tex \
devref-c-interface.tex \
devref-java-interface.tex \
devref-ocaml-interface.tex \
devref-prolog-interface.tex \
user-configured-c-interface.tex \
user-configured-java-interface.tex \
user-configured-ocaml-interface.tex \
user-configured-prolog-interface.tex \
user-c-interface.tex \
user-java-interface.tex \
user-ocaml-interface.tex \
user-prolog-interface.tex

mostlyclean-local:
	rm -rf $(HTML_DIRS) $(LATEX_DIRS) $(NONDIST_MANUALS) *.aux *.log
	rm -rf $(GENERATED_LATEX_HEADER_FILES) Doxyfile.*

CLEANFILES = \
$(USER_CORE_HTML_DIR).tag \
$(DEVREF_CORE_HTML_DIR).tag

../src/ppl_include_files.hh:
	$(MAKE) $(AM_MAKEFLAGS) -C ../src ppl_include_files.hh
