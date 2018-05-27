/* Test PIP_Problem Java test class of the Parma Polyhedra Library Java
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


public class PIP_Problem_test1 {
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

    // This code tests the PIP_Problem methods.
    public static boolean test01() {
	Variable A = new Variable(0);
	Variable B = new Variable(1);
	Variable C = new Variable(2);
	Variable D = new Variable(3);
        Variables_Set var_set_D = new Variables_Set();
        var_set_D.add(D);
	Linear_Expression_Variable le_b = new Linear_Expression_Variable(B);
	Linear_Expression_Variable le_c = new Linear_Expression_Variable(C);
	Linear_Expression_Variable le_a = new Linear_Expression_Variable(A);
	Linear_Expression_Variable le_d = new Linear_Expression_Variable(D);
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
	Constraint c_d_leq_1
          = new Constraint(le_d, Relation_Symbol.LESS_OR_EQUAL, le_1);
	Constraint constraint1 = c_a_geq_1;
	Constraint constraint2 = c_b_geq_3;
	Constraint_System constraints1 = new Constraint_System();
        constraints1.add(constraint1);
        C_Polyhedron ph1 = new C_Polyhedron(3, Degenerate_Element.UNIVERSE);
        ph1.add_constraints(constraints1);
        C_Polyhedron ph2 = new C_Polyhedron(4, Degenerate_Element.UNIVERSE);
        ph2.add_constraints(constraints1);
        ph2.add_constraint(constraint2);

	PIP_Problem pip1 = new PIP_Problem(3);
        pip1.add_constraints(constraints1);
        Constraint pip1_c1 = pip1.constraint_at_index(0);
        long pip1_dim = pip1.space_dimension();

        PIP_Problem pip2 = new PIP_Problem(pip1_dim);
        pip2.add_constraints(constraints1);

        boolean ok = (pip2.space_dimension() == 3);
        if (!ok)
          return false;

        PIP_Problem pip3 = new PIP_Problem(3);
        pip3.add_constraints(constraints1);
        pip3.add_space_dimensions_and_embed(0, 1);
        pip3.add_constraint(constraint2);
        ok = ok && (pip3.space_dimension() == 4);

	Constraint constraint4 = c_d_leq_1;
	Constraint_System constraints4 = new Constraint_System();
        constraints4.add(constraint1);
        constraints4.add(constraint4);
        PIP_Problem pip4 = new PIP_Problem(4, constraints4, var_set_D);
        ok = ok
             && (pip4.space_dimension() == 4)
             && (pip4.number_of_parameter_space_dimensions() == 1);

	return ok;
    }

    // This code tests more PIP_Problem methods.
    public static boolean test02() {
	Variable A = new Variable(0);
	Variable B = new Variable(1);
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

        Variables_Set var_set_B = new Variables_Set();
        var_set_B.add(B);

	PIP_Problem pip1 = new PIP_Problem(2);
        pip1.add_constraints(constraints1);
         long pip1_dim = pip1.space_dimension();
        pip1.add_to_parameter_space_dimensions(var_set_B);
        Variables_Set var_set1 = pip1.parameter_space_dimensions();
        boolean ok = (var_set1.contains(B));
        if (!ok)
          return false;
        ok = pip1.is_satisfiable();
        if (!ok)
          return false;

        PIP_Problem_Status pip1_status;
        pip1_status = pip1.solve();
	ok = ok && (pip1_status == PIP_Problem_Status.OPTIMIZED_PIP_PROBLEM);

        PIP_Problem_Status pip2_status;
        pip1.add_constraint(c_a_leq_5);
        pip2_status = pip1.solve();
	ok = ok && (pip2_status == PIP_Problem_Status.OPTIMIZED_PIP_PROBLEM);
        if (!ok)
          return false;

        PPL_Test.println_if_noisy("Testing toString() and wrap_string(): ");
        PPL_Test.println_if_noisy(IO.wrap_string(pip1.toString(), 4, 64, 60));
        PPL_Test.println_if_noisy();

        PPL_Test.print_if_noisy("Testing max_space_dimension(): ");
        long max_space_dim = pip1.max_space_dimension();
        PPL_Test.println_if_noisy(max_space_dim);

        PIP_Problem_Control_Parameter_Value cp_value1
          = pip1.get_pip_problem_control_parameter
              (PIP_Problem_Control_Parameter_Name.CUTTING_STRATEGY);
        pip1.set_pip_problem_control_parameter(
          PIP_Problem_Control_Parameter_Value.CUTTING_STRATEGY_FIRST);
        PIP_Problem_Control_Parameter_Value cp_value2
          = pip1.get_pip_problem_control_parameter
              (PIP_Problem_Control_Parameter_Name.CUTTING_STRATEGY);
        ok = ok
          && (cp_value2
                == PIP_Problem_Control_Parameter_Value.CUTTING_STRATEGY_FIRST);
        if (!ok)
          return false;

        pip1.set_pip_problem_control_parameter(
          PIP_Problem_Control_Parameter_Value.PIVOT_ROW_STRATEGY_MAX_COLUMN);
        PIP_Problem_Control_Parameter_Value cp_value3
          = pip1.get_pip_problem_control_parameter
              (PIP_Problem_Control_Parameter_Name.PIVOT_ROW_STRATEGY);
        ok = ok
          && (cp_value3
                == PIP_Problem_Control_Parameter_Value
                    .PIVOT_ROW_STRATEGY_MAX_COLUMN);

        pip1.set_pip_problem_control_parameter(
          PIP_Problem_Control_Parameter_Value.CUTTING_STRATEGY_ALL);
        PIP_Problem_Control_Parameter_Value cp_value4
          = pip1.get_pip_problem_control_parameter
              (PIP_Problem_Control_Parameter_Name.CUTTING_STRATEGY);
        ok = ok
          && (cp_value4
                == PIP_Problem_Control_Parameter_Value.CUTTING_STRATEGY_ALL);

	return ok && pip1.OK();
    }


    // This code tests PIP_Tree and its methods.
    public static boolean test03() {
	Variable A = new Variable(0);
	Variable B = new Variable(1);
	Variable C = new Variable(2);
	Variable D = new Variable(3);
        Variables_Set var_set_D = new Variables_Set();
        var_set_D.add(D);
	Linear_Expression_Variable le_b = new Linear_Expression_Variable(B);
	Linear_Expression_Variable le_c = new Linear_Expression_Variable(C);
	Linear_Expression_Variable le_a = new Linear_Expression_Variable(A);
	Linear_Expression_Variable le_d = new Linear_Expression_Variable(D);
        Coefficient coeff_0 = new Coefficient(0);
        Coefficient coeff_1 = new Coefficient(1);
        Coefficient coeff_3 = new Coefficient(3);
        Coefficient coeff_5 = new Coefficient(5);
        Coefficient coeff_8 = new Coefficient(8);
	Linear_Expression le_1 = new Linear_Expression_Coefficient(coeff_1);
	Linear_Expression le_3 = new Linear_Expression_Coefficient(coeff_3);
	Linear_Expression le_5 = new Linear_Expression_Coefficient(coeff_5);
	Linear_Expression le_8 = new Linear_Expression_Coefficient(coeff_8);

	// Constraint declarations.
	Constraint c_a_geq_1
          = new Constraint(le_a, Relation_Symbol.GREATER_OR_EQUAL, le_1);
	Constraint c_a_leq_5
          = new Constraint(le_a, Relation_Symbol.LESS_OR_EQUAL, le_5);
	Constraint c_a_eq_8
          = new Constraint(le_a, Relation_Symbol.EQUAL, le_8);
	Constraint c_d_leq_1
          = new Constraint(le_d, Relation_Symbol.LESS_OR_EQUAL, le_1);
	Constraint constraint1 = c_a_geq_1;
	Constraint_System constraints1 = new Constraint_System();
        constraints1.add(constraint1);

        Variables_Set var_set_B = new Variables_Set();
        var_set_B.add(B);

	PIP_Problem pip1 = new PIP_Problem(2);
        pip1.add_constraints(constraints1);
        long pip1_dim = pip1.space_dimension();
        pip1.add_to_parameter_space_dimensions(var_set_B);
        Variables_Set var_set1 = pip1.parameter_space_dimensions();
        boolean ok = pip1.is_satisfiable();
        if (!ok)
          return false;
        PIP_Tree_Node ptree1 = pip1.solution();
        long num_of_arts1 = ptree1.number_of_artificials();
        Artificial_Parameter_Sequence arts1 = ptree1.artificials();
        ok = ptree1.OK() && num_of_arts1 == 0 && arts1.size() == 0;
        if (!ok)
          return false;
        PIP_Solution_Node psol1 = ptree1.as_solution();
        PIP_Decision_Node pdec1 = ptree1.as_decision();
        if (psol1 == null || pdec1 != null)
            return false;
        Constraint_System ptree1_constraints = ptree1.constraints();
        C_Polyhedron pip1_ph = new C_Polyhedron(pip1_dim,
                                                Degenerate_Element.UNIVERSE);
        C_Polyhedron ph1 = new C_Polyhedron(pip1_dim,
                                            Degenerate_Element.UNIVERSE);
        ph1.add_constraint(constraint1);
        pip1_ph.add_constraints(ptree1_constraints);
        ok = ok && !pip1_ph.equals(ph1);
        if (!ok)
          return false;

        PPL_Test.println_if_noisy("Testing toString() and wrap_string(): ");
        PPL_Test.println_if_noisy(IO.wrap_string(ptree1.toString(), 4, 64, 60));
        PPL_Test.println_if_noisy();

	Constraint constraint2 = c_d_leq_1;
	Constraint_System constraints2 = new Constraint_System();
        constraints2.add(constraint1);
        constraints2.add(constraint2);
        PIP_Problem pip2 = new PIP_Problem(4, constraints2, var_set_D);
        PIP_Tree_Node ptree2 = pip2.solution();
        long num_of_arts2 = ptree2.number_of_artificials();
        ok = (ptree2.OK() && (num_of_arts2 == 0));
        if (!ok)
          return false;
        PIP_Solution_Node psol2 = ptree2.as_solution();
        PIP_Decision_Node pdec2 = ptree2.as_decision();
        Artificial_Parameter_Sequence arts2 = ptree2.artificials();
        if (psol2 == null || pdec2 != null)
            return false;
        ok = arts2.isEmpty();
        if (!ok)
          return false;

        PPL_Test.println_if_noisy("Testing toString() and wrap_string(): ");
        PPL_Test.println_if_noisy(IO.wrap_string(ptree2.toString(), 4, 64, 60));
        PPL_Test.println_if_noisy();

        return true;
   }

   // This code is based on test02 in pipproblem1.cc.
    public static boolean test04() {
	Variable I = new Variable(0);
	Variable J = new Variable(1);
	Variable N = new Variable(2);
	Variable M = new Variable(3);
        Variables_Set params = new Variables_Set();
        params.add(N);
        params.add(M);
	Linear_Expression_Variable le_i = new Linear_Expression_Variable(I);
	Linear_Expression_Variable le_j = new Linear_Expression_Variable(J);
	Linear_Expression_Variable le_n = new Linear_Expression_Variable(N);
	Linear_Expression_Variable le_m = new Linear_Expression_Variable(M);
        Coefficient coeff_0 = new Coefficient(0);
        Coefficient coeff_1 = new Coefficient(1);
        Coefficient coeff_2 = new Coefficient(2);
        Coefficient coeff_3 = new Coefficient(3);
        Coefficient coeff_4 = new Coefficient(4);
        Coefficient coeff_7 = new Coefficient(7);
        Coefficient coeff_8 = new Coefficient(8);
        Coefficient coeff_10 = new Coefficient(10);
	Linear_Expression le_0 = new Linear_Expression_Coefficient(coeff_0);
	Linear_Expression le_1 = new Linear_Expression_Coefficient(coeff_1);
	Linear_Expression le_2 = new Linear_Expression_Coefficient(coeff_2);
	Linear_Expression le_3 = new Linear_Expression_Coefficient(coeff_3);
	Linear_Expression le_4 = new Linear_Expression_Coefficient(coeff_4);
	Linear_Expression le_7 = new Linear_Expression_Coefficient(coeff_7);
	Linear_Expression le_8 = new Linear_Expression_Coefficient(coeff_8);
	Linear_Expression le_10 = new Linear_Expression_Coefficient(coeff_10);
	Linear_Expression le_2i = le_i.times(coeff_2);
	Linear_Expression le_3j = le_j.times(coeff_3);
	Linear_Expression le_2i_plus_3j = le_2i.sum(le_3j);
	Linear_Expression le_4i = le_i.times(coeff_4);
	Linear_Expression le_4i_diff_j = le_4i.subtract(le_j);
	Linear_Expression le_m_diff_j = le_m.subtract(le_j);
	Linear_Expression le_n_diff_i = le_n.subtract(le_i);

	// Constraint declarations.
	Constraint c_1
          = new Constraint(le_2i_plus_3j, Relation_Symbol.GREATER_OR_EQUAL,
                           le_8);
	Constraint c_2
          = new Constraint(le_4i_diff_j, Relation_Symbol.GREATER_OR_EQUAL,
                           le_4);
	Constraint c_3
          = new Constraint(le_m_diff_j, Relation_Symbol.GREATER_OR_EQUAL,
                           le_0);
	Constraint c_4
          = new Constraint(le_n_diff_i, Relation_Symbol.GREATER_OR_EQUAL,
                           le_0);
	Constraint_System cs = new Constraint_System();
        cs.add(c_1);
        cs.add(c_2);
        cs.add(c_3);
        cs.add(c_4);

	PIP_Problem pip = new PIP_Problem(4, cs, params);

        // Known to overflow if PPL uses 8-bits checked integers.
        try {
            pip.solve();
        }
        catch (Overflow_Error_Exception ex) {
            if (Coefficient.bits() != 8)
                throw ex;
            PPL_Test.println_if_noisy("Expected overflow exception caught:");
            PPL_Test.println_if_noisy(ex.getMessage());
            return true;
        }

        PIP_Problem_Status pip_status = pip.solve();
        boolean ok = (pip_status == PIP_Problem_Status.OPTIMIZED_PIP_PROBLEM);
        if (!ok)
          return false;
        PIP_Tree_Node solution = pip.solution();
        ok = solution.OK();
        if (!ok)
          return false;

        PPL_Test.println_if_noisy();
        PPL_Test.println_if_noisy("Testing toString():");
        PPL_Test.println_if_noisy(solution.toString());
        PPL_Test.println_if_noisy();

        PIP_Decision_Node root = solution.as_decision();
        if (root == null || !root.OK())
          return false;
        if (root.number_of_artificials() != 0)
          return false;
        Constraint_System cs1 = root.constraints();
        if (cs1.size() != 1)
          return false;
        Constraint c = cs.firstElement();
        Linear_Expression le_left = c.left_hand_side();
        Linear_Expression le_right = c.right_hand_side();
        Relation_Symbol relsym = c.kind();
        if (relsym != Relation_Symbol.GREATER_OR_EQUAL
            || (le_left.is_zero()) || (le_right.is_zero()))
          return false;
        PIP_Tree_Node root_true_branch = root.child_node(true);
        if (root_true_branch == null)
          return false;

        PPL_Test.println_if_noisy("PIP Tree root node Constraint: ");
        PPL_Test.println_if_noisy(IO.wrap_string(c.toString(), 4, 64, 60));
        PPL_Test.println_if_noisy();

        PIP_Decision_Node t_child = root_true_branch.as_decision();
        if (t_child == null || !t_child.OK())
          return false;

        // Check t_child node context.
        if (t_child.number_of_artificials() != 0)
          return false;
        Constraint_System cs2 = t_child.constraints();
        if (cs2.size() != 1)
          return false;
        Constraint c2 = cs2.firstElement();
        Linear_Expression le_left2 = c2.left_hand_side();
        Linear_Expression le_right2 = c2.right_hand_side();
        Relation_Symbol relsym2 = c2.kind();
        if (relsym2 != Relation_Symbol.GREATER_OR_EQUAL
          || (le_left2.is_zero()) || (le_right2.is_zero()))
          return false;

        if (t_child.child_node(true) == null)
          return false;
        PIP_Solution_Node t_t_child = t_child.child_node(true).as_solution();
        if (t_t_child == null || !t_t_child.OK())
          return false;

        // Check t_t_child node.
        if (t_t_child.number_of_artificials() != 0)
          return false;
        Constraint_System cs3 = t_t_child.constraints();
        if (cs3.size() != 0)
          return false;
        Linear_Expression v_i = t_t_child.parametric_values(I);
        Linear_Expression v_j = t_t_child.parametric_values(J);

        PIP_Solution_Node t_f_child = t_child.child_node(false).as_solution();
        if (t_f_child == null || !t_f_child.OK())
          return false;

        // Check t_f_child node.
        // Check artificial parameter.
        if (t_f_child.number_of_artificials() != 1)
          return false;
        Artificial_Parameter ap = t_f_child.artificials().firstElement();
        Linear_Expression ap_le = ap.linear_expression();
        Coefficient ap_den = ap.denominator();

        PPL_Test.println_if_noisy("Artificial_Parameter linear_expression: ");
        PPL_Test.println_if_noisy(IO.wrap_string(ap_le.toString(), 4, 64, 60));
        PPL_Test.println_if_noisy();
        PPL_Test.println_if_noisy("Artificial_Parameter denominator: ");
        PPL_Test.println_if_noisy(IO.wrap_string(ap_den.toString(), 4, 64, 60));
        PPL_Test.println_if_noisy();
         // Check context.
        Constraint_System cs4 = t_f_child.constraints();
        if (cs4.size() != 1)
          return false;
        Constraint c4 = cs4.firstElement();
        if (c4.kind() != Relation_Symbol.GREATER_OR_EQUAL)
          return false;
        // Check parametric values.
        Linear_Expression v_i4 = t_f_child.parametric_values(I);
        Linear_Expression v_j4 = t_f_child.parametric_values(J);

      return true;
    }

    public static void main(String[] args) {
        Parma_Polyhedra_Library.initialize_library();
	boolean test_result_ok =
	    Test_Executor.executeTests(PIP_Problem_test1.class);
        Parma_Polyhedra_Library.finalize_library();
	if (!test_result_ok)
	    System.exit(1);
	System.exit(0);
    }
}
