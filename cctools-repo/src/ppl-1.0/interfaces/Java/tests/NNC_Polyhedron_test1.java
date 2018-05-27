/* Test NNC_Polyhedron Java test class of the Parma Polyhedra Library Java
   interface.
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

import java.math.BigInteger;
import java.util.Iterator;
import java.util.Vector;
import parma_polyhedra_library.*;


public class NNC_Polyhedron_test1 {
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

    public static boolean test01() {
	Variable X = new Variable(0);
	Variable Y = new Variable(1);
	Variable Z = new Variable(2);
	NNC_Polyhedron ph = new NNC_Polyhedron(3, Degenerate_Element.UNIVERSE);
	Linear_Expression le_X = new Linear_Expression_Variable(X);
	Linear_Expression le_Y = new Linear_Expression_Variable(Y);
	Linear_Expression le_Z = new Linear_Expression_Variable(Z);
	Linear_Expression le_2Y = le_Y.times(new Coefficient(2));
	Linear_Expression le_5Z = le_Z.times(new Coefficient(5));
	Linear_Expression le_7
	    = new Linear_Expression_Coefficient(new Coefficient(7));
	Linear_Expression le_5
	    = new Linear_Expression_Coefficient(new Coefficient(5));
	Linear_Expression lhs1 = le_X.sum(le_2Y.sum(le_5Z));
	NNC_Polyhedron ph1
            = new NNC_Polyhedron(3, Degenerate_Element.UNIVERSE);
	ph1.add_constraint(new Constraint(lhs1,
                                          Relation_Symbol.GREATER_OR_EQUAL,
                                          le_7));
	ph1.add_constraint(new Constraint(le_X,
                                          Relation_Symbol.LESS_THAN,
					  le_5Z));
	PPL_Test.println_if_noisy(ph1.constraints().toString());
        Constraint c = new Constraint(le_5Z,
                                      Relation_Symbol.GREATER_THAN,
                                      le_X);
        Poly_Con_Relation rel = ph1.relation_with(c);
	return rel.implies(Poly_Con_Relation.is_included());
    }

    public static boolean test02() {
	// Test if `minimized_constraints' returns an empty Constraint_System
	// if the Polyhedron is built from universe with a dimension greater
	// than zero.
	Variable X = new Variable(0);
	Variable Y = new Variable(1);
	Variable Z = new Variable(2);
	NNC_Polyhedron ph = new NNC_Polyhedron(3, Degenerate_Element.UNIVERSE);
	Constraint_System cs = ph.minimized_constraints();
	return cs.isEmpty();
    }

    public static boolean test03() {
	// Test termination methods.
	Variable X1 = new Variable(0);
	Variable X2 = new Variable(1);
	Variable XP1 = new Variable(2);
	Variable XP2 = new Variable(3);
	NNC_Polyhedron ph = new NNC_Polyhedron(4, Degenerate_Element.UNIVERSE);
        Coefficient coeff_1 = new Coefficient(1);
        Coefficient coeff_0 = new Coefficient(0);
	Linear_Expression le_X1 = new Linear_Expression_Variable(X1);
	Linear_Expression le_X2 = new Linear_Expression_Variable(X2);
	Linear_Expression le_XP1 = new Linear_Expression_Variable(XP1);
	Linear_Expression le_XP2 = new Linear_Expression_Variable(XP2);
	Linear_Expression le_1 = new Linear_Expression_Coefficient(coeff_1);
	Linear_Expression le_0 = new Linear_Expression_Coefficient(coeff_0);
	Linear_Expression le_X1_difference_XP1 = le_X1.sum(le_XP1);
	Constraint c_XP2_eq_1
          = new Constraint(le_XP2, Relation_Symbol.EQUAL, le_1);
	Constraint c_X1_geq_1
          = new Constraint(le_X1, Relation_Symbol.GREATER_OR_EQUAL, le_1);
	Constraint c_X1_minus_XP1_geq_1
          = new Constraint(le_X1_difference_XP1, Relation_Symbol.GREATER_OR_EQUAL, le_1);
	Constraint c_X2_leq_0
          = new Constraint(le_X2, Relation_Symbol.LESS_OR_EQUAL, le_0);
	ph.add_constraint(c_XP2_eq_1);
	ph.add_constraint(c_X1_geq_1);
	ph.add_constraint(c_X1_minus_XP1_geq_1);
	ph.add_constraint(c_X2_leq_0);
	return Termination.termination_test_MS_NNC_Polyhedron(ph);
    }

    public static void main(String[] args) {
        Parma_Polyhedra_Library.initialize_library();
	boolean test_result_ok =
	    Test_Executor.executeTests(NNC_Polyhedron_test1.class);
        Parma_Polyhedra_Library.finalize_library();
	if (!test_result_ok)
	    System.exit(1);
	System.exit(0);
    }

}
