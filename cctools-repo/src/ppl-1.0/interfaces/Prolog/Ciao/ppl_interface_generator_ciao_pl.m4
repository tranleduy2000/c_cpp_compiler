m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file ppl_ciao.pl.

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
/* Ciao Prolog interface: Ciao Prolog part.
m4_include(`ppl_interface_generator_copyright')dnl
*/

:- module(ppl_ciao,
[
m4_divert(1)
],
[
        assertions,
        basicmodes,
        regtypes,
        foreign_interface
]).

m4_divert(2)dnl
:- extra_linker_opts('-L.libs').
:- use_foreign_library(ppl_ciao).

:- impl_defined(
[
m4_divert(3)
]).

:- comment(version_maintenance,off).

m4_divert`'dnl
m4_define(`m4_expanded_procedure_schema', `m4_ifelse($4, 0, , `COMMA
')	  $1/$2')dnl
m4_patsubst(ppl_prolog_sys_code, COMMA, `,')`'dnl
m4_undivert(1)`'dnl
m4_divert`'dnl
m4_define(`check_fail', `m4_ifelse(m4_index($1, nofail), -1, failok, nofail)')
m4_define(`term', `Term$1')dnl
m4_define(`anyterm', `any_term')dnl
m4_define(`interm', `in(Term$1)')dnl
m4_define(`m4_expanded_procedure_schema', `m4_ifelse(check_fail($3), nofail,
  `:- true pred $1`'m4_ifelse(`$2', 0, ,(`'m4_term_sequence($2, `interm'))
          :: `'m4_term_sequence($2, `anyterm', ` *')) +  foreign.

',
  `:- true pred $1_2(`'m4_term_sequence($2, `interm')`'m4_ifelse(`$2', 0, `go(Success)', `, go(Success)'))
          :: `'m4_term_sequence($2, `anyterm', ` *')`'m4_ifelse(`$2', 0, ` int', ` * int')
  + (returns(Success), foreign($1)).

$1`'m4_ifelse(`$2', 0, ,`(m4_term_sequence($2, `term'))') :-
   $1_2(`'m4_term_sequence($2, `term')`'m4_ifelse(`$2', 0, `1', `, 1')).

')')dnl
ppl_prolog_sys_code`'dnl
m4_undivert(2)dnl
m4_divert`'dnl
m4_define(`m4_expanded_procedure_schema',
  `m4_ifelse(check_fail($3), nofail, ,m4_ifelse($4, 0, , `COMMA
')	`'$1_2/m4_incr($2))')dnl
m4_patsubst(ppl_prolog_sys_code, COMMA, `,')`'dnl
dnl
dnl End of file generation.
