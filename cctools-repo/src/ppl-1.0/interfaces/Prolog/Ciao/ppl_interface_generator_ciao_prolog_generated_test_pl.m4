m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file defines macros needed for generating
dnl the Ciao dependent code for ciao_prolog_generated_test.pl.

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

m4_include(`ppl_interface_generator_prolog_systems.m4')dnl

m4_define(`m4_expanded_procedure_schema', `m4_ifelse($4, 0, , `COMMA
')  $1/$2')

m4_divert`'dnl
/* Ciao Prolog interface: Ciao Prolog part for checking all predicates.
m4_include(`../../../../ppl/interfaces/ppl_interface_generator_copyright')
*/

:- module(_, [main/0], []).
:- use_module(library(debugger)).
:- use_module(library(dynamic)).
:- use_module(library(lists)).
:- use_module(library(prolog_sys)).
:- use_module(library(read)).
:- use_module(library(write)).
:- use_module(ppl_ciao,
[
m4_patsubst(ppl_prolog_sys_code, COMMA, `,')`'dnl
]).

:- set_prolog_flag(multi_arity_warnings, off).

:-  `include'('ppl_prolog_generated_test_main.pl').

prolog_system('Ciao').

main:-
   (check_all ->
        write('OK')
   ;
        write('FAILURE')
   ),
   nl.
dnl
dnl End of file generation.
