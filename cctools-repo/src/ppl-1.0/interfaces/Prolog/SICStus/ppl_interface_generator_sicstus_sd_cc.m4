m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file ppl_sicstus_sd.cc.

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

dnl Include common macros for generating system dependent code.
m4_include(`ppl_interface_generator_prolog_systems.m4')dnl

m4_divert`'dnl
/* SICStus Prolog interface.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

#include "../ppl_prolog_domains.hh"

#define SP_STUB_0(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref /* goal */, void*) { \
  return name(); \
}

#define SP_STUB_1(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref goal, void*) { \
  Prolog_term_ref arg1 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(1, goal, arg1)) \
    return PROLOG_FAILURE; \
  return name(arg1); \
}

#define SP_STUB_2(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref goal, void*) { \
  Prolog_term_ref arg1 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(1, goal, arg1)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg2 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(2, goal, arg2)) \
    return PROLOG_FAILURE; \
  return name(arg1, arg2); \
}

#define SP_STUB_3(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref goal, void*) { \
  Prolog_term_ref arg1 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(1, goal, arg1)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg2 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(2, goal, arg2)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg3 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(3, goal, arg3)) \
    return PROLOG_FAILURE; \
  return name(arg1, arg2, arg3); \
}

#define SP_STUB_4(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref goal, void*) { \
  Prolog_term_ref arg1 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(1, goal, arg1)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg2 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(2, goal, arg2)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg3 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(3, goal, arg3)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg4 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(4, goal, arg4)) \
    return PROLOG_FAILURE; \
  return name(arg1, arg2, arg3, arg4); \
}

#define SP_STUB_5(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref goal, void*) { \
  Prolog_term_ref arg1 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(1, goal, arg1)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg2 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(2, goal, arg2)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg3 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(3, goal, arg3)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg4 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(4, goal, arg4)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg5 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(5, goal, arg5)) \
    return PROLOG_FAILURE; \
  return name(arg1, arg2, arg3, arg4, arg5); \
}

#define SP_STUB_6(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref goal, void*) { \
  Prolog_term_ref arg1 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(1, goal, arg1)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg2 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(2, goal, arg2)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg3 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(3, goal, arg3)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg4 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(4, goal, arg4)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg5 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(5, goal, arg5)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg6 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(6, goal, arg6)) \
    return PROLOG_FAILURE; \
  return name(arg1, arg2, arg3, arg4, arg5, arg6); \
}

#define SP_STUB_7(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref goal, void*) { \
  Prolog_term_ref arg1 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(1, goal, arg1)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg2 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(2, goal, arg2)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg3 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(3, goal, arg3)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg4 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(4, goal, arg4)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg5 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(5, goal, arg5)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg6 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(6, goal, arg6)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg7 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(7, goal, arg7)) \
    return PROLOG_FAILURE; \
  return name(arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
}

#define SP_STUB_8(name) \
extern "C" Prolog_foreign_return_type \
sp_stub_##name(Prolog_term_ref goal, void*) { \
  Prolog_term_ref arg1 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(1, goal, arg1)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg2 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(2, goal, arg2)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg3 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(3, goal, arg3)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg4 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(4, goal, arg4)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg5 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(5, goal, arg5)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg6 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(6, goal, arg6)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg7 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(7, goal, arg7)) \
    return PROLOG_FAILURE; \
  Prolog_term_ref arg8 = Prolog_new_term_ref(); \
  if (!Prolog_get_arg(8, goal, arg8)) \
    return PROLOG_FAILURE; \
  return name(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
}

m4_divert(1)
#define SP_DEFINE_C_PREDICATE(name, arity) \
  SP_define_c_predicate(#name, arity, "user", sp_stub_##name, NULL)

extern "C" void
ppl_sicstus_init(int /* when */) {
  ppl_initialize();
  for (size_t i = 0; prolog_interface_atoms[i].p_atom != 0; ++i) {
    if (SP_register_atom(*prolog_interface_atoms[i].p_atom) == 0) {
      Prolog_term_ref et = Prolog_new_term_ref();
      Prolog_put_atom_chars(et, "Cannot initialize the PPL interface");
      Prolog_raise_exception(et);
      return;
    }
  }
m4_divert(2)dnl
}

extern "C" void
ppl_sicstus_deinit(int /* when */) {
  for (size_t i = 0; prolog_interface_atoms[i].p_atom != 0; ++i)
    // SP_unregister_atom can fail.
    // We ignore such failures: what else can we do?
    (void) SP_unregister_atom(*prolog_interface_atoms[i].p_atom);
  ppl_finalize();
}
dnl
m4_divert`'dnl
dnl
dnl Redefine m4_expanded_procedure_schema to generate SICStus stubs.
dnl m4_expanded_procedure_schema(Predicate_Name, Arity)
m4_define(`m4_expanded_procedure_schema', `dnl
SP_STUB_$2($1)
')dnl
dnl Generate stubs.
ppl_prolog_sys_code`'dnl
m4_undivert(1)`'dnl
m4_divert(-1)

dnl Redefine m4_extension to generate SICStus user predicates.
dnl m4_extension(Predicate_Name, Arity)
m4_define(`m4_expanded_procedure_schema', `dnl
  SP_DEFINE_C_PREDICATE($1, $2);
')

dnl Generate user predicates.
m4_divert`'dnl
ppl_prolog_sys_code`'dnl
dnl
dnl End of file generation.

