/* Parma_Polyhedra_Library Java test class of the
   Parma Polyhedra Library Java interface.
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

import parma_polyhedra_library.*;

public class Parma_Polyhedra_Library_test2 {
static {
    try {
        System.loadLibrary("ppl_java");
    }
    catch (UnsatisfiedLinkError  e) {
        System.out.println("Unable to load the library");
        System.out.println(e.getMessage());
        System.exit(-1);
    }
}

    // This code tests the timeout functions.
    public static boolean test01() {
        int csecs = 50;
        int max_dimension = 20;
        return timed_compute_open_hypercube_generators(csecs, max_dimension);
    }

    // This code tests the deterministic timeout functions.
    public static boolean test02() {
        int weight = 2000;
        int max_dimension = 20;
        return weighted_compute_open_hypercube_generators(weight,
                                                          max_dimension);
    }

    public static void main(String[] args) {
        Parma_Polyhedra_Library.initialize_library();
	boolean test_result_ok =
	    Test_Executor.executeTests(Parma_Polyhedra_Library_test2.class);
        Parma_Polyhedra_Library.finalize_library();
	if (!test_result_ok)
	    System.exit(1);
	System.exit(0);
    }

    private static void open_hypercube(int dimension, Polyhedron ph) {
        Linear_Expression le_zero
            = new Linear_Expression_Coefficient(new Coefficient("0"));
        Linear_Expression le_one
            = new Linear_Expression_Coefficient(new Coefficient("1"));

        Constraint_System cs = new Constraint_System();
        Linear_Expression le;
        Constraint c;
        for (int i = 0; i < dimension; ++i) {
            // Variable(i) > 0.
            le = new Linear_Expression_Variable(new Variable(i));
            c = new Constraint(le, Relation_Symbol.GREATER_THAN, le_zero);
            cs.add(c);
            // Variable(i) < 1.
            c = new Constraint(le, Relation_Symbol.LESS_THAN, le_one);
            cs.add(c);
        }
        ph.add_constraints(cs);
    }

    private static boolean
        timed_compute_open_hypercube_generators(int csecs,
                                                int max_dimension) {
        for (int i = 0; i <= max_dimension; ++i) {
            Polyhedron ph = new NNC_Polyhedron(i, Degenerate_Element.UNIVERSE);
            open_hypercube(i, ph);
            PPL_Test.println_if_noisy("Hypercube of dimension " + i);
            try {
                Parma_Polyhedra_Library.set_timeout(csecs);
                try {
                    ph.generators();
                } finally {
                    Parma_Polyhedra_Library.reset_timeout();
                }
            } catch (Timeout_Exception e) {
                PPL_Test.println_if_noisy("Expected timeout exception caught:");
                PPL_Test.println_if_noisy(e.getMessage());
                return true;
            } catch (Exception e) {
                PPL_Test.println_if_noisy("Unexpected exception caught:");
                PPL_Test.println_if_noisy(e.getMessage());
                return false;
            }
        }
        // Should not reach this point.
        PPL_Test.println_if_noisy("Expected timeout exception NOT caught!");
        return false;
    }

    private static boolean
        weighted_compute_open_hypercube_generators(int weight,
                                                   int max_dimension) {
        for (int i = 0; i <= max_dimension; ++i) {
            Polyhedron ph = new NNC_Polyhedron(i, Degenerate_Element.UNIVERSE);
            open_hypercube(i, ph);
            PPL_Test.println_if_noisy("Hypercube of dimension " + i);
            try {
                Parma_Polyhedra_Library.set_deterministic_timeout(weight, 0);
                try {
                    ph.generators();
                } finally {
                    Parma_Polyhedra_Library.reset_deterministic_timeout();
                }
            } catch (Timeout_Exception e) {
                PPL_Test.println_if_noisy("Expected timeout exception caught:");
                PPL_Test.println_if_noisy(e.getMessage());
                return true;
            } catch (Exception e) {
                PPL_Test.println_if_noisy("Unexpected exception caught:");
                PPL_Test.println_if_noisy(e.getMessage());
                return false;
            }
        }
        // Should not reach this point.
        PPL_Test.println_if_noisy("Expected timeout exception NOT caught!");
        return false;
    }
}
