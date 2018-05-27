/* MIP_Problem Java class declaration and implementation.
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

//! A Mixed Integer (linear) Programming problem.
/*! \ingroup PPL_Java_interface
  An object of this class encodes a mixed integer (linear) programming problem.
  The MIP problem is specified by providing:
   - the dimension of the vector space;
   - the feasible region, by means of a finite set of linear equality
     and non-strict inequality constraints;
   - the subset of the unknown variables that range over the integers
     (the other variables implicitly ranging over the reals);
   - the objective function, described by a Linear_Expression;
   - the optimization mode (either maximization or minimization).

  The class provides support for the (incremental) solution of the
  MIP problem based on variations of the revised simplex method and
  on branch-and-bound techniques. The result of the resolution
  process is expressed in terms of an enumeration, encoding the
  feasibility and the unboundedness of the optimization problem.
  The class supports simple feasibility tests (i.e., no optimization),
  as well as the extraction of an optimal (resp., feasible) point,
  provided the MIP_Problem is optimizable (resp., feasible).

  By exploiting the incremental nature of the solver, it is possible
  to reuse part of the computational work already done when solving
  variants of a given MIP_Problem: currently, incremental resolution
  supports the addition of space dimensions, the addition of constraints,
  the change of objective function and the change of optimization mode.
*/
public class MIP_Problem extends PPL_Object {

    //! \name Constructors and Destructor
    /*@{*/

    //! Builds a trivial MIP problem.
    /*!
      A trivial MIP problem requires to maximize the objective function
      \f$0\f$ on a vector space under no constraints at all:
      the origin of the vector space is an optimal solution.

      \param dim
      The dimension of the vector space enclosing \p this.

      \exception Length_Error_Exception
      Thrown if \p dim exceeds <CODE>max_space_dimension()</CODE>.
    */
    public MIP_Problem(long dim) {
        build_cpp_object(dim);
    }

    /*! \brief
      Builds an MIP problem having space dimension \p dim from the constraint
      system \p cs, the objective function \p obj and optimization mode
      \p mode.

      \param dim
      The dimension of the vector space enclosing \p this.

      \param cs
      The constraint system defining the feasible region.

      \param obj
      The objective function.

      \param mode
      The optimization mode.

      \exception Length_Error_Exception
      Thrown if \p dim exceeds <CODE>max_space_dimension()</CODE>.

      \exception Invalid_Argument_Exception
      Thrown if the constraint system contains any strict inequality
      or if the space dimension of the constraint system (resp., the
      objective function) is strictly greater than \p dim.
    */
    public MIP_Problem(long dim, Constraint_System cs, Linear_Expression obj,
                       Optimization_Mode mode) {
        build_cpp_object(dim, cs, obj, mode);
    }

    //! Builds a copy of \p y.
    public MIP_Problem(MIP_Problem y) {
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

    //! \name Functions that Do Not Modify the MIP_Problem
    /*@{*/

    //! Returns the maximum space dimension an MIP_Problem can handle.
    public native long max_space_dimension();

    //! Returns the space dimension of the MIP problem.
    public native long space_dimension();

    /*! \brief
      Returns a set containing all the variables' indexes constrained
      to be integral.
    */
    public native Variables_Set integer_space_dimensions();

    //! Returns the constraints .
    public native Constraint_System constraints();

    //! Returns the objective function.
    public native Linear_Expression objective_function();

    //! Returns the optimization mode.
    public native Optimization_Mode optimization_mode();

    //! Returns an ascii formatted internal representation of \p this.
    public native String ascii_dump();

    //! Returns a string representation of \p this.
    public native String toString();

    /*! \brief
      Returns the total size in bytes of the memory occupied by the
      underlying C++ object.
    */
    public native long total_memory_in_bytes();

    //! Checks if all the invariants are satisfied.
    public native boolean OK();

    /*@}*/ /* Functions that Do Not Modify the MIP_Problem */

    //! \name Functions that May Modify the MIP_Problem
    /*@{*/

    //! Resets \p this to be equal to the trivial MIP problem.
    /*!
      The space dimension is reset to \f$0\f$.
    */
    public native void clear();

    /*! \brief
      Adds \p m new space dimensions and embeds the old MIP problem
      in the new vector space.

      \param m
      The number of dimensions to add.

      \exception Length_Error_Exception
      Thrown if adding \p m new space dimensions would cause the
      vector space to exceed dimension <CODE>max_space_dimension()</CODE>.

      The new space dimensions will be those having the highest indexes
      in the new MIP problem; they are initially unconstrained.
    */
    public native void add_space_dimensions_and_embed(long m);

    /*! \brief
      Sets the variables whose indexes are in set \p i_vars to be
      integer space dimensions.

      \exception Invalid_Argument_Exception
      Thrown if some index in \p i_vars does not correspond to
      a space dimension in \p this.
    */
    public native void add_to_integer_space_dimensions(Variables_Set i_vars);

    /*! \brief
      Adds a copy of constraint \p c to the MIP problem.

      \exception Invalid_Argument_Exception
      Thrown if the constraint \p c is a strict inequality or if its space
      dimension is strictly greater than the space dimension of \p this.
    */
    public native void add_constraint(Constraint c);

    /*! \brief
      Adds a copy of the constraints in \p cs to the MIP problem.

      \exception Invalid_Argument_Exception
      Thrown if the constraint system \p cs contains any strict inequality
      or if its space dimension is strictly greater than the space dimension
      of \p this.
    */
    public native void add_constraints(Constraint_System cs);

    //! Sets the objective function to \p obj.
    /*!
      \exception Invalid_Argument_Exception
      Thrown if the space dimension of \p obj is strictly greater than
      the space dimension of \p this.
    */
    public native void set_objective_function(Linear_Expression obj);

    //! Sets the optimization mode to \p mode.
    public native void set_optimization_mode(Optimization_Mode mode);

    /*@}*/ /* Functions that May Modify the MIP_Problem */

    //! \name Computing the Solution of the MIP_Problem
    /*@{*/

    //! Checks satisfiability of \p this.
    /*!
      \return
      <CODE>true</CODE> if and only if the MIP problem is satisfiable.
    */
    public native boolean is_satisfiable();

    //! Optimizes the MIP problem.
    /*!
      \return
      An MIP_Problem_Status flag indicating the outcome of the optimization
      attempt (unfeasible, unbounded or optimized problem).
    */
    public native MIP_Problem_Status solve();

    /*! \brief
      Sets \p num and \p den so that \f$\frac{num}{den}\f$ is the result
      of evaluating the objective function on \p evaluating_point.

      \param evaluating_point
      The point on which the objective function will be evaluated.

      \param num
      On exit will contain the numerator of the evaluated value.

      \param den
      On exit will contain the denominator of the evaluated value.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p evaluating_point are dimension-incompatible
      or if the generator \p evaluating_point is not a point.
    */
    public native void evaluate_objective_function(Generator evaluating_point,
                                                   Coefficient num,
                                                   Coefficient den);

    //! Returns a feasible point for \p this, if it exists.
    /*!
      \exception Domain_Error_Exception
      Thrown if the MIP problem is not satisfiable.
    */
    public native Generator feasible_point();

    //! Returns an optimal point for \p this, if it exists.
    /*!
      \exception Domain_Error_Exception
      Thrown if \p this doesn't not have an optimizing point, i.e.,
      if the MIP problem is unbounded or not satisfiable.
    */
    public native Generator optimizing_point();

    /*! \brief
      Sets \p num and \p den so that \f$\frac{num}{den}\f$ is
      the solution of the optimization problem.

      \exception Domain_Error_Exception
      Thrown if \p this doesn't not have an optimizing point, i.e.,
      if the MIP problem is unbounded or not satisfiable.
    */
    public native void optimal_value(Coefficient num, Coefficient den);

    /*@}*/ /* Computing the Solution of the MIP_Problem */

    //! \name Querying/Setting Control Parameters
    /*@{*/

    /*! \brief
      Returns the value of control parameter \p name.
    */
    public native Control_Parameter_Value
      get_control_parameter(Control_Parameter_Name name);

    /*! \brief
      Sets control parameter \p value.
    */
    public native void set_control_parameter(Control_Parameter_Value value);

    /*@}*/ /* Querying/Setting Control Parameters */

    //! Builds the underlying C++ object.
    private native void build_cpp_object(long dim);

    //! Builds the underlying C++ object.
    private native void build_cpp_object(long dim,
                                         Constraint_System cs,
                                         Linear_Expression obj,
                                         Optimization_Mode mode);

    //! Builds the underlying C++ object.
    private native void build_cpp_object(MIP_Problem y);
}
