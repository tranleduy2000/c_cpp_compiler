/* Domain-independent part of the OCaml interface: declarations.
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

#ifndef PPL_ppl_ocaml_common_defs_hh
#define PPL_ppl_ocaml_common_defs_hh 1

#include "ppl.hh"
#include "interfaced_boxes.hh"
#include "marked_pointers.hh"

// Note: throw exception if the error is at the OCaml interface level.
#define PPL_OCAML_UNEXPECTED \
  throw std::runtime_error("PPL OCaml interface internal error")
#define PPL_OCAML_UNEXPECTED_MSG(msg) \
  throw std::runtime_error("PPL OCaml interface internal error:\n" msg)


// OCaml include files.
extern "C" {

#define CAML_NAME_SPACE
#include "caml/mlvalues.h"
#include "caml/memory.h"
#include "caml/custom.h"
#include "caml/fail.h"
#include "caml/callback.h"
#include "caml/alloc.h"
#undef CAML_NAME_SPACE

} // extern "C"

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace OCaml {

template <typename U_Int>
U_Int value_to_unsigned(value v);

value
ppl_dimension_to_value(dimension_type dim);

dimension_type
value_to_ppl_dimension(value dim);

Variable
build_ppl_Variable(value var);

Degenerate_Element
build_ppl_Degenerate_Element(value de);

Complexity_Class
build_ppl_Complexity_Class(value cc);

Relation_Symbol
build_ppl_relsym(value caml_relsym);

Bounded_Integer_Type_Overflow
build_ppl_bounded_integer_type_overflow(value caml_oflow);

Bounded_Integer_Type_Representation
build_ppl_bounded_integer_type_representation(value caml_rep);

Bounded_Integer_Type_Width
build_ppl_bounded_integer_type_width(value caml_width);

Coefficient
build_ppl_Coefficient(value coeff);

Variable
build_ppl_Variable(value caml_var);

Variables_Set
build_ppl_Variables_Set(value caml_vset);

Linear_Expression
build_ppl_Linear_Expression(value e);

Constraint
build_ppl_Constraint(value c);

Congruence
build_ppl_Congruence(value c);

Generator
build_ppl_Generator(value g);

Grid_Generator
build_ppl_Grid_Generator(value gg);

Constraint_System
build_ppl_Constraint_System(value cl);

Congruence_System
build_ppl_Congruence_System(value cgl);

Generator_System
build_ppl_Generator_System(value gl);

Grid_Generator_System
build_ppl_Grid_Generator_System(value ggs);

value
build_ocaml_poly_con_relation(Poly_Con_Relation& r);

value
build_ocaml_poly_gen_relation(Poly_Gen_Relation& r);

value
build_ocaml_coefficient(const Coefficient& c);

value
build_ocaml_constraint(const Constraint& c);

value
build_ocaml_congruence(const Congruence& cg);

value
build_ocaml_generator(const Generator& g);

value
build_ocaml_constraint_system(const Constraint_System& cs);

value
build_ocaml_congruence_system(const Congruence_System& cgs);

value
build_ocaml_grid_generator(const Grid_Generator& gg);

value
build_ocaml_generator_system(const Generator_System& gs);

value
build_ocaml_grid_generator_system(const Grid_Generator_System& ggs);

class timeout_exception
  : public Parma_Polyhedra_Library::Throwable {
public:
  void throw_me() const {
    throw *this;
  }
  int priority() const {
    return 0;
  }
};

class deterministic_timeout_exception
  : public Parma_Polyhedra_Library::Throwable {
public:
  void throw_me() const {
    throw *this;
  }
  int priority() const {
    return 0;
  }
};

void reset_timeout();

void reset_deterministic_timeout();

} // namespace OCaml

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

#define CATCH_ALL							\
catch(std::bad_alloc&) {						\
  caml_raise_out_of_memory();						\
}									\
catch(std::invalid_argument& e) {					\
  caml_invalid_argument(const_cast<char*>(e.what()));			\
}									\
catch(std::overflow_error& e) {					        \
  caml_raise_with_string(*caml_named_value("PPL_arithmetic_overflow"),  \
                         (const_cast<char*>(e.what())));		\
}									\
catch(std::domain_error& e) {					\
  caml_raise_with_string(*caml_named_value("PPL_domain_error"),  \
                         (const_cast<char*>(e.what())));		\
}									\
catch(std::length_error& e) {					\
  caml_raise_with_string(*caml_named_value("PPL_length_error"),  \
                         (const_cast<char*>(e.what())));		\
}									\
catch(std::logic_error& e) {						\
  caml_raise_with_string(*caml_named_value("PPL_logic_error"),		\
                         (const_cast<char*>(e.what())));		\
}									\
catch(std::runtime_error& e) {                                          \
  caml_raise_with_string(*caml_named_value("PPL_internal_error"),	\
                         (const_cast<char*>(e.what())));		\
}									\
catch(std::exception& e) {						\
  caml_raise_with_string(*caml_named_value("PPL_unknown_standard_exception"), \
                         (const_cast<char*>(e.what())));		\
}									\
catch(timeout_exception&) {                                             \
  reset_timeout();                                                      \
  caml_raise_constant(*caml_named_value("PPL_timeout_exception"));      \
}                                                                       \
catch(deterministic_timeout_exception&) {                               \
  reset_deterministic_timeout();                                        \
  caml_raise_constant(*caml_named_value("PPL_timeout_exception"));      \
}                                                                       \
catch(...) {								\
  caml_raise_constant(*caml_named_value("PPL_unexpected_error"));	\
}

#include "ppl_ocaml_common.inlines.hh"

#endif // !defined(PPL_ppl_prolog_common_defs_hh)
