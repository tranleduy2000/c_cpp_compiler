/* MIP_Problem class implementation: inline functions.
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

#ifndef PPL_MIP_Problem_inlines_hh
#define PPL_MIP_Problem_inlines_hh 1

#include "Constraint.defs.hh"
#include <stdexcept>

namespace Parma_Polyhedra_Library {

inline dimension_type
MIP_Problem::max_space_dimension() {
  return Constraint::max_space_dimension();
}

inline dimension_type
MIP_Problem::space_dimension() const {
  return external_space_dim;
}


inline
MIP_Problem::MIP_Problem(const MIP_Problem& y)
  : external_space_dim(y.external_space_dim),
    internal_space_dim(y.internal_space_dim),
    tableau(y.tableau),
    working_cost(y.working_cost),
    mapping(y.mapping),
    base(y.base),
    status(y.status),
    pricing(y.pricing),
    initialized(y.initialized),
    input_cs(),
    inherited_constraints(0),
    first_pending_constraint(),
    input_obj_function(y.input_obj_function),
    opt_mode(y.opt_mode),
    last_generator(y.last_generator),
    i_variables(y.i_variables) {
  input_cs.reserve(y.input_cs.size());
  for (Constraint_Sequence::const_iterator
       i = y.input_cs.begin(), i_end = y.input_cs.end();
       i != i_end; ++i)
    add_constraint_helper(*(*i));
  PPL_ASSERT(OK());
}

inline
MIP_Problem::MIP_Problem(const MIP_Problem& y, Inherit_Constraints)
  : external_space_dim(y.external_space_dim),
    internal_space_dim(y.internal_space_dim),
    tableau(y.tableau),
    working_cost(y.working_cost),
    mapping(y.mapping),
    base(y.base),
    status(y.status),
    pricing(y.pricing),
    initialized(y.initialized),
    input_cs(y.input_cs),
    // NOTE: The constraints are inherited, NOT copied!
    inherited_constraints(y.input_cs.size()),
    first_pending_constraint(y.first_pending_constraint),
    input_obj_function(y.input_obj_function),
    opt_mode(y.opt_mode),
    last_generator(y.last_generator),
    i_variables(y.i_variables) {
  PPL_ASSERT(OK());
}

inline void
MIP_Problem::add_constraint_helper(const Constraint& c) {
  // For exception safety, reserve space for the new element.
  const dimension_type size = input_cs.size();
  if (size == input_cs.capacity()) {
    const dimension_type max_size = input_cs.max_size();
    if (size == max_size)
      throw std::length_error("MIP_Problem::add_constraint(): "
                              "too many constraints");
    // Use an exponential grow policy to avoid too many reallocations.
    input_cs.reserve(compute_capacity(size + 1, max_size));
  }

  // This operation does not throw, because the space for the new element
  // has already been reserved: hence the new-ed Constraint is safe.
  input_cs.push_back(new Constraint(c));
}

inline
MIP_Problem::~MIP_Problem() {
  // NOTE: do NOT delete inherited constraints; they are owned
  // (and will eventually be deleted) by ancestors.
  for (Constraint_Sequence::const_iterator
         i = nth_iter(input_cs, inherited_constraints),
         i_end = input_cs.end(); i != i_end; ++i)
    delete *i;
}


inline void
MIP_Problem::set_optimization_mode(const Optimization_Mode mode) {
  if (opt_mode != mode) {
    opt_mode = mode;
    if (status == UNBOUNDED || status == OPTIMIZED)
      status = SATISFIABLE;
    PPL_ASSERT(OK());
  }
}

inline const Linear_Expression&
MIP_Problem::objective_function() const {
  return input_obj_function;
}

inline Optimization_Mode
MIP_Problem::optimization_mode() const {
  return opt_mode;
}

inline void
MIP_Problem::optimal_value(Coefficient& numer, Coefficient& denom) const {
  const Generator& g = optimizing_point();
  evaluate_objective_function(g, numer, denom);
}

inline MIP_Problem::const_iterator
MIP_Problem::constraints_begin() const {
  return const_iterator(input_cs.begin());
}

inline MIP_Problem::const_iterator
MIP_Problem::constraints_end() const {
  return const_iterator(input_cs.end());
}

inline const Variables_Set&
MIP_Problem::integer_space_dimensions() const {
  return i_variables;
}

inline MIP_Problem::Control_Parameter_Value
MIP_Problem::get_control_parameter(Control_Parameter_Name name) const {
  PPL_USED(name);
  PPL_ASSERT(name == PRICING);
  return pricing;
}

inline void
MIP_Problem::set_control_parameter(Control_Parameter_Value value) {
  pricing = value;
}

inline void
MIP_Problem::m_swap(MIP_Problem& y) {
  using std::swap;
  swap(external_space_dim, y.external_space_dim);
  swap(internal_space_dim, y.internal_space_dim);
  swap(tableau, y.tableau);
  swap(working_cost, y.working_cost);
  swap(mapping, y.mapping);
  swap(initialized, y.initialized);
  swap(base, y.base);
  swap(status, y.status);
  swap(pricing, y.pricing);
  swap(input_cs, y.input_cs);
  swap(inherited_constraints, y.inherited_constraints);
  swap(first_pending_constraint, y.first_pending_constraint);
  swap(input_obj_function, y.input_obj_function);
  swap(opt_mode, y.opt_mode);
  swap(last_generator, y.last_generator);
  swap(i_variables, y.i_variables);
}

inline MIP_Problem&
MIP_Problem::operator=(const MIP_Problem& y) {
  MIP_Problem tmp(y);
  m_swap(tmp);
  return *this;
}

inline void
MIP_Problem::clear() {
  MIP_Problem tmp;
  m_swap(tmp);
}


inline memory_size_type
MIP_Problem::external_memory_in_bytes() const {
  memory_size_type n
    = working_cost.external_memory_in_bytes()
    + tableau.external_memory_in_bytes()
    + input_obj_function.external_memory_in_bytes()
    + last_generator.external_memory_in_bytes();

  // Adding the external memory for `input_cs'.
  // NOTE: disregard inherited constraints, as they are owned by ancestors.
  n += input_cs.capacity() * sizeof(Constraint*);
  for (Constraint_Sequence::const_iterator
         i = nth_iter(input_cs, inherited_constraints),
         i_end = input_cs.end(); i != i_end; ++i)
    n += ((*i)->total_memory_in_bytes());

  // Adding the external memory for `base'.
  n += base.capacity() * sizeof(dimension_type);
  // Adding the external memory for `mapping'.
  n += mapping.capacity() * sizeof(std::pair<dimension_type, dimension_type>);
  return n;
}

inline memory_size_type
MIP_Problem::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline
MIP_Problem::const_iterator::const_iterator(Base base)
  : itr(base) {
}

inline MIP_Problem::const_iterator::difference_type
MIP_Problem::const_iterator::operator-(const const_iterator& y) const {
  return itr - y.itr;
}

inline MIP_Problem::const_iterator&
MIP_Problem::const_iterator::operator++() {
  ++itr;
  return *this;
}

inline MIP_Problem::const_iterator&
MIP_Problem::const_iterator::operator--() {
  --itr;
  return *this;
}

inline MIP_Problem::const_iterator
MIP_Problem::const_iterator::operator++(int) {
  const_iterator x = *this;
  operator++();
  return x;
}

inline MIP_Problem::const_iterator
MIP_Problem::const_iterator::operator--(int) {
  const_iterator x = *this;
  operator--();
  return x;
}

inline MIP_Problem::const_iterator
MIP_Problem::const_iterator::operator+(difference_type n) const {
  return const_iterator(itr + n);
}

inline MIP_Problem::const_iterator
MIP_Problem::const_iterator::operator-(difference_type n) const {
  return const_iterator(itr - n);
}

inline MIP_Problem::const_iterator&
MIP_Problem::const_iterator::operator+=(difference_type n) {
  itr += n;
  return *this;
}

inline MIP_Problem::const_iterator&
MIP_Problem::const_iterator::operator-=(difference_type n) {
  itr -= n;
  return *this;
}

inline MIP_Problem::const_iterator::reference
MIP_Problem::const_iterator::operator*() const {
  return *(*itr);
}

inline MIP_Problem::const_iterator::pointer
MIP_Problem::const_iterator::operator->() const {
  return *itr;
}

inline bool
MIP_Problem::const_iterator::operator==(const const_iterator& y) const {
  return itr == y.itr;
}

inline bool
MIP_Problem::const_iterator::operator!=(const const_iterator& y) const {
  return itr != y.itr;
}

/*! \relates MIP_Problem */
inline void
swap(MIP_Problem& x, MIP_Problem& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_MIP_Problem_inlines_hh)
