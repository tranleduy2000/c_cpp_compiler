/* Declarations of print_ppl_*_to_buffer() functions.
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

#ifndef PPL_print_to_buffer_h
#define PPL_print_to_buffer_h 1

#include "ppl_c.h"

/*
  Returns a buffer allocated with malloc() containing a printable
  representation of the PPL object referenced by `p', where each
  newline is followed by `indent_depth' blank spaces and the preferred
  maximum length of the first and the following lines are given by
  `preferred_first_line_length' and `preferred_line_length',
  respectively.
*/
#define DECLARE_PRINT_TO_BUFFER(Type)					\
char*                                                                   \
print_ppl_##Type##_to_buffer(ppl_const_##Type##_t p,			\
			     unsigned indent_depth,			\
			     unsigned preferred_first_line_length,	\
			     unsigned preferred_line_length);

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_PRINT_TO_BUFFER(Coefficient)

DECLARE_PRINT_TO_BUFFER(Linear_Expression)

DECLARE_PRINT_TO_BUFFER(Constraint)

DECLARE_PRINT_TO_BUFFER(Constraint_System)

DECLARE_PRINT_TO_BUFFER(Constraint_System_const_iterator)

DECLARE_PRINT_TO_BUFFER(Generator)

DECLARE_PRINT_TO_BUFFER(Generator_System)

DECLARE_PRINT_TO_BUFFER(Generator_System_const_iterator)

DECLARE_PRINT_TO_BUFFER(Congruence)

DECLARE_PRINT_TO_BUFFER(Congruence_System)

DECLARE_PRINT_TO_BUFFER(Grid_Generator)

DECLARE_PRINT_TO_BUFFER(Grid_Generator_System)

DECLARE_PRINT_TO_BUFFER(MIP_Problem)

DECLARE_PRINT_TO_BUFFER(PIP_Problem)

#ifdef __cplusplus
} /* extern "C" */
#endif

#undef DECLARE_PRINT_TO_BUFFER

#endif /* !defined(PPL_print_to_buffer_h) */
