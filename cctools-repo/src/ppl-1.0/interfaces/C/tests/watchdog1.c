/* Test the timeout facility of the PPL C interface library.
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
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

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
  if (check_noisy() || check_very_noisy())
    fprintf(stderr, "PPL error code %d: %s\n", code, description);
#if !PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED
  /* If Watchdog objects are not supported, a logic error will occur:
     this is normal. */
  if (code == PPL_ERROR_LOGIC_ERROR)
    my_exit(0);
#endif /* !PPL_WATCHDOG_OBJECTS_ARE_SUPPORTED */
}

void
open_hypercube(int dimension, ppl_Polyhedron_t ph) {
  int i;
  mpz_t z_one;
  mpz_t z_minus_one;
  ppl_Coefficient_t coeff_one;
  ppl_Coefficient_t coeff_minus_one;
  ppl_Linear_Expression_t le;
  ppl_Constraint_t c;
  ppl_Constraint_System_t cs;

  mpz_init_set_si(z_one, 1);
  mpz_init_set_si(z_minus_one, -1);
  ppl_new_Coefficient(&coeff_one);
  ppl_assign_Coefficient_from_mpz_t(coeff_one, z_one);
  ppl_new_Coefficient(&coeff_minus_one);
  ppl_assign_Coefficient_from_mpz_t(coeff_minus_one, z_minus_one);
  ppl_new_Linear_Expression_with_dimension(&le, dimension);
  ppl_new_Constraint_System(&cs);
  for (i = 0; i < dimension; ++i) {
    ppl_Linear_Expression_add_to_coefficient(le, i, coeff_one);
    /* Variable(i) > 0 */
    ppl_new_Constraint(&c, le, PPL_CONSTRAINT_TYPE_GREATER_THAN);
    ppl_Constraint_System_insert_Constraint(cs, c);
    ppl_delete_Constraint(c);
    /* Variable(i) < 1 */
    ppl_Linear_Expression_add_to_inhomogeneous(le, coeff_minus_one);
    ppl_new_Constraint(&c, le, PPL_CONSTRAINT_TYPE_LESS_THAN);
    ppl_Constraint_System_insert_Constraint(cs, c);
    ppl_delete_Constraint(c);
    /* Zero `le' */
    ppl_Linear_Expression_add_to_coefficient(le, i, coeff_minus_one);
    ppl_Linear_Expression_add_to_inhomogeneous(le, coeff_one);
  }
  ppl_Polyhedron_add_constraints(ph, cs);
  ppl_delete_Constraint_System(cs);
  ppl_delete_Linear_Expression(le);
  ppl_delete_Coefficient(coeff_minus_one);
  ppl_delete_Coefficient(coeff_one);
  mpz_clear(z_minus_one);
  mpz_clear(z_one);
}

void
timed_compute_open_hypercube_generators(int csecs, int max_dimension) {
  int i;
  int result;
  ppl_const_Generator_System_t gs;
  ppl_Polyhedron_t ph;

  for (i = 0; i <= max_dimension; ++i) {
    ppl_new_NNC_Polyhedron_from_space_dimension(&ph, i, 0);
    open_hypercube(i, ph);
    ppl_set_timeout(csecs);
    result = ppl_Polyhedron_get_generators(ph, &gs);
    ppl_reset_timeout();
    ppl_delete_Polyhedron(ph);
    if (result == PPL_TIMEOUT_EXCEPTION)
      /* Timeout expired */
      return;
    else if (result != 0)
      /* Unexpected error */
      exit(1);
  }
  /* Should not reach this point */
  exit(1);
}

int
main(int argc, char **argv) {
  program_name = argv[0];

  if (argc != 1) {
    fprintf(stderr, "usage: %s\n", program_name);
    exit(1);
  }

  if (ppl_initialize() < 0)
    fatal("cannot initialize the Parma Polyhedra Library");

  if (ppl_set_error_handler(error_handler) < 0)
    fatal("cannot install the custom error handler");

  timed_compute_open_hypercube_generators(200, 20);

  ppl_finalize();
  return 0;
}

