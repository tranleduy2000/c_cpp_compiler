/* Test the use of C++ output facilities from C code.
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

#include "print_to_buffer.h"
#include <stdio.h>

#define DIMENSION 100

int
main() {
  int i;
  mpz_t z;
  ppl_Coefficient_t coeff;
  ppl_Linear_Expression_t le;
  ppl_Constraint_t c;
  ppl_Constraint_System_t cs;
  char* p;

  mpz_init_set_si(z, 1);
  ppl_initialize();
  ppl_new_Coefficient(&coeff);
  ppl_assign_Coefficient_from_mpz_t(coeff, z);
  ppl_new_Constraint_System(&cs);
  for (i = 0; i < DIMENSION; ++i) {
    ppl_new_Linear_Expression_with_dimension(&le, DIMENSION);
    ppl_Linear_Expression_add_to_coefficient(le, i, coeff);
    ppl_new_Constraint(&c, le, PPL_CONSTRAINT_TYPE_GREATER_OR_EQUAL);
    ppl_Constraint_System_insert_Constraint(cs, c);
    ppl_delete_Constraint(c);
    ppl_delete_Linear_Expression(le);
  }
  p = print_ppl_Constraint_System_to_buffer(cs, 4, 64, 64);
  if (check_noisy()) {
    if (p == 0)
      printf("print_ppl_Constraint_System_to_buffer() returned NULL!\n");
    else
      printf("    %s\n", p);
  }
  ppl_delete_Constraint_System(cs);
  ppl_finalize();
  return 0;
}
