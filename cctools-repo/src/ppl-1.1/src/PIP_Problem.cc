/* PIP_Problem class implementation: non-inline functions.
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

#include "ppl-config.h"
#include "PIP_Problem_defs.hh"
#include "PIP_Tree_defs.hh"

namespace PPL = Parma_Polyhedra_Library;

/*! \relates Parma_Polyhedra_Library::PIP_Problem */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const PIP_Problem& pip) {
  s << "Space dimension: " << pip.space_dimension();
  s << "\nConstraints:";
  for (PIP_Problem::const_iterator i = pip.constraints_begin(),
         i_end = pip.constraints_end(); i != i_end; ++i)
    s << "\n" << *i;
  s << "\nProblem parameters: " << pip.parameter_space_dimensions();
  if (pip.get_big_parameter_dimension() == not_a_dimension())
    s << "\nNo big-parameter set.\n";
  else
    s << "\nBig-parameter: " << Variable(pip.get_big_parameter_dimension());
  s << "\n";
  return s;
}

PPL::PIP_Problem::PIP_Problem(const dimension_type dim)
  : external_space_dim(dim),
    internal_space_dim(0),
    status(PARTIALLY_SATISFIABLE),
    current_solution(0),
    input_cs(),
    first_pending_constraint(0),
    parameters(),
    initial_context(),
    big_parameter_dimension(not_a_dimension()) {
  // Check for space dimension overflow.
  if (dim > max_space_dimension())
    throw std::length_error("PPL::PIP_Problem::PIP_Problem(dim):\n"
                            "dim exceeds the maximum allowed "
                            "space dimension.");
  control_parameters_init();
  PPL_ASSERT(OK());
}

PPL::PIP_Problem::PIP_Problem(const PIP_Problem& y)
  : external_space_dim(y.external_space_dim),
    internal_space_dim(y.internal_space_dim),
    status(y.status),
    current_solution(0),
    input_cs(y.input_cs),
    first_pending_constraint(y.first_pending_constraint),
    parameters(y.parameters),
    initial_context(y.initial_context),
    big_parameter_dimension(y.big_parameter_dimension) {
  if (y.current_solution != 0) {
    current_solution = y.current_solution->clone();
    current_solution->set_owner(this);
  }
  control_parameters_copy(y);
  PPL_ASSERT(OK());
}

PPL::PIP_Problem::~PIP_Problem() {
  delete current_solution;
}

void
PPL::PIP_Problem::control_parameters_init() {
  control_parameters[CUTTING_STRATEGY] = CUTTING_STRATEGY_FIRST;
  control_parameters[PIVOT_ROW_STRATEGY] = PIVOT_ROW_STRATEGY_FIRST;
}

void
PPL::PIP_Problem::control_parameters_copy(const PIP_Problem& y) {
  for (dimension_type i = CONTROL_PARAMETER_NAME_SIZE; i-- > 0; )
    control_parameters[i] = y.control_parameters[i];
}

PPL::PIP_Problem_Status
PPL::PIP_Problem::solve() const {
  switch (status) {

  case UNSATISFIABLE:
    PPL_ASSERT(OK());
    return UNFEASIBLE_PIP_PROBLEM;

  case OPTIMIZED:
    PPL_ASSERT(OK());
    return OPTIMIZED_PIP_PROBLEM;

  case PARTIALLY_SATISFIABLE:
    {
      PIP_Problem& x = const_cast<PIP_Problem&>(*this);
      // Allocate PIP solution tree root, if needed.
      if (current_solution == 0)
        x.current_solution = new PIP_Solution_Node(this);

      // Properly resize context matrix.
      const dimension_type new_num_cols = parameters.size() + 1;
      const dimension_type old_num_cols = initial_context.num_columns();
      if (old_num_cols < new_num_cols)
        x.initial_context.add_zero_columns(new_num_cols - old_num_cols);

      // Computed once for all (to be used inside loop).
      const Variables_Set::const_iterator param_begin = parameters.begin();
      const Variables_Set::const_iterator param_end = parameters.end();

      // This flag will be set if we insert a pending constraint
      // in the initial context.
      bool check_feasible_context = false;

      // Go through all pending constraints.
      for (Constraint_Sequence::const_iterator
             cs_i = nth_iter(input_cs, first_pending_constraint),
             cs_end = input_cs.end(); cs_i != cs_end; ++cs_i) {
        const Constraint& c = *cs_i;
        const dimension_type c_space_dim = c.space_dimension();
        PPL_ASSERT(external_space_dim >= c_space_dim);

        // Constraints having a non-zero variable coefficient
        // should not be inserted in context.
        if (!c.expression().all_zeroes_except(parameters, 1, c_space_dim + 1))
          continue;

        check_feasible_context = true;

        x.initial_context.add_zero_rows(1);

        Row& row = x.initial_context[x.initial_context.num_rows()-1];

        {
          Row::iterator itr = row.end();

          if (c.inhomogeneous_term() != 0) {
            itr = row.insert(0, c.inhomogeneous_term());
            // Adjust inhomogeneous term if strict.
            if (c.is_strict_inequality())
              --(*itr);
          }
          else {
            // Adjust inhomogeneous term if strict.
            if (c.is_strict_inequality())
              itr = row.insert(0, -1);
          }
          dimension_type i = 1;

          // TODO: This loop may be optimized more, if needed.
          // If the size of param_end is expected to be greater than the
          // number of nonzeroes of c in most cases, then this implementation
          // can't be optimized further.
          // itr may still be end(), but it can still be used as hint.
          for (Variables_Set::const_iterator
               pi = param_begin; pi != param_end; ++pi, ++i) {
            if (*pi < c_space_dim) {
              Coefficient_traits::const_reference coeff_pi
                = c.coefficient(Variable(*pi));
              if (coeff_pi != 0)
                itr = row.insert(itr, i, coeff_pi);
            }
            else
              break;
          }
        }

        // If it is an equality, also insert its negation.
        if (c.is_equality()) {
          x.initial_context.add_zero_rows(1);

          // The reference `row' has been invalidated.

          Row& last_row = x.initial_context[x.initial_context.num_rows()-1];

          last_row = x.initial_context[x.initial_context.num_rows()-2];

          for (Row::iterator i = last_row.begin(),
                 i_end = last_row.end(); i != i_end; ++i)
            neg_assign(*i);
        }
      }

      if (check_feasible_context) {
        // Check for feasibility of initial context.
        Matrix<Row> ctx_copy(initial_context);
        if (!PIP_Solution_Node::compatibility_check(ctx_copy)) {
          // Problem found to be unfeasible.
          delete x.current_solution;
          x.current_solution = 0;
          x.status = UNSATISFIABLE;
          PPL_ASSERT(OK());
          return UNFEASIBLE_PIP_PROBLEM;
        }
      }

      // Update tableau and mark constraints as no longer pending.
      x.current_solution->update_tableau(*this,
                                         external_space_dim,
                                         first_pending_constraint,
                                         input_cs,
                                         parameters);
      x.internal_space_dim = external_space_dim;
      x.first_pending_constraint = input_cs.size();

      // Actually solve problem.
      x.current_solution = x.current_solution->solve(*this,
                                                     check_feasible_context,
                                                     initial_context,
                                                     parameters,
                                                     external_space_dim,
                                                     /*indent_level=*/ 0);
      // Update problem status.
      x.status = (x.current_solution != 0) ? OPTIMIZED : UNSATISFIABLE;

      PPL_ASSERT(OK());
      return (x.current_solution != 0)
        ? OPTIMIZED_PIP_PROBLEM
        : UNFEASIBLE_PIP_PROBLEM;
    } // End of handler for PARTIALLY_SATISFIABLE case.

  } // End of switch.

  // We should not be here!
  PPL_UNREACHABLE;
  return UNFEASIBLE_PIP_PROBLEM;
}

PPL::PIP_Tree
PPL::PIP_Problem::solution() const {
  if (status == PARTIALLY_SATISFIABLE)
    solve();
  return current_solution;
}

PPL::PIP_Tree
PPL::PIP_Problem::optimizing_solution() const {
  if (status == PARTIALLY_SATISFIABLE)
    solve();
  return current_solution;
}

bool
PPL::PIP_Problem::OK() const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  if (external_space_dim < internal_space_dim) {
#ifndef NDEBUG
      cerr << "The internal space dimension of the PIP_Problem is "
           << "greater than its external space dimension."
           << endl;
      ascii_dump(cerr);
#endif
      return false;
    }

  // Constraint system should be space dimension compatible.
  const dimension_type input_cs_num_rows = input_cs.size();
  for (dimension_type i = input_cs_num_rows; i-- > 0; ) {
    if (input_cs[i].space_dimension() > external_space_dim) {
#ifndef NDEBUG
      cerr << "The space dimension of the PIP_Problem is smaller than "
           << "the space dimension of one of its constraints."
           << endl;
      ascii_dump(cerr);
#endif
      return false;
    }
  }

  // Test validity of control parameter values.
  Control_Parameter_Value strategy = control_parameters[CUTTING_STRATEGY];
  if (strategy != CUTTING_STRATEGY_FIRST
      && strategy != CUTTING_STRATEGY_DEEPEST
      && strategy != CUTTING_STRATEGY_ALL) {
#ifndef NDEBUG
    cerr << "Invalid value for the CUTTING_STRATEGY control parameter."
         << endl;
    ascii_dump(cerr);
#endif
    return false;
  }

  strategy = control_parameters[PIVOT_ROW_STRATEGY];
  if (strategy < PIVOT_ROW_STRATEGY_FIRST
      || strategy > PIVOT_ROW_STRATEGY_MAX_COLUMN) {
#ifndef NDEBUG
    cerr << "Invalid value for the PIVOT_ROW_STRATEGY control parameter."
        << endl;
    ascii_dump(cerr);
#endif
    return false;
  }

  if (big_parameter_dimension != not_a_dimension()
      && parameters.count(big_parameter_dimension) == 0) {
#ifndef NDEBUG
    cerr << "The big parameter is set, but it is not a parameter." << endl;
    ascii_dump(cerr);
#endif
    return false;
  }

  if (!parameters.OK())
    return false;
  if (!initial_context.OK())
    return false;

  if (current_solution != 0) {
    // Check well formedness of the solution tree.
    if (!current_solution->OK()) {
#ifndef NDEBUG
      cerr << "The computed solution tree is broken.\n";
      ascii_dump(cerr);
#endif
      return false;
    }
    // Check that all nodes in the solution tree belong to *this.
    if (!current_solution->check_ownership(this)) {
#ifndef NDEBUG
      cerr << "There are nodes in the solution tree "
           << "that are not owned by *this.\n";
      ascii_dump(cerr);
#endif
      return false;
    }
  }

  // All checks passed.
  return true;
}

void
PPL::PIP_Problem::ascii_dump(std::ostream& s) const {
  using namespace IO_Operators;
  s << "\nexternal_space_dim: " << external_space_dim << "\n";
  s << "\ninternal_space_dim: " << internal_space_dim << "\n";

  const dimension_type input_cs_size = input_cs.size();

  s << "\ninput_cs( " << input_cs_size << " )\n";
  for (dimension_type i = 0; i < input_cs_size; ++i)
    input_cs[i].ascii_dump(s);

  s << "\nfirst_pending_constraint: " <<  first_pending_constraint << "\n";

  s << "\nstatus: ";
  switch (status) {
  case UNSATISFIABLE:
    s << "UNSATISFIABLE";
    break;
  case OPTIMIZED:
    s << "OPTIMIZED";
    break;
  case PARTIALLY_SATISFIABLE:
    s << "PARTIALLY_SATISFIABLE";
    break;
  }
  s << "\n";

  s << "\nparameters";
  parameters.ascii_dump(s);

  s << "\ninitial_context\n";
  initial_context.ascii_dump(s);

  s << "\ncontrol_parameters\n";
  for (dimension_type i = 0; i < CONTROL_PARAMETER_NAME_SIZE; ++i) {
    const Control_Parameter_Value value = control_parameters[i];
    switch (value) {
    case CUTTING_STRATEGY_FIRST:
      s << "CUTTING_STRATEGY_FIRST";
      break;
    case CUTTING_STRATEGY_DEEPEST:
      s << "CUTTING_STRATEGY_DEEPEST";
      break;
    case CUTTING_STRATEGY_ALL:
      s << "CUTTING_STRATEGY_ALL";
      break;
    case PIVOT_ROW_STRATEGY_FIRST:
      s << "PIVOT_ROW_STRATEGY_FIRST";
      break;
    case PIVOT_ROW_STRATEGY_MAX_COLUMN:
      s << "PIVOT_ROW_STRATEGY_MAX_COLUMN";
      break;
    default:
      s << "Invalid control parameter value";
    }
    s << "\n";
  }

  s << "\nbig_parameter_dimension: " << big_parameter_dimension << "\n";

  s << "\ncurrent_solution: ";
  if (current_solution == 0)
    s << "BOTTOM\n";
  else if (const PIP_Decision_Node* const dec
             = current_solution->as_decision()) {
    s << "DECISION\n";
    dec->ascii_dump(s);
  }
  else {
    const PIP_Solution_Node* const sol = current_solution->as_solution();
    PPL_ASSERT(sol != 0);
    s << "SOLUTION\n";
    sol->ascii_dump(s);
  }
}

PPL_OUTPUT_DEFINITIONS(PIP_Problem)

bool
PPL::PIP_Problem::ascii_load(std::istream& s) {
  std::string str;
  if (!(s >> str) || str != "external_space_dim:")
    return false;

  if (!(s >> external_space_dim))
    return false;

  if (!(s >> str) || str != "internal_space_dim:")
    return false;

  if (!(s >> internal_space_dim))
    return false;

  if (!(s >> str) || str != "input_cs(")
    return false;

  dimension_type input_cs_size;

  if (!(s >> input_cs_size))
    return false;

  if (!(s >> str) || str != ")")
    return false;

  Constraint c(Constraint::zero_dim_positivity());
  for (dimension_type i = 0; i < input_cs_size; ++i) {
    if (!c.ascii_load(s))
      return false;
    input_cs.push_back(c);
  }

  if (!(s >> str) || str != "first_pending_constraint:")
    return false;

  if (!(s >> first_pending_constraint))
    return false;

  if (!(s >> str) || str != "status:")
    return false;

  if (!(s >> str))
    return false;

  if (str == "UNSATISFIABLE")
    status = UNSATISFIABLE;
  else if (str == "OPTIMIZED")
    status = OPTIMIZED;
  else if (str == "PARTIALLY_SATISFIABLE")
    status = PARTIALLY_SATISFIABLE;
  else
    return false;

  if (!(s >> str) || str != "parameters")
    return false;

  if (!parameters.ascii_load(s))
    return false;

  if (!(s >> str) || str != "initial_context")
    return false;

  if (!initial_context.ascii_load(s))
    return false;

  if (!(s >> str) || str != "control_parameters")
    return false;

  for (dimension_type i = 0; i < CONTROL_PARAMETER_NAME_SIZE; ++i) {
    if (!(s >> str))
      return false;
    Control_Parameter_Value value;
    if (str == "CUTTING_STRATEGY_FIRST")
      value = CUTTING_STRATEGY_FIRST;
    else if (str == "CUTTING_STRATEGY_DEEPEST")
      value = CUTTING_STRATEGY_DEEPEST;
    else if (str == "CUTTING_STRATEGY_ALL")
      value = CUTTING_STRATEGY_ALL;
    else if (str == "PIVOT_ROW_STRATEGY_FIRST")
      value = PIVOT_ROW_STRATEGY_FIRST;
    else if (str == "PIVOT_ROW_STRATEGY_MAX_COLUMN")
      value = PIVOT_ROW_STRATEGY_MAX_COLUMN;
    else
      return false;
    control_parameters[i] = value;
  }

  if (!(s >> str) || str != "big_parameter_dimension:")
    return false;
  if (!(s >> big_parameter_dimension))
    return false;

  // Release current_solution tree (if any).
  delete current_solution;
  current_solution = 0;
  // Load current_solution (if any).
  if (!(s >> str) || str != "current_solution:")
    return false;
  if (!(s >> str))
    return false;
  if (str == "BOTTOM")
    current_solution = 0;
  else if (str == "DECISION") {
    PIP_Decision_Node* const dec = new PIP_Decision_Node(0, 0, 0);
    current_solution = dec;
    if (!dec->ascii_load(s))
      return false;
    dec->set_owner(this);
  }
  else if (str == "SOLUTION") {
    PIP_Solution_Node* const sol = new PIP_Solution_Node(0);
    current_solution = sol;
    if (!sol->ascii_load(s))
      return false;
    sol->set_owner(this);
  }
  else
    // Unknown node kind.
    return false;

  PPL_ASSERT(OK());
  return true;
}

void
PPL::PIP_Problem::clear() {
  external_space_dim = 0;
  internal_space_dim = 0;
  status = PARTIALLY_SATISFIABLE;
  if (current_solution != 0) {
    delete current_solution;
    current_solution = 0;
  }
  input_cs.clear();
  first_pending_constraint = 0;
  parameters.clear();
  initial_context.clear();
  control_parameters_init();
  big_parameter_dimension = not_a_dimension();
}

void
PPL::PIP_Problem
::add_space_dimensions_and_embed(const dimension_type m_vars,
                                 const dimension_type m_params) {
  // Adding no space dims at all is a no-op:
  // this avoids invalidating problem status (if it was optimized).
  if (m_vars == 0 && m_params == 0)
    return;

  // The space dimension of the resulting PIP problem should not
  // overflow the maximum allowed space dimension.
  dimension_type available = max_space_dimension() - space_dimension();
  bool should_throw = (m_vars > available);
  if (!should_throw) {
    available -= m_vars;
    should_throw = (m_params > available);
  }
  if (should_throw)
    throw std::length_error("PPL::PIP_Problem::"
                            "add_space_dimensions_and_embed(m_v, m_p):\n"
                            "adding m_v+m_p new space dimensions exceeds "
                            "the maximum allowed space dimension.");
  // First add PIP variables ...
  external_space_dim += m_vars;
  // ... then add PIP parameters.
  for (dimension_type i = m_params; i-- > 0; ) {
    parameters.insert(Variable(external_space_dim));
    ++external_space_dim;
  }
  // Update problem status.
  if (status != UNSATISFIABLE)
    status = PARTIALLY_SATISFIABLE;
  PPL_ASSERT(OK());
}

void
PPL::PIP_Problem
::add_to_parameter_space_dimensions(const Variables_Set& p_vars) {
  if (p_vars.space_dimension() > external_space_dim)
    throw std::invalid_argument("PPL::PIP_Problem::"
                                "add_to_parameter_space_dimension(p_vars):\n"
                                "*this and p_vars are dimension "
                                "incompatible.");
  const dimension_type original_size = parameters.size();
  parameters.insert(p_vars.begin(), p_vars.end());
  // Do not allow to turn variables into parameters.
  for (Variables_Set::const_iterator p = p_vars.begin(),
         end = p_vars.end(); p != end; ++p) {
    if (*p < internal_space_dim) {
      throw std::invalid_argument("PPL::PIP_Problem::"
                                  "add_to_parameter_space_dimension(p_vars):"
                                  "p_vars contain variable indices.");
    }
  }

  // If a new parameter was inserted, set the internal status to
  // PARTIALLY_SATISFIABLE.
  if (parameters.size() != original_size && status != UNSATISFIABLE)
    status = PARTIALLY_SATISFIABLE;
}

void
PPL::PIP_Problem::add_constraint(const Constraint& c) {
  if (c.space_dimension() > external_space_dim) {
    std::ostringstream s;
    s << "PPL::PIP_Problem::add_constraint(c):\n"
      << "dim == "<< external_space_dim << " and c.space_dimension() == "
      << c.space_dimension() << " are dimension incompatible.";
    throw std::invalid_argument(s.str());
  }
  input_cs.push_back(c);
  // Update problem status.
  if (status != UNSATISFIABLE)
    status = PARTIALLY_SATISFIABLE;
}

void
PPL::PIP_Problem::add_constraints(const Constraint_System& cs) {
  for (Constraint_System::const_iterator ci = cs.begin(),
         ci_end = cs.end(); ci != ci_end; ++ci)
    add_constraint(*ci);
}

bool
PPL::PIP_Problem::is_satisfiable() const {
  if (status == PARTIALLY_SATISFIABLE)
    solve();
  return status == OPTIMIZED;
}

void
PPL::PIP_Problem::set_control_parameter(Control_Parameter_Value value) {
  switch (value) {
  case CUTTING_STRATEGY_FIRST:
    // Intentionally fall through.
  case CUTTING_STRATEGY_DEEPEST:
    // Intentionally fall through.
  case CUTTING_STRATEGY_ALL:
    control_parameters[CUTTING_STRATEGY] = value;
    break;
  case PIVOT_ROW_STRATEGY_FIRST:
    // Intentionally fall through.
  case PIVOT_ROW_STRATEGY_MAX_COLUMN:
    control_parameters[PIVOT_ROW_STRATEGY] = value;
    break;
  default:
    throw std::invalid_argument("PPL::PIP_Problem::set_control_parameter(v)"
                                ":\ninvalid value.");
  }
}

void
PPL::PIP_Problem::set_big_parameter_dimension(dimension_type big_dim) {
  if (parameters.count(big_dim) == 0)
    throw std::invalid_argument("PPL::PIP_Problem::"
                                "set_big_parameter_dimension(big_dim):\n"
                                "dimension 'big_dim' is not a parameter.");
  if (big_dim < internal_space_dim)
    throw std::invalid_argument("PPL::PIP_Problem::"
                                "set_big_parameter_dimension(big_dim):\n"
                                "only newly-added parameters can be"
                                "converted into the big parameter.");
  big_parameter_dimension = big_dim;
}

PPL::memory_size_type
PPL::PIP_Problem::external_memory_in_bytes() const {
  memory_size_type n = initial_context.external_memory_in_bytes();
  // Adding the external memory for `current_solution'.
  if (current_solution != 0)
    n += current_solution->total_memory_in_bytes();
  // Adding the external memory for `input_cs'.
  n += input_cs.capacity() * sizeof(Constraint);
  for (const_iterator i = input_cs.begin(),
         i_end = input_cs.end(); i != i_end; ++i)
    n += (i->external_memory_in_bytes());
  // FIXME: Adding the external memory for `parameters'.
  n += parameters.size() * sizeof(dimension_type);

  return n;
}

PPL::memory_size_type
PPL::PIP_Problem::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

void
PPL::PIP_Problem::print_solution(std::ostream& s, int indent) const {
  switch (status) {

  case UNSATISFIABLE:
    PPL_ASSERT(current_solution == 0);
    PIP_Tree_Node::indent_and_print(s, indent, "_|_\n");
    break;

  case OPTIMIZED:
    PPL_ASSERT(current_solution != 0);
    current_solution->print(s, indent);
    break;

  case PARTIALLY_SATISFIABLE:
    throw std::logic_error("PIP_Problem::print_solution():\n"
                           "the PIP problem has not been solved.");
  }
}

