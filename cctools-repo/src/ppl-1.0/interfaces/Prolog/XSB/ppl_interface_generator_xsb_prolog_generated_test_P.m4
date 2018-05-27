m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file defines macros needed for generating
dnl the XSB dependent code for xsb_prolog_generated_test.pl.

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

m4_include(`ppl_interface_generator_common_dat.m4')
m4_include(`ppl_interface_generator_prolog_systems.m4')

m4_divert`'dnl
/* XSB Prolog interface: XSB Prolog part for checking all predicates.
m4_include(`ppl_interface_generator_copyright')
*/

:- compiler_options([xpp_on]).
:- import xpp_include_dir/1 from parse.
:- assert(xpp_include_dir('.')).

#include "ppl_prolog_generated_test_main.pl"
#include "ppl_prolog_generated_test_common.pl"
m4_divert(-1)

m4_pushdef(`m4_one_class_code', `dnl
m4_replace_all_patterns($1,
  `#includeSPACE"../tests/ppl_prolog_generated_test_'`'m4_interface_class$1`'`.pl"
')`'dnl
')`'dnl
dnl
dnl -----------------------------------------------------------------
dnl Generate #include declarations for all the classes.
dnl -----------------------------------------------------------------
dnl
m4_divert
m4_patsubst(m4_patsubst(m4_all_code, ` ', `'), SPACE, ` ')`'dnl
m4_popdef(`m4_one_class_code')`'dnl

:- import append/3, length/2, member/2 from basics.
:- import
m4_divert(1)
   from ppl_xsb.

:- [ppl_xsb].

prolog_system('XSB').

discontiguous(_).
include(_).

main :-
    (check_all ->
        write('OK')
    ;
        write('FAILURE')
    ),
    nl.

:- main.
m4_divert`'dnl
m4_define(`m4_expanded_procedure_schema', `m4_ifelse($4, 0, , `COMMA
')	  $1/$2')dnl
m4_patsubst(ppl_prolog_sys_code, COMMA, `,')`'dnl
m4_undivert(1)
dnl
dnl End of file generation.
