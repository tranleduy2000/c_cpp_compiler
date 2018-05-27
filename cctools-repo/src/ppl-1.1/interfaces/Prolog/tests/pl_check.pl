/* Various tests on the Prolog interface.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://bugseng.com/products/ppl/ . */


% noisy(F)
% When F = 1, a message is displayed if a time out occurs
% when running the `timeout'` predicate.
% Also, the values of the PPL versions and banner are displayed.
% When F = 0, no 'time out' message or versions are displayed.
% When F = 2, if a test fails and the backtracking returns to a polyhedron
% constructor, the caught error will cause the constraint and generator systems
% for the polyhedron to be displayed.
% noisy/1 can be reset by calling make_noisy/0 or make_quiet/0.

:- dynamic(noisy/1).

% check_all/0
% This executes all the test predicates which, together, check all
% the ppl interface predicates.

check_all :-
   (noisy(_) -> true ; make_quiet),
   list_groups(Groups),
   catch(run_all(Groups), Exception,
         (print_exception_term(Exception), fail)).

% check_quiet/0
% This also executes all the test predicates with no output.

check_quiet :-
   make_quiet,
   check_all.

% check_noisy/0
% check_extra_noisy/0
%
% These also execute all the test predicates but also outputs
% information including the banner, version numbers and expected
% output from the exception tests.

check_noisy :-
   make_noisy,
   check_all.

check_extra_noisy :-
   make_extra_noisy,
   check_all.

% run_all/1
% This executes all the given list of tests, catching any exceptions.
% If any test fails then an error message is output and then it fails.

run_all([Group|Groups]):-
   ppl_initialize,
   (catch(run_one(Group), Exception, run_exception(Group, Exception)) ->
       ppl_finalize,
       run_all(Groups)
   ;
       run_fail(Group)
   ).
run_all([]).

% run_fail/1
% This is used when a test in run_all/1 fails.
% A message is output saying which group of tests has failed;
% then it finalizes the ppl and fails.

run_fail(Group) :-
   group_predicates(Group, Predicates),
   prolog_system(System),
   error_message(['Error occurred while performing test', Group,
                  'which checks predicates:', Predicates]),
   error_message(['Prolog interface checks failed on', System,
                  'Prolog.']),
   ppl_finalize,
   fail.

% run_exception/2
% This is used when a test in run_all/1 causes an exception to be thrown.
% A message is output saying which group of tests was being run when
% the exception was thrown and then it fails.

run_exception(Group, Exception) :-
        group_predicates(Group, Predicates),
        (Exception = ppl_overflow_error(_) ->
            Kind = 'Overflow exception'
        ;
            Kind = 'Exception'
        ),
        prolog_system(System),
        display_message([Kind, 'occurred while performing test ', Group,
                        'which checks predicates ', nl, Predicates]),
        display_message(['Prolog interface checks failed on', System,
                  'Prolog.']),
        print_exception_term(Exception),
        % Do fail for all but overflow exceptions.
        Exception = ppl_overflow_error(_).

% run_one/1
% Runs the named group of tests.

% Tests predicates that return PPL version information and the PPL banner.
% If noisy(0) holds, there is no output but if not,
% all the versions are printed and the banner is pretty printed.
run_one(all_versions_and_banner) :-
  \+ ppl_version_major(-1),
  ppl_version_major(Vmajor),
  ppl_version_minor(Vminor),
  ppl_version_revision(Vrevision),
  ppl_version_beta(Vbeta),
  ppl_version(V),
  ppl_banner(B),
  (noisy(0) -> true ;
     (
      nl,
      write('Version major is '), write(Vmajor), nl,
      write('Version minor is '), write(Vminor), nl,
      write('Version revision is '), write(Vrevision), nl,
      write('Version beta is '), write(Vbeta), nl,
      write('Version is '), write(V), nl,
      banner_pp(B), nl
     )
  ).

% Tests predicates that return the maximum allowed dimension and coefficients.
% If noisy(0) holds, there is no output but if not, the maximums/minimums
% are printed.
run_one(numeric_bounds) :-
  max_dimension,
  coefficient_bounds,
  set_restore_rounding,
  set_restore_irrational_precision.

run_one(new_polyhedron_from_dimension) :-
  new_polyhedron_from_dim.

run_one(new_polyhedron_from_polyhedron) :-
  new_polyhedron_from_polyhedron.

run_one(new_polyhedron_from_representations) :-
  new_polyhedron_from_cons,
  new_polyhedron_from_gens.

run_one(swap_polyhedra) :-
  swap.

run_one(polyhedron_dimension) :-
   space,
   affine_dim,
   constrains,
   unconstrain_space_dimension,
   unconstrain_space_dimensions.

run_one(basic_operators) :-
   inters_assign,
   polyhull_assign,
   polydiff_assign,
   time_elapse,
   top_close_assign.

run_one(add_to_system) :-
   add_con,
   add_gen,
   add_cons,
   add_gens.

run_one(revise_dimensions) :-
   project,
   embed,
   conc_assign,
   remove_dim,
   remove_high_dim,
   expand_dim,
   map_dim,
   fold_dims.

run_one(transform_polyhedron) :-
   affine_image,
   affine_preimage,
   bounded_affine_image,
   bounded_affine_preimage,
   affine_image_gen,
   affine_preimage_gen,
   affine_image_genlr,
   affine_preimage_genlr.

run_one(extrapolation_operators) :-
   widen_BHRZ03,
   widen_BHRZ03_with_tokens,
   lim_extrapolate_BHRZ03,
   lim_extrapolate_BHRZ03_with_tokens,
   bound_extrapolate_BHRZ03,
   bound_extrapolate_BHRZ03_with_tokens,
   widen_H79,
   widen_H79_with_tokens,
   lim_extrapolate_H79,
   lim_extrapolate_H79_with_tokens,
   bound_extrapolate_H79,
   bound_extrapolate_H79_with_tokens.

run_one(get_system) :-
   get_cons,
   get_min_cons,
   get_gens,
   get_min_gens.

run_one(check_polyhedron) :-
   rel_cons,
   rel_gens,
   checks,
   bounds_from_above,
   bounds_from_below.

run_one(minmax_polyhedron) :-
   maximize,
   minimize,
   maximize_with_point,
   minimize_with_point.

run_one(compare_polyhedra) :-
   contains,
   strict_contains,
   disjoint_from,
   equals,
   ok.

run_one(catch_time) :-
   time_out.

run_one(mip_problem) :-
   mip_problem.

run_one(pip_problem) :-
   pip_problem.

% Checks how the PPL Prolog system performs with large integers
% XSB has problems with large numbers - hence tests for XSB disallowed.
% We catch the exception if it is caused by integer overflow in C++
% and suppress output as this is expected when C++ uses checked_integers.
run_one(large_integers) :-
   prolog_system(Prolog_System),
   (Prolog_System \== 'XSB' ->
     catch(large_integers, ppl_overflow_error(Cause),
        check_exception_term(ppl_overflow_error(Cause)))
   ;
     true
   ).

% Checks the handling of exceptions.
run_one(handle_exceptions) :-
   exceptions.

%%%%%%%%%%%%%%%%% numeric bounds %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

max_dimension :-
  ppl_max_space_dimension(M),
  (noisy(0) -> true ;
     display_message(['Maximum possible dimension is', M, nl])
  ).

% coefficient_bounds/0
% This is mainly to check ppl_Coefficient_is_bounded/0,
% ppl_Coefficient_max/1, ppl_Coefficient_min/1.
% But it has to catch the case when the numeric bounds in the
% prolog system are smaller than any finite bounds in C++
% As the test does not know the configuration, all that can be tested
% here is that the results are consistent and the bounds are
% in a list of possible bounds.

coefficient_bounds :-
    (pl_check_prolog_flag(bounded, true) ->
     (pl_check_prolog_flag(max_integer, PLMax),
      pl_check_prolog_flag(min_integer, PLMin))
   ;
     PLMax = 0, PLMin = 0
  ),
  (ppl_Coefficient_is_bounded ->
     (cpp_bounded_values(Max, Min) -> true
      ;
       (Max = PLMax, Min = PLMin)
     )
  ;
     (cpp_unbounded_check, Max = PLMax, Min = PLMin)
  ),
  (noisy(0) -> true ;
     display_message(['Maximum possible coefficient is', Max, nl]),
     display_message(['Minimum possible coefficient is', Min, nl])
  ).

cpp_unbounded_check :-
  \+ ppl_Coefficient_max(_),
  \+ ppl_Coefficient_min(_).

cpp_bounded_values(Max, Min) :-
  ppl_Coefficient_max(Max),
  ppl_Coefficient_min(Min).

set_restore_rounding :-
  ppl_set_rounding_for_PPL,
  ppl_restore_pre_PPL_rounding.

set_restore_irrational_precision :-
  ppl_irrational_precision(Default),
  Default == 128,
  Non_Default = 100,
  ppl_set_irrational_precision(Non_Default),
  ppl_irrational_precision(Changed),
  Changed == Non_Default,
  ppl_set_irrational_precision(Default),
  ppl_irrational_precision(Restored),
  Restored == Default.

%%%%%%%%%%%%%%%%% New Polyhedron %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Note that throughout the tests, all "new_Polyhedron_from_...(...,P)" calls
% are made in such a way that, if the test fails, P is deleted.
% This is done by using special "clean_new_Polyhedron_from_...(...,P)"
% forms of the predicates that are defined later.
%
% As we also delete P on success of the test, to prevent trying to
% delete P again when a later test fails, we always have a cut before these
% in-line calls to ppl_Polyhedron_delete(P).

% Tests new_C_Polyhedron_from_space_dimension/3,
%       new_NNC_Polyhedron_from_space_dimension/3 and
%       ppl_delete_Polyhedron/1.
new_polyhedron_from_dim :-
  new_polyhedron_from_dim(c, universe),
  new_polyhedron_from_dim(nnc, universe),
  new_polyhedron_from_dim(c, empty),
  new_polyhedron_from_dim(nnc, empty).

% This also uses ppl_Polyhedron_is_universe/1
% and ppl_Polyhedron_is_empty.
new_polyhedron_from_dim(T, Universe_Or_Empty) :-
  \+ clean_ppl_new_Polyhedron_from_space_dimension(T, 3, Universe_Or_Empty, 0),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, Universe_Or_Empty, P),
  (Universe_Or_Empty = universe ->
      (ppl_Polyhedron_is_universe(P),
      \+ ppl_Polyhedron_is_empty(P))
   ;
      (ppl_Polyhedron_is_empty(P),
      \+ ppl_Polyhedron_is_universe(P))
  ),
  !,
  ppl_delete_Polyhedron(P).

% Tests ppl_new_C_Polyhedron_from_C_Polyhedron/2,
%       ppl_new_C_Polyhedron_from_NNC_Polyhedron/2,
%       ppl_new_NNC_Polyhedron_from_C_Polyhedron/2, and
%       ppl_new_NNC_Polyhedron_from_NNC_Polyhedron/2,
new_polyhedron_from_polyhedron :-
  new_polyhedron_from_polyhedron(c, c),
  new_polyhedron_from_polyhedron(nnc, nnc),
  new_polyhedron_from_polyhedron(c, nnc),
  new_polyhedron_from_polyhedron(nnc, c).

% This also uses ppl_new_Polyhedron_from_constraints/3 and
% ppl_Polyhedron_equals_Polyhedron/2.
new_polyhedron_from_polyhedron(T1, T2) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T1, 3, universe, P1),
  \+ clean_ppl_new_Polyhedron_from_Polyhedron(T1, P1, T2, 0),
  clean_ppl_new_Polyhedron_from_Polyhedron(T1, P1, T2, P2),
  clean_ppl_new_Polyhedron_from_Polyhedron(T2, P2, T1, P1a),
  ppl_Polyhedron_equals_Polyhedron(P1, P1a),
  clean_ppl_new_Polyhedron_from_Polyhedron(T1, P1a, T2, P2a),
  ppl_Polyhedron_equals_Polyhedron(P2, P2a),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P1a),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P2a),
  make_vars(3, [A, B, C]),
  (T1 = c
          -> CS = [3 >= A, 4 >= A, 4*A + B - 2*C >= 5]
          ;  CS = [3 >= A, 4 >  A, 4*A + B - 2*C >= 5]
  ),
  clean_ppl_new_Polyhedron_from_constraints(T1, CS, P3),
  clean_ppl_new_Polyhedron_from_Polyhedron(T1, P3, T2, P4),
  clean_ppl_new_Polyhedron_from_Polyhedron(T2, P4, T1, P3a),
  clean_ppl_new_Polyhedron_from_Polyhedron(T1, P3a, T2, P4a),
  ppl_Polyhedron_equals_Polyhedron(P3, P3a),
  ppl_Polyhedron_equals_Polyhedron(P4, P4a),
  !,
  ppl_delete_Polyhedron(P3),
  ppl_delete_Polyhedron(P4),
  ppl_delete_Polyhedron(P3a),
  ppl_delete_Polyhedron(P4a).

% Tests ppl_new_Polyhedron_from_constraints/2
%       ppl_new_Polyhedron_from_constraints/2.
new_polyhedron_from_cons :-
  new_polyhedron_from_cons(c, [3 >= '$VAR'(1)]),
  make_vars(4, [A, B, C, D]),
  new_polyhedron_from_cons(c, [3 >= A, 4*A + B - 2*C >= 5, D = 1]),
  new_polyhedron_from_cons(c, [B >= A, 4*A + B - 2*C >= 5, D = 1]),
  new_polyhedron_from_cons(nnc, [3 > A, 4*A + B - 2*C >= 5, D = 1]),
  new_polyhedron_from_cons(nnc, [B > A, 4*A + B - 2*C >= 5, D = 1]).

new_polyhedron_from_cons(T, CS) :-
  clean_ppl_new_Polyhedron_from_constraints(T, [], P),
  \+ clean_ppl_new_Polyhedron_from_constraints(T, [], 0),
  ppl_Polyhedron_is_universe(P),
  clean_ppl_new_Polyhedron_from_constraints(T, CS, Pa),
  \+ ppl_Polyhedron_is_universe(Pa),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Pa).

% Tests ppl_new_Polyhedron_from_generators/2 and
%       ppl_new_Polyhedron_from_generators/2.
new_polyhedron_from_gens :-
  make_vars(3, [A, B, C]),
  new_polyhedron_from_gens(c,[point(A + B + C, 1), point(A + B + C)] ),
  new_polyhedron_from_gens(nnc,  [point(A + B + C), closure_point(A + B + C)]).

new_polyhedron_from_gens(T, GS) :-
  \+ clean_ppl_new_Polyhedron_from_generators(T, [], 0),
  clean_ppl_new_Polyhedron_from_generators(T, [], P),
  ppl_Polyhedron_is_empty(P),
  clean_ppl_new_Polyhedron_from_generators(T, GS, Pa),
  \+ ppl_Polyhedron_is_empty(Pa),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Pa).


%%%%%%%%%%%%%%%%% Swap Polyhedra %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_swap/2.
swap :-
  swap(c), swap(nnc).

swap(T) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, empty, Q),
  ppl_Polyhedron_swap(P, Q),
  ppl_Polyhedron_is_empty(P),
  ppl_Polyhedron_is_universe(Q),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q).

%%%%%%%%%%%%%%%%%% Poly Dimension %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_space_dimension/2.
space :-
 space(c), space(nnc).

space(T) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_space_dimension(P, N),
  N = 3,
  \+ ppl_Polyhedron_space_dimension(P, 4),
  clean_ppl_new_Polyhedron_from_generators(T, [], Q),
  ppl_Polyhedron_space_dimension(Q, M),
  M == 0,
  clean_ppl_new_Polyhedron_from_constraints(T, [], Q1),
  ppl_Polyhedron_space_dimension(Q1, M1),
  M1 == 0,
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q),
  ppl_delete_Polyhedron(Q1).


% Tests ppl_Polyhedron_affine_dimension/2.
affine_dim :-
 affine_dim(c), affine_dim(nnc).

affine_dim(T) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_affine_dimension(P, N),
  N == 3,
  \+ ppl_Polyhedron_affine_dimension(P, 2),
  clean_ppl_new_Polyhedron_from_generators(T, [], Q),
  ppl_Polyhedron_affine_dimension(Q, M),
  M == 0,
  clean_ppl_new_Polyhedron_from_constraints(T, [], Q1),
  ppl_Polyhedron_affine_dimension(Q1, M1),
  M1 == 0,
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q),
  ppl_delete_Polyhedron(Q1),
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_generators(T,
                                     [point(A), ray(B)],
                                     P1),
  ppl_Polyhedron_space_dimension(P1, 2),
  ppl_Polyhedron_affine_dimension(P1, 1),
  clean_ppl_new_Polyhedron_from_generators(T,
                                     [point(A + B, 2)],
                                     P2),
  ppl_Polyhedron_space_dimension(P2, 2),
  ppl_Polyhedron_affine_dimension(P2, 0),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A - B >= 0, B >= 0,
                                       A + B =< 1, B =< 0],
                                      P3),
  ppl_Polyhedron_space_dimension(P3, 2),
  ppl_Polyhedron_affine_dimension(P3, 1),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A - B >= 0, B >= 1,
                                       A + B =< 1],
                                      P4),
  (T = c ->
            ppl_Polyhedron_add_constraint(P4, B =< 0)
         ;
            ppl_Polyhedron_add_constraint(P4, B < 1)
  ),
  ppl_Polyhedron_space_dimension(P4, 2),
  ppl_Polyhedron_affine_dimension(P4, 0),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P3),
  ppl_delete_Polyhedron(P4).

% Tests ppl_Polyhedron_constrains/2.
constrains :-
  constrains(c), constrains(nnc).

constrains(T) :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, [B >= 0, B - C >= 2]),
  ppl_Polyhedron_constrains(P, B),
  \+ppl_Polyhedron_constrains(P, A),
  ppl_Polyhedron_OK(P),
  !,
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_unconstrain_space_dimension/2.
unconstrain_space_dimension :-
  unconstrain_space_dimension(c), unconstrain_space_dimension(nnc).

unconstrain_space_dimension(T) :-
  make_vars(3, [_A, B, C]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, [B >= 0, B - C >= 2]),
  ppl_Polyhedron_unconstrain_space_dimension(P, B),
  \+ppl_Polyhedron_constrains(P, B),
  ppl_Polyhedron_OK(P),
  !,
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_unconstrain_space_dimensions/2.
unconstrain_space_dimensions :-
  unconstrain_space_dimensions(c), unconstrain_space_dimensions(nnc).

unconstrain_space_dimensions(T) :-
  make_vars(3, [_A, B, C]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, [B >= 0, B - C >= 2]),
  ppl_Polyhedron_unconstrain_space_dimensions(P, []),
  ppl_Polyhedron_constrains(P, B),
  ppl_Polyhedron_unconstrain_space_dimensions(P, [B]),
  \+ppl_Polyhedron_constrains(P, B),
  ppl_Polyhedron_unconstrain_space_dimensions(P, [B]),
  \+ppl_Polyhedron_constrains(P, B),
  ppl_Polyhedron_OK(P),
  !,
  ppl_delete_Polyhedron(P).

%%%%%%%%%%%%%%%% Basic Operators %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_intersection_assign/2.
inters_assign :-
  inters_assign(c), inters_assign(nnc).

inters_assign(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_generators(T,
                                     [point(0), point(B),
                                      point(A), point(A, 2)],
                                     P1),
  clean_ppl_new_Polyhedron_from_generators(T,
                                     [point(0), point(A),
                                      point(A + B), point(A, 2)],
                                     P2),
  ppl_Polyhedron_intersection_assign(P1, P2),
  clean_ppl_new_Polyhedron_from_generators(T,
                                     [point(A + B, 2),
                                      point(A), point(0)],
                                     P1a),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A - B >= 0, B >= 0,
                                       A + B =< 1],
                                      P1b),
  ppl_Polyhedron_equals_Polyhedron(P1, P1a),
  ppl_Polyhedron_equals_Polyhedron(P1, P1b),
  clean_ppl_new_Polyhedron_from_constraints(T, [A =< -1, B =< -1], P3),
  ppl_Polyhedron_intersection_assign(P1, P3),
  ppl_Polyhedron_is_empty(P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P3),
  ppl_delete_Polyhedron(P1a),
  ppl_delete_Polyhedron(P1b).

% Tests ppl_Polyhedron_concatenate_assign/2.
conc_assign :-
  conc_assign(c), conc_assign(nnc).

conc_assign(T) :-
  make_vars(5, [A, B, C, D, E]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  clean_ppl_new_Polyhedron_from_constraints(T, [A >= 1, B >= 0, C >= 0], Q),
  ppl_Polyhedron_concatenate_assign(P, Q),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [C >= 1, D >= 0, E >= 0],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_poly_hull_assign/2.
polyhull_assign :-
  polyhull_assign(c), polyhull_assign(nnc).

polyhull_assign(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_generators(T,
                                     [point(0), point(B),
                                      point(A), point(A,2)],
                                     P1),
  clean_ppl_new_Polyhedron_from_generators(T,
                                     [point(0), point(A),
                                      point(A + B), point(A, 2)],
                                     P2),
  ppl_Polyhedron_poly_hull_assign(P1, P2),
  clean_ppl_new_Polyhedron_from_generators(T,
      [point(1*A+1*B), point(1*A, 2), point(1*A), point(1*B), point(0)], P1a),
  clean_ppl_new_Polyhedron_from_constraints(T,
      [1*A>=0, 1*B>=0, -1*B>= -1, -1*A>= -1], P1b),
  ppl_Polyhedron_equals_Polyhedron(P1, P1a),
  ppl_Polyhedron_equals_Polyhedron(P1, P1b),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P1a),
  ppl_delete_Polyhedron(P1b).

% Tests ppl_Polyhedron_poly_difference_assign/2.
polydiff_assign :-
  make_vars(2, [A, B]),
  GS0 = [point(2*A)],
  GS1 = [point(0), point(2*A)],
  GS2 = [point(0), point(A)],
  GS3 = [point(A), point(2*A)],
  GS4 = [closure_point(A), point(2*A)],
  GS4a = [closure_point(A), point(3*A, 2), closure_point(2*A)],
  polydiff_assign(c, GS1, GS2, GS3),
  polydiff_assign(c, GS1, GS3, GS2),
  polydiff_assign(c, GS3, GS0, GS3),
  polydiff_assign(nnc, GS1, GS2, GS4),
  polydiff_assign(nnc, GS1, GS4, GS2),
  polydiff_assign(nnc, GS4, GS0, GS4a),
  GS5 = [point(0), point(B), point(A)],
  GS6 = [point(0), point(A), point(A + B)],
  GS6a = [point(0), point(A), point(A + B, 2)],
  GS7 = [point(A + B, 2), point(B), point(0)],
  GS8 = [closure_point(A + B, 2), point(B), closure_point(0)],
  polydiff_assign(c, GS5, GS6, GS7),
  polydiff_assign(c, GS5, GS7, GS6a),
  polydiff_assign(nnc, GS5, GS6, GS8),
  polydiff_assign(nnc, GS5, GS8, GS6a),
  GS_empty = [],
  polydiff_assign(c, GS0, GS0, GS_empty),
  polydiff_assign(nnc, GS4, GS4, GS_empty),
  polydiff_assign(nnc, GS4, GS3, GS_empty).

polydiff_assign(T, GS1, GS2, GS3) :-
  clean_ppl_new_Polyhedron_from_generators(T, GS1, P1),
  ppl_Polyhedron_space_dimension(P1, Dim),
  clean_ppl_new_Polyhedron_from_space_dimension(T, Dim, empty, P2),
  ppl_Polyhedron_add_generators(P2, GS2),
  ppl_Polyhedron_poly_difference_assign(P1, P2),
  clean_ppl_new_Polyhedron_from_space_dimension(T, Dim, empty, P3),
  ppl_Polyhedron_add_generators(P3, GS3),
  ppl_Polyhedron_equals_Polyhedron(P1, P3),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P3).

% Tests ppl_Polyhedron_time_elapse_assign/2.
time_elapse :-
  time_elapse(c), time_elapse(nnc).

% Tests ppl_Polyhedron_time_elapse for C Polyhedra.
time_elapse(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraints(P,
                          [A >= 1, A =< 3, B >= 1, B =< 3]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, Q),
  ppl_Polyhedron_add_constraints(Q, [B = 5]),
  ppl_Polyhedron_time_elapse_assign(P, Q),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, Pa),
  ppl_Polyhedron_add_constraints(Pa, [B >= 1]),
  clean_ppl_new_Polyhedron_from_constraints(T, [B = 5], Qa),
  ppl_Polyhedron_equals_Polyhedron(Q, Qa),
  ppl_Polyhedron_equals_Polyhedron(P, Pa),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q),
  ppl_delete_Polyhedron(Pa),
  ppl_delete_Polyhedron(Qa).

% Tests ppl_Polyhedron_topological_closure_assign/1.
top_close_assign :-
  make_vars(3, [A, B, C]),
  GS_close = [point(A + B), point(0), ray(A), ray(B)],
  GS_not_close = [point(A + B), closure_point(0), ray(A), ray(B)],
  CS_close = [4*A + B + -2*C >= 5, A =< 3],
  CS_not_close = [4*A + B - 2*C >= 5, A < 3],
  top_close_assign(c, gensys, GS_close, GS_close),
  top_close_assign(nnc, gensys, GS_not_close, GS_close),
  top_close_assign(c, consys, CS_close, CS_close),
  top_close_assign(nnc, consys, CS_not_close, CS_close).

top_close_assign(T, gensys, GS, GS_close) :-
  clean_ppl_new_Polyhedron_from_generators(T, GS, P),
  ppl_Polyhedron_topological_closure_assign(P),
  clean_ppl_new_Polyhedron_from_generators(T, GS_close, Pa),
  ppl_Polyhedron_equals_Polyhedron(P, Pa),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Pa).

top_close_assign(T, consys, CS, CS_close) :-
  clean_ppl_new_Polyhedron_from_constraints(T, CS, P),
  ppl_Polyhedron_topological_closure_assign(P),
  clean_ppl_new_Polyhedron_from_constraints(T, CS_close, Pa),
  ppl_Polyhedron_equals_Polyhedron(P, Pa),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Pa).


%%%%%%%%%%%%%%%%%% Add Constraints or Generators %%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_add_constraint/2.
add_con :-
  add_con(c), add_con(nnc).

add_con(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraint(P, A - B >= 1),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A - B >= 1],
                                      Pa),
  ppl_Polyhedron_equals_Polyhedron(P, Pa),
  ppl_Polyhedron_add_constraint(P, A = 0),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A = 0, B =< -1],
                                      Pb),
  ppl_Polyhedron_equals_Polyhedron(P, Pb),
  ppl_Polyhedron_add_constraint(P, A = 1),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, empty, Pc),
  ppl_Polyhedron_equals_Polyhedron(P, Pc),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Pa),
  ppl_delete_Polyhedron(Pb),
  ppl_delete_Polyhedron(Pc).

% Tests ppl_Polyhedron_add_generator/2.
add_gen :-
  add_gen(c), add_gen(nnc).

add_gen(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_generator(P, point(A + B)),
  clean_ppl_new_Polyhedron_from_generators(T,
                                     [point(A + B), point(0),
                                      line(A), line(B)], P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(P1).

% Tests ppl_Polyhedron_add_constraints/2.
add_cons :-
  make_vars(3, [A, B, C]),
  add_cons(c, [A >= 1, B >= 0, 4*A + B - 2*C >= 5], [A =< 0]),
  add_cons(nnc, [A > 1, B >= 0, 4*A + B - 2*C > 5], [A < 0]).

add_cons(T, CS, CS1) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, CS),
  clean_ppl_new_Polyhedron_from_constraints(T, CS, P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  ppl_Polyhedron_add_constraints(P, CS1),
  ppl_Polyhedron_is_empty(P),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(P1).

% Tests ppl_Polyhedron_add_generators/2.
add_gens :-
  make_vars(3, [A, B, C]),
  add_gens(c, [point(A + B + C), ray(A), ray(2*A), point(A + B + C, 1),
               point(100*A + 5*B, -8)]),
  add_gens(nnc, [point(A + B + C), ray(A), ray(2*A), point(A + B + C, 1),
                 closure_point(100*A + 5*B, -8)]).

add_gens(T, GS) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, empty, P),
  ppl_Polyhedron_add_generators(P, GS),
  clean_ppl_new_Polyhedron_from_generators(T, GS, P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(P1).

%%%%%%%%%%%%%%%%%% Change Dimensions %%%%%%%%%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_add_space_dimensions_and_project/2.
project :-
  project(c), project(nnc).

project(T) :-
  make_vars(4, [A, B, C, D]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraints(P, [A >= 1, B >= 0]),
  ppl_Polyhedron_add_space_dimensions_and_project(P, 0),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A >= 1, B >= 0],
                                      P0),
  ppl_Polyhedron_equals_Polyhedron(P, P0),
  ppl_delete_Polyhedron(P0),
  ppl_Polyhedron_add_space_dimensions_and_project(P, 2),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A >= 1, B >= 0, C = 0, D = 0],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_add_space_dimensions_and_embed/2.
embed :-
  embed(c), embed(nnc).

embed(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraints(P, [A >= 1, B >= 0]),
  ppl_Polyhedron_add_space_dimensions_and_embed(P, 0),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A >= 1, B >= 0],
                                      P0),
  ppl_Polyhedron_equals_Polyhedron(P, P0),
  ppl_delete_Polyhedron(P0),
  ppl_Polyhedron_add_space_dimensions_and_embed(P, 2),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 4, universe, P1),
  ppl_Polyhedron_add_constraints(P1, [A >= 1, B >= 0]),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_remove_space_dimensions/2.
remove_dim :-
  remove_dim(c), remove_dim(nnc).

remove_dim(T) :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, [A >= 1, B >= 0, C >= 2]),
  ppl_Polyhedron_remove_space_dimensions(P, []),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A >= 1, B >= 0, C >= 2],
                                      P0),
  ppl_Polyhedron_equals_Polyhedron(P, P0),
  ppl_delete_Polyhedron(P0),
  ppl_Polyhedron_remove_space_dimensions(P,[B]),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A >= 1, B >= 2],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  ppl_delete_Polyhedron(P1),
  % Note: now 'B' refers to the old 'C' variable.
  ppl_Polyhedron_remove_space_dimensions(P,[A, B]),
  ppl_Polyhedron_space_dimension(P, 0),
  !,
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_remove_higher_space_dimensions/2.
remove_high_dim :-
  remove_high_dim(c), remove_high_dim(nnc).

remove_high_dim(T) :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, [A >= 1, B >= 0, C >= 0]),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A >= 1, B >= 0, C >= 0],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  ppl_Polyhedron_remove_higher_space_dimensions(P, 1),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A >= 1],
                                      P2),
  ppl_Polyhedron_equals_Polyhedron(P, P2),
  ppl_Polyhedron_remove_higher_space_dimensions(P, 1),
  ppl_Polyhedron_equals_Polyhedron(P, P2),
  ppl_Polyhedron_remove_higher_space_dimensions(P, 0),
  ppl_Polyhedron_space_dimension(P, 0),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_expand_space_dimension/3.
expand_dim :-
  expand_dim(c), expand_dim(nnc).

expand_dim(T) :-
  make_vars(4, [A, B, C, D]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, [A >= 1, B >= 0, C >= 2]),
  ppl_Polyhedron_expand_space_dimension(P, B, 1),
  ppl_Polyhedron_space_dimension(P, 4),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A >= 1, B >= 0, C >= 2, D >= 0],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  ppl_delete_Polyhedron(P1),
  ppl_Polyhedron_remove_higher_space_dimensions(P, 2),
  ppl_Polyhedron_expand_space_dimension(P, A, 2),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [D >= 1, C >= 1, A >= 1, B >= 0],
                                      P2),
  ppl_Polyhedron_equals_Polyhedron(P, P2),
  ppl_delete_Polyhedron(P2),
  ppl_Polyhedron_space_dimension(P, 4),
  !,
  ppl_delete_Polyhedron(P),
% Example taken from [GopanDMDRS04], page 519.
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, empty, Ptacas),
  ppl_Polyhedron_add_generators(Ptacas,
       [point(A + 2*B), point(A + 3*B), point(A + 4*B)]),
  ppl_Polyhedron_expand_space_dimension(Ptacas, B, 1),
  ppl_Polyhedron_space_dimension(Ptacas, 3),
  clean_ppl_new_Polyhedron_from_generators(T,
       [point(A + 2*B + 2*C), point(A + 2*B + 3*C), point(A + 2*B + 4*C),
        point(A + 3*B + 2*C), point(A + 3*B + 3*C), point(A + 3*B + 4*C),
        point(A + 4*B + 2*C), point(A + 4*B + 3*C), point(A + 4*B + 4*C)],
                                      Ptacas1),
  ppl_Polyhedron_equals_Polyhedron(Ptacas, Ptacas1),
  !,
  ppl_delete_Polyhedron(Ptacas1),
  ppl_delete_Polyhedron(Ptacas).

% Tests ppl_Polyhedron_fold_space_dimension/3.
fold_dims :-
  fold_dims(c), fold_dims(nnc).

fold_dims(T) :-
  make_vars(4, [A, B, C, D]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 4, universe, P),
  ppl_Polyhedron_add_constraints(P, [A >= 1, B >= 0, C >= 2, D >= 0]),
  ppl_Polyhedron_fold_space_dimensions(P, [D], B),
  ppl_Polyhedron_space_dimension(P, 3),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P1),
  ppl_Polyhedron_add_constraints(P1, [A >= 1, B >= 0, C >= 2]),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_Polyhedron_fold_space_dimensions(P, [A, C], B),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 1, universe, P2),
  ppl_Polyhedron_add_constraints(P2, [A >= 0]),
  ppl_Polyhedron_equals_Polyhedron(P, P2),
  ppl_delete_Polyhedron(P2),
  ppl_Polyhedron_space_dimension(P, 1),
  !,
  ppl_delete_Polyhedron(P),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, Ptacas),
  ppl_Polyhedron_add_constraints(Ptacas, [A >= 1, A =< 3, B >= 7, B =< 12]),
  ppl_Polyhedron_fold_space_dimensions(Ptacas, [A], B),
  ppl_Polyhedron_space_dimension(Ptacas, 1),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 1, universe, Ptacas1),
  ppl_Polyhedron_add_constraints(Ptacas1, [A >= 1, A =< 12]),
  ppl_Polyhedron_equals_Polyhedron(Ptacas, Ptacas1),
  !,
  ppl_delete_Polyhedron(Ptacas1),
  ppl_delete_Polyhedron(Ptacas).

% Tests ppl_Polyhedron_map_space_dimensions/2.
map_dim:-
  map_dim(c), map_dim(nnc).

map_dim(T) :-
  make_vars(7, [A, B, C, D, E, F, G]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, [A >= 2, B >= 1, C >= 0]),
  ppl_Polyhedron_map_space_dimensions(P, [A-B, B-C, C-A]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, Q),
  ppl_Polyhedron_add_constraints(Q, [A >= 0, B >= 2, C >= 1]),
  ppl_Polyhedron_equals_Polyhedron(P, Q),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 4, empty, P0),
  ppl_Polyhedron_add_generators(P0, [point(2*C), line(A+B), ray(A+C)]),
  ppl_delete_Polyhedron(P0),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 4, empty, P1),
  ppl_Polyhedron_add_generators(P1, [point(2*C), line(A+B), ray(A+C)]),
  ppl_Polyhedron_map_space_dimensions(P1, [A-C, C-A, B-B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, empty, Q1),
  ppl_Polyhedron_add_generators(Q1, [point(2*A), ray(A+C), line(B+C)]),
  ppl_Polyhedron_equals_Polyhedron(P1, Q1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(Q1),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 5, universe, P2),
  ppl_Polyhedron_add_constraints(P2, [B = 2, E = 8]),
  ppl_Polyhedron_add_space_dimensions_and_embed(P2, 2),
  ppl_Polyhedron_map_space_dimensions(P2, [A-A, B-B, C-E, D-F, E-G, F-C, G-D]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 7, universe, Q2),
  ppl_Polyhedron_add_constraints(Q2, [B = 2, G = 8]),
  ppl_Polyhedron_equals_Polyhedron(P2, Q2),
  !,
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(Q2).


%%%%%%%%%%%%%%%%%% Affine Transformations %%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_affine_image/4.
affine_image :-
  affine_image(c), affine_image(nnc).

affine_image(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraint(P, A - B = 1),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A - B = 1],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  ppl_Polyhedron_affine_image(P, A, A + 1, 1),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A - B = 2],
                                      P2),
  ppl_Polyhedron_equals_Polyhedron(P, P2),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_affine_preimage/4.
affine_preimage :-
  affine_preimage(c), affine_preimage(nnc).

affine_preimage(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraint(P, A + B >= 10),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A + B >= 10],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  ppl_Polyhedron_affine_preimage(P, A, A + 1, 1),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A + B >= 9],
                                      P2),
  ppl_Polyhedron_equals_Polyhedron(P, P2),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_bounded_affine_image/5
bounded_affine_image :-
  bounded_affine_image(c), bounded_affine_image(nnc).

bounded_affine_image(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraint(P, A - B = 1),
  ppl_Polyhedron_bounded_affine_image(P, B, A - 1, A + 1, 2),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A - 2*B =< 1, 2*B - A =< 1],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_bounded_affine_preimage/5
bounded_affine_preimage :-
  bounded_affine_preimage(c), bounded_affine_preimage(nnc).

bounded_affine_preimage(T) :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_Polyhedron_from_constraints(T,
            [A >= 0, A =< 4, B >= 0, B =< 4, A - B =< 2, A - B >= -2],
            P),
  clean_ppl_new_Polyhedron_from_Polyhedron(T, P, T, P1),
  ppl_Polyhedron_add_space_dimensions_and_embed(P1, 1),
  ppl_Polyhedron_bounded_affine_preimage(P, B, 7 - A, A + 3, 1),
  ppl_Polyhedron_add_constraint(P1, 7 - A =< B),
  ppl_Polyhedron_add_constraint(P1, B =< A + 3),
  ppl_Polyhedron_remove_space_dimensions(P1, [B]),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P),
  clean_ppl_new_Polyhedron_from_constraints(T,
            [A >= 0, A =< 4, B >= 0, B =< 4, A - B =< 2, A - B >= -2],
            Q),
  clean_ppl_new_Polyhedron_from_Polyhedron(T, Q, T, Q1),
  ppl_Polyhedron_add_space_dimensions_and_embed(Q1, 1),
  ppl_Polyhedron_bounded_affine_preimage(Q, B, 7 - 3*A + 2*B, B + 5*A - 3, 1),
  ppl_Polyhedron_add_constraint(Q1, 7 - 3*A + 2*C =< B),
  ppl_Polyhedron_add_constraint(Q1, B =< C + 5*A - 3),
  ppl_Polyhedron_remove_space_dimensions(Q1, [B]),
  ppl_Polyhedron_equals_Polyhedron(Q, Q1),
  !,
  ppl_delete_Polyhedron(Q1),
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_generalized_affine_image/5.
affine_image_gen :-
  affine_image_gen(c), affine_image_gen(nnc).

affine_image_gen(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraint(P, A - B = 1),
  ppl_Polyhedron_generalized_affine_image(P, A, =<, A + 1, 1),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [A - B =< 2],
                                      P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_generalized_affine_preimage/5.
affine_preimage_gen :-
  affine_preimage_gen(c), affine_preimage_gen(nnc).

affine_preimage_gen(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraints(P, [A >= 0, A =< 4, B =< 5, A =< B]),
  ppl_Polyhedron_generalized_affine_preimage(P, B, >=, A+2, 1),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P1),
  ppl_Polyhedron_add_constraints(P1, [A >= 0, A =< 3]),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_generalized_affine_image_lhs_rhs/4.
affine_image_genlr :-
  make_vars(2, [A, B]),
  affine_image_genlr(c, =<, [B - A =< 2], [A,B]),
  affine_image_genlr(c, =, [B - A = 2], [A,B]),
  affine_image_genlr(nnc, <, [B - A < 2], [A,B]),
  affine_image_genlr(nnc, =<, [B - A =< 2], [A,B]).

affine_image_genlr(T, R, CS, [A,B]) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraint(P, A - B = 1),
  ppl_Polyhedron_generalized_affine_image_lhs_rhs(P, B - 1, R, A + 1),
  clean_ppl_new_Polyhedron_from_constraints(T, CS, P1),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P).

% % Tests ppl_Polyhedron_generalized_affine_preimage_lhs_rhs/4.
affine_preimage_genlr :-
  make_vars(2, [A, B]),
  affine_preimage_genlr(c, B - 1, =<,  A + 1,
      [A >= 0, A =< 4, B >= 0, B =< A], [A,B]),
  affine_preimage_genlr(c, B + 1, =, A + 1,
      [A >= 0, A =< 4, B >= 0, B =< A], [A,B]),
  affine_preimage_genlr(nnc, B - 1, <, A + 1,
      [A >= 0, A =< 4, B >= 0, B =< A], [A,B]),
  affine_preimage_genlr(nnc, B - 1, =<, A + 1,
      [A >= 0, A =< 4, B >= 0, B =< A], [A,B]).

affine_preimage_genlr(T, LHS, R, RHS, CS, [A,_B]) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_add_constraints(P, CS),
  ppl_Polyhedron_generalized_affine_preimage_lhs_rhs(P, LHS, R, RHS),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P1),
  ppl_Polyhedron_add_constraints(P1, [A >= 0, A =< 4]),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P).

%%%%%%%%%%%%%%%%%% Widen and Extrapolation Operators %%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_BHRZ03_widening_assign/2.
widen_BHRZ03 :-
  make_vars(2, [A, B]),
  widen_BHRZ03(c, [A >= 1, B >= 0], [A >= 1, B >= 1],
                  [A >= 1],         [A >= 1, B >= 1]
              ),
  widen_BHRZ03(nnc, [A > 1, B > 0], [A > 1, B > 1],
                    [A > 1],        [A > 1, B > 1]
              ).

widen_BHRZ03(Topology, CS_P, CS_Q, CS_Pa, CS_Qa) :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  ppl_Polyhedron_BHRZ03_widening_assign(P, Q),
  widen_extrapolation_final(P, CS_Pa, Topology),
  widen_extrapolation_final(Q, CS_Qa, Topology).

% Tests ppl_Polyhedron_BHRZ03_widening_assign_with_tokens/4.
widen_BHRZ03_with_tokens :-
  make_vars(2, [A, B]),
  widen_BHRZ03_with_tokens(c, [A >= 1], [A >= 1, B >= 1],
                             [A >= 1], [A >= 1, B >= 1], 1, 1
                         ),
  widen_BHRZ03_with_tokens(c, [A >= 1, B >= 0], [A >= 1, B >= 1],
                             [A >= 1], [A >= 1, B >= 1], 1, 0
                         ),
  widen_BHRZ03_with_tokens(nnc, [A > 1], [A > 1, B > 1],
                               [A > 1], [A > 1, B > 1], 2, 2
                         ),
  widen_BHRZ03_with_tokens(nnc, [A > 1, B >= 0], [A > 1, B >= 1],
                               [A > 1, B >= 0], [A > 1, B >= 1], 3, 1
                         ).

widen_BHRZ03_with_tokens(Topology,
                      CS_P, CS_Q, CS_Pa, CS_Qa, Token_i, Token_o) :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  \+ ppl_Polyhedron_BHRZ03_widening_assign_with_tokens(P, Q,
                                            Token_i, 4),
  \+ ppl_Polyhedron_BHRZ03_widening_assign_with_tokens(P, Q,
                                            Token_i, not_a_number),
  ppl_Polyhedron_BHRZ03_widening_assign_with_tokens(P, Q, Token_i, X),
  ppl_Polyhedron_BHRZ03_widening_assign_with_tokens(P, Q, X, Y),
  Y == Token_o,
  widen_extrapolation_final(P, CS_Pa, Topology),
  widen_extrapolation_final(Q, CS_Qa, Topology), !.

% Tests ppl_Polyhedron_limited_BHRZ03_extrapolation_assign/3.
lim_extrapolate_BHRZ03 :-
  make_vars(2, [A, B]),
  lim_extrapolate_BHRZ03(c, [A >= 1, B >= 0], [A >= 2, B >= 1],
                            [A >= 1, B >= 0], [A >= 1, B >= 0]
                        ),
  lim_extrapolate_BHRZ03(c, [A >= 1, B >= 0], [A >= 2, B >= 1],
                            [A >= 2],         []
                        ),
  lim_extrapolate_BHRZ03(nnc, [A > 1, B > 0], [A > 2, B > 1],
                              [A > 1, B > 0], [A > 1, B > 0]
                        ),
  lim_extrapolate_BHRZ03(nnc, [A > 1, B >= 0], [A > 2, B >= 1],
                              [A >= 2],        []
                        ).

lim_extrapolate_BHRZ03(Topology, CS_P, CS_Q, CS_lim, CS_Pa)  :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  ppl_Polyhedron_limited_BHRZ03_extrapolation_assign(P, Q, CS_lim),
  widen_extrapolation_final(P, CS_Pa, Topology),
  !,
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens/5.
lim_extrapolate_BHRZ03_with_tokens :-
  make_vars(2, [A, B]),
  lim_extrapolate_BHRZ03_with_tokens(c,
                  [A >= 1, B >= 0], [A >= 1, B >= 1],
                  [A >= 1, B >= 0], [A >= 1, B >= 0], 1, 0
                                   ),
  lim_extrapolate_BHRZ03_with_tokens(nnc,
                    [A > 1, B > 0], [A > 1, B > 1],
                    [A > 1, B > 0], [A > 1, B > 0], 1, 0
                                   ).

lim_extrapolate_BHRZ03_with_tokens(Topology,
                 CS_P, CS_Q, CS_lim, CS_Pa, Token_i, Token_o) :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  Wrong_Token is Token_i + 1,
  \+ ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens(P, Q,
                                            CS_lim, Token_i, Wrong_Token),
  \+ ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens(P, Q,
                                            CS_lim, Token_i, not_a_number),
  ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens(P, Q,
                                            CS_lim, Token_i, X),
  ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens(P, Q,
                                            CS_lim, X, Y),
  Y == Token_o,
  widen_extrapolation_final(P, CS_Pa, Topology),
  !,
  ppl_delete_Polyhedron(Q).


% Tests ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign/3.
bound_extrapolate_BHRZ03 :-
  make_vars(2, [A, B]),
  bound_extrapolate_BHRZ03(c, [A >= 1, B >= 0], [A >= 2, B >= 1],
                              [A >= 1, B >= 0], [A >= 1, B >= 0]
                          ),
  bound_extrapolate_BHRZ03(c, [A >= 1, B >= 0], [A >= 2, B >= 1],
                              [A >= 2],         [A >= 1, B >= 0]
                          ),
  bound_extrapolate_BHRZ03(nnc, [A > 1, B > 0], [A > 2, B > 1],
                                [A > 1, B > 0], [A > 1, B > 0]
                          ),
  bound_extrapolate_BHRZ03(nnc, [A > 1, B >= 0], [A > 2, B >= 1],
                                [A >= 2],        [A > 1, B >= 0]
                          ).

bound_extrapolate_BHRZ03(Topology, CS_P, CS_Q, CS_lim, CS_Pa)  :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign(P, Q, CS_lim),
  widen_extrapolation_final(P, CS_Pa, Topology),
  !,
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens/5.
bound_extrapolate_BHRZ03_with_tokens :-
  make_vars(2, [A, B]),
  bound_extrapolate_BHRZ03_with_tokens(c,
                            [A >= 1, B >= 0], [A >= 1, B >= 1],
                            [A >= 1, B >= 0], [A >= 1, B >= 0], 1, 0
                                     ),
  bound_extrapolate_BHRZ03_with_tokens(nnc,
                            [A > 1, B > 0], [A > 1, B > 1],
                            [A > 1, B > 0], [A > 1, B > 0], 1, 0
                                     ).

bound_extrapolate_BHRZ03_with_tokens(Topology,
                 CS_P, CS_Q, CS_lim, CS_Pa, Token_i, Token_o) :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  Wrong_Token is Token_i + 1,
  \+ ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens(P, Q,
                                         CS_lim, Token_i, Wrong_Token),
  \+ ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens(P, Q,
                                         CS_lim, Token_i, not-a_number),
  ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens(P, Q,
                                         CS_lim, Token_i, X),
  ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens(P, Q,
                                         CS_lim, X, Y),
  Y == Token_o,
  widen_extrapolation_final(P, CS_Pa, Topology),
  !,
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_H79_widening_assign/2.
widen_H79 :-
  make_vars(2, [A, B]),
  widen_H79(c,   [A >= 1, B >= 0], [A >= 1, B >= 1],
                 [A >= 1],         [A >= 1, B >= 1]
           ),
  widen_H79(nnc, [A > 1, B > 0], [A > 1, B > 1],
                 [A > 1], [A > 1, B > 1]
           ),
  widen_H79(c,   [A >= 0, A =< 1], [A = 0],
                 [A >= 0],         [A = 0]
           ),
  widen_H79(nnc, [A >= 0, A =< 1], [A = 0],
                 [A >= 0],         [A = 0]
           ).

widen_H79(Topology, CS_P, CS_Q, CS_Pa, CS_Qa) :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  ppl_Polyhedron_H79_widening_assign(P, Q),
  widen_extrapolation_final(P, CS_Pa, Topology),
  widen_extrapolation_final(Q, CS_Qa, Topology).

% Tests ppl_Polyhedron_H79_widening_assign_with_tokens/4.
widen_H79_with_tokens :-
  make_vars(2, [A, B]),
  widen_H79_with_tokens(c, [A >= 1], [A >= 1, B >= 1],
                             [A >= 1], [A >= 1, B >= 1], 1, 1
                         ),
  widen_H79_with_tokens(c, [A >= 1, B >= 0], [A >= 1, B >= 1],
                             [A >= 1], [A >= 1, B >= 1], 1, 0
                         ),
  widen_H79_with_tokens(nnc, [A > 1], [A > 1, B > 1],
                               [A > 1], [A > 1, B > 1], 2, 2
                         ),
  widen_H79_with_tokens(nnc, [A > 1, B >= 0], [A > 1, B >= 1],
                               [A > 1, B >= 0], [A > 1, B >= 1], 3, 1
                         ).

widen_H79_with_tokens(Topology,
                      CS_P, CS_Q, CS_Pa, CS_Qa, Token_i, Token_o) :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  \+ ppl_Polyhedron_H79_widening_assign_with_tokens(P, Q,
                                            Token_i, 4),
  \+ ppl_Polyhedron_H79_widening_assign_with_tokens(P, Q,
                                            Token_i, not_a_number),
  ppl_Polyhedron_H79_widening_assign_with_tokens(P, Q, Token_i, X),
  ppl_Polyhedron_H79_widening_assign_with_tokens(P, Q, X, Y),
  Y == Token_o,
  widen_extrapolation_final(P, CS_Pa, Topology),
  widen_extrapolation_final(Q, CS_Qa, Topology), !.

% Tests ppl_Polyhedron_limited_H79_extrapolation_assign/3.
lim_extrapolate_H79 :-
  make_vars(2, [A, B]),
  lim_extrapolate_H79(c,   [A >= 1, B >= 0], [A >= 2, B >= 1],
                           [A >= 1, B >= 0], [A >= 1, B >= 0]
                     ),
  lim_extrapolate_H79(c,   [A >= 1, B >= 0], [A >= 2, B >= 1],
                           [A >= 2],         []
                     ),
  lim_extrapolate_H79(nnc, [A > 1, B > 0], [A > 2, B > 1],
                           [A > 1, B > 0], [A > 1, B > 0]
                     ),
  lim_extrapolate_H79(nnc, [A > 1, B >= 0], [A > 2, B >= 1],
                           [A >= 2],        []
                     ),
  lim_extrapolate_H79(c,   [A >= 0, A =< 1], [A = 0],
                           [A >= 0], [A >= 0]
                     ),
  lim_extrapolate_H79(nnc, [A >= 0, A =< 1], [A = 0],
                           [A >= 0], [A >= 0]
                    ).

lim_extrapolate_H79(Topology, CS_P, CS_Q, CS_lim, CS_Pa)  :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  ppl_Polyhedron_limited_H79_extrapolation_assign(P, Q, CS_lim),
  widen_extrapolation_final(P, CS_Pa, Topology),
  !,
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens/5.
lim_extrapolate_H79_with_tokens :-
  make_vars(2, [A, B]),
  lim_extrapolate_H79_with_tokens(c,
                  [A >= 1, B >= 0], [A >= 1, B >= 1],
                  [A >= 1, B >= 0], [A >= 1, B >= 0], 1, 0
                                   ),
  lim_extrapolate_H79_with_tokens(nnc,
                    [A > 1, B > 0], [A > 1, B > 1],
                    [A > 1, B > 0], [A > 1, B > 0], 1, 0
                                   ).

lim_extrapolate_H79_with_tokens(Topology,
                 CS_P, CS_Q, CS_lim, CS_Pa, Token_i, Token_o) :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  Wrong_Token is Token_i + 1,
  \+ ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens(P, Q,
                                            CS_lim, Token_i, Wrong_Token),
  \+ ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens(P, Q,
                                            CS_lim, Token_i, not_a_number),
  ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens(P, Q,
                                            CS_lim, Token_i, X),
  ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens(P, Q,
                                            CS_lim, X, Y),
  Y == Token_o,
  widen_extrapolation_final(P, CS_Pa, Topology),
  !,
  ppl_delete_Polyhedron(Q).


% Tests ppl_Polyhedron_bounded_H79_extrapolation_assign/3.
bound_extrapolate_H79 :-
  make_vars(2, [A, B]),
  bound_extrapolate_H79(c,   [A >= 1, B >= 0], [A >= 2, B >= 1],
                             [A >= 1, B >= 0], [A >= 1, B >= 0]
                       ),
  bound_extrapolate_H79(c,   [A >= 1, B >= 0], [A >= 2, B >= 1],
                             [A >= 2],         [A >= 1, B >= 0]
                       ),
  bound_extrapolate_H79(nnc, [A > 1, B > 0], [A > 2, B > 1],
                             [A > 1, B > 0], [A > 1, B > 0]
                       ),
  bound_extrapolate_H79(nnc, [A > 1, B >= 0], [A > 2, B >= 1],
                             [A >= 2],        [A > 1, B >= 0]
                       ).

bound_extrapolate_H79(Topology, CS_P, CS_Q, CS_lim, CS_Pa)  :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  ppl_Polyhedron_bounded_H79_extrapolation_assign(P, Q, CS_lim),
  widen_extrapolation_final(P, CS_Pa, Topology),
  !,
  ppl_delete_Polyhedron(Q).


% Tests ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens/5.
bound_extrapolate_H79_with_tokens :-
  make_vars(2, [A, B]),
  bound_extrapolate_H79_with_tokens(c,
                            [A >= 1, B >= 0], [A >= 1, B >= 1],
                            [A >= 1, B >= 0], [A >= 1, B >= 0], 1, 0
                                     ),
  bound_extrapolate_H79_with_tokens(nnc,
                            [A > 1, B > 0], [A > 1, B > 1],
                            [A > 1, B > 0], [A > 1, B > 0], 1, 0
                                     ).

bound_extrapolate_H79_with_tokens(Topology,
                 CS_P, CS_Q, CS_lim, CS_Pa, Token_i, Token_o) :-
  widen_extrapolation_init(P, CS_P, Topology),
  widen_extrapolation_init(Q, CS_Q, Topology),
  Wrong_Token is Token_i + 1,
  \+ ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens(P, Q,
                                         CS_lim, Token_i, Wrong_Token),
  \+ ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens(P, Q,
                                         CS_lim, Token_i, not_a_number),
  ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens(P, Q,
                                         CS_lim, Token_i, X),
  ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens(P, Q,
                                         CS_lim, X, Y),
  Y == Token_o,
  widen_extrapolation_final(P, CS_Pa, Topology),
  !,
  ppl_delete_Polyhedron(Q).

% widen_extrapolation_init/3 and widen_extrapolation_final/3
% are used in the tests for widening and extrapolation predicates.
widen_extrapolation_init(P, CS, Topology):-
  clean_ppl_new_Polyhedron_from_space_dimension(Topology, 2, universe, P),
  ppl_Polyhedron_add_constraints(P, CS).

widen_extrapolation_final(P,CS, Topology):-
  clean_ppl_new_Polyhedron_from_space_dimension(Topology, 2, universe, P1),
  ppl_Polyhedron_add_constraints(P1, CS),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(P1).

%%%%%%%%%%%%%%%%%% Get Constraint or Generator System %%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_get_constraints/2.
get_cons :-
  get_cons(c), get_cons(nnc).

get_cons(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_get_constraints(P, []),
  ppl_Polyhedron_add_constraint(P, A - B >= 1),
  \+  ppl_Polyhedron_get_constraints(P, []),
  ppl_Polyhedron_get_constraints(P, [C]),
  clean_ppl_new_Polyhedron_from_constraints(T, [C], Q),
  ppl_Polyhedron_equals_Polyhedron(P, Q),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_get_minimized_constraints/2.
get_min_cons :-
  get_min_cons(c), get_min_cons(nnc).

get_min_cons(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, universe, P),
  ppl_Polyhedron_get_minimized_constraints(P, []),
  ppl_Polyhedron_add_constraints(P, [A - B >= 1, A - B >= 0]),
  ppl_Polyhedron_get_minimized_constraints(P, [C]),
  clean_ppl_new_Polyhedron_from_constraints(T, [C], Q),
  ppl_Polyhedron_equals_Polyhedron(P, Q),
  ppl_Polyhedron_add_constraints(P, [A - B =< 0]),
  \+ppl_Polyhedron_get_minimized_constraints(P, [C]),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_get_generators/2.
get_gens :-
  get_gens(c), get_gens(nnc).

get_gens(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, empty, P),
  ppl_Polyhedron_get_generators(P, []),
  \+ ppl_Polyhedron_get_generators(P, [_]),
  ppl_Polyhedron_add_generator(P, point(A+B)),
  ppl_Polyhedron_get_generators(P, [G]),
  clean_ppl_new_Polyhedron_from_generators(T, [G], Q),
  ppl_Polyhedron_equals_Polyhedron(P, Q),
  ppl_Polyhedron_add_generator(P, point(A+B, 2)),
  ppl_Polyhedron_get_generators(P, GS1),
  ppl_Polyhedron_add_generators(Q, GS1),
  ppl_Polyhedron_equals_Polyhedron(P, Q),
  ppl_Polyhedron_add_generator(P, line(A)),
  ppl_Polyhedron_get_generators(P, GS2),
  ppl_Polyhedron_add_generators(Q, GS2),
  ppl_Polyhedron_equals_Polyhedron(P, Q),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q).

% Tests ppl_Polyhedron_get_minimized_generators/2.
get_min_gens :-
  get_min_gens(c), get_min_gens(nnc).

get_min_gens(T) :-
  make_vars(2, [A, B]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 2, empty, P),
  ppl_Polyhedron_add_generators(P, [point(2*A), point(A+B), point(2*B)]),
  \+ ppl_Polyhedron_get_minimized_generators(P, [_]),
  ppl_Polyhedron_get_minimized_generators(P, [G1, G2]),
  clean_ppl_new_Polyhedron_from_generators(T, [G1, G2], Q),
  ppl_Polyhedron_equals_Polyhedron(P, Q),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q).


%%%%%%%%%%%%%%%%%% Polyhedral Relations %%%%%%%%%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_relation_with_constraint/3.
rel_cons :-
  make_vars(3, [A, B, C]),
  rel_cons(c, [A >= 1, B >= 0, C = 0], [A, B, C]),
  rel_cons(nnc, [A > 1, B >= 0, C = 0], [A, B, C]).

rel_cons(T, CS, [A, B, C]) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  ppl_Polyhedron_add_constraints(P, CS),
  \+ ppl_Polyhedron_relation_with_constraint(P, A = 0, x),
  ppl_Polyhedron_relation_with_constraint(P, A = 0, R),
  R = [is_disjoint],
  ppl_Polyhedron_relation_with_constraint(P, B = 0, R1),
  R1 = [strictly_intersects],
  ppl_Polyhedron_relation_with_constraint(P, A >= 0, R2),
  R2 = [is_included],
  ppl_Polyhedron_relation_with_constraint(P, C >= 0, R3),
  (R3 = [is_included, saturates] ; R3 = [saturates, is_included]),
  ppl_Polyhedron_relation_with_constraint(P, A = B, R4),
  R4 = [strictly_intersects],
  ppl_Polyhedron_add_constraint(P, A = B),
  ppl_Polyhedron_relation_with_constraint(P, A = B, R5),
  (R5 = [is_included, saturates] ; R5 = [saturates, is_included]),
  !,
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_relation_with_generator/3.
rel_gens :-
  make_vars(3, [A, B, C]),
  rel_gens(c, [point(A + B + C), ray(A)], [A, B, C]),
  rel_gens(nnc, [point(A + B + C), ray(A)], [A, B, C]).

rel_gens(T, GS, [A, _, _]) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, empty, P),
  ppl_Polyhedron_add_generators(P, GS),
  \+ppl_Polyhedron_relation_with_generator(P, point(A), x),
  ppl_Polyhedron_relation_with_generator(P, point(A), R),
  R = [],
  ppl_Polyhedron_relation_with_generator(P, ray(A), R1),
  R1 = [subsumes],
  !,
  ppl_delete_Polyhedron(P).

%%%%%%%%%%%%%%%%%% Check Properties %%%%%%%%%%%%%%%%%%%%%%%%%%

%  tests ppl_Polyhedron_is_universe/1,
%        ppl_Polyhedron_is_empty/1,
%        ppl_Polyhedron_is_bounded/1,
%        ppl_Polyhedron_is_topologically_closed/1,
%        ppl_Polyhedron_contains_integer_point/2.
checks :-
  checks(c), checks(nnc).

checks(T) :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, empty, P1),
  ppl_Polyhedron_is_universe(P),
  ppl_Polyhedron_is_empty(P1),
  \+ppl_Polyhedron_is_universe(P1),
  \+ppl_Polyhedron_is_empty(P),
  \+ppl_Polyhedron_contains_integer_point(P1),
  ppl_Polyhedron_contains_integer_point(P),
  ppl_Polyhedron_add_generators(P1, [point(A + B + C, 2)]),
  \+ppl_Polyhedron_contains_integer_point(P1),
  ppl_Polyhedron_add_generators(P1, [point(A + B + C)]),
  ppl_Polyhedron_is_bounded(P1),
  ppl_Polyhedron_contains_integer_point(P1),
  ppl_Polyhedron_add_generators(P1, [ray(A + B + C)]),
  \+ ppl_Polyhedron_is_bounded(P1),
  ppl_Polyhedron_add_constraints(P, [A >= 1, B =< 3, A =< 2]),
  ppl_Polyhedron_contains_integer_point(P),
  ppl_Polyhedron_is_topologically_closed(P),
   (T = nnc ->
     (ppl_Polyhedron_add_constraints(P, [A > 1, B =< 3, A =< 2]),
      \+ ppl_Polyhedron_is_topologically_closed(P),
      ppl_Polyhedron_contains_integer_point(P),
      ppl_Polyhedron_add_constraints(P, [A > 2]),
      ppl_Polyhedron_is_topologically_closed(P))
   ; true
   ),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(P1).

% Tests ppl_Polyhedron_contains_Polyhedron/2.
contains :-
  contains(c), contains(nnc).

contains(T) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P1),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, empty, P2),
  ppl_Polyhedron_contains_Polyhedron(P1, P2),
  \+ppl_Polyhedron_contains_Polyhedron(P2, P1),
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2).

% Tests ppl_Polyhedron_strictly_contains_Polyhedron for C/2.
strict_contains :-
  strict_contains(c), strict_contains(nnc).

strict_contains(T) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P1),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, empty, P2),
  ppl_Polyhedron_strictly_contains_Polyhedron(P1, P2),
  \+ppl_Polyhedron_strictly_contains_Polyhedron(P1, P1),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2).

% Tests ppl_Polyhedron_is_disjoint_from_Polyhedron/2.
disjoint_from :-
  disjoint_from(c), disjoint_from(nnc).

disjoint_from(T) :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [3 >= A, 4*A + B - 2*C >= 5],
                                      P1),
  clean_ppl_new_Polyhedron_from_constraints(T,
                                      [4 =< A, 4*A + B - 2*C >= 5],
                                      P2),
  ppl_Polyhedron_is_disjoint_from_Polyhedron(P1, P2),
  \+ppl_Polyhedron_is_disjoint_from_Polyhedron(P1, P1),
  (T = nnc ->
     (clean_ppl_new_Polyhedron_from_constraints(nnc,
                                      [3 < A, 4*A + B - 2*C >= 5],
                                      P2a),
    ppl_Polyhedron_is_disjoint_from_Polyhedron(P1, P2a),
    ppl_delete_Polyhedron(P2a))
  ; true
  ),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2).

% Tests ppl_Polyhedron_equals_Polyhedron/2.
equals :-
  equals(c), equals(nnc).

equals(T) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P1),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P2),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, empty, P3),
  ppl_Polyhedron_equals_Polyhedron(P1, P2),
  \+ ppl_Polyhedron_equals_Polyhedron(P1, P3),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P3).

% Tests ppl_Polyhedron_OK/1.
ok :-
  ok(c), ok(nnc).

ok(T) :-
  clean_ppl_new_Polyhedron_from_space_dimension(T, 0, universe, P1),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, universe, P2),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 0, empty, P3),
  clean_ppl_new_Polyhedron_from_space_dimension(T, 3, empty, P4),
  ppl_Polyhedron_OK(P1),
  ppl_Polyhedron_OK(P2),
  ppl_Polyhedron_OK(P3),
  ppl_Polyhedron_OK(P4),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2),
  ppl_delete_Polyhedron(P3),
  ppl_delete_Polyhedron(P4).

% Tests ppl_termination_test_C_Polyhedron_MS/2,
%       ppl_termination_test_C_Polyhedron_PR/2.
%       ppl_termination_test_NNC_Polyhedron_MS/2,
%       ppl_termination_test_NNC_Polyhedron_PR/2.
termination_test :-
  make_vars(4, [A, B, C, D]),
  clean_ppl_new_Polyhedron_from_constraints(c,
                                            [A - C >= 0,
                                             -A + C >= 0,
                                             -B + D >= 1,
                                             B >= 0,
                                             A >= 1], P1),
  ppl_termination_test_C_Polyhedron_MS(P1),
  ppl_termination_test_C_Polyhedron_PR(P1),
  clean_ppl_new_Polyhedron_from_constraints(nnc,
                                            [A - C >= 0,
                                             -A + C >= 0,
                                             -B + D >= 1,
                                             B >= 0],
                                            P2),
  ppl_termination_test_NNC_Polyhedron_MS(P2),
  ppl_termination_test_NNC_Polyhedron_PR(P2),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2).

%%%%%%%%%%%%%%%%%%%%%%%%% Polyhedron Bounding Values %%%%%%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_bounds_from_above/2.
bounds_from_above :-
  make_vars(2, [A, B]),
  bounds_from_above(c, [A >= 1, B >= 0], [B =< 2], B),
  bounds_from_above(nnc, [A > 1, B > 0, B < 1], [A < 2], A).

bounds_from_above(T, CS1, CS2, Var) :-
  clean_ppl_new_Polyhedron_from_constraints(T, CS1, P),
  \+ ppl_Polyhedron_bounds_from_above(P, Var),
  ppl_Polyhedron_add_constraints(P, CS2),
  ppl_Polyhedron_bounds_from_above(P, Var),
  !,
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_bounds_from_below/2.
bounds_from_below :-
  make_vars(2, [A, B]),
  bounds_from_below(c, [B >= 0, B =< 1], [A >= 1], A),
  bounds_from_below(nnc, [B > 0, B < 1], [A > 2], A).

bounds_from_below(T, CS1, CS2, Var) :-
  clean_ppl_new_Polyhedron_from_constraints(T, CS1, P),
  \+ ppl_Polyhedron_bounds_from_below(P, Var),
  ppl_Polyhedron_add_constraints(P, CS2),
  ppl_Polyhedron_bounds_from_below(P, Var),
  ppl_delete_Polyhedron(P).

%%%%%%%%%%%%%%%%%%%%%%%%% Maximize and Minimize %%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Tests ppl_Polyhedron_maximize/5.
maximize :-
  make_vars(2, [A, B]),
  maximize(c, [A >= -1, A =< 1, B >= -1, B =< 1], A + B, 2, 1, true),
  maximize(c, [B >= -1, B =< 1], B, 1, 1, true),
  maximize(nnc, [A > -1, A < 1, B > -1, B < 1], A + B -1, 1, 1, false),
  maximize(nnc, [B > -1, B < 1], B, 1, 1, false).

maximize(T, CS, LE, N, D, Max) :-
  clean_ppl_new_Polyhedron_from_constraints(T, CS, P),
  ppl_Polyhedron_maximize(P, LE, N, D, Max),
  ppl_Polyhedron_add_generator(P, ray(LE)),
  \+ ppl_Polyhedron_maximize(P, LE, _, _, _),
  !,
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_maximize_with_point/5.
maximize_with_point :-
  make_vars(2, [A, B]),
  maximize_with_point(c, [A >= -1, A =< 1, B >= -1, B =< 1],
                                        A + B, 2, 1, true, point(A+B)),
  maximize_with_point(c, [A =< 0],
                                        A, 0, 1, true, point(0)),
  maximize_with_point(nnc, [A > -1, A < 1, B > -1, B < 1],
                                        A + B -1, 1, 1, false, point(A+B)).

maximize_with_point(T, CS, LE, N, D, Max, Point) :-
  clean_ppl_new_Polyhedron_from_constraints(T, CS, P),
  ppl_Polyhedron_maximize_with_point(P, LE, N, D, Max, Point_Max),
  (Point_Max = closure_point(E) ; Point_Max = point(E)),
  clean_ppl_new_Polyhedron_from_generators(T, [point(E)], Pm),
  clean_ppl_new_Polyhedron_from_generators(T, [Point], Qm),
  ppl_Polyhedron_equals_Polyhedron(Pm, Qm),
  !,
  ppl_delete_Polyhedron(Pm),
  ppl_delete_Polyhedron(Qm),
  \+ ppl_Polyhedron_maximize_with_point(P, LE, _N, 0, _, _),
  !,
  ppl_delete_Polyhedron(P).


% Tests ppl_Polyhedron_minimize/5.
minimize :-
  make_vars(2, [A, B]),
  minimize(c, [A >= -1, A =< 1, B >= -1, B =< 1], A + B, -2, 1, true),
  minimize(c, [B >= -1, B =< 1], B, -1, 1, true),
  minimize(nnc, [A > -2, A =< 2, B > -2, B =< 2], A + B + 1, -3, 1, false),
  minimize(nnc, [B > -1, B < 1], B, -1, 1, false).

minimize(T, CS, LE, N, D, Min) :-
  clean_ppl_new_Polyhedron_from_constraints(T, CS, P),
  ppl_Polyhedron_minimize(P, LE, N, D, Min),
  ppl_Polyhedron_add_generator(P, ray(-LE)),
  \+ ppl_Polyhedron_minimize(P, LE, _, _, _),
  !,
  ppl_delete_Polyhedron(P).

% Tests ppl_Polyhedron_minimize_with_point/5.
minimize_with_point :-
  make_vars(2, [A, B]),
  minimize_with_point(c, [A >= -1, A =< 1, B >= -1, B =< 1],
                                        A + B, -2, 1, true, point(-A-B)),
  minimize_with_point(c, [A >= 0],
                                        A, 0, 1, true, point(0)),
  minimize_with_point(nnc, [A > -2, A =< 2, B > -2, B =< 2],
                                        A + B, -4, 1, false, point(-2*A-2*B)).

minimize_with_point(T, CS, LE, N, D, Min, Point) :-
  clean_ppl_new_Polyhedron_from_constraints(T, CS, P),
  ppl_Polyhedron_minimize_with_point(P, LE, N, D, Min, Point_Min),
  (Point_Min = closure_point(E) ; Point_Min = point(E)),
  clean_ppl_new_Polyhedron_from_generators(T, [point(E)], Pm),
  clean_ppl_new_Polyhedron_from_generators(T, [Point], Qm),
  ppl_Polyhedron_equals_Polyhedron(Pm, Qm),
  !,
  ppl_delete_Polyhedron(Pm),
  ppl_delete_Polyhedron(Qm),
   \+ ppl_Polyhedron_minimize_with_point(P, LE, _N, 0, _, _),
  !,
  ppl_delete_Polyhedron(P).

%%%%%%%%%%%%%%%%% Watchdog tests %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Tests Watchdog predicates
% ppl_set_timeout/1
% ppl_set_timeout_exception_atom/1
% ppl_timeout_exception_atom/1
% ppl_reset_timeout/0
%

time_out :-
  %% FIXME!
  %% Ciao does not throw a timeout exception.
  prolog_system('Ciao'), !.
time_out :-
  time_out(c), time_out(nnc).

add_constraints_and_get_minimized_constraints(P, CS) :-
    ppl_Polyhedron_add_constraints(P, CS),
    ppl_Polyhedron_get_minimized_constraints(P, _).

% Find the constraints for a hypercube for a given dimension.
build_hypercube_constraints(0, [], []).
build_hypercube_constraints(Dim, [V|Vars], [V >= 0, V =< 1|CS]) :-
    Dim1 is Dim - 1,
    build_hypercube_constraints(Dim1, Vars, CS).

% Find the dimension and constraints for
% a hypercube that causes a timeout exception.
compute_timeout_hypercube(Csecs, T, Dim_in, Dim_out, CS_out) :-
    Dim_in =< 100,
    clean_ppl_new_Polyhedron_from_space_dimension(T, Dim_in, universe, P),
    make_vars(Dim_in, Vars),
    build_hypercube_constraints(Dim_in, Vars, CS),
    ppl_timeout_exception_atom(Time_Out_Atom),
    catch((ppl_set_timeout(Csecs),
           add_constraints_and_get_minimized_constraints(P, CS)),
          Time_Out_Atom, Catch_Exception = ok),
    ppl_reset_timeout,
    (Catch_Exception == ok ->
        Dim_out = Dim_in,
        ppl_delete_Polyhedron(P),
        CS_out = CS
    ;
        Dim1 is Dim_in+1,
        ppl_delete_Polyhedron(P),
        compute_timeout_hypercube(Csecs, T, Dim1, Dim_out, CS_out)
    ).

time_out(T) :-
  ppl_set_timeout_exception_atom(pl_time_out),
  \+  ppl_timeout_exception_atom(pl_x),
  ppl_timeout_exception_atom(pl_time_out),
  compute_timeout_hypercube(10, T, 1, Dim, CS),
  !,
  N1 is 1,
  clean_ppl_new_Polyhedron_from_space_dimension(T, Dim, universe, P),
  catch((ppl_set_timeout(N1),
         add_constraints_and_get_minimized_constraints(P, CS)),
        pl_time_out, Catch_Exception = ok),
  ppl_reset_timeout,
  (Catch_Exception == ok ->
      display_message(['while testing time_out, polyhedron with topology',
                 T, 'timeout after', N1, ms])
  ;
      display_message(['while testing time_out, polyhedron with topology',
                 T, 'no timeout after', N1, ms]),
      fail
  ),
  ppl_Polyhedron_OK(P),
  !,
  ppl_delete_Polyhedron(P),
  N2 is 40,
  clean_ppl_new_Polyhedron_from_space_dimension(T, Dim, universe, Q),
  catch((ppl_set_timeout(N2),
         ppl_Polyhedron_is_universe(Q)),
        pl_time_out, Catch_Exception = not_ok),
  ppl_reset_timeout,
  (Catch_Exception == not_ok ->
      display_message(['while testing time_out, polyhedron with topology',
                 T, 'timeout after', N2, ms]),
      fail
  ;
      display_message(['while testing time_out, polyhedron with topology',
                 T, 'no timeout after', N2, ms])
  ),
  ppl_Polyhedron_OK(Q),
  !,
  ppl_delete_Polyhedron(Q).

%%%%%%%%%%%%%%%%% MIP_Problem tests %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

mip_problem :-
  mip_from_cons,
  mip_from_mip,
  mip_swap,
  mip_get,
  mip_control_parameters,
  mip_clear,
  mip_satisfiable,
  mip_add,
  mip_set,
  mip_solve,
  mip_eval.

mip_from_cons :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem(3, [A >= -1, B >= 5, C >= 0, C =< 3], C, max, MIP),
  ppl_MIP_Problem_space_dimension(MIP, 3),
  ppl_MIP_Problem_constraints(MIP, CS),
  ppl_MIP_Problem_objective_function(MIP, Obj),
  compare_lin_expressions(Obj, C),
  ppl_MIP_Problem_optimization_mode(MIP, max),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  clean_ppl_new_Polyhedron_from_constraints(c,
       [A >= -1, B >= 5, C >= 0, C =< 3], Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_MIP_Problem(MIP).

mip_from_mip :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem(
    3, [A >= -1, B >= 5, C >= 0, C =< 3], C, max, MIP1),
  clean_ppl_new_MIP_Problem_from_MIP_Problem(MIP1, MIP),
  ppl_MIP_Problem_objective_function(MIP, Obj),
  compare_lin_expressions(Obj, C),
  ppl_MIP_Problem_optimization_mode(MIP, max),
  ppl_MIP_Problem_constraints(MIP, CS),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  ppl_MIP_Problem_constraints(MIP1, Expect_CS),
  clean_ppl_new_Polyhedron_from_constraints(c, Expect_CS, Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_MIP_Problem(MIP1),
  ppl_delete_MIP_Problem(MIP).

mip_swap :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem(0, [], 0, max, MIP),
  clean_ppl_new_MIP_Problem(
    3, [A >= -1, B >= 5, C >= 0, C =< 3], C, max, MIP1),
  ppl_MIP_Problem_swap(MIP, MIP1),
  ppl_MIP_Problem_constraints(MIP, CS),
  ppl_MIP_Problem_constraints(MIP1, CS1),
  clean_ppl_new_Polyhedron_from_constraints(c, CS1, PH1),
  ppl_Polyhedron_is_universe(PH1),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  clean_ppl_new_Polyhedron_from_constraints(c,
       [A >= -1, B >= 5, C >= 0, C =< 3], Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(PH1),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_MIP_Problem(MIP1),
  ppl_delete_MIP_Problem(MIP).

mip_get :-
  make_vars(3, [A, B, C]),

  ppl_new_MIP_Problem(3, [], A + 3, min, MIP0),
  ppl_MIP_Problem_objective_function(MIP0, Obj0),
  compare_lin_expressions(Obj0, A + 3),

  ppl_new_MIP_Problem(3, [], 3, min, MIP1),
  ppl_MIP_Problem_objective_function(MIP1, Obj1),
  compare_lin_expressions(Obj1, 3),

  clean_ppl_new_MIP_Problem(3, [A >= -1, B >= 5, C >= 0, C =< 3], C, max, MIP),
  ppl_MIP_Problem_constraints(MIP, CS),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  clean_ppl_new_Polyhedron_from_constraints(c,
       [A >= -1, B >= 5, C >= 0, C =< 3], Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  ppl_MIP_Problem_objective_function(MIP, Obj),
  compare_lin_expressions(Obj, C),
  ppl_MIP_Problem_optimization_mode(MIP, Opt),
  Opt = max,
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_MIP_Problem(MIP0),
  ppl_delete_MIP_Problem(MIP1),
  ppl_delete_MIP_Problem(MIP).

mip_control_parameters :-
  make_vars(1, [A]),

  ppl_new_MIP_Problem(3, [], A + 3, min, MIP0),
  clean_ppl_new_MIP_Problem_from_MIP_Problem(MIP0, MIP1),
  ppl_MIP_Problem_get_control_parameter(MIP0, pricing, Cp_value0),
  ppl_MIP_Problem_set_control_parameter(MIP1, Cp_value0),
  ppl_MIP_Problem_get_control_parameter(MIP1, pricing, Cp_value1),
  Cp_value0 == Cp_value1,
  ppl_MIP_Problem_set_control_parameter(MIP0, pricing_steepest_edge_float),
  ppl_MIP_Problem_get_control_parameter(MIP0, pricing, Cp_value2),
  Cp_value2 == pricing_steepest_edge_float,
  ppl_MIP_Problem_set_control_parameter(MIP0, pricing_steepest_edge_exact),
  ppl_MIP_Problem_get_control_parameter(MIP0, pricing, Cp_value3),
  Cp_value3 == pricing_steepest_edge_exact,
  ppl_MIP_Problem_set_control_parameter(MIP0, pricing_textbook),
  ppl_MIP_Problem_get_control_parameter(MIP0, pricing, Cp_value4),
  Cp_value4 == pricing_textbook,
  !,
  ppl_delete_MIP_Problem(MIP0),
  ppl_delete_MIP_Problem(MIP1).

mip_clear :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem(3, [A >= -1, B >= 5, C >= 0, C =< 3], C, min, MIP),
  ppl_MIP_Problem_clear(MIP),
  ppl_MIP_Problem_space_dimension(MIP, D),
  D == 0,
  ppl_MIP_Problem_constraints(MIP, CS),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  ppl_Polyhedron_is_universe(PH),
  ppl_MIP_Problem_objective_function(MIP, Obj),
  compare_lin_expressions(Obj, 0),
  ppl_MIP_Problem_optimization_mode(MIP, Opt),
  Opt == max,
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_MIP_Problem(MIP).

mip_satisfiable :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem(3, [A >= -1, B >= 5, C >= 0, C =< 3], C, max, MIP),
  ppl_MIP_Problem_is_satisfiable(MIP),
  ppl_MIP_Problem_add_constraint(MIP, A + B =< 0),
  \+ ppl_MIP_Problem_is_satisfiable(MIP),
  !,
  ppl_delete_MIP_Problem(MIP).

mip_add :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem_from_space_dimension(0, MIP),
  ppl_MIP_Problem_add_space_dimensions_and_embed(MIP, 1),
  ppl_MIP_Problem_add_constraint(MIP, A >= 0),
  ppl_MIP_Problem_add_space_dimensions_and_embed(MIP, 2),
  ppl_MIP_Problem_add_constraints(
    MIP,[A =< 3, A + B + C >= 9, B >= 5, C =< 5]),
  clean_ppl_new_MIP_Problem(
    3, [A >= 0, A =< 3, A + B + C >= 9, B >= 5, C =< 5], 2*B-C, max, MIP1),
  ppl_MIP_Problem_solve(MIP, Status),
  Status == optimized,
  ppl_MIP_Problem_solve(MIP1, Status1),
  Status1 == unbounded,
  ppl_MIP_Problem_optimal_value(MIP, N, D),
  N == 0,
  D == 1,
  ppl_MIP_Problem_constraints(MIP, CS),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  ppl_MIP_Problem_constraints(MIP1, Expect_CS),
  clean_ppl_new_Polyhedron_from_constraints(c, Expect_CS, Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_MIP_Problem(MIP),
  ppl_delete_MIP_Problem(MIP1).

mip_set :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem(
    3, [A >= 0, A =< 3, A + B + C >= 9, B >= 5, C =< 5], 0, max, MIP),
  ppl_MIP_Problem_objective_function(MIP, 0),
  ppl_MIP_Problem_optimization_mode(MIP, max),
  ppl_MIP_Problem_set_objective_function(MIP, 2*B-C),
  ppl_MIP_Problem_set_optimization_mode(MIP, min),
  ppl_MIP_Problem_objective_function(MIP, Obj),
  compare_lin_expressions(Obj, 2*B-C),
  ppl_MIP_Problem_optimization_mode(MIP, min),
  ppl_MIP_Problem_solve(MIP, optimized),
  !,
  ppl_delete_MIP_Problem(MIP).

mip_solve :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem(
    3, [A >= 0, A =< 3, A + B + C >= 9, B >= 5, C =< 5], 0, max, MIP),
  ppl_MIP_Problem_objective_function(MIP, 0),
  ppl_MIP_Problem_optimization_mode(MIP, max),
  ppl_MIP_Problem_set_objective_function(MIP, 2*B-C),
  ppl_MIP_Problem_set_optimization_mode(MIP, min),
  ppl_MIP_Problem_solve(MIP, optimized),
  ppl_MIP_Problem_set_objective_function(MIP, C),
  ppl_MIP_Problem_solve(MIP, unbounded),
  ppl_MIP_Problem_add_constraint(MIP, B = 0),
  ppl_MIP_Problem_solve(MIP, unfeasible),
  \+ppl_MIP_Problem_solve(MIP, invalid_status),
  !,
  ppl_delete_MIP_Problem(MIP).

mip_eval :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_MIP_Problem(
    3, [A >= 0, A =< 3, A + B + C >= 9, B >= 5, C =< 5], 2*B-C, min, MIP),
  \+ ppl_MIP_Problem_optimizing_point(MIP, closure_point(_X)),
  ppl_MIP_Problem_optimizing_point(MIP, Point),
  ppl_MIP_Problem_feasible_point(MIP, Point),
  \+ ppl_MIP_Problem_feasible_point(MIP, point(B)),
  clean_ppl_new_Polyhedron_from_generators(c, [Point], PH),
  clean_ppl_new_Polyhedron_from_generators(c, [point(5*B+5*C)], Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  \+ ppl_MIP_Problem_optimal_value(MIP, 2, 1),
  ppl_MIP_Problem_optimal_value(MIP, N, D),
  \+ ppl_MIP_Problem_evaluate_objective_function(MIP, Point, 2, 1),
  ppl_MIP_Problem_evaluate_objective_function(MIP, Point, N1, D1),
  N == N1,
  D == D1,
  ppl_MIP_Problem_OK(MIP),
  !,
  ppl_delete_MIP_Problem(MIP),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_Polyhedron(PH).

%%%%%%%%%%%%%%%%% PIP_Problem tests %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pip_problem :-
  pip_from_cons,
  pip_from_pip,
  pip_swap,
  pip_control_parameters,
  pip_clear,
  pip_satisfiable,
  pip_add,
  pip_set,
  pip_solve,
  pip_solution.

pip_from_cons :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_PIP_Problem_from_space_dimension(3, PIP1),
  ppl_PIP_Problem_space_dimension(PIP1, 3),
  ppl_PIP_Problem_OK(PIP1),
  clean_ppl_new_PIP_Problem(3, [A >= -1, B >= 5, C >= 0, C =< 3], [C], PIP),
  ppl_PIP_Problem_space_dimension(PIP, 3),
  ppl_PIP_Problem_parameter_space_dimensions(PIP, [C]),
  ppl_PIP_Problem_constraints(PIP, CS),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  clean_ppl_new_Polyhedron_from_constraints(c,
       [A >= -1, B >= 5, C >= 0, C =< 3], Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_PIP_Problem(PIP1),
  ppl_delete_PIP_Problem(PIP).

pip_from_pip :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_PIP_Problem(
    3, [A >= -1, B >= 5, C >= 0, C =< 3], [C], PIP1),
  clean_ppl_new_PIP_Problem_from_PIP_Problem(PIP1, PIP),
  ppl_PIP_Problem_constraints(PIP, CS),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  ppl_PIP_Problem_constraints(PIP1, Expect_CS),
  clean_ppl_new_Polyhedron_from_constraints(c, Expect_CS, Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_PIP_Problem(PIP1),
  ppl_delete_PIP_Problem(PIP).

pip_swap :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_PIP_Problem_from_space_dimension(0, PIP),
  clean_ppl_new_PIP_Problem(
    3, [A >= -1, B >= 5, C >= 0, C =< 3], [C], PIP1),
  ppl_PIP_Problem_swap(PIP, PIP1),
  ppl_PIP_Problem_constraints(PIP, CS),
  ppl_PIP_Problem_constraints(PIP1, CS1),
  clean_ppl_new_Polyhedron_from_constraints(c, CS1, PH1),
  ppl_Polyhedron_is_universe(PH1),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  clean_ppl_new_Polyhedron_from_constraints(c,
       [A >= -1, B >= 5, C >= 0, C =< 3], Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(PH1),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_PIP_Problem(PIP1),
  ppl_delete_PIP_Problem(PIP).

pip_control_parameters :-
  make_vars(1, [A]),

  ppl_new_PIP_Problem(3, [], [A], PIP0),
  clean_ppl_new_PIP_Problem_from_PIP_Problem(PIP0, PIP1),
  ppl_PIP_Problem_get_control_parameter(PIP0, cutting_strategy, Cp_value0),
  ppl_PIP_Problem_set_control_parameter(PIP1, Cp_value0),
  ppl_PIP_Problem_get_control_parameter(PIP1, cutting_strategy, Cp_value1),
  Cp_value0 == Cp_value1,
  ppl_PIP_Problem_set_control_parameter(PIP0, cutting_strategy_first),
  ppl_PIP_Problem_get_control_parameter(PIP0, cutting_strategy, Cp_value2),
  Cp_value2 == cutting_strategy_first,
  ppl_PIP_Problem_set_control_parameter(PIP0, cutting_strategy_deepest),
  ppl_PIP_Problem_get_control_parameter(PIP0, cutting_strategy, Cp_value3),
  Cp_value3 == cutting_strategy_deepest,
  ppl_PIP_Problem_set_control_parameter(PIP0, cutting_strategy_all),
  ppl_PIP_Problem_get_control_parameter(PIP0, cutting_strategy, Cp_value4),
  Cp_value4 == cutting_strategy_all,

  ppl_PIP_Problem_get_control_parameter(PIP0, pivot_row_strategy, Cp_value5),
  ppl_PIP_Problem_set_control_parameter(PIP1, Cp_value5),
  ppl_PIP_Problem_get_control_parameter(PIP1, pivot_row_strategy, Cp_value6),
  Cp_value5 == Cp_value6,
  ppl_PIP_Problem_set_control_parameter(PIP0, pivot_row_strategy_first),
  ppl_PIP_Problem_get_control_parameter(PIP0, pivot_row_strategy, Cp_value7),
  Cp_value7 == pivot_row_strategy_first,
  ppl_PIP_Problem_set_control_parameter(PIP0, pivot_row_strategy_max_column),
  ppl_PIP_Problem_get_control_parameter(PIP0, pivot_row_strategy, Cp_value8),
  Cp_value8 == pivot_row_strategy_max_column,
 !,
  ppl_delete_PIP_Problem(PIP0),
  ppl_delete_PIP_Problem(PIP1).

pip_clear :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_PIP_Problem(3, [A >= -1, B >= 5, C >= 0, C =< 3], [C], PIP),
  ppl_PIP_Problem_clear(PIP),
  ppl_PIP_Problem_space_dimension(PIP, D),
  D == 0,
  ppl_PIP_Problem_constraints(PIP, CS),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  ppl_Polyhedron_is_universe(PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_PIP_Problem(PIP).

pip_satisfiable :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_PIP_Problem(3, [A >= -1, B >= 5, C >= 0, C =< 3], [C], PIP),
  ppl_PIP_Problem_is_satisfiable(PIP),
  ppl_PIP_Problem_add_constraint(PIP, A + B =< 0),
  \+ ppl_PIP_Problem_is_satisfiable(PIP),
  !,
  ppl_delete_PIP_Problem(PIP).

pip_add :-
  make_vars(4, [A, B, C, D]),
  clean_ppl_new_PIP_Problem_from_space_dimension(0, PIP),
  ppl_PIP_Problem_add_space_dimensions_and_embed(PIP, 1, 0),
  ppl_PIP_Problem_add_constraint(PIP, A >= 0),
  ppl_PIP_Problem_add_space_dimensions_and_embed(PIP, 2, 1),
  ppl_PIP_Problem_add_constraints(
    PIP, [A =< 3, A + B + C >= 9, B >= 5, C =< 5]),
  clean_ppl_new_PIP_Problem(
    4, [A >= 0, A =< 3, A + B + C >= 9, B >= 5, C =< 5], [D], PIP1),
  ppl_PIP_Problem_constraints(PIP, CS),
  clean_ppl_new_Polyhedron_from_constraints(c, CS, PH),
  ppl_PIP_Problem_constraints(PIP1, Expect_CS),
  clean_ppl_new_Polyhedron_from_constraints(c, Expect_CS, Expect_PH),
  ppl_Polyhedron_equals_Polyhedron(PH, Expect_PH),
  !,
  ppl_delete_Polyhedron(PH),
  ppl_delete_Polyhedron(Expect_PH),
  ppl_delete_PIP_Problem(PIP),
  ppl_delete_PIP_Problem(PIP1).

pip_set :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_PIP_Problem(
    3, [A >= 0, A =< 3, A + B + C >= 9, B >= 5, C =< 5], [B], PIP),
  \+ ppl_PIP_Problem_has_big_parameter_dimension(PIP, _X),
  ppl_PIP_Problem_solve(PIP, optimized),
  !,
  ppl_delete_PIP_Problem(PIP),

  make_vars(4, [X, Y, P, M]),
  clean_ppl_new_PIP_Problem(
    4, [Y - M >= -2*X + 2*M - 4, 2*Y - 2*M =< X - M + 2*P], [P, M], PIP1),
  ppl_PIP_Problem_set_big_parameter_dimension(PIP1, 3),
  ppl_PIP_Problem_has_big_parameter_dimension(PIP1, 3),
  ppl_PIP_Problem_solve(PIP1, optimized),
  !,
  ppl_delete_PIP_Problem(PIP1).

pip_solve :-
  make_vars(3, [A, B, C]),
  clean_ppl_new_PIP_Problem(
    3, [A >= 0, A =< 3, A + B + C >= 9, B >= 5, C =< 5], [B], PIP),
  ppl_PIP_Problem_solve(PIP, Status),
  Status == optimized,
  clean_ppl_new_PIP_Problem(
    3, [A >= 0, A =< 3, A + B + C >= 9, B >= 5, C =< 5], [B,C], PIP1),
  ppl_PIP_Problem_add_constraint(PIP1, C >= 6),
  ppl_PIP_Problem_solve(PIP1, Status1),
  Status1 == unfeasible,
  ppl_PIP_Problem_add_constraint(PIP, B = 0),
  ppl_PIP_Problem_solve(PIP, unfeasible),
  \+ppl_PIP_Problem_solve(PIP, invalid_status),
  !,
  ppl_delete_PIP_Problem(PIP),
  ppl_delete_PIP_Problem(PIP1).

pip_solution :-
  make_vars(4, [I, J, M, N]),
  clean_ppl_new_PIP_Problem(
    4, [3*J >= -2*I + 8, J =< 4*I - 4, J =< M, I =< N], [M,N], PIP),
  ppl_PIP_Problem_solution(PIP, Node),
  ppl_PIP_Tree_Node_artificials(Node, Artificials),
  Artificials = [],
  \+ ppl_PIP_Tree_Node_is_solution(Node),
  ppl_PIP_Tree_Node_constraints(Node, _CS),
  ppl_PIP_Tree_Node_is_decision(Node),
  ppl_PIP_Tree_Node_constraints(Node, _CS1),
  ppl_PIP_Tree_Node_true_child(Node, TChild),
  ppl_PIP_Tree_Node_false_child(Node, _FChild),
  ppl_PIP_Tree_Node_is_decision(TChild),
  ppl_PIP_Tree_Node_true_child(TChild, TTChild),
  \+ ppl_PIP_Tree_Node_is_decision(TTChild),
  ppl_PIP_Tree_Node_is_solution(TTChild),
  ppl_PIP_Tree_Node_parametric_values(TTChild, I, _TPV),
  ppl_PIP_Tree_Node_false_child(TChild, FTChild),
  ppl_PIP_Tree_Node_artificials(FTChild, FTChild_Artificials),
  FTChild_Artificials = [Art_LinExpr/Art_Den],
  compare_lin_expressions(Art_LinExpr, M),
  Art_Den = 2,
  ppl_PIP_Tree_Node_is_solution(FTChild),
  ppl_PIP_Tree_Node_parametric_values(FTChild, I, _FPV),
  ppl_PIP_Problem_OK(PIP),
  ppl_PIP_Tree_Node_OK(Node),
  !,
  ppl_delete_PIP_Problem(PIP).

%%%%%%%%%%%%%%%% Tool to compare linear expressions %%%%%%%%%%%%%%%%%%%%%%%

% compare_lin_expressions/2 checks if 2 linear expressions
% are semantically the same.
%
% If we need to compare 2 linear expressions, then this is better
% than a syntactic check- since we want 1*C equal to C.

compare_lin_expressions(LE1, LE2) :-
  clean_ppl_new_Polyhedron_from_constraints(c, [LE1 = 0], PH1),
  clean_ppl_new_Polyhedron_from_constraints(c, [LE2 = 0], PH2),
  ppl_Polyhedron_equals_Polyhedron(PH1, PH2),
  !,
  ppl_delete_Polyhedron(PH1),
  ppl_delete_Polyhedron(PH2).

%%%%%%%%%%%%%%%% Check C++ <--> Prolog numbers %%%%%%%%%%%%%%%%%%%%%%%

/*
 This test checks the transfer of large numbers between Prolog and C++.
 We test all numbers (BigNum) which are +/- (2^E +/- A) where E is one of
 the numbers in the list defined by large_integers_exponents/1 and
 A is one of the numbers in the list defined by large_integers_additions/1.

 Thus we pass a BigNum from the Prolog to C++ and construct a polyhedron
 P (space dimension = 1) consisting of a single point A = BigNum.
 We also get the constraint defining P and then construct a second
 polyhedron P1 from this constraint; P is then compared with P1.
 To ensure that errors from Prolog to C++ and C++ to Prolog do not cancel
 each other out, we also construct a polyhedron P2 consisting of just
 the point A = 1 and use affine transformations (on polyhedra) to change P2
 to a polyhedron with the point A = BigNum; then P2 is compared with P.

 To see exactly which numbers are tested, first make the test "extra noisy"
 using make_extra_noisy/0; i.e., type:
 make_extra_noisy, large_integers.
*/

large_integers_exponents([0, 7, 8, 15, 16, 27, 28, 29, 30, 31, 32, 63, 64]).

large_integers_additions([-3, -2, -1, 0, 1, 2, 3]).

large_integers :-
  large_integers_exponents(Exps),
  large_integers_additions(Adds),
  out(large_int, init),
  pl_check_prolog_flag(bounded, Y),
  (Y == true ->
     large_integers_prolog_cpp_bounded(Exps, Adds, 0),
     out(sys_large_int, init),
     large_integers_sys_prolog_cpp(Adds)
   ;
     large_integers_prolog_cpp_unbounded(Exps, Adds)
  ).

large_integers_prolog_cpp_bounded([], _, _).
large_integers_prolog_cpp_bounded([Exp|Exps], Adds, Prev_value) :-
  /* If the test value is too large, it may be wrap.
     So we compare it with the previous value that was ok
     as well as checking it against the maximum value. */
  Test_value is 1 << Exp + 3,
  ( ( Test_value =< Prev_value ;
      (pl_check_prolog_flag(max_integer, Max_int),
         Max_int >> 1 =< Test_value)
    ) ->
     true
   ;
     large_integers_prolog_cpp1(Adds, Exp),
     large_integers_prolog_cpp_bounded(Exps, Adds, Test_value)
  ).

large_integers_prolog_cpp_unbounded([], _).
large_integers_prolog_cpp_unbounded([Exp|Exps], Adds) :-
   large_integers_prolog_cpp1(Adds, Exp),
   large_integers_prolog_cpp_unbounded(Exps, Adds).

large_integers_prolog_cpp1([], _).
large_integers_prolog_cpp1([Add|Adds], Exp) :-
  large_integers_prolog_cpp2(Exp, Add, 1),
  large_integers_prolog_cpp2(Exp, Add, -1),
  large_integers_prolog_cpp1(Adds, Exp).

large_integers_prolog_cpp2(Exp, Add, Sign) :-
  Inhomo is Sign * ((1 << Exp) + Add),
  out(large_int, Inhomo, Sign, Add, Exp),
  make_vars(1, [A]),
  clean_ppl_new_Polyhedron_from_space_dimension(c, 1, universe, P),
  ppl_Polyhedron_add_constraints(P, [A = Inhomo]),
  ppl_Polyhedron_get_constraints(P, CS),
  clean_ppl_new_Polyhedron_from_space_dimension(c, 1, universe, P1),
  ppl_Polyhedron_add_constraints(P1, CS),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  clean_ppl_new_Polyhedron_from_space_dimension(c, 1, universe, P2),
  ppl_Polyhedron_add_constraint(P2, A = 1),
  large_integers_affine_transform_loop(Exp, P2, A),
  ppl_Polyhedron_affine_image(P2, A, Sign * (A + Add), 1),
  ppl_Polyhedron_equals_Polyhedron(P, P2),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2).

large_integers_sys_prolog_cpp([]).
large_integers_sys_prolog_cpp([Add|Adds]) :-
  pl_check_prolog_flag(max_integer, Max_int),
  pl_check_prolog_flag(min_integer, Min_int),
  Max is Max_int-3,
  Min is Min_int+3,
  large_integers_sys_prolog_cpp2(Max, Add, 1),
  large_integers_sys_prolog_cpp2(Min, Add, -1),
  large_integers_sys_prolog_cpp(Adds).

large_integers_sys_prolog_cpp2(MaxMin, Add, Sign) :-
  make_vars(1, [A]),
  Inhomo is MaxMin + Sign* Add,
  out(sys_large_int, Inhomo),
  clean_ppl_new_Polyhedron_from_space_dimension(c, 1, universe, P),
  ppl_Polyhedron_add_constraints(P, [A = Inhomo]),
  ppl_Polyhedron_get_constraints(P, CS),
  clean_ppl_new_Polyhedron_from_space_dimension(c, 1, universe, P1),
  ppl_Polyhedron_add_constraints(P1, CS),
  ppl_Polyhedron_equals_Polyhedron(P, P1),
  clean_ppl_new_Polyhedron_from_space_dimension(c, 1, universe, P2),
  InhomoDiv2 is Inhomo // 2,
  InhomoMod2 is Inhomo mod 2,
  ppl_Polyhedron_add_constraint(P2, A = InhomoDiv2),
  ppl_Polyhedron_affine_image(P2, A, 2*A + Sign* InhomoMod2, 1),
  ppl_Polyhedron_equals_Polyhedron(P, P2),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2).

large_integers_affine_transform_loop(0, _P, _).
large_integers_affine_transform_loop(Exp, P, A) :-
  Exp >= 1,
  ppl_Polyhedron_affine_image(P, A, 2*A, 1),
  Exp1 is Exp - 1,
  large_integers_affine_transform_loop(Exp1, P, A).

%%%%%%%%%%%%%%%%% Exceptions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% exceptions/0 tests both Prolog and C++ exceptions using:
%
% exception_prolog(+N, +V)
% exception_sys_prolog(+N, +V)
% exception_cplusplus(+N, +V)
%
% N is the number of the test while V is a list of 3 PPL variables
%
% In exceptions/0, the calls to these predicates should fail
% so that all the tests are tried on backtracking.
% When all the tests have been tried,
% (and, for the Prolog interface, providing the correct
% exception message),
% the call to exceptions/0 succeeds.
% If one of the tests succeeds or a Prolog interface exception
% has a wrong exception message, then exceptions/0 will fail.

exceptions :-
   pl_check_prolog_flag(bounded, Y),
   make_vars(3, V),
   exception_prolog(V),
   ((Y == true,\+prolog_system('XSB'))  -> exception_sys_prolog(V) ; true),
   exception_cplusplus(V),
   !.
exceptions :-
   prolog_system('XSB').

%% TEST: Prolog_unsigned_out_of_range
exception_yap :-
     I = 21474836470, J = 3, K = 0,
     ppl_new_C_Polyhedron_from_generators(
        [point('$VAR'(I)),point('$VAR'(J))], P),
     ppl_Polyhedron_get_generators(P, GS),
     nl, write(GS), nl,
     ppl_new_C_Polyhedron_from_generators(
        [point('$VAR'(I)),point('$VAR'(K))], P1),
     ppl_Polyhedron_get_generators(P1, GS1),
     nl, write(GS1), nl,
     ppl_delete_Polyhedron(P),
     ppl_delete_Polyhedron(P1).

% exception_prolog(+N, +V) checks exceptions thrown by the Prolog interface.
% It does not check those that are dependent on a specific Prolog system.

exception_prolog(V) :-
   exception_prolog1(14, V).

exception_prolog1(0, _) :- !.
exception_prolog1(N, V) :-
   exception_prolog(N, V),
   N1 is N - 1,
   exception_prolog1(N1, V).

%% TEST: Prolog_unsigned_out_of_range.
%% This test accepts any one of three exceptions:
%% ppl_invalid_argument: with a 32 bit system, the number 1 << 59 is expected
%%                       to throw this Prolog exception;
%% out_of_memory:        with a 64 bit system, the number 1 << 59 does not
%%                       throw an exception on the Prolog side, but the
%%                       large number of dimensions will cause a bad_alloc
%%                       exception in C++.
%%
exception_prolog(1, _) :-
    pl_check_prolog_flag(bounded, Y),
   (Y == true ->
     true
    ;
     (I is 1 << 59,
        must_catch(ppl_new_C_Polyhedron_from_generators([point('$VAR'(I))], _),
                   prolog_exception_error)
      )
   ).

%% TEST: not_unsigned_integer
exception_prolog(2, _) :-
  must_catch(ppl_new_C_Polyhedron_from_space_dimension(n, universe, _),
             ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_space_dimension(-1, universe,  _),
             ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_generators([point('$VAR'(n))], _),
             ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_generators([point('$VAR'(-1))], _),
             ppl_invalid_argument).

%% TEST: not_unsigned_integer
exception_prolog(3, _) :-
    must_catch(ppl_set_timeout(-1), ppl_invalid_argument).

%% TEST: not_unsigned_integer
exception_prolog(4, _) :-
  clean_ppl_new_Polyhedron_from_space_dimension(c, 3, universe, P),
  clean_ppl_new_Polyhedron_from_space_dimension(c, 3, universe, Q),
  must_catch(ppl_Polyhedron_BHRZ03_widening_assign_with_tokens(
             Q, P, -1, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens(
             Q, P, [], -1, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens(
             Q, P, [], -1, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_H79_widening_assign_with_tokens(
             Q, P, -1, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens(
             Q, P, [], -1, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens(
             Q, P, [], -1, _), ppl_invalid_argument),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q).

%% TEST: non_linear
exception_prolog(5, [A,B,C]) :-
  must_catch(ppl_new_C_Polyhedron_from_generators([point(B + A*C)], _),
             ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_generators(
                     [point(C), ray(B + C, 1)], _), ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_generators(
                     [point], _), ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_generators(
                     [point(_D)], _), ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_constraints(
                     [_E >= 3], _), ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_constraints(
                     [A*B = 0], _), ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_constraints(
                     [A], _), ppl_invalid_argument).

%% TEST: not_a_variable
exception_prolog(6, [A,_,_]) :-
  clean_ppl_new_Polyhedron_from_space_dimension(c, 3, universe, P),
  must_catch(ppl_Polyhedron_remove_space_dimensions(P, [A,1]),
             ppl_invalid_argument),
  !,
  ppl_delete_Polyhedron(P).

%% TEST: not_an_integer
exception_prolog(7, [A,B,_]) :-
  clean_ppl_new_Polyhedron_from_generators(c,
               [point(A + B), ray(A), ray(B)], P),
  must_catch(ppl_Polyhedron_affine_image(P, A, A + B + 1, i),
             ppl_invalid_argument),
  !,
  ppl_delete_Polyhedron(P).

%% TEST: not_a_polyhedron_handle
exception_prolog(8, _) :-
  must_catch(ppl_Polyhedron_space_dimension(_, _N), ppl_invalid_argument).

%% TEST: not_a_complexity_class
exception_prolog(9, [A, _, _]) :-
   clean_ppl_new_Polyhedron_from_generators(c,
               [point(A)], P),
   must_catch(
     clean_ppl_new_Polyhedron_from_Polyhedron_with_complexity(a, c, P,
                                                              c, _P_copy),
              ppl_invalid_argument),
   !,
   ppl_delete_Polyhedron(P).

%% TEST: not_universe_or_empty
exception_prolog(10, _) :-
  must_catch(ppl_new_C_Polyhedron_from_space_dimension(3, xxx, _),
             ppl_invalid_argument).

%% TEST: not_relation
exception_prolog(11, [A, B, _]) :-
  clean_ppl_new_Polyhedron_from_generators(c,
               [point(A)], P),
  must_catch(ppl_Polyhedron_generalized_affine_image(P, A, x, A + 1, 1),
             ppl_invalid_argument),
  must_catch(
     ppl_Polyhedron_generalized_affine_image_lhs_rhs(P, B - 1, x, A + 1),
             ppl_invalid_argument),
  must_catch(
     ppl_Polyhedron_generalized_affine_image_lhs_rhs(P, B - 1, x + y, A + 1),
             ppl_invalid_argument),
   !,
   ppl_delete_Polyhedron(P).

%% TEST: not_a_nil_terminated_list
exception_prolog(12, [A, B, C]) :-
  must_catch(ppl_new_C_Polyhedron_from_generators(
     [point(A + B + C, 1) | not_a_list], _), ppl_invalid_argument),
  must_catch(ppl_new_C_Polyhedron_from_constraints(
     [A = 0, B >= C | not_a_list], _), ppl_invalid_argument),
  clean_ppl_new_Polyhedron_from_space_dimension(nnc, 3, universe, P),
  must_catch(ppl_Polyhedron_add_constraints(P, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_add_constraints(P, not_a_list),
             ppl_invalid_argument),
  must_catch(ppl_Polyhedron_add_generators(P, not_a_list),
             ppl_invalid_argument),
  must_catch(ppl_Polyhedron_add_generators(P, _), ppl_invalid_argument),
  clean_ppl_new_Polyhedron_from_space_dimension(c, 3, empty, Q),
  must_catch(ppl_Polyhedron_map_space_dimensions(Q, not_a_list),
             ppl_invalid_argument),
  must_catch(ppl_Polyhedron_fold_space_dimensions(Q, not_a_list, B),
             ppl_invalid_argument),
  must_catch(ppl_Polyhedron_remove_space_dimensions(Q, not_a_list),
             ppl_invalid_argument),
  must_catch(ppl_Polyhedron_limited_H79_extrapolation_assign(
             Q, P, not_a_list), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens(
             Q, P, not_a_list, 1, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_bounded_H79_extrapolation_assign(
             Q, P, not_a_list), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens(
             Q, P, not_a_list, 1, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_limited_BHRZ03_extrapolation_assign(
             Q, P, not_a_list), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens(
             Q, P, not_a_list, 1, _), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign(
             Q, P, not_a_list), ppl_invalid_argument),
  must_catch(ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens(
             Q, P, not_a_list, 1, _), ppl_invalid_argument),
  !,
  ppl_delete_Polyhedron(P),
  ppl_delete_Polyhedron(Q).

%% TEST: not_an_mip_problem_handle
exception_prolog(13, _) :-
  must_catch(ppl_MIP_Problem_space_dimension(_, _N), ppl_invalid_argument),
  must_catch(ppl_MIP_Problem_constraints(p, []), ppl_invalid_argument).

%% TEST: not_an_pip_problem_handle
exception_prolog(14, _) :-
  must_catch(ppl_PIP_Problem_space_dimension(_, _N), ppl_invalid_argument),
  must_catch(ppl_PIP_Problem_constraints(p, []), ppl_invalid_argument).

% exception_sys_prolog(+N, +V) checks exceptions thrown by Prolog interfaces
% that are dependent on a specific Prolog system.
% These are only checked if current_prolog_flag(bounded, true) holds.

exception_sys_prolog(V) :-
   exception_sys_prolog1(4, V).

exception_sys_prolog1(0, _) :- !.
exception_sys_prolog1(N, V) :-
   exception_sys_prolog(N, V),
   N1 is N - 1,
   exception_sys_prolog1(N1, V).

exception_sys_prolog(1, [A,B,_]) :-
  pl_check_prolog_flag(max_integer, Max_Int),
  catch((
          clean_ppl_new_Polyhedron_from_constraints(c,
               [Max_Int * A - B =< 0, 3 >= A], P),
          must_catch(ppl_Polyhedron_get_generators(P, _GS),
                ppl_sys_prolog_error),
          !,
          ppl_delete_Polyhedron(P)
        ),
        ppl_overflow_error(Cause),
        check_exception_term(ppl_overflow_error(Cause))
       ).

 exception_sys_prolog(2, [A,B,_]) :-
  pl_check_prolog_flag(min_integer, Min_Int),
  catch((
          clean_ppl_new_Polyhedron_from_constraints(c,
               [Min_Int * A - B =< 0, 2 >= A], P),
          must_catch(ppl_Polyhedron_get_generators(P, _GS),
                ppl_sys_prolog_error),
          !,
          ppl_delete_Polyhedron(P)
        ),
        ppl_overflow_error(Cause),
        check_exception_term(ppl_overflow_error(Cause))
       ).

exception_sys_prolog(3, [A,B,_]) :-
  pl_check_prolog_flag(max_integer, Max_Int),
  catch((
          clean_ppl_new_Polyhedron_from_generators(c,
               [point(Max_Int * A + B)], P),
          ppl_Polyhedron_affine_image(P, A, A + 1, 1),
          must_catch(ppl_Polyhedron_get_generators(P, _GS),
                ppl_sys_prolog_error),
          !,
          ppl_delete_Polyhedron(P)
        ),
        ppl_overflow_error(Cause),
        check_exception_term(ppl_overflow_error(Cause))
       ).

exception_sys_prolog(4, [A,_,_]) :-
   pl_check_prolog_flag(min_integer, Min_Int),
   catch((
          clean_ppl_new_Polyhedron_from_generators(c,
                                                   [point(Min_Int*A)], P),
          ppl_Polyhedron_affine_image(P, A, A - 1, 1),
          must_catch(ppl_Polyhedron_get_generators(P, _GS),
                     ppl_sys_prolog_error),
          !,
          ppl_delete_Polyhedron(P)
         ),
         ppl_overflow_error(Cause),
         check_exception_term(ppl_overflow_error(Cause))
        ).

% exception_cplusplus(+N, +V) checks exceptions thrown by the C++
% interface for the PPL.

exception_cplusplus(V) :-
   exception_cplusplus1(10, V).

exception_cplusplus1(0, _) :- !.
exception_cplusplus1(N, V) :-
   exception_cplusplus(N, V),
   N1 is N - 1,
   exception_cplusplus1(N1, V).

exception_cplusplus(1, [A, B, C]) :-
  must_catch(ppl_new_C_Polyhedron_from_generators([point(A + B + C, 0)], _),
             cpp_error).

exception_cplusplus(2, [A, B, _]) :-
  clean_ppl_new_Polyhedron_from_generators(c,
               [point(A + B), ray(A), ray(B)], P),
  must_catch(ppl_Polyhedron_affine_image(P, A, A + B + 1, 0),
             cpp_error),
  !,
  ppl_delete_Polyhedron(P).

exception_cplusplus(3, [A, B, _]) :-
  clean_ppl_new_Polyhedron_from_space_dimension(c, 0, universe, P1),
  clean_ppl_new_Polyhedron_from_generators(c,
               [point(A + B)], P2),
  must_catch(ppl_Polyhedron_poly_hull_assign(P1, P2),
             cpp_error),
  !,
  ppl_delete_Polyhedron(P1),
  ppl_delete_Polyhedron(P2).

exception_cplusplus(4, [A, B, C]) :-
   must_catch(ppl_new_C_Polyhedron_from_generators([line(A + B + C)], _),
             cpp_error).

exception_cplusplus(5, [A,B,C]) :-
  clean_ppl_new_Polyhedron_from_generators(c, [point(B + 2*C)], P),
  ppl_Polyhedron_remove_space_dimensions(P, [C]),
  must_catch(ppl_Polyhedron_remove_space_dimensions(P, [A, C]),
             cpp_error),
  !,
  ppl_delete_Polyhedron(P).

exception_cplusplus(6, [A, B, _]) :-
  clean_ppl_new_Polyhedron_from_constraints(c,
               [A >= 1], P),
  must_catch(ppl_Polyhedron_affine_image(P, B, A + 1, 1),
             cpp_error),
  !,
  ppl_delete_Polyhedron(P).

exception_cplusplus(7, [A, B, C]) :-
  clean_ppl_new_Polyhedron_from_constraints(c,
               [A >= 1, B>= 1], P),
  must_catch(ppl_Polyhedron_affine_image(P, B, A + C + 1, 1),
             cpp_error),
  !,
  ppl_delete_Polyhedron(P).

exception_cplusplus(8, [A, B, _]) :-
  clean_ppl_new_Polyhedron_from_constraints(c,
               [A >= B], P),
  must_catch(ppl_Polyhedron_affine_preimage(P, A, A + B + 1, 0),
             cpp_error),
  !,
  ppl_delete_Polyhedron(P).

exception_cplusplus(9, [A, B, C]) :-
  clean_ppl_new_Polyhedron_from_generators(c,
               [point(0), ray(A + B), ray(A)], P),
  must_catch(ppl_Polyhedron_affine_preimage(P, C, A + 1, 1),
             cpp_error),
  !,
  ppl_delete_Polyhedron(P).


exception_cplusplus(10, [A, B, C]) :-
  clean_ppl_new_Polyhedron_from_generators(c,
               [point(0), point(A), line(A + B)], P),
  must_catch(ppl_Polyhedron_affine_preimage(P, B, A + C, 1),
             cpp_error),
  !,
  ppl_delete_Polyhedron(P).

% must_catch(+Call) calls Call using catch and checks exception.
% If expected exception is caught, it succeeds and fails if not.

must_catch(Call, prolog_exception_error) :-
    !,
    catch( Call, Message, format_exception_message( Message ) ),
    ( \+var(Message) ->
        ((
          Message =.. [ppl_invalid_argument|_]
         ;
          Message =.. [ppl_length_error|_]
         ;
          Message = out_of_memory
         ) ->
            true
        ;
            check_exception_term(Message)
        )
    ;
        fail
    ).
must_catch(Call, cpp_error) :-
    !,
    catch( Call, Message, format_exception_message( cpp_error(Message) ) ),
    ( ( \+ var(Message),
          functor(Message, ppl_invalid_argument, _) ) ->
        true
    ;
        fail
    ).
must_catch(Call, ppl_sys_prolog_error) :-
    !,
    catch( Call, Message, format_exception_message(Message) ),
    ( ( \+ var(Message),
          (Message =.. [ppl_representation_error|_] )) ->
        true
    ;
        fail
    ).
must_catch(Call, Expected) :-
    catch(Call, Message, format_exception_message(Message) ),
    (\+ var(Message), Message = ppl_overflow_error(_) ->
        true
    ;
        ( \+ var(Message), Message =.. [Expected|_] ->
            true
        ;
            fail
        )
    ).

%%%%%%%%%%%% predicate for making list of ppl variables %%%%%%

% make_var_list(+I,+Dimension,?Variable_List)
% constructs a list of variables with indices from I to Dimension - 1.
% It is assumed that I =< Dimension.

make_vars(Dim, Var_List):-
  make_var_list(0, Dim, Var_List).
make_var_list(Dim,Dim,[]):- !.
make_var_list(I,Dim,['$VAR'(I)|Var_List]):-
  I1 is I + 1,
  make_var_list(I1,Dim,Var_List).

%%%%%%%%%%%% predicate for safely deleting polyhedra on failure %

cleanup_ppl_Polyhedron(_).
cleanup_ppl_Polyhedron(P) :-
  out(cs, P),
  out(gs, P),
  ppl_delete_Polyhedron(P), fail.

cleanup_ppl_Polyhedra([]).
cleanup_ppl_Polyhedra([_|_]).
cleanup_ppl_Polyhedra([P|Ps]) :-
  delete_all_ppl_Polyhedra([P|Ps]).

delete_all_ppl_Polyhedra([]).
delete_all_ppl_Polyhedra([P|Ps]) :-
  ppl_delete_Polyhedron(P),
  delete_all_ppl_Polyhedra(Ps).

cleanup_ppl_MIP_Problem(_).
cleanup_ppl_MIP_Problem(MIP) :-
  out(mip, MIP),
  ppl_delete_MIP_Problem(MIP), fail.

cleanup_ppl_PIP_Problem(_).
cleanup_ppl_PIP_Problem(PIP) :-
  out(pip, PIP),
  ppl_delete_PIP_Problem(PIP), fail.

out(cs, P):-
  ((noisy(N), N < 2) -> true ;
    ppl_Polyhedron_get_constraints(P, CS),
    nl, write(CS), nl
  ).

out(gs, P):-
  ((noisy(N), N < 2) -> true ;
    ppl_Polyhedron_get_generators(P, GS),
    nl, write(GS), nl
  ).

out(mip, MIP):-
  ((noisy(N), N < 2) -> true ;
    ppl_MIP_Problem_constraints(MIP, CS),
    ppl_MIP_Problem_objective_function(MIP, Obj),
    ppl_MIP_Problem_optimization_mode(MIP, Opt),
    ppl_MIP_Problem_get_control_parameter(MIP, pricing, Cp_value),
    nl,
    write(' constraint system is: '), write(CS), nl,
    write(' objective function is: '), write(Obj), nl,
    write(' optimization mode is: '), write(Opt),
    write(' control_parameter_value is: '), write(Cp_value),
    nl
  ).

out(pip, PIP):-
  ((noisy(N), N < 2) -> true ;
    ppl_PIP_Problem_constraints(PIP, CS),
    ppl_PIP_Problem_parameter_space_dimensions(PIP, Dims),
    ppl_PIP_Problem_get_control_parameter(PIP, cutting_strategy, Cp_value),
    nl,
    write(' constraint system is: '), write(CS), nl,
    write(' parameter space dimensions are: '), write(Dims), nl,
    write(' control_parameter_value is: '), write(Cp_value),
    nl
  ).

out(sys_large_int, init):-
  !,
  prolog_system(System),
  ((noisy(N), N < 2) -> true ;
    nl, write_all([' At the Prolog/C++ interface, for', System, 'Prolog', nl,
       ' the extra numbers tested are: ']),
    nl
  ).

out(sys_large_int, Num):-
  ((noisy(N), N < 2) -> true ;
      write_all([Num, ',  '])
  ).

out(large_int, init):-
  !,
  ((noisy(N), N < 2) -> true ;
    nl, write(' At the Prolog/C++ interface, the numbers tested are: '),
    nl
  ).

out(large_int, Num, Sign, Add, Exp):-
  ((noisy(N), N < 2) -> true ;
    write_all([Num, ' = ', Sign, ' * ', '((1 << ', Exp, ') + ', Add, '),  '])
  ).

%%% predicates for ensuring new polyhedra are always deleted on failure %

clean_ppl_new_Polyhedron_from_space_dimension(T, D, Universe_or_Empty, P) :-
  (T = c ->
    ppl_new_C_Polyhedron_from_space_dimension(D, Universe_or_Empty, P)
  ;
    ppl_new_NNC_Polyhedron_from_space_dimension(D, Universe_or_Empty, P)
  ),
  cleanup_ppl_Polyhedron(P).

clean_ppl_new_Polyhedron_from_constraints(T, CS, P) :-
  (T = c ->
    ppl_new_C_Polyhedron_from_constraints(CS, P)
   ;
    ppl_new_NNC_Polyhedron_from_constraints(CS, P)
  ),
  cleanup_ppl_Polyhedron(P).

clean_ppl_new_Polyhedron_from_generators(T, GS, P) :-
  (T = c ->
    ppl_new_C_Polyhedron_from_generators(GS, P)
   ;
    ppl_new_NNC_Polyhedron_from_generators(GS, P)
  ),
  cleanup_ppl_Polyhedron(P).

clean_ppl_new_Polyhedron_from_Polyhedron(TQ, Q, TP, P) :-
  ((TP == c, TQ == c) ->
    ppl_new_C_Polyhedron_from_C_Polyhedron(Q, P)
   ;
    ((TP == c, TQ == nnc) ->
      ppl_new_C_Polyhedron_from_NNC_Polyhedron(Q, P)
    ;
      ((TP == nnc, TQ == c) ->
        ppl_new_NNC_Polyhedron_from_C_Polyhedron(Q, P)
      ;
        ppl_new_NNC_Polyhedron_from_NNC_Polyhedron(Q, P)
      )
    )
  ),
  cleanup_ppl_Polyhedron(P).

clean_ppl_new_Polyhedron_from_Polyhedron_with_complexity(C, TQ, Q, TP, P) :-
  ((TP == c, TQ == c) ->
    ppl_new_C_Polyhedron_from_C_Polyhedron_with_complexity(C, Q, P)
   ;
    ((TP == c, TQ == nnc) ->
      ppl_new_C_Polyhedron_from_NNC_Polyhedron_with_complexity(C, Q, P)
    ;
      ((TP == nnc, TQ == c) ->
        ppl_new_NNC_Polyhedron_from_C_Polyhedron_with_complexity(C, Q, P)
      ;
        ppl_new_NNC_Polyhedron_from_NNC_Polyhedron_with_complexity(C, Q, P)
      )
    )
  ),
  cleanup_ppl_Polyhedron(P).

clean_ppl_new_MIP_Problem_from_space_dimension(Dim, MIP) :-
  ppl_new_MIP_Problem_from_space_dimension(Dim, MIP),
  cleanup_ppl_MIP_Problem(MIP).

clean_ppl_new_MIP_Problem(Dim, CS, Obj, Opt, MIP) :-
  ppl_new_MIP_Problem(Dim, CS, Obj, Opt, MIP),
  cleanup_ppl_MIP_Problem(MIP).

clean_ppl_new_MIP_Problem_from_MIP_Problem(MIP1, MIP) :-
  ppl_new_MIP_Problem_from_MIP_Problem(MIP1, MIP),
  cleanup_ppl_MIP_Problem(MIP).

clean_ppl_new_PIP_Problem_from_space_dimension(Dim, PIP) :-
  ppl_new_PIP_Problem_from_space_dimension(Dim, PIP),
  cleanup_ppl_PIP_Problem(PIP).

clean_ppl_new_PIP_Problem(Dim, CS, Vars, PIP) :-
  ppl_new_PIP_Problem(Dim, CS, Vars, PIP),
  cleanup_ppl_PIP_Problem(PIP).

clean_ppl_new_PIP_Problem_from_PIP_Problem(PIP1, PIP) :-
  ppl_new_PIP_Problem_from_PIP_Problem(PIP1, PIP),
  cleanup_ppl_PIP_Problem(PIP).

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

%%%%%%%%%%%% predicates for pretty printing the PPL banner %%%%%%%%%%
%
% The banner is read as an atom with"/n" denoting where there should
% new lines. Here we print the banner as intended with new lines instead
% of "/n".
%

banner_pp(B) :-
  name(B,Bcodes),
  nl,
  !,
  format_banner(Bcodes).

format_banner([]) :- nl.
format_banner([C]) :- put_code(C), nl.
format_banner([C,C1|Chars]):-
  ([C,C1] == "/n" ->
     (nl,
     format_banner(Chars))
   ;
     (put_code(C),
     format_banner([C1|Chars]))
  ).

%%%%%%%%%%%% predicate for handling an unintended exception %%%%

check_exception_term(ppl_overflow_error(Cause)) :-
  ((Cause == 'Negative overflow.'; Cause == 'Positive overflow.') ->
    true
  ;
    print_exception_term(ppl_overflow_error(Cause))
  ),
  !.

print_exception_term(ppl_overflow_error(Cause)) :-
  nl,
  write('Error: an overflow has been detected by the PPL: '),
  write(Cause),
  nl,
  !.

print_exception_term(Exception) :-
  write('exception'), nl,
  nl,
  writeq(Exception),
  nl.

%%%%%%%%%%%% predicate for printing exception messages %%%%%%%%%%

format_exception_message(
             ppl_invalid_argument( found(F), expected(E), where(W))
                        ) :-
  !,
  display_message(['PPL Prolog Interface Exception:', nl, '   ',
                   F, 'is an invalid argument for', W, nl, '   ',
                  F, 'should be', E, '.']).

format_exception_message(ppl_length_error(Error)) :-
  !,
  display_message(['PPL Prolog Interface Exception: ', nl, '   ',
                   'ppl_length_error', Error]).

format_exception_message(
             ppl_representation_error(I, where(W))
                        ) :-
  !,
  display_message(['PPL Prolog Interface Exception:', nl, '   ',
                   'This Prolog system has bounded integers', nl, '   ',
                   I, 'is not in the allowed range of integers', nl, '   ',
                   'in call to', W, '.']).

format_exception_message(out_of_memory) :-
  !,
  display_message(['PPL Prolog Interface Exception: ', nl, '   ',
                   'out of memory']).

format_exception_message(ppl_overflow_error(Type)) :-
  !,
  display_message(['PPL Prolog Interface Exception: ', nl, '   ',
                   'ppl_overflow_error: ', Type]).

format_exception_message(cpp_error(Error)) :-
  !,
  display_message(['PPL C++ Interface Exception:', nl, '   ', Error]).

format_exception_message(Error) :-
  display_message(['Unknown exception: ', Error]), fail.

%%%%%%%%%%%% predicates for output messages %%%%%%%%%%%%%%%%%%

error_message(Message):-
   write_all(Message).

display_message(Message):-
    noisy(_),
    (noisy(0) -> true ;
     (nl, write_all(Message))
    ).

write_all([]) :- nl.
write_all([Phrase|Phrases]):-
   (Phrase == nl ->
      nl
   ;
      write(Phrase),
      write(' ')
   ),
   write_all(Phrases).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% list_groups(G)
% The interface predicates are partitioned into related sets called
% groups and here is a list of the groups.

list_groups( [
   large_integers,
   all_versions_and_banner,
   numeric_bounds,
   new_polyhedron_from_dimension,
   new_polyhedron_from_polyhedron,
   new_polyhedron_from_representations,
   swap_polyhedra,
   polyhedron_dimension,
   basic_operators,
%   transform_polyhedron,
   extrapolation_operators,
   get_system,
%   add_to_system,
   revise_dimensions,
   check_polyhedron,
   minmax_polyhedron,
   compare_polyhedra,
   mip_problem,
   pip_problem,
   transform_polyhedron,
   add_to_system,
   catch_time,
   handle_exceptions
             ] ).

% group_predicates(G, P)
% P is a list of the interface predicates checked by test for group G.
% This is used to generate more informative error and exception messages.

group_predicates(all_versions_and_banner,
  [ppl_version_major/1,
   ppl_version_minor/1,
   ppl_version_revision/1,
   ppl_version_beta/1,
   ppl_version/1,
   ppl_banner/1
  ]).

group_predicates(numeric_bounds,
  [ppl_max_space_dimension/1,
   ppl_Coefficient_is_bounded/0,
   ppl_Coefficient_max/1,
   ppl_Coefficient_min/1,
   ppl_set_rounding_for_PPL/0,
   ppl_restore_pre_PPL_rounding/0
  ]).

group_predicates(new_polyhedron_from_dimension,
  [ppl_new_C_Polyhedron_from_space_dimension/4,
   ppl_new_NNC_Polyhedron_from_space_dimension/4,
   ppl_Polyhedron_is_universe/1,
   ppl_Polyhedron_is_empty/1,
   ppl_delete_polyhedron/1
  ]).

group_predicates(new_polyhedron_from_polyhedron,
  [ppl_new_C_Polyhedron_from_C_Polyhedron/3,
   ppl_new_C_Polyhedron_from_NNC_Polyhedron/3,
   ppl_new_NNC_Polyhedron_from_C_Polyhedron/3,
   ppl_new_NNC_Polyhedron_from_NNC_Polyhedron/3,
   ppl_new_C_Polyhedron_from_constraints/2,
   ppl_new_NNC_Polyhedron_from_constraints/2,
   ppl_Polyhedron_equals_Polyhedron/2
  ]).

group_predicates(new_polyhedron_from_representations,
  [ppl_new_C_Polyhedron_from_constraints/2,
   ppl_new_NNC_Polyhedron_from_constraints/2,
   ppl_new_C_Polyhedron_from_generators/2,
   ppl_new_NNC_Polyhedron_from_generators/2
  ]).

group_predicates(swap_polyhedra,
  [ppl_Polyhedron_swap/2
  ]).

group_predicates(polyhedron_dimension,
  [ppl_Polyhedron_affine_dimension/2,
   ppl_Polyhedron_space_dimension/2,
   ppl_Polyhedron_constrains/2,
   ppl_Polyhedron_unconstrain_space_dimension/2,
   ppl_Polyhedron_unconstrain_space_dimensions/2]).

group_predicates(basic_operators,
  [ppl_Polyhedron_intersection_assign/2,
   ppl_Polyhedron_poly_hull_assign/2,
   ppl_Polyhedron_poly_difference_assign/2,
   ppl_Polyhedron_time_elapse_assign/2,
   ppl_Polyhedron_topological_closure_assign/1
  ]).

group_predicates(add_to_system,
  [ppl_Polyhedron_add_constraint/2,
   ppl_Polyhedron_add_generator/2,
   ppl_Polyhedron_add_constraints/2,
   ppl_Polyhedron_add_generators/2
  ]).

group_predicates(revise_dimensions,
  [ppl_Polyhedron_remove_space_dimensions/2,
   ppl_Polyhedron_remove_higher_space_dimensions/2,
   ppl_Polyhedron_expand_space_dimension/3,
   ppl_Polyhedron_fold_space_dimensions/3,
   ppl_Polyhedron_map_space_dimensions/2,
   ppl_Polyhedron_concatenate_assign/2
  ]).

group_predicates(transform_polyhedron,
  [ppl_Polyhedron_affine_image/4,
   ppl_Polyhedron_affine_preimage/4,
   ppl_Polyhedron_bounded_affine_image/5,
   ppl_Polyhedron_bounded_affine_preimage/5,
   ppl_Polyhedron_generalized_affine_image/5,
   ppl_Polyhedron_generalized_affine_preimage/5,
   ppl_Polyhedron_generalized_affine_image_lhs_rhs/4,
   ppl_Polyhedron_generalized_affine_preimage_lhs_rhs/4
  ]).

group_predicates(extrapolation_operators,
  [ppl_Polyhedron_BHRZ03_widening_assign_with_token/3,
   ppl_Polyhedron_BHRZ03_widening_assign/2,
   ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_token/4,
   ppl_Polyhedron_limited_BHRZ03_extrapolation_assign/3,
   ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_token/4,
   ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign/3,
   ppl_Polyhedron_H79_widening_assign_with_token/3,
   ppl_Polyhedron_H79_widening_assign/2,
   ppl_Polyhedron_limited_H79_extrapolation_assign_with_token/4,
   ppl_Polyhedron_limited_H79_extrapolation_assign/3,
   ppl_Polyhedron_bounded_H79_extrapolation_assign_with_token/4,
   ppl_Polyhedron_bounded_H79_extrapolation_assign/3
  ]).

group_predicates(get_system,
  [ppl_Polyhedron_get_constraints/2,
   ppl_Polyhedron_get_minimized_constraints/2,
   ppl_Polyhedron_get_generators/2,
   ppl_Polyhedron_get_minimized_generators/2
  ]).

group_predicates(check_polyhedron,
  [ppl_Polyhedron_relation_with_constraint/3,
   ppl_Polyhedron_relation_with_generator/3,
   ppl_Polyhedron_is_topologically_closed/1,
   ppl_Polyhedron_is_universe,
   ppl_Polyhedron_is_empty,
   ppl_Polyhedron_is_bounded,
   ppl_Polyhedron_contains_integer_point,
   ppl_Polyhedron_contains_Polyhedron/2,
   ppl_Polyhedron_strictly_contains_Polyhedron/2,
   ppl_Polyhedron_is_disjoint_from_Polyhedron/2,
   ppl_Polyhedron_equals_Polyhedron/2,
   ppl_Polyhedron_termination_test_MS/2,
   ppl_Polyhedron_termination_test_PR/2,
   ppl_Polyhedron_OK/1
  ]).

group_predicates(minmax_polyhedron,
  [ppl_Polyhedron_maximize/5,
   ppl_Polyhedron_maximize_with_point/6,
   ppl_Polyhedron_minimize/5,
   ppl_Polyhedron_minimize_with_point/6
  ]).

group_predicates(compare_polyhedra,
  [ppl_Polyhedron_contains_Polyhedron/2,
   ppl_Polyhedron_strictly_contains_Polyhedron/2,
   ppl_Polyhedron_is_disjoint_from_Polyhedron/2,
   ppl_Polyhedron_equals_Polyhedron/2
  ]).

group_predicates(catch_time,
  [ppl_set_timeout_exception_atom/1,
   ppl_timeout_exception_atom/1,
   ppl_set_timeout/1,
   ppl_reset_timeout/0
  ]).

group_predicates(mip_problem,
  ['all MIP_Prolog predicates'
  ]).

group_predicates(pip_problem,
  ['all PIP_Prolog predicates'
  ]).

group_predicates(large_integers,
  ['large integer tests '
  ]).

group_predicates(handle_exceptions,
  'all predicates'' exception handling.'
  ).

%%%%%%%%%%%%%%%%%%%%%%% System flags %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% pl_check_prolog_flag/2
% returns true or false (if the 1st argument is 'bounded')
% or (if the 1st argument is 'max_integer' or  'min_integer')
% the maximum or minimum integer for Prolog
% systems that have bounded integers.
% Note that 268435456 is 2^28.

pl_check_prolog_flag(bounded, TF) :-
  current_prolog_flag(bounded, TF).

pl_check_prolog_flag(max_integer, Max_Int) :-
  \+ prolog_system('XSB'),
  current_prolog_flag(max_integer, Max_Int).

pl_check_prolog_flag(max_integer, Max_Int) :-
  prolog_system('XSB'), Max_Int is 268435455.

pl_check_prolog_flag(min_integer, Min_Int) :-
  \+ prolog_system('XSB'),
  current_prolog_flag(min_integer, Min_Int).

pl_check_prolog_flag(min_integer, Min_Int) :-
  prolog_system('XSB'), Min_Int is -268435456.
