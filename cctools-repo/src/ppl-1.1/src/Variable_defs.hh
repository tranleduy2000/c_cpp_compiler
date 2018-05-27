/* Variable class declaration.
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

#ifndef PPL_Variable_defs_hh
#define PPL_Variable_defs_hh 1

#include "Variable_types.hh"
#include "Init_types.hh"
#include "globals_types.hh"
#include <iosfwd>
#include <set>

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

//! Output operator.
/*! \relates Parma_Polyhedra_Library::Variable */
std::ostream&
operator<<(std::ostream& s, const Variable v);

} // namespace IO_Operators

//! Defines a total ordering on variables.
/*! \relates Variable */
bool less(Variable v, Variable w);

/*! \relates Variable */
void
swap(Variable& x, Variable& y);

} // namespace Parma_Polyhedra_Library

//! A dimension of the vector space.
/*! \ingroup PPL_CXX_interface
  An object of the class Variable represents a dimension of the space,
  that is one of the Cartesian axes.
  Variables are used as basic blocks in order to build
  more complex linear expressions.
  Each variable is identified by a non-negative integer,
  representing the index of the corresponding Cartesian axis
  (the first axis has index 0).
  The space dimension of a variable is the dimension of the vector space
  made by all the Cartesian axes having an index less than or equal to
  that of the considered variable; thus, if a variable has index \f$i\f$,
  its space dimension is \f$i+1\f$.

  Note that the ``meaning'' of an object of the class Variable
  is completely specified by the integer index provided to its
  constructor:
  be careful not to be mislead by C++ language variable names.
  For instance, in the following example the linear expressions
  <CODE>e1</CODE> and <CODE>e2</CODE> are equivalent,
  since the two variables <CODE>x</CODE> and <CODE>z</CODE> denote
  the same Cartesian axis.
  \code
  Variable x(0);
  Variable y(1);
  Variable z(0);
  Linear_Expression e1 = x + y;
  Linear_Expression e2 = y + z;
  \endcode

*/
class Parma_Polyhedra_Library::Variable {

public:
  //! Builds the variable corresponding to the Cartesian axis of index \p i.
  /*!
    \exception std::length_error
    Thrown if <CODE>i+1</CODE> exceeds
    <CODE>Variable::max_space_dimension()</CODE>.
  */
  explicit Variable(dimension_type i);

  //! Returns the index of the Cartesian axis associated to the variable.
  dimension_type id() const;

  //! Returns the maximum space dimension a Variable can handle.
  static dimension_type max_space_dimension();

  //! Returns the dimension of the vector space enclosing \p *this.
  /*!
    The returned value is <CODE>id()+1</CODE>.
  */
  dimension_type space_dimension() const;

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

  //! Type of output functions.
  typedef void output_function_type(std::ostream& s, const Variable v);

  //! The default output function.
  static void default_output_function(std::ostream& s, const Variable v);

  //! Sets the output function to be used for printing Variable objects.
  static void set_output_function(output_function_type* p);

  //! Returns the pointer to the current output function.
  static output_function_type* get_output_function();

  //! Binary predicate defining the total ordering on variables.
  /*! \ingroup PPL_CXX_interface */
  struct Compare {
    //! Returns <CODE>true</CODE> if and only if \p x comes before \p y.
    bool operator()(Variable x, Variable y) const;
  };

  //! Swaps *this and v.
  void m_swap(Variable& v);

private:
  //! The index of the Cartesian axis.
  dimension_type varid;

  // The initialization class needs to set the default output function.
  friend class Init;

  friend std::ostream&
  Parma_Polyhedra_Library::IO_Operators::operator<<(std::ostream& s,
                                                    const Variable v);

  //! Pointer to the current output function.
  static output_function_type* current_output_function;

};

#include "Variable_inlines.hh"

#endif // !defined(PPL_Variable_defs_hh)
