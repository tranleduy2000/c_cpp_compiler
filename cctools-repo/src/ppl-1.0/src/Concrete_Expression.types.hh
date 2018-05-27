/* Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

This file is free software; as a special exception the author gives
unlimited permission to copy and/or distribute it, with or without
modifications, as long as this notice is preserved.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. */

#ifndef PPL_Concrete_Expression_types_hh
#define PPL_Concrete_Expression_types_hh 1

namespace Parma_Polyhedra_Library {

/*
  NOTE: Doxygen seems to ignore documentation blocks attached to
  template class declarations that are not provided with a definition.
  This justifies (here below) the explicit use of Doxygen command \class.
*/

/*! \brief \class Parma_Polyhedra_Library::Concrete_Expression
  The base class of all concrete expressions.
*/
template <typename Target>
class Concrete_Expression;

/*! \brief \class Parma_Polyhedra_Library::Binary_Operator
  A binary operator applied to two concrete expressions.
*/
template <typename Target>
class Binary_Operator;

/*! \brief \class Parma_Polyhedra_Library::Unary_Operator
  A unary operator applied to one concrete expression.
*/
template <typename Target>
class Unary_Operator;

/*! \brief \class Parma_Polyhedra_Library::Cast_Operator
  A cast operator converting one concrete expression to some type.
*/
template <typename Target>
class Cast_Operator;

/*! \brief \class Parma_Polyhedra_Library::Integer_Constant
  An integer constant concrete expression.
*/
template <typename Target>
class Integer_Constant;

/*! \brief \class Parma_Polyhedra_Library::Floating_Point_Constant
  A floating-point constant concrete expression.
*/
template <typename Target>
class Floating_Point_Constant;

/*! \brief \class Parma_Polyhedra_Library::Approximable_Reference
  A concrete expression representing a reference to some approximable.
*/
template <typename Target>
class Approximable_Reference;

class Concrete_Expression_Type;

/*! \brief
  Encodes the kind of concrete expression.

  The values should be defined by the particular instance
  and uniquely identify one of: Binary_Operator, Unary_Operator,
  Cast_Operator, Integer_Constant, Floating_Point_Constant, or
  Approximable_Reference.  For example, the Binary_Operator kind
  integer constant should be defined by an instance as the member
  <CODE>Binary_Operator<T>::KIND</CODE>
*/
typedef int Concrete_Expression_Kind;

/*! \brief
  Encodes a binary operator of concrete expressions.

  The values should be uniquely defined by the particular instance and
  named: ADD, SUB, MUL, DIV, REM, BAND, BOR, BXOR, LSHIFT, RSHIFT.
*/
typedef int Concrete_Expression_BOP;

/*! \brief
  Encodes a unary operator of concrete expressions.

  The values should be uniquely defined by the particular instance and
  named: PLUS, MINUS, BNOT.
*/
typedef int Concrete_Expression_UOP;

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Concrete_Expression_types_hh)
