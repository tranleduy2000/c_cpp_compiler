/* Test C_Polyhedron Java test class of the Parma Polyhedra Library Java
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

public class C_Polyhedron_test1 {
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

    // This code tests the method `map_space_dimension(pfunc)'.
    public static boolean test01() {
	Partial_Function partial_function = new Partial_Function();
	partial_function.insert(0, 2);
	partial_function.insert(2, 0);
	partial_function.insert(1, 1);
	Variable A = new Variable(0);
	Variable B = new Variable(1);
	Variable C = new Variable(2);
	Linear_Expression_Variable le_b = new Linear_Expression_Variable(B);
	Linear_Expression_Variable le_c = new Linear_Expression_Variable(C);
	Linear_Expression_Variable le_a = new Linear_Expression_Variable(A);
	Linear_Expression_Sum le_a_plus_b
            = new Linear_Expression_Sum(le_a, le_b);
	Linear_Expression_Sum le_a_plus_c
            = new Linear_Expression_Sum(le_a, le_c);
	Linear_Expression_Sum le_c_plus_b
            = new Linear_Expression_Sum(le_c, le_b);
	Linear_Expression_Sum le_c_plus_a
            = new Linear_Expression_Sum(le_c, le_a);
	Linear_Expression_Coefficient le_two
	    = new Linear_Expression_Coefficient(new Coefficient(2));
	Linear_Expression_Times le_2c
	    = new Linear_Expression_Times(le_c, new Coefficient(2));
	Linear_Expression_Times le_2a
	    = new Linear_Expression_Times(le_a, new Coefficient(2));

	Generator_System gs = new Generator_System();
	gs.add(Generator.point(le_2c, new Coefficient(1)));
	gs.add(Generator.line(le_a_plus_b));
	gs.add(Generator.ray(le_a_plus_c));

	C_Polyhedron poly1 = new C_Polyhedron(gs);
	poly1.map_space_dimensions(partial_function);

	Generator_System known_gs = new Generator_System();
	known_gs.add(Generator.point(le_2a, new Coefficient(1)));
	known_gs.add(Generator.line(le_c_plus_b));
	known_gs.add(Generator.ray(le_c_plus_a));

	C_Polyhedron known_result = new C_Polyhedron(known_gs);
	return known_result.equals(poly1);
    }


    public static boolean test02() {
	// Test if `minimized_constraints' returns an empty Constraint_System
	// if the Polyhedron is built from universe with a dimension greater
	// than zero.
	Variable X = new Variable(0);
	Variable Y = new Variable(1);
	Variable Z = new Variable(2);
	C_Polyhedron ph = new C_Polyhedron(3, Degenerate_Element.UNIVERSE);
	Constraint_System cs = ph.minimized_constraints();
	return cs.isEmpty();
    }

    public static void main(String[] args) {
        Parma_Polyhedra_Library.initialize_library();
	boolean test_result_ok =
	    Test_Executor.executeTests(C_Polyhedron_test1.class);
        Parma_Polyhedra_Library.finalize_library();
	if (!test_result_ok)
	    System.exit(1);
	System.exit(0);
    }
}
