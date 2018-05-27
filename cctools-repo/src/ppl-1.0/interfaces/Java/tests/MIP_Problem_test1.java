/* Test MIP_Problem Java test class of the Parma Polyhedra Library Java
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


public class MIP_Problem_test1 {
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

    // This code tests the MIP_Problem methods.
    public static boolean test01() {
	Variable A = new Variable(0);
	Variable B = new Variable(1);
	Variable C = new Variable(2);
	Linear_Expression_Variable le_b = new Linear_Expression_Variable(B);
	Linear_Expression_Variable le_c = new Linear_Expression_Variable(C);
	Linear_Expression_Variable le_a = new Linear_Expression_Variable(A);
        Coefficient coeff_1 = new Coefficient(1);
        Coefficient coeff_3 = new Coefficient(3);
        Coefficient coeff_5 = new Coefficient(5);
	Linear_Expression le_1 = new Linear_Expression_Coefficient(coeff_1);
	Linear_Expression le_3 = new Linear_Expression_Coefficient(coeff_3);
	Linear_Expression le_5 = new Linear_Expression_Coefficient(coeff_5);

	// Constraint declarations.
	Constraint c_a_geq_1
          = new Constraint(le_a, Relation_Symbol.GREATER_OR_EQUAL, le_1);
	Constraint c_a_leq_5
          = new Constraint(le_a, Relation_Symbol.LESS_OR_EQUAL, le_5);
	Constraint c_b_geq_3
          = new Constraint(le_b, Relation_Symbol.GREATER_OR_EQUAL, le_3);
	Constraint constraint1 = c_a_geq_1;
	Constraint constraint2 = c_b_geq_3;
	Constraint_System constraints1 = new Constraint_System();
        constraints1.add(constraint1);
        C_Polyhedron ph1 = new C_Polyhedron(3, Degenerate_Element.UNIVERSE);
        ph1.add_constraints(constraints1);
        C_Polyhedron ph2 = new C_Polyhedron(4, Degenerate_Element.UNIVERSE);
        ph2.add_constraints(constraints1);
        ph2.add_constraint(constraint2);

	MIP_Problem mip1
          = new MIP_Problem(3, constraints1, le_a,
                           Optimization_Mode.MAXIMIZATION);
        Constraint_System mip1_constraints = mip1.constraints();
        long mip1_dim = mip1.space_dimension();
        Linear_Expression mip1_obj = mip1.objective_function();
        Optimization_Mode mip1_opt = mip1.optimization_mode();

        MIP_Problem mip2 = new MIP_Problem(mip1_dim);
        mip2.add_constraints(mip1_constraints);
        mip2.set_objective_function(mip1_obj);
        mip2.set_optimization_mode(mip1_opt);

        boolean ok = (mip2.space_dimension() == 3)
           && (mip2.optimization_mode() == Optimization_Mode.MAXIMIZATION);
        C_Polyhedron mip2_ph = new C_Polyhedron(3,
                                                Degenerate_Element.UNIVERSE);
        mip2_ph.add_constraints(mip1_constraints);
        ok = ok && new Boolean(mip2_ph.equals(ph1));
        if (!ok)
          return false;

        MIP_Problem mip3 = new MIP_Problem(3);
        mip3.add_constraints(constraints1);
        mip3.add_space_dimensions_and_embed(1);
        mip3.set_objective_function(le_b);
        mip3.add_constraint(constraint2);
        mip3.set_optimization_mode(Optimization_Mode.MINIMIZATION);
        ok = ok && (mip3.space_dimension() == 4)
           && (mip3.optimization_mode() == Optimization_Mode.MINIMIZATION);
        Constraint_System mip3_constraints = mip3.constraints();
        C_Polyhedron mip3_ph = new C_Polyhedron(4,
                                                Degenerate_Element.UNIVERSE);
        mip3_ph.add_constraints(mip3_constraints);
        ok = ok && mip3_ph.equals(ph2);

	return ok;
    }

    // This code tests more MIP_Problem methods.
    public static boolean test02() {
	Variable A = new Variable(0);
	Linear_Expression_Variable le_a = new Linear_Expression_Variable(A);
        Coefficient coeff_0 = new Coefficient(0);
        Coefficient coeff_1 = new Coefficient(1);
        Coefficient coeff_5 = new Coefficient(5);
        Coefficient coeff_8 = new Coefficient(8);
	Linear_Expression le_1 = new Linear_Expression_Coefficient(coeff_1);
	Linear_Expression le_5 = new Linear_Expression_Coefficient(coeff_5);
	Linear_Expression le_8 = new Linear_Expression_Coefficient(coeff_8);

	// Constraint declarations.
	Constraint c_a_geq_1
          = new Constraint(le_a, Relation_Symbol.GREATER_OR_EQUAL, le_1);
	Constraint c_a_leq_5
          = new Constraint(le_a, Relation_Symbol.LESS_OR_EQUAL, le_5);
	Constraint c_a_eq_8
          = new Constraint(le_a, Relation_Symbol.EQUAL, le_8);
	Constraint constraint1 = c_a_geq_1;
	Constraint_System constraints1 = new Constraint_System();
        constraints1.add(constraint1);

        Variables_Set var_set_A = new Variables_Set();
        var_set_A.add(A);

	MIP_Problem mip1
          = new MIP_Problem(1, constraints1, le_a,
                           Optimization_Mode.MAXIMIZATION);
        Constraint_System mip1_constraints = mip1.constraints();
        long mip1_dim = mip1.space_dimension();
        Linear_Expression mip1_obj = mip1.objective_function();
        Optimization_Mode mip1_opt = mip1.optimization_mode();

        Variables_Set var_set = mip1.integer_space_dimensions();
        boolean ok = var_set.isEmpty();
        mip1.add_to_integer_space_dimensions(var_set_A);
        Variables_Set var_set1 = mip1.integer_space_dimensions();
        ok = ok && (var_set1.contains(A));
        if (!ok)
          return false;

        ok = mip1.is_satisfiable();
        if (!ok)
          return false;

        MIP_Problem_Status mip1_status;
        mip1_status = mip1.solve();
	ok = ok && (mip1_status == MIP_Problem_Status.UNBOUNDED_MIP_PROBLEM);

        MIP_Problem_Status mip2_status;
        mip1.add_constraint(c_a_leq_5);
        mip2_status = mip1.solve();
	ok = ok && (mip2_status == MIP_Problem_Status.OPTIMIZED_MIP_PROBLEM);
        if (!ok)
          return false;

	MIP_Problem mip3
          = new MIP_Problem(1, constraints1, le_a,
                           Optimization_Mode.MAXIMIZATION);
        MIP_Problem_Status mip3_status;
        mip3.add_constraint(c_a_leq_5);
        mip3.add_constraint(c_a_eq_8);

        Constraint_System cs = mip3.constraints();
        mip3_status = mip3.solve();
        ok = !mip3.is_satisfiable();
	ok = ok && (mip3_status == MIP_Problem_Status.UNFEASIBLE_MIP_PROBLEM);
        if (!ok)
          return false;

	Generator g1 = Generator.point(le_a, coeff_1);
        Coefficient num = coeff_1;
        Coefficient den = coeff_1;
        mip1.evaluate_objective_function(g1, num, den);
        ok = (num == coeff_1 && den == coeff_1);
        if (!ok)
          return false;

	Linear_Expression le_5a = le_a.times(coeff_5);

        Generator f_point = mip1.feasible_point();
        C_Polyhedron f_ph = new C_Polyhedron(1, Degenerate_Element.EMPTY);
        f_ph.add_generator(f_point);
	Generator expected_f_point = Generator.point(le_5a, coeff_1);
        C_Polyhedron expected_f_ph
          = new C_Polyhedron(1, Degenerate_Element.EMPTY);
        expected_f_ph.add_generator(expected_f_point);
        ok = f_ph.equals(expected_f_ph);

        Generator o_point = mip1.optimizing_point();
        C_Polyhedron o_ph = new C_Polyhedron(1, Degenerate_Element.EMPTY);
        o_ph.add_generator(o_point);
	Generator expected_o_point = Generator.point(le_5a, coeff_1);
        C_Polyhedron expected_o_ph
          = new C_Polyhedron(1, Degenerate_Element.EMPTY);
        expected_o_ph.add_generator(expected_o_point);
        ok = o_ph.equals(expected_o_ph);

        Coefficient ov_num = new Coefficient(0);
        Coefficient ov_den = new Coefficient(0);
        mip1.optimal_value(ov_num, ov_den);
        Linear_Expression le_ov_num
          = new Linear_Expression_Coefficient(ov_num);
        Linear_Expression le_ov_den
          = new Linear_Expression_Coefficient(ov_den);
        // ok = (le_ov_num == le_5 && le_ov_den == le_1);
        C_Polyhedron ov_ph
          = new C_Polyhedron(1, Degenerate_Element.EMPTY);
	Constraint c_a_leq_ov_num
          = new Constraint(le_a, Relation_Symbol.LESS_OR_EQUAL, le_ov_num);
	Constraint c_a_geq_ov_num
          = new Constraint(le_a, Relation_Symbol.GREATER_OR_EQUAL, le_ov_den);
        ov_ph.add_constraint(c_a_leq_ov_num);
        C_Polyhedron expected_ov_ph
          = new C_Polyhedron(1, Degenerate_Element.EMPTY);
        expected_ov_ph.add_constraint(c_a_leq_5);
        expected_ov_ph.add_constraint(c_a_geq_1);
        ok = (ov_ph.equals(expected_ov_ph));

        PPL_Test.println_if_noisy("Testing toString() and wrap_string(): ");
        PPL_Test.println_if_noisy(IO.wrap_string(mip1.toString(), 4, 64, 60));
        PPL_Test.println_if_noisy();

        PPL_Test.print_if_noisy("Testing max_space_dimension(): ");
        long max_space_dim = mip1.max_space_dimension();
        PPL_Test.println_if_noisy(max_space_dim);

        Control_Parameter_Value cp_value
          = mip1.get_control_parameter(Control_Parameter_Name.PRICING);
        mip1.set_control_parameter(
          Control_Parameter_Value.PRICING_STEEPEST_EDGE_FLOAT);
        Control_Parameter_Value cp_value1
          = mip1.get_control_parameter(Control_Parameter_Name.PRICING);
        ok = ok
          && (cp_value1
                == Control_Parameter_Value.PRICING_STEEPEST_EDGE_FLOAT);
        mip1.set_control_parameter(
          Control_Parameter_Value.PRICING_STEEPEST_EDGE_EXACT);
        Control_Parameter_Value cp_value2
          = mip1.get_control_parameter(Control_Parameter_Name.PRICING);
        ok = ok
          && (cp_value2
                == Control_Parameter_Value.PRICING_STEEPEST_EDGE_EXACT);
        mip1.set_control_parameter(
          Control_Parameter_Value.PRICING_TEXTBOOK);
        Control_Parameter_Value cp_value3
          = mip1.get_control_parameter(Control_Parameter_Name.PRICING);
        ok = ok
          && (cp_value3
                == Control_Parameter_Value.PRICING_TEXTBOOK);

	return ok && mip1.OK();
    }

    public static void main(String[] args) {
        Parma_Polyhedra_Library.initialize_library();
	boolean test_result_ok =
	    Test_Executor.executeTests(MIP_Problem_test1.class);
        Parma_Polyhedra_Library.finalize_library();
	if (!test_result_ok)
	    System.exit(1);
	System.exit(0);
    }
}
