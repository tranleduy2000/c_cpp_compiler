/* Definitions for the C_Expr class and its subclasses: inline functions.
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

#ifndef PPL_C_Expr_inlines_hh
#define PPL_C_Expr_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline
Concrete_Expression<C_Expr>::
Concrete_Expression(const Concrete_Expression_Type type,
                    const C_Expr_Kind KIND)
  : expr_type(type),
    expr_kind(KIND) {
}

inline Concrete_Expression_Kind
Concrete_Expression<C_Expr>::kind() const {
  return expr_kind;
}

inline Concrete_Expression_Type
Concrete_Expression<C_Expr>::type() const {
  return expr_type;
}

inline
Binary_Operator<C_Expr>
::Binary_Operator(const Concrete_Expression_Type type,
                  const Concrete_Expression_BOP binary_operator,
                  const Concrete_Expression<C_Expr>* left_hand_side,
                  const Concrete_Expression<C_Expr>* right_hand_side)
  : Concrete_Expression<C_Expr>(type, BOP),
    bop(binary_operator),
    lhs(left_hand_side),
    rhs(right_hand_side) {
}

inline
Binary_Operator<C_Expr>::~Binary_Operator<C_Expr>() {
}

inline Concrete_Expression_Type
Binary_Operator<C_Expr>::type() const {
  return expr_type;
}

inline Concrete_Expression_BOP
Binary_Operator<C_Expr>::binary_operator() const {
  return bop;
}

inline const Concrete_Expression<C_Expr>*
Binary_Operator<C_Expr>::left_hand_side() const {
  return lhs;
}

inline const Concrete_Expression<C_Expr>*
Binary_Operator<C_Expr>::right_hand_side() const {
  return rhs;
}

inline
Unary_Operator<C_Expr>
::Unary_Operator(const Concrete_Expression_Type type,
                 const Concrete_Expression_UOP unary_operator,
                 const Concrete_Expression<C_Expr>* argument)
  : Concrete_Expression<C_Expr>(type, UOP),
    uop(unary_operator),
    arg(argument) {
}

inline
Unary_Operator<C_Expr>::~Unary_Operator<C_Expr>() {
}

inline Concrete_Expression_Type
Unary_Operator<C_Expr>::type() const {
  return expr_type;
}

inline Concrete_Expression_BOP
Unary_Operator<C_Expr>::unary_operator() const {
  return uop;
}

inline const Concrete_Expression<C_Expr>*
Unary_Operator<C_Expr>::argument() const {
  return arg;
}

inline
Cast_Operator<C_Expr>::
Cast_Operator(const Concrete_Expression_Type type,
              const Concrete_Expression<C_Expr>* ar)
  : Concrete_Expression<C_Expr>(type, CAST),
    arg(ar) {
}

inline
Cast_Operator<C_Expr>::~Cast_Operator<C_Expr>() {
}

inline Concrete_Expression_Type
Cast_Operator<C_Expr>::type() const {
  return expr_type;
}

inline const Concrete_Expression<C_Expr>*
Cast_Operator<C_Expr>::argument() const {
  return arg;
}

inline
Integer_Constant<C_Expr>::
Integer_Constant(Concrete_Expression_Type type,
                 const Integer_Interval& val)
  : Concrete_Expression<C_Expr>(type, INT_CON),
    value(val) {
}

inline
Integer_Constant<C_Expr>::~Integer_Constant<C_Expr>() {
}

inline
Floating_Point_Constant<C_Expr>::
Floating_Point_Constant(const char* value_string,
                        const unsigned int string_size)
  : Concrete_Expression<C_Expr>(Concrete_Expression_Type::floating_point(ANALYZED_FP_FORMAT), FP_CON),
    value(new char[string_size]) {
  strcpy(value, value_string);
}

inline
Floating_Point_Constant<C_Expr>::~Floating_Point_Constant<C_Expr>() {
  delete[] value;
}

inline Concrete_Expression_Type
Floating_Point_Constant<C_Expr>::type() const {
  return expr_type;
}

inline
Approximable_Reference<C_Expr>::
Approximable_Reference(Concrete_Expression_Type type,
		       const Integer_Interval& val,
                       dimension_type index)
  : Concrete_Expression<C_Expr>(type, APPROX_REF),
    value(val),
    dimensions() {
  dimensions.insert(index);
}

inline
Approximable_Reference<C_Expr>::~Approximable_Reference<C_Expr>() {
}

inline Concrete_Expression_Type
Approximable_Reference<C_Expr>::type() const {
  return expr_type;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_C_Expr_inlines_hh)
