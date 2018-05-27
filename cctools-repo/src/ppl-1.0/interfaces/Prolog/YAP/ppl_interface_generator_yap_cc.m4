m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file ppl_yap.cc.
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
/* YAP Prolog interface.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

#include "../ppl_prolog_domains.hh"

#define YAP_STUB_0(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  return name(); \
}

#define YAP_STUB_1(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  Prolog_term_ref arg1 = YAP_ARG1; \
  return name(arg1); \
}

#define YAP_STUB_2(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  Prolog_term_ref arg1 = YAP_ARG1; \
  Prolog_term_ref arg2 = YAP_ARG2; \
  return name(arg1, arg2); \
}

#define YAP_STUB_3(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  Prolog_term_ref arg1 = YAP_ARG1; \
  Prolog_term_ref arg2 = YAP_ARG2; \
  Prolog_term_ref arg3 = YAP_ARG3; \
  return name(arg1, arg2, arg3); \
}

#define YAP_STUB_4(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  Prolog_term_ref arg1 = YAP_ARG1; \
  Prolog_term_ref arg2 = YAP_ARG2; \
  Prolog_term_ref arg3 = YAP_ARG3; \
  Prolog_term_ref arg4 = YAP_ARG4; \
  return name(arg1, arg2, arg3, arg4); \
}

#define YAP_STUB_5(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  Prolog_term_ref arg1 = YAP_ARG1; \
  Prolog_term_ref arg2 = YAP_ARG2; \
  Prolog_term_ref arg3 = YAP_ARG3; \
  Prolog_term_ref arg4 = YAP_ARG4; \
  Prolog_term_ref arg5 = YAP_ARG5; \
  return name(arg1, arg2, arg3, arg4, arg5); \
}

#define YAP_STUB_6(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  Prolog_term_ref arg1 = YAP_ARG1; \
  Prolog_term_ref arg2 = YAP_ARG2; \
  Prolog_term_ref arg3 = YAP_ARG3; \
  Prolog_term_ref arg4 = YAP_ARG4; \
  Prolog_term_ref arg5 = YAP_ARG5; \
  Prolog_term_ref arg6 = YAP_ARG6; \
  return name(arg1, arg2, arg3, arg4, arg5, arg6); \
}

#define YAP_STUB_7(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  Prolog_term_ref arg1 = YAP_ARG1; \
  Prolog_term_ref arg2 = YAP_ARG2; \
  Prolog_term_ref arg3 = YAP_ARG3; \
  Prolog_term_ref arg4 = YAP_ARG4; \
  Prolog_term_ref arg5 = YAP_ARG5; \
  Prolog_term_ref arg6 = YAP_ARG6; \
  Prolog_term_ref arg7 = YAP_ARG7; \
  return name(arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
}

#define YAP_STUB_8(name) \
extern "C" Prolog_foreign_return_type \
yap_stub_##name() { \
  Prolog_term_ref arg1 = YAP_ARG1; \
  Prolog_term_ref arg2 = YAP_ARG2; \
  Prolog_term_ref arg3 = YAP_ARG3; \
  Prolog_term_ref arg4 = YAP_ARG4; \
  Prolog_term_ref arg5 = YAP_ARG5; \
  Prolog_term_ref arg6 = YAP_ARG6; \
  Prolog_term_ref arg7 = YAP_ARG7; \
  Prolog_term_ref arg8 = YAP_ARG8; \
  return name(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
}

dnl
dnl Place here YAP_STUB macros.
dnl
m4_divert(1)dnl

#define YAP_USER_C_PREDICATE(name, arity) \
 YAP_UserCPredicate(#name, reinterpret_cast<int(*)()>(yap_stub_##name), arity)

extern "C" void
init() {
  ppl_initialize();

dnl
dnl Place here YAP_USER_C_PREDICATE macros.
dnl
m4_divert(2)dnl
}
dnl
m4_divert`'dnl
dnl
dnl Redefine m4_expanded_procedure_schema to generate YAP stubs.
dnl m4_expanded_procedure_schema(Predicate_Name, Arity)
m4_define(`m4_expanded_procedure_schema', `dnl
YAP_STUB_$2($1)
')`'dnl
dnl Generate stubs.
ppl_prolog_sys_code`'dnl
m4_undivert(1)

dnl Redefine m4_expanded_procedure_schema to generate YAP user predicates.
dnl m4_expanded_procedure_schema(Predicate_Name, Arity)
m4_define(`m4_expanded_procedure_schema', `dnl
  YAP_USER_C_PREDICATE($1, $2);
')

dnl Generate user predicates.
m4_divert`'dnl
ppl_prolog_sys_code`'dnl
dnl
dnl End of file generation.
