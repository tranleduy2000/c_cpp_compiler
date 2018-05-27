/* PIP_Problem Java class declaration and implementation.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
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


package parma_polyhedra_library;

//! A Parametric Integer Programming problem.
/*! \ingroup PPL_Java_interface
  An object of this class encodes a parametric integer (linear)
  programming problem. The PIP problem is specified by providing:
   - the dimension of the vector space;
   - the subset of those dimensions of the vector space that are
     interpreted as integer parameters (the other space dimensions
     are interpreted as non-parameter integer variables);
   - a finite set of linear equality and (strict or non-strict)
     inequality constraints involving variables and/or parameters;
     these constraints are used to define:
       - the <EM>feasible region</EM>, if they involve one or more
         problem variable (and maybe some parameters);
       - the <EM>initial context</EM>, if they only involve the
         parameters;
   - optionally, the so-called <EM>big parameter</EM>,
     i.e., a problem parameter to be considered arbitrarily big.

  Note that all problem variables and problem parameters are assumed
  to take non-negative integer values, so that there is no need
  to specify non-negativity constraints.

  The class provides support for the (incremental) solution of the
  PIP problem based on variations of the revised simplex method and
  on Gomory cut generation techniques.

  The solution for a PIP problem is the lexicographic minimum of the
  integer points of the feasible region, expressed in terms of the
  parameters. As the problem to be solved only involves non-negative
  variables and parameters, the problem will always be either unfeasible
  or optimizable.

  As the feasibility and the solution value of a PIP problem depend on the
  values of the parameters, the solution is a binary decision tree,
  dividing the context parameter set into subsets.
  The tree nodes are of two kinds:
   - \e Decision nodes.
     These are internal tree nodes encoding one or more linear tests
     on the parameters; if all the tests are satisfied, then the solution
     is the node's \e true child; otherwise, the solution is the node's
     \e false child;
   - \e Solution nodes.
     These are leaf nodes in the tree, encoding the solution of the problem
     in the current context subset, where each variable is defined in terms
     of a linear expression of the parameters.
     Solution nodes also optionally embed a set of parameter constraints:
     if all these constraints are satisfied, the solution is described by
     the node, otherwise the problem has no solution.

  It may happen that a decision node has no \e false child. This means
  that there is no solution if at least one of the corresponding
  constraints is not satisfied. Decision nodes having two or more linear
  tests on the parameters cannot have a \e false child. Decision nodes
  always have a \e true child.

  Both kinds of tree nodes may also contain the definition of extra
  parameters which are artificially introduced by the solver to enforce
  an integral solution. Such artificial parameters are defined by
  the integer division of a linear expression on the parameters
  by an integer coefficient.

  By exploiting the incremental nature of the solver, it is possible
  to reuse part of the computational work already done when solving
  variants of a given PIP_Problem: currently, incremental resolution
  supports the addition of space dimensions, the addition of parameters
  and the addition of constraints.
*/
public class PIP_Problem extends PPL_Object {

  //! Builds a trivial PIP problem.
  /*!
    A trivial PIP problem requires to compute the lexicographic minimum
    on a vector space under no constraints and with no parameters:
    due to the implicit non-negativity constraints, the origin of the
    vector space is an optimal solution.

    \param dim
    The dimension of the vector space enclosing \p this
    (optional argument with default value \f$0\f$).

    \exception Length_Error_Exception
    Thrown if \p dim exceeds <CODE>max_space_dimension()</CODE>.
  */
    public PIP_Problem(long dim) {
        build_cpp_object(dim);
    }

  //! Builds a PIP problem from a sequence of constraints.
  /*!
    Builds a PIP problem having space dimension \p dim from the
    constraint system cs; the dimensions \p vars are interpreted as
    parameters.
  */
   public PIP_Problem(long dim, Constraint_System cs, Variables_Set params) {
        build_cpp_object(dim, cs, params);
    }

    //! Builds a copy of \p y.
    public PIP_Problem(PIP_Problem y) {
        build_cpp_object(y);
    }

    /*! \brief
      Releases all resources managed by \p this,
      also resetting it to a null reference.
    */
    public native void free();

    //! Releases all resources managed by \p this.
    protected native void finalize();

    /*@}*/ /* Constructors and Destructor */

    //! \name Functions that Do Not Modify the PIP_Problem
    /*@{*/

    //! Returns the maximum space dimension an PIP_Problem can handle.
    public native long max_space_dimension();

    //! Returns the space dimension of the PIP problem.
    public native long space_dimension();

    //! Returns the number of parameter space dimensions of the PIP problem.
    public native long number_of_parameter_space_dimensions();

    /*! \brief
      Returns all the parameter space dimensions of problem \p pip.
    */
    public native Variables_Set parameter_space_dimensions();

    //! Returns the big parameter dimension of PIP problem \p pip.
    public native long get_big_parameter_dimension();

    /*! \brief
      Returns the number of constraints defining the feasible
      region of \p pip.
    */
    public native long number_of_constraints();

    /*! \brief
      Returns the \p i-th constraint defining the feasible region
      of the PIP problem \p pip.
    */
    public native Constraint constraint_at_index(long dim);

    //! Returns the constraints .
    public native Constraint_System constraints();

    //! Returns an ascii formatted internal representation of \p this.
    public native String ascii_dump();

    //! Returns a string representation of \p this.
    public native String toString();

    /*! \brief
      Returns the size in bytes of the memory occupied by the
      underlying C++ object.
    */
    public native long total_memory_in_bytes();

    /*! \brief
      Returns the size in bytes of the memory managed by the
      underlying C++ object.
    */
    public native long external_memory_in_bytes();

    /*! \brief
      Returns true if the pip problem is well formed, i.e., if it
      satisfies all its implementation invariants; returns 0 and perhaps
      makes some noise if broken.  Useful for debugging purposes.
    */
    public native boolean OK();

    /*@}*/ /* Functions that Do Not Modify the MIP_Problem */


    //! \name Functions that May Modify the PIP_Problem
    /*@{*/

    //! Resets \p this to be equal to the trivial PIP problem.
    /*!
      The space dimension is reset to \f$0\f$.
    */
    public native void clear();


    /*! \brief
      Adds <CODE>pip_vars + pip_params</CODE> new space dimensions
      and embeds the PIP problem in the new vector space.

      \param pip_vars
      The number of space dimensions to add that are interpreted as
      PIP problem variables (i.e., non parameters). These are added
      before adding the \p pip_params parameters.

      \param pip_params
      The number of space dimensions to add that are interpreted as
      PIP problem parameters. These are added after having added the
      \p pip_vars problem variables.

      The new space dimensions will be those having the highest indexes
      in the new PIP problem; they are initially unconstrained.
    */
    public native void add_space_dimensions_and_embed(long pip_vars,
                                                      long pip_params);

    /*! \brief
      Sets the space dimensions in \p vars to be parameter dimensions of
      the PIP problem.
    */
    public native void add_to_parameter_space_dimensions(Variables_Set vars);

    /*! \brief
      Sets the big parameter dimension of PIP problem to \p d.
    */
    public native void set_big_parameter_dimension(long d);

    /*! \brief
      Adds a copy of constraint \p c to the PIP problem.

      \exception Invalid_Argument_Exception
      Thrown if the constraint \p c is a strict inequality or if its space
      dimension is strictly greater than the space dimension of \p this.
    */
    public native void add_constraint(Constraint c);

    /*! \brief
      Adds a copy of the constraints in \p cs to the PIP problem.

      \exception Invalid_Argument_Exception
      Thrown if the constraint system \p cs contains any strict inequality
      or if its space dimension is strictly greater than the space dimension
      of \p this.
    */
    public native void add_constraints(Constraint_System cs);

    /*@}*/ /* Functions that May Modify the PIP_Problem */

    //! \name Computing the Solution of the PIP_Problem
    /*@{*/

    //! Checks satisfiability of \p this.
    /*!
      \return
      <CODE>true</CODE> if and only if the PIP problem is satisfiable.
    */
    public native boolean is_satisfiable();

    //! Optimizes the PIP problem.
    /*!
      Solves the PIP problem, returning an exit status.

      \return
      <CODE>UNFEASIBLE_PIP_PROBLEM</CODE> if the PIP problem
      is not satisfiable;
      <CODE>OPTIMIZED_PIP_PROBLEM</CODE> if the PIP problem
      admits an optimal solution.
    */
    public native PIP_Problem_Status solve();

    //! Returns a solution for the PIP problem, if it exists.
    public native PIP_Tree_Node solution();

    //! Returns an optimizing solution for the PIP problem, if it exists.
    public native PIP_Tree_Node optimizing_solution();

    /*@}*/ /* Computing the Solution of the PIP_Problem */

    //! \name Querying/Setting Control Parameters
    /*@{*/

    /*! \brief
      Returns the value of control parameter \p name.
    */
    public native PIP_Problem_Control_Parameter_Value
      get_pip_problem_control_parameter
        (PIP_Problem_Control_Parameter_Name name);

    /*! \brief
      Sets control parameter \p value.
    */
    public native void set_pip_problem_control_parameter
      (PIP_Problem_Control_Parameter_Value value);

    /*@}*/ /* Querying/Setting Control Parameters */

    //! Builds the underlying C++ object.
    private native void build_cpp_object(long dim);

    //! Builds the underlying C++ object.
    private native void build_cpp_object(long dim,
                                         Constraint_System cs,
                                         Variables_Set vars);

    //! Builds the underlying C++ object.
    private native void build_cpp_object(PIP_Problem y);
}
