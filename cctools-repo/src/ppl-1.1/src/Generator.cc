/* Generator class implementation (non-inline functions).
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
#include "Generator_defs.hh"
#include "Variable_defs.hh"
#include "Variables_Set_defs.hh"
#include "math_utilities_defs.hh"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace PPL = Parma_Polyhedra_Library;

void
PPL::Generator::throw_dimension_incompatible(const char* method,
                                             const char* v_name,
                                             const Variable v) const {
  std::ostringstream s;
  s << "PPL::Generator::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension() << ", "
    << v_name << ".space_dimension() == " << v.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

void
PPL::Generator::throw_invalid_argument(const char* method,
                                       const char* reason) const {
  std::ostringstream s;
  s << "PPL::Generator::" << method << ":" << std::endl
    << reason << ".";
  throw std::invalid_argument(s.str());
}

PPL::Generator
PPL::Generator::point(const Linear_Expression& e,
                      Coefficient_traits::const_reference d,
                      Representation r) {
  if (d == 0)
    throw std::invalid_argument("PPL::point(e, d):\n"
                                "d == 0.");
  Linear_Expression ec(e, r);
  ec.set_inhomogeneous_term(d);
  Generator g(ec, Generator::POINT, NECESSARILY_CLOSED);

  // If the divisor is negative, we negate it as well as
  // all the coefficients of the point, because we want to preserve
  // the invariant: the divisor of a point is strictly positive.
  if (d < 0)
    neg_assign(g.expr);

  // Enforce normalization.
  g.expr.normalize();
  return g;
}

PPL::Generator
PPL::Generator::point(const Linear_Expression& e,
                      Representation r) {
  return point(e, Coefficient_one(), r);
}

PPL::Generator
PPL::Generator::point(Representation r) {
  return point(Linear_Expression::zero(), Coefficient_one(), r);
}

PPL::Generator
PPL::Generator::closure_point(const Linear_Expression& e,
                              Coefficient_traits::const_reference d,
                              Representation r) {
  if (d == 0)
    throw std::invalid_argument("PPL::closure_point(e, d):\n"
                                "d == 0.");
  Linear_Expression ec(e, r);
  ec.set_inhomogeneous_term(d);

  Generator g(ec, Generator::POINT, NOT_NECESSARILY_CLOSED);

  // If the divisor is negative, we negate it as well as
  // all the coefficients of the point, because we want to preserve
  // the invariant: the divisor of a point is strictly positive.
  if (d < 0)
    neg_assign(g.expr);

  // Enforce normalization.
  g.expr.normalize();
  return g;
}

PPL::Generator
PPL::Generator::closure_point(const Linear_Expression& e,
                              Representation r) {
  return closure_point(e, Coefficient_one(), r);
}

PPL::Generator
PPL::Generator::closure_point(Representation r) {
  return closure_point(Linear_Expression::zero(), Coefficient_one(), r);
}

PPL::Generator
PPL::Generator::ray(const Linear_Expression& e, Representation r) {
  // The origin of the space cannot be a ray.
  if (e.all_homogeneous_terms_are_zero())
    throw std::invalid_argument("PPL::ray(e):\n"
                                "e == 0, but the origin cannot be a ray.");

  Linear_Expression ec(e, r);
  ec.set_inhomogeneous_term(0);
  const Generator g(ec, Generator::RAY, NECESSARILY_CLOSED);

  return g;
}

PPL::Generator
PPL::Generator::line(const Linear_Expression& e, Representation r) {
  // The origin of the space cannot be a line.
  if (e.all_homogeneous_terms_are_zero())
    throw std::invalid_argument("PPL::line(e):\n"
                                "e == 0, but the origin cannot be a line.");

  Linear_Expression ec(e, r);
  ec.set_inhomogeneous_term(0);
  const Generator g(ec, Generator::LINE, NECESSARILY_CLOSED);

  return g;
}

void
PPL::Generator::swap_space_dimensions(Variable v1, Variable v2) {
  PPL_ASSERT(v1.space_dimension() <= space_dimension());
  PPL_ASSERT(v2.space_dimension() <= space_dimension());
  expr.swap_space_dimensions(v1, v2);
  // *this is still normalized but it may not be strongly normalized.
  sign_normalize();
  PPL_ASSERT(OK());
}

bool
PPL::Generator::remove_space_dimensions(const Variables_Set& vars) {
  PPL_ASSERT(vars.space_dimension() <= space_dimension());
  expr.remove_space_dimensions(vars);

  if (is_line_or_ray() && expr.all_homogeneous_terms_are_zero()) {
    // Become a point.
    set_is_ray_or_point();
    expr.set_inhomogeneous_term(1);
    if (is_not_necessarily_closed())
      set_epsilon_coefficient(1);

    PPL_ASSERT(OK());
    return false;
  }
  else {
    strong_normalize();
    PPL_ASSERT(OK());
    return true;
  }
}

void
PPL::Generator
::permute_space_dimensions(const std::vector<Variable>& cycle) {
  if (cycle.size() < 2)
    // No-op. No need to call sign_normalize().
    return;

  expr.permute_space_dimensions(cycle);

  // *this is still normalized but may be not strongly normalized: sign
  // normalization is necessary.
  sign_normalize();
  PPL_ASSERT(OK());
}

void
PPL::Generator::linear_combine(const Generator& y, dimension_type i) {
  expr.linear_combine(y.expr, i);
  strong_normalize();
}

/*! \relates Parma_Polyhedra_Library::Generator */
int
PPL::compare(const Generator& x, const Generator& y) {
  const bool x_is_line_or_equality = x.is_line_or_equality();
  const bool y_is_line_or_equality = y.is_line_or_equality();
  if (x_is_line_or_equality != y_is_line_or_equality)
    // Equalities (lines) precede inequalities (ray/point).
    return y_is_line_or_equality ? 2 : -2;

  return compare(x.expr, y.expr);
}

bool
PPL::Generator::is_equivalent_to(const Generator& y) const {
  const Generator& x = *this;
  const dimension_type x_space_dim = x.space_dimension();
  if (x_space_dim != y.space_dimension())
    return false;

  const Type x_type = x.type();
  if (x_type != y.type())
    return false;

  if (x_type == POINT
      && !(x.is_necessarily_closed() && y.is_necessarily_closed())) {
    // Due to the presence of epsilon-coefficients, syntactically
    // different points may actually encode the same generator.
    // First, drop the epsilon-coefficient ...
    Linear_Expression x_expr(x.expression());
    Linear_Expression y_expr(y.expression());
    // ... second, re-normalize ...
    x_expr.normalize();
    y_expr.normalize();
    // ... and finally check for syntactic equality.
    return x_expr.is_equal_to(y_expr);
  }

  // Here the epsilon-coefficient, if present, is zero.
  // It is sufficient to check for syntactic equality.
  return x.expr.is_equal_to(y.expr);
}

bool
PPL::Generator::is_equal_to(const Generator& y) const {
  return expr.is_equal_to(y.expr) && kind_ == y.kind_
         && topology_ == y.topology_;
}

void
PPL::Generator::sign_normalize() {
  if (is_line_or_equality())
    expr.sign_normalize();
}

bool
PPL::Generator::check_strong_normalized() const {
  Generator tmp = *this;
  tmp.strong_normalize();
  return compare(*this, tmp) == 0;
}

const PPL::Generator* PPL::Generator::zero_dim_point_p = 0;
const PPL::Generator* PPL::Generator::zero_dim_closure_point_p = 0;

void
PPL::Generator::initialize() {
  PPL_ASSERT(zero_dim_point_p == 0);
  zero_dim_point_p
    = new Generator(point());

  PPL_ASSERT(zero_dim_closure_point_p == 0);
  zero_dim_closure_point_p
    = new Generator(closure_point());
}

void
PPL::Generator::finalize() {
  PPL_ASSERT(zero_dim_point_p != 0);
  delete zero_dim_point_p;
  zero_dim_point_p = 0;

  PPL_ASSERT(zero_dim_closure_point_p != 0);
  delete zero_dim_closure_point_p;
  zero_dim_closure_point_p = 0;
}

void
PPL::Generator::fancy_print(std::ostream& s) const {
    bool needed_divisor = false;
  bool extra_parentheses = false;
  const dimension_type num_variables = space_dimension();
  const Generator::Type t = type();
  switch (t) {
  case Generator::LINE:
    s << "l(";
    break;
  case Generator::RAY:
    s << "r(";
    break;
  case Generator::POINT:
    s << "p(";
    goto any_point;
  case Generator::CLOSURE_POINT:
    s << "c(";
  any_point:
    if (expr.inhomogeneous_term() != 1) {
      needed_divisor = true;
      if (!expr.all_zeroes(1, num_variables + 1)) {
        extra_parentheses = true;
        s << "(";
      }
    }
    break;
  }

  PPL_DIRTY_TEMP_COEFFICIENT(c);
  bool first = true;
  for (Linear_Expression::const_iterator i = expr.begin(),
          i_end = expr.lower_bound(Variable(num_variables)); i != i_end; ++i) {
    c = *i;
    if (!first) {
      if (c > 0)
        s << " + ";
      else {
        s << " - ";
        neg_assign(c);
      }
    }
    else
      first = false;
    if (c == -1)
      s << "-";
    else if (c != 1)
      s << c << "*";
    IO_Operators::operator<<(s, i.variable());
  }
  if (first)
    // A point or closure point in the origin.
    s << 0;
  if (extra_parentheses)
    s << ")";
  if (needed_divisor)
    s << "/" << expr.inhomogeneous_term();
  s << ")";
}

/*! \relates Parma_Polyhedra_Library::Generator */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Generator& g) {
  g.fancy_print(s);
  return s;
}

/*! \relates Parma_Polyhedra_Library::Generator */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Generator::Type& t) {
  const char* n = 0;
  switch (t) {
  case Generator::LINE:
    n = "LINE";
    break;
  case Generator::RAY:
    n = "RAY";
    break;
  case Generator::POINT:
    n = "POINT";
    break;
  case Generator::CLOSURE_POINT:
    n = "CLOSURE_POINT";
    break;
  }
  s << n;
  return s;
}

bool
PPL::Generator::is_matching_closure_point(const Generator& p) const {
  PPL_ASSERT(topology() == p.topology()
         && space_dimension() == p.space_dimension()
         && type() == CLOSURE_POINT
         && p.type() == POINT);
  const Generator& cp = *this;
  if (cp.expr.inhomogeneous_term() == p.expr.inhomogeneous_term()) {
    // Divisors are equal: we can simply compare coefficients
    // (disregarding the epsilon coefficient).
    return cp.expr.is_equal_to(p.expr, 1, cp.expr.space_dimension());
  }
  else {
    // Divisors are different: divide them by their GCD
    // to simplify the following computation.
    PPL_DIRTY_TEMP_COEFFICIENT(gcd);
    gcd_assign(gcd, cp.expr.inhomogeneous_term(), p.expr.inhomogeneous_term());
    const bool rel_prime = (gcd == 1);
    PPL_DIRTY_TEMP_COEFFICIENT(cp_0_scaled);
    PPL_DIRTY_TEMP_COEFFICIENT(p_0_scaled);
    if (!rel_prime) {
      exact_div_assign(cp_0_scaled, cp.expr.inhomogeneous_term(), gcd);
      exact_div_assign(p_0_scaled, p.expr.inhomogeneous_term(), gcd);
    }
    const Coefficient& cp_div = rel_prime ? cp.expr.inhomogeneous_term() : cp_0_scaled;
    const Coefficient& p_div = rel_prime ? p.expr.inhomogeneous_term() : p_0_scaled;
    return cp.expr.is_equal_to(p.expr, p_div, cp_div, 1, cp.expr.space_dimension());
  }
}

PPL_OUTPUT_DEFINITIONS(Generator)


bool
PPL::Generator::OK() const {
  // Topology consistency checks.
  if (is_not_necessarily_closed() && expr.space_dimension() == 0) {
#ifndef NDEBUG
    std::cerr << "Generator has fewer coefficients than the minimum "
              << "allowed by its topology."
              << std::endl;
#endif
    return false;
  }

  // Normalization check.
  Generator tmp = *this;
  tmp.strong_normalize();
  if (tmp != *this) {
#ifndef NDEBUG
    std::cerr << "Generators should be strongly normalized!"
              << std::endl;
#endif
    return false;
  }

  switch (type()) {
  case LINE:
    // Intentionally fall through.
  case RAY:
    if (expr.inhomogeneous_term() != 0) {
#ifndef NDEBUG
      std::cerr << "Lines must have a zero inhomogeneous term!"
                << std::endl;
#endif
      return false;
    }
    if (!is_necessarily_closed() && epsilon_coefficient() != 0) {
#ifndef NDEBUG
      std::cerr << "Lines and rays must have a zero coefficient "
                << "for the epsilon dimension!"
                << std::endl;
#endif
      return false;
    }
    // The following test is correct, since we already checked
    // that the epsilon coordinate is zero.
    if (expr.all_homogeneous_terms_are_zero()) {
#ifndef NDEBUG
      std::cerr << "The origin of the vector space cannot be a line or a ray!"
                << std::endl;
#endif
      return false;
    }
    break;

  case POINT:
    if (expr.inhomogeneous_term() <= 0) {
#ifndef NDEBUG
      std::cerr << "Points must have a positive divisor!"
                << std::endl;
#endif
      return false;
    }
    if (!is_necessarily_closed())
      if (epsilon_coefficient() <= 0) {
#ifndef NDEBUG
        std::cerr << "In the NNC topology, points must have epsilon > 0"
                  << std::endl;
#endif
        return false;
      }
    break;

  case CLOSURE_POINT:
    if (expr.inhomogeneous_term() <= 0) {
#ifndef NDEBUG
      std::cerr << "Closure points must have a positive divisor!"
                << std::endl;
#endif
      return false;
    }
    break;
  }

  // All tests passed.
  return true;
}
