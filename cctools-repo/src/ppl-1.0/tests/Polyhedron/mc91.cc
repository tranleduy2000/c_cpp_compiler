/* Test the Pointset_Powerset construction with McCarthy's 91 function.
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

bool
test01() {
  typedef Pointset_Powerset<C_Polyhedron> PCS;

  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);

  // This is the base case:
  // mc91(A, B) :- A >= 101, B = A-10.
  C_Polyhedron base_ph(2);
  base_ph.add_constraint(A >= 101);
  base_ph.add_constraint(B == A-10);
  PCS base(2, EMPTY);
  base.add_disjunct(base_ph);

  print_constraints(base, "*** base ***");

  // This is the inductive case:
  // mc91(A, B) :- A =< 100, C = A+11, E = D, F = B, mc91(C, D), mc91(E, F).
  C_Polyhedron inductive_ph(6);
  inductive_ph.add_constraint(A <= 100);
  inductive_ph.add_constraint(C == A+11);
  inductive_ph.add_constraint(E == D);
  inductive_ph.add_constraint(F == B);
  PCS inductive(6, EMPTY);
  inductive.add_disjunct(inductive_ph);

  print_constraints(inductive, "*** inductive ***");

  // Initialize the fixpoint iteration.
  PCS current = base;

  print_constraints(current, "*** start ***");

  // Contains the description computed at the previous iteration.
  PCS previous;
  do {
    previous = current;
    current = inductive;
    Pointset_Powerset<C_Polyhedron> b1(2);
    b1.concatenate_assign(previous);
    b1.add_space_dimensions_and_embed(2);
    current.intersection_assign(b1);
    Pointset_Powerset<C_Polyhedron> b2(4);
    b2.concatenate_assign(previous);
    current.intersection_assign(b2);

    print_constraints(current, "*** after body solving ***");

    Variables_Set dimensions_to_remove;
    // Deliberately inserted out of order (!).
    dimensions_to_remove.insert(D);
    dimensions_to_remove.insert(C);
    dimensions_to_remove.insert(F);
    dimensions_to_remove.insert(E);
    current.remove_space_dimensions(dimensions_to_remove);

    print_constraints(current, "*** after remove_space_dimensions ***");

    current.least_upper_bound_assign(previous);

    current.BHZ03_widening_assign<BHRZ03_Certificate>
      (previous, widen_fun_ref(&Polyhedron::H79_widening_assign));

    print_constraints(current, "*** after lub+widening ***");

  } while (current != previous);

  C_Polyhedron expected_ph(2);
  expected_ph.add_constraint(A - B <= 10);
  expected_ph.add_constraint(B >= 91);
  Pointset_Powerset<C_Polyhedron> expected(2, EMPTY);
  expected.add_disjunct(expected_ph);

  bool ok = (expected == current);

  print_constraints(expected, "*** expected ***");
  print_constraints(current, "*** final result ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
END_MAIN
