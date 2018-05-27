/* Implementation of the C interface: variables and non-inline functions.
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

/* Interface for Coefficient. */

#include "ppl_c_implementation_common.defs.hh"

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace C {

error_handler_type user_error_handler = 0;

extern "C" const char*
c_variable_default_output_function(ppl_dimension_type var) {
#if PPL_SIZEOF_SIZE_T == PPL_SIZEOF_INT
# define FORMAT "%u"
# define CONVERSION (unsigned)
#elif PPL_SIZEOF_SIZE_T == PPL_SIZEOF_LONG
# define FORMAT "%lu"
# define CONVERSION (unsigned long)
#elif PPL_SIZEOF_SIZE_T == PPL_SIZEOF_LONG_LONG
# define FORMAT "%llu"
# define CONVERSION (unsigned long long)
#else
# error "Unsupported definition for `size_t'."
#endif
  // On a 64-bits architecture, `var' will not be more than 2^64-1,
  // (2^64-1)/26 is written with 18 decimal digits, plus one letter,
  // plus one terminator makes 20.
#if defined(ULLONG_MAX) && ULLONG_MAX > 18446744073709551615ULL
# error "Please enlarge the buffer in the following line."
#endif
  static char buffer[20];
  buffer[0] = static_cast<char>('A' + var % 26);
  if (ppl_dimension_type i = var / 26) {
    int r = sprintf(buffer+1, FORMAT, CONVERSION i);
    if (r < 0)
      return 0;
    else if (r >= 19) {
      errno = ERANGE;
      return 0;
    }
  }
  else
    buffer[1] = '\0';
  return buffer;
}

// Holds a pointer to the C current output function.
ppl_io_variable_output_function_type* c_variable_output_function;

void
cxx_Variable_output_function(std::ostream& s, const Variable v) {
  const char* b = c_variable_output_function(v.id());
  if (b == 0)
    // Something went wrong in the client's output function.
    // Client code will know what to do: we do nothing.
    return;
  s << b;
}

extern "C" typedef const char*
c_variable_output_function_type(ppl_dimension_type var);

// Holds a pointer to the C++ saved output function.
Variable::output_function_type* saved_cxx_Variable_output_function;

void
notify_error(enum ppl_enum_error_code code, const char* description) {
  if (user_error_handler != 0)
    user_error_handler(code, description);
}

Parma_Polyhedra_Library::Watchdog* p_timeout_object = 0;

typedef
Parma_Polyhedra_Library::Threshold_Watcher
<Parma_Polyhedra_Library::Weightwatch_Traits> Weightwatch;

Weightwatch* p_deterministic_timeout_object = 0;

void
reset_timeout() {
  if (p_timeout_object != 0) {
    delete p_timeout_object;
    p_timeout_object = 0;
    abandon_expensive_computations = 0;
  }
}

void
reset_deterministic_timeout() {
  if (p_deterministic_timeout_object != 0) {
    delete p_deterministic_timeout_object;
    p_deterministic_timeout_object = 0;
    abandon_expensive_computations = 0;
  }
}

} // namespace C

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::Interfaces::C;

unsigned int PPL_POLY_CON_RELATION_IS_DISJOINT;
unsigned int PPL_POLY_CON_RELATION_STRICTLY_INTERSECTS;
unsigned int PPL_POLY_CON_RELATION_IS_INCLUDED;
unsigned int PPL_POLY_CON_RELATION_SATURATES;

unsigned int PPL_POLY_GEN_RELATION_SUBSUMES;

unsigned int PPL_COMPLEXITY_CLASS_POLYNOMIAL;
unsigned int PPL_COMPLEXITY_CLASS_SIMPLEX;
unsigned int PPL_COMPLEXITY_CLASS_ANY;

int PPL_MIP_PROBLEM_STATUS_UNFEASIBLE;
int PPL_MIP_PROBLEM_STATUS_UNBOUNDED;
int PPL_MIP_PROBLEM_STATUS_OPTIMIZED;

int PPL_MIP_PROBLEM_CONTROL_PARAMETER_NAME_PRICING;
int PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_STEEPEST_EDGE_FLOAT;
int PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_STEEPEST_EDGE_EXACT;
int PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_TEXTBOOK;

int PPL_PIP_PROBLEM_STATUS_UNFEASIBLE;
int PPL_PIP_PROBLEM_STATUS_OPTIMIZED;

int PPL_PIP_PROBLEM_CONTROL_PARAMETER_NAME_CUTTING_STRATEGY;
int PPL_PIP_PROBLEM_CONTROL_PARAMETER_CUTTING_STRATEGY_FIRST;
int PPL_PIP_PROBLEM_CONTROL_PARAMETER_CUTTING_STRATEGY_DEEPEST;
int PPL_PIP_PROBLEM_CONTROL_PARAMETER_CUTTING_STRATEGY_ALL;
int PPL_PIP_PROBLEM_CONTROL_PARAMETER_NAME_PIVOT_ROW_STRATEGY;
int PPL_PIP_PROBLEM_CONTROL_PARAMETER_PIVOT_ROW_STRATEGY_FIRST;
int PPL_PIP_PROBLEM_CONTROL_PARAMETER_PIVOT_ROW_STRATEGY_MAX_COLUMN;

int PPL_OPTIMIZATION_MODE_MINIMIZATION;
int PPL_OPTIMIZATION_MODE_MAXIMIZATION;

int
ppl_set_error_handler(error_handler_type h) {
  user_error_handler = h;
  return 0;
}

int
ppl_initialize(void) try {
  initialize();

  PPL_POLY_CON_RELATION_IS_DISJOINT
    = Poly_Con_Relation::is_disjoint().get_flags();
  PPL_POLY_CON_RELATION_STRICTLY_INTERSECTS
    = Poly_Con_Relation::strictly_intersects().get_flags();
  PPL_POLY_CON_RELATION_IS_INCLUDED
    = Poly_Con_Relation::is_included().get_flags();
  PPL_POLY_CON_RELATION_SATURATES
    = Poly_Con_Relation::saturates().get_flags();

  PPL_POLY_GEN_RELATION_SUBSUMES
    = Poly_Gen_Relation::subsumes().get_flags();

  PPL_COMPLEXITY_CLASS_POLYNOMIAL = POLYNOMIAL_COMPLEXITY;
  PPL_COMPLEXITY_CLASS_SIMPLEX = SIMPLEX_COMPLEXITY;
  PPL_COMPLEXITY_CLASS_ANY = ANY_COMPLEXITY;

  PPL_MIP_PROBLEM_STATUS_UNFEASIBLE = UNFEASIBLE_MIP_PROBLEM;
  PPL_MIP_PROBLEM_STATUS_UNBOUNDED = UNBOUNDED_MIP_PROBLEM;
  PPL_MIP_PROBLEM_STATUS_OPTIMIZED = OPTIMIZED_MIP_PROBLEM;

  PPL_MIP_PROBLEM_CONTROL_PARAMETER_NAME_PRICING
    = MIP_Problem::PRICING;
  PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_STEEPEST_EDGE_FLOAT
    = MIP_Problem::PRICING_STEEPEST_EDGE_FLOAT;
  PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_STEEPEST_EDGE_EXACT
    = MIP_Problem::PRICING_STEEPEST_EDGE_EXACT;
  PPL_MIP_PROBLEM_CONTROL_PARAMETER_PRICING_TEXTBOOK
    = MIP_Problem::PRICING_TEXTBOOK;

  PPL_PIP_PROBLEM_STATUS_UNFEASIBLE = UNFEASIBLE_PIP_PROBLEM;
  PPL_PIP_PROBLEM_STATUS_OPTIMIZED = OPTIMIZED_PIP_PROBLEM;

  PPL_PIP_PROBLEM_CONTROL_PARAMETER_NAME_CUTTING_STRATEGY
    = PIP_Problem::CUTTING_STRATEGY;
  PPL_PIP_PROBLEM_CONTROL_PARAMETER_CUTTING_STRATEGY_FIRST
    = PIP_Problem::CUTTING_STRATEGY_FIRST;
  PPL_PIP_PROBLEM_CONTROL_PARAMETER_CUTTING_STRATEGY_DEEPEST
    = PIP_Problem::CUTTING_STRATEGY_DEEPEST;
  PPL_PIP_PROBLEM_CONTROL_PARAMETER_CUTTING_STRATEGY_ALL
    = PIP_Problem::CUTTING_STRATEGY_ALL;
  PPL_PIP_PROBLEM_CONTROL_PARAMETER_NAME_PIVOT_ROW_STRATEGY
    = PIP_Problem::PIVOT_ROW_STRATEGY;
  PPL_PIP_PROBLEM_CONTROL_PARAMETER_PIVOT_ROW_STRATEGY_FIRST
    = PIP_Problem::PIVOT_ROW_STRATEGY_FIRST;
  PPL_PIP_PROBLEM_CONTROL_PARAMETER_PIVOT_ROW_STRATEGY_MAX_COLUMN
    = PIP_Problem::PIVOT_ROW_STRATEGY_MAX_COLUMN;

  PPL_OPTIMIZATION_MODE_MINIMIZATION = MINIMIZATION;
  PPL_OPTIMIZATION_MODE_MAXIMIZATION = MAXIMIZATION;

  c_variable_output_function = c_variable_default_output_function;
  saved_cxx_Variable_output_function = Variable::get_output_function();
  Variable::set_output_function(cxx_Variable_output_function);

  return 0;
}
CATCH_ALL

int
ppl_finalize(void) try {
  Variable::set_output_function(saved_cxx_Variable_output_function);

  finalize();
  return 0;
}
CATCH_ALL

int
ppl_set_timeout(unsigned csecs) try {
  // In case a timeout was already set.
  reset_timeout();
  static timeout_exception e;
  using Parma_Polyhedra_Library::Watchdog;
  p_timeout_object = new Watchdog(csecs, abandon_expensive_computations, e);
  return 0;
}
CATCH_ALL

int
ppl_reset_timeout(void) try {
  reset_timeout();
  return 0;
}
CATCH_ALL

int
ppl_set_deterministic_timeout(unsigned long unscaled_weight,
                              unsigned scale) try {
  // In case a deterministic timeout was already set.
  reset_deterministic_timeout();
  static timeout_exception e;
  typedef Parma_Polyhedra_Library::Weightwatch_Traits Traits;
  p_deterministic_timeout_object
    = new Weightwatch(Traits::compute_delta(unscaled_weight, scale),
                      abandon_expensive_computations, e);
  return 0;
}
CATCH_ALL

int
ppl_reset_deterministic_timeout(void) try {
  reset_deterministic_timeout();
  return 0;
}
CATCH_ALL

int
ppl_set_rounding_for_PPL(void) try {
  set_rounding_for_PPL();
  return 0;
}
CATCH_ALL

int
ppl_restore_pre_PPL_rounding(void) try {
  restore_pre_PPL_rounding();
  return 0;
}
CATCH_ALL

int
ppl_irrational_precision(unsigned* p) try {
  *p = irrational_precision();
  return 0;
}
CATCH_ALL

int
ppl_set_irrational_precision(unsigned p) try {
  set_irrational_precision(p);
  return 0;
}
CATCH_ALL

int
ppl_version_major(void) try {
  return static_cast<int>(version_major());
}
CATCH_ALL

int
ppl_version_minor(void) try {
  return static_cast<int>(version_minor());
}
CATCH_ALL

int
ppl_version_revision(void) try {
  return static_cast<int>(version_revision());
}
CATCH_ALL

int
ppl_version_beta(void) try {
  return static_cast<int>(version_beta());
}
CATCH_ALL

int
ppl_version(const char** p) try {
  // Note: use explicit qualification to avoid clashes on, e.g.,
  // Solaris 2.9, where `version' is the name of an enum defined in
  // math.h.
  *p = Parma_Polyhedra_Library::version();
  return 0;
}
CATCH_ALL

int
ppl_banner(const char** p) try {
  *p = banner();
  return 0;
}
CATCH_ALL

int
ppl_max_space_dimension(ppl_dimension_type* m) try {
  *m = max_space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_not_a_dimension(ppl_dimension_type* m) try {
  *m = not_a_dimension();
  return 0;
}
CATCH_ALL

int
ppl_new_Coefficient(ppl_Coefficient_t* pc) try {
  *pc = to_nonconst(new Coefficient(0));
  return 0;
}
CATCH_ALL

int
ppl_new_Coefficient_from_mpz_t(ppl_Coefficient_t* pc, mpz_t z) try {
  *pc = to_nonconst(new Coefficient(reinterpret_mpz_class(z)));
  return 0;
}
CATCH_ALL

int
ppl_new_Coefficient_from_Coefficient(ppl_Coefficient_t* pc,
				     ppl_const_Coefficient_t c) try {
  const Coefficient& cc = *to_const(c);
  *pc = to_nonconst(new Coefficient(cc));
  return 0;
}
CATCH_ALL

int
ppl_Coefficient_to_mpz_t(ppl_const_Coefficient_t c, mpz_t z) try {
  assign_r(reinterpret_mpz_class(z), *to_const(c), ROUND_NOT_NEEDED);
  return 0;
}
CATCH_ALL

int
ppl_delete_Coefficient(ppl_const_Coefficient_t c) try {
  delete to_const(c);
  return 0;
}
CATCH_ALL

int
ppl_assign_Coefficient_from_mpz_t(ppl_Coefficient_t dst, mpz_t z) try {
  Coefficient& ddst = *to_nonconst(dst);
  ddst = reinterpret_mpz_class(z);
  return 0;
}
CATCH_ALL

int
ppl_assign_Coefficient_from_Coefficient(ppl_Coefficient_t dst,
					ppl_const_Coefficient_t src) try {
  const Coefficient& ssrc = *to_const(src);
  Coefficient& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Coefficient_OK(ppl_const_Coefficient_t /* c */) try {
  return 1;
}
CATCH_ALL

int
ppl_Coefficient_is_bounded(void) try {
  return std::numeric_limits<Coefficient>::is_bounded ? 1 : 0;
}
CATCH_ALL

int
ppl_Coefficient_min(mpz_t min) try {
  if (std::numeric_limits<Coefficient>::is_bounded) {
    assign_r(reinterpret_mpz_class(min),
	     std::numeric_limits<Coefficient>::min(),
	     ROUND_NOT_NEEDED);
    return 1;
  }
  else
    return 0;
}
CATCH_ALL

int
ppl_Coefficient_max(mpz_t max) try {
  if (std::numeric_limits<Coefficient>::is_bounded) {
    assign_r(reinterpret_mpz_class(max),
	     std::numeric_limits<Coefficient>::max(),
	     ROUND_NOT_NEEDED);
    return 1;
  }
  else
    return 0;
}
CATCH_ALL

/* Interface for Linear_Expression. */

int
ppl_new_Linear_Expression(ppl_Linear_Expression_t* ple) try {
  *ple = to_nonconst(new Linear_Expression());
  return 0;
}
CATCH_ALL

int
ppl_new_Linear_Expression_with_dimension(ppl_Linear_Expression_t* ple,
					 ppl_dimension_type d) try {
  *ple = to_nonconst(d == 0
		     ? new Linear_Expression(0)
		     : new Linear_Expression(0*Variable(d-1)));
  return 0;
}
CATCH_ALL

int
ppl_new_Linear_Expression_from_Linear_Expression
(ppl_Linear_Expression_t* ple, ppl_const_Linear_Expression_t le) try {
  const Linear_Expression& lle = *to_const(le);
  *ple = to_nonconst(new Linear_Expression(lle));
  return 0;
}
CATCH_ALL

int
ppl_delete_Linear_Expression(ppl_const_Linear_Expression_t le) try {
  delete to_const(le);
  return 0;
}
CATCH_ALL

int
ppl_assign_Linear_Expression_from_Linear_Expression
(ppl_Linear_Expression_t dst, ppl_const_Linear_Expression_t src) try {
  const Linear_Expression& ssrc = *to_const(src);
  Linear_Expression& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Linear_Expression_add_to_coefficient(ppl_Linear_Expression_t le,
					 ppl_dimension_type var,
					 ppl_const_Coefficient_t n) try {
  Linear_Expression& lle = *to_nonconst(le);
  const Coefficient& nn = *to_const(n);
  add_mul_assign(lle, nn, Variable(var));
  return 0;
}
CATCH_ALL

int
ppl_Linear_Expression_add_to_inhomogeneous(ppl_Linear_Expression_t le,
					   ppl_const_Coefficient_t n) try {
  Linear_Expression& lle = *to_nonconst(le);
  const Coefficient& nn = *to_const(n);
  lle += nn;
  return 0;
}
CATCH_ALL

int
ppl_add_Linear_Expression_to_Linear_Expression
(ppl_Linear_Expression_t dst, ppl_const_Linear_Expression_t src) try {
  Linear_Expression& ddst = *to_nonconst(dst);
  const Linear_Expression& ssrc = *to_const(src);
  ddst += ssrc;
  return 0;
}
CATCH_ALL

int
ppl_subtract_Linear_Expression_from_Linear_Expression
(ppl_Linear_Expression_t dst, ppl_const_Linear_Expression_t src) try {
  Linear_Expression& ddst = *to_nonconst(dst);
  const Linear_Expression& ssrc = *to_const(src);
  ddst -= ssrc;
  return 0;
}
CATCH_ALL

int
ppl_multiply_Linear_Expression_by_Coefficient(ppl_Linear_Expression_t le,
					      ppl_const_Coefficient_t n) try {
  Linear_Expression& lle = *to_nonconst(le);
  const Coefficient& nn = *to_const(n);
  lle *= nn;
  return 0;
}
CATCH_ALL

int
ppl_Linear_Expression_space_dimension(ppl_const_Linear_Expression_t le,
				      ppl_dimension_type* m) try {
  *m = to_const(le)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Linear_Expression_coefficient(ppl_const_Linear_Expression_t le,
				  ppl_dimension_type var,
				  ppl_Coefficient_t n) try {
  const Linear_Expression& lle = *to_const(le);
  Coefficient& nn = *to_nonconst(n);
  nn = lle.coefficient(Variable(var));
  return 0;
}
CATCH_ALL

int
ppl_Linear_Expression_inhomogeneous_term(ppl_const_Linear_Expression_t le,
					 ppl_Coefficient_t n) try {
  const Linear_Expression& lle = *to_const(le);
  Coefficient& nn = *to_nonconst(n);
  nn = lle.inhomogeneous_term();
  return 0;
}
CATCH_ALL

int
ppl_Linear_Expression_OK(ppl_const_Linear_Expression_t le) try {
  return to_const(le)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_Linear_Expression_is_zero(ppl_const_Linear_Expression_t le) try {
  return to_const(le)->is_zero() ? 1 : 0;
}
CATCH_ALL

int
ppl_Linear_Expression_all_homogeneous_terms_are_zero
  (ppl_const_Linear_Expression_t le) try {
  return to_const(le)->all_homogeneous_terms_are_zero() ? 1 : 0;
}
CATCH_ALL

/* Interface for Constraint. */

int
ppl_new_Constraint(ppl_Constraint_t* pc,
		   ppl_const_Linear_Expression_t le,
		   enum ppl_enum_Constraint_Type t) try {
  Constraint* ppc;
  const Linear_Expression& lle = *to_const(le);
  switch (t) {
  case PPL_CONSTRAINT_TYPE_EQUAL:
    ppc = new Constraint(lle == 0);
    break;
  case PPL_CONSTRAINT_TYPE_GREATER_OR_EQUAL:
    ppc = new Constraint(lle >= 0);
    break;
  case PPL_CONSTRAINT_TYPE_GREATER_THAN:
    ppc = new Constraint(lle > 0);
    break;
  case PPL_CONSTRAINT_TYPE_LESS_OR_EQUAL:
    ppc = new Constraint(lle <= 0);
    break;
  case PPL_CONSTRAINT_TYPE_LESS_THAN:
    ppc = new Constraint(lle < 0);
    break;
  default:
    throw std::invalid_argument("ppl_new_Constraint(pc, le, t): "
				"t invalid");
  }
  *pc = to_nonconst(ppc);
  return 0;
}
CATCH_ALL

int
ppl_new_Constraint_zero_dim_false(ppl_Constraint_t* pc) try {
  *pc = to_nonconst(new Constraint(Constraint::zero_dim_false()));
  return 0;
}
CATCH_ALL

int
ppl_new_Constraint_zero_dim_positivity(ppl_Constraint_t* pc) try {
  *pc = to_nonconst(new Constraint(Constraint::zero_dim_positivity()));
  return 0;
}
CATCH_ALL

int
ppl_new_Constraint_from_Constraint(ppl_Constraint_t* pc,
				   ppl_const_Constraint_t c) try {
  const Constraint& cc = *to_const(c);
  *pc = to_nonconst(new Constraint(cc));
  return 0;
}
CATCH_ALL

int
ppl_delete_Constraint(ppl_const_Constraint_t le) try {
  delete to_const(le);
  return 0;
}
CATCH_ALL

int
ppl_assign_Constraint_from_Constraint(ppl_Constraint_t dst,
				      ppl_const_Constraint_t src) try {
  const Constraint& ssrc = *to_const(src);
  Constraint& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Constraint_space_dimension(ppl_const_Constraint_t c,
			       ppl_dimension_type* m) try {
  *m = to_const(c)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Constraint_type(ppl_const_Constraint_t c) try {
  switch (to_const(c)->type()) {
  case Constraint::EQUALITY:
    return PPL_CONSTRAINT_TYPE_EQUAL;
  case Constraint::NONSTRICT_INEQUALITY:
    return PPL_CONSTRAINT_TYPE_GREATER_OR_EQUAL;
  case Constraint::STRICT_INEQUALITY:
    return PPL_CONSTRAINT_TYPE_GREATER_THAN;
  }
  PPL_UNREACHABLE;
}
CATCH_ALL

int
ppl_Constraint_coefficient(ppl_const_Constraint_t c,
			   ppl_dimension_type var,
			   ppl_Coefficient_t n) try {
  const Constraint& cc = *to_const(c);
  Coefficient& nn = *to_nonconst(n);
  nn = cc.coefficient(Variable(var));
  return 0;
}
CATCH_ALL

int
ppl_Constraint_inhomogeneous_term(ppl_const_Constraint_t c,
				  ppl_Coefficient_t n) try {
  const Constraint& cc = *to_const(c);
  Coefficient& nn = *to_nonconst(n);
  nn = cc.inhomogeneous_term();
  return 0;
}
CATCH_ALL

int
ppl_Constraint_OK(ppl_const_Constraint_t c) try {
  return to_const(c)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_new_Linear_Expression_from_Constraint(ppl_Linear_Expression_t* ple,
					  ppl_const_Constraint_t c) try {
  const Constraint& cc = *to_const(c);
  *ple = to_nonconst(new Linear_Expression(cc));
  return 0;
}
CATCH_ALL

/* Interface for Constraint_System. */

int
ppl_new_Constraint_System(ppl_Constraint_System_t* pcs) try {
  *pcs = to_nonconst(new Constraint_System());
  return 0;
}
CATCH_ALL

int
ppl_new_Constraint_System_zero_dim_empty(ppl_Constraint_System_t* pcs) try {
  *pcs = to_nonconst(new
		     Constraint_System(Constraint_System::zero_dim_empty()));
  return 0;
}
CATCH_ALL


int
ppl_new_Constraint_System_from_Constraint(ppl_Constraint_System_t* pcs,
					  ppl_const_Constraint_t c) try {
  const Constraint& cc = *to_const(c);
  *pcs = to_nonconst(new Constraint_System(cc));
  return 0;
}
CATCH_ALL

int
ppl_new_Constraint_System_from_Constraint_System
(ppl_Constraint_System_t* pcs, ppl_const_Constraint_System_t cs) try {
  const Constraint_System& ccs = *to_const(cs);
  *pcs = to_nonconst(new Constraint_System(ccs));
  return 0;
}
CATCH_ALL

int
ppl_delete_Constraint_System(ppl_const_Constraint_System_t cs) try {
  delete to_const(cs);
  return 0;
}
CATCH_ALL

int
ppl_assign_Constraint_System_from_Constraint_System
(ppl_Constraint_System_t dst, ppl_const_Constraint_System_t src) try {
  const Constraint_System& ssrc = *to_const(src);
  Constraint_System& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_space_dimension(ppl_const_Constraint_System_t cs,
				      ppl_dimension_type* m) try {
  *m = to_const(cs)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_empty
(ppl_const_Constraint_System_t cs) try {
  const Constraint_System& ccs = *to_const(cs);
  return ccs.empty() ? 1 : 0;
}
CATCH_ALL

int
ppl_Constraint_System_has_strict_inequalities
(ppl_const_Constraint_System_t cs) try {
  const Constraint_System& ccs = *to_const(cs);
  return ccs.has_strict_inequalities() ? 1 : 0;
}
CATCH_ALL

int
ppl_Constraint_System_clear(ppl_Constraint_System_t cs) try {
  to_nonconst(cs)->clear();
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_insert_Constraint(ppl_Constraint_System_t cs,
					ppl_const_Constraint_t c) try {
  const Constraint& cc = *to_const(c);
  Constraint_System& ccs = *to_nonconst(cs);
  ccs.insert(cc);
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_OK(ppl_const_Constraint_System_t cs) try {
  return to_const(cs)->OK() ? 1 : 0;
}
CATCH_ALL

/* Interface for Constraint_System::const_iterator. */

int
ppl_new_Constraint_System_const_iterator
(ppl_Constraint_System_const_iterator_t* pcit) try {
  *pcit = to_nonconst(new Constraint_System::const_iterator());
  return 0;
}
CATCH_ALL

int
ppl_new_Constraint_System_const_iterator_from_Constraint_System_const_iterator
(ppl_Constraint_System_const_iterator_t* pcit,
 ppl_const_Constraint_System_const_iterator_t cit)  try {
  *pcit = to_nonconst(new Constraint_System::const_iterator(*to_const(cit)));
  return 0;
}
CATCH_ALL

int
ppl_delete_Constraint_System_const_iterator
(ppl_const_Constraint_System_const_iterator_t cit)
  try {
  delete to_const(cit);
  return 0;
}
CATCH_ALL

int
ppl_assign_Constraint_System_const_iterator_from_Constraint_System_const_iterator
(ppl_Constraint_System_const_iterator_t dst,
 ppl_const_Constraint_System_const_iterator_t src) try {
  const Constraint_System::const_iterator& ssrc = *to_const(src);
  Constraint_System::const_iterator& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_begin(ppl_const_Constraint_System_t cs,
			    ppl_Constraint_System_const_iterator_t cit) try {
  const Constraint_System& ccs = *to_const(cs);
  Constraint_System::const_iterator& ccit = *to_nonconst(cit);
  ccit = ccs.begin();
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_end(ppl_const_Constraint_System_t cs,
			  ppl_Constraint_System_const_iterator_t cit) try {
  const Constraint_System& ccs = *to_const(cs);
  Constraint_System::const_iterator& ccit = *to_nonconst(cit);
  ccit = ccs.end();
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_const_iterator_dereference
(ppl_const_Constraint_System_const_iterator_t cit,
 ppl_const_Constraint_t* pc) try {
  const Constraint_System::const_iterator& ccit = *to_const(cit);
  const Constraint& c = *ccit;
  *pc = to_const(&c);
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_const_iterator_increment
(ppl_Constraint_System_const_iterator_t cit) try {
  Constraint_System::const_iterator& ccit = *to_nonconst(cit);
  ++ccit;
  return 0;
}
CATCH_ALL

int
ppl_Constraint_System_const_iterator_equal_test
(ppl_const_Constraint_System_const_iterator_t x,
 ppl_const_Constraint_System_const_iterator_t y) try {
  const Constraint_System::const_iterator& xx = *to_const(x);
  const Constraint_System::const_iterator& yy = *to_const(y);
  return (xx == yy) ? 1 : 0;
}
CATCH_ALL

/* Interface for Generator. */

int
ppl_new_Generator(ppl_Generator_t* pg,
		  ppl_const_Linear_Expression_t le,
		  enum ppl_enum_Generator_Type t,
		  ppl_const_Coefficient_t d) try {
  Generator* ppg;
  const Linear_Expression& lle = *to_const(le);
  const Coefficient& dd = *to_const(d);
  switch (t) {
  case PPL_GENERATOR_TYPE_POINT:
    ppg = new Generator(Generator::point(lle, dd));
    break;
  case PPL_GENERATOR_TYPE_CLOSURE_POINT:
    ppg = new Generator(Generator::closure_point(lle, dd));
    break;
  case PPL_GENERATOR_TYPE_RAY:
    ppg = new Generator(Generator::ray(lle));
    break;
  case PPL_GENERATOR_TYPE_LINE:
    ppg = new Generator(Generator::line(lle));
    break;
  default:
    throw std::invalid_argument("ppl_new_Generator(pg, le, t, d): t invalid");
  }
  *pg = to_nonconst(ppg);
  return 0;
}
CATCH_ALL

int
ppl_new_Generator_zero_dim_point(ppl_Generator_t* pg) try {
  *pg = to_nonconst(new Generator(Generator::zero_dim_point()));
  return 0;
}
CATCH_ALL

int
ppl_new_Generator_zero_dim_closure_point(ppl_Generator_t* pg) try {
  *pg = to_nonconst(new Generator(Generator::zero_dim_closure_point()));
  return 0;
}
CATCH_ALL

int
ppl_new_Generator_from_Generator(ppl_Generator_t* pg,
				 ppl_const_Generator_t g) try {
  const Generator& gg = *to_const(g);
  *pg = to_nonconst(new Generator(gg));
  return 0;
}
CATCH_ALL

int
ppl_delete_Generator(ppl_const_Generator_t le) try {
  delete to_const(le);
  return 0;
}
CATCH_ALL

int
ppl_assign_Generator_from_Generator(ppl_Generator_t dst,
				      ppl_const_Generator_t src) try {
  const Generator& ssrc = *to_const(src);
  Generator& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Generator_space_dimension(ppl_const_Generator_t g,
			      ppl_dimension_type* m) try {
  *m = to_const(g)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Generator_type(ppl_const_Generator_t g) try {
  switch (to_const(g)->type()) {
  case Generator::LINE:
    return PPL_GENERATOR_TYPE_LINE;
  case Generator::RAY:
    return PPL_GENERATOR_TYPE_RAY;
  case Generator::POINT:
    return PPL_GENERATOR_TYPE_POINT;
  case Generator::CLOSURE_POINT:
    return PPL_GENERATOR_TYPE_CLOSURE_POINT;
  }
  PPL_UNREACHABLE;
}
CATCH_ALL

int
ppl_Generator_coefficient(ppl_const_Generator_t g,
			  ppl_dimension_type var,
			  ppl_Coefficient_t n) try {
  const Generator& gg = *to_const(g);
  Coefficient& nn = *to_nonconst(n);
  nn = gg.coefficient(Variable(var));
  return 0;
}
CATCH_ALL

int
ppl_Generator_divisor(ppl_const_Generator_t g,
		      ppl_Coefficient_t n) try {
  const Generator& gg = *to_const(g);
  Coefficient& nn = *to_nonconst(n);
  nn = gg.divisor();
  return 0;
}
CATCH_ALL

int
ppl_Generator_OK(ppl_const_Generator_t g) try {
  return to_const(g)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_new_Linear_Expression_from_Generator(ppl_Linear_Expression_t* ple,
					 ppl_const_Generator_t g) try {
  const Generator& gg = *to_const(g);
  *ple = to_nonconst(new Linear_Expression(gg));
  return 0;
}
CATCH_ALL

/* Interface for Generator_System. */

int
ppl_new_Generator_System(ppl_Generator_System_t* pgs) try {
  *pgs = to_nonconst(new Generator_System());
  return 0;
}
CATCH_ALL

int
ppl_new_Generator_System_zero_dim_univ(ppl_Generator_System_t* pgs) try {
  *pgs = to_nonconst(new Generator_System(Generator_System::zero_dim_univ()));
  return 0;
}
CATCH_ALL

int
ppl_new_Generator_System_from_Generator(ppl_Generator_System_t* pgs,
			      ppl_const_Generator_t g) try {
  const Generator& gg = *to_const(g);
  *pgs = to_nonconst(new Generator_System(gg));
  return 0;
}
CATCH_ALL

int
ppl_new_Generator_System_from_Generator_System
(ppl_Generator_System_t* pgs, ppl_const_Generator_System_t gs) try {
  const Generator_System& ggs = *to_const(gs);
  *pgs = to_nonconst(new Generator_System(ggs));
  return 0;
}
CATCH_ALL

int
ppl_delete_Generator_System(ppl_const_Generator_System_t gs) try {
  delete to_const(gs);
  return 0;
}
CATCH_ALL

int
ppl_assign_Generator_System_from_Generator_System
(ppl_Generator_System_t dst, ppl_const_Generator_System_t src) try {
  const Generator_System& ssrc = *to_const(src);
  Generator_System& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_space_dimension(ppl_const_Generator_System_t gs,
				     ppl_dimension_type* m) try {
  *m = to_const(gs)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_empty
(ppl_const_Generator_System_t gs) try {
  const Generator_System& cgs = *to_const(gs);
  return cgs.empty() ? 1 : 0;
}
CATCH_ALL

int
ppl_Generator_System_clear(ppl_Generator_System_t gs) try {
  to_nonconst(gs)->clear();
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_insert_Generator(ppl_Generator_System_t gs,
				      ppl_const_Generator_t g) try {
  const Generator& gg = *to_const(g);
  Generator_System& ggs = *to_nonconst(gs);
  ggs.insert(gg);
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_OK(ppl_const_Generator_System_t gs) try {
  return to_const(gs)->OK() ? 1 : 0;
}
CATCH_ALL

/* Interface for Generator_System::const_iterator. */

int
ppl_new_Generator_System_const_iterator
(ppl_Generator_System_const_iterator_t* pgit) try {
  *pgit = to_nonconst(new Generator_System::const_iterator());
  return 0;
}
CATCH_ALL

int
ppl_new_Generator_System_const_iterator_from_Generator_System_const_iterator
(ppl_Generator_System_const_iterator_t* pgit,
 ppl_const_Generator_System_const_iterator_t git)  try {
  *pgit = to_nonconst(new Generator_System::const_iterator(*to_const(git)));
  return 0;
}
CATCH_ALL

int
ppl_delete_Generator_System_const_iterator
(ppl_const_Generator_System_const_iterator_t git) try {
  delete to_const(git);
  return 0;
}
CATCH_ALL

int
ppl_assign_Generator_System_const_iterator_from_Generator_System_const_iterator
(ppl_Generator_System_const_iterator_t dst,
 ppl_const_Generator_System_const_iterator_t src) try {
  const Generator_System::const_iterator& ssrc = *to_const(src);
  Generator_System::const_iterator& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_begin(ppl_const_Generator_System_t gs,
			   ppl_Generator_System_const_iterator_t git) try {
  const Generator_System& ggs = *to_const(gs);
  Generator_System::const_iterator& ggit = *to_nonconst(git);
  ggit = ggs.begin();
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_end(ppl_const_Generator_System_t gs,
			 ppl_Generator_System_const_iterator_t git) try {
  const Generator_System& ggs = *to_const(gs);
  Generator_System::const_iterator& ggit = *to_nonconst(git);
  ggit = ggs.end();
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_const_iterator_dereference
(ppl_const_Generator_System_const_iterator_t git,
 ppl_const_Generator_t* pg) try {
  const Generator_System::const_iterator& ggit = *to_const(git);
  const Generator& c = *ggit;
  *pg = to_const(&c);
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_const_iterator_increment
(ppl_Generator_System_const_iterator_t git) try {
  Generator_System::const_iterator& ggit = *to_nonconst(git);
  ++ggit;
  return 0;
}
CATCH_ALL

int
ppl_Generator_System_const_iterator_equal_test
(ppl_const_Generator_System_const_iterator_t x,
 ppl_const_Generator_System_const_iterator_t y) try {
  const Generator_System::const_iterator& xx = *to_const(x);
  const Generator_System::const_iterator& yy = *to_const(y);
  return (xx == yy) ? 1 : 0;
}
CATCH_ALL

/* Interface for Congruence. */

int
ppl_new_Congruence(ppl_Congruence_t* pc,
		   ppl_const_Linear_Expression_t le,
		   ppl_const_Coefficient_t m) try {
  Congruence* ppc;
  const Linear_Expression& lle = *to_const(le);
  const Coefficient& mm = *to_const(m);
  ppc = new Congruence((lle %= 0) / mm);
  *pc = to_nonconst(ppc);
  return 0;
}
CATCH_ALL

int
ppl_new_Congruence_zero_dim_false(ppl_Congruence_t* pc) try {
  *pc = to_nonconst(new Congruence(Congruence::zero_dim_false()));
  return 0;
}
CATCH_ALL

int
ppl_new_Congruence_zero_dim_integrality(ppl_Congruence_t* pc) try {
  *pc = to_nonconst(new Congruence(Congruence::zero_dim_integrality()));
  return 0;
}
CATCH_ALL

int
ppl_new_Congruence_from_Congruence(ppl_Congruence_t* pc,
				   ppl_const_Congruence_t c) try {
  const Congruence& cc = *to_const(c);
  *pc = to_nonconst(new Congruence(cc));
  return 0;
}
CATCH_ALL

int
ppl_delete_Congruence(ppl_const_Congruence_t le) try {
  delete to_const(le);
  return 0;
}
CATCH_ALL

int
ppl_assign_Congruence_from_Congruence(ppl_Congruence_t dst,
				      ppl_const_Congruence_t src) try {
  const Congruence& ssrc = *to_const(src);
  Congruence& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Congruence_space_dimension(ppl_const_Congruence_t c,
			       ppl_dimension_type* m) try {
  *m = to_const(c)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Congruence_coefficient(ppl_const_Congruence_t c,
			   ppl_dimension_type var,
			   ppl_Coefficient_t n) try {
  const Congruence& cc = *to_const(c);
  Coefficient& nn = *to_nonconst(n);
  nn = cc.coefficient(Variable(var));
  return 0;
}
CATCH_ALL

int
ppl_Congruence_inhomogeneous_term(ppl_const_Congruence_t c,
				  ppl_Coefficient_t n) try {
  const Congruence& cc = *to_const(c);
  Coefficient& nn = *to_nonconst(n);
  nn = cc.inhomogeneous_term();
  return 0;
}
CATCH_ALL

int
ppl_Congruence_modulus(ppl_const_Congruence_t c,
		       ppl_Coefficient_t m) try {
  const Congruence& cc = *to_const(c);
  Coefficient& mm = *to_nonconst(m);
  mm = cc.modulus();
  return 0;
}
CATCH_ALL

int
ppl_Congruence_OK(ppl_const_Congruence_t c) try {
  return to_const(c)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_new_Linear_Expression_from_Congruence(ppl_Linear_Expression_t* ple,
					  ppl_const_Congruence_t c) try {
  const Congruence& cc = *to_const(c);
  *ple = to_nonconst(new Linear_Expression(cc));
  return 0;
}
CATCH_ALL

/* Interface for Congruence_System. */

int
ppl_new_Congruence_System(ppl_Congruence_System_t* pcs) try {
  *pcs = to_nonconst(new Congruence_System());
  return 0;
}
CATCH_ALL

int
ppl_new_Congruence_System_zero_dim_empty(ppl_Congruence_System_t* pcs) try {
  *pcs = to_nonconst(new
		     Congruence_System(Congruence_System::zero_dim_empty()));
  return 0;
}
CATCH_ALL


int
ppl_new_Congruence_System_from_Congruence(ppl_Congruence_System_t* pcs,
					  ppl_const_Congruence_t c) try {
  const Congruence& cc = *to_const(c);
  *pcs = to_nonconst(new Congruence_System(cc));
  return 0;
}
CATCH_ALL

int
ppl_new_Congruence_System_from_Congruence_System
(ppl_Congruence_System_t* pcs, ppl_const_Congruence_System_t cs) try {
  const Congruence_System& ccs = *to_const(cs);
  *pcs = to_nonconst(new Congruence_System(ccs));
  return 0;
}
CATCH_ALL

int
ppl_delete_Congruence_System(ppl_const_Congruence_System_t cs) try {
  delete to_const(cs);
  return 0;
}
CATCH_ALL

int
ppl_assign_Congruence_System_from_Congruence_System
(ppl_Congruence_System_t dst, ppl_const_Congruence_System_t src) try {
  const Congruence_System& ssrc = *to_const(src);
  Congruence_System& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_space_dimension(ppl_const_Congruence_System_t cs,
				      ppl_dimension_type* m) try {
  *m = to_const(cs)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_empty
(ppl_const_Congruence_System_t cs) try {
  const Congruence_System& ccs = *to_const(cs);
  return ccs.empty() ? 1 : 0;
}
CATCH_ALL

int
ppl_Congruence_System_clear(ppl_Congruence_System_t cs) try {
  to_nonconst(cs)->clear();
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_insert_Congruence(ppl_Congruence_System_t cs,
					ppl_const_Congruence_t c) try {
  const Congruence& cc = *to_const(c);
  Congruence_System& ccs = *to_nonconst(cs);
  ccs.insert(cc);
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_OK(ppl_const_Congruence_System_t cs) try {
  return to_const(cs)->OK() ? 1 : 0;
}
CATCH_ALL

/* Interface for Congruence_System::const_iterator. */

int
ppl_new_Congruence_System_const_iterator
(ppl_Congruence_System_const_iterator_t* pcit) try {
  *pcit = to_nonconst(new Congruence_System::const_iterator());
  return 0;
}
CATCH_ALL

int
ppl_new_Congruence_System_const_iterator_from_Congruence_System_const_iterator
(ppl_Congruence_System_const_iterator_t* pcit,
 ppl_const_Congruence_System_const_iterator_t cit)  try {
  *pcit = to_nonconst(new Congruence_System::const_iterator(*to_const(cit)));
  return 0;
}
CATCH_ALL

int
ppl_delete_Congruence_System_const_iterator
(ppl_const_Congruence_System_const_iterator_t cit)
  try {
  delete to_const(cit);
  return 0;
}
CATCH_ALL

int
ppl_assign_Congruence_System_const_iterator_from_Congruence_System_const_iterator
(ppl_Congruence_System_const_iterator_t dst,
 ppl_const_Congruence_System_const_iterator_t src) try {
  const Congruence_System::const_iterator& ssrc = *to_const(src);
  Congruence_System::const_iterator& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_begin(ppl_const_Congruence_System_t cs,
			    ppl_Congruence_System_const_iterator_t cit) try {
  const Congruence_System& ccs = *to_const(cs);
  Congruence_System::const_iterator& ccit = *to_nonconst(cit);
  ccit = ccs.begin();
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_end(ppl_const_Congruence_System_t cs,
			  ppl_Congruence_System_const_iterator_t cit) try {
  const Congruence_System& ccs = *to_const(cs);
  Congruence_System::const_iterator& ccit = *to_nonconst(cit);
  ccit = ccs.end();
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_const_iterator_dereference
(ppl_const_Congruence_System_const_iterator_t cit,
 ppl_const_Congruence_t* pc) try {
  const Congruence_System::const_iterator& ccit = *to_const(cit);
  const Congruence& c = *ccit;
  *pc = to_const(&c);
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_const_iterator_increment
(ppl_Congruence_System_const_iterator_t cit) try {
  Congruence_System::const_iterator& ccit = *to_nonconst(cit);
  ++ccit;
  return 0;
}
CATCH_ALL

int
ppl_Congruence_System_const_iterator_equal_test
(ppl_const_Congruence_System_const_iterator_t x,
 ppl_const_Congruence_System_const_iterator_t y) try {
  const Congruence_System::const_iterator& xx = *to_const(x);
  const Congruence_System::const_iterator& yy = *to_const(y);
  return (xx == yy) ? 1 : 0;
}
CATCH_ALL

/* Interface for Grid_Generator. */

int
ppl_new_Grid_Generator(ppl_Grid_Generator_t* pg,
		       ppl_const_Linear_Expression_t le,
		       enum ppl_enum_Grid_Generator_Type t,
		       ppl_const_Coefficient_t d) try {
  Grid_Generator* ppg;
  const Linear_Expression& lle = *to_const(le);
  const Coefficient& dd = *to_const(d);
  switch (t) {
  case PPL_GRID_GENERATOR_TYPE_LINE:
    ppg = new Grid_Generator(Grid_Generator::grid_line(lle));
    break;
  case PPL_GRID_GENERATOR_TYPE_PARAMETER:
    ppg = new Grid_Generator(Grid_Generator::parameter(lle));
    break;
  case PPL_GRID_GENERATOR_TYPE_POINT:
    ppg = new Grid_Generator(Grid_Generator::grid_point(lle, dd));
    break;
  default:
    throw std::invalid_argument("ppl_new_Grid_Generator(pg, le, t, d): "
				"t invalid");
  }
  *pg = to_nonconst(ppg);
  return 0;
}
CATCH_ALL

int
ppl_new_Grid_Generator_zero_dim_point(ppl_Grid_Generator_t* pg) try {
  *pg = to_nonconst(new Grid_Generator(Grid_Generator::zero_dim_point()));
  return 0;
}
CATCH_ALL

int
ppl_new_Grid_Generator_from_Grid_Generator(ppl_Grid_Generator_t* pg,
					   ppl_const_Grid_Generator_t g) try {
  const Grid_Generator& gg = *to_const(g);
  *pg = to_nonconst(new Grid_Generator(gg));
  return 0;
}
CATCH_ALL

int
ppl_delete_Grid_Generator(ppl_const_Grid_Generator_t le) try {
  delete to_const(le);
  return 0;
}
CATCH_ALL

int
ppl_assign_Grid_Generator_from_Grid_Generator
(ppl_Grid_Generator_t dst,
 ppl_const_Grid_Generator_t src) try {
  const Grid_Generator& ssrc = *to_const(src);
  Grid_Generator& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_space_dimension(ppl_const_Grid_Generator_t g,
				   ppl_dimension_type* m) try {
  *m = to_const(g)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_type(ppl_const_Grid_Generator_t g) try {
  switch (to_const(g)->type()) {
  case Grid_Generator::LINE:
    return PPL_GRID_GENERATOR_TYPE_LINE;
  case Grid_Generator::PARAMETER:
    return PPL_GRID_GENERATOR_TYPE_PARAMETER;
  case Grid_Generator::POINT:
    return PPL_GRID_GENERATOR_TYPE_POINT;
  }
  PPL_UNREACHABLE;
}
CATCH_ALL

int
ppl_Grid_Generator_coefficient(ppl_const_Grid_Generator_t g,
			       ppl_dimension_type var,
			       ppl_Coefficient_t n) try {
  const Grid_Generator& gg = *to_const(g);
  Coefficient& nn = *to_nonconst(n);
  nn = gg.coefficient(Variable(var));
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_divisor(ppl_const_Grid_Generator_t g,
			   ppl_Coefficient_t n) try {
  const Grid_Generator& gg = *to_const(g);
  Coefficient& nn = *to_nonconst(n);
  nn = gg.divisor();
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_OK(ppl_const_Grid_Generator_t g) try {
  return to_const(g)->OK() ? 1 : 0;
}
CATCH_ALL

// FIXME: to be restored soon.
// int
// ppl_new_Linear_Expression_from_Grid_Generator
// (ppl_Linear_Expression_t* ple,
//  ppl_const_Grid_Generator_t g) try {
//   const Grid_Generator& gg = *to_const(g);
//   *ple = to_nonconst(new Linear_Expression(gg));
//   return 0;
// }
// CATCH_ALL

/* Interface for Grid_Generator_System. */

int
ppl_new_Grid_Generator_System(ppl_Grid_Generator_System_t* pgs) try {
  *pgs = to_nonconst(new Grid_Generator_System());
  return 0;
}
CATCH_ALL

int
ppl_new_Grid_Generator_System_zero_dim_univ
(ppl_Grid_Generator_System_t* pgs) try {
  *pgs = to_nonconst
    (new Grid_Generator_System(Grid_Generator_System::zero_dim_univ()));
  return 0;
}
CATCH_ALL

int
ppl_new_Grid_Generator_System_from_Grid_Generator
(ppl_Grid_Generator_System_t* pgs, ppl_const_Grid_Generator_t g) try {
  const Grid_Generator& gg = *to_const(g);
  *pgs = to_nonconst(new Grid_Generator_System(gg));
  return 0;
}
CATCH_ALL

int
ppl_new_Grid_Generator_System_from_Grid_Generator_System
(ppl_Grid_Generator_System_t* pgs, ppl_const_Grid_Generator_System_t gs) try {
  const Grid_Generator_System& ggs = *to_const(gs);
  *pgs = to_nonconst(new Grid_Generator_System(ggs));
  return 0;
}
CATCH_ALL

int
ppl_delete_Grid_Generator_System(ppl_const_Grid_Generator_System_t gs) try {
  delete to_const(gs);
  return 0;
}
CATCH_ALL

int
ppl_assign_Grid_Generator_System_from_Grid_Generator_System
(ppl_Grid_Generator_System_t dst, ppl_const_Grid_Generator_System_t src) try {
  const Grid_Generator_System& ssrc = *to_const(src);
  Grid_Generator_System& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_space_dimension(ppl_const_Grid_Generator_System_t gs,
					  ppl_dimension_type* m) try {
  *m = to_const(gs)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_empty
(ppl_const_Grid_Generator_System_t gs) try {
  const Grid_Generator_System& cgs = *to_const(gs);
  return cgs.empty() ? 1 : 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_clear(ppl_Grid_Generator_System_t gs) try {
  to_nonconst(gs)->clear();
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_insert_Grid_Generator
(ppl_Grid_Generator_System_t gs,
 ppl_const_Grid_Generator_t g) try {
  const Grid_Generator& gg = *to_const(g);
  Grid_Generator_System& ggs = *to_nonconst(gs);
  ggs.insert(gg);
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_OK(ppl_const_Grid_Generator_System_t gs) try {
  return to_const(gs)->OK() ? 1 : 0;
}
CATCH_ALL

/* Interface for Grid_Generator_System::const_iterator. */

int
ppl_new_Grid_Generator_System_const_iterator
(ppl_Grid_Generator_System_const_iterator_t* pgit) try {
  *pgit = to_nonconst(new Grid_Generator_System::const_iterator());
  return 0;
}
CATCH_ALL

int
ppl_new_Grid_Generator_System_const_iterator_from_Grid_Generator_System_const_iterator
(ppl_Grid_Generator_System_const_iterator_t* pgit,
 ppl_const_Grid_Generator_System_const_iterator_t git)  try {
  *pgit = to_nonconst
    (new Grid_Generator_System::const_iterator(*to_const(git)));
  return 0;
}
CATCH_ALL

int
ppl_delete_Grid_Generator_System_const_iterator
(ppl_const_Grid_Generator_System_const_iterator_t git) try {
  delete to_const(git);
  return 0;
}
CATCH_ALL

int
ppl_assign_Grid_Generator_System_const_iterator_from_Grid_Generator_System_const_iterator
(ppl_Grid_Generator_System_const_iterator_t dst,
 ppl_const_Grid_Generator_System_const_iterator_t src) try {
  const Grid_Generator_System::const_iterator& ssrc = *to_const(src);
  Grid_Generator_System::const_iterator& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_begin
(ppl_const_Grid_Generator_System_t gs,
 ppl_Grid_Generator_System_const_iterator_t git) try {
  const Grid_Generator_System& ggs = *to_const(gs);
  Grid_Generator_System::const_iterator& ggit = *to_nonconst(git);
  ggit = ggs.begin();
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_end
(ppl_const_Grid_Generator_System_t gs,
 ppl_Grid_Generator_System_const_iterator_t git) try {
  const Grid_Generator_System& ggs = *to_const(gs);
  Grid_Generator_System::const_iterator& ggit = *to_nonconst(git);
  ggit = ggs.end();
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_const_iterator_dereference
(ppl_const_Grid_Generator_System_const_iterator_t git,
 ppl_const_Grid_Generator_t* pg) try {
  const Grid_Generator_System::const_iterator& ggit = *to_const(git);
  const Grid_Generator& g = *ggit;
  *pg = to_const(&g);
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_const_iterator_increment
(ppl_Grid_Generator_System_const_iterator_t git) try {
  Grid_Generator_System::const_iterator& ggit = *to_nonconst(git);
  ++ggit;
  return 0;
}
CATCH_ALL

int
ppl_Grid_Generator_System_const_iterator_equal_test
(ppl_const_Grid_Generator_System_const_iterator_t x,
 ppl_const_Grid_Generator_System_const_iterator_t y) try {
  const Grid_Generator_System::const_iterator& xx = *to_const(x);
  const Grid_Generator_System::const_iterator& yy = *to_const(y);
  return (xx == yy) ? 1 : 0;
}
CATCH_ALL

int
ppl_new_MIP_Problem_from_space_dimension(ppl_MIP_Problem_t* pmip,
                                         ppl_dimension_type d) try {
  *pmip = to_nonconst(new MIP_Problem(d));
  return 0;
}
CATCH_ALL

int
ppl_new_MIP_Problem(ppl_MIP_Problem_t* pmip,
		    ppl_dimension_type d,
		    ppl_const_Constraint_System_t cs,
		    ppl_const_Linear_Expression_t le, int m) try {
  const Constraint_System& ccs = *to_const(cs);
  const Linear_Expression& lle = *to_const(le);
  Optimization_Mode mm = (m == PPL_OPTIMIZATION_MODE_MINIMIZATION)
    ? MINIMIZATION : MAXIMIZATION;
  *pmip = to_nonconst(new MIP_Problem(d, ccs, lle, mm));
  return 0;
}
CATCH_ALL

int
ppl_new_MIP_Problem_from_MIP_Problem(ppl_MIP_Problem_t* pmip,
				     ppl_const_MIP_Problem_t mip) try {
  const MIP_Problem& mmip = *to_const(mip);
  *pmip = to_nonconst(new MIP_Problem(mmip));
  return 0;
}
CATCH_ALL

int
ppl_delete_MIP_Problem(ppl_const_MIP_Problem_t mip) try {
  delete to_const(mip);
  return 0;
}
CATCH_ALL

int
ppl_assign_MIP_Problem_from_MIP_Problem(ppl_MIP_Problem_t dst,
					ppl_const_MIP_Problem_t src) try {
  const MIP_Problem& ssrc = *to_const(src);
  MIP_Problem& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_space_dimension(ppl_const_MIP_Problem_t mip,
				ppl_dimension_type* m) try {
  *m = to_const(mip)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_number_of_integer_space_dimensions(ppl_const_MIP_Problem_t mip,
						   ppl_dimension_type* m) try {
  const MIP_Problem& mmip = *to_const(mip);
  *m = mmip.integer_space_dimensions().size();
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_integer_space_dimensions(ppl_const_MIP_Problem_t mip,
					 ppl_dimension_type ds[]) try {
  const Variables_Set& vars = to_const(mip)->integer_space_dimensions();
  ppl_dimension_type* ds_i = ds;
  for (Variables_Set::const_iterator v_iter = vars.begin(),
	 v_end = vars.end(); v_iter != v_end; ++v_iter, ++ds_i)
    *ds_i = *v_iter;
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_number_of_constraints(ppl_const_MIP_Problem_t mip,
				      ppl_dimension_type* m) try {
  const MIP_Problem& mmip = *to_const(mip);
  *m = static_cast<ppl_dimension_type>(mmip.constraints_end() - mmip.constraints_begin());
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_constraint_at_index(ppl_const_MIP_Problem_t mip,
				    ppl_dimension_type i,
				    ppl_const_Constraint_t* pc) try {
#ifndef NDEBUG
  ppl_dimension_type num_constraints;
  ppl_MIP_Problem_number_of_constraints(mip, &num_constraints);
  assert(i < num_constraints);
#endif
  const MIP_Problem& mmip = *to_const(mip);
  const Constraint& c = *(mmip.constraints_begin() + i);
  *pc = to_const(&c);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_objective_function(ppl_const_MIP_Problem_t mip,
				   ppl_const_Linear_Expression_t* ple) try {
  const Linear_Expression& le = to_const(mip)->objective_function();
  *ple = to_const(&le);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_optimization_mode(ppl_const_MIP_Problem_t mip) try {
  return to_const(mip)->optimization_mode();
}
CATCH_ALL

int
ppl_MIP_Problem_clear(ppl_MIP_Problem_t mip) try {
  to_nonconst(mip)->clear();
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_add_space_dimensions_and_embed(ppl_MIP_Problem_t mip,
					       ppl_dimension_type d) try {
  MIP_Problem& mmip = *to_nonconst(mip);
  mmip.add_space_dimensions_and_embed(d);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_add_to_integer_space_dimensions(ppl_MIP_Problem_t mip,
					        ppl_dimension_type ds[],
					        size_t n) try {
  MIP_Problem& mmip = *to_nonconst(mip);
  Variables_Set vars;
  for (ppl_dimension_type i = n; i-- > 0; )
    vars.insert(ds[i]);
  mmip.add_to_integer_space_dimensions(vars);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_add_constraint(ppl_MIP_Problem_t mip,
			       ppl_const_Constraint_t c) try {
  const Constraint& cc = *to_const(c);
  MIP_Problem& mmip = *to_nonconst(mip);
  mmip.add_constraint(cc);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_add_constraints(ppl_MIP_Problem_t mip,
				ppl_const_Constraint_System_t cs) try {
  const Constraint_System& ccs = *to_const(cs);
  MIP_Problem& mmip = *to_nonconst(mip);
  mmip.add_constraints(ccs);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_set_objective_function(ppl_MIP_Problem_t mip,
				       ppl_const_Linear_Expression_t le) try {
  const Linear_Expression& lle = *to_const(le);
  MIP_Problem& mmip = *to_nonconst(mip);
  mmip.set_objective_function(lle);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_set_optimization_mode(ppl_MIP_Problem_t mip, int mode) try {
  MIP_Problem& mmip = *to_nonconst(mip);
  Optimization_Mode m = (mode == PPL_OPTIMIZATION_MODE_MINIMIZATION)
    ? MINIMIZATION : MAXIMIZATION;
  mmip.set_optimization_mode(m);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_is_satisfiable(ppl_const_MIP_Problem_t mip) try {
  return to_const(mip)->is_satisfiable() ? 1 : 0;
}
CATCH_ALL

int
ppl_MIP_Problem_solve(ppl_const_MIP_Problem_t mip) try {
  return to_const(mip)->solve();
}
CATCH_ALL

int
ppl_MIP_Problem_evaluate_objective_function(ppl_const_MIP_Problem_t mip,
					    ppl_const_Generator_t g,
					    ppl_Coefficient_t num,
					    ppl_Coefficient_t den) try {
  const MIP_Problem& mmip = *to_const(mip);
  const Generator& gg = *to_const(g);
  Coefficient& nnum = *to_nonconst(num);
  Coefficient& dden = *to_nonconst(den);
  mmip.evaluate_objective_function(gg, nnum, dden);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_feasible_point(ppl_const_MIP_Problem_t mip,
			       ppl_const_Generator_t* pg) try {
  const Generator& g = to_const(mip)->feasible_point();
  *pg = to_const(&g);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_optimizing_point(ppl_const_MIP_Problem_t mip,
				 ppl_const_Generator_t* pg) try {
  const Generator& g = to_const(mip)->optimizing_point();
  *pg = to_const(&g);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_optimal_value(ppl_const_MIP_Problem_t mip,
			      ppl_Coefficient_t num,
			      ppl_Coefficient_t den) try {
  Coefficient& nnum = *to_nonconst(num);
  Coefficient& dden = *to_nonconst(den);
  to_const(mip)->optimal_value(nnum, dden);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_get_control_parameter(ppl_const_MIP_Problem_t mip,
                                      int name) try {
  MIP_Problem::Control_Parameter_Name n
    = static_cast<MIP_Problem::Control_Parameter_Name>(name);
  return to_const(mip)->get_control_parameter(n);
}
CATCH_ALL

int
ppl_MIP_Problem_set_control_parameter(ppl_MIP_Problem_t mip,
                                      int value) try {
  MIP_Problem::Control_Parameter_Value v
    = static_cast<MIP_Problem::Control_Parameter_Value>(value);
  to_nonconst(mip)->set_control_parameter(v);
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_OK(ppl_const_MIP_Problem_t mip) try {
  return to_const(mip)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_MIP_Problem_total_memory_in_bytes(ppl_const_MIP_Problem_t mip,
                                      size_t* sz) try {
  *sz = to_const(mip)->total_memory_in_bytes();
  return 0;
}
CATCH_ALL

int
ppl_MIP_Problem_external_memory_in_bytes(ppl_const_MIP_Problem_t mip,
                                         size_t* sz) try {
  *sz = to_const(mip)->external_memory_in_bytes();
  return 0;
}
CATCH_ALL

int
ppl_new_PIP_Problem_from_space_dimension(ppl_PIP_Problem_t* ppip,
                                         ppl_dimension_type d) try {
  *ppip = to_nonconst(new PIP_Problem(d));
  return 0;
}
CATCH_ALL

int
ppl_new_PIP_Problem_from_PIP_Problem(ppl_PIP_Problem_t* dpip,
				     ppl_const_PIP_Problem_t pip) try {
  const PIP_Problem& spip = *to_const(pip);
  *dpip = to_nonconst(new PIP_Problem(spip));
  return 0;
}
CATCH_ALL

int
ppl_new_PIP_Problem_from_constraints
(ppl_PIP_Problem_t* ppip,
 ppl_dimension_type d,
 ppl_Constraint_System_const_iterator_t first,
 ppl_Constraint_System_const_iterator_t last,
 size_t n,
 ppl_dimension_type ds[]) try {
  Variables_Set p_vars;
  for (ppl_dimension_type i = n; i-- > 0; )
    p_vars.insert(ds[i]);
  *ppip = to_nonconst(new PIP_Problem(d, *to_const(first),
                                      *to_const(last), p_vars));
  return 0;
}
CATCH_ALL

int
ppl_assign_PIP_Problem_from_PIP_Problem(ppl_PIP_Problem_t dst,
					ppl_const_PIP_Problem_t src) try {
  const PIP_Problem& ssrc = *to_const(src);
  PIP_Problem& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_delete_PIP_Problem(ppl_const_PIP_Problem_t pip) try {
  delete to_const(pip);
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_space_dimension(ppl_const_PIP_Problem_t pip,
				ppl_dimension_type* m) try {
  *m = to_const(pip)->space_dimension();
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_number_of_parameter_space_dimensions
(ppl_const_PIP_Problem_t pip, ppl_dimension_type* m) try {
  const PIP_Problem& ppip = *to_const(pip);
  *m = ppip.parameter_space_dimensions().size();
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_parameter_space_dimensions(ppl_const_PIP_Problem_t pip,
                                           ppl_dimension_type ds[]) try {
  const Variables_Set& vars = to_const(pip)->parameter_space_dimensions();
  ppl_dimension_type* ds_i = ds;
  for (Variables_Set::const_iterator v_iter = vars.begin(),
	 v_end = vars.end(); v_iter != v_end; ++v_iter, ++ds_i)
    *ds_i = *v_iter;
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_number_of_constraints(ppl_const_PIP_Problem_t pip,
				      ppl_dimension_type* m) try {
  const PIP_Problem& ppip = *to_const(pip);
  *m = static_cast<ppl_dimension_type>(ppip.constraints_end() - ppip.constraints_begin());
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_constraint_at_index(ppl_const_PIP_Problem_t pip,
				    ppl_dimension_type i,
				    ppl_const_Constraint_t* pc) try {
#ifndef NDEBUG
  ppl_dimension_type num_constraints;
  ppl_PIP_Problem_number_of_constraints(pip, &num_constraints);
  assert(i < num_constraints);
#endif
  const PIP_Problem& ppip = *to_const(pip);
  const Constraint& c = *(ppip.constraints_begin() + i);
  *pc = to_const(&c);
  return 0;
}
CATCH_ALL
int
ppl_PIP_Problem_clear(ppl_PIP_Problem_t pip) try {
  to_nonconst(pip)->clear();
  return 0;
}
CATCH_ALL
int
ppl_PIP_Problem_add_space_dimensions_and_embed(ppl_PIP_Problem_t pip,
					       ppl_dimension_type pip_vars,
					       ppl_dimension_type pip_params)
  try {
  PIP_Problem& spip = *to_nonconst(pip);
  spip.add_space_dimensions_and_embed(pip_vars,pip_params);
  return 0;
}
CATCH_ALL
int
ppl_PIP_Problem_add_to_parameter_space_dimensions(ppl_PIP_Problem_t pip,
					          ppl_dimension_type ds[],
					          size_t n) try {
  PIP_Problem& ppip = *to_nonconst(pip);
  Variables_Set vars;
  for (ppl_dimension_type i = n; i-- > 0; )
    vars.insert(ds[i]);
  ppip.add_to_parameter_space_dimensions(vars);
  return 0;
}
CATCH_ALL
int
ppl_PIP_Problem_add_constraint(ppl_PIP_Problem_t pip,
			       ppl_const_Constraint_t c) try {
  const Constraint& cc = *to_const(c);
  PIP_Problem& ppip = *to_nonconst(pip);
  ppip.add_constraint(cc);
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_add_constraints(ppl_PIP_Problem_t pip,
				ppl_const_Constraint_System_t cs) try {
  const Constraint_System& ccs = *to_const(cs);
  PIP_Problem& ppip = *to_nonconst(pip);
  ppip.add_constraints(ccs);
  return 0;
}
CATCH_ALL
int
ppl_PIP_Problem_is_satisfiable(ppl_const_PIP_Problem_t pip) try {
  return to_const(pip)->is_satisfiable() ? 1 : 0;
}
CATCH_ALL
int
ppl_PIP_Problem_solve(ppl_const_PIP_Problem_t pip) try {
  return to_const(pip)->solve();
}
CATCH_ALL

int
ppl_PIP_Problem_solution(ppl_const_PIP_Problem_t pip,
                         ppl_const_PIP_Tree_Node_t* ppip_tree) try {
  *ppip_tree = to_const(to_const(pip)->solution());
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_optimizing_solution(ppl_const_PIP_Problem_t pip,
                                    ppl_const_PIP_Tree_Node_t* ppip_tree) try {
  *ppip_tree = to_const(to_const(pip)->optimizing_solution());
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_OK(ppl_const_PIP_Problem_t pip) try {
  return to_const(pip)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_PIP_Problem_get_control_parameter(ppl_const_PIP_Problem_t pip,
                                      int name) try {
  PIP_Problem::Control_Parameter_Name n
    = static_cast<PIP_Problem::Control_Parameter_Name>(name);
  return to_const(pip)->get_control_parameter(n);
}
CATCH_ALL

int
ppl_PIP_Problem_set_control_parameter(ppl_PIP_Problem_t pip,
                                      int value) try {
  PIP_Problem::Control_Parameter_Value v
    = static_cast<PIP_Problem::Control_Parameter_Value>(value);
  to_nonconst(pip)->set_control_parameter(v);
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_get_big_parameter_dimension(ppl_const_PIP_Problem_t pip,
                                            ppl_dimension_type* pd) try {
  *pd = to_const(pip)->get_big_parameter_dimension();
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_set_big_parameter_dimension(ppl_PIP_Problem_t pip,
                                            ppl_dimension_type d) try {
  to_nonconst(pip)->set_big_parameter_dimension(d);
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_total_memory_in_bytes(ppl_const_PIP_Problem_t pip,
                                      size_t* sz) try {
  *sz = to_const(pip)->total_memory_in_bytes();
  return 0;
}
CATCH_ALL

int
ppl_PIP_Problem_external_memory_in_bytes(ppl_const_PIP_Problem_t pip,
                                         size_t* sz) try {
  *sz = to_const(pip)->external_memory_in_bytes();
  return 0;
}
CATCH_ALL

int
ppl_PIP_Tree_Node_as_solution(ppl_const_PIP_Tree_Node_t spip_tree,
                              ppl_const_PIP_Solution_Node_t* dpip_tree) try {
  *dpip_tree = to_const(to_const(spip_tree)->as_solution());
  return 0;
}
CATCH_ALL

int
ppl_PIP_Tree_Node_as_decision(ppl_const_PIP_Tree_Node_t spip_tree,
                              ppl_const_PIP_Decision_Node_t* dpip_tree) try {
  *dpip_tree = to_const(to_const(spip_tree)->as_decision());
  return 0;
}
CATCH_ALL

int
ppl_PIP_Tree_Node_get_constraints(ppl_const_PIP_Tree_Node_t pip_tree,
                                  ppl_const_Constraint_System_t* pcs) try {
  const PIP_Tree_Node& spip_tree = *to_const(pip_tree);
  const Constraint_System& cs = spip_tree.constraints();
  *pcs = to_const(&cs);
  return 0;
}
CATCH_ALL

int
ppl_PIP_Tree_Node_OK(ppl_const_PIP_Tree_Node_t pip_tree) try {
  return to_const(pip_tree)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_PIP_Tree_Node_number_of_artificials(ppl_const_PIP_Tree_Node_t pip_tree,
					ppl_dimension_type* m) try {
  const PIP_Tree_Node& node = *to_const(pip_tree);
  *m = node.art_parameter_count();
  return 0;
}
CATCH_ALL

int
ppl_PIP_Tree_Node_begin
(ppl_const_PIP_Tree_Node_t pip_tree,
 ppl_Artificial_Parameter_Sequence_const_iterator_t pit) try {
  PIP_Tree_Node::Artificial_Parameter_Sequence::const_iterator& spit
    = *to_nonconst(pit);
  spit = to_const(pip_tree)->art_parameter_begin();
  return 0;
}
CATCH_ALL

int
ppl_PIP_Tree_Node_end
(ppl_const_PIP_Tree_Node_t pip_tree,
 ppl_Artificial_Parameter_Sequence_const_iterator_t pit) try {
  PIP_Tree_Node::Artificial_Parameter_Sequence::const_iterator& spit
    = *to_nonconst(pit);
  spit = to_const(pip_tree)->art_parameter_end();
  return 0;
}
CATCH_ALL

int
ppl_PIP_Solution_Node_get_parametric_values
  (ppl_const_PIP_Solution_Node_t pip_sol,
   ppl_dimension_type var,
   ppl_const_Linear_Expression_t* le) try {
  const PIP_Solution_Node& spip_sol = *to_const(pip_sol);
  const Linear_Expression& lle = spip_sol.parametric_values(Variable(var));
  *le = to_const(&lle);
  return 0;
}
CATCH_ALL

int
ppl_PIP_Solution_Node_OK(ppl_const_PIP_Solution_Node_t pip_sol) try {
  return to_const(pip_sol)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_PIP_Decision_Node_OK(ppl_const_PIP_Decision_Node_t pip_dec) try {
  return to_const(pip_dec)->OK() ? 1 : 0;
}
CATCH_ALL

int
ppl_PIP_Decision_Node_get_child_node(ppl_const_PIP_Decision_Node_t pip_dec,
                                     int b,
                                     ppl_const_PIP_Tree_Node_t* pip_tree) try {
  *pip_tree = to_const(to_const(pip_dec)->child_node(b != 0));
  return 0;
}
CATCH_ALL

int
ppl_Artificial_Parameter_get_Linear_Expression
(ppl_const_Artificial_Parameter_t ap,
 ppl_Linear_Expression_t le) try {
  const Artificial_Parameter& sap = *to_const(ap);
  Linear_Expression& lle = *to_nonconst(le);
  lle = sap;
  return 0;
}
CATCH_ALL

int
ppl_Artificial_Parameter_coefficient(ppl_const_Artificial_Parameter_t ap,
                                     ppl_dimension_type var,
                                     ppl_Coefficient_t n) try {
  const Artificial_Parameter& sap = *to_const(ap);
  Coefficient& nn = *to_nonconst(n);
  nn = sap.coefficient(Variable(var));
  return 0;
}
CATCH_ALL

int
ppl_Artificial_Parameter_inhomogeneous_term
(ppl_const_Artificial_Parameter_t ap, ppl_Coefficient_t n) try {
  const Artificial_Parameter& sap = *to_const(ap);
  Coefficient& nn = *to_nonconst(n);
  nn = sap.inhomogeneous_term();
  return 0;
}
CATCH_ALL


int
ppl_Artificial_Parameter_denominator(ppl_const_Artificial_Parameter_t ap,
                                     ppl_Coefficient_t n) try {
  const Artificial_Parameter& sap = *to_const(ap);
  Coefficient& nn = *to_nonconst(n);
  nn = sap.denominator();
  return 0;
}
CATCH_ALL


/* Interface for Artificial_Parameter_Sequence::const_iterator. */

int
ppl_new_Artificial_Parameter_Sequence_const_iterator
(ppl_Artificial_Parameter_Sequence_const_iterator_t* papit) try {
  *papit = to_nonconst(new Artificial_Parameter_Sequence::const_iterator());
  return 0;
}
CATCH_ALL

int
ppl_new_Artificial_Parameter_Sequence_const_iterator_from_Artificial_Parameter_Sequence_const_iterator
(ppl_Artificial_Parameter_Sequence_const_iterator_t* papit,
 ppl_const_Artificial_Parameter_Sequence_const_iterator_t apit)  try {
  *papit = to_nonconst(new Artificial_Parameter_Sequence::const_iterator(*to_const(apit)));
  return 0;
}
CATCH_ALL

int
ppl_delete_Artificial_Parameter_Sequence_const_iterator
(ppl_const_Artificial_Parameter_Sequence_const_iterator_t apit)
  try {
  delete to_const(apit);
  return 0;
}
CATCH_ALL

int
ppl_assign_Artificial_Parameter_Sequence_const_iterator_from_Artificial_Parameter_Sequence_const_iterator
(ppl_Artificial_Parameter_Sequence_const_iterator_t dst,
 ppl_const_Artificial_Parameter_Sequence_const_iterator_t src) try {
  const Artificial_Parameter_Sequence::const_iterator& ssrc = *to_const(src);
  Artificial_Parameter_Sequence::const_iterator& ddst = *to_nonconst(dst);
  ddst = ssrc;
  return 0;
}
CATCH_ALL

int
ppl_Artificial_Parameter_Sequence_const_iterator_dereference
(ppl_const_Artificial_Parameter_Sequence_const_iterator_t apit,
 ppl_const_Artificial_Parameter_t* pap) try {
  const Artificial_Parameter_Sequence::const_iterator& papit = *to_const(apit);
  const Artificial_Parameter& ap = *papit;
  *pap = to_const(&ap);
  return 0;
}
CATCH_ALL

int
ppl_Artificial_Parameter_Sequence_const_iterator_increment
(ppl_Artificial_Parameter_Sequence_const_iterator_t apit) try {
  Artificial_Parameter_Sequence::const_iterator& papit = *to_nonconst(apit);
  ++papit;
  return 0;
}
CATCH_ALL

int
ppl_Artificial_Parameter_Sequence_const_iterator_equal_test
(ppl_const_Artificial_Parameter_Sequence_const_iterator_t x,
 ppl_const_Artificial_Parameter_Sequence_const_iterator_t y) try {
  const Artificial_Parameter_Sequence::const_iterator& xx = *to_const(x);
  const Artificial_Parameter_Sequence::const_iterator& yy = *to_const(y);
  return (xx == yy) ? 1 : 0;
}
CATCH_ALL

int
ppl_io_print_variable(ppl_dimension_type var) try {
  const char* b = c_variable_output_function(var);
  if (b == 0 || puts(b) < 0)
    return PPL_STDIO_ERROR;
  return 0;
}
CATCH_ALL

int
ppl_io_fprint_variable(FILE* stream, ppl_dimension_type var) try {
  const char* b = c_variable_output_function(var);
  if (b == 0 || fputs(b, stream) < 0)
    return PPL_STDIO_ERROR;
  return 0;
}
CATCH_ALL

int
ppl_io_asprint_variable(char** strp, ppl_dimension_type var) try {
  const char* b = c_variable_output_function(var);
  if (b == 0)
    return PPL_STDIO_ERROR;
  *strp = strdup(b);
  if (*strp == 0)
    return PPL_ERROR_OUT_OF_MEMORY;
  return 0;
}
CATCH_ALL

/* No ascii dump for Coefficient. */
DEFINE_PRINT_FUNCTIONS(Coefficient)

DEFINE_OUTPUT_FUNCTIONS(Linear_Expression)

DEFINE_OUTPUT_FUNCTIONS(Constraint)

DEFINE_OUTPUT_FUNCTIONS(Constraint_System)

DEFINE_OUTPUT_FUNCTIONS(Generator)

DEFINE_OUTPUT_FUNCTIONS(Generator_System)

DEFINE_OUTPUT_FUNCTIONS(Congruence)

DEFINE_OUTPUT_FUNCTIONS(Congruence_System)

DEFINE_OUTPUT_FUNCTIONS(Grid_Generator)

DEFINE_OUTPUT_FUNCTIONS(Grid_Generator_System)

DEFINE_OUTPUT_FUNCTIONS(MIP_Problem)

DEFINE_OUTPUT_FUNCTIONS(PIP_Problem)

DEFINE_OUTPUT_FUNCTIONS(PIP_Tree_Node)

DEFINE_OUTPUT_FUNCTIONS(PIP_Decision_Node)

DEFINE_OUTPUT_FUNCTIONS(PIP_Solution_Node)

DEFINE_OUTPUT_FUNCTIONS(Artificial_Parameter)

char*
ppl_io_wrap_string(const char* src,
		   unsigned indent_depth,
		   unsigned preferred_first_line_length,
		   unsigned preferred_line_length) {
  using namespace IO_Operators;
  return strdup(wrap_string(src, indent_depth,
			    preferred_first_line_length,
			    preferred_line_length).c_str());
}

int
ppl_io_set_variable_output_function(ppl_io_variable_output_function_type* p)
try {
  c_variable_output_function = p;
  return 0;
}
CATCH_ALL

int
ppl_io_get_variable_output_function(ppl_io_variable_output_function_type** pp)
try {
  *pp = c_variable_output_function;
  return 0;
}
CATCH_ALL
