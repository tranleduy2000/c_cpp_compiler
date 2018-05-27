/* Implementation of the C interface: declarations.
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

#ifndef PPL_ppl_c_implementation_common_defs_hh
#define PPL_ppl_c_implementation_common_defs_hh 1

#define PPL_NO_AUTOMATIC_INITIALIZATION
#include "ppl.hh"
#include "ppl_c.h"
#include <stdexcept>

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace C {

extern "C" typedef void
(*error_handler_type)(enum ppl_enum_error_code code, const char* description);

extern error_handler_type user_error_handler;

void notify_error(enum ppl_enum_error_code code, const char* description);

Relation_Symbol relation_symbol(enum ppl_enum_Constraint_Type t);

Bounded_Integer_Type_Width
bounded_integer_type_width(enum ppl_enum_Bounded_Integer_Type_Width w);

Bounded_Integer_Type_Representation
bounded_integer_type_representation(enum ppl_enum_Bounded_Integer_Type_Representation r);

/*! \brief
  A class to wrap an array of fixed length into a partial function interface
  suitable for the map_space_dimension() methods.
*/
class Array_Partial_Function_Wrapper {
public:
  /*! \brief
    Construct a partial function wrapping the first \p n positions of
    \p v.
  */
  Array_Partial_Function_Wrapper(dimension_type* v, size_t n);

  /*! \brief
    Returns <CODE>true</CODE> if and only if the represented partial
    function has an empty codomain (i.e., it is always undefined).
  */
  bool has_empty_codomain() const;

  /*! \brief
    Returns the maximum value that belongs to the codomain
    of the partial function.
  */
  dimension_type max_in_codomain() const;

  /*! \brief
    Assigns to \p j the value associated to \p i by \p *this, if any.

    Let \f$f\f$ be the function represented by \p *this and \f$k\f$ be
    the value of \p i.  If \f$f\f$ is defined in \f$k\f$, then
    \f$f(k)\f$ is assigned to \p j and <CODE>true</CODE> is returned.
    If \f$f\f$ is undefined in \f$k\f$, then <CODE>false</CODE> is
    returned.
  */
  bool maps(dimension_type i, dimension_type& j) const;

private:
  //! Holds the vector implementing the map.
  dimension_type* vec;

  //! Holds the size of \p vec.
  size_t vec_size;

  //! Cache for computing the maximum dimension in the codomain.
  mutable dimension_type max_in_codomain_;

  //! Cache for computing emptiness:
  //! -1 if we still don't know, 0 if not empty, 1 if empty.
  mutable int empty;
};

class timeout_exception : public Parma_Polyhedra_Library::Throwable {
public:
  void throw_me() const {
    throw *this;
  }
  int priority() const {
    return 0;
  }
};

void reset_timeout();

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

void reset_deterministic_timeout();

} // namespace C

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library


#define CATCH_STD_EXCEPTION(exception, code) \
catch (const std::exception& e) {	     \
  notify_error(code, e.what()); \
  return code; \
}

#define CATCH_ALL \
CATCH_STD_EXCEPTION(bad_alloc, PPL_ERROR_OUT_OF_MEMORY) \
CATCH_STD_EXCEPTION(invalid_argument, PPL_ERROR_INVALID_ARGUMENT) \
CATCH_STD_EXCEPTION(domain_error, PPL_ERROR_DOMAIN_ERROR) \
CATCH_STD_EXCEPTION(length_error, PPL_ERROR_LENGTH_ERROR) \
CATCH_STD_EXCEPTION(logic_error, PPL_ERROR_LOGIC_ERROR) \
CATCH_STD_EXCEPTION(overflow_error, PPL_ARITHMETIC_OVERFLOW) \
CATCH_STD_EXCEPTION(runtime_error, PPL_ERROR_INTERNAL_ERROR) \
CATCH_STD_EXCEPTION(exception, PPL_ERROR_UNKNOWN_STANDARD_EXCEPTION) \
catch (timeout_exception&) { \
  reset_timeout(); \
  notify_error(PPL_TIMEOUT_EXCEPTION, "PPL timeout expired"); \
  return PPL_TIMEOUT_EXCEPTION; \
} \
catch (deterministic_timeout_exception&) { \
  reset_deterministic_timeout(); \
  notify_error(PPL_TIMEOUT_EXCEPTION, "PPL deterministic timeout expired"); \
  return PPL_TIMEOUT_EXCEPTION; \
} \
catch (...) { \
  notify_error(PPL_ERROR_UNEXPECTED_ERROR, \
	       "completely unexpected error: a bug in the PPL"); \
  return PPL_ERROR_UNEXPECTED_ERROR; \
}

#define DECLARE_CONVERSIONS(Type, CPP_Type)             \
  inline const CPP_Type*                                \
  to_const(ppl_const_##Type##_t x) {                    \
    return reinterpret_cast<const CPP_Type*>(x);        \
  }                                                     \
                                                        \
  inline CPP_Type*                                      \
  to_nonconst(ppl_##Type##_t x) {                       \
    return reinterpret_cast<CPP_Type*>(x);              \
  }                                                     \
                                                        \
  inline ppl_const_##Type##_t                           \
  to_const(const CPP_Type* x) {                         \
    return reinterpret_cast<ppl_const_##Type##_t>(x);   \
  }                                                     \
                                                        \
  inline ppl_##Type##_t                                 \
  to_nonconst(CPP_Type* x) {                            \
    return reinterpret_cast<ppl_##Type##_t>(x);         \
  }

#define DEFINE_PRINT_FUNCTIONS(Type)                                    \
  int                                                                   \
  ppl_io_print_##Type(ppl_const_##Type##_t x) try {                     \
    using namespace IO_Operators;                                       \
    stdiobuf sb(stdout);                                                \
    std::ostream os(&sb);                                               \
    os << *to_const(x);                                                 \
    if (!os)                                                            \
      return PPL_STDIO_ERROR;                                           \
    return 0;                                                           \
  }                                                                     \
  CATCH_ALL                                                             \
                                                                        \
  int                                                                   \
  ppl_io_fprint_##Type(FILE* file, ppl_const_##Type##_t x) try {        \
    using namespace IO_Operators;                                       \
    stdiobuf sb(file);                                                  \
    std::ostream os(&sb);                                               \
    os << *to_const(x);                                                 \
    if (!os)                                                            \
      return PPL_STDIO_ERROR;                                           \
    return 0;                                                           \
  }                                                                     \
  CATCH_ALL                                                             \
                                                                        \
  int                                                                   \
  ppl_io_asprint_##Type(char** strp, ppl_const_##Type##_t x) try {      \
    using namespace IO_Operators;                                       \
    std::ostringstream os;                                              \
    os << *to_const(x);                                                 \
    if (!os)                                                            \
      return PPL_STDIO_ERROR;                                           \
    *strp = strdup(os.str().c_str());                                   \
    if (*strp == 0)                                                     \
      return PPL_ERROR_OUT_OF_MEMORY;                                   \
    return 0;                                                           \
  }                                                                     \
  CATCH_ALL

#define DEFINE_ASCII_DUMP_FUNCTIONS(Type)                               \
  int                                                                   \
  ppl_##Type##_ascii_dump(ppl_const_##Type##_t x, FILE* file) try {     \
    stdiobuf sb(file);                                                  \
    std::ostream os(&sb);                                               \
    to_const(x)->ascii_dump(os);                                        \
    if (!os)                                                            \
      return PPL_STDIO_ERROR;                                           \
    return 0;                                                           \
  }                                                                     \
  CATCH_ALL

#define DEFINE_ASCII_LOAD_FUNCTIONS(Type)                               \
  int                                                                   \
  ppl_##Type##_ascii_load(ppl_##Type##_t x, FILE* file) try {           \
    stdiobuf sb(file);                                                  \
    std::istream is(&sb);                                               \
    if (!to_nonconst(x)->ascii_load(is))                                \
      return PPL_STDIO_ERROR;                                           \
    return 0;                                                           \
  }                                                                     \
  CATCH_ALL

#define DEFINE_ASCII_DUMP_LOAD_FUNCTIONS(Type)  \
  DEFINE_ASCII_DUMP_FUNCTIONS(Type)             \
  DEFINE_ASCII_LOAD_FUNCTIONS(Type)

#define DEFINE_OUTPUT_FUNCTIONS(Type)           \
  DEFINE_PRINT_FUNCTIONS(Type)                  \
  DEFINE_ASCII_DUMP_LOAD_FUNCTIONS(Type)

#include "ppl_c_implementation_common.inlines.hh"

#endif // !defined(PPL_ppl_c_implementation_common_defs_hh)
