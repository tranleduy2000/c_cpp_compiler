% Extra test code for the generated tests for the Prolog interfaces.
% Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
% Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
%
% This file is part of the Parma Polyhedra Library (PPL).
%
% The PPL is free software; you can redistribute it and/or modify it
% under the terms of the GNU General Public License as published by the
% Free Software Foundation; either version 3 of the License, or (at your
% option) any later version.
%
% The PPL is distributed in the hope that it will be useful, but WITHOUT
% ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
% FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
% for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program; if not, write to the Free Software Foundation,
% Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
%
% For the most up-to-date information see the Parma Polyhedra Library
% site: http://bugseng.com/products/ppl/ .

:- dynamic(noisy/1).

% check_noisy
% This also executes all the test predicates but also prints some messages
% including the banner, version numbers and expected output from
% the exception tests.

check_noisy :-
   make_noisy,
   check_all,
   make_quiet.

check_extra_noisy :-
   make_extra_noisy,
   check_all,
   make_quiet.

%%%%%%%%%%%% predicates for switching on/off output messages %

make_extra_noisy :-
  (retract(noisy(_)) ->
      make_extra_noisy
  ;
      assertz(noisy(2))
  ).

make_noisy :-
  (retract(noisy(_)) ->
      make_noisy
  ;
      assertz(noisy(1))
  ).

make_quiet :-
  (retract(noisy(_)) ->
      make_quiet
   ; assertz(noisy(0))
  ).

%%%%%%%%%%%% predicates for output messages %%%%%%%%%%%%%%%%%%%%%

error_message(Message):-
   write_all(Message),
   fail.

display_message(Message):-
    noisy(_),
    (noisy(0) -> true ;
     (write_all(Message))
    ).

write_all([]) :- nl.
write_all([Phrase|Phrases]):-
   (Phrase == nl ->
      nl
   ;
      (write(Phrase),
      write(' '))
   ),
   write_all(Phrases).

%%%%%%%%%%%% predicates for variable generation %%%%%%%%%%%%%%%%%%

% make_var_list(+I,+Dimension,?Variable_List)
% constructs a list of variables with indices from I to Dimension - 1.
% It is assumed that I =< Dimension.

make_vars(Dim, Var_List) :-
  make_var_list(0, Dim, Var_List).
make_var_list(Dim, Dim, []) :- !.
make_var_list(I, Dim, ['$VAR'(I)|Var_List]) :-
  I < Dim,
  (I1 is I + 1,
  make_var_list(I1, Dim, Var_List)).

make_map_vars(Dim, Var_Map_List) :-
  make_map_var_list(0, Dim, Var_Map_List).
make_map_var_list(Dim, Dim, []) :- !.
make_map_var_list(I, Dim, ['$VAR'(I)-'$VAR'(I)|Var_Map_List]) :-
  I < Dim,
  (I1 is I + 1,
  make_map_var_list(I1, Dim, Var_Map_List)).

%%%%%%%%%%%% predicates for success and errors.  %%%%%%%%%%%%%%%%%

write_success(Predicate_name) :-
  display_message([test, for, Predicate_name, 'succeeded.']).

write_error(Predicate_name) :-
  write_all([test, for, Predicate_name, 'failed.']).


%%%%%%%%%%%% test data selection       %%%%%%%%%%%%%%%%%%%%%%%%%%%%

all_tests(0, [test00, test02]).
all_tests(1, [test01, test03, test04, test05, test06, test07]).

choose_test(TEST_DATA, Dim) :-
   all_tests(_, Tests),
   member(TEST_DATA, Tests),
   ppl_dimension_test_data(TEST_DATA, space_dimension, Dim).

choose_2_tests(TEST_DATA1, TEST_DATA2, Dim) :-
   all_tests(0, Tests0),
   all_tests(1, Tests1),
   (
     (member(TEST_DATA1, Tests0),
      member(TEST_DATA2, Tests0)) ;
     (member(TEST_DATA1, Tests1),
      member(TEST_DATA2, Tests1))
   ),
   ppl_dimension_test_data(TEST_DATA1, space_dimension, Dim),
   ppl_dimension_test_data(TEST_DATA2, space_dimension, Dim).

:- discontiguous(ppl_build_test_data/4).
:- discontiguous(ppl_dimension_test_data/3).
:- discontiguous(ppl_relation_test_data/4).
:- discontiguous(ppl_property_test_data/4).
:- discontiguous(ppl_bounds_test_data/5).
:- discontiguous(ppl_maxmin_test_data/10).
:- discontiguous(ppl_frequency_test_data/9).
:- discontiguous(ppl_constrains_test_data/3).
:- dynamic(all_class_dependent_predicates/1).
:- discontiguous(all_class_dependent_predicates/1).

ppl_initial_test_system(constraint, universe).
ppl_initial_test_system(congruence, universe).
ppl_initial_test_system(generator, empty).
ppl_initial_test_system(grid_generator, empty).

predicate_exists(Predicate) :-
  all_class_dependent_predicates(List_of_Predicates),
  member(Predicate, List_of_Predicates).


%%%%%%%%%%%% predicates for test data %%%%%%%%%%%%%%%%%%%%%%%%%%%%

/* Test data for test test00 (an empty object in 0 dimensions) */

ppl_build_test_data(test00, _Topology, constraints, [0 = 1]).
ppl_build_test_data(test00, _Topology, generators, []).
ppl_build_test_data(test00, _Topology, congruences, [(0 =:= 1) / 0]).
ppl_build_test_data(test00, _Topology, grid_generators, []).

ppl_dimension_test_data(test00, _, 0).

ppl_relation_test_data(test00, constraint, 0 = 1, Rel) :-
  Rel = [saturates, is_included, is_disjoint]
  ; Rel = [saturates, is_disjoint, is_included]
  ; Rel = [is_included, is_disjoint, saturates]
  ; Rel = [is_included, saturates, is_disjoint]
  ; Rel = [is_disjoint, is_included, saturates]
  ; Rel = [is_disjoint, saturates, is_included]
  ; Rel = [is_disjoint, is_included]
  ; Rel = [is_included, is_disjoint].
ppl_relation_test_data(test00, generator, point(0), []).
ppl_relation_test_data(test00, congruence, (0 =:= 1) / 0, Rel) :-
  Rel = [saturates, is_included, is_disjoint]
  ; Rel = [saturates, is_disjoint, is_included]
  ; Rel = [is_included, is_disjoint, saturates]
  ; Rel = [is_included, saturates, is_disjoint]
  ; Rel = [is_disjoint, is_included, saturates]
  ; Rel = [is_disjoint, saturates, is_included]
  ; Rel = [is_disjoint, is_included]
  ; Rel = [is_included, is_disjoint].
ppl_relation_test_data(test00, grid_generator, grid_point(0), []).

ppl_property_test_data(test00, _, _, is_empty).
ppl_property_test_data(test00, _, _, is_bounded).
ppl_property_test_data(test00, _, _, is_topologically_closed).
ppl_property_test_data(test00, _, _, is_discrete).

ppl_bounds_test_data(test00, _, 0, _, true).

ppl_maxmin_test_data(test00, _Topology, _, _, 0, _, _, _, _, false).

ppl_frequency_test_data(test00, _Topology, _, 0, _, _, _, _, false).

/* Test data for test test01 (an empty object in 1 dimension) */

ppl_build_test_data(test01, _Topology, constraints, CS) :-
  (make_vars(1, [A]),
   CS = [A >= 1, A =< 0]).
ppl_build_test_data(test01, _Topology, generators, []).
ppl_build_test_data(test01, _Topology, congruences, CS) :-
  (make_vars(1, [A]),
   CS = [(A =:= 1) / 2, (A =:= 0) / 2]).
ppl_build_test_data(test01, _Topology, grid_generators, []).

ppl_dimension_test_data(test01, space_dimension, 1).
ppl_dimension_test_data(test01, affine_dimension, 0).

ppl_relation_test_data(test01, constraint, 0 = 1, Rel) :-
  Rel = [saturates, is_included, is_disjoint]
  ; Rel = [saturates, is_disjoint, is_included]
  ; Rel = [is_included, is_disjoint, saturates]
  ; Rel = [is_included, saturates, is_disjoint]
  ; Rel = [is_disjoint, is_included, saturates]
  ; Rel = [is_disjoint, saturates, is_included]
  ; Rel = [is_disjoint, is_included]
  ; Rel = [is_included, is_disjoint].
ppl_relation_test_data(test01, generator, point(0), []).
ppl_relation_test_data(test01, congruence, (0 =:= 1) / 0, Rel) :-
  Rel = [saturates, is_included, is_disjoint]
  ; Rel = [saturates, is_disjoint, is_included]
  ; Rel = [is_included, is_disjoint, saturates]
  ; Rel = [is_included, saturates, is_disjoint]
  ; Rel = [is_disjoint, is_included, saturates]
  ; Rel = [is_disjoint, saturates, is_included]
  ; Rel = [is_disjoint, is_included]
  ; Rel = [is_included, is_disjoint].
ppl_relation_test_data(test01, grid_generator, grid_point(0), []).

ppl_property_test_data(test01, _, _, is_empty).
ppl_property_test_data(test01, _, _, is_bounded).
ppl_property_test_data(test01, _, _, is_topologically_closed).
ppl_property_test_data(test01, _, _, is_discrete).

ppl_bounds_test_data(test01, _, A, _, true) :-
  make_vars(1, [A]).

ppl_maxmin_test_data(test01, _Topology, _, _, 0, _, _, _, _, false).

ppl_frequency_test_data(test01, _Topology, _, 0, _, _, _, _, false).

ppl_constrains_test_data(test01, _, true).

/* Test data for test test02 (a universe object in 0 dimensions) */

ppl_build_test_data(test02, _Topology, constraints, []).
ppl_build_test_data(test02, _Topology, generators, [point(0)]).
ppl_build_test_data(test02, _Topology, congruences, []).
ppl_build_test_data(test02, _Topology, grid_generators, [grid_point(0)]).

ppl_dimension_test_data(test02, _, 0).

ppl_relation_test_data(test02, constraint, 0 = 0, Rel) :-
  Rel = [saturates, is_included] ; Rel = [is_included, saturates]
  ; Rel = [is_included].
ppl_relation_test_data(test02, generator, point(0), [subsumes]).
ppl_relation_test_data(test02, congruence, (0 =:= 0) / 0, Rel) :-
  Rel = [saturates, is_included] ; Rel = [is_included, saturates]
  ; Rel = [is_included].
ppl_relation_test_data(test02, grid_generator, grid_point(0), [subsumes]).

ppl_property_test_data(test02, _, _, is_universe).
ppl_property_test_data(test02, _, _, is_bounded).
ppl_property_test_data(test02, _, _, is_topologically_closed).
ppl_property_test_data(test02, _, _, is_discrete).
ppl_property_test_data(test02, _, _, contains_integer_point).

ppl_bounds_test_data(test02, _, 0, _, true).

ppl_maxmin_test_data(test02, _Topology, constraint, _, 0, 0, 1, true,
                     point(0), true).
ppl_maxmin_test_data(test02, _Topology, congruence, _, 0, 0, 1, true,
                     grid_point(0), true).

ppl_frequency_test_data(test02, _Topology, _, 0, 0, 1, 0, 1, true).

/* Test data for test test03 (a universe object in 1 dimension) */

ppl_build_test_data(test03, _Topology, constraints, CS) :-
  (make_vars(1, [A]),
   CS = [A = A]).
ppl_build_test_data(test03, _Topology, generators, GS) :-
  (make_vars(1, [A]),
   GS = [point(0), line(A)]).
ppl_build_test_data(test03, _Topology, congruences, CS) :-
  (make_vars(1, [A]),
   CS = [(A =:= A) / 0]).
ppl_build_test_data(test03, _Topology, grid_generators, GS) :-
  (make_vars(1, [A]),
   GS = [grid_point(0), grid_line(A)]).

ppl_dimension_test_data(test03, _, 1).

ppl_relation_test_data(test03, constraint, A = 0, [strictly_intersects]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test03, generator, point(0), [subsumes]).
ppl_relation_test_data(test03, congruence, (A =:= 0) / 0, [strictly_intersects]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test03, grid_generator, grid_point(0), [subsumes]).

ppl_property_test_data(test03, _, _, is_universe).
ppl_property_test_data(test03, _, _, is_topologically_closed).
ppl_property_test_data(test03, _, _, contains_integer_point).

ppl_bounds_test_data(test03, _, A, _, false) :-
  make_vars(1, [A]).

ppl_maxmin_test_data(test03, _Topology, _, _, A, _, _, _, _, false) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test03, _Topology, _, 0, 0, 1, 0, 1, true).

ppl_constrains_test_data(test03, _, false).

/* Test data for test test04 and test05
   (an object in 1 dimension with a single point) */

ppl_build_test_data(test04, _Topology, constraints, CS) :-
  (make_vars(1, [A]),
   CS = [A = 1]).

ppl_build_test_data(test04, _Topology, generators, GS) :-
  (make_vars(1, [A]),
   GS = [point(1*A)]).
ppl_build_test_data(test04, _Topology, congruences, CS) :-
  (make_vars(1, [A]),
   CS = [(A =:= 1) / 0]).
ppl_build_test_data(test04, _Topology, grid_generators, GS) :-
  (make_vars(1, [A]),
   GS = [grid_point(1*A)]).

ppl_dimension_test_data(test04, space_dimension, 1).
ppl_dimension_test_data(test04, affine_dimension, 0).

ppl_relation_test_data(test04, constraint, A = 0, [is_disjoint]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test04, generator, point(0), []).
ppl_relation_test_data(test04, congruence, (A =:= 0) / 0, [is_disjoint]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test04, grid_generator, grid_point(0), []).

ppl_property_test_data(test04, _, _, is_bounded).
ppl_property_test_data(test04, _, _, is_topologically_closed).
ppl_property_test_data(test04, _, _, contains_integer_point).
ppl_property_test_data(test04, _, _, is_discrete).

ppl_bounds_test_data(test04, _, A, _, true) :-
  make_vars(1, [A]).

ppl_maxmin_test_data(test04, _Topology, constraint, _, A, 1, 1, true,
                     point(A), true) :-
   make_vars(1, [A]).
ppl_maxmin_test_data(test04, _Topology, congruence, _, A, 1, 1, true,
             grid_point(A), true) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test04, _Topology, _, A, 0, 1, 1, 1, true) :-
   make_vars(1, [A]).

ppl_constrains_test_data(test04, _, true).

ppl_build_test_data(test05, _Topology, constraints, CS) :-
  (make_vars(1, [A]),
   CS = [A = -1]).
ppl_build_test_data(test05, _Topology, generators, GS) :-
  (make_vars(1, [A]),
   GS = [point(-1*A)]).
ppl_build_test_data(test05, _Topology, congruences, CS) :-
  (make_vars(1, [A]),
   CS = [(A =:= -1) / 0]).
ppl_build_test_data(test05, _Topology, grid_generators, GS) :-
  (make_vars(1, [A]),
   GS = [grid_point(-1*A)]).

ppl_dimension_test_data(test05, space_dimension, 1).
ppl_dimension_test_data(test05, affine_dimension, 0).

ppl_relation_test_data(test05, constraint, A = 0, [is_disjoint]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test05, generator, point(0), []).
ppl_relation_test_data(test05, congruence, (A =:= 0) / 0, [is_disjoint]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test05, grid_generator, grid_point(0), []).

ppl_property_test_data(test05, _, _, is_bounded).
ppl_property_test_data(test05, _, _, is_topologically_closed).
ppl_property_test_data(test05, _, _, contains_integer_point).
ppl_property_test_data(test05, _, _, is_discrete).

ppl_bounds_test_data(test05, _, A, _, true) :-
  make_vars(1, [A]).

ppl_maxmin_test_data(test05, _Topology, constraint, _, A, -1, 1, true,
                     point(-1*A), true) :-
   make_vars(1, [A]).
ppl_maxmin_test_data(test05, _Topology, congruence, _, A, -1, 1, true,
             grid_point(-1*A), true) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test05, _Topology, _, A, 0, 1, -1, 1, true) :-
   make_vars(1, [A]).

ppl_constrains_test_data(test05, _, true).

/* Test data for test test06,
            a non-universe object in 1 dimension with no upper bound */

ppl_build_test_data(test06, T, constraints, CS) :-
  (\+ T == t_NNC_,
   make_vars(1, [A]),
   CS = [A >= 0]).
ppl_build_test_data(test06, T, generators, GS) :-
  (\+ T == t_NNC_,
   make_vars(1, [A]),
   GS = [point(0*A), ray(A)]).
ppl_build_test_data(test06, t_NNC_, constraints, CS) :-
  (make_vars(1, [A]),
   CS = [A > 0]).
ppl_build_test_data(test06, t_NNC_, generators, GS) :-
  (make_vars(1, [A]),
   GS = [point(A), closure_point(0*A), ray(A)]).
ppl_build_test_data(test06, _Topology, congruences, CS) :-
  (make_vars(1, [A]),
   CS = [A =:= 0]).
ppl_build_test_data(test06, _Topology, grid_generators, GS) :-
  (make_vars(1, [A]),
   GS = [grid_point(0*A), parameter(1*A)]).

ppl_dimension_test_data(test06, _, 1).

ppl_relation_test_data(test06, constraint, A =< 3, [strictly_intersects]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test06, generator, point(3*A), [subsumes]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test06, congruence, (2*A =:= 1) / 3,
                                              [strictly_intersects]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test06, grid_generator, grid_point(3*A), [subsumes]) :-
  make_vars(1, [A]).

ppl_property_test_data(test06, T, _, is_topologically_closed) :-
  \+ T == t_NNC_.
ppl_property_test_data(test06, _, _, contains_integer_point).
ppl_property_test_data(test06, _, congruence, is_discrete).

ppl_bounds_test_data(test06, _, A, above, false) :-
  make_vars(1, [A]).
ppl_bounds_test_data(test06, constraint, A, below, true) :-
  make_vars(1, [A]).
ppl_bounds_test_data(test06, congruence, A, below, false) :-
  make_vars(1, [A]).

ppl_maxmin_test_data(test06, T, constraint, maximize, A, _, _, _, _, false) :-
   (T = t_ ; T = t_C_), make_vars(1, [A]).

ppl_maxmin_test_data(test06, T, constraint, minimize, A, 0, 1, true,
                     point(0), true) :-
   (T = t_ ; T = t_C_), make_vars(1, [A]).
ppl_maxmin_test_data(test06, t_NNC_, constraint, maximize, A, _, _, _,
                     _, false) :-
   make_vars(1, [A]).
ppl_maxmin_test_data(test06, t_NNC_, constraint, minimize, A, 0, 1, false,
                     closure_point(0), true) :-
   make_vars(1, [A]).
ppl_maxmin_test_data(test06, _, congruence, _, A, _, _, _, _, false) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test06, _Topology, constraint, A, _, _, _, _, false) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test06, _Topology, congruence, A, 1, 1, 0, 1, true) :-
   make_vars(1, [A]).

ppl_constrains_test_data(test06, _, true).

/* Test data for test test07,
    a non-universe object in 1 dimension with no lower bound */

ppl_build_test_data(test07, T, constraints, CS) :-
  (\+ T == t_NNC_,
   make_vars(1, [A]),
   CS = [A =< 0]).
ppl_build_test_data(test07, T, generators, GS) :-
  (\+ T == t_NNC_,
   make_vars(1, [A]),
   GS = [point(0*A), ray(-1*A)]).
ppl_build_test_data(test07, t_NNC_, constraints, CS) :-
  (make_vars(1, [A]),
   CS = [A < 0]).
ppl_build_test_data(test07, t_NNC_, generators, GS) :-
  (make_vars(1, [A]),
   GS = [point(A), closure_point(0*A), ray(-1*A)]).
ppl_build_test_data(test07, _Topology, congruences, CS) :-
  (make_vars(1, [A]),
   CS = [(A =:= 0) / 2]).
ppl_build_test_data(test07, _Topology, grid_generators, GS) :-
  (make_vars(1, [A]),
   GS = [grid_point(0*A), parameter(-2*A)]).

ppl_dimension_test_data(test07, _, 1).

ppl_relation_test_data(test07, constraint, A >= -3, [strictly_intersects]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test07, generator, point(-4*A), [subsumes]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test07, congruence, (2*A =:= 1) / 3,
                                              [strictly_intersects]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test07, grid_generator, grid_point(6*A), [subsumes]) :-
  make_vars(1, [A]).

ppl_property_test_data(test07, T, _, is_topologically_closed) :-
  \+ T == t_NNC_.
ppl_property_test_data(test07, _, _, contains_integer_point).
ppl_property_test_data(test07, _, congruence, is_discrete).

ppl_bounds_test_data(test07, _, A, below, false) :-
  make_vars(1, [A]).
ppl_bounds_test_data(test07, constraint, A, above, true) :-
  make_vars(1, [A]).
ppl_bounds_test_data(test07, congruence, A, above, false) :-
  make_vars(1, [A]).

ppl_maxmin_test_data(test07, T, constraint, minimize, A, _, _, _, _, false) :-
   (T = t_ ; T = t_C_), make_vars(1, [A]).

ppl_maxmin_test_data(test07, T, constraint, maximize, A, 0, 1, true,
                     point(0), true) :-
   (T = t_ ; T = t_C_), make_vars(1, [A]).
ppl_maxmin_test_data(test07, t_NNC_, constraint, minimize, A, _, _, _,
                     _, false) :-
   make_vars(1, [A]).
ppl_maxmin_test_data(test07, t_NNC_, constraint, maximize, A, 0, 1, false,
                     closure_point(0), true) :-
   make_vars(1, [A]).
ppl_maxmin_test_data(test07, _, congruence, _, A, _, _, _, _, false) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test07, _Topology, constraint, A, _, _, _, _, false) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test07, _Topology, congruence, A, 2, 1, 0, 1, true) :-
   make_vars(1, [A]).

ppl_constrains_test_data(test07, _, true).

/* Test data for test test08,
    a non-universe bounded object in 1 dimension */

ppl_build_test_data(test08, T, constraints, CS) :-
  (\+ T == t_NNC_,
   make_vars(1, [A]),
   CS = [2*A >= 3, A =< 5]).
ppl_build_test_data(test08, T, generators, GS) :-
  (\+ T == t_NNC_,
   make_vars(1, [A]),
   GS = [point(3*A, 2), point(5*A)]).
ppl_build_test_data(test08, t_NNC_, constraints, CS) :-
  (make_vars(1, [A]),
   CS = [2*A > 3, A < 5]).
ppl_build_test_data(test08, t_NNC_, generators, GS) :-
  (make_vars(1, [A]),
   GS = [point(2*A), closure_point(3*A, 2), closure_point(5*A)]).
ppl_build_test_data(test08, _Topology, congruences, CS) :-
  (make_vars(1, [A]),
   CS = [(2*A =:= 3) / 7]).
ppl_build_test_data(test08, _Topology, grid_generators, GS) :-
  (make_vars(1, [A]),
   GS = [grid_point(3*A, 2), parameter(7*A, 2)]).

ppl_dimension_test_data(test08, _, 1).

ppl_relation_test_data(test08, constraint, A >= 3, [strictly_intersects]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test08, generator, point(5*A, 2), [subsumes]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test08, congruence, (2*A =:= 1) / 3,
                                              [strictly_intersects]) :-
  make_vars(1, [A]).
ppl_relation_test_data(test08, grid_generator, grid_point(5*A), [subsumes]) :-
  make_vars(1, [A]).

ppl_property_test_data(test08, T, _, is_topologically_closed) :-
  \+ T == t_NNC_.
ppl_property_test_data(test08, _, _, contains_integer_point).
ppl_property_test_data(test08, _, congruence, is_discrete).

ppl_bounds_test_data(test08, constraint, A, above, true) :-
  make_vars(1, [A]).
ppl_bounds_test_data(test08, congruence, A, above, false) :-
  make_vars(1, [A]).
ppl_bounds_test_data(test08, constraint, A, below, true) :-
  make_vars(1, [A]).
ppl_bounds_test_data(test08, congruence, A, below, false) :-
  make_vars(1, [A]).

ppl_maxmin_test_data(test08, T, constraint, minimize, A, 3, 2, true,
                     point(3*A, 2), true) :-
   (T = t_ ; T = t_C_), make_vars(1, [A]).

ppl_maxmin_test_data(test08, T, constraint, maximize, A, 5, 1, true,
                     point(5*A), true) :-
   (T = t_ ; T = t_C_), make_vars(1, [A]).
ppl_maxmin_test_data(test08, t_NNC_, constraint, minimize, A, 3, 2, false,
                     closure_point(3*A, 2), true) :-
   make_vars(1, [A]).
ppl_maxmin_test_data(test08, t_NNC_, constraint, maximize, A, 5, 1, false,
                     closure_point(5*A), true) :-
   make_vars(1, [A]).
ppl_maxmin_test_data(test08, _, congruence, _, A, _, _, _, _, false) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test07, _Topology, constraint, A, _, _, _, _, false) :-
   make_vars(1, [A]).

ppl_frequency_test_data(test07, _Topology, congruence, A, 7, 2, 3, 2, true) :-
   make_vars(1, [A]).

ppl_constrains_test_data(test08, _, true).

/* boxes  */

ppl_dimension_test_data(test10, _, 2).
ppl_dimension_test_data(test11, _, 2).
ppl_dimension_test_data(test12, _, 2).

ppl_build_test_data(test00, _Topology, box, [empty]).
ppl_build_test_data(test01, _Topology, box, [empty]).
ppl_build_test_data(test02, _Topology, box, []).
ppl_build_test_data(test03, _Topology, box, [i(o(minf), o(pinf))]).
ppl_build_test_data(test04, _Topology, box, [i(c(1), c(1))]).
ppl_build_test_data(test05, _Topology, box, [i(c(-1), c(-1))]).
ppl_build_test_data(test06, _Topology, box, [i(c(0), o(pinf))]).
ppl_build_test_data(test07, _Topology, box, [i(o(minf), c(0))]).
ppl_build_test_data(test08, _Topology, box, [i(c(3/2), c(5))]).
ppl_build_test_data(test10, _Topology, box,
                               [i(c(1/2), o(pinf)), i(o(minf), c(-1/2))]).
ppl_build_test_data(test11, _Topology, box, [i(c(-4), c(1)), i(c(-1), c(1))]).
ppl_build_test_data(test12, T, box, [i(c(0/2), o(pinf)), i(o(minf), c(1))]) :-
  \+ T == t_NNC_.
ppl_build_test_data(test12, t_NNC_, box, [i(o(0/2), o(pinf)), i(o(minf), o(1))]).
