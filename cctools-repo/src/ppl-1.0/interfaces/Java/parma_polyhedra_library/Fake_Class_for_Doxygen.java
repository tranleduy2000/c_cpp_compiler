/* Doxumentation for the Java interface.
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

package parma_polyhedra_library;

// A fake class, just to cheat Doxygen.
public class Fake_Class_For_Doxygen {}

//! The Java base class for (C and NNC) convex polyhedra.
/*! \ingroup PPL_Java_interface
  The base class Polyhedron provides declarations for most of
  the methods common to classes C_Polyhedron and NNC_Polyhedron.
  Note that the user should always use the derived classes.
  Moreover, C and NNC polyhedra can not be freely interchanged:
  as specified in the main manual, most library functions require
  their arguments to be topologically compatible.
*/
public class Polyhedron extends PPL_Object {

    //! \name Member Functions that Do Not Modify the Polyhedron
    //@{

    //! Returns the dimension of the vector space enclosing \p this.
    public native long space_dimension();

    /*! \brief
      Returns \f$0\f$, if \p this is empty; otherwise, returns the
      \extref{Affine_Independence_and_Affine_Dimension, affine dimension}
      of \p this.
    */
    public native long affine_dimension();

    //! Returns the system of constraints.
    public native Constraint_System constraints();

    //! Returns a system of (equality) congruences satisfied by \p this.
    public native Congruence_System congruences();

    //! Returns the system of constraints, with no redundant constraint.
    public native Constraint_System minimized_constraints();

    /*! \brief
      Returns a system of (equality) congruences satisfied by \p this,
      with no redundant congruences and having the same affine dimension
      as \p this.
    */
    public native Congruence_System minimized_congruences();

    //! Returns \c true if and only if \p this is an empty polyhedron.
    public native boolean is_empty();

    //! Returns \c true if and only if \p this is a universe polyhedron.
    public native boolean is_universe();

    /*! \brief
      Returns \c true if and only if \p this
      is a bounded polyhedron.
    */
    public native boolean is_bounded();

    //! Returns \c true if and only if \p this is discrete.
    public native boolean is_discrete();

    /*! \brief
      Returns \c true if and only if \p this
      is a topologically closed subset of the vector space.
    */
    public native boolean is_topologically_closed();

    /*! \brief
      Returns \c true if and only if \p this
      contains at least one integer point.
    */
    public native boolean contains_integer_point();

    /*! \brief
      Returns \c true if and only if \p var is constrained in
      \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p var is not a space dimension of \p this.
    */
    public native boolean constrains(Variable var);

    /*! \brief
      Returns \c true if and only if \p expr is
      bounded from above in \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p expr and \p this are dimension-incompatible.
    */
    public native boolean bounds_from_above(Linear_Expression expr);

    /*! \brief
      Returns \c true if and only if \p expr is
      bounded from below in \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p expr and \p this are dimension-incompatible.
    */
    public native boolean bounds_from_below(Linear_Expression expr);

    /*! \brief
      Returns \c true if and only if \p this is not empty
      and \p expr is bounded from above in \p this, in which case
      the supremum value is computed.

      \param expr
      The linear expression to be maximized subject to \p this;

      \param sup_n
      The numerator of the supremum value;

      \param sup_d
      The denominator of the supremum value;

      \param maximum
      \c true if and only if the supremum is also the maximum value.

      \exception Invalid_Argument_Exception
      Thrown if \p expr and \p this are dimension-incompatible.

      If \p this is empty or \p expr is not bounded from above,
      \c false is returned and \p sup_n, \p sup_d
      and \p maximum are left untouched.
    */
    public native boolean maximize(Linear_Expression expr,
				   Coefficient sup_n, Coefficient sup_d,
				   By_Reference<Boolean> maximum);

    /*! \brief
      Returns \c true if and only if \p this is not empty
      and \p expr is bounded from below in \p this, in which case
      the infimum value is computed.

      \param expr
      The linear expression to be minimized subject to \p this;

      \param inf_n
      The numerator of the infimum value;

      \param inf_d
      The denominator of the infimum value;

      \param minimum
      \c true if and only if the infimum is also the minimum value.

      \exception Invalid_Argument_Exception
      Thrown if \p expr and \p this are dimension-incompatible.

      If \p this is empty or \p expr is not bounded from below,
      \c false is returned and \p inf_n, \p inf_d
      and \p minimum are left untouched.
    */
    public native boolean minimize(Linear_Expression expr,
				   Coefficient inf_n, Coefficient inf_d,
				   By_Reference<Boolean> minimum);

    /*! \brief
      Returns \c true if and only if \p this is not empty
      and \p expr is bounded from above in \p this, in which case
      the supremum value and a point where \p expr reaches it are computed.

      \param expr
      The linear expression to be maximized subject to \p this;

      \param sup_n
      The numerator of the supremum value;

      \param sup_d
      The denominator of the supremum value;

      \param maximum
      \c true if and only if the supremum is also the maximum value;

      \param g
      When maximization succeeds, will be assigned the point or
      closure point where \p expr reaches its supremum value.

      \exception Invalid_Argument_Exception
      Thrown if \p expr and \p this are dimension-incompatible.

      If \p this is empty or \p expr is not bounded from above,
      \c false is returned and \p sup_n, \p sup_d, \p maximum
      and \p g are left untouched.
    */
    public native boolean maximize(Linear_Expression expr,
				   Coefficient sup_n, Coefficient sup_d,
				   By_Reference<Boolean> maximum,
				   Generator g);

    /*! \brief
      Returns \c true if and only if \p this is not empty
      and \p expr is bounded from below in \p this, in which case
      the infimum value and a point where \p expr reaches it are computed.

      \param expr
      The linear expression to be minimized subject to \p this;

      \param inf_n
      The numerator of the infimum value;

      \param inf_d
      The denominator of the infimum value;

      \param minimum
      \c true if and only if the infimum is also the minimum value;

      \param g
      When minimization succeeds, will be assigned a point or
      closure point where \p expr reaches its infimum value.

      \exception Invalid_Argument_Exception
      Thrown if \p expr and \p this are dimension-incompatible.

      If \p this is empty or \p expr is not bounded from below,
      \c false is returned and \p inf_n, \p inf_d, \p minimum
      and \p g are left untouched.
    */
    public native boolean minimize(Linear_Expression expr,
				   Coefficient inf_n, Coefficient inf_d,
				   By_Reference<Boolean> minimum,
				   Generator g);

    /*! \brief
      Returns the relations holding between the polyhedron \p this
      and the constraint \p c.

      \exception Invalid_Argument_Exception
      Thrown if \p this and constraint \p c are dimension-incompatible.
    */
    public native Poly_Con_Relation relation_with(Constraint c);

    /*! \brief
      Returns the relations holding between the polyhedron \p this
      and the generator \p g.

      \exception Invalid_Argument_Exception
      Thrown if \p this and generator \p g are dimension-incompatible.
    */
    public native Poly_Gen_Relation relation_with(Generator c);

    /*! \brief
      Returns the relations holding between the polyhedron \p this
      and the congruence \p c.

      \exception Invalid_Argument_Exception
      Thrown if \p this and congruence \p c are dimension-incompatible.
    */
    public native Poly_Con_Relation relation_with(Congruence c);

    //! Returns \c true if and only if \p this contains \p y.
    /*!
      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native boolean contains(Polyhedron y);

    //! Returns \c true if and only if \p this strictly contains \p y.
    /*!
      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native boolean strictly_contains(Polyhedron y);

    //! Returns \c true if and only if \p this and \p y are disjoint.
    /*!
      \exception Invalid_Argument_Exception
      Thrown if \p x and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native boolean is_disjoint_from(Polyhedron y);

    //! Returns \c true if and only if \p this and \p y are equal.
    public native boolean equals(Polyhedron y);

    //! Returns \c true if and only if \p this and \p y are equal.
    public boolean equals(Object y);

    //! Returns a hash code for \p this.
    /*!
      If \p x and \p y are such that <CODE>x == y</CODE>,
      then <CODE>x.hash_code() == y.hash_code()</CODE>.
    */
    public native int hashCode();

    //! Returns the size in bytes of the memory managed by \p this.
    public native long external_memory_in_bytes();

    //! Returns the total size in bytes of the memory occupied by \p this.
    public native long total_memory_in_bytes();

    //! Returns a string representing \p this.
    public native String toString();

    /*! \brief
      Returns a string containing a low-level representation of \p this.

      Useful for debugging purposes.
    */
    public native String ascii_dump();

    //! Checks if all the invariants are satisfied.
    public native boolean OK();

    //@} // Member Functions that Do Not Modify the Polyhedron

    //! \name Space Dimension Preserving Member Functions that May Modify the Polyhedron
    //@{

    /*! \brief
      Adds a copy of constraint \p c to the system of constraints
      of \p this (without minimizing the result).

      \param c
      The constraint that will be added to the system of
      constraints of \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p this and constraint \p c are topology-incompatible
      or dimension-incompatible.
    */
    public native void add_constraint(Constraint c);

    /*! \brief
      Adds a copy of congruence \p cg to \p this,
      if \p cg can be exactly represented by a polyhedron.

      \exception Invalid_Argument_Exception
      Thrown if \p this and congruence \p cg are dimension-incompatible,
      of if \p cg is a proper congruence which is neither a tautology,
      nor a contradiction.
    */
    public native void add_congruence(Congruence cg);

    /*! \brief
      Adds a copy of the constraints in \p cs to the system
      of constraints of \p this (without minimizing the result).

      \param cs
      Contains the constraints that will be added to the system of
      constraints of \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p cs are topology-incompatible or
      dimension-incompatible.
    */
    public native void add_constraints(Constraint_System cs);

    /*! \brief
      Adds a copy of the congruences in \p cgs to \p this,
      if all the congruences can be exactly represented by a polyhedron.

      \param cgs
      The congruences to be added.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p cgs are dimension-incompatible,
      of if there exists in \p cgs a proper congruence which is
      neither a tautology, nor a contradiction.
    */
    public native void add_congruences(Congruence_System cgs);

    /*! \brief
      Uses a copy of constraint \p c to refine \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p this and constraint \p c are dimension-incompatible.
    */
    public native void refine_with_constraint(Constraint c);

    /*! \brief
      Uses a copy of congruence \p cg to refine \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p this and congruence \p cg are dimension-incompatible.
    */
    public native void refine_with_congruence(Congruence cg);

    /*! \brief
      Uses a copy of the constraints in \p cs to refine \p this.

      \param cs
      Contains the constraints used to refine the system of
      constraints of \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p cs are dimension-incompatible.
    */
    public native void refine_with_constraints(Constraint_System cs);

    /*! \brief
      Uses a copy of the congruences in \p cgs to refine \p this.

      \param cgs
      Contains the congruences used to refine the system of
      constraints of \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p cgs are dimension-incompatible.
    */
    public native void refine_with_congruences(Congruence_System cgs);

    /*! \brief
      Assigns to \p this the intersection of \p this and \p y.
      The result is not guaranteed to be minimized.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native void intersection_assign(Polyhedron y);

    /*! \brief
      Assigns to \p this the upper bound of \p this and \p y.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native void upper_bound_assign(Polyhedron y);

    /*! \brief
      Assigns to \p this
      the \extref{Convex_Polyhedral_Difference, poly-difference}
      of \p this and \p y. The result is not guaranteed to be minimized.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native void difference_assign(Polyhedron y);

    /*! \brief
      Assigns to \p this the result of computing the
      \extref{Time_Elapse_Operator, time-elapse} between \p this and \p y.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native void time_elapse_assign(Polyhedron y);

    //! Assigns to \p this its topological closure.
    public native void topological_closure_assign();

    /*! \brief
      Assigns to \p this a \extref{Meet_Preserving_Simplification,
      meet-preserving simplification} of \p this with respect to \p y.
      If \c false is returned, then the intersection is empty.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native boolean simplify_using_context_assign(Polyhedron y);

    /*! \brief
      Assigns to \p this the
      \extref{Single_Update_Affine_Functions, affine image}
      of \p this under the function mapping variable \p var to the
      affine expression specified by \p expr and \p denominator.

      \param var
      The variable to which the affine expression is assigned;

      \param expr
      The numerator of the affine expression;

      \param denominator
      The denominator of the affine expression (optional argument with
      default value 1).

      \exception Invalid_Argument_Exception
      Thrown if \p denominator is zero or if \p expr and \p this are
      dimension-incompatible or if \p var is not a space dimension of
      \p this.
    */
    public native void affine_image(Variable var, Linear_Expression expr,
				    Coefficient denominator);

    /*! \brief
      Assigns to \p this the
      \extref{Single_Update_Affine_Functions, affine preimage}
      of \p this under the function mapping variable \p var to the
      affine expression specified by \p expr and \p denominator.

      \param var
      The variable to which the affine expression is substituted;

      \param expr
      The numerator of the affine expression;

      \param denominator
      The denominator of the affine expression (optional argument with
      default value 1).

      \exception Invalid_Argument_Exception
      Thrown if \p denominator is zero or if \p expr and \p this are
      dimension-incompatible or if \p var is not a space dimension of \p this.
    */
    public native void affine_preimage(Variable var, Linear_Expression expr,
                                       Coefficient denominator);

    /*!
      \brief
      Assigns to \p this the image of \p this with respect to the
      \extref{Single_Update_Bounded_Affine_Relations, bounded affine relation}
      \f$\frac{\mathrm{lb\_expr}}{\mathrm{denominator}}
      \leq \mathrm{var}'
      \leq \frac{\mathrm{ub\_expr}}{\mathrm{denominator}}\f$.

      \param var
      The variable updated by the affine relation;

      \param lb_expr
      The numerator of the lower bounding affine expression;

      \param ub_expr
      The numerator of the upper bounding affine expression;

      \param denominator
      The (common) denominator for the lower and upper bounding
      affine expressions (optional argument with default value 1).

      \exception Invalid_Argument_Exception
      Thrown if \p denominator is zero or if \p lb_expr (resp., \p ub_expr)
      and \p this are dimension-incompatible or if \p var is not a space
      dimension of \p this.
    */
    public native void bounded_affine_image(Variable var,
					    Linear_Expression lb_expr,
					    Linear_Expression ub_expr,
					    Coefficient denominator);

    /*! \brief
      Assigns to \p this the preimage of \p this with respect to the
      \extref{Single_Update_Bounded_Affine_Relations, bounded affine relation}
      \f$\frac{\mathrm{lb\_expr}}{\mathrm{denominator}}
      \leq \mathrm{var}'
      \leq \frac{\mathrm{ub\_expr}}{\mathrm{denominator}}\f$.

      \param var
      The variable updated by the affine relation;

      \param lb_expr
      The numerator of the lower bounding affine expression;

      \param ub_expr
      The numerator of the upper bounding affine expression;

      \param denominator
      The (common) denominator for the lower and upper bounding
      affine expressions (optional argument with default value 1).

      \exception Invalid_Argument_Exception
      Thrown if \p denominator is zero or if \p lb_expr (resp., \p ub_expr)
      and \p this are dimension-incompatible or if \p var is not a space
      dimension of \p this.
    */
    public native void bounded_affine_preimage(Variable var,
                                               Linear_Expression lb_expr,
                                               Linear_Expression ub_expr,
                                               Coefficient denominator);

    /*! \brief
      Assigns to \p this the image of \p this with respect to the
      \extref{Generalized_Affine_Relations, generalized affine relation}
      \f$\mathrm{var}' \relsym \frac{\mathrm{expr}}{\mathrm{denominator}}\f$,
      where \f$\mathord{\relsym}\f$ is the relation symbol encoded
      by \p relsym.

      \param var
      The left hand side variable of the generalized affine relation;

      \param relsym
      The relation symbol;

      \param expr
      The numerator of the right hand side affine expression;

      \param denominator
      The denominator of the right hand side affine expression (optional
      argument with default value 1).

      \exception Invalid_Argument_Exception
      Thrown if \p denominator is zero or if \p expr and \p this are
      dimension-incompatible or if \p var is not a space dimension of \p this
      or if \p this is a C_Polyhedron and \p relsym is a strict
      relation symbol.
    */
    public native void generalized_affine_image(Variable var,
                                                Relation_Symbol relsym,
                                                Linear_Expression expr,
                                                Coefficient denominator);

    /*! \brief
      Assigns to \p this the preimage of \p this with respect to the
      \extref{Generalized_Affine_Relations, generalized affine relation}
      \f$\mathrm{var}' \relsym \frac{\mathrm{expr}}{\mathrm{denominator}}\f$,
      where \f$\mathord{\relsym}\f$ is the relation symbol encoded
      by \p relsym.

      \param var
      The left hand side variable of the generalized affine relation;

      \param relsym
      The relation symbol;

      \param expr
      The numerator of the right hand side affine expression;

      \param denominator
      The denominator of the right hand side affine expression (optional
      argument with default value 1).

      \exception Invalid_Argument_Exception
      Thrown if \p denominator is zero or if \p expr and \p this are
      dimension-incompatible or if \p var is not a space dimension of \p this
      or if \p this is a C_Polyhedron and \p relsym is a strict
      relation symbol.
    */
    public native void generalized_affine_preimage(Variable var,
                                                   Relation_Symbol relsym,
                                                   Linear_Expression expr,
                                                   Coefficient denominator);

    /*! \brief
      Assigns to \p this the image of \p this with respect to the
      \extref{Generalized_Affine_Relations, generalized affine relation}
      \f$\mathrm{lhs}' \relsym \mathrm{rhs}\f$, where
      \f$\mathord{\relsym}\f$ is the relation symbol encoded by \p relsym.

      \param lhs
      The left hand side affine expression;

      \param relsym
      The relation symbol;

      \param rhs
      The right hand side affine expression.

      \exception Invalid_Argument_Exception
      Thrown if \p this is dimension-incompatible with \p lhs or \p rhs
      or if \p this is a C_Polyhedron and \p relsym is a strict
      relation symbol.
    */
    public native void generalized_affine_image(Linear_Expression lhs,
                                                Relation_Symbol relsym,
                                                Linear_Expression rhs);

    /*! \brief
      Assigns to \p this the preimage of \p this with respect to the
      \extref{Generalized_Affine_Relations, generalized affine relation}
      \f$\mathrm{lhs}' \relsym \mathrm{rhs}\f$, where
      \f$\mathord{\relsym}\f$ is the relation symbol encoded by \p relsym.

      \param lhs
      The left hand side affine expression;

      \param relsym
      The relation symbol;

      \param rhs
      The right hand side affine expression.

      \exception Invalid_Argument_Exception
      Thrown if \p this is dimension-incompatible with \p lhs or \p rhs
      or if \p this is a C_Polyhedron and \p relsym is a strict
      relation symbol.
    */
    public native void generalized_affine_preimage(Linear_Expression lhs,
                                                   Relation_Symbol relsym,
                                                   Linear_Expression rhs);

    /*! \brief
      Computes the \extref{Cylindrification, cylindrification} of \p this
      with respect to space dimension \p var, assigning the result to \p this.

      \param var
      The space dimension that will be unconstrained.

      \exception Invalid_Argument_Exception
      Thrown if \p var is not a space dimension of \p this.
    */
    public native void unconstrain_space_dimension(Variable var);

    /*! \brief
      Computes the \extref{Cylindrification, cylindrification} of \p this
      with respect to the set of space dimensions \p vars,
      assigning the result to \p this.

      \param vars
      The set of space dimension that will be unconstrained.

      \exception Invalid_Argument_Exception
      Thrown if \p this is dimension-incompatible with one of the
      Variable objects contained in \p vars.
    */
    public native void unconstrain_space_dimensions(Variables_Set vars);

    /*! \brief
      Assigns to \p this the result of computing the
      \extref{H79_widening, H79-widening} between \p this and \p y.

      \param y
      A polyhedron that <EM>must</EM> be contained in \p this;

      \param tp
      A reference to an unsigned variable storing the number of
      available tokens (to be used when applying the
      \extref{Widening_with_Tokens, widening with tokens} delay technique).

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native void widening_assign(Polyhedron y,
                                       By_Reference<Integer> tp);

    //@} // Space Dimension Preserving Member Functions that May Modify [...]

    //! \name Member Functions that May Modify the Dimension of the Vector Space
    //@{

    /*! \brief
      Swaps \p this with polyhedron \p y.
      (\p this and \p y can be dimension-incompatible.)

      \exception Invalid_Argument_Exception
      Thrown if \p x and \p y are topology-incompatible.
    */
    public native void swap(Polyhedron y);

    /*! \brief
      Adds \p m new space dimensions and embeds the old polyhedron
      in the new vector space.

      \param m
      The number of dimensions to add.

      \exception Length_Error_Exception
      Thrown if adding \p m new space dimensions would cause the
      vector space to exceed dimension <CODE>max_space_dimension()</CODE>.
    */
    public native void add_space_dimensions_and_embed(long m);

    /*! \brief
      Adds \p m new space dimensions to the polyhedron
      and does not embed it in the new vector space.

      \param m
      The number of space dimensions to add.

      \exception Length_Error_Exception
      Thrown if adding \p m new space dimensions would cause the
      vector space to exceed dimension <CODE>max_space_dimension()</CODE>.
    */
    public native void add_space_dimensions_and_project(long m);

    /*! \brief
      Assigns to \p this the \extref{Concatenating_Polyhedra, concatenation}
      of \p this and \p y, taken in this order.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible.

      \exception Length_Error_Exception
      Thrown if the concatenation would cause the vector space
      to exceed dimension <CODE>max_space_dimension()</CODE>.
    */
    public native void concatenate_assign(Polyhedron y);

    //! Removes all the specified dimensions from the vector space.
    /*!
      \param vars
      The set of Variable objects corresponding to the space dimensions
      to be removed.

      \exception Invalid_Argument_Exception
      Thrown if \p this is dimension-incompatible with one of the
      Variable objects contained in \p vars.
    */
    public native void remove_space_dimensions(Variables_Set vars);

    /*! \brief
      Removes the higher dimensions of the vector space so that
      the resulting space will have dimension \p new_dimension.

      \exception Invalid_Argument_Exception
      Thrown if \p new_dimensions is greater than the space dimension of
      \p this.
    */
    public native void remove_higher_space_dimensions(long
						      new_dimension);

    //! Creates \p m copies of the space dimension corresponding to \p var.
    /*!
      \param var
      The variable corresponding to the space dimension to be replicated;

      \param m
      The number of replicas to be created.

      \exception Invalid_Argument_Exception
      Thrown if \p var does not correspond to a dimension of the vector space.

      \exception Length_Error_Exception
      Thrown if adding \p m new space dimensions would cause the
      vector space to exceed dimension <CODE>max_space_dimension()</CODE>.
    */
    public native void expand_space_dimension(Variable var, long m);

    //! Folds the space dimensions in \p vars into \p dest.
    /*!
      \param vars
      The set of Variable objects corresponding to the space dimensions
      to be folded;

      \param dest
      The variable corresponding to the space dimension that is the
      destination of the folding operation.

      \exception Invalid_Argument_Exception
      Thrown if \p this is dimension-incompatible with \p dest or with
      one of the Variable objects contained in \p vars.
      Also thrown if \p dest is contained in \p vars.
    */
    public native void fold_space_dimensions(Variables_Set vars,
					     Variable dest);

    /*! \brief
      Remaps the dimensions of the vector space according to
      a \extref{Mapping_the_Dimensions_of_the_Vector_Space, partial function}.

      \param pfunc
      The partial function specifying the destiny of each space dimension.
    */
    public native void map_space_dimensions(Partial_Function pfunc);

    //@} // Member Functions that May Modify the Dimension of the Vector Space

    /*! \name Ad Hoc Functions for (C or NNC) Polyhedra
      The functions listed here below, being specific of the polyhedron
      domains, do not have a correspondence in other semantic geometric
      descriptions.
    */
    //@{

    //! Returns the system of generators.
    public native Generator_System generators();

    //! Returns the system of generators, with no redundant generator.
    public native Generator_System minimized_generators();

    /*! \brief
      Adds a copy of generator \p g to the system of generators
      of \p this (without minimizing the result).

      \exception Invalid_Argument_Exception
      Thrown if \p this and generator \p g are topology-incompatible or
      dimension-incompatible, or if \p this is an empty polyhedron and
      \p g is not a point.
    */
    public native void add_generator(Generator g);

    /*! \brief
      Adds a copy of the generators in \p gs to the system
      of generators of \p this (without minimizing the result).

      \param gs
      Contains the generators that will be added to the system of
      generators of \p this.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p gs are topology-incompatible or
      dimension-incompatible, or if \p this is empty and the system of
      generators \p gs is not empty, but has no points.
    */
    public native void add_generators(Generator_System gs);

    //! Same as upper_bound_assign.
    public native void poly_hull_assign(Polyhedron y);

    //! Same as difference_assign.
    public native void poly_difference_assign(Polyhedron y);

    /*! \brief
      Assigns to \p this the result of computing the
      \extref{BHRZ03_widening, BHRZ03-widening} between \p this and \p y.

      \param y
      A polyhedron that <EM>must</EM> be contained in \p this;

      \param tp
      A reference to an unsigned variable storing the number of
      available tokens (to be used when applying the
      \extref{Widening_with_Tokens, widening with tokens} delay technique).

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native void BHRZ03_widening_assign(Polyhedron y,
                                              By_Reference<Integer> tp);

    /*! \brief
      Assigns to \p this the result of computing the
      \extref{H79_widening, H79-widening} between \p this and \p y.

      \param y
      A polyhedron that <EM>must</EM> be contained in \p this;

      \param tp
      A reference to an unsigned variable storing the number of
      available tokens (to be used when applying the
      \extref{Widening_with_Tokens, widening with tokens} delay technique).

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are topology-incompatible or
      dimension-incompatible.
    */
    public native void H79_widening_assign(Polyhedron y,
                                           By_Reference<Integer> tp);

    /*! \brief
      Improves the result of the \extref{BHRZ03_widening, BHRZ03-widening}
      computation by also enforcing those constraints in \p cs that are
      satisfied by all the points of \p this.

      \param y
      A polyhedron that <EM>must</EM> be contained in \p this;

      \param cs
      The system of constraints used to improve the widened polyhedron;

      \param tp
      A reference to an unsigned variable storing the number of
      available tokens (to be used when applying the
      \extref{Widening_with_Tokens, widening with tokens} delay technique).

      \exception Invalid_Argument_Exception
      Thrown if \p this, \p y and \p cs are topology-incompatible or
      dimension-incompatible.
    */
    public native
	void limited_BHRZ03_extrapolation_assign(Polyhedron y,
						 Constraint_System cs,
						 By_Reference<Integer> tp);

    /*! \brief
      Improves the result of the \extref{H79_widening, H79-widening}
      computation by also enforcing those constraints in \p cs that are
      satisfied by all the points of \p this.

      \param y
      A polyhedron that <EM>must</EM> be contained in \p this;

      \param cs
      The system of constraints used to improve the widened polyhedron;

      \param tp
      A reference to an unsigned variable storing the number of
      available tokens (to be used when applying the
      \extref{Widening_with_Tokens, widening with tokens} delay technique).

      \exception Invalid_Argument_Exception
      Thrown if \p this, \p y and \p cs are topology-incompatible or
      dimension-incompatible.
    */
    public native
	void limited_H79_extrapolation_assign(Polyhedron y,
                                              Constraint_System cs,
                                              By_Reference<Integer> tp);

    /*! \brief
      Improves the result of the \extref{BHRZ03_widening, BHRZ03-widening}
      computation by also enforcing those constraints in \p cs that are
      satisfied by all the points of \p this, plus all the constraints
      of the form \f$\pm x \leq r\f$ and \f$\pm x < r\f$, with
      \f$r \in \Qset\f$, that are satisfied by all the points of \p this.

      \param y
      A polyhedron that <EM>must</EM> be contained in \p this;

      \param cs
      The system of constraints used to improve the widened polyhedron;

      \param tp
      A reference to an unsigned variable storing the number of
      available tokens (to be used when applying the
      \extref{Widening_with_Tokens, widening with tokens} delay technique).

      \exception Invalid_Argument_Exception
      Thrown if \p this, \p y and \p cs are topology-incompatible or
      dimension-incompatible.
    */
    public native
	void bounded_BHRZ03_extrapolation_assign(Polyhedron y,
						 Constraint_System cs,
						 By_Reference<Integer> tp);

    /*! \brief
      Improves the result of the \extref{H79_widening, H79-widening}
      computation by also enforcing those constraints in \p cs that are
      satisfied by all the points of \p this, plus all the constraints
      of the form \f$\pm x \leq r\f$ and \f$\pm x < r\f$, with
      \f$r \in \Qset\f$, that are satisfied by all the points of \p this.

      \param y
      A polyhedron that <EM>must</EM> be contained in \p this;

      \param cs
      The system of constraints used to improve the widened polyhedron;

      \param tp
      A reference to an unsigned variable storing the number of
      available tokens (to be used when applying the
      \extref{Widening_with_Tokens, widening with tokens} delay technique).

      \exception Invalid_Argument_Exception
      Thrown if \p this, \p y and \p cs are topology-incompatible or
      dimension-incompatible.
    */
    public native
	void bounded_H79_extrapolation_assign(Polyhedron y,
                                              Constraint_System cs,
                                              By_Reference<Integer> tp);

    //@} // Ad Hoc Functions for (C or NNC) Polyhedra

} // class Polyhedron


//! A topologically closed convex polyhedron.
/*! \ingroup PPL_Java_interface */
public class C_Polyhedron extends Polyhedron {

    //! \name Standard Constructors and Destructor
    //@{

    //! Builds a new C polyhedron of dimension \p d.
    /*!
      If \p kind is \c EMPTY, the newly created polyhedron will be empty;
      otherwise, it will be a universe polyhedron.
    */
    public C_Polyhedron(long d, Degenerate_Element kind);

    //! Builds a new C polyhedron that is copy of \p y.
    public C_Polyhedron(C_Polyhedron y);

    //! Builds a new C polyhedron that is a copy of \p ph.
    /*!
      The complexity argument is ignored.
    */
    public C_Polyhedron(C_Polyhedron y, Complexity_Class complexity);

    //! Builds a new C polyhedron from the system of constraints \p cs.
    /*!
      The new polyhedron will inherit the space dimension of \p cs.
    */
    public C_Polyhedron(Constraint_System cs);

    //! Builds a new C polyhedron from the system of congruences \p cgs.
    /*!
      The new polyhedron will inherit the space dimension of \p cgs.
    */
    public C_Polyhedron(Congruence_System cgs);

    /*! \brief
      Releases all resources managed by \p this,
      also resetting it to a null reference.
    */
    public native void free();

    //@} // Standard Constructors and Destructor

    /*! \name Constructors Behaving as Conversion Operators
      Besides the conversions listed here below, the library also
      provides conversion operators that build a semantic geometric
      description starting from \b any other semantic geometric
      description (e.g., <code>Grid(C_Polyhedron y)</code>,
      <code>C_Polyhedron(BD_Shape_mpq_class y)</code>, etc.).
      Clearly, the conversion operators are only available if both
      the source and the target semantic geometric descriptions have
      been enabled when configuring the library.
      The conversions also taking as argument a complexity class
      sometimes provide non-trivial precision/efficiency trade-offs.
    */
    //@{

    /*! \brief
      Builds a C polyhedron that is a copy of the topological closure
      of the NNC polyhedron \p y.
    */
    public C_Polyhedron(NNC_Polyhedron y);

    /*! \brief
      Builds a C polyhedron that is a copy of the topological closure
      of the NNC polyhedron \p y.

      The complexity argument is ignored, since the exact constructor
      has polynomial complexity.
    */
    public C_Polyhedron(NNC_Polyhedron y, Complexity_Class complexity);

    //! Builds a new C polyhedron from the system of generators \p gs.
    /*!
      The new polyhedron will inherit the space dimension of \p gs.
    */
    public C_Polyhedron(Generator_System gs);

    //@} // Constructors Behaving as Conversion Operators

    //! \name Other Methods
    //@{

    /*! \brief
      If the upper bound of \p this and \p y is exact it is assigned
      to \p this and \c true is returned; otherwise \c false is returned.

      \exception Invalid_Argument_Exception
      Thrown if \p this and \p y are dimension-incompatible.
    */
    public native boolean upper_bound_assign_if_exact(C_Polyhedron y);

    //@} // Other Methods.

    //! Partitions \p q with respect to \p p.
    /*!
      Let \p p and \p q be two polyhedra.
      The function returns a pair object \p r such that
      - <CODE>r.first</CODE> is the intersection of \p p and \p q;
      - <CODE>r.second</CODE> has the property that all its elements are
      pairwise disjoint and disjoint from \p p;
      - the set-theoretical union of <CODE>r.first</CODE> with all the
      elements of <CODE>r.second</CODE> gives \p q (i.e., <CODE>r</CODE>
      is the representation of a partition of \p q).
    */
    public static native
        Pair<C_Polyhedron, Pointset_Powerset_NNC_Polyhedron>
        linear_partition(C_Polyhedron p, C_Polyhedron q);

    //! Releases all resources managed by \p this.
    protected native void finalize();

} // class C_Polyhedron


//! A powerset of C_Polyhedron objects.
/*! \ingroup PPL_Java_interface
  The powerset domains can be instantiated by taking as a base domain
  any fixed semantic geometric description
  (C and NNC polyhedra, BD and octagonal shapes, boxes and grids).
  An element of the powerset domain represents a disjunctive collection
  of base objects (its disjuncts), all having the same space dimension.

  Besides the methods that are available in all semantic geometric
  descriptions (whose documentation is not repeated here),
  the powerset domain also provides several ad hoc methods.
  In particular, the iterator types allow for the examination and
  manipulation of the collection of disjuncts.
*/
public class Pointset_Powerset_C_Polyhedron extends PPL_Object {

    //! \name Ad Hoc Functions for Pointset_Powerset domains
    /*@{*/

    /*! \brief
      Drops from the sequence of disjuncts in \p this all the non-maximal
      elements, so that a non-redundant powerset if obtained.
    */
    public native void omega_reduce();

    //! Returns the number of disjuncts.
    /*!
      If present, Omega-redundant elements will be counted too.
    */
    public native long size();

    //! Returns \c true if and only if \p this geometrically covers \p y.
    public native boolean
        geometrically_covers(Pointset_Powerset_C_Polyhedron y);

    //! Returns \c true if and only if \p this is geometrically equal to \p y.
    public native boolean
        geometrically_equals(Pointset_Powerset_C_Polyhedron y);

    /*! \brief
      Returns an iterator referring to the beginning of the sequence
      of disjuncts of \p this.
    */
    public native Pointset_Powerset_C_Polyhedron_Iterator begin_iterator();

    /*! \brief
      Returns an iterator referring to past the end of the sequence
      of disjuncts of \p this.
    */
    public native Pointset_Powerset_C_Polyhedron_Iterator end_iterator();

    //! Adds to \p this a copy of disjunct \p d.
    public native void add_disjunct(C_Polyhedron d);

    // FIXME: this method needs correction, as it returns nothing.
    /*! \brief
      Drops from \p this the disjunct referred by \p iter; returns an
      iterator referring to the disjunct following the dropped one.
    */
    public native void
        drop_disjunct(Pointset_Powerset_C_Polyhedron_Iterator iter);


    /*! \brief
      Drops from \p this all the disjuncts from \p first to \p last
      (excluded).
    */
    public native void
        drop_disjuncts(Pointset_Powerset_C_Polyhedron_Iterator first,
                       Pointset_Powerset_C_Polyhedron_Iterator last);


    /*! \brief
      Modifies \p this by (recursively) merging together the pairs of
      disjuncts whose upper-bound is the same as their set-theoretical union.
    */
    public native void pairwise_reduce();

    /*@}*/ /* Ad Hoc Functions for Pointset_Powerset domains */

} // class Pointset_Powerset_C_Polyhedron


//! An iterator class for the disjuncts of a Pointset_Powerset_C_Polyhedron.
/*! \ingroup PPL_Java_interface */
public class Pointset_Powerset_C_Polyhedron_Iterator extends PPL_Object {

    //! Builds a copy of iterator \p y.
    public Pointset_Powerset_C_Polyhedron_Iterator
        (Pointset_Powerset_C_Polyhedron_Iterator y);

    //! Returns \c true if and only if \p this and \p itr are equal.
    public native boolean equals(Pointset_Powerset_C_Polyhedron_Iterator itr);

    //! Modifies \p this so that it refers to the next disjunct.
    public native void next();

    //! Modifies \p this so that it refers to the previous disjunct.
    public native void prev();

    //! Returns the disjunct referenced by \p this.
    /*!
      \warning
      On exit, the C_Polyhedron disjunct is still owned by the powerset
      object: any function call on the owning powerset object may
      invalidate it. Moreover, the disjunct is meant to be immutable
      and should not be modified in any way (its resources will
      be released when deleting the owning powerset). If really needed,
      the disjunct may be copied into a new object, which will be under
      control of the user.
    */
    public native C_Polyhedron get_disjunct();

    //! Releases resources and resets \p this to a null reference.
    public native void free();

    //! Releases the resources managed by \p this.
    protected native void finalize();

} // class Pointset_Powerset_C_Polyhedron_Iterator
