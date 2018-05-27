m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)
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

dnl
dnl ==================================================================
dnl Common files are included here
dnl ==================================================================
dnl
m4_include(`ppl_interface_generator_common.m4')
m4_include(`ppl_interface_generator_common_dat.m4')
m4_include(`ppl_interface_generator_ocaml_procedure_generators.m4')
m4_include(`ppl_interface_generator_ocaml_test_ml_code.m4')
dnl
dnl This file generates ppl_ocaml_test.ml.

dnl ==================================================================
dnl Print name of domain being tested
dnl ==================================================================

m4_pushdef(`m4_pre_extra_class_code', `dnl
m4_define(`m4_current_interface', m4_interface_class`'$1)`'dnl
print_string_if_noisy "\n";;
print_string_if_noisy "\n";;
print_string_if_noisy "m4_current_interface";;
print_string_if_noisy "\n";;
m4_undefine(`m4_current_interface')`'dnl
m4_replace_all_patterns($1, m4_add_init_class_code($1))`'dnl
')

dnl ==================================================================
dnl Test all methods
dnl ==================================================================

m4_divert`'dnl
m4_include(`ppl_ocaml_tests_common')
m4_all_code`'dnl

print_string_if_noisy "\n";;
at_exit Gc.full_major;;
print_string_if_noisy "Bye!\n"
