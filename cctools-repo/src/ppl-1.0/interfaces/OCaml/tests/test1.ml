(* Simple program testing the PPL OCaml interface on a few random things.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

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
site: http://bugseng.com/products/ppl/ . *)

open Ppl_ocaml
open Printf
open Gmp

let print_string_if_very_noisy = function str ->
  let less_noisy = "LESS_NOISY"
  in let very_noisy = try Sys.getenv "PPL_VERY_NOISY_TESTS"
    with Not_found -> less_noisy
  in
    if (very_noisy != less_noisy)
    then print_string str;
;;

let print_string_if_noisy = function str ->
  let quiet = "QUIET"
  in let noisy = try Sys.getenv "PPL_NOISY_TESTS"
    with Not_found -> quiet
  in
    if (noisy != quiet)
    then print_string str
    else print_string_if_very_noisy str;
;;

let print_int_if_very_noisy = function num ->
  let less_noisy = "LESS_NOISY"
  in let very_noisy = try Sys.getenv "PPL_VERY_NOISY_TESTS"
    with Not_found -> less_noisy
  in
    if (very_noisy != less_noisy)
    then print_int num;
;;

let print_int_if_noisy = function num ->
  let quiet = "QUIET"
  in let noisy = try Sys.getenv "PPL_NOISY_TESTS"
    with Not_found -> quiet
  in
    if (noisy != quiet)
    then print_int num
    else print_int_if_very_noisy num;
;;

let rec print_linear_expression = function
    Variable v ->
      print_string_if_noisy "V(";
      print_int_if_noisy v;
      print_string_if_noisy ")";
  | Coefficient c ->
      print_int_if_noisy(Z.to_int c)
  | Unary_Minus e ->
      print_string_if_noisy "-(";
      print_linear_expression e;
      print_string_if_noisy ")";
  | Unary_Plus e ->
      print_linear_expression e
  | Plus (e1, e2) ->
      print_string_if_noisy "(";
      print_linear_expression e1;
      print_string_if_noisy " + ";
      print_linear_expression e2;
      print_string_if_noisy ")";
  | Minus (e1, e2) ->
      print_string_if_noisy "(";
      print_linear_expression e1;
      print_string_if_noisy " - ";
      print_linear_expression e2;
      print_string_if_noisy ")";
  | Times (c, e) ->
      print_int_if_noisy(Z.to_int c);
      print_string_if_noisy "*(";
      print_linear_expression e;
      print_string_if_noisy ")";
;;

let rec print_constraint = function
    Less_Than (le1, le2) ->
      print_linear_expression le1;
      print_string_if_noisy " < ";
      print_linear_expression le2;
      print_string_if_noisy ", ";
  | Less_Or_Equal (le1, le2) ->
      print_linear_expression le1;
      print_string_if_noisy " <= ";
      print_linear_expression le2;
      print_string_if_noisy ", ";
  | Equal (le1, le2) ->
      print_linear_expression le1;
      print_string_if_noisy " == ";
      print_linear_expression le2;
      print_string_if_noisy ", ";
  | Greater_Than (le1, le2) ->
      print_linear_expression le1;
      print_string_if_noisy " > ";
      print_linear_expression le2;
      print_string_if_noisy ", ";
  | Greater_Or_Equal (le1, le2) ->
      print_linear_expression le1;
      print_string_if_noisy " >= ";
      print_linear_expression le2;
      print_string_if_noisy ", ";
;;

let rec print_generator = function
    Ray (le1) ->
      print_string_if_noisy "Ray: ";
      print_linear_expression le1;
      print_string_if_noisy ", ";
  |  Line (le1) ->
      print_string_if_noisy "Line: ";
      print_linear_expression le1;
      print_string_if_noisy ", ";
  | Point (le1, c) ->
      print_string_if_noisy "Point: ";
      print_linear_expression le1;
      print_string_if_noisy " den: ";
      print_int_if_noisy(Z.to_int c);
      print_string_if_noisy ", ";
| Closure_Point (le1, c) ->
      print_string_if_noisy "Closure_Point: ";
      print_linear_expression le1;
      print_string_if_noisy " den: ";
      print_int_if_noisy(Z.to_int c);
      print_string_if_noisy ", ";;

let print_congruence = function x,y,z ->
  print_linear_expression x;
  print_string_if_noisy " %= ";
  print_linear_expression y;
  print_string_if_noisy " mod ";
  print_int_if_noisy(Z.to_int z);
  print_string_if_noisy ", ";;

let print_artificial_parameter = function x,y ->
  print_linear_expression x;
  print_string_if_noisy " / ";
  print_int_if_noisy(Z.to_int y);
  print_string_if_noisy ", ";;

(* Build linear expressions the hard way. *)

print_string_if_noisy "Build linear expressions manually:\n" ;;

let rec a = Variable 0
and b = Variable 1
and c = Variable 2
and n = Coefficient (Z.from_int 3)
and e1 = Plus (c, c)
and e2 = Times ((Z.from_int 7), a)
and e3 = Plus (n, c)
;;

print_linear_expression a; print_string_if_noisy "\n" ;;
print_linear_expression b; print_string_if_noisy "\n" ;;
print_linear_expression c; print_string_if_noisy "\n" ;;
print_linear_expression n; print_string_if_noisy "\n" ;;
print_linear_expression e1; print_string_if_noisy "\n" ;;
print_linear_expression e2; print_string_if_noisy "\n" ;;

(* See whether operators can make life better. *)

print_string_if_noisy "Build linear expressions with operators:\n" ;;

let linear_expression_of_int n = Coefficient (Z.of_int n) ;;
let linear_expression_plus e1 e2 = Plus (e1, e2) ;;
let linear_expression_minus e1 e2 = Minus (e1, e2) ;;
let linear_expression_times c e = Times (c, e) ;;
let linear_constraint_eq e1 e2 = Equal (e1, e2) ;;
let linear_constraint_lt e1 e2 = Less_Than (e1, e2) ;;
let linear_constraint_gt e1 e2 = Greater_Than (e1, e2) ;;
let linear_constraint_le e1 e2 = Less_Or_Equal (e1, e2) ;;
let linear_constraint_ge e1 e2 = Greater_Or_Equal (e1, e2) ;;

let ( +/ ) = linear_expression_plus
let ( -/ ) = linear_expression_minus
let ( */ ) = linear_expression_times
let ( =/ ) = linear_constraint_eq
let ( </ ) = linear_constraint_lt
let ( >/ ) = linear_constraint_gt
let ( <=/ ) = linear_constraint_le
let ( >=/ ) = linear_constraint_ge

let e3 =
  (Z.of_int 3) */ a
  +/
  (Z.of_int 4) */ b
  -/
  (linear_expression_of_int 7)
;;
print_linear_expression e3; print_string_if_noisy "\n" ;;

(* Probably the most convenient thing for the user will be to use the
   the Camlp4 preprocessor: see
   http://caml.inria.fr/pub/docs/manual-ocaml/manual003.html#htoc10 *)

(* Build some PPL::C_Polyhedron. *)

let constraint1 = (e2 >=/ e2);;
let constraints1 = [e3 >=/ e1; e1 >=/ e2; e1 <=/ e2 -/ n] ;;
let generator1 = Point (e3, (Z.from_int 1));;
let generators1 = [Point (e2, (Z.from_int 1)); Point (e1, (Z.from_int 2))] ;;
let congruence1 = (e2, e2 , (Z.from_int 1));;
let congruences1 = [e3, e2 , (Z.from_int 20)];;
let grid_generator1 = Grid_Point (e3, (Z.from_int 1));;

let mip1 = ppl_new_MIP_Problem 10 constraints1 e3 Maximization;;
let objective_func = ppl_MIP_Problem_objective_function mip1;;
print_string_if_noisy "\n";;
print_linear_expression objective_func;;
let i = ppl_MIP_Problem_space_dimension mip1;;
print_string_if_noisy "\n";;
let i = ppl_MIP_Problem_constraints mip1;;
print_string_if_noisy "\n";;
List.iter print_constraint i;;
let i = ppl_MIP_Problem_get_control_parameter mip1 Pricing;;
print_string_if_noisy "\n";;
ppl_MIP_Problem_set_control_parameter mip1 Pricing_Textbook;;
let i = ppl_MIP_Problem_get_control_parameter mip1 Pricing;;
let out = if (i == Pricing_Textbook)
  then "Control Parameter test succeeded"
  else "Control Parameter test failed"
    in (print_string_if_noisy out);;

print_string_if_noisy "\n";;
print_string_if_noisy ("space dimensions: ");;
for i = 6 downto 0 do
  let polyhedron1 = ppl_new_C_Polyhedron_from_space_dimension i Empty
  in let dimension =  ppl_Polyhedron_space_dimension(polyhedron1)
  in (print_int_if_noisy dimension;
     print_string_if_noisy ", ")
done;;
print_string_if_noisy "\n";;

let c1 = (a  >=/ linear_expression_of_int 0);;
let c2 = (a  <=/ linear_expression_of_int 2);;
let c2a = (a  <=/ linear_expression_of_int 3);;
let c3 = (b  >=/ linear_expression_of_int 0);;
let c4 = (b  <=/ linear_expression_of_int 2);;
let cs1 = [c1; c2; c3; c4];;
let cs2 = [c1; c2a; c3; c4];;
let poly1 = ppl_new_C_Polyhedron_from_constraints(cs1);;
let poly2 = ppl_new_C_Polyhedron_from_constraints(cs2);;

let polyhedron1 = ppl_new_C_Polyhedron_from_constraints(constraints1);;
let polyhedron2 = ppl_new_C_Polyhedron_from_generators(generators1);;
let result =  ppl_Polyhedron_bounds_from_above polyhedron1 e2;;
let p = Ppl_ocaml.ppl_new_C_Polyhedron_from_constraints [];;
let u = Ppl_ocaml.ppl_Polyhedron_add_constraints p [];;
let out = if (u == ())
  then "ppl_Polyhedron_add_constraints returns unit"
  else "ppl_Polyhedron_add_constraints does not return unit"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;

ppl_Polyhedron_add_constraint polyhedron1 constraint1;;
ppl_Polyhedron_add_generator polyhedron1 generator1;;
let b = ppl_Polyhedron_is_disjoint_from_Polyhedron
  polyhedron1 polyhedron2;;
ppl_Polyhedron_concatenate_assign polyhedron1 polyhedron2;;
let congruences = ppl_Polyhedron_get_congruences polyhedron1 in
List.iter print_congruence congruences;;
print_string_if_noisy "\n";;

print_string_if_noisy "\nTesting affine transformations \n";;
ppl_Polyhedron_bounded_affine_preimage polyhedron1 1 e1 e2 (Z.from_int 10);;
ppl_Polyhedron_bounded_affine_preimage polyhedron1 1 e1 e2 (Z.from_int 10);;
ppl_Polyhedron_affine_image polyhedron1 1 e1 (Z.from_int 10);;

print_string_if_noisy "\nTesting widenings and extrapolations \n";;
let tokens_l_BHRZ03 =
  ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens
  polyhedron1 polyhedron1 constraints1 10;;
let tokens_b_BHRZ03 =
  ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens
  polyhedron1 polyhedron1 constraints1 10;;
let tokens_b_H79 = ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens
  polyhedron1 polyhedron1 constraints1 10;;
let tokens_H79 = ppl_Polyhedron_H79_widening_assign_with_tokens poly2 poly1 2;;
ppl_Polyhedron_H79_widening_assign polyhedron1 polyhedron1 ;;
print_string_if_noisy "tokens b_H79 = ";;
print_int_if_noisy tokens_b_H79;;
print_string_if_noisy "\n";;
print_string_if_noisy "tokens b_BHRZ03 = ";;
print_int_if_noisy tokens_b_BHRZ03;;
print_string_if_noisy "\n";;
print_string_if_noisy "tokens H79 = ";;
print_int_if_noisy tokens_H79;;
print_string_if_noisy "\n";;

let b = ppl_Polyhedron_OK polyhedron1;;
ppl_Polyhedron_generalized_affine_preimage_lhs_rhs
  polyhedron1 e1 Equal_RS e1;;
ppl_Polyhedron_generalized_affine_image
  polyhedron1 1 Equal_RS e2 (Z.from_int 10);;
print_string_if_noisy "\n";;
print_string_if_noisy "Testing minimization";;
let is_bounded, num, den, is_supremum, gen
  = ppl_Polyhedron_minimize_with_point polyhedron1 e3;;
print_string_if_noisy "\n";;
print_string_if_noisy "Value: ";;
print_int_if_noisy(Z.to_int num);;
print_string_if_noisy "/";;
print_int_if_noisy(Z.to_int den);;
print_string_if_noisy ", is_bounded: ";;
print_string_if_noisy (string_of_bool is_bounded);;
print_string_if_noisy ", is_supremum: ";;
print_string_if_noisy (string_of_bool is_supremum);;
print_string_if_noisy ", generator: ";;
print_generator(gen);;
print_string_if_noisy "\n";;
let dimensions_to_remove = [3;0];;
ppl_Polyhedron_remove_space_dimensions polyhedron1 dimensions_to_remove;;
let dimensions_to_fold = [1];;
ppl_Polyhedron_fold_space_dimensions polyhedron1 dimensions_to_fold 0;;
let dimensions_to_map = [(0,1);(1,2);(2,0);];;
let i = ppl_Polyhedron_space_dimension polyhedron1;;
print_string_if_noisy "\n";;
print_string_if_noisy "Space dimension is: ";
print_int_if_noisy i;;
print_string_if_noisy "\n";;
ppl_Polyhedron_map_space_dimensions polyhedron1 dimensions_to_map;;
ppl_Polyhedron_constrains polyhedron1 1;;
ppl_Polyhedron_unconstrain_space_dimension polyhedron1 1;;
let dimensions_to_unconstrain = [1];;
ppl_Polyhedron_unconstrain_space_dimensions
  polyhedron1 dimensions_to_unconstrain;;
print_string_if_noisy "\n";;
ppl_Polyhedron_swap polyhedron1 polyhedron2;;
let result = ppl_Polyhedron_simplify_using_context_assign
  polyhedron1 polyhedron2;;
let c_polyhedron1 = ppl_new_C_Polyhedron_from_space_dimension 10 Universe;;
let constraints = ppl_Polyhedron_get_minimized_constraints c_polyhedron1;;
let cs_size =  List.length constraints;;
print_string_if_noisy "\nSize of minimized constraints get from a C_Polyhedron built from Universe : ";;
print_int_if_noisy(cs_size);;
let nnc_polyhedron1 = ppl_new_NNC_Polyhedron_from_space_dimension 10 Universe;;
let constraints = ppl_Polyhedron_get_minimized_constraints nnc_polyhedron1;;
let cs_size =  List.length constraints;;
print_string_if_noisy "\nSize of minimized constraints get from a NNC_Polyhedron built from Universe : ";;
print_int_if_noisy(cs_size);;
ppl_set_rounding_for_PPL ();;
ppl_restore_pre_PPL_rounding ();;
print_string_if_noisy "\n";;
print_string_if_noisy "Major version is: ";
print_int_if_noisy(ppl_version_major ());;
print_string_if_noisy ", minor version is: ";
print_int_if_noisy(ppl_version_minor ());;
print_string_if_noisy ", revision is: ";
print_int_if_noisy(ppl_version_revision ());;
print_string_if_noisy ", beta is: ";
print_int_if_noisy(ppl_version_beta ());;
print_string_if_noisy "\n";;
let v = ppl_version ();;
print_string_if_noisy "Version is: ";
print_string_if_noisy(v);;
let b = ppl_banner ();;
print_string_if_noisy "\n";;
print_string_if_noisy "Banner is: ";
print_string_if_noisy(b);;
print_string_if_noisy "\n\n";;
print_string_if_noisy "PPL Coefficient integer datatype is " ;;
if (ppl_Coefficient_is_bounded())
then print_string_if_noisy "bounded\n"
else print_string_if_noisy "unbounded\n" ;;
print_string_if_noisy "Maximum space dimension is: ";
let i = ppl_max_space_dimension()
in print_int_if_noisy i;;
print_string_if_noisy "\n";;
print_string_if_noisy "Irrational precision is: ";
let p = ppl_irrational_precision()
in print_int_if_noisy(p);;
print_string_if_noisy "\n";;

(* Testing exceptions *)
try
  let _ = ppl_new_MIP_Problem_from_space_dimension (-10)
  in print_string_if_noisy "Exception test failed"
with Invalid_argument what ->
  print_string_if_noisy "Exception test succeeded; caught exception is:\n";
  print_string_if_noisy what;
  print_string_if_noisy "\n";;

(* Testing timeouts *)
let lower = Coefficient(Gmp.Z.of_int 0)
and upper = Coefficient(Gmp.Z.of_int 1)
in let rec hypercube_cs dim =
  if dim < 0
  then []
  else Greater_Or_Equal(Variable dim, lower)
         :: Less_Or_Equal(Variable dim, upper)
           :: hypercube_cs (dim-1)
and hypercube_ph dim =
  ppl_new_C_Polyhedron_from_constraints (hypercube_cs dim)
and compute_timeout_hypercube dim_in dim_out =
  if dim_in < dim_out then (
    let _ = ppl_Polyhedron_get_minimized_constraints (hypercube_ph dim_in)
    in (
        print_string_if_noisy "Built hypercube of dimension ";
        print_int_if_noisy dim_in;
        print_string_if_noisy "\n"
    );
    compute_timeout_hypercube (dim_in + 1) dim_out
  )
in (
  begin
    (* FIXME: in the following try/with, the timeouts should be reset
       unconditionally. Is there a way to implement try/finally? *)
    try
      print_string_if_noisy "\nStarting ppl_reset_timeout test:\n";
      ppl_set_timeout 100;
      compute_timeout_hypercube 0 2;
      ppl_reset_timeout ();
      print_string_if_noisy "ppl_reset_timeout test succeeded.\n"
    with
    | PPL_timeout_exception ->
      ppl_reset_timeout ();
      print_string_if_noisy "ppl_reset_timeout test seems to be failed:\n";
      print_string_if_noisy "Unexpected PPL timeout exception caught.\n"
    | _ ->
      ppl_reset_timeout ();
      print_string_if_noisy "ppl_reset_timeout test seems to be failed.";
      (* FIXME: print the contents of the exception. *)
      print_string_if_noisy "\n"
  end;
  begin
    try
      print_string_if_noisy "\nStarting ppl_set_timeout test:\n";
      ppl_set_timeout 100;
      compute_timeout_hypercube 0 100;
      ppl_reset_timeout ();
      print_string_if_noisy "ppl_set_timeout test seems to be failed!\n"
    with
    | PPL_timeout_exception ->
      ppl_reset_timeout ();
      print_string_if_noisy "ppl_set_timeout test succeded\n";
      print_string_if_noisy "Expected PPL timeout exception caught.\n"
    | _ ->
      ppl_reset_timeout ();
      print_string_if_noisy "ppl_set_timeout test failed:\n";
      print_string_if_noisy "generic exception caught.\n"
  end;
  begin
    try
      print_string_if_noisy "\nStarting ppl_reset_deterministic_timeout test:\n";
      ppl_set_deterministic_timeout 2000 0;
      compute_timeout_hypercube 0 2;
      ppl_reset_deterministic_timeout ();
      print_string_if_noisy "ppl_reset_deterministic_timeout test succeeded.\n"
    with
    | PPL_timeout_exception ->
      ppl_reset_deterministic_timeout ();
      print_string_if_noisy "ppl_reset_deterministic_timeout test seems to be failed:\n";
      print_string_if_noisy "Unexpected PPL timeout exception caught.\n"
    | _ ->
      ppl_reset_deterministic_timeout ();
      print_string_if_noisy "ppl_reset_deterministic_timeout test seems to be failed.";
      (* FIXME: print the contents of the exception. *)
      print_string_if_noisy "\n"
  end;
  begin
    try
      print_string_if_noisy "\nStarting ppl_set_deterministic_timeout test:\n";
      ppl_set_deterministic_timeout 2000 0;
      compute_timeout_hypercube 0 100;
      ppl_reset_deterministic_timeout ();
      print_string_if_noisy "ppl_set_deterministic_timeout test seems to be failed!\n"
    with
    | PPL_timeout_exception ->
      ppl_reset_deterministic_timeout ();
      print_string_if_noisy "ppl_set_deterministic_timeout test succeded\n";
      print_string_if_noisy "Expected PPL timeout exception caught.\n"
    | _ ->
      ppl_reset_deterministic_timeout ();
      print_string_if_noisy "ppl_set_deterministic_timeout test failed:\n";
      print_string_if_noisy "generic exception caught.\n"
  end
);;


print_string_if_noisy "\nStarting PIP_Problem \n";;

print_string_if_noisy "PIP_Problem 1 \n";;
print_string_if_noisy "testing PIP_Problem_solution, ";;
print_string_if_noisy "PIP_Problem_OK ";;
print_string_if_noisy "and PIP_Tree_Node_is_bottom \n";;
let pip = ppl_new_PIP_Problem 3
             [e1 >=/ e2; e1 <=/ e2 -/ (linear_expression_of_int 7)] [2];;

begin
try
  let ptree = ppl_PIP_Problem_solution pip
  in let out = if (ppl_PIP_Problem_OK pip & ppl_PIP_Tree_Node_is_bottom ptree)
                 then "success" else "failed"
     in (print_string_if_noisy out)
with PPL_arithmetic_overflow what ->
  let unexpected = (ppl_Coefficient_bits() != 8)
  in if (unexpected)
       then raise (PPL_arithmetic_overflow(what));
  print_string_if_noisy "Expected overflow exception caught: ";
  print_string_if_noisy what;
  print_string_if_noisy "\n"
end;;
print_string_if_noisy "\n";;

print_string_if_noisy "PIP_Problem 2 \n";;
let params = [7];;
let pip2 = ppl_new_PIP_Problem 10 constraints1 params;;
let i = ppl_PIP_Problem_space_dimension pip2;;
let i = ppl_PIP_Problem_constraints pip2;;
print_string_if_noisy "PIP_Problem constraints are \n";;
List.iter print_constraint i;;
print_string_if_noisy "\n";;
ppl_PIP_Problem_add_constraint pip2 constraint1;;
ppl_PIP_Problem_add_constraints pip2 constraints1;;
let i = ppl_PIP_Problem_parameter_space_dimensions pip2;;
let i = ppl_PIP_Problem_get_control_parameter pip2 Cutting_Strategy;;
ppl_PIP_Problem_set_control_parameter pip2 Cutting_Strategy_First;;
let i = ppl_PIP_Problem_get_control_parameter pip2 Cutting_Strategy;;
let out = if (i == Cutting_Strategy_First)
  then "PIP Problem Control Parameter test succeeded"
  else "PIP Problem Control Parameter test failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
let b0 = ppl_PIP_Problem_has_big_parameter_dimension pip2;;
ppl_PIP_Problem_set_big_parameter_dimension pip2 7;;
let big_par = ppl_PIP_Problem_get_big_parameter_dimension pip2;;
let b1 = ppl_PIP_Problem_has_big_parameter_dimension pip2;;
let out = if (not b0 & b1 & (big_par = 7))
  then "ppl_PIP_Problem big parameter dimension tests succeeded"
  else "ppl_PIP_Problem big parameter dimension tests failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;

begin
try
  let out = if (ppl_PIP_Problem_is_satisfiable pip2)
    then "ppl_PIP_Problem_is_satisfiable test succeeded"
    else "ppl_PIP_Problem_is_satisfiable test failed"
      in (print_string_if_noisy out);
  print_string_if_noisy "\n";
  print_string_if_noisy "testing ppl_PIP_Problem_ascii_dump: ";
  print_string_if_noisy (ppl_PIP_Problem_ascii_dump pip2);
  print_string_if_noisy "\n";
  let ptree2 = ppl_PIP_Problem_solution pip2
  in let out2 = if (ppl_PIP_Tree_Node_is_bottom ptree2)
    then "ppl_PIP_Tree_Node_is_bottom test succeeded"
    else "ppl_PIP_Tree_Node_is_bottom test failed"
      in (print_string_if_noisy out2);
  print_string_if_noisy "\n"
with PPL_arithmetic_overflow what ->
  let unexpected = (ppl_Coefficient_bits() != 8)
  in if (unexpected)
       then raise (PPL_arithmetic_overflow(what));
  print_string_if_noisy "Expected overflow exception caught: ";
  print_string_if_noisy what;
  print_string_if_noisy "\n"
end;;

print_string_if_noisy "\nPIP_Problem 3 \n";;
let i = Variable 0
and j = Variable 1
and n = Variable 2
and m = Variable 3
and v2 = Z.from_int 2
and v3 = Z.from_int 3
and v4 = Z.from_int 4
and v8 = Z.from_int 8
and inhomogeneous n = linear_expression_of_int n
;;

let pip_c1 = v3 */ j +/ v2 */ i >=/ (inhomogeneous 8);;
print_constraint pip_c1; print_string_if_noisy "\n" ;;
let pip_c2 = v4 */ i -/ j >=/ (inhomogeneous 4);;
print_constraint pip_c2; print_string_if_noisy "\n" ;;
let pip_c3 = m >=/ j;;
print_constraint pip_c3; print_string_if_noisy "\n" ;;
let pip_c4 = n >=/ i;;
print_constraint pip_c4; print_string_if_noisy "\n" ;;
let pip_cs = [pip_c1; pip_c2; pip_c3; pip_c4] ;;

let ps = [2; 3];;
let pip3 = ppl_new_PIP_Problem 4 pip_cs ps;;

begin
try
  let result = ppl_PIP_Problem_solve pip3 in
  let out = if (result == Optimized_Pip_Problem)
    then "PIP Problem_solve pip3 test succeeded"
    else "PIP Problem_solve pip3 test failed"
      in (print_string_if_noisy out);
  print_string_if_noisy "\n";
  let node = ppl_PIP_Problem_solution pip3 in
  begin
  let out = if (ppl_PIP_Tree_Node_OK node)
    then "ppl_PIP_Tree_Node_OK node test succeeded"
    else "ppl_PIP_Tree_Node_OK node test failed"
      in (print_string_if_noisy out);
  print_string_if_noisy "\n";
  let out = if (ppl_PIP_Tree_Node_is_decision node)
    then "ppl_PIP_Tree_Node_is_decision node test succeeded"
    else "ppl_PIP_Tree_Node_is_decision node test failed"
      in (print_string_if_noisy out);
  print_string_if_noisy "\n";
  let out = if (ppl_PIP_Tree_Node_artificials node = [])
    then "ppl_PIP_Tree_Node_artificials node test succeeded"
    else "ppl_PIP_Tree_Node_artificials node test failed"
      in (print_string_if_noisy out);
  print_string_if_noisy "\n";
  let _node_cs = ppl_PIP_Tree_Node_constraints node in ();
  let
    _fchild = ppl_PIP_Tree_Node_false_child node
  and
    tchild = (ppl_PIP_Tree_Node_true_child node)
  in (
    let out = if (ppl_PIP_Tree_Node_is_decision tchild)
      then "ppl_PIP_Tree_Node_is_decision tchild test succeeded"
      else "ppl_PIP_Tree_Node_is_decision tchild test failed"
        in (print_string_if_noisy out);
    print_string_if_noisy "\n";
    let
      ttchild = (ppl_PIP_Tree_Node_true_child tchild)
    and
      ftchild = (ppl_PIP_Tree_Node_false_child tchild)
    in (
      let out = if (ppl_PIP_Tree_Node_is_solution ttchild)
        then "ppl_PIP_Tree_Node_is_decision ttchild test succeeded"
        else "ppl_PIP_Tree_Node_is_decision ttchild test failed"
          in (print_string_if_noisy out);
      print_string_if_noisy "\n";
      let _par_vals = ppl_PIP_Tree_Node_parametric_values ttchild 0 in ();
      let _ftchild_arts = ppl_PIP_Tree_Node_artificials ftchild in ()
    )
  );
  print_string_if_noisy "\n"
  end
with PPL_arithmetic_overflow what ->
  let unexpected = (ppl_Coefficient_bits() != 8)
  in if (unexpected)
       then raise (PPL_arithmetic_overflow(what));
  print_string_if_noisy "Expected overflow exception caught: ";
  print_string_if_noisy what;
  print_string_if_noisy "\n"
end;;

(* Pointset_Powerset_Grid is not enabled by default, the following code is *)
(* commented *)
(* let pps = ppl_new_Pointset_Powerset_Grid_from_space_dimension 3;; *)
(* let space_dim = ppl_Pointset_Powerset_Grid_space_dimension pps;; *)
(* ppl_Pointset_Powerset_Grid_add_constraints  pps constraints1;; *)
(* let caml_grid_it = ppl_Pointset_Powerset_Grid_begin_iterator pps;; *)
(* let grid1 =  ppl_Pointset_Powerset_Grid_iterator_get_disjunct caml_grid_it;; *)
(* let space_dim = ppl_Grid_space_dimension grid1;; *)
(* let grid2 = ppl_new_Grid_from_space_dimension 3;; *)
(* ppl_Pointset_Powerset_Grid_add_disjunct pps grid2;; *)
(* let space_dim = ppl_Pointset_Powerset_Grid_space_dimension pps;; *)
(* let caml_grid_it1 = ppl_Pointset_Powerset_Grid_end_iterator pps;; *)
(* let caml_grid_it2 = ppl_Pointset_Powerset_Grid_end_iterator pps;; *)
(* ppl_Pointset_Powerset_Grid_iterator_decrement caml_grid_it1;; *)
(* ppl_Pointset_Powerset_Grid_drop_disjunct pps caml_grid_it1;; *)
(* ppl_Pointset_Powerset_Grid_iterator_equals_iterator caml_grid_it1 caml_grid_it1;; *)
(* print_int_if_noisy space_dim;; *)
(* print_string_if_noisy "\n";; *)
(* print_string_if_noisy "PPS size : ";; *)
(* let size = ppl_Pointset_Powerset_Grid_size pps;; *)
(* print_int_if_noisy size;; *)

at_exit Gc.full_major;;
print_string_if_noisy "\nBye!\n"
