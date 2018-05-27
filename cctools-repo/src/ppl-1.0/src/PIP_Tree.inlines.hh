/* PIP_Tree related class implementation: inline functions.
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

#ifndef PPL_PIP_Tree_inlines_hh
#define PPL_PIP_Tree_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline
PIP_Solution_Node::Tableau::Tableau()
  : s(), t(), denom(1) {
  PPL_ASSERT(OK());
}

inline
PIP_Solution_Node::Tableau::Tableau(const Tableau& y)
  : s(y.s), t(y.t), denom(y.denom) {
  PPL_ASSERT(OK());
}

inline
PIP_Solution_Node::Tableau::~Tableau() {
}

inline bool
PIP_Solution_Node::Tableau::is_integer() const {
  return denom == 1;
}

inline Coefficient_traits::const_reference
PIP_Solution_Node::Tableau::denominator() const {
  return denom;
}

inline
PIP_Tree_Node::~PIP_Tree_Node() {
}

inline void
PIP_Tree_Node::set_parent(const PIP_Decision_Node* p) {
  parent_ = p;
}

inline const PIP_Decision_Node*
PIP_Tree_Node::parent() const {
  return parent_;
}

inline const PIP_Problem*
PIP_Tree_Node::get_owner() const {
  return owner_;
}

inline const Constraint_System&
PIP_Tree_Node::constraints() const {
  return constraints_;
}

inline PIP_Tree_Node::Artificial_Parameter_Sequence::const_iterator
PIP_Tree_Node::art_parameter_begin() const {
  return artificial_parameters.begin();
}

inline PIP_Tree_Node::Artificial_Parameter_Sequence::const_iterator
PIP_Tree_Node::art_parameter_end() const {
  return artificial_parameters.end();
}

inline dimension_type
PIP_Tree_Node::art_parameter_count() const {
  return artificial_parameters.size();
}

inline
const PIP_Tree_Node*
PIP_Decision_Node::child_node(bool b) const {
  return b ? true_child : false_child;
}

inline
PIP_Tree_Node*
PIP_Decision_Node::child_node(bool b) {
  return b ? true_child : false_child;
}

inline
PIP_Tree_Node::Artificial_Parameter::Artificial_Parameter()
  : Linear_Expression(), denom(1) {
  PPL_ASSERT(OK());
}

inline
PIP_Tree_Node::Artificial_Parameter
::Artificial_Parameter(const Artificial_Parameter& y)
  : Linear_Expression(y), denom(y.denom) {
  PPL_ASSERT(OK());
}

inline Coefficient_traits::const_reference
PIP_Tree_Node::Artificial_Parameter::denominator() const {
  return denom;
}

inline void
PIP_Tree_Node::Artificial_Parameter::m_swap(Artificial_Parameter& y) {
  Linear_Expression::m_swap(y);
  using std::swap;
  swap(denom, y.denom);
}

/*! \relates PIP_Tree_Node::Artificial_Parameter */
inline void
swap(PIP_Tree_Node::Artificial_Parameter& x,
     PIP_Tree_Node::Artificial_Parameter& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_PIP_Tree_inlines_hh)
