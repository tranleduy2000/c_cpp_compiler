/* Linear_Expression class implementation: inline functions.
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

#ifndef PPL_Linear_Expression_inlines_hh
#define PPL_Linear_Expression_inlines_hh 1

#include "Expression_Adapter_defs.hh"

namespace Parma_Polyhedra_Library {

inline Linear_Expression&
Linear_Expression::operator=(const Linear_Expression& e) {
  Linear_Expression tmp = e;
  swap(*this, tmp);
  return *this;
}

inline
Linear_Expression::~Linear_Expression() {
  delete impl;
}

inline Representation
Linear_Expression::representation() const {
  return impl->representation();
}

inline dimension_type
Linear_Expression::space_dimension() const {
  return impl->space_dimension();
}

inline void
Linear_Expression::set_space_dimension(dimension_type n) {
  impl->set_space_dimension(n);
}

inline Coefficient_traits::const_reference
Linear_Expression::coefficient(Variable v) const {
  return impl->coefficient(v);
}

inline void
Linear_Expression
::set_coefficient(Variable v, Coefficient_traits::const_reference n) {
  impl->set_coefficient(v, n);
}

inline Coefficient_traits::const_reference
Linear_Expression::inhomogeneous_term() const {
  return impl->inhomogeneous_term();
}

inline void
Linear_Expression
::set_inhomogeneous_term(Coefficient_traits::const_reference n) {
  impl->set_inhomogeneous_term(n);
}

inline void
Linear_Expression::swap_space_dimensions(Variable v1, Variable v2) {
  impl->swap_space_dimensions(v1, v2);
}

inline void
Linear_Expression::shift_space_dimensions(Variable v, dimension_type n) {
  impl->shift_space_dimensions(v, n);
}

inline bool
Linear_Expression::is_zero() const {
  return impl->is_zero();
}

inline bool
Linear_Expression::all_homogeneous_terms_are_zero() const {
  return impl->all_homogeneous_terms_are_zero();
}

inline const Linear_Expression&
Linear_Expression::zero() {
  PPL_ASSERT(zero_p != 0);
  return *zero_p;
}

inline memory_size_type
Linear_Expression::external_memory_in_bytes() const {
  return impl->total_memory_in_bytes();
}

inline memory_size_type
Linear_Expression::total_memory_in_bytes() const {
  return external_memory_in_bytes() + sizeof(*this);
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator+(const Linear_Expression& e) {
  return e;
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator+(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  Linear_Expression x = e;
  x += n;
  return x;
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator+(const Linear_Expression& e, const Variable v) {
  Linear_Expression x = e;
  x += v;
  return x;
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator-(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  Linear_Expression x = e;
  x -= n;
  return x;
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator-(const Variable v, const Variable w) {
  const dimension_type v_space_dim = v.space_dimension();
  const dimension_type w_space_dim = w.space_dimension();
  const dimension_type space_dim = std::max(v_space_dim, w_space_dim);
  if (space_dim > Linear_Expression::max_space_dimension())
    throw std::length_error("Linear_Expression "
                            "PPL::operator+(v, w):\n"
                            "v or w exceed the maximum allowed "
                            "space dimension.");
  if (v_space_dim >= w_space_dim) {
    Linear_Expression e(v);
    e -= w;
    return e;
  }
  else {
    Linear_Expression e(w.space_dimension(), true);
    e -= w;
    e += v;
    return e;
  }
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator*(const Linear_Expression& e, Coefficient_traits::const_reference n) {
  Linear_Expression x = e;
  x *= n;
  return x;
}

/*! \relates Linear_Expression */
inline Linear_Expression&
operator+=(Linear_Expression& e, Coefficient_traits::const_reference n) {
  *e.impl += n;
  return e;
}

/*! \relates Linear_Expression */
inline Linear_Expression&
operator-=(Linear_Expression& e, Coefficient_traits::const_reference n) {
  *e.impl -= n;
  return e;
}

inline void
Linear_Expression::m_swap(Linear_Expression& y) {
  using std::swap;
  swap(impl, y.impl);
}

inline void
Linear_Expression::normalize() {
  impl->normalize();
}

inline void
Linear_Expression::ascii_dump(std::ostream& s) const {
  impl->ascii_dump(s);
}

inline bool
Linear_Expression::ascii_load(std::istream& s) {
  return impl->ascii_load(s);
}

inline void
Linear_Expression::remove_space_dimensions(const Variables_Set& vars) {
  impl->remove_space_dimensions(vars);
}

inline void
Linear_Expression::permute_space_dimensions(const std::vector<Variable>& cycle) {
  impl->permute_space_dimensions(cycle);
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression
operator+(const Linear_Expression& e1, const Linear_Expression& e2) {
  if (e1.space_dimension() >= e2.space_dimension()) {
    Linear_Expression e = e1;
    e += e2;
    return e;
  }
  else {
    Linear_Expression e = e2;
    e += e1;
    return e;
  }
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator+(const Variable v, const Linear_Expression& e) {
  return e + v;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression
operator+(Coefficient_traits::const_reference n,
               const Linear_Expression& e) {
  return e + n;
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator+(const Variable v, const Variable w) {
  const dimension_type v_space_dim = v.space_dimension();
  const dimension_type w_space_dim = w.space_dimension();
  const dimension_type space_dim = std::max(v_space_dim, w_space_dim);
  if (space_dim > Linear_Expression::max_space_dimension())
    throw std::length_error("Linear_Expression "
                            "PPL::operator+(v, w):\n"
                            "v or w exceed the maximum allowed "
                            "space dimension.");
  if (v_space_dim >= w_space_dim) {
    Linear_Expression e(v);
    e += w;
    return e;
  }
  else {
    Linear_Expression e(w);
    e += v;
    return e;
  }
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression
operator-(const Linear_Expression& e) {
  Linear_Expression r(e);
  neg_assign(r);
  return r;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression
operator-(const Linear_Expression& e1, const Linear_Expression& e2) {
  if (e1.space_dimension() >= e2.space_dimension()) {
    Linear_Expression e = e1;
    e -= e2;
    return e;
  }
  else {
    Linear_Expression e = e2;
    neg_assign(e);
    e += e1;
    return e;
  }
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression
operator-(const Variable v, const Linear_Expression& e) {
  Linear_Expression result(e, std::max(v.space_dimension(), e.space_dimension()));
  result.negate(0, e.space_dimension() + 1);
  result += v;
  return result;
}

/*! \relates Linear_Expression */
inline Linear_Expression
operator-(const Linear_Expression& e, const Variable v) {
  Linear_Expression result(e, std::max(v.space_dimension(), e.space_dimension()));
  result -= v;
  return result;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression
operator-(Coefficient_traits::const_reference n,
               const Linear_Expression& e) {
  Linear_Expression result(e);
  neg_assign(result);
  result += n;
  return result;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression
operator*(Coefficient_traits::const_reference n,
               const Linear_Expression& e) {
  return e * n;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression&
operator+=(Linear_Expression& e1, const Linear_Expression& e2) {
  *e1.impl += *e2.impl;
  return e1;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression&
operator+=(Linear_Expression& e, const Variable v) {
  *e.impl += v;
  return e;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression&
operator-=(Linear_Expression& e1, const Linear_Expression& e2) {
  *e1.impl -= *e2.impl;
  return e1;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression&
operator-=(Linear_Expression& e, const Variable v) {
  *e.impl -= v;
  return e;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression&
operator*=(Linear_Expression& e, Coefficient_traits::const_reference n) {
  *e.impl *= n;
  return e;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression&
operator/=(Linear_Expression& e, Coefficient_traits::const_reference n) {
  *e.impl /= n;
  return e;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline void
neg_assign(Linear_Expression& e) {
  e.impl->negate();
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression&
add_mul_assign(Linear_Expression& e,
               Coefficient_traits::const_reference n,
               const Variable v) {
  e.impl->add_mul_assign(n, v);
  return e;
}

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline Linear_Expression&
sub_mul_assign(Linear_Expression& e,
                    Coefficient_traits::const_reference n,
                    const Variable v) {
  e.impl->sub_mul_assign(n, v);
  return e;
}

inline void
add_mul_assign(Linear_Expression& e1,
               Coefficient_traits::const_reference factor,
               const Linear_Expression& e2) {
  e1.impl->add_mul_assign(factor, *e2.impl);
}

inline void
sub_mul_assign(Linear_Expression& e1,
                    Coefficient_traits::const_reference factor,
                    const Linear_Expression& e2) {
  e1.impl->sub_mul_assign(factor, *e2.impl);
}

inline Coefficient_traits::const_reference
Linear_Expression::get(dimension_type i) const {
  return impl->get(i);
}

inline void
Linear_Expression::set(dimension_type i,
                       Coefficient_traits::const_reference n) {
  impl->set(i, n);
}

inline Coefficient_traits::const_reference
Linear_Expression::get(Variable v) const {
  return impl->get(v.space_dimension());
}

inline void
Linear_Expression::set(Variable v,
                       Coefficient_traits::const_reference n) {
  impl->set(v.space_dimension(), n);
}

inline bool
Linear_Expression::all_zeroes(dimension_type start, dimension_type end) const {
  return impl->all_zeroes(start, end);
}

inline dimension_type
Linear_Expression::num_zeroes(dimension_type start, dimension_type end) const {
  return impl->num_zeroes(start, end);
}

inline Coefficient
Linear_Expression::gcd(dimension_type start, dimension_type end) const {
  return impl->gcd(start, end);
}

inline void
Linear_Expression
::exact_div_assign(Coefficient_traits::const_reference c,
                   dimension_type start, dimension_type end) {
  impl->exact_div_assign(c, start, end);
}

inline void
Linear_Expression
::mul_assign(Coefficient_traits::const_reference c,
             dimension_type start, dimension_type end) {
  impl->mul_assign(c, start, end);
}

inline void
Linear_Expression::sign_normalize() {
  impl->sign_normalize();
}

inline void
Linear_Expression::negate(dimension_type first, dimension_type last) {
  impl->negate(first, last);
}

inline bool
Linear_Expression::all_zeroes(const Variables_Set& vars) const {
  return impl->all_zeroes(vars);
}

inline bool
Linear_Expression::all_zeroes_except(const Variables_Set& vars,
                                     dimension_type start,
                                     dimension_type end) const {
  return impl->all_zeroes_except(vars, start, end);
}

inline dimension_type
Linear_Expression::last_nonzero() const {
  return impl->last_nonzero();
}

inline void
Linear_Expression
::scalar_product_assign(Coefficient& result, const Linear_Expression& y) const {
  scalar_product_assign(result, y, 0, space_dimension() + 1);
}

inline void
Linear_Expression
::scalar_product_assign(Coefficient& result, const Linear_Expression& y,
                        dimension_type start, dimension_type end) const {
  impl->scalar_product_assign(result, *(y.impl), start, end);
}

inline int
Linear_Expression
::scalar_product_sign(const Linear_Expression& y) const {
  return scalar_product_sign(y, 0, space_dimension() + 1);
}

inline int
Linear_Expression
::scalar_product_sign(const Linear_Expression& y,
                      dimension_type start, dimension_type end) const {
  return impl->scalar_product_sign(*(y.impl), start, end);
}

inline dimension_type
Linear_Expression
::first_nonzero(dimension_type first, dimension_type last) const {
  return impl->first_nonzero(first, last);
}

inline dimension_type
Linear_Expression
::last_nonzero(dimension_type first, dimension_type last) const {
  return impl->last_nonzero(first, last);
}

inline void
Linear_Expression
::has_a_free_dimension_helper(std::set<dimension_type>& x) const {
  return impl->has_a_free_dimension_helper(x);
}

inline bool
Linear_Expression
::is_equal_to(const Linear_Expression& x,
              dimension_type start, dimension_type end) const {
  return impl->is_equal_to(*(x.impl), start, end);
}

inline bool
Linear_Expression
::is_equal_to(const Linear_Expression& x,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  return impl->is_equal_to(*(x.impl), c1, c2, start, end);
}

inline void
Linear_Expression
::get_row(Dense_Row& row) const {
  return impl->get_row(row);
}

inline void
Linear_Expression
::get_row(Sparse_Row& row) const {
  return impl->get_row(row);
}

inline void
Linear_Expression
::linear_combine(const Linear_Expression& y, dimension_type i) {
  impl->linear_combine(*y.impl, i);
}

inline void
Linear_Expression
::linear_combine(const Linear_Expression& y,
                 Coefficient_traits::const_reference c1,
                 Coefficient_traits::const_reference c2) {
  impl->linear_combine(*y.impl, c1, c2);
}

inline void
Linear_Expression
::linear_combine_lax(const Linear_Expression& y,
                     Coefficient_traits::const_reference c1,
                     Coefficient_traits::const_reference c2) {
  impl->linear_combine_lax(*y.impl, c1, c2);
}

inline int
compare(const Linear_Expression& x, const Linear_Expression& y) {
  return x.impl->compare(*y.impl);
}

inline bool
Linear_Expression::is_equal_to(const Linear_Expression& x) const {
  return impl->is_equal_to(*x.impl);
}

inline void
Linear_Expression::linear_combine(const Linear_Expression& y,
                                  Coefficient_traits::const_reference c1,
                                  Coefficient_traits::const_reference c2,
                                  dimension_type start,
                                  dimension_type end) {
  impl->linear_combine(*y.impl, c1, c2, start, end);
}

inline void
Linear_Expression::linear_combine_lax(const Linear_Expression& y,
                                      Coefficient_traits::const_reference c1,
                                      Coefficient_traits::const_reference c2,
                                      dimension_type start,
                                      dimension_type end) {
  impl->linear_combine_lax(*y.impl, c1, c2, start, end);
}

inline bool
Linear_Expression
::have_a_common_variable(const Linear_Expression& x,
                         Variable first, Variable last) const {
  return impl->have_a_common_variable(*(x.impl), first, last);
}

inline
Linear_Expression::const_iterator
::const_iterator()
  : itr(NULL) {
}

inline
Linear_Expression::const_iterator
::const_iterator(const const_iterator& x)
  : itr(x.itr->clone()) {
}

inline
Linear_Expression::const_iterator
::~const_iterator() {
  // Note that this does nothing if itr==NULL.
  delete itr;
}

inline void
Linear_Expression::const_iterator::m_swap(const_iterator& x) {
  using std::swap;
  swap(itr, x.itr);
}

inline Linear_Expression::const_iterator&
Linear_Expression::const_iterator
::operator=(const const_iterator& itr) {
  const_iterator tmp = itr;
  using std::swap;
  swap(*this, tmp);
  return *this;
}

inline Linear_Expression::const_iterator&
Linear_Expression::const_iterator
::operator++() {
  PPL_ASSERT(itr != NULL);
  ++(*itr);
  return *this;
}

inline Linear_Expression::const_iterator&
Linear_Expression::const_iterator
::operator--() {
  PPL_ASSERT(itr != NULL);
  --(*itr);
  return *this;
}

inline Linear_Expression::const_iterator::reference
Linear_Expression::const_iterator
::operator*() const {
  PPL_ASSERT(itr != NULL);
  return *(*itr);
}

inline Variable
Linear_Expression::const_iterator
::variable() const {
  PPL_ASSERT(itr != NULL);
  return itr->variable();
}

inline bool
Linear_Expression::const_iterator
::operator==(const const_iterator& x) const {
  PPL_ASSERT(itr != NULL);
  PPL_ASSERT(x.itr != NULL);
  return *itr == *(x.itr);
}

inline bool
Linear_Expression::const_iterator
::operator!=(const const_iterator& x) const {
  return !(*this == x);
}

inline
Linear_Expression::const_iterator
::const_iterator(Linear_Expression_Interface::const_iterator_interface* itr)
  : itr(itr) {
  PPL_ASSERT(itr != NULL);
}

inline Linear_Expression::const_iterator
Linear_Expression
::begin() const {
  return const_iterator(impl->begin());
}

inline Linear_Expression::const_iterator
Linear_Expression
::end() const {
  return const_iterator(impl->end());
}

inline Linear_Expression::const_iterator
Linear_Expression
::lower_bound(Variable v) const {
  return const_iterator(impl->lower_bound(v));
}

template <typename LE_Adapter>
inline
Linear_Expression::Linear_Expression(const LE_Adapter& e,
                                     typename Enable_If<Is_Same_Or_Derived<Expression_Adapter_Base, LE_Adapter>::value, void*>::type)
  : impl(NULL) {
  Linear_Expression tmp(e.representation());
  tmp.set_space_dimension(e.space_dimension());
  tmp.set_inhomogeneous_term(e.inhomogeneous_term());
  for (typename LE_Adapter::const_iterator i = e.begin(),
         i_end = e.end(); i != i_end; ++i)
    add_mul_assign(tmp, *i, i.variable());
  using std::swap;
  swap(impl, tmp.impl);
}

template <typename LE_Adapter>
inline
Linear_Expression::Linear_Expression(const LE_Adapter& e,
                                     Representation r,
                                     typename Enable_If<Is_Same_Or_Derived<Expression_Adapter_Base, LE_Adapter>::value, void*>::type)
  : impl(NULL) {
  Linear_Expression tmp(r);
  tmp.set_space_dimension(e.space_dimension());
  tmp.set_inhomogeneous_term(e.inhomogeneous_term());
  for (typename LE_Adapter::const_iterator i = e.begin(),
         i_end = e.end(); i != i_end; ++i)
    add_mul_assign(tmp, *i, i.variable());
  using std::swap;
  swap(impl, tmp.impl);
}

template <typename LE_Adapter>
inline
Linear_Expression::Linear_Expression(const LE_Adapter& e,
                                     dimension_type space_dim,
                                     typename Enable_If<Is_Same_Or_Derived<Expression_Adapter_Base, LE_Adapter>::value, void*>::type)
  : impl(NULL) {
  Linear_Expression tmp(e.representation());
  tmp.set_space_dimension(space_dim);
  tmp.set_inhomogeneous_term(e.inhomogeneous_term());
  typedef typename LE_Adapter::const_iterator itr_t;
  itr_t i_end;
  if (space_dim <= e.space_dimension())
    i_end = e.lower_bound(Variable(space_dim));
  else
    i_end = e.end();
  for (itr_t i = e.begin(); i != i_end; ++i)
    add_mul_assign(tmp, *i, i.variable());
  using std::swap;
  swap(impl, tmp.impl);
}

template <typename LE_Adapter>
inline
Linear_Expression::Linear_Expression(const LE_Adapter& e,
                                     dimension_type space_dim,
                                     Representation r,
                                     typename Enable_If<Is_Same_Or_Derived<Expression_Adapter_Base, LE_Adapter>::value, void*>::type)
  : impl(NULL) {
  Linear_Expression tmp(r);
  tmp.set_space_dimension(space_dim);
  tmp.set_inhomogeneous_term(e.inhomogeneous_term());
  typedef typename LE_Adapter::const_iterator itr_t;
  itr_t i_end;
  if (space_dim <= e.space_dimension())
    i_end = e.lower_bound(Variable(space_dim));
  else
    i_end = e.end();
  for (itr_t i = e.begin(); i != i_end; ++i)
    add_mul_assign(tmp, *i, i.variable());
  using std::swap;
  swap(impl, tmp.impl);
}

namespace IO_Operators {

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline std::ostream&
operator<<(std::ostream& s, const Linear_Expression& e) {
  e.impl->print(s);
  return s;
}

} // namespace IO_Operators

/*! \relates Parma_Polyhedra_Library::Linear_Expression */
inline void
swap(Linear_Expression& x, Linear_Expression& y) {
  x.m_swap(y);
}

/*! \relates Linear_Expression::const_iterator */
inline void
swap(Linear_Expression::const_iterator& x,
     Linear_Expression::const_iterator& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Linear_Expression_inlines_hh)
