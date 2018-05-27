m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file ppl_xsb.cc.
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
/* XSB Prolog interface.
m4_include(`ppl_interface_generator_copyright')dnl
*/

#include "../ppl_prolog_domains.hh"

m4_divert(1)dnl

#define XSB_ENTRY_0(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  return xsb_stub_##name(); \
}

#define XSB_ENTRY_1(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  Prolog_term_ref arg1 = reg_term(1); \
  return xsb_stub_##name(arg1); \
}

#define XSB_ENTRY_2(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  Prolog_term_ref arg1 = reg_term(1); \
  Prolog_term_ref arg2 = reg_term(2); \
  return xsb_stub_##name(arg1, arg2); \
}

#define XSB_ENTRY_3(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  Prolog_term_ref arg1 = reg_term(1); \
  Prolog_term_ref arg2 = reg_term(2); \
  Prolog_term_ref arg3 = reg_term(3); \
  return xsb_stub_##name(arg1, arg2, arg3); \
}

#define XSB_ENTRY_4(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  Prolog_term_ref arg1 = reg_term(1); \
  Prolog_term_ref arg2 = reg_term(2); \
  Prolog_term_ref arg3 = reg_term(3); \
  Prolog_term_ref arg4 = reg_term(4); \
  return xsb_stub_##name(arg1, arg2, arg3, arg4); \
}

#define XSB_ENTRY_5(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  Prolog_term_ref arg1 = reg_term(1); \
  Prolog_term_ref arg2 = reg_term(2); \
  Prolog_term_ref arg3 = reg_term(3); \
  Prolog_term_ref arg4 = reg_term(4); \
  Prolog_term_ref arg5 = reg_term(5); \
  return xsb_stub_##name(arg1, arg2, arg3, arg4, arg5); \
}

#define XSB_ENTRY_6(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  Prolog_term_ref arg1 = reg_term(1); \
  Prolog_term_ref arg2 = reg_term(2); \
  Prolog_term_ref arg3 = reg_term(3); \
  Prolog_term_ref arg4 = reg_term(4); \
  Prolog_term_ref arg5 = reg_term(5); \
  Prolog_term_ref arg6 = reg_term(6); \
  return xsb_stub_##name(arg1, arg2, arg3, arg4, arg5, arg6); \
}

#define XSB_ENTRY_7(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  Prolog_term_ref arg1 = reg_term(1); \
  Prolog_term_ref arg2 = reg_term(2); \
  Prolog_term_ref arg3 = reg_term(3); \
  Prolog_term_ref arg4 = reg_term(4); \
  Prolog_term_ref arg5 = reg_term(5); \
  Prolog_term_ref arg6 = reg_term(6); \
  Prolog_term_ref arg7 = reg_term(7); \
  return xsb_stub_##name(arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
}

#define XSB_ENTRY_8(name) \
extern "C" Prolog_foreign_return_type \
name() { \
  Prolog_term_ref arg1 = reg_term(1); \
  Prolog_term_ref arg2 = reg_term(2); \
  Prolog_term_ref arg3 = reg_term(3); \
  Prolog_term_ref arg4 = reg_term(4); \
  Prolog_term_ref arg5 = reg_term(5); \
  Prolog_term_ref arg6 = reg_term(6); \
  Prolog_term_ref arg7 = reg_term(7); \
  Prolog_term_ref arg8 = reg_term(8); \
  return xsb_stub_##name(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
}

m4_divert(2)dnl

extern "C" void
init() {
   ppl_initialize();
}
m4_divert`'dnl
m4_define(`m4_expanded_procedure_schema', `#undef $1
')dnl
ppl_prolog_sys_code`'dnl
m4_undivert(1)

m4_define(`m4_expanded_procedure_schema', `XSB_ENTRY_$2($1)
')

m4_divert`'dnl
ppl_prolog_sys_code`'dnl
dnl
dnl End of file generation.
