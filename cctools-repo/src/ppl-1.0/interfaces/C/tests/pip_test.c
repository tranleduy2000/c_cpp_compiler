/* Test the use of the PPL PIP solver from C code.
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
site: http://bugseng.com/products/ppl/ . */

#include "ppl_c_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static const char* program_name = 0;

static void
my_exit(int status) {
  (void) ppl_finalize();
  exit(status);
}

static void
fatal(const char* format, ...) {
  va_list ap;
  fprintf(stderr, "%s: ", program_name);
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  my_exit(1);
}

static void
error_handler(enum ppl_enum_error_code code,
	      const char* description) {
  fatal("PPL error code %d: %s", code, description);
}

static void
display_solution_i(ppl_const_PIP_Tree_Node_t node,
                   ppl_dimension_type n_vars,
                   ppl_dimension_type n_params,
                   const ppl_dimension_type vars[],
                   const ppl_dimension_type parameters[],
                   int indent) {
  if (!node) {
    /* A null pointer indicates the polyhedron is empty. */
    printf("%*s_|_\n", indent*2, "");
  }
  else {
    ppl_dimension_type space_dimension = n_vars + n_params;
    ppl_dimension_type new_params;
    ppl_const_Constraint_System_t constraints;
    ppl_const_PIP_Decision_Node_t dn;
    int constraints_empty;
    ppl_PIP_Tree_Node_number_of_artificials(node, &new_params);
    if (new_params > 0) {
      /* Display the artificial parameters. */
      ppl_Artificial_Parameter_Sequence_const_iterator_t i, i_end;
      ppl_new_Artificial_Parameter_Sequence_const_iterator(&i);
      ppl_new_Artificial_Parameter_Sequence_const_iterator(&i_end);
      ppl_PIP_Tree_Node_begin(node, i);
      ppl_PIP_Tree_Node_end(node, i_end);
      while (!ppl_Artificial_Parameter_Sequence_const_iterator_equal_test
              (i,i_end)) {
        ppl_const_Artificial_Parameter_t ap;
        ppl_Artificial_Parameter_Sequence_const_iterator_dereference(i, &ap);
        printf("%*sParameter ", indent*2, "");
        ppl_io_print_variable(space_dimension++);
        printf(" = ");
        ppl_io_print_Artificial_Parameter(ap);
        printf("\n");
        ppl_Artificial_Parameter_Sequence_const_iterator_increment(i);
      }
      ppl_delete_Artificial_Parameter_Sequence_const_iterator(i_end);
      ppl_delete_Artificial_Parameter_Sequence_const_iterator(i);
    }

    ppl_PIP_Tree_Node_get_constraints(node, &constraints);
    constraints_empty = ppl_Constraint_System_empty(constraints);
    if (!constraints_empty) {
      /* Display the constraints on the parameters. */
      int notfirst = 0;
      ppl_Constraint_System_const_iterator_t end, i;
      ppl_new_Constraint_System_const_iterator(&i);
      ppl_new_Constraint_System_const_iterator(&end);
      ppl_Constraint_System_begin(constraints, i);
      ppl_Constraint_System_end(constraints, end);
      printf("%*sif ", indent*2, "");
      for (; !ppl_Constraint_System_const_iterator_equal_test(i,end);
           ppl_Constraint_System_const_iterator_increment(i)) {
        ppl_const_Constraint_t c;
        if (notfirst)
          printf(" and ");
        ppl_Constraint_System_const_iterator_dereference(i, &c);
        ppl_io_print_Constraint(c);
        notfirst = 1;
      }
      printf(" then\n");
    }
    ppl_PIP_Tree_Node_as_decision(node, &dn);
    if (dn) {
      /* The node is a decision Node: display the children nodes. */
      ppl_const_PIP_Tree_Node_t child;
      ppl_PIP_Decision_Node_get_child_node(dn, 1, &child);
      display_solution_i(child, n_vars, n_params, vars, parameters, indent+1);
      printf("%*selse\n", indent*2, "");
      ppl_PIP_Decision_Node_get_child_node(dn, 0, &child);
      display_solution_i(child, n_vars, n_params, vars, parameters, indent+1);
    }
    else {
      /* The node is a solution Node: display the expression of the vars. */
      int notfirst = 0;
      ppl_const_PIP_Solution_Node_t sn;
      ppl_dimension_type i;
      ppl_const_Linear_Expression_t le;
      ppl_PIP_Tree_Node_as_solution(node, &sn);
      printf("%*s{", indent*2+(constraints_empty?0:2), "");
      for (i=0; i<n_vars; ++i) {
        if (notfirst)
          printf(" ; ");
        ppl_PIP_Solution_Node_get_parametric_values(sn, vars[i], &le);
        ppl_io_print_Linear_Expression(le);
        notfirst = 1;
      }
      printf("}\n");
      if (!constraints_empty)
        printf("%*selse\n%*s_|_\n", indent*2, "", indent*2+2, "");
    }
  }
}

static void
display_solution(ppl_const_PIP_Tree_Node_t node,
                 ppl_dimension_type n_vars,
                 ppl_dimension_type n_params,
                 const ppl_dimension_type params[]) {
  ppl_dimension_type* vars;
  ppl_dimension_type i, j, k, dim;
  vars = malloc(n_vars*sizeof(ppl_dimension_type));
  dim = n_vars+n_params;

  /* Construct the array of variable space indices. This requires the
     params array to be sorted in ascending order. */
  for (i = 0, j = 0, k = 0; i < dim; ++i) {
    if (k == n_params || i < params[k])
      vars[j++] = i;
    else
      ++k;
  }
  display_solution_i(node, n_vars, n_params, vars, params, 0);
  free(vars);
}

#define N_VARS 2
#define N_PARAMETERS 2
#define N_CONSTRAINTS 4

int
main(int argc, char **argv) {
  ppl_PIP_Problem_t pip;
  ppl_Constraint_t ct;
  ppl_Coefficient_t c;
  ppl_Linear_Expression_t le;
  ppl_dimension_type i, j;
  mpz_t mpc;
  int ok;
  size_t sz = 0;

  static ppl_dimension_type parameter_dim[N_PARAMETERS];

  static int coef[N_CONSTRAINTS][N_VARS+N_PARAMETERS+1] = {
    {  2,  3,  0,  0, -8 },
    {  4, -1,  0,  0, -4 },
    {  0, -1,  0,  1,  0 },
    { -1,  0,  1,  0,  0 },
  };

  program_name = argv[0];

  if (argc != 1) {
    fprintf(stderr, "usage: %s\n", program_name);
    exit(1);
  }

  if (ppl_initialize() < 0)
    fatal("cannot initialize the Parma Polyhedra Library");

  if (ppl_set_error_handler(error_handler) < 0)
    fatal("cannot install the custom error handler");

  mpz_init(mpc);

  /* At least 32-bit coefficients are needed. */
  if (ppl_Coefficient_max(mpc) > 0 && mpz_fits_sshort_p(mpc)) {
    mpz_clear(mpc);
    ppl_finalize();
    return 0;
  }

  for (i = 0; i < N_PARAMETERS; ++i)
    parameter_dim[i] = i + N_VARS;

  ppl_new_PIP_Problem_from_space_dimension(&pip, N_VARS+N_PARAMETERS);
  ppl_PIP_Problem_add_to_parameter_space_dimensions(pip, parameter_dim,
                                                    N_PARAMETERS);
  ppl_new_Coefficient(&c);
  for (i = 0; i < N_CONSTRAINTS; ++i) {
    ppl_new_Linear_Expression(&le);
    for (j = 0; j < N_VARS+N_PARAMETERS; ++j) {
      mpz_set_si(mpc, coef[i][j]);
      ppl_assign_Coefficient_from_mpz_t(c, mpc);
      ppl_Linear_Expression_add_to_coefficient(le, j, c);
    }
    mpz_set_si(mpc, coef[i][j]);
    ppl_assign_Coefficient_from_mpz_t(c, mpc);
    ppl_Linear_Expression_add_to_inhomogeneous(le, c);
    ppl_new_Constraint(&ct, le, PPL_CONSTRAINT_TYPE_GREATER_OR_EQUAL);
    ppl_PIP_Problem_add_constraint(pip, ct);
    ppl_delete_Linear_Expression(le);
  }
  ppl_delete_Coefficient(c);
  mpz_clear(mpc);

  ppl_PIP_Problem_total_memory_in_bytes(pip, &sz);
  ppl_PIP_Problem_external_memory_in_bytes(pip, &sz);

  ok = (ppl_PIP_Problem_solve(pip) == PPL_PIP_PROBLEM_STATUS_OPTIMIZED);
  if (ok) {
    ppl_dimension_type dim;
    ppl_const_PIP_Tree_Node_t solution;
    ppl_PIP_Problem_t pip0;
    ppl_Constraint_System_t constraints;
    ppl_Constraint_System_const_iterator_t begin, end;

    ppl_PIP_Problem_space_dimension(pip, &dim);
    ppl_PIP_Problem_solution(pip, &solution);
    if (check_noisy() || check_very_noisy())
      display_solution(solution, N_VARS, N_PARAMETERS, parameter_dim);
    ppl_new_Constraint_System_const_iterator(&begin);
    ppl_new_Constraint_System_const_iterator(&end);
    ppl_new_Constraint_System_from_Constraint(&constraints, ct);
    ppl_Constraint_System_begin(constraints, begin);
    ppl_Constraint_System_end(constraints, end);
    ppl_new_PIP_Problem_from_constraints(&pip0, N_VARS+N_PARAMETERS,
                                         begin, end, N_PARAMETERS,
                                         parameter_dim);
    ok = ppl_PIP_Problem_OK(pip0);
    ppl_delete_Constraint(ct);
    ppl_delete_Constraint_System(constraints);
  }

  ppl_delete_PIP_Problem(pip);
  ppl_finalize();
  return ok ? 0 : 1;
}
