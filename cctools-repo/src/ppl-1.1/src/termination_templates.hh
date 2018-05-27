/* Utilities for termination analysis: template functions.
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

#ifndef PPL_termination_templates_hh
#define PPL_termination_templates_hh 1

#include "globals_defs.hh"
#include "Variable_defs.hh"
#include "Generator_defs.hh"
#include "Constraint_System_defs.hh"
#include "C_Polyhedron_defs.hh"
#include "NNC_Polyhedron_defs.hh"

#include <stdexcept>

#define PRINT_DEBUG_INFO 0

#if PRINT_DEBUG_INFO
#include <iostream>
#endif

namespace Parma_Polyhedra_Library {

namespace Implementation {

namespace Termination {

#if PRINT_DEBUG_INFO
static dimension_type output_function_MS_n;
static dimension_type output_function_MS_m;

/* Encodes which object are we printing:

   0 means input constraint system;
   1 means first output constraint system;
   2 means second output constraint system;
   3 means only output constraint system
     (i.e., when first and second are the same);
   4 means mu space.
*/
static int output_function_MS_which = -1;

/*
  Debugging output function.  See the documentation of
  fill_constraint_systems_MS() for the allocation of variable indices.
*/
inline void
output_function_MS(std::ostream& s, const Variable v) {
  dimension_type id = v.id();
  switch (output_function_MS_which) {
  case 0:
    if (id < output_function_MS_n)
      s << "x'" << id + 1;
    else if (id < 2*output_function_MS_n)
      s << "x" << id - output_function_MS_n + 1;
    else
      s << "WHAT?";
    break;
  case 1:
    if (id < output_function_MS_n)
      s << "mu" << id + 1;
    else if (id == output_function_MS_n)
      s << "WHAT?";
    else if (id <= output_function_MS_n + output_function_MS_m)
      s << "y" << id - output_function_MS_n;
    else
      s << "WHAT?";
    break;
  case 2:
  case 4:
    if (id < output_function_MS_n)
      s << "mu" << id + 1;
    else if (id == output_function_MS_n)
      s << "mu0";
    else if (output_function_MS_which == 2
             && id <= output_function_MS_n + output_function_MS_m + 2)
      s << "z" << id - output_function_MS_n;
    else
      s << "WHAT?";
    break;
  case 3:
    if (id < output_function_MS_n)
      s << "mu" << id + 1;
    else if (id == output_function_MS_n)
      s << "mu0";
    else if (id <= output_function_MS_n + output_function_MS_m)
      s << "y" << id - output_function_MS_n;
    else if (id <= output_function_MS_n + 2*output_function_MS_m + 2)
      s << "z" << id - (output_function_MS_n + output_function_MS_m);
    else
      s << "WHAT?";
    break;
  default:
    abort();
    break;
  }
}

static dimension_type output_function_PR_s;
static dimension_type output_function_PR_r;

/*
  Debugging output function.  See the documentation of
  fill_constraint_system_PR() for the allocation of variable indices.
*/
inline void
output_function_PR(std::ostream& s, const Variable v) {
  dimension_type id = v.id();
  if (id < output_function_PR_s)
    s << "u3_" << id + 1;
  else if (id < output_function_PR_s + output_function_PR_r)
    s << "u2_" << id - output_function_PR_s + 1;
  else if (id < output_function_PR_s + 2*output_function_PR_r)
    s << "u1_" << id - (output_function_PR_s + output_function_PR_r) + 1;
  else
    s << "WHAT?";
}
#endif

void
assign_all_inequalities_approximation(const Constraint_System& cs_in,
                                      Constraint_System& cs_out);

template <typename PSET>
inline void
assign_all_inequalities_approximation(const PSET& pset,
                                      Constraint_System& cs) {
  assign_all_inequalities_approximation(pset.minimized_constraints(), cs);
}

template <>
void
assign_all_inequalities_approximation(const C_Polyhedron& ph,
                                      Constraint_System& cs);

bool
termination_test_MS(const Constraint_System& cs);

bool
one_affine_ranking_function_MS(const Constraint_System& cs,
                               Generator& mu);

void
all_affine_ranking_functions_MS(const Constraint_System& cs,
                                C_Polyhedron& mu_space);

void
all_affine_quasi_ranking_functions_MS(const Constraint_System& cs,
                                      C_Polyhedron& decreasing_mu_space,
                                      C_Polyhedron& bounded_mu_space);

bool
termination_test_PR(const Constraint_System& cs_before,
                    const Constraint_System& cs_after);

bool
one_affine_ranking_function_PR(const Constraint_System& cs_before,
                               const Constraint_System& cs_after,
                               Generator& mu);

void
all_affine_ranking_functions_PR(const Constraint_System& cs_before,
                                const Constraint_System& cs_after,
                                NNC_Polyhedron& mu_space);

bool
termination_test_PR_original(const Constraint_System& cs);

bool
one_affine_ranking_function_PR_original(const Constraint_System& cs,
                                        Generator& mu);

void
all_affine_ranking_functions_PR_original(const Constraint_System& cs,
                                         NNC_Polyhedron& mu_space);

} // namespace Termination

} // namespace Implementation

template <typename PSET>
void
Termination_Helpers
::assign_all_inequalities_approximation(const PSET& pset_before,
                                        const PSET& pset_after,
                                        Constraint_System& cs) {
  Implementation::Termination
    ::assign_all_inequalities_approximation(pset_before, cs);
  cs.shift_space_dimensions(Variable(0), cs.space_dimension());
  Constraint_System cs_after;
  Implementation::Termination
    ::assign_all_inequalities_approximation(pset_after, cs_after);
  // FIXME: provide an "append" for constraint systems.
  for (Constraint_System::const_iterator i = cs_after.begin(),
         cs_after_end = cs_after.end(); i != cs_after_end; ++i)
    cs.insert(*i);
}

template <typename PSET>
bool
termination_test_MS(const PSET& pset) {
  const dimension_type space_dim = pset.space_dimension();
  if (space_dim % 2 != 0) {
    std::ostringstream s;
    s << "PPL::termination_test_MS(pset):\n"
         "pset.space_dimension() == " << space_dim
      << " is odd.";
    throw std::invalid_argument(s.str());
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  assign_all_inequalities_approximation(pset, cs);
  return termination_test_MS(cs);
}

template <typename PSET>
bool
termination_test_MS_2(const PSET& pset_before, const PSET& pset_after) {
  const dimension_type before_space_dim = pset_before.space_dimension();
  const dimension_type after_space_dim = pset_after.space_dimension();
  if (after_space_dim != 2*before_space_dim) {
    std::ostringstream s;
    s << "PPL::termination_test_MS_2(pset_before, pset_after):\n"
         "pset_before.space_dimension() == " << before_space_dim
      << ", pset_after.space_dimension() == " << after_space_dim
      << ";\nthe latter should be twice the former.";
    throw std::invalid_argument(s.str());
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  Termination_Helpers
    ::assign_all_inequalities_approximation(pset_before, pset_after, cs);
  return termination_test_MS(cs);
}

template <typename PSET>
bool
one_affine_ranking_function_MS(const PSET& pset, Generator& mu) {
  const dimension_type space_dim = pset.space_dimension();
  if (space_dim % 2 != 0) {
    std::ostringstream s;
    s << "PPL::one_affine_ranking_function_MS(pset, mu):\n"
         "pset.space_dimension() == " << space_dim
      << " is odd.";
    throw std::invalid_argument(s.str());
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  assign_all_inequalities_approximation(pset, cs);
  return one_affine_ranking_function_MS(cs, mu);
}

template <typename PSET>
bool
one_affine_ranking_function_MS_2(const PSET& pset_before,
                                 const PSET& pset_after,
                                 Generator& mu) {
  const dimension_type before_space_dim = pset_before.space_dimension();
  const dimension_type after_space_dim = pset_after.space_dimension();
  if (after_space_dim != 2*before_space_dim) {
    std::ostringstream s;
    s << "PPL::one_affine_ranking_function_MS_2(pset_before, pset_after, mu):\n"
         "pset_before.space_dimension() == " << before_space_dim
      << ", pset_after.space_dimension() == " << after_space_dim
      << ";\nthe latter should be twice the former.";
    throw std::invalid_argument(s.str());
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  Termination_Helpers
    ::assign_all_inequalities_approximation(pset_before, pset_after, cs);
  return one_affine_ranking_function_MS(cs, mu);
}

template <typename PSET>
void
all_affine_ranking_functions_MS(const PSET& pset, C_Polyhedron& mu_space) {
  const dimension_type space_dim = pset.space_dimension();
  if (space_dim % 2 != 0) {
    std::ostringstream s;
    s << "PPL::all_affine_ranking_functions_MS(pset, mu_space):\n"
         "pset.space_dimension() == " << space_dim
      << " is odd.";
    throw std::invalid_argument(s.str());
  }

  if (pset.is_empty()) {
    mu_space = C_Polyhedron(1 + space_dim/2, UNIVERSE);
    return;
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  assign_all_inequalities_approximation(pset, cs);
  all_affine_ranking_functions_MS(cs, mu_space);
}

template <typename PSET>
void
all_affine_ranking_functions_MS_2(const PSET& pset_before,
                                  const PSET& pset_after,
                                  C_Polyhedron& mu_space) {
  const dimension_type before_space_dim = pset_before.space_dimension();
  const dimension_type after_space_dim = pset_after.space_dimension();
  if (after_space_dim != 2*before_space_dim) {
    std::ostringstream s;
    s << "PPL::all_affine_ranking_functions_MS_2"
      << "(pset_before, pset_after, mu_space):\n"
      << "pset_before.space_dimension() == " << before_space_dim
      << ", pset_after.space_dimension() == " << after_space_dim
      << ";\nthe latter should be twice the former.";
    throw std::invalid_argument(s.str());
  }

  if (pset_before.is_empty()) {
    mu_space = C_Polyhedron(1 + before_space_dim, UNIVERSE);
    return;
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  Termination_Helpers
    ::assign_all_inequalities_approximation(pset_before, pset_after, cs);
  all_affine_ranking_functions_MS(cs, mu_space);
}

template <typename PSET>
void
all_affine_quasi_ranking_functions_MS(const PSET& pset,
                                      C_Polyhedron& decreasing_mu_space,
                                      C_Polyhedron& bounded_mu_space) {
  const dimension_type space_dim = pset.space_dimension();
  if (space_dim % 2 != 0) {
    std::ostringstream s;
    s << "PPL::all_affine_quasi_ranking_functions_MS"
      << "(pset, decr_space, bounded_space):\n"
      << "pset.space_dimension() == " << space_dim
      << " is odd.";
    throw std::invalid_argument(s.str());
  }

  if (pset.is_empty()) {
    decreasing_mu_space = C_Polyhedron(1 + space_dim/2, UNIVERSE);
    bounded_mu_space = decreasing_mu_space;
    return;
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  assign_all_inequalities_approximation(pset, cs);
  all_affine_quasi_ranking_functions_MS(cs,
                                        decreasing_mu_space,
                                        bounded_mu_space);
}

template <typename PSET>
void
all_affine_quasi_ranking_functions_MS_2(const PSET& pset_before,
                                        const PSET& pset_after,
                                        C_Polyhedron& decreasing_mu_space,
                                        C_Polyhedron& bounded_mu_space) {
  const dimension_type before_space_dim = pset_before.space_dimension();
  const dimension_type after_space_dim = pset_after.space_dimension();
  if (after_space_dim != 2*before_space_dim) {
    std::ostringstream s;
    s << "PPL::all_affine_quasi_ranking_functions_MS_2"
      << "(pset_before, pset_after, decr_space, bounded_space):\n"
      << "pset_before.space_dimension() == " << before_space_dim
      << ", pset_after.space_dimension() == " << after_space_dim
      << ";\nthe latter should be twice the former.";
    throw std::invalid_argument(s.str());
  }

  if (pset_before.is_empty()) {
    decreasing_mu_space = C_Polyhedron(1 + before_space_dim, UNIVERSE);
    bounded_mu_space = decreasing_mu_space;
    return;
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  Termination_Helpers
    ::assign_all_inequalities_approximation(pset_before, pset_after, cs);
  all_affine_quasi_ranking_functions_MS(cs,
                                        decreasing_mu_space,
                                        bounded_mu_space);
}

template <typename PSET>
bool
termination_test_PR_2(const PSET& pset_before, const PSET& pset_after) {
  const dimension_type before_space_dim = pset_before.space_dimension();
  const dimension_type after_space_dim = pset_after.space_dimension();
  if (after_space_dim != 2*before_space_dim) {
    std::ostringstream s;
    s << "PPL::termination_test_PR_2(pset_before, pset_after):\n"
      << "pset_before.space_dimension() == " << before_space_dim
      << ", pset_after.space_dimension() == " << after_space_dim
      << ";\nthe latter should be twice the former.";
    throw std::invalid_argument(s.str());
  }

  using namespace Implementation::Termination;
  Constraint_System cs_before;
  Constraint_System cs_after;
  assign_all_inequalities_approximation(pset_before, cs_before);
  assign_all_inequalities_approximation(pset_after, cs_after);
  return termination_test_PR(cs_before, cs_after);
}

template <typename PSET>
bool
termination_test_PR(const PSET& pset) {
  const dimension_type space_dim = pset.space_dimension();
  if (space_dim % 2 != 0) {
    std::ostringstream s;
    s << "PPL::termination_test_PR(pset):\n"
      << "pset.space_dimension() == " << space_dim
      << " is odd.";
    throw std::invalid_argument(s.str());
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  assign_all_inequalities_approximation(pset, cs);
  return termination_test_PR_original(cs);
}

template <typename PSET>
bool
one_affine_ranking_function_PR_2(const PSET& pset_before,
                                 const PSET& pset_after,
                                 Generator& mu) {
  const dimension_type before_space_dim = pset_before.space_dimension();
  const dimension_type after_space_dim = pset_after.space_dimension();
  if (after_space_dim != 2*before_space_dim) {
    std::ostringstream s;
    s << "PPL::one_affine_ranking_function_PR_2"
      << "(pset_before, pset_after, mu):\n"
      << "pset_before.space_dimension() == " << before_space_dim
      << ", pset_after.space_dimension() == " << after_space_dim
      << ";\nthe latter should be twice the former.";
    throw std::invalid_argument(s.str());
  }

  using namespace Implementation::Termination;
  Constraint_System cs_before;
  Constraint_System cs_after;
  assign_all_inequalities_approximation(pset_before, cs_before);
  assign_all_inequalities_approximation(pset_after, cs_after);
  return one_affine_ranking_function_PR(cs_before, cs_after, mu);
}

template <typename PSET>
bool
one_affine_ranking_function_PR(const PSET& pset, Generator& mu) {
  const dimension_type space_dim = pset.space_dimension();
  if (space_dim % 2 != 0) {
    std::ostringstream s;
    s << "PPL::one_affine_ranking_function_PR(pset, mu):\n"
      << "pset.space_dimension() == " << space_dim
      << " is odd.";
    throw std::invalid_argument(s.str());
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  assign_all_inequalities_approximation(pset, cs);
  return one_affine_ranking_function_PR_original(cs, mu);
}

template <typename PSET>
void
all_affine_ranking_functions_PR_2(const PSET& pset_before,
                                  const PSET& pset_after,
                                  NNC_Polyhedron& mu_space) {
  const dimension_type before_space_dim = pset_before.space_dimension();
  const dimension_type after_space_dim = pset_after.space_dimension();
  if (after_space_dim != 2*before_space_dim) {
    std::ostringstream s;
    s << "PPL::all_affine_ranking_functions_MS_2"
      << "(pset_before, pset_after, mu_space):\n"
      << "pset_before.space_dimension() == " << before_space_dim
      << ", pset_after.space_dimension() == " << after_space_dim
      << ";\nthe latter should be twice the former.";
    throw std::invalid_argument(s.str());
  }

  if (pset_before.is_empty()) {
    mu_space = NNC_Polyhedron(1 + before_space_dim);
    return;
  }

  using namespace Implementation::Termination;
  Constraint_System cs_before;
  Constraint_System cs_after;
  assign_all_inequalities_approximation(pset_before, cs_before);
  assign_all_inequalities_approximation(pset_after, cs_after);
  all_affine_ranking_functions_PR(cs_before, cs_after, mu_space);
}

template <typename PSET>
void
all_affine_ranking_functions_PR(const PSET& pset,
                                NNC_Polyhedron& mu_space) {
  const dimension_type space_dim = pset.space_dimension();
  if (space_dim % 2 != 0) {
    std::ostringstream s;
    s << "PPL::all_affine_ranking_functions_PR(pset, mu_space):\n"
      << "pset.space_dimension() == " << space_dim
      << " is odd.";
    throw std::invalid_argument(s.str());
  }

  if (pset.is_empty()) {
    mu_space = NNC_Polyhedron(1 + space_dim/2);
    return;
  }

  using namespace Implementation::Termination;
  Constraint_System cs;
  assign_all_inequalities_approximation(pset, cs);
  all_affine_ranking_functions_PR_original(cs, mu_space);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_termination_templates_hh)
