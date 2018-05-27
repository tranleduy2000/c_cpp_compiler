m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file ppl_swiprolog.cc.
dnl Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
dnl Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
dnl
dnl This file is part of the Parma Polyhedra Library (PPL).
dnl
dnl The PPL is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by the
dnl Free Software Foundation; either version 3 of the License, or (at your
dnl option) any later version.
dnl
dnl The PPL is distributed in the hope that it will be useful, but WITHOUT
dnl ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
dnl FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
dnl for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software Foundation,
dnl Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
dnl
dnl For the most up-to-date information see the Parma Polyhedra Library
dnl site: http://bugseng.com/products/ppl/ .

m4_include(`ppl_interface_generator_prolog_systems.m4')

m4_divert`'dnl
/* SWI-Prolog interface.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

#include "../ppl_prolog_domains.hh"

#define PL_EXTENSION_ENTRY(name, arity) { #name, arity, (void*) name, 0 },

namespace {

PL_extension predicates[] = {
m4_divert(1)dnl
  { NULL, 0, NULL, 0 }
};

} // namespace

extern "C" install_t
install() {
  ppl_initialize();
  PL_register_extensions(predicates);
}

extern "C" install_t
uninstall() {
  ppl_finalize();
}
m4_divert(-1)
dnl Include common macros for generating system dependent code.
dnl
dnl Redefine m4_expanded_procedure_schema as useful for SWI-Prolog.
dnl m4_expanded_procedure_schema(Predicate_Name, Arity)
dnl Note: SPACES is just a marker to generated the two spaces of
dnl indentation following it.
m4_define(`m4_expanded_procedure_schema', `dnl
SPACES  PL_EXTENSION_ENTRY($1, $2)
')

dnl Now remove the marker SPACES.
m4_divert`'dnl
m4_patsubst(ppl_prolog_sys_code, SPACES, `')dnl
dnl
dnl End of file generation.
