/* Box class declaration.
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

#ifndef PPL_Box_defs_hh
#define PPL_Box_defs_hh 1

#include "Box_types.hh"
#include "globals_types.hh"
#include "Coefficient_defs.hh"
#include "Variable_types.hh"
#include "Variables_Set_types.hh"
#include "Linear_Expression_types.hh"
#include "Constraint_types.hh"
#include "Constraint_defs.hh"
#include "Constraint_System_types.hh"
#include "Generator_types.hh"
#include "Generator_System_types.hh"
#include "Congruence_types.hh"
#include "Congruence_System_types.hh"
#include "BD_Shape_types.hh"
#include "Octagonal_Shape_types.hh"
#include "Poly_Con_Relation_types.hh"
#include "Poly_Gen_Relation_types.hh"
#include "Polyhedron_types.hh"
#include "Grid_types.hh"
#include "Partially_Reduced_Product_types.hh"
#include "intervals_defs.hh"
#include "Interval_types.hh"
#include "Linear_Form_types.hh"
#include <vector>
#include <iosfwd>

namespace Parma_Polyhedra_Library {

struct Interval_Base;

//! Swaps \p x with \p y.
/*! \relates Box */
template <typename ITV>
void swap(Box<ITV>& x, Box<ITV>& y);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are the same box.
/*! \relates Box
  Note that \p x and \p y may be dimension-incompatible boxes:
  in this case, the value <CODE>false</CODE> is returned.
*/
template <typename ITV>
bool operator==(const Box<ITV>& x, const Box<ITV>& y);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are not the same box.
/*! \relates Box
  Note that \p x and \p y may be dimension-incompatible boxes:
  in this case, the value <CODE>true</CODE> is returned.
*/
template <typename ITV>
bool operator!=(const Box<ITV>& x, const Box<ITV>& y);

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Box */
template <typename ITV>
std::ostream& operator<<(std::ostream& s, const Box<ITV>& box);

} // namespace IO_Operators

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates Box
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.
*/
template <typename To, typename ITV>
bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                            const Box<ITV>& x,
                            const Box<ITV>& y,
                            Rounding_Dir dir);

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates Box
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.
*/
template <typename Temp, typename To, typename ITV>
bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                            const Box<ITV>& x,
                            const Box<ITV>& y,
                            Rounding_Dir dir);

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates Box
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
template <typename Temp, typename To, typename ITV>
bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                            const Box<ITV>& x,
                            const Box<ITV>& y,
                            Rounding_Dir dir,
                            Temp& tmp0,
                            Temp& tmp1,
                            Temp& tmp2);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates Box
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.
*/
template <typename To, typename ITV>
bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                          const Box<ITV>& x,
                          const Box<ITV>& y,
                          Rounding_Dir dir);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates Box
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.
*/
template <typename Temp, typename To, typename ITV>
bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                          const Box<ITV>& x,
                          const Box<ITV>& y,
                          Rounding_Dir dir);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates Box
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
template <typename Temp, typename To, typename ITV>
bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                          const Box<ITV>& x,
                          const Box<ITV>& y,
                          Rounding_Dir dir,
                          Temp& tmp0,
                          Temp& tmp1,
                          Temp& tmp2);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates Box
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.
*/
template <typename To, typename ITV>
bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                           const Box<ITV>& x,
                           const Box<ITV>& y,
                           Rounding_Dir dir);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates Box
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.
*/
template <typename Temp, typename To, typename ITV>
bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                           const Box<ITV>& x,
                           const Box<ITV>& y,
                           Rounding_Dir dir);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates Box
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
template <typename Temp, typename To, typename ITV>
bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                           const Box<ITV>& x,
                           const Box<ITV>& y,
                           Rounding_Dir dir,
                           Temp& tmp0,
                           Temp& tmp1,
                           Temp& tmp2);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Box
  Helper function for computing distances.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Specialization,
          typename Temp, typename To, typename ITV>
bool
l_m_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                    const Box<ITV>& x, const Box<ITV>& y,
                    Rounding_Dir dir,
                    Temp& tmp0, Temp& tmp1, Temp& tmp2);

} // namespace Parma_Polyhedra_Library

//! A not necessarily closed, iso-oriented hyperrectangle.
/*! \ingroup PPL_CXX_interface
  A Box object represents the smash product of \f$n\f$
  not necessarily closed and possibly unbounded intervals
  represented by objects of class \p ITV,
  where \f$n\f$ is the space dimension of the box.

  An <EM>interval constraint</EM> (resp., <EM>interval congruence</EM>)
  is a syntactic constraint (resp., congruence) that only mentions
  a single space dimension.

  The Box domain <EM>optimally supports</EM>:
    - tautological and inconsistent constraints and congruences;
    - the interval constraints that are optimally supported by
      the template argument class \c ITV;
    - the interval congruences that are optimally supported by
      the template argument class \c ITV.

  Depending on the method, using a constraint or congruence that is not
  optimally supported by the domain will either raise an exception or
  result in a (possibly non-optimal) upward approximation.

  The user interface for the Box domain is meant to be as similar
  as possible to the one developed for the polyhedron class C_Polyhedron.
*/
template <typename ITV>
class Parma_Polyhedra_Library::Box {
public:
  //! The type of intervals used to implement the box.
  typedef ITV interval_type;

  //! Returns the maximum space dimension that a Box can handle.
  static dimension_type max_space_dimension();

  /*! \brief
    Returns false indicating that this domain does not recycle constraints
  */
  static bool can_recycle_constraint_systems();

  /*! \brief
    Returns false indicating that this domain does not recycle congruences
  */
  static bool can_recycle_congruence_systems();

  //! \name Constructors, Assignment, Swap and Destructor
  //@{

  //! Builds a universe or empty box of the specified space dimension.
  /*!
    \param num_dimensions
    The number of dimensions of the vector space enclosing the box;

    \param kind
    Specifies whether the universe or the empty box has to be built.
  */
  explicit Box(dimension_type num_dimensions = 0,
               Degenerate_Element kind = UNIVERSE);

  //! Ordinary copy constructor.
  /*!
    The complexity argument is ignored.
  */
  Box(const Box& y,
      Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a conservative, upward approximation of \p y.
  /*!
    The complexity argument is ignored.
  */
  template <typename Other_ITV>
  explicit Box(const Box<Other_ITV>& y,
               Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a box from the system of constraints \p cs.
  /*!
    The box inherits the space dimension of \p cs.

    \param cs
    A system of constraints: constraints that are not
    \ref intervals "interval constraints"
    are ignored (even though they may have contributed
    to the space dimension).
  */
  explicit Box(const Constraint_System& cs);

  //! Builds a box recycling a system of constraints \p cs.
  /*!
    The box inherits the space dimension of \p cs.

    \param cs
    A system of constraints: constraints that are not
    \ref intervals "interval constraints"
    are ignored (even though they may have contributed
    to the space dimension).

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.
  */
  Box(const Constraint_System& cs, Recycle_Input dummy);

  //! Builds a box from the system of generators \p gs.
  /*!
    Builds the smallest box containing the polyhedron defined by \p gs.
    The box inherits the space dimension of \p gs.

    \exception std::invalid_argument
    Thrown if the system of generators is not empty but has no points.
  */
  explicit Box(const Generator_System& gs);

  //! Builds a box recycling the system of generators \p gs.
  /*!
    Builds the smallest box containing the polyhedron defined by \p gs.
    The box inherits the space dimension of \p gs.

    \param gs
    The generator system describing the polyhedron to be approximated.

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.

    \exception std::invalid_argument
    Thrown if the system of generators is not empty but has no points.
  */
  Box(const Generator_System& gs, Recycle_Input dummy);

  /*!
    Builds the smallest box containing the grid defined by a
    system of congruences \p cgs.
    The box inherits the space dimension of \p cgs.

    \param cgs
    A system of congruences: congruences that are not
    non-relational equality constraints are ignored
    (though they may have contributed to the space dimension).
  */
  explicit Box(const Congruence_System& cgs);

  /*!
    Builds the smallest box containing the grid defined by a
    system of congruences \p cgs, recycling \p cgs.
    The box inherits the space dimension of \p cgs.

    \param cgs
    A system of congruences: congruences that are not
    non-relational equality constraints are ignored
    (though they will contribute to the space dimension).

    \param dummy
    A dummy tag to syntactically differentiate this one
    from the other constructors.
  */
  Box(const Congruence_System& cgs, Recycle_Input dummy);

  //! Builds a box containing the BDS \p bds.
  /*!
    Builds the smallest box containing \p bds using a polynomial algorithm.
    The \p complexity argument is ignored.
  */
  template <typename T>
  explicit Box(const BD_Shape<T>& bds,
               Complexity_Class complexity = POLYNOMIAL_COMPLEXITY);

  //! Builds a box containing the octagonal shape \p oct.
  /*!
    Builds the smallest box containing \p oct using a polynomial algorithm.
    The \p complexity argument is ignored.
  */
  template <typename T>
  explicit Box(const Octagonal_Shape<T>& oct,
               Complexity_Class complexity = POLYNOMIAL_COMPLEXITY);

  //! Builds a box containing the polyhedron \p ph.
  /*!
    Builds a box containing \p ph using algorithms whose complexity
    does not exceed the one specified by \p complexity.  If
    \p complexity is \p ANY_COMPLEXITY, then the built box is the
    smallest one containing \p ph.
  */
  explicit Box(const Polyhedron& ph,
               Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a box containing the grid \p gr.
  /*!
    Builds the smallest box containing \p gr using a polynomial algorithm.
    The \p complexity argument is ignored.
  */
  explicit Box(const Grid& gr,
               Complexity_Class complexity = POLYNOMIAL_COMPLEXITY);

  //! Builds a box containing the partially reduced product \p dp.
  /*!
    Builds a box containing \p ph using algorithms whose complexity
    does not exceed the one specified by \p complexity.
  */
  template <typename D1, typename D2, typename R>
  explicit Box(const Partially_Reduced_Product<D1, D2, R>& dp,
               Complexity_Class complexity = ANY_COMPLEXITY);

  /*! \brief
    The assignment operator
    (\p *this and \p y can be dimension-incompatible).
  */
  Box& operator=(const Box& y);

  /*! \brief
    Swaps \p *this with \p y
    (\p *this and \p y can be dimension-incompatible).
  */
  void m_swap(Box& y);

  //@} Constructors, Assignment, Swap and Destructor

  //! \name Member Functions that Do Not Modify the Box
  //@{

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  /*! \brief
    Returns \f$0\f$, if \p *this is empty; otherwise, returns the
    \ref Affine_Independence_and_Affine_Dimension "affine dimension"
    of \p *this.
  */
  dimension_type affine_dimension() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is an empty box.
  bool is_empty() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a universe box.
  bool is_universe() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this
    is a topologically closed subset of the vector space.
  */
  bool is_topologically_closed() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is discrete.
  bool is_discrete() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a bounded box.
  bool is_bounded() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this
    contains at least one integer point.
  */
  bool contains_integer_point() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p var is constrained in
    \p *this.

    \exception std::invalid_argument
    Thrown if \p var is not a space dimension of \p *this.
  */
  bool constrains(Variable var) const;

  //! Returns the relations holding between \p *this and the constraint \p c.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and constraint \p c are dimension-incompatible.
  */
  Poly_Con_Relation relation_with(const Constraint& c) const;

  //! Returns the relations holding between \p *this and the congruence \p cg.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and constraint \p cg are dimension-incompatible.
  */
  Poly_Con_Relation relation_with(const Congruence& cg) const;

  //! Returns the relations holding between \p *this and the generator \p g.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and generator \p g are dimension-incompatible.
  */
  Poly_Gen_Relation relation_with(const Generator& g) const;

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
    Returns <CODE>true</CODE> if and only if there exist a
    unique value \p val such that \p *this
    saturates the equality <CODE>expr = val</CODE>.

    \param expr
    The linear expression for which the frequency is needed;

    \param freq_n
    If <CODE>true</CODE> is returned, the value is set to \f$0\f$;
    Present for interface compatibility with class Grid, where
    the \ref Grid_Frequency "frequency" can have a non-zero value;

    \param freq_d
    If <CODE>true</CODE> is returned, the value is set to \f$1\f$;

    \param val_n
    The numerator of \p val;

    \param val_d
    The denominator of \p val;

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If <CODE>false</CODE> is returned, then \p freq_n, \p freq_d,
    \p val_n and \p val_d are left untouched.
  */
  bool frequency(const Linear_Expression& expr,
                 Coefficient& freq_n, Coefficient& freq_d,
                 Coefficient& val_n, Coefficient& val_d) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this contains \p y.

    \exception std::invalid_argument
    Thrown if \p x and \p y are dimension-incompatible.
  */
  bool contains(const Box& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this strictly contains \p y.

    \exception std::invalid_argument
    Thrown if \p x and \p y are dimension-incompatible.
  */
  bool strictly_contains(const Box& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this and \p y are disjoint.

    \exception std::invalid_argument
    Thrown if \p x and \p y are dimension-incompatible.
  */
  bool is_disjoint_from(const Box& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this satisfies
    all its invariants.
  */
  bool OK() const;

  //@} Member Functions that Do Not Modify the Box

  //! \name Space-Dimension Preserving Member Functions that May Modify the Box
  //@{

  /*! \brief
    Adds a copy of constraint \p c to the system of constraints
    defining \p *this.

    \param c
    The constraint to be added.

    \exception std::invalid_argument
    Thrown if \p *this and constraint \p c are dimension-incompatible,
    or \p c is not optimally supported by the Box domain.
  */
  void add_constraint(const Constraint& c);

  /*! \brief
    Adds the constraints in \p cs to the system of constraints
    defining \p *this.

    \param  cs
    The constraints to be added.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible,
    or \p cs contains a constraint which is not optimally supported
    by the box domain.
  */
  void add_constraints(const Constraint_System& cs);

  /*! \brief
    Adds the constraints in \p cs to the system of constraints
    defining \p *this.

    \param  cs
    The constraints to be added. They may be recycled.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible,
    or \p cs contains a constraint which is not optimally supported
    by the box domain.

    \warning
    The only assumption that can be made on \p cs upon successful or
    exceptional return is that it can be safely destroyed.
  */
  void add_recycled_constraints(Constraint_System& cs);

  /*! \brief
    Adds to \p *this a constraint equivalent to the congruence \p cg.

    \param cg
    The congruence to be added.

    \exception std::invalid_argument
    Thrown if \p *this and congruence \p cg are dimension-incompatible,
    or \p cg is not optimally supported by the box domain.
  */
  void add_congruence(const Congruence& cg);

  /*! \brief
    Adds to \p *this constraints equivalent to the congruences in \p cgs.

    \param cgs
    The congruences to be added.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible,
    or \p cgs contains a congruence which is not optimally supported
    by the box domain.
  */
  void add_congruences(const Congruence_System& cgs);

  /*! \brief
    Adds to \p *this constraints equivalent to the congruences in \p cgs.

    \param cgs
    The congruence system to be added to \p *this.  The congruences in
    \p cgs may be recycled.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible,
    or \p cgs contains a congruence which is not optimally supported
    by the box domain.

    \warning
    The only assumption that can be made on \p cgs upon successful or
    exceptional return is that it can be safely destroyed.
  */
  void add_recycled_congruences(Congruence_System& cgs);

  /*! \brief
    Use the constraint \p c to refine \p *this.

    \param c
    The constraint to be used for refinement.

    \exception std::invalid_argument
    Thrown if \p *this and \p c are dimension-incompatible.
  */
  void refine_with_constraint(const Constraint& c);

  /*! \brief
    Use the constraints in \p cs to refine \p *this.

    \param  cs
    The constraints to be used for refinement.
    To avoid termination problems, each constraint in \p cs
    will be used for a single refinement step.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible.

    \note
    The user is warned that the accuracy of this refinement operator
    depends on the order of evaluation of the constraints in \p cs,
    which is in general unpredictable. If a fine control on such an
    order is needed, the user should consider calling the method
    <code>refine_with_constraint(const Constraint& c)</code> inside
    an appropriate looping construct.
  */
  void refine_with_constraints(const Constraint_System& cs);

  /*! \brief
    Use the congruence \p cg to refine \p *this.

    \param cg
    The congruence to be used for refinement.

    \exception std::invalid_argument
    Thrown if \p *this and \p cg are dimension-incompatible.
  */
  void refine_with_congruence(const Congruence& cg);

  /*! \brief
    Use the congruences in \p cgs to refine \p *this.

    \param  cgs
    The congruences to be used for refinement.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible.
  */
  void refine_with_congruences(const Congruence_System& cgs);

  /*! \brief
    Use the constraint \p c for constraint propagation on \p *this.

    \param c
    The constraint to be used for constraint propagation.

    \exception std::invalid_argument
    Thrown if \p *this and \p c are dimension-incompatible.
  */
  void propagate_constraint(const Constraint& c);

  /*! \brief
    Use the constraints in \p cs for constraint propagation on \p *this.

    \param cs
    The constraints to be used for constraint propagation.

    \param max_iterations
    The maximum number of propagation steps for each constraint in
    \p cs.  If zero (the default), the number of propagation steps
    will be unbounded, possibly resulting in an infinite loop.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible.

    \warning
    This method may lead to non-termination if \p max_iterations is 0.
  */
  void propagate_constraints(const Constraint_System& cs,
                             dimension_type max_iterations = 0);

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

  //! Assigns to \p *this the intersection of \p *this and \p y.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void intersection_assign(const Box& y);

  /*! \brief
    Assigns to \p *this the smallest box containing the union
    of \p *this and \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void upper_bound_assign(const Box& y);

  /*! \brief
    If the upper bound of \p *this and \p y is exact, it is assigned
    to \p *this and <CODE>true</CODE> is returned,
    otherwise <CODE>false</CODE> is returned.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool upper_bound_assign_if_exact(const Box& y);

  /*! \brief
    Assigns to \p *this the difference of \p *this and \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void difference_assign(const Box& y);

  /*! \brief
    Assigns to \p *this a \ref Meet_Preserving_Simplification
    "meet-preserving simplification" of \p *this with respect to \p y.
    If \c false is returned, then the intersection is empty.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool simplify_using_context_assign(const Box& y);

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

  // FIXME: To be completed.
  /*! \brief
    Assigns to \p *this the \ref affine_form_relation "affine form image"
    of \p *this under the function mapping variable \p var into the
    affine expression(s) specified by \p lf.

    \param var
    The variable to which the affine expression is assigned.

    \param lf
    The linear form on intervals with floating point boundaries that
    defines the affine expression(s). ALL of its coefficients MUST be bounded.

    \exception std::invalid_argument
    Thrown if \p lf and \p *this are dimension-incompatible or if \p var
    is not a dimension of \p *this.

    This function is used in abstract interpretation to model an assignment
    of a value that is correctly overapproximated by \p lf to the
    floating point variable represented by \p var.
  */
  void affine_form_image(Variable var,
                         const Linear_Form<ITV>& lf);

  /*! \brief
    Assigns to \p *this the
    \ref Single_Update_Affine_Functions "affine preimage"
    of \p *this under the function mapping variable \p var to the
    affine expression specified by \p expr and \p denominator.

    \param var
    The variable to which the affine expression is substituted;

    \param expr
    The numerator of the affine expression;

    \param denominator
    The denominator of the affine expression (optional argument with
    default value 1).

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this are
    dimension-incompatible or if \p var is not a space dimension of \p *this.
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
    dimension-incompatible or if \p var is not a space dimension of \p *this.
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
    dimension-incompatible or if \p var is not a space dimension of \p *this.
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
    Thrown if \p *this is dimension-incompatible with \p lhs or \p rhs.
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
    Thrown if \p *this is dimension-incompatible with \p lhs or \p rhs.
  */
  void generalized_affine_preimage(const Linear_Expression& lhs,
                                   Relation_Symbol relsym,
                                   const Linear_Expression& rhs);

  /*! \brief
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

  /*! \brief
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

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void time_elapse_assign(const Box& y);

  //! Assigns to \p *this its topological closure.
  void topological_closure_assign();

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
    Possibly null pointer to a constraint system.  When non-null,
    the pointed-to constraint system is assumed to represent the
    conditional or looping construct guard with respect to which
    wrapping is performed.  Since wrapping requires the computation
    of upper bounds and due to non-distributivity of constraint
    refinement over upper bounds, passing a constraint system in this
    way can be more precise than refining the result of the wrapping
    operation with the constraints in <CODE>*cs_p</CODE>.

    \param complexity_threshold
    A precision parameter which is ignored for the Box domain.

    \param wrap_individually
    A precision parameter which is ignored for the Box domain.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with one of the
    Variable objects contained in \p vars or with <CODE>*cs_p</CODE>.
  */
  void wrap_assign(const Variables_Set& vars,
                   Bounded_Integer_Type_Width w,
                   Bounded_Integer_Type_Representation r,
                   Bounded_Integer_Type_Overflow o,
                   const Constraint_System* cs_p = 0,
                   unsigned complexity_threshold = 16,
                   bool wrap_individually = true);

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

  /*! \brief
    Assigns to \p *this the result of computing the
    \ref CC76_extrapolation "CC76-widening" between \p *this and \p y.

    \param y
    A box that <EM>must</EM> be contained in \p *this.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  template <typename T>
  typename Enable_If<Is_Same<T, Box>::value
                     && Is_Same_Or_Derived<Interval_Base, ITV>::value,
                     void>::type
  CC76_widening_assign(const T& y, unsigned* tp = 0);

  /*! \brief
    Assigns to \p *this the result of computing the
    \ref CC76_extrapolation "CC76-widening" between \p *this and \p y.

    \param y
    A box that <EM>must</EM> be contained in \p *this.

    \param first
    An iterator that points to the first stop-point.

    \param last
    An iterator that points one past the last stop-point.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  template <typename T, typename Iterator>
  typename Enable_If<Is_Same<T, Box>::value
                     && Is_Same_Or_Derived<Interval_Base, ITV>::value,
                     void>::type
  CC76_widening_assign(const T& y,
                       Iterator first, Iterator last);

  //! Same as CC76_widening_assign(y, tp).
  void widening_assign(const Box& y, unsigned* tp = 0);

  /*! \brief
    Improves the result of the \ref CC76_extrapolation "CC76-extrapolation"
    computation by also enforcing those constraints in \p cs that are
    satisfied by all the points of \p *this.

    \param y
    A box that <EM>must</EM> be contained in \p *this.

    \param cs
    The system of constraints used to improve the widened box.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this, \p y and \p cs are dimension-incompatible or
    if \p cs contains a strict inequality.
  */
  void limited_CC76_extrapolation_assign(const Box& y,
                                         const Constraint_System& cs,
                                         unsigned* tp = 0);

  /*! \brief
    Assigns to \p *this the result of restoring in \p y the constraints
    of \p *this that were lost by
    \ref CC76_extrapolation "CC76-extrapolation" applications.

    \param y
    A Box that <EM>must</EM> contain \p *this.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.

    \note
    As was the case for widening operators, the argument \p y is meant to
    denote the value computed in the previous iteration step, whereas
    \p *this denotes the value computed in the current iteration step
    (in the <EM>decreasing</EM> iteration sequence). Hence, the call
    <CODE>x.CC76_narrowing_assign(y)</CODE> will assign to \p x
    the result of the computation \f$\mathtt{y} \Delta \mathtt{x}\f$.
  */
  template <typename T>
  typename Enable_If<Is_Same<T, Box>::value
                     && Is_Same_Or_Derived<Interval_Base, ITV>::value,
                     void>::type
  CC76_narrowing_assign(const T& y);

  //@} Space-Dimension Preserving Member Functions that May Modify [...]

  //! \name Member Functions that May Modify the Dimension of the Vector Space
  //@{

  //! Adds \p m new dimensions and embeds the old box into the new space.
  /*!
    \param m
    The number of dimensions to add.

    The new dimensions will be those having the highest indexes in the new
    box, which is defined by a system of interval constraints in which the
    variables running through the new dimensions are unconstrained.
    For instance, when starting from the box \f$\cB \sseq \Rset^2\f$
    and adding a third dimension, the result will be the box
    \f[
      \bigl\{\,
        (x, y, z)^\transpose \in \Rset^3
      \bigm|
        (x, y)^\transpose \in \cB
      \,\bigr\}.
    \f]
  */
  void add_space_dimensions_and_embed(dimension_type m);

  /*! \brief
    Adds \p m new dimensions to the box and does not embed it in
    the new vector space.

    \param m
    The number of dimensions to add.

    The new dimensions will be those having the highest indexes in the
    new box, which is defined by a system of bounded differences in
    which the variables running through the new dimensions are all
    constrained to be equal to 0.
    For instance, when starting from the box \f$\cB \sseq \Rset^2\f$
    and adding a third dimension, the result will be the box
    \f[
      \bigl\{\,
        (x, y, 0)^\transpose \in \Rset^3
      \bigm|
        (x, y)^\transpose \in \cB
      \,\bigr\}.
    \f]
  */
  void add_space_dimensions_and_project(dimension_type m);

  /*! \brief
    Seeing a box as a set of tuples (its points),
    assigns to \p *this all the tuples that can be obtained by concatenating,
    in the order given, a tuple of \p *this with a tuple of \p y.

    Let \f$B \sseq \Rset^n\f$ and \f$D \sseq \Rset^m\f$ be the boxes
    corresponding, on entry, to \p *this and \p y, respectively.
    Upon successful completion, \p *this will represent the box
    \f$R \sseq \Rset^{n+m}\f$ such that
    \f[
      R \defeq
          \Bigl\{\,
            (x_1, \ldots, x_n, y_1, \ldots, y_m)^\transpose
          \Bigm|
            (x_1, \ldots, x_n)^\transpose \in B,
            (y_1, \ldots, y_m)^\transpose \in D
          \,\Bigl\}.
    \f]
    Another way of seeing it is as follows: first increases the space
    dimension of \p *this by adding \p y.space_dimension() new
    dimensions; then adds to the system of constraints of \p *this a
    renamed-apart version of the constraints of \p y.
  */
  void concatenate_assign(const Box& y);

  //! Removes all the specified dimensions.
  /*!
    \param vars
    The set of Variable objects corresponding to the dimensions to be removed.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with one of the Variable
    objects contained in \p vars.
  */
  void remove_space_dimensions(const Variables_Set& vars);

  /*! \brief
    Removes the higher dimensions so that the resulting space
    will have dimension \p new_dimension.

    \exception std::invalid_argument
    Thrown if \p new_dimension is greater than the space dimension
    of \p *this.
  */
  void remove_higher_space_dimensions(dimension_type new_dimension);

  /*! \brief
    Remaps the dimensions of the vector space according to
    a \ref Mapping_the_Dimensions_of_the_Vector_Space "partial function".

    \param pfunc
    The partial function specifying the destiny of each dimension.

    The template type parameter Partial_Function must provide
    the following methods.
    \code
      bool has_empty_codomain() const
    \endcode
    returns <CODE>true</CODE> if and only if the represented partial
    function has an empty co-domain (i.e., it is always undefined).
    The <CODE>has_empty_codomain()</CODE> method will always be called
    before the methods below.  However, if
    <CODE>has_empty_codomain()</CODE> returns <CODE>true</CODE>, none
    of the functions below will be called.
    \code
      dimension_type max_in_codomain() const
    \endcode
    returns the maximum value that belongs to the co-domain
    of the partial function.
    \code
      bool maps(dimension_type i, dimension_type& j) const
    \endcode
    Let \f$f\f$ be the represented function and \f$k\f$ be the value
    of \p i.  If \f$f\f$ is defined in \f$k\f$, then \f$f(k)\f$ is
    assigned to \p j and <CODE>true</CODE> is returned.
    If \f$f\f$ is undefined in \f$k\f$, then <CODE>false</CODE> is
    returned.

    The result is undefined if \p pfunc does not encode a partial
    function with the properties described in the
    \ref Mapping_the_Dimensions_of_the_Vector_Space
    "specification of the mapping operator".
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
    Thrown if \p var does not correspond to a dimension of the vector space.

    \exception std::length_error
    Thrown if adding \p m new space dimensions would cause the
    vector space to exceed dimension <CODE>max_space_dimension()</CODE>.

    If \p *this has space dimension \f$n\f$, with \f$n > 0\f$,
    and <CODE>var</CODE> has space dimension \f$k \leq n\f$,
    then the \f$k\f$-th space dimension is
    \ref expand_space_dimension "expanded" to \p m new space dimensions
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
    one of the Variable objects contained in \p vars.
    Also thrown if \p dest is contained in \p vars.

    If \p *this has space dimension \f$n\f$, with \f$n > 0\f$,
    <CODE>dest</CODE> has space dimension \f$k \leq n\f$,
    \p vars is a set of variables whose maximum space dimension
    is also less than or equal to \f$n\f$, and \p dest is not a member
    of \p vars, then the space dimensions corresponding to
    variables in \p vars are \ref fold_space_dimensions "folded"
    into the \f$k\f$-th space dimension.
  */
  void fold_space_dimensions(const Variables_Set& vars, Variable dest);

  //@} // Member Functions that May Modify the Dimension of the Vector Space

  /*! \brief
    Returns a reference the interval that bounds \p var.

    \exception std::invalid_argument
    Thrown if \p var is not a space dimension of \p *this.
  */
  const ITV& get_interval(Variable var) const;

  /*! \brief
    Sets to \p i the interval that bounds \p var.

    \exception std::invalid_argument
    Thrown if \p var is not a space dimension of \p *this.
  */
  void set_interval(Variable var, const ITV& i);

  /*! \brief
    If the space dimension of \p var is unbounded below, return
    <CODE>false</CODE>. Otherwise return <CODE>true</CODE> and set
    \p n, \p d and \p closed accordingly.

    \note
    It is assumed that <CODE>*this</CODE> is a non-empty box
    having space dimension greater than or equal to that of \p var.
    An undefined behavior is obtained if this assumption is not met.
    \if Include_Implementation_Details
    To be more precise, if <CODE>*this</CODE> is an <EM>empty</EM> box
    (having space dimension greater than or equal to that of \p var)
    such that <CODE>!marked_empty()</CODE> holds, then the method can be
    called without incurring in undefined behavior: it will return
    <EM>unspecified</EM> boundary values that, if queried systematically
    on all space dimensions, will encode the box emptiness.
    \endif

    Let \f$I\f$ be the interval corresponding to variable \p var
    in the non-empty box <CODE>*this</CODE>.
    If \f$I\f$ is not bounded from below, simply return <CODE>false</CODE>
    (leaving all other parameters unchanged).
    Otherwise, set \p n, \p d and \p closed as follows:
     - \p n and \p d are assigned the integers \f$n\f$ and \f$d\f$ such
       that the fraction \f$n/d\f$ corresponds to the greatest lower bound
       of \f$I\f$. The fraction \f$n/d\f$ is in canonical form, meaning
       that \f$n\f$ and \f$d\f$ have no common factors, \f$d\f$ is positive,
       and if \f$n\f$ is zero then \f$d\f$ is one;
     - \p closed is set to <CODE>true</CODE> if and only if the lower
       boundary of \f$I\f$ is closed (i.e., it is included in the interval).
  */
  bool has_lower_bound(Variable var,
                       Coefficient& n, Coefficient& d, bool& closed) const;

  /*! \brief
    If the space dimension of \p var is unbounded above, return
    <CODE>false</CODE>. Otherwise return <CODE>true</CODE> and set
    \p n, \p d and \p closed accordingly.

    \note
    It is assumed that <CODE>*this</CODE> is a non-empty box
    having space dimension greater than or equal to that of \p var.
    An undefined behavior is obtained if this assumption is not met.
    \if Include_Implementation_Details
    To be more precise, if <CODE>*this</CODE> is an <EM>empty</EM> box
    (having space dimension greater than or equal to that of \p var)
    such that <CODE>!marked_empty()</CODE> holds, then the method can be
    called without incurring in undefined behavior: it will return
    <EM>unspecified</EM> boundary values that, if queried systematically
    on all space dimensions, will encode the box emptiness.
    \endif

    Let \f$I\f$ be the interval corresponding to variable \p var
    in the non-empty box <CODE>*this</CODE>.
    If \f$I\f$ is not bounded from above, simply return <CODE>false</CODE>
    (leaving all other parameters unchanged).
    Otherwise, set \p n, \p d and \p closed as follows:
     - \p n and \p d are assigned the integers \f$n\f$ and \f$d\f$ such
       that the fraction \f$n/d\f$ corresponds to the least upper bound
       of \f$I\f$. The fraction \f$n/d\f$ is in canonical form, meaning
       that \f$n\f$ and \f$d\f$ have no common factors, \f$d\f$ is positive,
       and if \f$n\f$ is zero then \f$d\f$ is one;
     - \p closed is set to <CODE>true</CODE> if and only if the upper
       boundary of \f$I\f$ is closed (i.e., it is included in the interval).
  */
  bool has_upper_bound(Variable var,
                       Coefficient& n, Coefficient& d, bool& closed) const;

  //! Returns a system of constraints defining \p *this.
  Constraint_System constraints() const;

  //! Returns a minimized system of constraints defining \p *this.
  Constraint_System minimized_constraints() const;

  //! Returns a system of congruences approximating \p *this.
  Congruence_System congruences() const;

  //! Returns a minimized system of congruences approximating \p *this.
  Congruence_System minimized_congruences() const;

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  /*! \brief
    Returns a 32-bit hash code for \p *this.

    If <CODE>x</CODE> and <CODE>y</CODE> are such that <CODE>x == y</CODE>,
    then <CODE>x.hash_code() == y.hash_code()</CODE>.
  */
  int32_t hash_code() const;

  PPL_OUTPUT_DECLARATIONS

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  bool ascii_load(std::istream& s);

private:
  template <typename Other_ITV>
  friend class Parma_Polyhedra_Library::Box;

  friend bool
  operator==<ITV>(const Box<ITV>& x, const Box<ITV>& y);

  friend std::ostream&
  Parma_Polyhedra_Library
  ::IO_Operators::operator<<<>(std::ostream& s, const Box<ITV>& box);

  template <typename Specialization, typename Temp, typename To, typename I>
  friend bool Parma_Polyhedra_Library::l_m_distance_assign
  (Checked_Number<To, Extended_Number_Policy>& r,
   const Box<I>& x, const Box<I>& y, const Rounding_Dir dir,
   Temp& tmp0, Temp& tmp1, Temp& tmp2);

  //! The type of sequence used to implement the box.
  typedef std::vector<ITV> Sequence;

  /*! \brief
    The type of intervals used by inner computations when trying to limit
    the cumulative effect of approximation errors.
  */
  typedef ITV Tmp_Interval_Type;

  //! A sequence of intervals, one for each dimension of the vector space.
  Sequence seq;

#define PPL_IN_Box_CLASS
#include "Box_Status_idefs.hh"
#undef PPL_IN_Box_CLASS

  //! The status flags to keep track of the internal state.
  Status status;

  /*! \brief
    Returns <CODE>true</CODE> if and only if the box is known to be empty.

    The return value <CODE>false</CODE> does not necessarily
    implies that \p *this is non-empty.
  */
  bool marked_empty() const;

public:
  //! Causes the box to become empty, i.e., to represent the empty set.
  void set_empty();

private:
  //! Marks \p *this as definitely not empty.
  void set_nonempty();

  //! Asserts the validity of the empty flag of \p *this.
  void set_empty_up_to_date();

  //! Invalidates empty flag of \p *this.
  void reset_empty_up_to_date();

  /*! \brief
    Checks the hard way whether \p *this is an empty box:
    returns <CODE>true</CODE> if and only if it is so.
  */
  bool check_empty() const;

   /*! \brief
     Returns a reference the interval that bounds
     the box on the <CODE>k</CODE>-th space dimension.
   */
  const ITV& operator[](dimension_type k) const;

  /*! \brief
    WRITE ME.
  */
  static I_Result
  refine_interval_no_check(ITV& itv,
                           Constraint::Type type,
                           Coefficient_traits::const_reference numer,
                           Coefficient_traits::const_reference denom);

  /*! \brief
    WRITE ME.
  */
  void
  add_interval_constraint_no_check(dimension_type var_id,
                                   Constraint::Type type,
                                   Coefficient_traits::const_reference numer,
                                   Coefficient_traits::const_reference denom);

  /*! \brief
    WRITE ME.
  */
  void add_constraint_no_check(const Constraint& c);

  /*! \brief
    WRITE ME.
  */
  void add_constraints_no_check(const Constraint_System& cs);

  /*! \brief
    WRITE ME.
  */
  void add_congruence_no_check(const Congruence& cg);

  /*! \brief
    WRITE ME.
  */
  void add_congruences_no_check(const Congruence_System& cgs);

  /*! \brief
    Uses the constraint \p c to refine \p *this.

    \param c
    The constraint to be used for the refinement.

    \warning
    If \p c and \p *this are dimension-incompatible,
    the behavior is undefined.
  */
  void refine_no_check(const Constraint& c);

  /*! \brief
    Uses the constraints in \p cs to refine \p *this.

    \param cs
    The constraints to be used for the refinement.
    To avoid termination problems, each constraint in \p cs
    will be used for a single refinement step.

    \warning
    If \p cs and \p *this are dimension-incompatible,
    the behavior is undefined.
  */
  void refine_no_check(const Constraint_System& cs);

  /*! \brief
    Uses the congruence \p cg to refine \p *this.

    \param cg
    The congruence to be added.
    Nontrivial proper congruences are ignored.

    \warning
    If \p cg and \p *this are dimension-incompatible,
    the behavior is undefined.
  */
  void refine_no_check(const Congruence& cg);

  /*! \brief
    Uses the congruences in \p cgs to refine \p *this.

    \param cgs
    The congruences to be added.
    Nontrivial proper congruences are ignored.

    \warning
    If \p cgs and \p *this are dimension-incompatible,
    the behavior is undefined.
  */
  void refine_no_check(const Congruence_System& cgs);

  /*! \brief
    Propagates the constraint \p c to refine \p *this.

    \param c
    The constraint to be propagated.

    \warning
    If \p c and \p *this are dimension-incompatible,
    the behavior is undefined.

    \warning
    This method may lead to non-termination.

    \if Include_Implementation_Details

    For any expression \f$e\f$, we denote by
    \f$\left\uparrow e \right\uparrow\f$ (resp., \f$\left\downarrow e
    \right\downarrow\f$) the result of any computation that is
    guaranteed to yield an upper (resp., lower) approximation of
    \f$e\f$.  So there exists \f$\epsilon \in \Rset\f$ with
    \f$\epsilon \geq 0\f$ such that
    \f$\left\uparrow e \right\uparrow = e + \epsilon\f$.
    If \f$\epsilon = 0\f$ we say that the computation of
    \f$\left\uparrow e \right\uparrow\f$ is <EM>exact</EM>;
    we say it is <EM>inexact</EM> otherwise.
    Similarly for \f$\left\downarrow e \right\downarrow\f$.

    Consider a constraint of the general form
    \f[
      z + \sum_{i \in I}{a_ix_i} \relsym 0,
    \f]
    where \f$z \in \Zset\f$, \f$I\f$ is a set of indices,
    \f$a_i \in \Zset\f$ with \f$a_i \neq 0\f$ for each \f$i \in I\f$, and
    \f$\mathord{\relsym} \in \{ \mathord{\geq}, \mathord{>}, \mathord{=} \}\f$.
    The set \f$I\f$ is subdivided into the disjoint sets \f$P\f$ and \f$N\f$
    such that, for each \f$i \in I\f$, \f$a_i > 0\f$ if \f$i \in P\f$ and
    \f$a_i < 0\f$ if \f$i \in N\f$.
    Suppose that, for each \f$i \in P \union N\f$ a variation interval
    \f$\chi_i \sseq \Rset\f$ is known for \f$x_i\f$ and that the infimum
    and the supremum of \f$\chi_i\f$ are denoted, respectively,
    by \f$\chi_i^\mathrm{l}\f$ and \f$\chi_i^\mathrm{u}\f$, where
    \f$\chi_i^\mathrm{l}, \chi_i^\mathrm{u} \in \Rset \union \{ -\infty, +\infty \}\f$.

    For each \f$k \in P\f$, we have
    \f[
      x_k
        \relsym
          \frac{1}{a_k}
            \Biggl(
              - z
              - \sum_{i \in N}{a_ix_i}
              - \sum_{\genfrac{}{}{0pt}{}
                              {\scriptstyle i \in P}
                              {\scriptstyle i \neq k}}{a_ix_i}
            \Biggr).
    \f]
    Thus, if \f$\chi_i^\mathrm{l} \in \Rset\f$ for each \f$i \in N\f$ and
    \f$\chi_i^\mathrm{u} \in \Rset\f$ for each \f$i \in P \setdiff \{ k \}\f$,
    we have
    \f[
      x_k
        \geq
          \Biggl\downarrow
          \frac{1}{a_k}
            \Biggl(
              - z
              - \sum_{i \in N}{a_i\chi_i^\mathrm{l}}
              - \sum_{\genfrac{}{}{0pt}{}
                              {\scriptstyle i \in P}
                              {\scriptstyle i \neq k}}{a_i\chi_i^\mathrm{u}}
            \Biggr)
          \Biggr\downarrow
    \f]
    and, if \f$\mathord{\relsym} \in \{ \mathord{=} \}\f$,
    \f$\chi_i^\mathrm{u} \in \Rset\f$ for each \f$i \in N\f$ and
    \f$\chi_i^\mathrm{l} \in \Rset\f$ for each \f$P \setdiff \{ k \}\f$,
    \f[
      x_k
        \leq
          \Biggl\uparrow
          \frac{1}{a_k}
            \Biggl(
              - z
              - \sum_{i \in N}{a_i\chi_i^\mathrm{u}}
              - \sum_{\genfrac{}{}{0pt}{}
                              {\scriptstyle i \in P}
                              {\scriptstyle i \neq k}}{a_i\chi_i^\mathrm{l}}
            \Biggr)
          \Biggl\uparrow.
    \f]
    In the first inequality, the relation is strict if
    \f$\mathord{\relsym} \in \{ \mathord{>} \}\f$, or if
    \f$\chi_i^\mathrm{l} \notin \chi_i\f$ for some \f$i \in N\f$, or if
    \f$\chi_i^\mathrm{u} \notin \chi_i\f$ for some
    \f$i \in P \setdiff \{ k \}\f$, or if the computation is inexact.
    In the second inequality, the relation is strict if
    \f$\chi_i^\mathrm{u} \notin \chi_i\f$ for some \f$i \in N\f$, or if
    \f$\chi_i^\mathrm{l} \notin \chi_i\f$ for some
    \f$i \in P \setdiff \{ k \}\f$, or if the computation is inexact.

    For each \f$k \in N\f$, we have
    \f[
      \frac{1}{a_k}
        \Biggl(
          - z
          - \sum_{\genfrac{}{}{0pt}{}
                          {\scriptstyle i \in N}
                          {\scriptstyle i \neq k}}{a_ix_i}
          - \sum_{i \in P}{a_ix_i}
        \Biggr)
          \relsym
            x_k.
    \f]
    Thus, if
    \f$\chi_i^\mathrm{l} \in \Rset\f$
    for each \f$i \in N \setdiff \{ k \}\f$ and
    \f$\chi_i^\mathrm{u} \in \Rset\f$ for each \f$i \in P\f$,
    we have
    \f[
      \Biggl\uparrow
      \frac{1}{a_k}
        \Biggl(
          - z
          - \sum_{\genfrac{}{}{0pt}{}
                          {\scriptstyle i \in N}
                          {\scriptstyle i \neq k}}{a_i\chi_i^\mathrm{l}}
          - \sum_{i \in P}{a_i\chi_i^\mathrm{u}}
        \Biggr)
      \Biggl\uparrow
        \geq
          x_k
    \f]
    and, if \f$\mathord{\relsym} \in \{ \mathord{=} \}\f$,
    \f$\chi_i^\mathrm{u} \in \Rset\f$ for each \f$i \in N \setdiff \{ k \}\f$
    and \f$\chi_i^\mathrm{l} \in \Rset\f$ for each \f$i \in P\f$,
    \f[
      \Biggl\downarrow
      \frac{1}{a_k}
        \Biggl(
          - z
          - \sum_{\genfrac{}{}{0pt}{}
                          {\scriptstyle i \in N}
                          {\scriptstyle i \neq k}}{a_i\chi_i^\mathrm{u}}
          - \sum_{i \in P}{a_i\chi_i^\mathrm{l}}
        \Biggr)
      \Biggl\downarrow
        \leq
          x_k.
    \f]
    In the first inequality, the relation is strict if
    \f$\mathord{\relsym} \in \{ \mathord{>} \}\f$, or if
    \f$\chi_i^\mathrm{u} \notin \chi_i\f$ for some \f$i \in P\f$, or if
    \f$\chi_i^\mathrm{l} \notin \chi_i\f$ for some
    \f$i \in N \setdiff \{ k \}\f$, or if the computation is inexact.
    In the second inequality, the relation is strict if
    \f$\chi_i^\mathrm{l} \notin \chi_i\f$ for some \f$i \in P\f$, or if
    \f$\chi_i^\mathrm{u} \notin \chi_i\f$ for some
    \f$i \in N \setdiff \{ k \}\f$, or if the computation is inexact.
    \endif
  */
  void propagate_constraint_no_check(const Constraint& c);

  /*! \brief
    Propagates the constraints in \p cs to refine \p *this.

    \param  cs
    The constraints to be propagated.

    \param max_iterations
    The maximum number of propagation steps for each constraint in \p cs.
    If zero, the number of propagation steps will be unbounded, possibly
    resulting in an infinite loop.

    \warning
    If \p cs and \p *this are dimension-incompatible,
    the behavior is undefined.

    \warning
    This method may lead to non-termination if \p max_iterations is 0.
  */
  void propagate_constraints_no_check(const Constraint_System& cs,
                                      dimension_type max_iterations);

  //! Checks if and how \p expr is bounded in \p *this.
  /*!
    Returns <CODE>true</CODE> if and only if \p from_above is
    <CODE>true</CODE> and \p expr is bounded from above in \p *this,
    or \p from_above is <CODE>false</CODE> and \p expr is bounded
    from below in \p *this.

    \param expr
    The linear expression to test;

    \param from_above
    <CODE>true</CODE> if and only if the boundedness of interest is
    "from above".

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.
  */
  bool bounds(const Linear_Expression& expr, bool from_above) const;

  //! Maximizes or minimizes \p expr subject to \p *this.
  /*!
    \param expr
    The linear expression to be maximized or minimized subject to \p *this;

    \param maximize
    <CODE>true</CODE> if maximization is what is wanted;

    \param ext_n
    The numerator of the extremum value;

    \param ext_d
    The denominator of the extremum value;

    \param included
    <CODE>true</CODE> if and only if the extremum of \p expr can
    actually be reached in \p *this;

    \param g
    When maximization or minimization succeeds, will be assigned
    a point or closure point where \p expr reaches the
    corresponding extremum value.

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded in the appropriate
    direction, <CODE>false</CODE> is returned and \p ext_n, \p ext_d,
    \p included and \p g are left untouched.
  */
  bool max_min(const Linear_Expression& expr,
               bool maximize,
               Coefficient& ext_n, Coefficient& ext_d, bool& included,
               Generator& g) const;

  //! Maximizes or minimizes \p expr subject to \p *this.
  /*!
    \param expr
    The linear expression to be maximized or minimized subject to \p *this;

    \param maximize
    <CODE>true</CODE> if maximization is what is wanted;

    \param ext_n
    The numerator of the extremum value;

    \param ext_d
    The denominator of the extremum value;

    \param included
    <CODE>true</CODE> if and only if the extremum of \p expr can
    actually be reached in \p * this;

    \exception std::invalid_argument
    Thrown if \p expr and \p *this are dimension-incompatible.

    If \p *this is empty or \p expr is not bounded in the appropriate
    direction, <CODE>false</CODE> is returned and \p ext_n, \p ext_d,
    \p included and \p point are left untouched.
  */
  bool max_min(const Linear_Expression& expr,
               bool maximize,
               Coefficient& ext_n, Coefficient& ext_d, bool& included) const;

  /*! \brief
    Adds to \p limiting_box the interval constraints in \p cs
    that are satisfied by \p *this.
  */
  void get_limiting_box(const Constraint_System& cs,
                        Box& limiting_box) const;

  //! \name Exception Throwers
  //@{
  void throw_dimension_incompatible(const char* method,
                                    const Box& y) const;

  void throw_dimension_incompatible(const char* method,
                                    dimension_type required_dim) const;

  void throw_dimension_incompatible(const char* method,
                                    const Constraint& c) const;

  void throw_dimension_incompatible(const char* method,
                                    const Congruence& cg) const;

  void throw_dimension_incompatible(const char* method,
                                    const Constraint_System& cs) const;

  void throw_dimension_incompatible(const char* method,
                                    const Congruence_System& cgs) const;

  void throw_dimension_incompatible(const char* method,
                                    const Generator& g) const;

  void throw_dimension_incompatible(const char* method,
                                    const char* le_name,
                                    const Linear_Expression& le) const;

  template <typename C>
  void throw_dimension_incompatible(const char* method,
                                    const char* lf_name,
                                    const Linear_Form<C>& lf) const;

  static void throw_constraint_incompatible(const char* method);

  static void throw_expression_too_complex(const char* method,
                                           const Linear_Expression& le);

  static void throw_invalid_argument(const char* method, const char* reason);
  //@} // Exception Throwers
};

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Returns the relations holding between an interval and
  an interval constraint.

  \param i
  The interval;

  \param constraint_type
  The constraint type;

  \param numer
  The numerator of the constraint bound;

  \param denom
  The denominator of the constraint bound

  The interval constraint has the form
  <CODE>denom * Variable(0) relsym numer</CODE>
  where relsym is  <CODE>==</CODE>,  <CODE>></CODE> or  <CODE>>=</CODE>
  depending on the <CODE>constraint_type</CODE>.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename ITV>
Poly_Con_Relation
interval_relation(const ITV& i,
                  const Constraint::Type constraint_type,
                  Coefficient_traits::const_reference numer,
                  Coefficient_traits::const_reference denom = 1);

class Box_Helpers {
public:
  // This is declared here so that Linear_Expression needs to be friend of
  // Box_Helpers only, and doesn't need to be friend of this, too.
  #ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! Decodes the constraint \p c as an interval constraint.
  /*! \relates Box
    \return
    <CODE>true</CODE> if the constraint \p c is an
    \ref intervals "interval constraint";
    <CODE>false</CODE> otherwise.

    \param c
    The constraint to be decoded.

    \param c_num_vars
    If <CODE>true</CODE> is returned, then it will be set to the number
    of variables having a non-zero coefficient. The only legal values
    will therefore be 0 and 1.

    \param c_only_var
    If <CODE>true</CODE> is returned and if \p c_num_vars is not set to 0,
    then it will be set to the index of the only variable having
    a non-zero coefficient in \p c.
  */
  #endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  static bool extract_interval_constraint(const Constraint& c,
                                          dimension_type& c_num_vars,
                                          dimension_type& c_only_var);

  // This is declared here so that Linear_Expression needs to be friend of
  // Box_Helpers only, and doesn't need to be friend of this, too.
  static bool extract_interval_congruence(const Congruence& cg,
                                          dimension_type& cg_num_vars,
                                          dimension_type& cg_only_var);
};

} // namespace Parma_Polyhedra_Library

#include "Box_Status_inlines.hh"
#include "Box_inlines.hh"
#include "Box_templates.hh"

#endif // !defined(PPL_Box_defs_hh)
