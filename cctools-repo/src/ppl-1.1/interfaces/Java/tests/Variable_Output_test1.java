/* Test customization of variable output in Java.
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

public class Variable_Output_test1 {
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
        Variable A = new Variable(0);
        Variable B = new Variable(1);
        Variable C = new Variable(2);
        Linear_Expression_Variable le_a = new Linear_Expression_Variable(A);
        Coefficient coeff_1 = new Coefficient(1);
        Linear_Expression le_1 = new Linear_Expression_Coefficient(coeff_1);
        Constraint con
          = new Constraint(le_a, Relation_Symbol.GREATER_OR_EQUAL, le_1);
        C_Polyhedron ph = new C_Polyhedron(1, Degenerate_Element.UNIVERSE);
        ph.add_constraint(con);
        boolean ok = false;

        // Printing with default output function.
        ok = (A.toString().equals("A") &&
              B.toString().equals("B") &&
              C.toString().equals("C") &&
              con.toString().equals("A >= 1") &&
              ph.toString().equals("A >= 1"));
        if (!ok)
          return false;

        // Changing output function to custom one.
        Variable_Stringifier vs = new Custom_Variable_Stringifier();
        Variable.setStringifier(vs);

        ok = (A.toString().equals("Var_0") &&
              B.toString().equals("Var_1") &&
              C.toString().equals("Var_2") &&
              con.toString().equals("Var_0 >= 1") &&
              ph.toString().equals("Var_0 >= 1"));
        if (!ok)
            return false;

        // Restoring default output function.
        Variable.setStringifier(null);

        ok = (A.toString().equals("A") &&
              B.toString().equals("B") &&
              C.toString().equals("C") &&
              con.toString().equals("A >= 1") &&
              ph.toString().equals("A >= 1"));
        if (!ok)
          return false;

        return ok;
    }

    public static void main(String[] args) {
        Parma_Polyhedra_Library.initialize_library();
        boolean test_result_ok =
            Test_Executor.executeTests(Variable_Output_test1.class);
        Parma_Polyhedra_Library.finalize_library();
        if (!test_result_ok)
            System.exit(1);
        System.exit(0);
    }
}
