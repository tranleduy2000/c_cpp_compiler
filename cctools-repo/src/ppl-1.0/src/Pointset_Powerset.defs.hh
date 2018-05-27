/* Pointset_Powerset class declaration.
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

#ifndef PPL_Pointset_Powerset_defs_hh
#define PPL_Pointset_Powerset_defs_hh

#include "Pointset_Powerset.types.hh"
#include "globals.defs.hh"
#include "BHRZ03_Certificate.types.hh"
#include "Constraint.types.hh"
#include "Constraint_System.types.hh"
#include "Congruence.types.hh"
#include "Congruence_System.types.hh"
#include "C_Polyhedron.defs.hh"
#include "NNC_Polyhedron.defs.hh"
#include "Polyhedron.defs.hh"
#include "Grid.defs.hh"
#include "Partially_Reduced_Product.defs.hh"
#include "Variables_Set.types.hh"
#include "Determinate.defs.hh"
#include "Powerset.defs.hh"
#include "Poly_Con_Relation.defs.hh"
#include "Poly_Gen_Relation.defs.hh"
#include <iosfwd>
#include <list>
#include <map>

//! The powerset construction instantiated on PPL pointset domains.
/*! \ingroup PPL_CXX_interface */
/*!
  \warning
  At present, the supported instantiations for the
  disjunct domain template \p PSET are the simple pointset domains:
  <CODE>C_Polyhedron</CODE>,
  <CODE>NNC_Polyhedron</CODE>,
  <CODE>Grid</CODE>,
  <CODE>Octagonal_Shape<T></CODE>,
  <CODE>BD_Shape<T></CODE>,
  <CODE>Box<T></CODE>.
*/
template <typename PSET>
class Parma_Polyhedra_Library::Pointset_Powerset
  : public Parma_Polyhedra_Library::Powerset
<Parma_Polyhedra_Library::Determinate<PSET> > {
public:
  typedef PSET element_type;

private:
  typedef Determinate<PSET> Det_PSET;
  typedef Powerset<Det_PSET> Base;

public:
  //! Returns the maximum space dimension a Pointset_Powerset<PSET> can handle.
  static dimension_type max_space_dimension();

  //! \name Constructors
  //@{

  //! Builds a universe (top) or empty (bottom) Pointset_Powerset.
  /*!
    \param num_dimensions
    The number of dimensions of the vector space enclosing the powerset;

    \param kind
    Specifies whether the universe or the empty powerset has to be built.
  */
  explicit
  Pointset_Powerset(dimension_type num_dimensions = 0,
		    Degenerate_Element kind = UNIVERSE);

  //! Ordinary copy constructor.
  /*!
    The complexity argument is ignored.
  */
  Pointset_Powerset(const Pointset_Powerset& y,
                    Complexity_Class complexity = ANY_COMPLEXITY);

  /*! \brief
    Conversion constructor: the type <CODE>QH</CODE> of the disjuncts
    in the source powerset is different from <CODE>PSET</CODE>.

    \param y
    The powerset to be used to build the new powerset.

    \param complexity
    The maximal complexity of any algorithms used.
  */
  template <typename QH>
  explicit Pointset_Powerset(const Pointset_Powerset<QH>& y,
                             Complexity_Class complexity = ANY_COMPLEXITY);

  /*! \brief
    Creates a Pointset_Powerset from a product
    This will be created as a single disjunct of type PSET that
    approximates the product.
  */
  template <typename QH1, typename QH2, typename R>
  explicit
  Pointset_Powerset(const Partially_Reduced_Product<QH1, QH2, R>& prp,
                    Complexity_Class complexity = ANY_COMPLEXITY);

  /*! \brief
    Creates a Pointset_Powerset with a single disjunct approximating
    the system of constraints \p cs.
  */
  explicit Pointset_Powerset(const Constraint_System& cs);

  /*! \brief
    Creates a Pointset_Powerset with a single disjunct approximating
    the system of congruences \p cgs.
  */
  explicit Pointset_Powerset(const Congruence_System& cgs);


  //! Builds a pointset_powerset out of a closed polyhedron.
  /*!
    Builds a powerset that is either empty (if the polyhedron is found
    to be empty) or contains a single disjunct approximating the
    polyhedron; this must only use algorithms that do not exceed the
    specified complexity.  The powerset inherits the space dimension
    of the polyhedron.

    \param ph
    The closed polyhedron to be used to build the powerset.

    \param complexity
    The maximal complexity of any algorithms used.

    \exception std::length_error
    Thrown if the space dimension of \p ph exceeds the maximum
    allowed space dimension.
  */
  explicit Pointset_Powerset(const C_Polyhedron& ph,
                             Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a pointset_powerset out of an nnc polyhedron.
  /*!
    Builds a powerset that is either empty (if the polyhedron is found
    to be empty) or contains a single disjunct approximating the
    polyhedron; this must only use algorithms that do not exceed the
    specified complexity.  The powerset inherits the space dimension
    of the polyhedron.

    \param ph
    The closed polyhedron to be used to build the powerset.

    \param complexity
    The maximal complexity of any algorithms used.

    \exception std::length_error
    Thrown if the space dimension of \p ph exceeds the maximum
    allowed space dimension.
  */
  explicit Pointset_Powerset(const NNC_Polyhedron& ph,
                             Complexity_Class complexity = ANY_COMPLEXITY);


  //! Builds a pointset_powerset out of a grid.
  /*!
    If the grid is nonempty, builds a powerset containing a single
    disjunct approximating the grid. Builds the empty powerset
    otherwise. The powerset inherits the space dimension of the grid.

    \param gr
    The grid to be used to build the powerset.

    \param complexity
    This argument is ignored.

    \exception std::length_error
    Thrown if the space dimension of \p gr exceeds the maximum
    allowed space dimension.
  */
  explicit Pointset_Powerset(const Grid& gr,
                             Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a pointset_powerset out of an octagonal shape.
  /*!
    If the octagonal shape is nonempty, builds a powerset
    containing a single disjunct approximating the octagonal
    shape. Builds the empty powerset otherwise. The powerset
    inherits the space dimension of the octagonal shape.

    \param os
    The octagonal shape to be used to build the powerset.

    \param complexity
    This argument is ignored.

    \exception std::length_error
    Thrown if the space dimension of \p os exceeds the maximum
    allowed space dimension.
  */
  template <typename T>
  explicit Pointset_Powerset(const Octagonal_Shape<T>& os,
                             Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a pointset_powerset out of a bd shape.
  /*!
    If the bd shape is nonempty, builds a powerset containing a
    single disjunct approximating the bd shape. Builds the empty
    powerset otherwise.  The powerset inherits the space dimension
    of the bd shape.

    \param bds
    The bd shape to be used to build the powerset.

    \param complexity
    This argument is ignored.

    \exception std::length_error
    Thrown if the space dimension of \p bds exceeds the maximum
    allowed space dimension.
  */
  template <typename T>
  explicit Pointset_Powerset(const BD_Shape<T>& bds,
                             Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a pointset_powerset out of a box.
  /*!
    If the box is nonempty, builds a powerset containing a single
    disjunct approximating the box. Builds the empty powerset
    otherwise.  The powerset inherits the space dimension of the box.

    \param box
    The box to be used to build the powerset.

    \param complexity
    This argument is ignored.

    \exception std::length_error
    Thrown if the space dimension of \p box exceeds the maximum
    allowed space dimension.
  */
  template <typename Interval>
  explicit Pointset_Powerset(const Box<Interval>& box,
                             Complexity_Class complexity = ANY_COMPLEXITY);

  //@} // Constructors and Destructor

  //! \name Member Functions that Do Not Modify the Pointset_Powerset
  //@{

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type affine_dimension() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is
    an empty powerset.
  */
  bool is_empty() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this
    is the top element of the powerset lattice.
  */
  bool is_universe() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if all the disjuncts
    in \p *this are topologically closed.
  */
  bool is_topologically_closed() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if all elements in \p *this
    are bounded.
  */
  bool is_bounded() const;

  //! Returns <CODE>true</CODE> if and only if \p *this and \p y are disjoint.
  /*!
    \exception std::invalid_argument
    Thrown if \p x and \p y are topology-incompatible or
    dimension-incompatible.
  */
  bool is_disjoint_from(const Pointset_Powerset& y) const;

  //! Returns <CODE>true</CODE> if and only if \p *this is discrete.
  bool is_discrete() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p var is constrained in
    \p *this.

    \exception std::invalid_argument
    Thrown if \p var is not a space dimension of \p *this.

    \note
    A variable is constrained if there exists a non-redundant disjunct
    that is constraining the variable: this definition relies on the
    powerset lattice structure and may be somewhat different from the
    geometric intuition.
    For instance, variable \f$x\f$ is constrained in the powerset
    \f[
      \mathit{ps} = \bigl\{ \{ x \geq 0 \}, \{ x \leq 0 \} \bigr\},
    \f]
    even though \f$\mathit{ps}\f$ is geometrically equal to the
    whole vector space.
  */
  bool constrains(Variable var) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p expr is
    bounded from above in \p *this.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.
  */
  bool bounds_from_above(const Linear_Expression& expr) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p expr is
    bounded from below in \p *this.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.
  */
  bool bounds_from_below(const Linear_Expression& expr) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty
    and \p expr is bounded from above in \p *this, in which case
    the supremum value is computed.

    \param expr
    The linear expression to be maximized subject to \p *this;

    \param sup_n
    The numerator of the supremum value;

    \param sup_d
    The denominator of the supremum value;

    \param maximum
    <CODE>true</CODE> if and only if the supremum is also the maximum value.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded from above,
    <CODE>false</CODE> is returned and \p sup_n, \p sup_d
    and \p maximum are left untouched.
  */
  bool maximize(const Linear_Expression& expr,
		Coefficient& sup_n, Coefficient& sup_d, bool& maximum) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty
    and \p expr is bounded from above in \p *this, in which case
    the supremum value and a point where \p expr reaches it are computed.

    \param expr
    The linear expression to be maximized subject to \p *this;

    \param sup_n
    The numerator of the supremum value;

    \param sup_d
    The denominator of the supremum value;

    \param maximum
    <CODE>true</CODE> if and only if the supremum is also the maximum value;

    \param g
    When maximization succeeds, will be assigned the point or
    closure point where \p expr reaches its supremum value.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded from above,
    <CODE>false</CODE> is returned and \p sup_n, \p sup_d, \p maximum
    and \p g are left untouched.
  */
  bool maximize(const Linear_Expression& expr,
		Coefficient& sup_n, Coefficient& sup_d, bool& maximum,
		Generator& g) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty
    and \p expr is bounded from below in \p *this, in which case
    the infimum value is computed.

    \param expr
    The linear expression to be minimized subject to \p *this;

    \param inf_n
    The numerator of the infimum value;

    \param inf_d
    The denominator of the infimum value;

    \param minimum
    <CODE>true</CODE> if and only if the infimum is also the minimum value.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded from below,
    <CODE>false</CODE> is returned and \p inf_n, \p inf_d
    and \p minimum are left untouched.
  */
  bool minimize(const Linear_Expression& expr,
		Coefficient& inf_n, Coefficient& inf_d, bool& minimum) const;


  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is not empty
    and \p expr is bounded from below in \p *this, in which case
    the infimum value and a point where \p expr reaches it are computed.

    \param expr
    The linear expression to be minimized subject to \p *this;

    \param inf_n
    The numerator of the infimum value;

    \param inf_d
    The denominator of the infimum value;

    \param minimum
    <CODE>true</CODE> if and only if the infimum is also the minimum value;

    \param g
    When minimization succeeds, will be assigned a point or
    closure point where \p expr reaches its infimum value.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded from below,
    <CODE>false</CODE> is returned and \p inf_n, \p inf_d, \p minimum
    and \p g are left untouched.
  */
  bool minimize(const Linear_Expression& expr,
		Coefficient& inf_n, Coefficient& inf_d, bool& minimum,
		Generator& g) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this geometrically
    covers \p y, i.e., if any point (in some element) of \p y is also
    a point (of some element) of \p *this.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.

    \warning
    This may be <EM>really</EM> expensive!
  */
  bool geometrically_covers(const Pointset_Powerset& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this is geometrically
    equal to \p y, i.e., if (the elements of) \p *this and \p y
    contain the same set of points.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.

    \warning
    This may be <EM>really</EM> expensive!
  */
  bool geometrically_equals(const Pointset_Powerset& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if each disjunct
      of \p y is contained in a disjunct of \p *this.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are topology-incompatible or
    dimension-incompatible.
  */
  bool contains(const Pointset_Powerset& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if each disjunct
      of \p y is strictly contained in a disjunct of \p *this.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are topology-incompatible or
    dimension-incompatible.
  */
  bool strictly_contains(const Pointset_Powerset& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this
    contains at least one integer point.
  */
  bool contains_integer_point() const;

  /*! \brief
    Returns the relations holding between the powerset \p *this
    and the constraint \p c.

    \exception std::invalid_argument
    Thrown if \p *this and constraint \p c are dimension-incompatible.
  */
  Poly_Con_Relation relation_with(const Constraint& c) const;

  /*! \brief
    Returns the relations holding between the powerset \p *this
    and the generator \p g.

    \exception std::invalid_argument
    Thrown if \p *this and generator \p g are dimension-incompatible.
  */
  Poly_Gen_Relation relation_with(const Generator& g) const;

  /*! \brief
    Returns the relations holding between the powerset \p *this
    and the congruence \p c.

    \exception std::invalid_argument
    Thrown if \p *this and congruence \p c are dimension-incompatible.
  */
  Poly_Con_Relation relation_with(const Congruence& cg) const;

  /*! \brief
    Returns a lower bound to the total size in bytes of the memory
    occupied by \p *this.
  */
  memory_size_type total_memory_in_bytes() const;

  /*! \brief
    Returns a lower bound to the size in bytes of the memory
    managed by \p *this.
  */
  memory_size_type external_memory_in_bytes() const;

  /*! \brief
    Returns a 32-bit hash code for \p *this.

    If \p x and \p y are such that <CODE>x == y</CODE>,
    then <CODE>x.hash_code() == y.hash_code()</CODE>.
  */
  int32_t hash_code() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  //@} // Member Functions that Do Not Modify the Pointset_Powerset

  //! \name Space Dimension Preserving Member Functions that May Modify the Pointset_Powerset
  //@{

  //! Adds to \p *this the disjunct \p ph.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and \p ph are dimension-incompatible.
  */
  void add_disjunct(const PSET& ph);

  //! Intersects \p *this with constraint \p c.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and constraint \p c are topology-incompatible
    or dimension-incompatible.
  */
  void add_constraint(const Constraint& c);

  /*! \brief
    Use the constraint \p c to refine \p *this.

    \param c
    The constraint to be used for refinement.

    \exception std::invalid_argument
    Thrown if \p *this and \p c are dimension-incompatible.
  */
  void refine_with_constraint(const Constraint& c);

  //! Intersects \p *this with the constraints in \p cs.
  /*!
    \param cs
    The constraints to intersect with.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are topology-incompatible or
    dimension-incompatible.
  */
  void add_constraints(const Constraint_System& cs);

  /*! \brief
    Use the constraints in \p cs to refine \p *this.

    \param  cs
     The constraints to be used for refinement.

     \exception std::invalid_argument
     Thrown if \p *this and \p cs are dimension-incompatible.
  */
  void refine_with_constraints(const Constraint_System& cs);

  //! Intersects \p *this with congruence \p cg.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and congruence \p cg are topology-incompatible
    or dimension-incompatible.
  */
  void add_congruence(const Congruence& cg);

  /*! \brief
    Use the congruence \p cg to refine \p *this.

    \param cg
    The congruence to be used for refinement.

    \exception std::invalid_argument
    Thrown if \p *this and \p cg are dimension-incompatible.
  */
  void refine_with_congruence(const Congruence& cg);

  //! Intersects \p *this with the congruences in \p cgs.
  /*!
    \param cgs
    The congruences to intersect with.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are topology-incompatible or
    dimension-incompatible.
  */
  void add_congruences(const Congruence_System& cgs);

  /*! \brief
    Use the congruences in \p cgs to refine \p *this.

    \param  cgs
    The congruences to be used for refinement.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible.
  */
  void refine_with_congruences(const Congruence_System& cgs);

  /*! \brief
    Computes the \ref Cylindrification "cylindrification" of \p *this with
    respect to space dimension \p var, assigning the result to \p *this.

    \param var
    The space dimension that will be unconstrained.

    \exception std::invalid_argument
    Thrown if \p var is not a space dimension of \p *this.
  */
  void unconstrain(Variable var);

  /*! \brief
    Computes the \ref Cylindrification "cylindrification" of \p *this with
    respect to the set of space dimensions \p vars,
    assigning the result to \p *this.

    \param vars
    The set of space dimension that will be unconstrained.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with one of the
    Variable objects contained in \p vars.
  */
  void unconstrain(const Variables_Set& vars);

  /*! \brief
    Possibly tightens \p *this by dropping some points with non-integer
    coordinates.

    \param complexity
    The maximal complexity of any algorithms used.

    \note
    Currently there is no optimality guarantee, not even if
    \p complexity is <CODE>ANY_COMPLEXITY</CODE>.
  */
  void drop_some_non_integer_points(Complexity_Class complexity
                                    = ANY_COMPLEXITY);

  /*! \brief
    Possibly tightens \p *this by dropping some points with non-integer
    coordinates for the space dimensions corresponding to \p vars.

    \param vars
    Points with non-integer coordinates for these variables/space-dimensions
    can be discarded.

    \param complexity
    The maximal complexity of any algorithms used.

    \note
    Currently there is no optimality guarantee, not even if
    \p complexity is <CODE>ANY_COMPLEXITY</CODE>.
  */
  void drop_some_non_integer_points(const Variables_Set& vars,
                                    Complexity_Class complexity
                                    = ANY_COMPLEXITY);

  //! Assigns to \p *this its topological closure.
  void topological_closure_assign();

  //! Assigns to \p *this the intersection of \p *this and \p y.
  /*!
    The result is obtained by intersecting each disjunct in \p *this
    with each disjunct in \p y and collecting all these intersections.
  */
  void intersection_assign(const Pointset_Powerset& y);

  /*! \brief
    Assigns to \p *this an (a smallest)
    over-approximation as a powerset of the disjunct domain of the
    set-theoretical difference of \p *this and \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void difference_assign(const Pointset_Powerset& y);

  /*! \brief
    Assigns to \p *this a \ref Powerset_Meet_Preserving_Simplification
    "meet-preserving simplification" of \p *this with respect to \p y.
    If \c false is returned, then the intersection is empty.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are topology-incompatible or
    dimension-incompatible.
  */
  bool simplify_using_context_assign(const Pointset_Powerset& y);

  /*! \brief
    Assigns to \p *this the
    \ref Single_Update_Affine_Functions "affine image"
    of \p *this under the function mapping variable \p var to the
    affine expression specified by \p expr and \p denominator.

    \param var
    The variable to which the affine expression is assigned;

    \param expr
    The numerator of the affine expression;

    \param denominator
    The denominator of the affine expression (optional argument with
    default value 1).

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of
    \p *this.
  */
  void affine_image(Variable var,
		    const Linear_Expression& expr,
		    Coefficient_traits::const_reference denominator
		      = Coefficient_one());

  /*! \brief
    Assigns to \p *this the
    \ref Single_Update_Affine_Functions "affine preimage"
    of \p *this under the function mapping variable \p var to the
    affine expression specified by \p expr and \p denominator.

    \param var
    The variable to which the affine expression is assigned;

    \param expr
    The numerator of the affine expression;

    \param denominator
    The denominator of the affine expression (optional argument with
    default value 1).

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of
    \p *this.
  */
  void affine_preimage(Variable var,
		    const Linear_Expression& expr,
		    Coefficient_traits::const_reference denominator
		      = Coefficient_one());

  /*! \brief
    Assigns to \p *this the image of \p *this with respect to the
    \ref Generalized_Affine_Relations "generalized affine relation"
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

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of \p *this
    or if \p *this is a C_Polyhedron and \p relsym is a strict
    relation symbol.
  */
  void generalized_affine_image(Variable var,
				Relation_Symbol relsym,
				const Linear_Expression& expr,
				Coefficient_traits::const_reference denominator
				  = Coefficient_one());

  /*! \brief
    Assigns to \p *this the preimage of \p *this with respect to the
    \ref Generalized_Affine_Relations "generalized affine relation"
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

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of \p *this
    or if \p *this is a C_Polyhedron and \p relsym is a strict
    relation symbol.
  */
  void
  generalized_affine_preimage(Variable var,
			      Relation_Symbol relsym,
			      const Linear_Expression& expr,
			      Coefficient_traits::const_reference denominator
			      = Coefficient_one());

  /*! \brief
    Assigns to \p *this the image of \p *this with respect to the
    \ref Generalized_Affine_Relations "generalized affine relation"
    \f$\mathrm{lhs}' \relsym \mathrm{rhs}\f$, where
    \f$\mathord{\relsym}\f$ is the relation symbol encoded by \p relsym.

    \param lhs
    The left hand side affine expression;

    \param relsym
    The relation symbol;

    \param rhs
    The right hand side affine expression.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p lhs or \p rhs
    or if \p *this is a C_Polyhedron and \p relsym is a strict
    relation symbol.
  */
  void generalized_affine_image(const Linear_Expression& lhs,
				Relation_Symbol relsym,
				const Linear_Expression& rhs);

  /*! \brief
    Assigns to \p *this the preimage of \p *this with respect to the
    \ref Generalized_Affine_Relations "generalized affine relation"
    \f$\mathrm{lhs}' \relsym \mathrm{rhs}\f$, where
    \f$\mathord{\relsym}\f$ is the relation symbol encoded by \p relsym.

    \param lhs
    The left hand side affine expression;

    \param relsym
    The relation symbol;

    \param rhs
    The right hand side affine expression.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p lhs or \p rhs
    or if \p *this is a C_Polyhedron and \p relsym is a strict
    relation symbol.
  */
  void generalized_affine_preimage(const Linear_Expression& lhs,
				   Relation_Symbol relsym,
				   const Linear_Expression& rhs);

  /*!
    \brief
    Assigns to \p *this the image of \p *this with respect to the
    \ref Single_Update_Bounded_Affine_Relations "bounded affine relation"
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

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p lb_expr (resp., \p ub_expr)
    and \p *this are dimension-incompatible or if \p var is not a space
    dimension of \p *this.
  */
  void bounded_affine_image(Variable var,
			    const Linear_Expression& lb_expr,
			    const Linear_Expression& ub_expr,
			    Coefficient_traits::const_reference denominator
			    = Coefficient_one());

  /*!
    \brief
    Assigns to \p *this the preimage of \p *this with respect to the
    \ref Single_Update_Bounded_Affine_Relations "bounded affine relation"
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

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p lb_expr (resp., \p ub_expr)
    and \p *this are dimension-incompatible or if \p var is not a space
    dimension of \p *this.
  */
  void bounded_affine_preimage(Variable var,
			       const Linear_Expression& lb_expr,
			       const Linear_Expression& ub_expr,
			       Coefficient_traits::const_reference denominator
			       = Coefficient_one());

  /*! \brief
    Assigns to \p *this the result of computing the
    \ref Time_Elapse_Operator "time-elapse" between \p *this and \p y.

    The result is obtained by computing the pairwise
    \ref Time_Elapse_Operator "time elapse" of each disjunct
    in \p *this with each disjunct in \p y.
  */
  void time_elapse_assign(const Pointset_Powerset& y);

  /*! \brief
    \ref Wrapping_Operator "Wraps" the specified dimensions of the
    vector space.

    \param vars
    The set of Variable objects corresponding to the space dimensions
    to be wrapped.

    \param w
    The width of the bounded integer type corresponding to
    all the dimensions to be wrapped.

    \param r
    The representation of the bounded integer type corresponding to
    all the dimensions to be wrapped.

    \param o
    The overflow behavior of the bounded integer type corresponding to
    all the dimensions to be wrapped.

    \param cs_p
    Possibly null pointer to a constraint system whose variables
    are contained in \p vars.  If <CODE>*cs_p</CODE> depends on
    variables not in \p vars, the behavior is undefined.
    When non-null, the pointed-to constraint system is assumed to
    represent the conditional or looping construct guard with respect
    to which wrapping is performed.  Since wrapping requires the
    computation of upper bounds and due to non-distributivity of
    constraint refinement over upper bounds, passing a constraint
    system in this way can be more precise than refining the result of
    the wrapping operation with the constraints in <CODE>*cs_p</CODE>.

    \param complexity_threshold
    A precision parameter of the \ref Wrapping_Operator "wrapping operator":
    higher values result in possibly improved precision.

    \param wrap_individually
    <CODE>true</CODE> if the dimensions should be wrapped individually
    (something that results in much greater efficiency to the detriment of
    precision).

    \exception std::invalid_argument
    Thrown if <CODE>*cs_p</CODE> is dimension-incompatible with
    \p vars, or if \p *this is dimension-incompatible \p vars or with
    <CODE>*cs_p</CODE>.
  */
  void wrap_assign(const Variables_Set& vars,
                   Bounded_Integer_Type_Width w,
                   Bounded_Integer_Type_Representation r,
                   Bounded_Integer_Type_Overflow o,
                   const Constraint_System* cs_p = 0,
                   unsigned complexity_threshold = 16,
                   bool wrap_individually = true);

  /*! \brief
    Assign to \p *this the result of (recursively) merging together
    the pairs of disjuncts whose upper-bound is the same as their
    set-theoretical union.

    On exit, for all the pairs \f$\cP\f$, \f$\cQ\f$ of different disjuncts
    in \p *this, we have \f$\cP \uplus \cQ \neq \cP \union \cQ\f$.
  */
  void pairwise_reduce();

  /*! \brief
    Assigns to \p *this the result of applying the
    \ref pps_bgp99_extrapolation "BGP99 extrapolation operator"
    to \p *this and \p y, using the widening function \p widen_fun
    and the cardinality threshold \p max_disjuncts.

    \param y
    A powerset that <EM>must</EM> definitely entail \p *this;

    \param widen_fun
    The widening function to be used on polyhedra objects. It is obtained
    from the corresponding widening method by using the helper function
    Parma_Polyhedra_Library::widen_fun_ref. Legal values are, e.g.,
    <CODE>widen_fun_ref(&Polyhedron::H79_widening_assign)</CODE> and
    <CODE>widen_fun_ref(&Polyhedron::limited_H79_extrapolation_assign, cs)</CODE>;

    \param max_disjuncts
    The maximum number of disjuncts occurring in the powerset \p *this
    <EM>before</EM> starting the computation. If this number is exceeded,
    some of the disjuncts in \p *this are collapsed (i.e., joined together).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.

    For a description of the extrapolation operator,
    see \ref BGP99 "[BGP99]" and \ref BHZ03b "[BHZ03b]".
  */
  template <typename Widening>
  void BGP99_extrapolation_assign(const Pointset_Powerset& y,
				  Widening widen_fun,
				  unsigned max_disjuncts);

  /*! \brief
    Assigns to \p *this the result of computing the
    \ref pps_certificate_widening "BHZ03-widening"
    between \p *this and \p y, using the widening function \p widen_fun
    certified by the convergence certificate \p Cert.

    \param y
    The finite powerset computed in the previous iteration step.
    It <EM>must</EM> definitely entail \p *this;

    \param widen_fun
    The widening function to be used on disjuncts.
    It is obtained from the corresponding widening method by using
    the helper function widen_fun_ref. Legal values are, e.g.,
    <CODE>widen_fun_ref(&Polyhedron::H79_widening_assign)</CODE> and
    <CODE>widen_fun_ref(&Polyhedron::limited_H79_extrapolation_assign, cs)</CODE>.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.

    \warning
    In order to obtain a proper widening operator, the template parameter
    \p Cert should be a finite convergence certificate for the base-level
    widening function \p widen_fun; otherwise, an extrapolation operator is
    obtained.
    For a description of the methods that should be provided
    by \p Cert, see BHRZ03_Certificate or H79_Certificate.
  */
  template <typename Cert, typename Widening>
  void BHZ03_widening_assign(const Pointset_Powerset& y, Widening widen_fun);

  //@} // Space Dimension Preserving Member Functions that May Modify [...]

  //! \name Member Functions that May Modify the Dimension of the Vector Space
  //@{

  /*! \brief
    The assignment operator
    (\p *this and \p y can be dimension-incompatible).
  */
  Pointset_Powerset& operator=(const Pointset_Powerset& y);

  /*! \brief
    Conversion assignment: the type <CODE>QH</CODE> of the disjuncts
    in the source powerset is different from <CODE>PSET</CODE>
    (\p *this and \p y can be dimension-incompatible).
  */
  template <typename QH>
  Pointset_Powerset& operator=(const Pointset_Powerset<QH>& y);

  //! Swaps \p *this with \p y.
  void m_swap(Pointset_Powerset& y);

  /*! \brief
    Adds \p m new dimensions to the vector space containing \p *this
    and embeds each disjunct in \p *this in the new space.
  */
  void add_space_dimensions_and_embed(dimension_type m);

  /*! \brief
    Adds \p m new dimensions to the vector space containing \p *this
    without embedding the disjuncts in \p *this in the new space.
  */
  void add_space_dimensions_and_project(dimension_type m);

  //! Assigns to \p *this the concatenation of \p *this and \p y.
  /*!
    The result is obtained by computing the pairwise
    \ref Concatenating_Polyhedra "concatenation" of each disjunct
    in \p *this with each disjunct in \p y.
  */
  void concatenate_assign(const Pointset_Powerset& y);

  //! Removes all the specified space dimensions.
  /*!
    \param vars
    The set of Variable objects corresponding to the space dimensions
    to be removed.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with one of the
    Variable objects contained in \p vars.
  */
  void remove_space_dimensions(const Variables_Set& vars);

  /*! \brief
    Removes the higher space dimensions so that the resulting space
    will have dimension \p new_dimension.

    \exception std::invalid_argument
    Thrown if \p new_dimensions is greater than the space dimension
    of \p *this.
  */
  void remove_higher_space_dimensions(dimension_type new_dimension);

  /*! \brief
    Remaps the dimensions of the vector space according to
    a partial function.

    See also Polyhedron::map_space_dimensions.
  */
  template <typename Partial_Function>
  void map_space_dimensions(const Partial_Function& pfunc);

  //! Creates \p m copies of the space dimension corresponding to \p var.
  /*!
    \param var
    The variable corresponding to the space dimension to be replicated;

    \param m
    The number of replicas to be created.

    \exception std::invalid_argument
    Thrown if \p var does not correspond to a dimension of the vector
    space.

    \exception std::length_error
    Thrown if adding \p m new space dimensions would cause the vector
    space to exceed dimension <CODE>max_space_dimension()</CODE>.

    If \p *this has space dimension \f$n\f$, with \f$n > 0\f$,
    and <CODE>var</CODE> has space dimension \f$k \leq n\f$,
    then the \f$k\f$-th space dimension is
    \ref Expanding_One_Dimension_of_the_Vector_Space_to_Multiple_Dimensions
    "expanded" to \p m new space dimensions
    \f$n\f$, \f$n+1\f$, \f$\dots\f$, \f$n+m-1\f$.
  */
  void expand_space_dimension(Variable var, dimension_type m);

  //! Folds the space dimensions in \p vars into \p dest.
  /*!
    \param vars
    The set of Variable objects corresponding to the space dimensions
    to be folded;

    \param dest
    The variable corresponding to the space dimension that is the
    destination of the folding operation.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p dest or with
    one of the Variable objects contained in \p vars.  Also
    thrown if \p dest is contained in \p vars.

    If \p *this has space dimension \f$n\f$, with \f$n > 0\f$,
    <CODE>dest</CODE> has space dimension \f$k \leq n\f$,
    \p vars is a set of variables whose maximum space dimension
    is also less than or equal to \f$n\f$, and \p dest is not a member
    of \p vars, then the space dimensions corresponding to
    variables in \p vars are
    \ref Folding_Multiple_Dimensions_of_the_Vector_Space_into_One_Dimension
    "folded" into the \f$k\f$-th space dimension.
  */
  void fold_space_dimensions(const Variables_Set& vars, Variable dest);

  //@} // Member Functions that May Modify the Dimension of the Vector Space

public:
  typedef typename Base::size_type size_type;
  typedef typename Base::value_type value_type;
  typedef typename Base::iterator iterator;
  typedef typename Base::const_iterator const_iterator;
  typedef typename Base::reverse_iterator reverse_iterator;
  typedef typename Base::const_reverse_iterator const_reverse_iterator;

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

private:
  typedef typename Base::Sequence Sequence;
  typedef typename Base::Sequence_iterator Sequence_iterator;
  typedef typename Base::Sequence_const_iterator Sequence_const_iterator;

  //! The number of dimensions of the enclosing vector space.
  dimension_type space_dim;

  /*! \brief
    Assigns to \p dest a \ref Powerset_Meet_Preserving_Simplification
    "powerset meet-preserving enlargement" of itself with respect to
    \p *this.  If \c false is returned, then the intersection is empty.

    \note
    It is assumed that \p *this and \p dest are topology-compatible
    and dimension-compatible.
  */
  bool intersection_preserving_enlarge_element(PSET& dest) const;

  /*! \brief
    Assigns to \p *this the result of applying the BGP99 heuristics
    to \p *this and \p y, using the widening function \p widen_fun.
  */
  template <typename Widening>
  void BGP99_heuristics_assign(const Pointset_Powerset& y, Widening widen_fun);

  //! Records in \p cert_ms the certificates for this set of disjuncts.
  template <typename Cert>
  void collect_certificates(std::map<Cert, size_type,
			             typename Cert::Compare>& cert_ms) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if the current set of disjuncts
    is stabilizing with respect to the multiset of certificates \p y_cert_ms.
  */
  template <typename Cert>
  bool is_cert_multiset_stabilizing(const std::map<Cert, size_type,
                                                   typename Cert::Compare>&
				    y_cert_ms) const;

  // FIXME: here it should be enough to befriend the template constructor
  // template <typename QH>
  // Pointset_Powerset(const Pointset_Powerset<QH>&),
  // but, apparently, this cannot be done.
  friend class Pointset_Powerset<NNC_Polyhedron>;
};

namespace Parma_Polyhedra_Library {

//! Swaps \p x with \p y.
/*! \relates Pointset_Powerset */
template <typename PSET>
void swap(Pointset_Powerset<PSET>& x, Pointset_Powerset<PSET>& y);

//! Partitions \p q with respect to \p p.
/*! \relates Pointset_Powerset
  Let \p p and \p q be two polyhedra.
  The function returns an object <CODE>r</CODE> of type
  <CODE>std::pair\<PSET, Pointset_Powerset\<NNC_Polyhedron\> \></CODE>
  such that
  - <CODE>r.first</CODE> is the intersection of \p p and \p q;
  - <CODE>r.second</CODE> has the property that all its elements are
    pairwise disjoint and disjoint from \p p;
  - the set-theoretical union of <CODE>r.first</CODE> with all the
    elements of <CODE>r.second</CODE> gives \p q (i.e., <CODE>r</CODE>
    is the representation of a partition of \p q).

  \if Include_Implementation_Details

  See
  <A HREF="http://bugseng.com/products/ppl/Documentation/bibliography#Srivastava93">
  this paper</A> for more information about the implementation.
  \endif
*/
template <typename PSET>
std::pair<PSET, Pointset_Powerset<NNC_Polyhedron> >
linear_partition(const PSET& p, const PSET& q);

/*! \brief
  Returns <CODE>true</CODE> if and only if the union of
  the NNC polyhedra in \p ps contains the NNC polyhedron \p ph.

  \relates Pointset_Powerset
*/
bool
check_containment(const NNC_Polyhedron& ph,
		  const Pointset_Powerset<NNC_Polyhedron>& ps);


/*! \brief
  Partitions the grid \p q with respect to grid \p p if and only if
  such a partition is finite.

  \relates Parma_Polyhedra_Library::Pointset_Powerset
  Let \p p and \p q be two grids.
  The function returns an object <CODE>r</CODE> of type
  <CODE>std::pair\<PSET, Pointset_Powerset\<Grid\> \></CODE>
  such that
  - <CODE>r.first</CODE> is the intersection of \p p and \p q;
  - If there is a finite partition of \p q with respect to \p p
    the Boolean <CODE>finite_partition</CODE> is set to true and
    <CODE>r.second</CODE> has the property that all its elements are
    pairwise disjoint and disjoint from \p p and the set-theoretical
    union of <CODE>r.first</CODE> with all the elements of
    <CODE>r.second</CODE> gives \p q (i.e., <CODE>r</CODE>
    is the representation of a partition of \p q).
  - Otherwise the Boolean <CODE>finite_partition</CODE> is set to false
    and the singleton set that contains \p q is stored in
    <CODE>r.second</CODE>r.
*/
std::pair<Grid, Pointset_Powerset<Grid> >
approximate_partition(const Grid& p, const Grid& q, bool& finite_partition);

/*! \brief
  Returns <CODE>true</CODE> if and only if the union of
  the grids \p ps contains the grid \p g.

  \relates Pointset_Powerset
*/
bool
check_containment(const Grid& ph,
		  const Pointset_Powerset<Grid>& ps);

/*! \brief
  Returns <CODE>true</CODE> if and only if the union of
  the objects in \p ps contains \p ph.

  \relates Pointset_Powerset
  \note
  It is assumed that the template parameter PSET can be converted
  without precision loss into an NNC_Polyhedron; otherwise,
  an incorrect result might be obtained.
*/
template <typename PSET>
bool
check_containment(const PSET& ph, const Pointset_Powerset<PSET>& ps);

// CHECKME: according to the Intel compiler, the declaration of the
// following specialization (of the class template parameter) should come
// before the declaration of the corresponding full specialization
// (where the member template parameter is specialized too).
template <>
template <typename QH>
Pointset_Powerset<NNC_Polyhedron>
::Pointset_Powerset(const Pointset_Powerset<QH>& y,
                    Complexity_Class);

// Non-inline full specializations should be declared here
// so as to inhibit multiple instantiations of the generic template.
template <>
template <>
Pointset_Powerset<NNC_Polyhedron>
::Pointset_Powerset(const Pointset_Powerset<C_Polyhedron>& y,
                    Complexity_Class);

template <>
template <>
Pointset_Powerset<NNC_Polyhedron>
::Pointset_Powerset(const Pointset_Powerset<Grid>& y,
                    Complexity_Class);

template <>
template <>
Pointset_Powerset<C_Polyhedron>
::Pointset_Powerset(const Pointset_Powerset<NNC_Polyhedron>& y,
                    Complexity_Class);

template <>
void
Pointset_Powerset<NNC_Polyhedron>
::difference_assign(const Pointset_Powerset& y);

template <>
void
Pointset_Powerset<Grid>
::difference_assign(const Pointset_Powerset& y);

template <>
bool
Pointset_Powerset<NNC_Polyhedron>
::geometrically_covers(const Pointset_Powerset& y) const;

template <>
bool
Pointset_Powerset<Grid>
::geometrically_covers(const Pointset_Powerset& y) const;

} // namespace Parma_Polyhedra_Library

#include "Pointset_Powerset.inlines.hh"
#include "Pointset_Powerset.templates.hh"

#endif // !defined(PPL_Pointset_Powerset_defs_hh)
