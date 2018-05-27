/* Definitions of print_ppl_*_to_buffer() functions.
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

#include "ppl_c.h"
#include "print_to_buffer.h"
#include <stdlib.h>
#include <string.h>

#define DEFINE_PRINT_TO_BUFFER(Type)                                    \
char*                                                                   \
 print_ppl_##Type##_to_buffer(ppl_const_##Type##_t p,			\
			      unsigned indent_depth,			\
			      unsigned preferred_first_line_length,	\
			      unsigned preferred_line_length) {		\
  char *buf, *ret;							\
  int r = ppl_io_asprint_##Type(&buf, p);				\
  if (r != 0)								\
    return 0;								\
  ret = ppl_io_wrap_string(buf, indent_depth,				\
			   preferred_first_line_length,			\
			   preferred_line_length);			\
  free(buf);								\
  return ret;								\
}

DEFINE_PRINT_TO_BUFFER(Coefficient)

DEFINE_PRINT_TO_BUFFER(Linear_Expression)

DEFINE_PRINT_TO_BUFFER(Constraint)

DEFINE_PRINT_TO_BUFFER(Constraint_System)

DEFINE_PRINT_TO_BUFFER(Generator)

DEFINE_PRINT_TO_BUFFER(Generator_System)

DEFINE_PRINT_TO_BUFFER(Congruence)

DEFINE_PRINT_TO_BUFFER(Congruence_System)

DEFINE_PRINT_TO_BUFFER(Grid_Generator)

DEFINE_PRINT_TO_BUFFER(Grid_Generator_System)

DEFINE_PRINT_TO_BUFFER(MIP_Problem)

DEFINE_PRINT_TO_BUFFER(PIP_Problem)
