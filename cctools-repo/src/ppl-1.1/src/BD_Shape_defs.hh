/* BD_Shape class declaration.
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

#ifndef PPL_BD_Shape_defs_hh
#define PPL_BD_Shape_defs_hh 1

#include "BD_Shape_types.hh"
#include "globals_defs.hh"
#include "Constraint_types.hh"
#include "Generator_types.hh"
#include "Congruence_types.hh"
#include "Linear_Expression_types.hh"
#include "Constraint_System_types.hh"
#include "Generator_System_types.hh"
#include "Congruence_System_types.hh"
#include "Poly_Con_Relation_types.hh"
#include "Poly_Gen_Relation_types.hh"
#include "Polyhedron_types.hh"
#include "Box_types.hh"
#include "Grid_types.hh"
#include "Octagonal_Shape_types.hh"
#include "Variable_defs.hh"
#include "Variables_Set_types.hh"
#include "DB_Matrix_defs.hh"
#include "DB_Row_defs.hh"
#include "Checked_Number_defs.hh"
#include "WRD_coefficient_types_defs.hh"
#include "Bit_Matrix_defs.hh"
#include "Coefficient_defs.hh"
#include "Interval_types.hh"
#include "Linear_Form_types.hh"
#include <cstddef>
#include <iosfwd>
#include <vector>

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::BD_Shape
  Writes a textual representation of \p bds on \p s:
  <CODE>false</CODE> is written if \p bds is an empty polyhedron;
  <CODE>true</CODE> is written if \p bds is the universe polyhedron;
  a system of constraints defining \p bds is written otherwise,
  all constraints separated by ", ".
*/
template <typename T>
std::ostream&
operator<<(std::ostream& s, const BD_Shape<T>& bds);

} // namespace IO_Operators

//! Swaps \p x with \p y.
/*! \relates BD_Shape */
template <typename T>
void swap(BD_Shape<T>& x, BD_Shape<T>& y);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are the same BDS.
/*! \relates BD_Shape
  Note that \p x and \p y may be dimension-incompatible shapes:
  in this case, the value <CODE>false</CODE> is returned.
*/
template <typename T>
bool operator==(const BD_Shape<T>& x, const BD_Shape<T>& y);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are not the same BDS.
/*! \relates BD_Shape
  Note that \p x and \p y may be dimension-incompatible shapes:
  in this case, the value <CODE>true</CODE> is returned.
*/
template <typename T>
bool operator!=(const BD_Shape<T>& x, const BD_Shape<T>& y);

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates BD_Shape
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.
*/
template <typename To, typename T>
bool rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                 const BD_Shape<T>& x,
                                 const BD_Shape<T>& y,
                                 Rounding_Dir dir);

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates BD_Shape
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.
*/
template <typename Temp, typename To, typename T>
bool rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                 const BD_Shape<T>& x,
                                 const BD_Shape<T>& y,
                                 Rounding_Dir dir);

//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates BD_Shape
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
template <typename Temp, typename To, typename T>
bool rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                 const BD_Shape<T>& x,
                                 const BD_Shape<T>& y,
                                 Rounding_Dir dir,
                                 Temp& tmp0,
                                 Temp& tmp1,
                                 Temp& tmp2);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates BD_Shape
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.
*/
template <typename To, typename T>
bool euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                               const BD_Shape<T>& x,
                               const BD_Shape<T>& y,
                               Rounding_Dir dir);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates BD_Shape
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.
*/
template <typename Temp, typename To, typename T>
bool euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                               const BD_Shape<T>& x,
                               const BD_Shape<T>& y,
                               Rounding_Dir dir);

//! Computes the euclidean distance between \p x and \p y.
/*! \relates BD_Shape
  If the euclidean distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
template <typename Temp, typename To, typename T>
bool euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                               const BD_Shape<T>& x,
                               const BD_Shape<T>& y,
                               Rounding_Dir dir,
                               Temp& tmp0,
                               Temp& tmp1,
                               Temp& tmp2);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates BD_Shape
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<To, Extended_Number_Policy\></CODE>.
*/
template <typename To, typename T>
bool l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                const BD_Shape<T>& x,
                                const BD_Shape<T>& y,
                                Rounding_Dir dir);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates BD_Shape
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using variables of type
  <CODE>Checked_Number\<Temp, Extended_Number_Policy\></CODE>.
*/
template <typename Temp, typename To, typename T>
bool l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                const BD_Shape<T>& x,
                                const BD_Shape<T>& y,
                                Rounding_Dir dir);

//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates BD_Shape
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into \p r and returns <CODE>true</CODE>;
  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
template <typename Temp, typename To, typename T>
bool l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                const BD_Shape<T>& x,
                                const BD_Shape<T>& y,
                                Rounding_Dir dir,
                                Temp& tmp0,
                                Temp& tmp1,
                                Temp& tmp2);

// This class contains some helper functions that need to be friends of
// Linear_Expression.
class BD_Shape_Helpers {
public:
  #ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! Decodes the constraint \p c as a bounded difference.
  /*! \relates BD_Shape
    \return
    <CODE>true</CODE> if the constraint \p c is a
    \ref Bounded_Difference_Shapes "bounded difference";
    <CODE>false</CODE> otherwise.

    \param c
    The constraint to be decoded.

    \param c_num_vars
    If <CODE>true</CODE> is returned, then it will be set to the number
    of variables having a non-zero coefficient. The only legal values
    will therefore be 0, 1 and 2.

    \param c_first_var
    If <CODE>true</CODE> is returned and if \p c_num_vars is not set to 0,
    then it will be set to the index of the first variable having
    a non-zero coefficient in \p c.

    \param c_second_var
    If <CODE>true</CODE> is returned and if \p c_num_vars is set to 2,
    then it will be set to the index of the second variable having
    a non-zero coefficient in \p c. If \p c_num_vars is set to 1, this must be
    0.

    \param c_coeff
    If <CODE>true</CODE> is returned and if \p c_num_vars is not set to 0,
    then it will be set to the value of the first non-zero coefficient
    in \p c.
  */
  #endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  static bool extract_bounded_difference(const Constraint& c,
                                         dimension_type& c_num_vars,
                                         dimension_type& c_first_var,
                                         dimension_type& c_second_var,
                                         Coefficient& c_coeff);
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Extracts leader indices from the predecessor relation.
/*! \relates BD_Shape */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void compute_leader_indices(const std::vector<dimension_type>& predecessor,
                            std::vector<dimension_type>& indices);

} // namespace Parma_Polyhedra_Library

//! A bounded difference shape.
/*! \ingroup PPL_CXX_interface
  The class template BD_Shape<T> allows for the efficient representation
  of a restricted kind of <EM>topologically closed</EM> convex polyhedra
  called <EM>bounded difference shapes</EM> (BDSs, for short).
  The name comes from the fact that the closed affine half-spaces that
  characterize the polyhedron can be expressed by constraints of the form
  \f$\pm x_i \leq k\f$ or \f$x_i - x_j \leq k\f$, where the inhomogeneous
  term \f$k\f$ is a rational number.

  Based on the class template type parameter \p T, a family of extended
  numbers is built and used to approximate the inhomogeneous term of
  bounded differences. These extended numbers provide a representation
  for the value \f$+\infty\f$, as well as <EM>rounding-aware</EM>
  implementations for several arithmetic functions.
  The value of the type parameter \p T may be one of the following:
    - a bounded precision integer type (e.g., \c int32_t or \c int64_t);
    - a bounded precision floating point type (e.g., \c float or \c double);
    - an unbounded integer or rational type, as provided by GMP
      (i.e., \c mpz_class or \c mpq_class).

  The user interface for BDSs is meant to be as similar as possible to
  the one developed for the polyhedron class C_Polyhedron.

  The domain of BD shapes <EM>optimally supports</EM>:
    - tautological and inconsistent constraints and congruences;
    - bounded difference constraints;
    - non-proper congruences (i.e., equalities) that are expressible
      as bounded-difference constraints.

  Depending on the method, using a constraint or congruence that is not
  optimally supported by the domain will either raise an exception or
  result in a (possibly non-optimal) upward approximation.

  A constraint is a bounded difference if it has the form
    \f[
      a_i x_i - a_j x_j \relsym b
    \f]
  where \f$\mathord{\relsym} \in \{ \leq, =, \geq \}\f$ and
  \f$a_i\f$, \f$a_j\f$, \f$b\f$ are integer coefficients such that
  \f$a_i = 0\f$, or \f$a_j = 0\f$, or \f$a_i = a_j\f$.
  The user is warned that the above bounded difference Constraint object
  will be mapped into a \e correct and \e optimal approximation that,
  depending on the expressive power of the chosen template argument \p T,
  may loose some precision. Also note that strict constraints are not
  bounded differences.

  For instance, a Constraint object encoding \f$3x - 3y \leq 1\f$ will be
  approximated by:
    - \f$x - y \leq 1\f$,
      if \p T is a (bounded or unbounded) integer type;
    - \f$x - y \leq \frac{1}{3}\f$,
      if \p T is the unbounded rational type \c mpq_class;
    - \f$x - y \leq k\f$, where \f$k > \frac{1}{3}\f$,
      if \p T is a floating point type (having no exact representation
      for \f$\frac{1}{3}\f$).

  On the other hand, depending from the context, a Constraint object
  encoding \f$3x - y \leq 1\f$ will be either upward approximated
  (e.g., by safely ignoring it) or it will cause an exception.

  In the following examples it is assumed that the type argument \p T
  is one of the possible instances listed above and that variables
  <CODE>x</CODE>, <CODE>y</CODE> and <CODE>z</CODE> are defined
  (where they are used) as follows:
  \code
    Variable x(0);
    Variable y(1);
    Variable z(2);
  \endcode

  \par Example 1
  The following code builds a BDS corresponding to a cube in \f$\Rset^3\f$,
  given as a system of constraints:
  \code
    Constraint_System cs;
    cs.insert(x >= 0);
    cs.insert(x <= 1);
    cs.insert(y >= 0);
    cs.insert(y <= 1);
    cs.insert(z >= 0);
    cs.insert(z <= 1);
    BD_Shape<T> bd(cs);
  \endcode
  Since only those constraints having the syntactic form of a
  <EM>bounded difference</EM> are optimally supported, the following code
  will throw an exception (caused by constraints 7, 8 and 9):
  \code
    Constraint_System cs;
    cs.insert(x >= 0);
    cs.insert(x <= 1);
    cs.insert(y >= 0);
    cs.insert(y <= 1);
    cs.insert(z >= 0);
    cs.insert(z <= 1);
    cs.insert(x + y <= 0);      // 7
    cs.insert(x - z + x >= 0);  // 8
    cs.insert(3*z - y <= 1);    // 9
    BD_Shape<T> bd(cs);
  \endcode
*/
template <typename T>
class Parma_Polyhedra_Library::BD_Shape {
private:
  /*! \brief
    The (extended) numeric type of the inhomogeneous term of
    the inequalities defining a BDS.
  */
#ifndef NDEBUG
  typedef Checked_Number<T, Debug_WRD_Extended_Number_Policy> N;
#else
  typedef Checked_Number<T, WRD_Extended_Number_Policy> N;
#endif

public:
  //! The numeric base type upon which bounded differences are built.
  typedef T coefficient_type_base;

  /*! \brief
    The (extended) numeric type of the inhomogeneous term of the
    inequalities defining a BDS.
  */
  typedef N coefficient_type;

  //! Returns the maximum space dimension that a BDS can handle.
  static dimension_type max_space_dimension();

  /*! \brief
    Returns \c false indicating that this domain cannot recycle constraints.
  */
  static bool can_recycle_constraint_systems();

  /*! \brief
    Returns \c false indicating that this domain cannot recycle congruences.
  */
  static bool can_recycle_congruence_systems();

  //! \name Constructors, Assignment, Swap and Destructor
  //@{

  //! Builds a universe or empty BDS of the specified space dimension.
  /*!
    \param num_dimensions
    The number of dimensions of the vector space enclosing the BDS;

    \param kind
    Specifies whether the universe or the empty BDS has to be built.
  */
  explicit BD_Shape(dimension_type num_dimensions = 0,
                    Degenerate_Element kind = UNIVERSE);

  //! Ordinary copy constructor.
  /*!
    The complexity argument is ignored.
  */
  BD_Shape(const BD_Shape& y,
           Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a conservative, upward approximation of \p y.
  /*!
    The complexity argument is ignored.
  */
  template <typename U>
  explicit BD_Shape(const BD_Shape<U>& y,
                    Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a BDS from the system of constraints \p cs.
  /*!
    The BDS inherits the space dimension of \p cs.

    \param cs
    A system of BD constraints.

    \exception std::invalid_argument
    Thrown if \p cs contains a constraint which is not optimally supported
    by the BD shape domain.
  */
  explicit BD_Shape(const Constraint_System& cs);

  //! Builds a BDS from a system of congruences.
  /*!
    The BDS inherits the space dimension of \p cgs

    \param cgs
    A system of congruences.

    \exception std::invalid_argument
    Thrown if \p cgs contains congruences which are not optimally
    supported by the BD shape domain.
  */
  explicit BD_Shape(const Congruence_System& cgs);

  //! Builds a BDS from the system of generators \p gs.
  /*!
    Builds the smallest BDS containing the polyhedron defined by \p gs.
    The BDS inherits the space dimension of \p gs.

    \exception std::invalid_argument
    Thrown if the system of generators is not empty but has no points.
  */
  explicit BD_Shape(const Generator_System& gs);

  //! Builds a BDS from the polyhedron \p ph.
  /*!
    Builds a BDS containing \p ph using algorithms whose complexity
    does not exceed the one specified by \p complexity.  If
    \p complexity is \p ANY_COMPLEXITY, then the BDS built is the
    smallest one containing \p ph.
  */
  explicit BD_Shape(const Polyhedron& ph,
                    Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a BDS out of a box.
  /*!
    The BDS inherits the space dimension of the box.
    The built BDS is the most precise BDS that includes the box.

    \param box
    The box representing the BDS to be built.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.

    \exception std::length_error
    Thrown if the space dimension of \p box exceeds the maximum
    allowed space dimension.
  */
  template <typename Interval>
  explicit BD_Shape(const Box<Interval>& box,
                    Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a BDS out of a grid.
  /*!
    The BDS inherits the space dimension of the grid.
    The built BDS is the most precise BDS that includes the grid.

    \param grid
    The grid used to build the BDS.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.

    \exception std::length_error
    Thrown if the space dimension of \p grid exceeds the maximum
    allowed space dimension.
  */
  explicit BD_Shape(const Grid& grid,
                    Complexity_Class complexity = ANY_COMPLEXITY);

  //! Builds a BDS from an octagonal shape.
  /*!
    The BDS inherits the space dimension of the octagonal shape.
    The built BDS is the most precise BDS that includes the octagonal shape.

    \param os
    The octagonal shape used to build the BDS.

    \param complexity
    This argument is ignored as the algorithm used has
    polynomial complexity.

    \exception std::length_error
    Thrown if the space dimension of \p os exceeds the maximum
    allowed space dimension.
  */
  template <typename U>
  explicit BD_Shape(const Octagonal_Shape<U>& os,
                    Complexity_Class complexity = ANY_COMPLEXITY);

  /*! \brief
    The assignment operator
    (\p *this and \p y can be dimension-incompatible).
  */
  BD_Shape& operator=(const BD_Shape& y);

  /*! \brief
    Swaps \p *this with \p y
    (\p *this and \p y can be dimension-incompatible).
  */
  void m_swap(BD_Shape& y);

  //! Destructor.
  ~BD_Shape();

  //@} Constructors, Assignment, Swap and Destructor

  //! \name Member Functions that Do Not Modify the BD_Shape
  //@{

  //! Returns the dimension of the vector space enclosing \p *this.
  dimension_type space_dimension() const;

  /*! \brief
    Returns \f$0\f$, if \p *this is empty; otherwise, returns the
    \ref Affine_Independence_and_Affine_Dimension "affine dimension"
    of \p *this.
  */
  dimension_type affine_dimension() const;

  //! Returns a system of constraints defining \p *this.
  Constraint_System constraints() const;

  //! Returns a minimized system of constraints defining \p *this.
  Constraint_System minimized_constraints() const;

  //! Returns a system of (equality) congruences satisfied by \p *this.
  Congruence_System congruences() const;

  /*! \brief
    Returns a minimal system of (equality) congruences
    satisfied by \p *this with the same affine dimension as \p *this.
  */
  Congruence_System minimized_congruences() const;

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

  //! Returns <CODE>true</CODE> if and only if \p *this contains \p y.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool contains(const BD_Shape& y) const;

  //! Returns <CODE>true</CODE> if and only if \p *this strictly contains \p y.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool strictly_contains(const BD_Shape& y) const;

  //! Returns <CODE>true</CODE> if and only if \p *this and \p y are disjoint.
  /*!
    \exception std::invalid_argument
    Thrown if \p x and \p y are topology-incompatible or
    dimension-incompatible.
  */
  bool is_disjoint_from(const BD_Shape& y) const;

  //! Returns the relations holding between \p *this and the constraint \p c.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and constraint \p c are dimension-incompatible.
  */
  Poly_Con_Relation relation_with(const Constraint& c) const;

  //! Returns the relations holding between \p *this and the congruence \p cg.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and congruence \p cg are dimension-incompatible.
  */
  Poly_Con_Relation relation_with(const Congruence& cg) const;

  //! Returns the relations holding between \p *this and the generator \p g.
  /*!
    \exception std::invalid_argument
    Thrown if \p *this and generator \p g are dimension-incompatible.
  */
  Poly_Gen_Relation relation_with(const Generator& g) const;

  //! Returns <CODE>true</CODE> if and only if \p *this is an empty BDS.
  bool is_empty() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a universe BDS.
  bool is_universe() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is discrete.
  bool is_discrete() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this
    is a topologically closed subset of the vector space.
  */
  bool is_topologically_closed() const;

  //! Returns <CODE>true</CODE> if and only if \p *this is a bounded BDS.
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

  /*! \brief
    Returns <CODE>true</CODE> if and only if \p *this satisfies
    all its invariants.
  */
  bool OK() const;

  //@} Member Functions that Do Not Modify the BD_Shape

  //! \name Space-Dimension Preserving Member Functions that May Modify the BD_Shape
  //@{

  /*! \brief
    Adds a copy of constraint \p c to the system of bounded differences
    defining \p *this.

    \param c
    The constraint to be added.

    \exception std::invalid_argument
    Thrown if \p *this and constraint \p c are dimension-incompatible,
    or \p c is not optimally supported by the BD shape domain.
  */
  void add_constraint(const Constraint& c);

  /*! \brief
    Adds a copy of congruence \p cg to the system of congruences of \p *this.

    \param cg
    The congruence to be added.

    \exception std::invalid_argument
    Thrown if \p *this and congruence \p cg are dimension-incompatible,
    or \p cg is not optimally supported by the BD shape domain.
  */
  void add_congruence(const Congruence& cg);

  /*! \brief
    Adds the constraints in \p cs to the system of bounded differences
    defining \p *this.

    \param  cs
    The constraints that will be added.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible,
    or \p cs contains a constraint which is not optimally supported
    by the BD shape domain.
  */
  void add_constraints(const Constraint_System& cs);

  /*! \brief
    Adds the constraints in \p cs to the system of constraints
    of \p *this.

    \param cs
    The constraint system to be added to \p *this.  The constraints in
    \p cs may be recycled.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible,
    or \p cs contains a constraint which is not optimally supported
    by the BD shape domain.

    \warning
    The only assumption that can be made on \p cs upon successful or
    exceptional return is that it can be safely destroyed.
  */
  void add_recycled_constraints(Constraint_System& cs);

  /*! \brief
    Adds to \p *this constraints equivalent to the congruences in \p cgs.

    \param cgs
    Contains the congruences that will be added to the system of
    constraints of \p *this.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible,
    or \p cgs contains a congruence which is not optimally supported
    by the BD shape domain.
  */
  void add_congruences(const Congruence_System& cgs);

  /*! \brief
    Adds to \p *this constraints equivalent to the congruences in \p cgs.

    \param cgs
    Contains the congruences that will be added to the system of
    constraints of \p *this. Its elements may be recycled.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible,
    or \p cgs contains a congruence which is not optimally supported
    by the BD shape domain.

    \warning
    The only assumption that can be made on \p cgs upon successful or
    exceptional return is that it can be safely destroyed.
  */
  void add_recycled_congruences(Congruence_System& cgs);

  /*! \brief
    Uses a copy of constraint \p c to refine the system of bounded differences
    defining \p *this.

    \param c
    The constraint. If it is not a bounded difference, it will be ignored.

    \exception std::invalid_argument
    Thrown if \p *this and constraint \p c are dimension-incompatible.
  */
  void refine_with_constraint(const Constraint& c);

  /*! \brief
    Uses a copy of congruence \p cg to refine the system of
    bounded differences  of \p *this.

    \param cg
    The congruence. If it is not a bounded difference equality, it
    will be ignored.

    \exception std::invalid_argument
    Thrown if \p *this and congruence \p cg are dimension-incompatible.
  */
  void refine_with_congruence(const Congruence& cg);

  /*! \brief
    Uses a copy of the constraints in \p cs to refine the system of
    bounded differences defining \p *this.

    \param  cs
    The constraint system to be used. Constraints that are not bounded
    differences are ignored.

    \exception std::invalid_argument
    Thrown if \p *this and \p cs are dimension-incompatible.
  */
  void refine_with_constraints(const Constraint_System& cs);

  /*! \brief
    Uses a copy of the congruences in \p cgs to refine the system of
    bounded differences defining \p *this.

    \param  cgs
    The congruence system to be used. Congruences that are not bounded
    difference equalities are ignored.

    \exception std::invalid_argument
    Thrown if \p *this and \p cgs are dimension-incompatible.
  */
  void refine_with_congruences(const Congruence_System& cgs);

  /*! \brief
    Refines the system of BD_Shape constraints defining \p *this using
    the constraint expressed by \p left \f$\leq\f$ \p right.

    \param left
    The linear form on intervals with floating point boundaries that
    is at the left of the comparison operator. All of its coefficients
    MUST be bounded.

    \param right
    The linear form on intervals with floating point boundaries that
    is at the right of the comparison operator. All of its coefficients
    MUST be bounded.

    \exception std::invalid_argument
    Thrown if \p left (or \p right) is dimension-incompatible with \p *this.

    This function is used in abstract interpretation to model a filter
    that is generated by a comparison of two expressions that are correctly
    approximated by \p left and \p right respectively.
  */
  template <typename Interval_Info>
  void refine_with_linear_form_inequality(
                   const Linear_Form<Interval<T, Interval_Info> >& left,
                   const Linear_Form<Interval<T, Interval_Info> >& right);

  /*! \brief
    Refines the system of BD_Shape constraints defining \p *this using
    the constraint expressed by \p left \f$\relsym\f$ \p right, where
    \f$\relsym\f$ is the relation symbol specified by \p relsym.

    \param left
    The linear form on intervals with floating point boundaries that
    is at the left of the comparison operator. All of its coefficients
    MUST be bounded.

    \param right
    The linear form on intervals with floating point boundaries that
    is at the right of the comparison operator. All of its coefficients
    MUST be bounded.

    \param relsym
    The relation symbol.

    \exception std::invalid_argument
    Thrown if \p left (or \p right) is dimension-incompatible with \p *this.

    \exception std::runtime_error
    Thrown if \p relsym is not a valid relation symbol.

    This function is used in abstract interpretation to model a filter
    that is generated by a comparison of two expressions that are correctly
    approximated by \p left and \p right respectively.
  */
  template <typename Interval_Info>
  void generalized_refine_with_linear_form_inequality(
                   const Linear_Form<Interval<T, Interval_Info> >& left,
                   const Linear_Form<Interval<T, Interval_Info> >& right,
                   Relation_Symbol relsym);

  //! Applies to \p dest the interval constraints embedded in \p *this.
  /*!
    \param dest
    The object to which the constraints will be added.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p dest.

    The template type parameter U must provide the following methods.
    \code
      dimension_type space_dimension() const
    \endcode
    returns the space dimension of the object.
    \code
      void set_empty()
    \endcode
    sets the object to an empty object.
    \code
      bool restrict_lower(dimension_type dim, const T& lb)
    \endcode
    restricts the object by applying the lower bound \p lb to the space
    dimension \p dim and returns <CODE>false</CODE> if and only if the
    object becomes empty.
    \code
      bool restrict_upper(dimension_type dim, const T& ub)
    \endcode
    restricts the object by applying the upper bound \p ub to the space
    dimension \p dim and returns <CODE>false</CODE> if and only if the
    object becomes empty.
  */
  template <typename U>
  void export_interval_constraints(U& dest) const;

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
  void intersection_assign(const BD_Shape& y);

  /*! \brief
    Assigns to \p *this the smallest BDS containing the union
    of \p *this and \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void upper_bound_assign(const BD_Shape& y);

  /*! \brief
    If the upper bound of \p *this and \p y is exact, it is assigned
    to \p *this and <CODE>true</CODE> is returned,
    otherwise <CODE>false</CODE> is returned.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  bool upper_bound_assign_if_exact(const BD_Shape& y);

  /*! \brief
    If the \e integer upper bound of \p *this and \p y is exact,
    it is assigned to \p *this and <CODE>true</CODE> is returned;
    otherwise <CODE>false</CODE> is returned.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.

    \note
    The integer upper bound of two rational BDS is the smallest rational
    BDS containing all the integral points of the two arguments.
    This method requires that the coefficient type parameter \c T is
    an integral type.
  */
  bool integer_upper_bound_assign_if_exact(const BD_Shape& y);

  /*! \brief
    Assigns to \p *this the smallest BD shape containing
    the set difference of \p *this and \p y.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void difference_assign(const BD_Shape& y);

  /*! \brief
    Assigns to \p *this a \ref Meet_Preserving_Simplification
    "meet-preserving simplification" of \p *this with respect to \p y.
    If \c false is returned, then the intersection is empty.

    \exception std::invalid_argument
    Thrown if \p *this and \p y are topology-incompatible or
    dimension-incompatible.
  */
  bool simplify_using_context_assign(const BD_Shape& y);

  /*! \brief
    Assigns to \p *this the
    \ref Single_Update_Affine_Functions "affine image"
    of \p *this under the function mapping variable \p var into the
    affine expression specified by \p expr and \p denominator.

    \param var
    The variable to which the affine expression is assigned.

    \param expr
    The numerator of the affine expression.

    \param denominator
    The denominator of the affine expression.

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this
    are dimension-incompatible or if \p var is not a dimension of \p *this.
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
    The linear form on intervals with floating point coefficients that
    defines the affine expression. ALL of its coefficients MUST be bounded.

    \exception std::invalid_argument
    Thrown if \p lf and \p *this are dimension-incompatible or if \p var
    is not a dimension of \p *this.
  */
  template <typename Interval_Info>
  void affine_form_image(Variable var,
                        const Linear_Form<Interval<T, Interval_Info> >& lf);

  /*! \brief
    Assigns to \p *this the
    \ref Single_Update_Affine_Functions "affine preimage"
    of \p *this under the function mapping variable \p var into the
    affine expression specified by \p expr and \p denominator.

    \param var
    The variable to which the affine expression is substituted.

    \param expr
    The numerator of the affine expression.

    \param denominator
    The denominator of the affine expression.

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this
    are dimension-incompatible or if \p var is not a dimension of \p *this.
  */
  void affine_preimage(Variable var,
                       const Linear_Expression& expr,
                       Coefficient_traits::const_reference denominator
                       = Coefficient_one());

  /*! \brief
    Assigns to \p *this the image of \p *this with respect to the
    \ref Generalized_Affine_Relations "affine relation"
    \f$\mathrm{var}' \relsym \frac{\mathrm{expr}}{\mathrm{denominator}}\f$,
    where \f$\mathord{\relsym}\f$ is the relation symbol encoded
    by \p relsym.

    \param var
    The left hand side variable of the generalized affine transfer function.

    \param relsym
    The relation symbol.

    \param expr
    The numerator of the right hand side affine expression.

    \param denominator
    The denominator of the right hand side affine expression.

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this
    are dimension-incompatible or if \p var is not a dimension
    of \p *this or if \p relsym is a strict relation symbol.
  */
  void generalized_affine_image(Variable var,
                                Relation_Symbol relsym,
                                const Linear_Expression& expr,
                                Coefficient_traits::const_reference denominator
                                = Coefficient_one());

  /*! \brief
    Assigns to \p *this the image of \p *this with respect to the
    \ref Generalized_Affine_Relations "affine relation"
    \f$\mathrm{lhs}' \relsym \mathrm{rhs}\f$, where
    \f$\mathord{\relsym}\f$ is the relation symbol encoded by \p relsym.

    \param lhs
    The left hand side affine expression.

    \param relsym
    The relation symbol.

    \param rhs
    The right hand side affine expression.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p lhs or \p rhs
    or if \p relsym is a strict relation symbol.
  */
  void generalized_affine_image(const Linear_Expression& lhs,
                                Relation_Symbol relsym,
                                const Linear_Expression& rhs);

  /*! \brief
    Assigns to \p *this the preimage of \p *this with respect to the
    \ref Generalized_Affine_Relations "affine relation"
    \f$\mathrm{var}' \relsym \frac{\mathrm{expr}}{\mathrm{denominator}}\f$,
    where \f$\mathord{\relsym}\f$ is the relation symbol encoded
    by \p relsym.

    \param var
    The left hand side variable of the generalized affine transfer function.

    \param relsym
    The relation symbol.

    \param expr
    The numerator of the right hand side affine expression.

    \param denominator
    The denominator of the right hand side affine expression.

    \exception std::invalid_argument
    Thrown if \p denominator is zero or if \p expr and \p *this
    are dimension-incompatible or if \p var is not a dimension
    of \p *this or if \p relsym is a strict relation symbol.
  */
  void generalized_affine_preimage(Variable var,
                                   Relation_Symbol relsym,
                                   const Linear_Expression& expr,
                                   Coefficient_traits::const_reference
                                   denominator = Coefficient_one());

  /*! \brief
    Assigns to \p *this the preimage of \p *this with respect to the
    \ref Generalized_Affine_Relations "affine relation"
    \f$\mathrm{lhs}' \relsym \mathrm{rhs}\f$, where
    \f$\mathord{\relsym}\f$ is the relation symbol encoded by \p relsym.

    \param lhs
    The left hand side affine expression.

    \param relsym
    The relation symbol.

    \param rhs
    The right hand side affine expression.

    \exception std::invalid_argument
    Thrown if \p *this is dimension-incompatible with \p lhs or \p rhs
    or if \p relsym is a strict relation symbol.
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

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void time_elapse_assign(const BD_Shape& y);

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

  /*! \brief
    Assigns to \p *this the result of computing the
    \ref CC76_extrapolation "CC76-extrapolation" between \p *this and \p y.

    \param y
    A BDS that <EM>must</EM> be contained in \p *this.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void CC76_extrapolation_assign(const BD_Shape& y, unsigned* tp = 0);

  /*! \brief
    Assigns to \p *this the result of computing the
    \ref CC76_extrapolation "CC76-extrapolation" between \p *this and \p y.

    \param y
    A BDS that <EM>must</EM> be contained in \p *this.

    \param first
    An iterator referencing the first stop-point.

    \param last
    An iterator referencing one past the last stop-point.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  template <typename Iterator>
  void CC76_extrapolation_assign(const BD_Shape& y,
                                 Iterator first, Iterator last,
                                 unsigned* tp = 0);

  /*! \brief
    Assigns to \p *this the result of computing the
    \ref BHMZ05_widening "BHMZ05-widening" of \p *this and \p y.

    \param y
    A BDS that <EM>must</EM> be contained in \p *this.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void BHMZ05_widening_assign(const BD_Shape& y, unsigned* tp = 0);

  /*! \brief
    Improves the result of the \ref BHMZ05_widening "BHMZ05-widening"
    computation by also enforcing those constraints in \p cs that are
    satisfied by all the points of \p *this.

    \param y
    A BDS that <EM>must</EM> be contained in \p *this.

    \param cs
    The system of constraints used to improve the widened BDS.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this, \p y and \p cs are dimension-incompatible or
    if \p cs contains a strict inequality.
  */
  void limited_BHMZ05_extrapolation_assign(const BD_Shape& y,
                                           const Constraint_System& cs,
                                           unsigned* tp = 0);

  /*! \brief
    Assigns to \p *this the result of restoring in \p y the constraints
    of \p *this that were lost by
    \ref CC76_extrapolation "CC76-extrapolation" applications.

    \param y
    A BDS that <EM>must</EM> contain \p *this.

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
  void CC76_narrowing_assign(const BD_Shape& y);

  /*! \brief
    Improves the result of the \ref CC76_extrapolation "CC76-extrapolation"
    computation by also enforcing those constraints in \p cs that are
    satisfied by all the points of \p *this.

    \param y
    A BDS that <EM>must</EM> be contained in \p *this.

    \param cs
    The system of constraints used to improve the widened BDS.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this, \p y and \p cs are dimension-incompatible or
    if \p cs contains a strict inequality.
  */
  void limited_CC76_extrapolation_assign(const BD_Shape& y,
                                         const Constraint_System& cs,
                                         unsigned* tp = 0);

  /*! \brief
    Assigns to \p *this the result of computing the
    \ref H79_widening "H79-widening" between \p *this and \p y.

    \param y
    A BDS that <EM>must</EM> be contained in \p *this.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this and \p y are dimension-incompatible.
  */
  void H79_widening_assign(const BD_Shape& y, unsigned* tp = 0);

  //! Same as H79_widening_assign(y, tp).
  void widening_assign(const BD_Shape& y, unsigned* tp = 0);

  /*! \brief
    Improves the result of the \ref H79_widening "H79-widening"
    computation by also enforcing those constraints in \p cs that are
    satisfied by all the points of \p *this.

    \param y
    A BDS that <EM>must</EM> be contained in \p *this.

    \param cs
    The system of constraints used to improve the widened BDS.

    \param tp
    An optional pointer to an unsigned variable storing the number of
    available tokens (to be used when applying the
    \ref Widening_with_Tokens "widening with tokens" delay technique).

    \exception std::invalid_argument
    Thrown if \p *this, \p y and \p cs are dimension-incompatible.
  */
  void limited_H79_extrapolation_assign(const BD_Shape& y,
                                        const Constraint_System& cs,
                                        unsigned* tp = 0);

  //@} Space-Dimension Preserving Member Functions that May Modify [...]

  //! \name Member Functions that May Modify the Dimension of the Vector Space
  //@{

  //! Adds \p m new dimensions and embeds the old BDS into the new space.
  /*!
    \param m
    The number of dimensions to add.

    The new dimensions will be those having the highest indexes in the new
    BDS, which is defined by a system of bounded differences in which the
    variables running through the new dimensions are unconstrained.
    For instance, when starting from the BDS \f$\cB \sseq \Rset^2\f$
    and adding a third dimension, the result will be the BDS
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
    Adds \p m new dimensions to the BDS and does not embed it in
    the new vector space.

    \param m
    The number of dimensions to add.

    The new dimensions will be those having the highest indexes in the
    new BDS, which is defined by a system of bounded differences in
    which the variables running through the new dimensions are all
    constrained to be equal to 0.
    For instance, when starting from the BDS \f$\cB \sseq \Rset^2\f$
    and adding a third dimension, the result will be the BDS
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
    Assigns to \p *this the \ref Concatenating_Polyhedra "concatenation"
    of \p *this and \p y, taken in this order.

    \exception std::length_error
    Thrown if the concatenation would cause the vector space
    to exceed dimension <CODE>max_space_dimension()</CODE>.
  */
  void concatenate_assign(const BD_Shape& y);

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

  //! Refines \p store with the constraints defining \p *this.
  /*!
    \param store
    The interval floating point abstract store to refine.
  */
  template <typename Interval_Info>
  void refine_fp_interval_abstract_store(Box<Interval<T, Interval_Info> >&
                                         store) const;


  //@} // Member Functions that May Modify the Dimension of the Vector Space

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  /*! \brief
    Returns a 32-bit hash code for \p *this.

    If \p x and \p y are such that <CODE>x == y</CODE>,
    then <CODE>x.hash_code() == y.hash_code()</CODE>.
  */
  int32_t hash_code() const;

  friend bool operator==<T>(const BD_Shape<T>& x, const BD_Shape<T>& y);

  template <typename Temp, typename To, typename U>
  friend bool Parma_Polyhedra_Library::rectilinear_distance_assign
  (Checked_Number<To, Extended_Number_Policy>& r,
   const BD_Shape<U>& x, const BD_Shape<U>& y, const Rounding_Dir dir,
   Temp& tmp0, Temp& tmp1, Temp& tmp2);
  template <typename Temp, typename To, typename U>
  friend bool Parma_Polyhedra_Library::euclidean_distance_assign
  (Checked_Number<To, Extended_Number_Policy>& r,
   const BD_Shape<U>& x, const BD_Shape<U>& y, const Rounding_Dir dir,
   Temp& tmp0, Temp& tmp1, Temp& tmp2);
  template <typename Temp, typename To, typename U>
  friend bool Parma_Polyhedra_Library::l_infinity_distance_assign
  (Checked_Number<To, Extended_Number_Policy>& r,
   const BD_Shape<U>& x, const BD_Shape<U>& y, const Rounding_Dir dir,
   Temp& tmp0, Temp& tmp1, Temp& tmp2);

private:
  template <typename U> friend class Parma_Polyhedra_Library::BD_Shape;
  template <typename Interval> friend class Parma_Polyhedra_Library::Box;

  //! The matrix representing the system of bounded differences.
  DB_Matrix<N> dbm;

#define PPL_IN_BD_Shape_CLASS
#include "BDS_Status_idefs.hh"
#undef PPL_IN_BD_Shape_CLASS

  //! The status flags to keep track of the internal state.
  Status status;

  //! A matrix indicating which constraints are redundant.
  Bit_Matrix redundancy_dbm;

  //! Returns <CODE>true</CODE> if the BDS is the zero-dimensional universe.
  bool marked_zero_dim_univ() const;

  /*! \brief
    Returns <CODE>true</CODE> if the BDS is known to be empty.

    The return value <CODE>false</CODE> does not necessarily
    implies that \p *this is non-empty.
  */
  bool marked_empty() const;

  /*! \brief
    Returns <CODE>true</CODE> if the system of bounded differences
    is known to be shortest-path closed.

    The return value <CODE>false</CODE> does not necessarily
    implies that <CODE>this->dbm</CODE> is not shortest-path closed.
  */
  bool marked_shortest_path_closed() const;

  /*! \brief
    Returns <CODE>true</CODE> if the system of bounded differences
    is known to be shortest-path reduced.

    The return value <CODE>false</CODE> does not necessarily
    implies that <CODE>this->dbm</CODE> is not shortest-path reduced.
  */
  bool marked_shortest_path_reduced() const;

  //! Turns \p *this into an empty BDS.
  void set_empty();

  //! Turns \p *this into an zero-dimensional universe BDS.
  void set_zero_dim_univ();

  //! Marks \p *this as shortest-path closed.
  void set_shortest_path_closed();

  //! Marks \p *this as shortest-path closed.
  void set_shortest_path_reduced();

  //! Marks \p *this as possibly not shortest-path closed.
  void reset_shortest_path_closed();

  //! Marks \p *this as possibly not shortest-path reduced.
  void reset_shortest_path_reduced();

  //! Assigns to <CODE>this->dbm</CODE> its shortest-path closure.
  void shortest_path_closure_assign() const;

  /*! \brief
    Assigns to <CODE>this->dbm</CODE> its shortest-path closure and
    records into <CODE>this->redundancy_dbm</CODE> which of the entries
    in <CODE>this->dbm</CODE> are redundant.
  */
  void shortest_path_reduction_assign() const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if <CODE>this->dbm</CODE>
    is shortest-path closed and <CODE>this->redundancy_dbm</CODE>
    correctly flags the redundant entries in <CODE>this->dbm</CODE>.
  */
  bool is_shortest_path_reduced() const;

  /*! \brief
    Incrementally computes shortest-path closure, assuming that only
    constraints affecting variable \p var need to be considered.

    \note
    It is assumed that \c *this, which was shortest-path closed,
    has only been modified by adding constraints affecting variable
    \p var. If this assumption is not satisfied, i.e., if a non-redundant
    constraint not affecting variable \p var has been added, the behavior
    is undefined.
  */
  void incremental_shortest_path_closure_assign(Variable var) const;

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
    The linear expression to be maximized or minimized subject to \p
    *this;

    \param maximize
    <CODE>true</CODE> if maximization is what is wanted;

    \param ext_n
    The numerator of the extremum value;

    \param ext_d
    The denominator of the extremum value;

    \param included
    <CODE>true</CODE> if and only if the extremum of \p expr can
    actually be reached in \p * this;

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
    The linear expression to be maximized or minimized subject to \p
    *this;

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
    If the upper bound of \p *this and \p y is exact it is assigned
    to \p *this and \c true is returned, otherwise \c false is returned.

    Current implementation is based on a variant of Algorithm 4.1 in
      A. Bemporad, K. Fukuda, and F. D. Torrisi
      <em>Convexity Recognition of the Union of Polyhedra</em>
      Technical Report AUT00-13, ETH Zurich, 2000
    tailored to the special case of BD shapes.

    \note
    It is assumed that \p *this and \p y are dimension-compatible;
    if the assumption does not hold, the behavior is undefined.
  */
  bool BFT00_upper_bound_assign_if_exact(const BD_Shape& y);

  /*! \brief
    If the upper bound of \p *this and \p y is exact it is assigned
    to \p *this and \c true is returned, otherwise \c false is returned.

    Implementation for the rational (resp., integer) case is based on
    Theorem 5.2 (resp. Theorem 5.3) of \ref BHZ09b "[BHZ09b]".
    The Boolean template parameter \c integer_upper_bound allows for
    choosing between the rational and integer upper bound algorithms.

    \note
    It is assumed that \p *this and \p y are dimension-compatible;
    if the assumption does not hold, the behavior is undefined.

    \note
    The integer case is only enabled if T is an integer data type.
  */
  template <bool integer_upper_bound>
  bool BHZ09_upper_bound_assign_if_exact(const BD_Shape& y);

  /*! \brief
    Uses the constraint \p c to refine \p *this.

    \param c
    The constraint to be added. Non BD constraints are ignored.

    \warning
    If \p c and \p *this are dimension-incompatible,
    the behavior is undefined.
  */
  void refine_no_check(const Constraint& c);

  /*! \brief
    Uses the congruence \p cg to refine \p *this.

    \param cg
    The congruence to be added.
    Nontrivial proper congruences are ignored.
    Non BD equalities are ignored.

    \warning
    If \p cg and \p *this are dimension-incompatible,
    the behavior is undefined.
  */
  void refine_no_check(const Congruence& cg);

  //! Adds the constraint <CODE>dbm[i][j] \<= k</CODE>.
  void add_dbm_constraint(dimension_type i, dimension_type j, const N& k);

  //! Adds the constraint <CODE>dbm[i][j] \<= numer/denom</CODE>.
  void add_dbm_constraint(dimension_type i, dimension_type j,
                          Coefficient_traits::const_reference numer,
                          Coefficient_traits::const_reference denom);

  /*! \brief
    Adds to the BDS the constraint
    \f$\mathrm{var} \relsym \frac{\mathrm{expr}}{\mathrm{denominator}}\f$.

    Note that the coefficient of \p var in \p expr is null.
  */
  void refine(Variable var, Relation_Symbol relsym,
              const Linear_Expression& expr,
              Coefficient_traits::const_reference denominator
              = Coefficient_one());

  //! Removes all the constraints on row/column \p v.
  void forget_all_dbm_constraints(dimension_type v);
  //! Removes all binary constraints on row/column \p v.
  void forget_binary_dbm_constraints(dimension_type v);

  //! An helper function for the computation of affine relations.
  /*!
    For each dbm index \p u (less than or equal to \p last_v and different
    from \p v), deduce constraints of the form <CODE>v - u \<= c</CODE>,
    starting from \p ub_v which is an upper bound for \p v.

    The shortest-path closure is able to deduce the constraint
    <CODE>v - u \<= ub_v - lb_u</CODE>. We can be more precise if variable
    \p u played an active role in the computation of the upper bound for
    \p v, i.e., if the corresponding coefficient
    <CODE>q == sc_expr[u]/sc_denom</CODE> is greater than zero. In particular:
      - if <CODE>q \>= 1</CODE>, then <CODE>v - u \<= ub_v - ub_u</CODE>;
      - if <CODE>0 \< q \< 1</CODE>, then
        <CODE>v - u \<= ub_v - (q*ub_u + (1-q)*lb_u)</CODE>.
  */
  void deduce_v_minus_u_bounds(dimension_type v,
                               dimension_type last_v,
                               const Linear_Expression& sc_expr,
                               Coefficient_traits::const_reference sc_denom,
                               const N& ub_v);

  /*! \brief
    Auxiliary function for \ref affine_form_relation "affine form image" that
    handle the general case: \f$l = c\f$
  */
  template <typename Interval_Info>
  void inhomogeneous_affine_form_image(const dimension_type& var_id,
                                       const Interval<T, Interval_Info>& b);

  /*! \brief
    Auxiliary function for \ref affine_form_relation "affine form
    image" that handle the general case: \f$l = ax + c\f$
  */
  template <typename Interval_Info>
  void one_variable_affine_form_image
  (const dimension_type& var_id,
   const Interval<T, Interval_Info>& b,
   const Interval<T, Interval_Info>& w_coeff,
   const dimension_type& w_id,
   const dimension_type& space_dim);

  /*! \brief
    Auxiliary function for \ref affine_form_relation "affine form image" that
    handle the general case: \f$l = ax + by + c\f$
  */
  template <typename Interval_Info>
  void two_variables_affine_form_image
  (const dimension_type& var_id,
   const Linear_Form<Interval<T,Interval_Info> >& lf,
   const dimension_type& space_dim);

  /*! \brief
    Auxiliary function for refine with linear form that handle
    the general case: \f$l = ax + c\f$
  */
  template <typename Interval_Info>
  void left_inhomogeneous_refine
  (const dimension_type& right_t,
   const dimension_type& right_w_id,
   const Linear_Form<Interval<T, Interval_Info> >& left,
   const Linear_Form<Interval<T, Interval_Info> >& right);

  /*! \brief
    Auxiliary function for refine with linear form that handle
    the general case: \f$ax + b = cy + d\f$
  */
  template <typename Interval_Info>
  void left_one_var_refine
  (const dimension_type& left_w_id,
   const dimension_type& right_t,
   const dimension_type& right_w_id,
   const Linear_Form<Interval<T, Interval_Info> >& left,
   const Linear_Form<Interval<T, Interval_Info> >& right);

  /*! \brief
    Auxiliary function for refine with linear form that handle
    the general case.
  */
  template <typename Interval_Info>
  void general_refine(const dimension_type& left_w_id,
                      const dimension_type& right_w_id,
                      const Linear_Form<Interval<T, Interval_Info> >& left,
                      const Linear_Form<Interval<T, Interval_Info> >& right);

  template <typename Interval_Info>
  void linear_form_upper_bound(const Linear_Form<Interval<T, Interval_Info> >&
                               lf,
                               N& result) const;

  //! An helper function for the computation of affine relations.
  /*!
    For each dbm index \p u (less than or equal to \p last_v and different
    from \p v), deduce constraints of the form <CODE>u - v \<= c</CODE>,
    starting from \p minus_lb_v which is a lower bound for \p v.

    The shortest-path closure is able to deduce the constraint
    <CODE>u - v \<= ub_u - lb_v</CODE>. We can be more precise if variable
    \p u played an active role in the computation of the lower bound for
    \p v, i.e., if the corresponding coefficient
    <CODE>q == sc_expr[u]/sc_denom</CODE> is greater than zero.
    In particular:
      - if <CODE>q \>= 1</CODE>, then <CODE>u - v \<= lb_u - lb_v</CODE>;
      - if <CODE>0 \< q \< 1</CODE>, then
        <CODE>u - v \<= (q*lb_u + (1-q)*ub_u) - lb_v</CODE>.
  */
  void deduce_u_minus_v_bounds(dimension_type v,
                               dimension_type last_v,
                               const Linear_Expression& sc_expr,
                               Coefficient_traits::const_reference sc_denom,
                               const N& minus_lb_v);

  /*! \brief
    Adds to \p limiting_shape the bounded differences in \p cs
    that are satisfied by \p *this.
  */
  void get_limiting_shape(const Constraint_System& cs,
                          BD_Shape& limiting_shape) const;

  //! Compute the (zero-equivalence classes) predecessor relation.
  /*!
    It is assumed that the BDS is not empty and shortest-path closed.
  */
  void compute_predecessors(std::vector<dimension_type>& predecessor) const;

  //! Compute the leaders of zero-equivalence classes.
  /*!
    It is assumed that the BDS is not empty and shortest-path closed.
  */
  void compute_leaders(std::vector<dimension_type>& leaders) const;

  void drop_some_non_integer_points_helper(N& elem);

  friend std::ostream&
  Parma_Polyhedra_Library::IO_Operators
  ::operator<<<>(std::ostream& s, const BD_Shape<T>& c);

  //! \name Exception Throwers
  //@{
  void throw_dimension_incompatible(const char* method,
                                    const BD_Shape& y) const;

  void throw_dimension_incompatible(const char* method,
                                    dimension_type required_dim) const;

  void throw_dimension_incompatible(const char* method,
                                    const Constraint& c) const;

  void throw_dimension_incompatible(const char* method,
                                    const Congruence& cg) const;

  void throw_dimension_incompatible(const char* method,
                                    const Generator& g) const;

  void throw_dimension_incompatible(const char* method,
                                    const char* le_name,
                                    const Linear_Expression& le) const;

  template<typename Interval_Info>
  void
  throw_dimension_incompatible(const char* method,
                               const char* lf_name,
                               const Linear_Form<Interval<T, Interval_Info> >&
                               lf) const;

  static void throw_expression_too_complex(const char* method,
                                           const Linear_Expression& le);

  static void throw_invalid_argument(const char* method, const char* reason);
  //@} // Exception Throwers
};

#include "BDS_Status_inlines.hh"
#include "BD_Shape_inlines.hh"
#include "BD_Shape_templates.hh"

#endif // !defined(PPL_BD_Shape_defs_hh)
