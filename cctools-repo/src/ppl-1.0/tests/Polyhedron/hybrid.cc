/* Test three examples for linear hybrid systems.  These are based on
   the examples in HalbwachsPR94 and HalbwachsPR97 papers.
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

// The water monitor example.
// Note that we have had to add a closure operator as an action
// for each of the transitions. Without this, the results are empty.
bool
test01() {
  Variable w(0);
  Variable x(1);
  NNC_Polyhedron ph0(2, EMPTY);
  NNC_Polyhedron ph0_prev(2, EMPTY);
  NNC_Polyhedron ph1(2, EMPTY);
  NNC_Polyhedron ph2(2, EMPTY);
  NNC_Polyhedron ph3(2, EMPTY);

  NNC_Polyhedron ph0_init(2);
  ph0_init.add_constraint(w == 1);

  NNC_Polyhedron ph_tea01(2);
  ph_tea01.add_constraint(w == 1);
  ph_tea01.add_constraint(x == 1);

  NNC_Polyhedron ph_tea23(2);
  ph_tea23.add_constraint(w == -2);
  ph_tea23.add_constraint(x == 1);

  NNC_Polyhedron ph_tmp(2);

  while (true) {
    ph0_prev = ph0;

    // ph0
    ph0 = ph0_init;
    ph_tmp = ph3;
    // We must close the constraints since the values of x and w
    // increase continuously in the transition from l3 to l0.
    ph_tmp.topological_closure_assign();
    // Add constraint for the transition from l3 to l0.
    ph_tmp.add_constraint(x == 2);
    // Add invariant constraint for l0.
    ph_tmp.add_constraint(w < 10);
    // Join with the Init(l0).
    ph0.upper_bound_assign(ph_tmp);
    // Time elaspe for l0.
    ph0.time_elapse_assign(ph_tea01);
    // Add invariant constraint for l0.
    ph0.add_constraint(w < 10);

    ph0.H79_widening_assign(ph0_prev);

    if (ph0 == ph0_prev) {

      NNC_Polyhedron known_result0(2);
      known_result0.add_constraint(1 <= w);
      known_result0.add_constraint(w < 10);
      bool ok = (ph0 == known_result0);

      print_constraints(ph0, "*** ph0 final ***");

      NNC_Polyhedron known_result1(2);
      known_result1.add_constraint(w - x == 10);
      known_result1.add_constraint(x >= 0);
      known_result1.add_constraint(x < 2);
      ok = ok && (ph1 == known_result1);

      print_constraints(ph1, "*** ph1 final ***");

      NNC_Polyhedron known_result2(2);
      known_result2.add_constraint(w + 2*x == 16);
      known_result2.add_constraint(x >= 2);
      known_result2.add_constraint(w > 5);
      ok = ok && (ph2 == known_result2);

      print_constraints(ph2, "*** ph2 final ***");

      NNC_Polyhedron known_result3(2);
      known_result3.add_constraint(w + 2*x == 5);
      known_result3.add_constraint(w <= 5);
      known_result3.add_constraint(w > 1);
      ok = ok && (ph3 == known_result3);

      print_constraints(ph3, "*** ph3 final ***");

      return ok;
    }

    print_constraints(ph0, "*** ph0 ***");
    // ph1
    // The only incoming transition is from l0.
    ph1 = ph0;
    // We must close the constraints since the values of x and w
    // increase continuously in the transition from l0 to l1.
    ph1.topological_closure_assign();
    // Add constraint for the transition from l0 to l1.
    ph1.add_constraint(w == 10);
    // Do the action for the transition from l0 to l1.
    ph1.affine_image(x, 0*x);
    // Add invariant constraint for l1.
    ph1.add_constraint(x < 2);
    // Time elapse for l1.
    ph1.time_elapse_assign(ph_tea01);
    // Add invariant constraint for l1.
    print_constraints(ph1, "*** ph1 before time elapse ***");
    // Add invariant constraint for l1.
    ph1.add_constraint(x < 2);

    // The only incoming transition is from l1.
    ph2 = ph1;
    // We must close the constraints since the values of x and w
    // increase continuously in the transition from l1 to l2.
    ph2.topological_closure_assign();
    // Add constraint for the transition from l1 to l2.
    ph2.add_constraint(x == 2);
    // There is no action for the transition from l1 to l2.
    // add invariant constraint for l2.
    ph2.add_constraint(w > 5);
    // Time elaspe for l2.
    ph2.time_elapse_assign(ph_tea23);
    // Add invariant constraint for l2.
    ph2.add_constraint(w > 5);

    // The only incoming transition is from l2.
    ph3 = ph2;
    // We must close the constraints since the values of x and w
    // increase continuously in the transition from l2 to l3.
    ph3.topological_closure_assign();
    // Add constraint for the transition from l2 to l3.
    ph3.add_constraint(w == 5);
    // Do the action for the transition from l2 to l3.
    ph3.affine_image(x, 0*x);
    // Add invariant constraint for l3.
    ph3.add_constraint(x < 2);
    // Time elaspe for l3.
    ph3.time_elapse_assign(ph_tea23);
    // Add invariant constraint for l3.
    ph3.add_constraint(x < 2);
  }
  return false;
}

// The Fischer protocol example where we make the exclusion variable
// one of the variables of the automaton.
bool
test02() {
  Variable x1(0);
  Variable x2(1);
  Variable a(2);
  Variable b(3);
  Variable k(4);

  NNC_Polyhedron ph0(5, EMPTY);
  NNC_Polyhedron ph1(5, EMPTY);
  NNC_Polyhedron ph2(5, EMPTY);
  NNC_Polyhedron ph3(5, EMPTY);
  NNC_Polyhedron ph4(5, EMPTY);
  NNC_Polyhedron ph5(5, EMPTY);

  NNC_Polyhedron ph0_Init(5);
  ph0_Init.add_constraint(a >= 0);
  ph0_Init.add_constraint(b >= 0);
  ph0_Init.add_constraint(k >= 0);
  ph0_Init.add_constraint(k <= 2);

  NNC_Polyhedron ph0_prev(5, EMPTY);

  // The derivative polyhedron is the same at all locations.
  NNC_Polyhedron ph_tea(5);
  ph_tea.add_constraint(a == 0);
  ph_tea.add_constraint(b == 0);
  ph_tea.add_constraint(x1 == 1);
  ph_tea.add_constraint(10*x2 >= 9);
  ph_tea.add_constraint(10*x2 <= 11);

  NNC_Polyhedron ph_tmp(5);

  while (true) {
    //    nout << "next iteration" << "\n";

    // Location l0.
    ph0 = ph0_Init;
    ph_tmp = ph3;
    ph_tmp.add_constraint(x1 >= b);
    ph_tmp.add_constraint(k >= 0);
    ph_tmp.add_constraint(k <= 2);
    ph0.upper_bound_assign(ph_tmp);
    ph_tmp = ph4;
    ph_tmp.affine_image(k, Linear_Expression(0));
    ph_tmp.add_constraint(k >= 0);
    ph_tmp.add_constraint(k <= 2);
    ph0.upper_bound_assign(ph_tmp);
    ph_tmp = ph5;
    ph_tmp.add_constraint(k >= 0);
    ph_tmp.add_constraint(k <= 2);
    ph0.upper_bound_assign(ph_tmp);
    ph0.time_elapse_assign(ph_tea);
    ph0.add_constraint(k >= 0);
    ph0.add_constraint(k <= 2);

    // Location l1.
    ph1 = ph0;
    ph1.add_constraint(k == 0);
    ph1.affine_image(x1, Linear_Expression(0));
    ph1.add_constraint(x1 <= a);
    ph1.add_constraint(k == 0);
    ph1.time_elapse_assign(ph_tea);
    ph1.add_constraint(k == 0);
    ph1.add_constraint(x1 <= a);

    // Location l2.
    ph2 = ph1;
    ph2.affine_image(x1, Linear_Expression(0));
    ph2.affine_image(x2, Linear_Expression(0));
    ph2.affine_image(k, Linear_Expression(1));
    ph2.add_constraint(k == 1);
    ph2.time_elapse_assign(ph_tea);
    ph2.add_constraint(k == 1);

    // Location l3.
    ph3 = ph2;
    ph3.add_constraint(x2 <= a);
    ph3.add_constraint(x1 < b);
    ph3.affine_image(k, Linear_Expression(2));
    ph3.add_constraint(k == 2);
    ph3.time_elapse_assign(ph_tea);
    ph3.add_constraint(k == 2);

    // Location l4.
    ph4 = ph2;
    ph4.add_constraint(x1 >= b);
    ph4.add_constraint(k == 1);
    ph4.time_elapse_assign(ph_tea);
    ph4.add_constraint(k == 1);

    // Location l5.
    ph5 = ph4;
    ph5.add_constraint(x2 <= a);
    ph5.affine_image(k, Linear_Expression(2));
    ph5.add_constraint(k == 2);
    ph5.time_elapse_assign(ph_tea);
    ph5.add_constraint(k == 2);

    // When we have a fixpoint, check and print results.
    if (ph0 == ph0_prev) {

      NNC_Polyhedron known_result0(5);
      known_result0.add_constraint(a >= 0);
      known_result0.add_constraint(b >= 0);
      known_result0.add_constraint(k >= 0);
      known_result0.add_constraint(k <= 2);
      bool ok = (ph0 == known_result0);

      print_constraints(ph0, "*** ph0 final ***");

      NNC_Polyhedron known_result1(5);
      known_result1.add_constraint(a >= 0);
      known_result1.add_constraint(b >= 0);
      known_result1.add_constraint(x1 >= 0);
      known_result1.add_constraint(x1 <= a);
      known_result1.add_constraint(k == 0);
      ok = ok && (ph1 == known_result1);

      print_constraints(ph1, "*** ph1 final ***");

      NNC_Polyhedron known_result2(5);
      known_result2.add_constraint(9*x1 - 10*x2 <= 0);
      known_result2.add_constraint(11*x1 - 10*x2 >= 0);
      known_result2.add_constraint(a >= 0);
      known_result2.add_constraint(b >= 0);
      known_result2.add_constraint(k == 1);
      ok = ok && (ph2 == known_result2);

      print_constraints(ph2, "*** ph2 final ***");

      NNC_Polyhedron known_result3(5);
      known_result3.add_constraint(9*x1 - 10*x2 <= 0);
      known_result3.add_constraint(11*x1 - 10*x2 >= 0);
      known_result3.add_constraint(a >= 0);
      known_result3.add_constraint(b > 0);
      known_result3.add_constraint(k == 2);
      ok = ok && (ph3 == known_result3);

      print_constraints(ph3, "*** ph3 final ***");

      NNC_Polyhedron known_result4(5);
      known_result4.add_constraint(9*x1 - 10*x2 <= 0);
      known_result4.add_constraint(11*x1 - 10*x2 >= 0);
      known_result4.add_constraint(a >= 0);
      known_result4.add_constraint(b >= 0);
      known_result4.add_constraint(x1 >= b);
      known_result4.add_constraint(k == 1);
      ok = ok && (ph4 == known_result4);

      print_constraints(ph4, "*** ph4 final ***");

      NNC_Polyhedron known_result5(5);
      known_result5.add_constraint(9*b - 10*a <= 0);
      known_result5.add_constraint(11*x1 - 10*x2 >= 0);
      known_result5.add_constraint(11*x1 - 10*x2 + 10*a - 11*b >= 0);
      known_result5.add_constraint(9*x1 - 10*x2 <= 0);
      known_result5.add_constraint(b >= 0);
      known_result5.add_constraint(x1 >= b);
      known_result5.add_constraint(k == 2);
      ok = ok && (ph5 == known_result5);

      print_constraints(ph5, "*** ph5 final ***");

      return ok;
    }

    ph0.upper_bound_assign(ph0_prev);
    (void) ph0.is_empty();
    // ph0.H79_widening_assign(ph0_prev);

    ph0_prev = ph0;
  }
  return true;
}

// The Scheduler example using the Polyhedron domain.
bool
test03() {
  Variable c1(0);
  Variable c2(1);
  Variable x1(2);
  Variable x2(3);
  Variable k1(4);
  Variable k2(5);

  NNC_Polyhedron ph_idle_Init(6);
  ph_idle_Init.add_constraint(c1 >= 0);
  ph_idle_Init.add_constraint(c2 >= 0);
  ph_idle_Init.add_constraint(x1 == 0);
  ph_idle_Init.add_constraint(x2 == 0);
  ph_idle_Init.add_constraint(k1 == 0);
  ph_idle_Init.add_constraint(k2 == 0);

  NNC_Polyhedron ph_idle(6, EMPTY);
  NNC_Polyhedron ph_t1(6, EMPTY);
  NNC_Polyhedron ph_t2(6, EMPTY);

  NNC_Polyhedron ph_idle_prev(6, EMPTY);
  NNC_Polyhedron ph_t1_prev(6, EMPTY);
  NNC_Polyhedron ph_t2_prev(6, EMPTY);

  // The time elapse polyhedron at location Task1.
  NNC_Polyhedron ph_tea1(6);
  ph_tea1.add_constraint(c1 == 1);
  ph_tea1.add_constraint(c2 == 1);
  ph_tea1.add_constraint(x1 == 1);
  ph_tea1.add_constraint(x2 == 0);
  ph_tea1.add_constraint(k1 == 0);
  ph_tea1.add_constraint(k2 == 0);

  // The time elapse polyhedron at location Task2.
  NNC_Polyhedron ph_tea2(6);
  ph_tea2.add_constraint(c1 == 1);
  ph_tea2.add_constraint(c2 == 1);
  ph_tea2.add_constraint(x1 == 0);
  ph_tea2.add_constraint(x2 == 1);
  ph_tea2.add_constraint(k1 == 0);
  ph_tea2.add_constraint(k2 == 0);

  NNC_Polyhedron ph_tmp(6);

  while (true) {

    // Location Idle.
    ph_idle.upper_bound_assign(ph_idle_Init);
    ph_tmp = ph_t1;
    ph_tmp.add_constraint(x1 == 4);
    ph_tmp.add_constraint(k1 <= 1);
    ph_tmp.affine_image(k1, k1 - 1);
    ph_tmp.affine_image(x1, Linear_Expression(0));
    ph_idle.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(x2 == 8);
    ph_tmp.add_constraint(k2 <= 1);
    ph_tmp.add_constraint(k1 == 0);
    ph_tmp.affine_image(k2, k2 - 1);
    ph_tmp.affine_image(x2, Linear_Expression(0));
    ph_idle.upper_bound_assign(ph_tmp);

    // Location Task1.
    ph_tmp = ph_idle;
    ph_tmp.add_constraint(c1 >= 10);
    ph_tmp.affine_image(c1, Linear_Expression(0));
    ph_tmp.affine_image(k1, Linear_Expression(1));
    ph_tmp.add_constraint(x1 <= 4);
    ph_t1.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t1;
    ph_tmp.add_constraint(c1 >= 10);
    ph_tmp.affine_image(c1, Linear_Expression(0));
    ph_tmp.affine_image(k1, k1 + 1);
    ph_tmp.add_constraint(x1 <= 4);
    ph_t1.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t1;
    ph_tmp.add_constraint(x1 == 4);
    ph_tmp.add_constraint(k1 >= 2);
    ph_tmp.affine_image(x1, Linear_Expression(0));
    ph_tmp.affine_image(k1, k1 - 1);
    ph_tmp.add_constraint(x1 <= 4);
    ph_t1.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(x1 == 8);
    ph_tmp.add_constraint(k2 <= 1);
    ph_tmp.add_constraint(k1 >= 1);
    ph_tmp.affine_image(x2, Linear_Expression(0));
    ph_tmp.affine_image(k2, k2 - 1);
    ph_tmp.add_constraint(x1 <= 4);
    ph_t1.upper_bound_assign(ph_tmp);
    ph_t1.time_elapse_assign(ph_tea1);
    ph_t1.add_constraint(x1 <= 4);

    // Location Task2.
    ph_tmp = ph_idle;
    ph_tmp.add_constraint(c2 >= 20);
    ph_tmp.affine_image(c2, Linear_Expression(0));
    ph_tmp.affine_image(k2, Linear_Expression(1));
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(c2 >= 20);
    ph_tmp.affine_image(c2, Linear_Expression(0));
    ph_tmp.affine_image(k2, k2 + 1);
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(c1 >= 10);
    ph_tmp.affine_image(c1, Linear_Expression(0));
    ph_tmp.affine_image(k1, k1 + 1);
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(x2 == 8);
    ph_tmp.add_constraint(k2 >= 2);
    ph_tmp.affine_image(x2, Linear_Expression(0));
    ph_tmp.affine_image(k2, k2 - 1);
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t1;
    ph_tmp.add_constraint(c2 >= 20);
    ph_tmp.affine_image(c2, Linear_Expression(0));
    ph_tmp.affine_image(k2, Linear_Expression(1));
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_t2.time_elapse_assign(ph_tea2);
    ph_t2.add_constraint(x2 <= 8);

    ph_t2.H79_widening_assign(ph_t2_prev);

    if (ph_idle == ph_idle_prev
	&& ph_t1 == ph_t1_prev
	&& ph_t2 == ph_t2_prev) {

      NNC_Polyhedron known_result_idle(6);
      known_result_idle.add_constraint(c2 >= 0);
      known_result_idle.add_constraint(x1 == 0);
      known_result_idle.add_constraint(x2 == 0);
      known_result_idle.add_constraint(k1 == 0);
      known_result_idle.add_constraint(k2 == 0);
      bool ok = (ph_idle == known_result_idle);

      print_constraints(ph_idle, "*** ph_idle final ***");

      NNC_Polyhedron known_result_t1(6);
      known_result_t1.add_constraint(c1 <= c2);
      known_result_t1.add_constraint(c1 >= 0);
      known_result_t1.add_constraint(c1 <= 4);
      known_result_t1.add_constraint(x1 == c1);
      known_result_t1.add_constraint(x2 == 0);
      known_result_t1.add_constraint(k1 == 1);
      known_result_t1.add_constraint(k2 == 0);
      ok = ok &&  (ph_t1 == known_result_t1);

      print_constraints(ph_t1, "*** ph_t1 final ***");

      NNC_Polyhedron known_result_t2(6);
      known_result_t2.add_constraint(c2 >= 0);
      known_result_t2.add_constraint(x1 >= 0);
      known_result_t2.add_constraint(x1 <= 4*k1);
      known_result_t2.add_constraint(c2 <= 8);
      known_result_t2.add_constraint(x2 == c2);
      known_result_t2.add_constraint(k2 == 1);
      ok = ok &&  (ph_t2 == known_result_t2);

      print_constraints(ph_t2, "*** ph_t2 final ***");

      return ok;
    }

    ph_idle_prev = ph_idle;
    ph_t1_prev = ph_t1;
    ph_t2_prev = ph_t2;
  }
  return false;
}

// Additional procedures needed for the Scheduler example using powersets.

// Compute the time_elapse() for each element of the powerset.
void
time_elapse(Pointset_Powerset<NNC_Polyhedron>& ps,
            const NNC_Polyhedron& ph) {
  Pointset_Powerset<NNC_Polyhedron> ps_result(ps.space_dimension(), EMPTY);
  for (Pointset_Powerset<NNC_Polyhedron>::const_iterator i = ps.begin(),
         ps_end = ps.end(); i != ps_end; ++i) {
    NNC_Polyhedron di = i->pointset();
    NNC_Polyhedron d(di);
    d.time_elapse_assign(ph);
    ps_result.add_disjunct(d);
  }
  ps = ps_result;
}

// Compute the affine_image() for each element of the powerset.
void
affine_image(Pointset_Powerset<NNC_Polyhedron>& ps, Variable var,
             const Linear_Expression& expr,
             Coefficient_traits::const_reference denominator
             = Coefficient_one()) {
  Pointset_Powerset<NNC_Polyhedron> ps_result(ps.space_dimension(), EMPTY);
  for (Pointset_Powerset<NNC_Polyhedron>::iterator i = ps.begin(),
         ps_end = ps.end(); i != ps_end; ++i) {
    NNC_Polyhedron phi = i->pointset();
    NNC_Polyhedron ph(phi);
    ph.affine_image(var, expr, denominator);
    ps_result.add_disjunct(ph);
  }
  ps = ps_result;
}

// Compute the upper_bound of the powerset.
void
powerset_upper_bound(Pointset_Powerset<NNC_Polyhedron>& ps,
                     NNC_Polyhedron& ph) {
  Pointset_Powerset<NNC_Polyhedron> ps_result(ps.space_dimension(), EMPTY);
  for (Pointset_Powerset<NNC_Polyhedron>::iterator i = ps.begin(),
                                   ps_end = ps.end(); i != ps_end; ++i) {
    NNC_Polyhedron phi = i->pointset();
    ph.upper_bound_assign(phi);
    (void) ph.is_empty();
  }
}

// This has been added to replace the expensive geometrically_covers()
// test at each cycle of the iteration and intended to speed up the
// computation.
//
// Check if every disjunct of the 2nd powerset is contained in an
// disjunct of the first powerset.
// If this succeeds then geometrically covers() will also hold.
bool
disjunct_covers(Pointset_Powerset<NNC_Polyhedron>& ps1,
                Pointset_Powerset<NNC_Polyhedron>& ps2) {
  for (Pointset_Powerset<NNC_Polyhedron>::iterator i = ps2.begin(),
         ps2_end = ps2.end(); i != ps2_end; ++i) {
    bool is_contained = false;
    NNC_Polyhedron ph2 = i->pointset();
    for (Pointset_Powerset<NNC_Polyhedron>::iterator j = ps1.begin(),
           ps1_end = ps1.end(); j != ps1_end; ++j) {
      NNC_Polyhedron ph1 = j->pointset();
      if (ph1.contains(ph2)) {
        is_contained = true;
        break;
      }
    }
    if (!is_contained)
      return false;
  }
  return true;
}

// The Scheduler example using powersets.
bool
test04() {
  Variable x1(0);
  Variable x2(1);
  Variable k1(2);
  Variable k2(3);
  Variable c1(4);
  Variable c2(5);

  Pointset_Powerset<NNC_Polyhedron> ph_idle(6);
  ph_idle.add_constraint(c1 >= 0);
  ph_idle.add_constraint(c2 >= 0);
  ph_idle.add_constraint(x1 == 0);
  ph_idle.add_constraint(x2 == 0);
  ph_idle.add_constraint(k1 == 0);
  ph_idle.add_constraint(k2 == 0);
  Pointset_Powerset<NNC_Polyhedron>
    ph_t1 = Pointset_Powerset<NNC_Polyhedron>(6, EMPTY);
  Pointset_Powerset<NNC_Polyhedron>
    ph_t2 = Pointset_Powerset<NNC_Polyhedron>(6, EMPTY);

  Pointset_Powerset<NNC_Polyhedron>
    ph_idle_prev = Pointset_Powerset<NNC_Polyhedron>(6, EMPTY);
  Pointset_Powerset<NNC_Polyhedron>
    ph_t1_prev = Pointset_Powerset<NNC_Polyhedron>(6, EMPTY);
  Pointset_Powerset<NNC_Polyhedron>
    ph_t2_prev = Pointset_Powerset<NNC_Polyhedron>(6, EMPTY);

  NNC_Polyhedron ph_tea1(6);
  ph_tea1.add_constraint(c1 == 1);
  ph_tea1.add_constraint(c2 == 1);
  ph_tea1.add_constraint(x1 == 1);
  ph_tea1.add_constraint(x2 == 0);
  ph_tea1.add_constraint(k1 == 0);
  ph_tea1.add_constraint(k2 == 0);
  NNC_Polyhedron ph_tea2(6);
  ph_tea2.add_constraint(c1 == 1);
  ph_tea2.add_constraint(c2 == 1);
  ph_tea2.add_constraint(x1 == 0);
  ph_tea2.add_constraint(x2 == 1);
  ph_tea2.add_constraint(k1 == 0);
  ph_tea2.add_constraint(k2 == 0);

  Pointset_Powerset<NNC_Polyhedron> ph_tmp(6);

  while (true) {
    //    nout << "next iteration" << std::endl;

    // ph_idle
    ph_tmp = ph_t1;
    ph_tmp.add_constraint(x1 == 4);
    ph_tmp.add_constraint(k1 <= 1);
    affine_image(ph_tmp, k1, k1 - 1);
    affine_image(ph_tmp, x1, Linear_Expression(0));
    ph_idle.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(x2 == 8);
    ph_tmp.add_constraint(k2 <= 1);
    ph_tmp.add_constraint(k1 == 0);
    affine_image(ph_tmp, k2, k2 - 1);
    affine_image(ph_tmp, x2, Linear_Expression(0));
    ph_idle.upper_bound_assign(ph_tmp);

    // ph_t1
    ph_tmp = ph_idle;
    ph_tmp.add_constraint(c1 >= 10);
    affine_image(ph_tmp, c1, Linear_Expression(0));
    affine_image(ph_tmp, k1, Linear_Expression(1));
    ph_tmp.add_constraint(x1 <= 4);
    ph_t1.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t1;
    ph_tmp.add_constraint(c1 >= 10);
    affine_image(ph_tmp, c1, Linear_Expression(0));
    affine_image(ph_tmp, k1, k1 + 1);
    ph_tmp.add_constraint(x1 <= 4);
    ph_t1.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t1;
    ph_tmp.add_constraint(x1 == 4);
    ph_tmp.add_constraint(k1 >= 2);
    affine_image(ph_tmp, x1, Linear_Expression(0));
    affine_image(ph_tmp, k1, k1 - 1);
    ph_tmp.add_constraint(x1 <= 4);
    ph_t1.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(x1 == 8);
    ph_tmp.add_constraint(k2 <= 1);
    ph_tmp.add_constraint(k1 >= 1);
    affine_image(ph_tmp, x2, Linear_Expression(0));
    affine_image(ph_tmp, k2, k2 - 1);
    ph_tmp.add_constraint(x1 <= 4);
    ph_t1.upper_bound_assign(ph_tmp);
    time_elapse(ph_t1, ph_tea1);
    ph_t1.add_constraint(x1 <= 4);
    (void) ph_t1.is_empty();

    // ph_t2
    ph_tmp = ph_idle;
    ph_tmp.add_constraint(c2 >= 20);
    affine_image(ph_tmp, c2, Linear_Expression(0));
    affine_image(ph_tmp, k2, Linear_Expression(1));
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(c2 >= 20);
    affine_image(ph_tmp, c2, Linear_Expression(0));
    affine_image(ph_tmp, k2, k2 + 1);
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(c1 >= 10);
    affine_image(ph_tmp, c1, Linear_Expression(0));
    affine_image(ph_tmp, k1, k1 + 1);
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t2;
    ph_tmp.add_constraint(x2 == 8);
    ph_tmp.add_constraint(k2 >= 2);
    affine_image(ph_tmp, x2, Linear_Expression(0));
    affine_image(ph_tmp, k2, k2 - 1);
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    ph_tmp = ph_t1;
    ph_tmp.add_constraint(c2 >= 20);
    affine_image(ph_tmp, c2, Linear_Expression(0));
    affine_image(ph_tmp, k2, Linear_Expression(1));
    ph_tmp.add_constraint(x2 <= 8);
    ph_t2.upper_bound_assign(ph_tmp);
    time_elapse(ph_t2, ph_tea2);
    ph_t2.add_constraint(x2 <= 8);

    // Note that widening is not needed and the iteration terminates
    // with the ph_t2 consisting of 4 polyhedra.
    // The results without any widening are:

    // ph_idle = { F = 0, E = 0, D = 0, C = 0, B >= 0, A >= 0 }

    // ph_t1 =
    // { F = 0, E = 1, D = 0, A - C = 0, -A + B >= 0, A >= 0, -A >= -4 }

    // ph_t2 =
    // { F = 1, E = 0, C = 0, B - D = 0, -B >= -8, B >= 0, A - B >= 0 },
    // { F = 1, E = 1, B - D = 0, A - B - C = 0, -A + B >= -4, -B >= -8,
    //                                             B >= 0, A - B >= 0 },
    // { F = 1, E = 1, C = 0, B - D = 0, -A + B >= 0, -B >= -8, A >= 0 },
    // { F = 1, E = 2, B - D = 0, -A + B + C >= 10, -C >= -4, -B >= -8,
    //                                                          A >= 0 }


    // However, for ph_t2, a number of different
    // widening and extrapolation options have been tested.
    //
    //
    //    ph_t2.BGP99_extrapolation_assign(
    //      ph_t2_prev, widen_fun_ref(&Polyhedron::H79_widening_assign), 3);
    // The BGP99 extrapolation with the max number 3 keeps the information
    // that k1 <= 2 as well as that k2 = 1:
    // { F = 1, E = 0, C = 0, B - D = 0, -B >= -8, B >= 0, A - B >= 0 },
    // { F = 1, E = 1, B - D = 0, A - B - C = 0, -A + B >= -4, -B >= -8,
    //                                             B >= 0, A - B >= 0 },
    // { F = 1, B - D = 0, -A + B + C - 10*E >= -10, -C + 4*E >= 4,
    //                      -E >= -2, C - 2*E >= -2, -B >= -8, A >= 0 }

    //    ph_t2.BGP99_extrapolation_assign(
    //      ph_t2_prev, widen_fun_ref(&Polyhedron::H79_widening_assign), 2);
    // If we reduce the number 3 to 2, then we lose the constraint k1 <= 2:
    // { F = 1, E = 0, C = 0, B - D = 0, -B >= -8, B >= 0, A - B >= 0 },
    // { F = 1, B - D = 0, -A + B + C - 10*E >= -10, -C >= -4, E >= 1,
    //  C - 2*E >= -2, A - B - 3*C + 12*E >= 4, -B >= -8, A - C + 4*E >= 4 }

    //   ph_t2.BHZ03_widening_assign<BHRZ03_Certificate>
    //     (ph_t2_prev, widen_fun_ref(&Polyhedron::H79_widening_assign));
    // The BHZ03 widening loses the information that k1 <=2
    // because there is no "with tokens" option:
    // { F = 1, B - D = 0, -C + 4*E >= 0, C >= 0, B >= 0, -B >= -8 }

    // The simpler test disjunct_covers() which is sufficient here
    // should be more efficient than geometrically_covers().
    if (disjunct_covers(ph_idle_prev, ph_idle)
 	 && disjunct_covers(ph_t1_prev, ph_t1)
  	 && disjunct_covers(ph_t2_prev, ph_t2)) {

      //  if (ph_idle_prev.geometrically_covers(ph_idle)
      //       && ph_t1_prev.geometrically_covers(ph_t1)
      //       && ph_t2_prev.geometrically_covers(ph_t2)) {


      print_constraints(ph_idle, "*** ph_idle final ***");

      print_constraints(ph_t1, "*** ph_t1 final ***");

      print_constraints(ph_t2, "*** ph_t2 final ***");

      // We project away the variables c1 and C2 only compare the
      // upper_bounds of the powerset with the expected result.

      Variables_Set vs;
      vs.insert(c1);
      vs.insert(c2);

      NNC_Polyhedron ph_idle_hull(6, EMPTY);
      powerset_upper_bound(ph_idle, ph_idle_hull);

      ph_idle_hull.remove_space_dimensions(vs);

      NNC_Polyhedron known_result_idle(4);
      known_result_idle.add_constraint(x1 == 0);
      known_result_idle.add_constraint(x2 == 0);
      known_result_idle.add_constraint(k1 == 0);
      known_result_idle.add_constraint(k2 == 0);
      bool ok = (ph_idle_hull == known_result_idle);

      print_constraints(ph_idle_hull, "*** ph_idle_hull final projected ***");

      NNC_Polyhedron ph_t1_hull(6, EMPTY);
      powerset_upper_bound(ph_t1, ph_t1_hull);
      ph_t1_hull.remove_space_dimensions(vs);

      NNC_Polyhedron known_result_t1(4);
      known_result_t1.add_constraint(x1 >= 0);
      known_result_t1.add_constraint(x1 <= 4);
      known_result_t1.add_constraint(x2 == 0);
      known_result_t1.add_constraint(k1 == 1);
      known_result_t1.add_constraint(k2 == 0);
      ok = ok &&  (ph_t1_hull == known_result_t1);

      print_constraints(ph_t1_hull, "*** ph_t1_hull final projected ***");

      NNC_Polyhedron ph_t2_hull(6, EMPTY);
      powerset_upper_bound(ph_t2, ph_t2_hull);
      ph_t2_hull.remove_space_dimensions(vs);

      NNC_Polyhedron known_result_t2(4);
      known_result_t2.add_constraint(k2 == 1);
      known_result_t2.add_constraint(k1 <= 2);
      known_result_t2.add_constraint(x1 >= 0);
      known_result_t2.add_constraint(x1 <= 4);
      known_result_t2.add_constraint(x1 >= 0);
      known_result_t2.add_constraint(x2 <= 8);
      known_result_t2.add_constraint(x2 >= 0);
      known_result_t2.add_constraint(x1 <= 4*k1);
      known_result_t2.add_constraint(2*k1 - x1 <= 2);
      known_result_t2.add_constraint(x1 + x2 >= 10*k1 -10);
      known_result_t2.add_constraint(x2 >= 6*k1 - 6);
      ok = ok &&  (ph_t2_hull == known_result_t2);

      print_constraints(ph_t2_hull, "*** ph_t2_hull final projected ***");

      return ok;
    }

    ph_idle_prev = ph_idle;
    ph_t1_prev = ph_t1;
    ph_t2_prev = ph_t2;
  }

  return false;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8(test02);
  DO_TEST(test03);
  DO_TEST(test04);
END_MAIN
