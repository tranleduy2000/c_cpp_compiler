/* Implementation of the C interface: inline functions.
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

#ifndef PPL_ppl_c_implementation_common_inlines_hh
#define PPL_ppl_c_implementation_common_inlines_hh 1

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace C {

// FIXME: this same function is used also in the OCaml interface.
// It should be placed in some common header file in the interfaces
// directory.
//! Reinterpret an mpz_t as mpz_class.
inline mpz_class&
reinterpret_mpz_class(mpz_t n) {
  return reinterpret_cast<mpz_class&>(*n);
}

DECLARE_CONVERSIONS(Coefficient, Coefficient)

DECLARE_CONVERSIONS(Linear_Expression, Linear_Expression)

DECLARE_CONVERSIONS(Constraint, Constraint)

DECLARE_CONVERSIONS(Constraint_System, Constraint_System)

DECLARE_CONVERSIONS(Constraint_System_const_iterator,
                    Constraint_System_const_iterator)

DECLARE_CONVERSIONS(Generator, Generator)

DECLARE_CONVERSIONS(Generator_System, Generator_System)

DECLARE_CONVERSIONS(Generator_System_const_iterator,
                    Generator_System_const_iterator)

DECLARE_CONVERSIONS(Congruence, Congruence)

DECLARE_CONVERSIONS(Congruence_System, Congruence_System)

typedef Congruence_System::const_iterator Congruence_System_const_iterator;
DECLARE_CONVERSIONS(Congruence_System_const_iterator,
                    Congruence_System_const_iterator)

DECLARE_CONVERSIONS(Grid_Generator, Grid_Generator)

DECLARE_CONVERSIONS(Grid_Generator_System, Grid_Generator_System)

typedef Grid_Generator_System::const_iterator
Grid_Generator_System_const_iterator;
DECLARE_CONVERSIONS(Grid_Generator_System_const_iterator,
                    Grid_Generator_System_const_iterator)

typedef PIP_Tree_Node::Artificial_Parameter
Artificial_Parameter;
DECLARE_CONVERSIONS(Artificial_Parameter, Artificial_Parameter)

typedef PIP_Tree_Node::Artificial_Parameter_Sequence
Artificial_Parameter_Sequence;
DECLARE_CONVERSIONS(Artificial_Parameter_Sequence,
                    Artificial_Parameter_Sequence)

typedef PIP_Tree_Node::Artificial_Parameter_Sequence::const_iterator
Artificial_Parameter_Sequence_const_iterator;
DECLARE_CONVERSIONS(Artificial_Parameter_Sequence_const_iterator,
                    Artificial_Parameter_Sequence_const_iterator)

DECLARE_CONVERSIONS(MIP_Problem, MIP_Problem)

DECLARE_CONVERSIONS(PIP_Problem, PIP_Problem)

DECLARE_CONVERSIONS(PIP_Tree_Node, PIP_Tree_Node)

DECLARE_CONVERSIONS(PIP_Decision_Node, PIP_Decision_Node)

DECLARE_CONVERSIONS(PIP_Solution_Node, PIP_Solution_Node)

inline Relation_Symbol
relation_symbol(enum ppl_enum_Constraint_Type t) {
  switch (t) {
  case PPL_CONSTRAINT_TYPE_LESS_THAN:
    return LESS_THAN;
  case PPL_CONSTRAINT_TYPE_LESS_OR_EQUAL:
    return LESS_OR_EQUAL;
  case PPL_CONSTRAINT_TYPE_EQUAL:
    return EQUAL;
  case PPL_CONSTRAINT_TYPE_GREATER_OR_EQUAL:
    return GREATER_OR_EQUAL;
  case PPL_CONSTRAINT_TYPE_GREATER_THAN:
    return GREATER_THAN;
  default:
    return static_cast<Relation_Symbol>(t);
  }
}

inline Bounded_Integer_Type_Width
bounded_integer_type_width(enum ppl_enum_Bounded_Integer_Type_Width w) {
  switch (w) {
  case PPL_BITS_8:
    return BITS_8;
  case PPL_BITS_16:
    return BITS_16;
  case PPL_BITS_32:
    return BITS_32;
  case PPL_BITS_64:
    return BITS_64;
  case PPL_BITS_128:
    return BITS_128;
  default:
    return static_cast<Bounded_Integer_Type_Width>(w);
  }
}

inline Bounded_Integer_Type_Representation
bounded_integer_type_representation(enum ppl_enum_Bounded_Integer_Type_Representation r) {
  switch (r) {
  case PPL_UNSIGNED:
    return UNSIGNED;
  case PPL_SIGNED_2_COMPLEMENT:
    return SIGNED_2_COMPLEMENT;
  default:
    return static_cast<Bounded_Integer_Type_Representation>(r);
  }
}

inline Bounded_Integer_Type_Overflow
bounded_integer_type_overflow(enum ppl_enum_Bounded_Integer_Type_Overflow o) {
  switch (o) {
  case PPL_OVERFLOW_WRAPS:
    return OVERFLOW_WRAPS;
  case PPL_OVERFLOW_UNDEFINED:
    return OVERFLOW_UNDEFINED;
  case PPL_OVERFLOW_IMPOSSIBLE:
    return OVERFLOW_IMPOSSIBLE;
  default:
    return static_cast<Bounded_Integer_Type_Overflow>(o);
  }
}

inline
Array_Partial_Function_Wrapper
::Array_Partial_Function_Wrapper(dimension_type* v, size_t n)
  : vec(v), vec_size(n), max_in_codomain_(not_a_dimension()), empty(-1) {
}

inline bool
Array_Partial_Function_Wrapper::has_empty_codomain() const {
  if (empty < 0) {
    empty = 1;
    for (size_t i = vec_size; i-- > 0; )
      if (vec[i] != not_a_dimension()) {
        empty = 0;
        break;
      }
  }
  return (empty != 0);
}

inline dimension_type
Array_Partial_Function_Wrapper::max_in_codomain() const {
  if (max_in_codomain_ == not_a_dimension()) {
    for (size_t i = vec_size; i-- > 0; ) {
      dimension_type vec_i = vec[i];
      if (vec_i != not_a_dimension()
          && (max_in_codomain_ == not_a_dimension()
              || vec_i > max_in_codomain_))
        max_in_codomain_ = vec_i;
    }
  }
  return max_in_codomain_;
}

inline bool
Array_Partial_Function_Wrapper::maps(dimension_type i,
                                     dimension_type& j) const {
  if (i >= vec_size)
    return false;
  dimension_type vec_i = vec[i];
  if (vec_i == not_a_dimension())
    return false;
  j = vec_i;
  return true;
}

} // namespace C

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_ppl_c_implementation_common_inlines_hh)
