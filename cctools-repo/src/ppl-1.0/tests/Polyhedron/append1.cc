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

} // namespace

bool
test01() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);

  // This is the base case:
  // append(A,B,C) :- A = [], B = C.
  C_Polyhedron base(3);
  base.add_constraint(A == 0);
  base.add_constraint(B >= 0);
  base.add_constraint(C == B);

  print_constraints(base, "*** base ***");

  // This is the inductive case:
  // append(A,B,C) :- A = [X|D], B = E, C = [X|F], append(D,E,F).
  C_Polyhedron inductive(6);
  inductive.add_constraint(A + F == C + D);
  inductive.add_constraint(B == E);
  inductive.add_constraint(C + D >= A);
  inductive.add_constraint(D >= 0);
  inductive.add_constraint(B >= 0);
  inductive.add_constraint(A >= D + 1);

  print_constraints(inductive, "*** inductive ***");

  // Initialize the fixpoint iteration.
  C_Polyhedron current = base;

  print_constraints(current, "*** start ***");

  // Contains the polyhedron computed at the previous iteration.
  C_Polyhedron previous;
  do {
    previous = current;
    current = inductive;
    shift_rename_add(previous, 3, current);

    print_constraints(current, "*** after shift_rename_add ***");

    Variables_Set dimensions_to_remove;
    // Deliberately inserted out of order (!).
    dimensions_to_remove.insert(D);
    dimensions_to_remove.insert(F);
    dimensions_to_remove.insert(E);
    current.remove_space_dimensions(dimensions_to_remove);

    print_constraints(current, "*** after remove_space_dimensions ***");

    current.upper_bound_assign(previous);
    (void) current.is_empty();

    print_constraints(current, "*** after upper_bound_assign ***");

  } while (current != previous);

  C_Polyhedron expected(3);
  expected.add_constraint(A + B == C);
  expected.add_constraint(B >= 0);
  expected.add_constraint(C >= B);

  print_constraints(expected, "*** expected ***");

  return current == expected ? true : false;
}

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN


