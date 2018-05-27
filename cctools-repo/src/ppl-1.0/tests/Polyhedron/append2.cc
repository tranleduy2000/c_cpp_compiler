/* An example of iteration to a post-fixpoint.
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

#include "ppl_test.hh"

namespace {

void
shift_rename_add(const C_Polyhedron& p,
		 dimension_type offset,
		 C_Polyhedron& q) {
  C_Polyhedron r(offset);
  r.concatenate_assign(p);
  q.intersection_assign(r);
}

void
append_init(C_Polyhedron& base, C_Polyhedron& induct, C_Polyhedron& expect,
            dimension_type& offset, unsigned int& arity,
	    unsigned int& num_vars) {
  offset = 3;
  arity = 3;
  num_vars = 6;
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);

  // This is the base case:
  // append(A,B,C) :- A = [], B = C.
  base.add_space_dimensions_and_embed(3);
  base.add_constraint(A == 0);
  base.add_constraint(B >= 0);
  base.add_constraint(C == B);

  print_constraints(base, "*** base ***");

  // This is the inductive case:
  // append(A,B,C) :- A = [X|D], B = E, C = [X|F], append(D,E,F).
  induct.add_space_dimensions_and_embed(6);
  induct.add_constraint(A + F == C + D);
  induct.add_constraint(B == E);
  induct.add_constraint(C + D >= A);
  induct.add_constraint(D >= 0);
  induct.add_constraint(B >= 0);
  induct.add_constraint(A >= D + 1);

  print_constraints(induct, "*** inductive ***");

  expect.add_space_dimensions_and_embed(3);
  expect.add_constraint(A + B == C);
  expect.add_constraint(B >= 0);
  expect.add_constraint(C >= B);
}

void
fix_point(C_Polyhedron& start, C_Polyhedron& induct, C_Polyhedron& finish,
          dimension_type offset, unsigned int arity, unsigned int num_vars) {
  // Initialize the fixpoint iteration.
  C_Polyhedron current = start;

  print_constraints(current, "*** start ***");

  // Contains the polyhedron computed at the previous iteration.
  C_Polyhedron previous;
  do {
    previous = current;
    current = induct;
    shift_rename_add(previous, offset, current);

    print_constraints(current, "*** after shift_rename_add ***");

    Variables_Set dimensions_to_remove;
    for (unsigned int i = num_vars-1 ; i >= arity; --i )
      dimensions_to_remove.insert(Variable(i));
    current.remove_space_dimensions(dimensions_to_remove);

    print_constraints(current, "*** after remove_space_dimensions ***");

    current.upper_bound_assign(previous);
    (void) current.is_empty();

    print_constraints(current, "*** after upper_bound_assign ***");

  } while (current != previous);
  finish = current;
}

} // namespace

bool
test01() {
  C_Polyhedron start;
  C_Polyhedron induct;
  C_Polyhedron expect;
  dimension_type recursive_offset;
  unsigned int arity;
  unsigned int num_vars;
  append_init(start, induct, expect, recursive_offset, arity, num_vars);
  C_Polyhedron final;
  fix_point(start, induct, final, recursive_offset, arity, num_vars);

  print_constraints(expect, "*** expected ***");

  return final == expect ? true : false;
}

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN
