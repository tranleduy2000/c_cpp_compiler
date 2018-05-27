/* Generic implementation of the wrap_assign() function.
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

#ifndef PPL_wrap_assign_hh
#define PPL_wrap_assign_hh 1

#include "globals_defs.hh"
#include "Coefficient_defs.hh"
#include "Variable_defs.hh"
#include "Constraint_System_defs.hh"
#include "assert.hh"

namespace Parma_Polyhedra_Library {

namespace Implementation {

struct Wrap_Dim_Translations {
  Variable var;
  Coefficient first_quadrant;
  Coefficient last_quadrant;
  Wrap_Dim_Translations(Variable v,
                        Coefficient_traits::const_reference f,
                        Coefficient_traits::const_reference l)
    : var(v), first_quadrant(f), last_quadrant(l) {
  }
};

typedef std::vector<Wrap_Dim_Translations> Wrap_Translations;

template <typename PSET>
void
wrap_assign_ind(PSET& pointset,
                Variables_Set& vars,
                Wrap_Translations::const_iterator first,
                Wrap_Translations::const_iterator end,
                Bounded_Integer_Type_Width w,
                Coefficient_traits::const_reference min_value,
                Coefficient_traits::const_reference max_value,
                const Constraint_System& cs,
                Coefficient& tmp1,
                Coefficient& tmp2) {
  const dimension_type space_dim = pointset.space_dimension();
  for (Wrap_Translations::const_iterator i = first; i != end; ++i) {
    const Wrap_Dim_Translations& wrap_dim_translations = *i;
    const Variable x(wrap_dim_translations.var);
    const Coefficient& first_quadrant = wrap_dim_translations.first_quadrant;
    const Coefficient& last_quadrant = wrap_dim_translations.last_quadrant;
    Coefficient& quadrant = tmp1;
    Coefficient& shift = tmp2;
    PSET hull(space_dim, EMPTY);
    for (quadrant = first_quadrant; quadrant <= last_quadrant; ++quadrant) {
      PSET p(pointset);
      if (quadrant != 0) {
        mul_2exp_assign(shift, quadrant, w);
        p.affine_image(x, x - shift, 1);
      }
      // `x' has just been wrapped.
      vars.erase(x.id());

      // Refine `p' with all the constraints in `cs' not depending
      // on variables in `vars'.
      if (vars.empty())
        p.refine_with_constraints(cs);
      else {
        for (Constraint_System::const_iterator j = cs.begin(),
               cs_end = cs.end(); j != cs_end; ++j)
          if (j->expression().all_zeroes(vars))
            // `*j' does not depend on variables in `vars'.
            p.refine_with_constraint(*j);
      }
      p.refine_with_constraint(min_value <= x);
      p.refine_with_constraint(x <= max_value);
      hull.upper_bound_assign(p);
    }
    pointset.m_swap(hull);
  }
}

template <typename PSET>
void
wrap_assign_col(PSET& dest,
                const PSET& src,
                const Variables_Set& vars,
                Wrap_Translations::const_iterator first,
                Wrap_Translations::const_iterator end,
                Bounded_Integer_Type_Width w,
                Coefficient_traits::const_reference min_value,
                Coefficient_traits::const_reference max_value,
                const Constraint_System* cs_p,
                Coefficient& tmp) {
  if (first == end) {
    PSET p(src);
    if (cs_p != 0)
      p.refine_with_constraints(*cs_p);
    for (Variables_Set::const_iterator i = vars.begin(),
           vars_end = vars.end(); i != vars_end; ++i) {
      const Variable x(*i);
      p.refine_with_constraint(min_value <= x);
      p.refine_with_constraint(x <= max_value);
    }
    dest.upper_bound_assign(p);
  }
  else {
    const Wrap_Dim_Translations& wrap_dim_translations = *first;
    const Variable x(wrap_dim_translations.var);
    const Coefficient& first_quadrant = wrap_dim_translations.first_quadrant;
    const Coefficient& last_quadrant = wrap_dim_translations.last_quadrant;
    Coefficient& shift = tmp;
    PPL_DIRTY_TEMP_COEFFICIENT(quadrant);
    for (quadrant = first_quadrant; quadrant <= last_quadrant; ++quadrant) {
      if (quadrant != 0) {
        mul_2exp_assign(shift, quadrant, w);
        PSET p(src);
        p.affine_image(x, x - shift, 1);
        wrap_assign_col(dest, p, vars, first+1, end, w, min_value, max_value,
                        cs_p, tmp);
      }
      else
        wrap_assign_col(dest, src, vars, first+1, end, w, min_value, max_value,
                        cs_p, tmp);
    }
  }
}

template <typename PSET>
void
wrap_assign(PSET& pointset,
            const Variables_Set& vars,
            const Bounded_Integer_Type_Width w,
            const Bounded_Integer_Type_Representation r,
            const Bounded_Integer_Type_Overflow o,
            const Constraint_System* cs_p,
            const unsigned complexity_threshold,
            const bool wrap_individually,
            const char* class_name) {
  // We must have cs_p->space_dimension() <= vars.space_dimension()
  //         and  vars.space_dimension() <= pointset.space_dimension().

  // Dimension-compatibility check of `*cs_p', if any.
  if (cs_p != 0) {
    const dimension_type vars_space_dim = vars.space_dimension();
    if (cs_p->space_dimension() > vars_space_dim) {
      std::ostringstream s;
      s << "PPL::" << class_name << "::wrap_assign(..., cs_p, ...):"
        << std::endl
        << "vars.space_dimension() == " << vars_space_dim
        << ", cs_p->space_dimension() == " << cs_p->space_dimension() << ".";
      throw std::invalid_argument(s.str());
    }

#ifndef NDEBUG
    // Check that all variables upon which `*cs_p' depends are in `vars'.
    // An assertion is violated otherwise.
    const Constraint_System cs = *cs_p;
    const dimension_type cs_space_dim = cs.space_dimension();
    Variables_Set::const_iterator vars_end = vars.end();
    for (Constraint_System::const_iterator i = cs.begin(),
           cs_end = cs.end(); i != cs_end; ++i) {
      const Constraint& c = *i;
      for (dimension_type d = cs_space_dim; d-- > 0; ) {
        PPL_ASSERT(c.coefficient(Variable(d)) == 0
                   || vars.find(d) != vars_end);
      }
    }
#endif
  }

  // Wrapping no variable only requires refining with *cs_p, if any.
  if (vars.empty()) {
    if (cs_p != 0)
      pointset.refine_with_constraints(*cs_p);
    return;
  }

  // Dimension-compatibility check of `vars'.
  const dimension_type space_dim = pointset.space_dimension();
  if (vars.space_dimension() > space_dim) {
    std::ostringstream s;
    s << "PPL::" << class_name << "::wrap_assign(vs, ...):" << std::endl
      << "this->space_dimension() == " << space_dim
      << ", required space dimension == " << vars.space_dimension() << ".";
    throw std::invalid_argument(s.str());
  }

  // Wrapping an empty polyhedron is a no-op.
  if (pointset.is_empty())
    return;

  // Set `min_value' and `max_value' to the minimum and maximum values
  // a variable of width `w' and signedness `s' can take.
  PPL_DIRTY_TEMP_COEFFICIENT(min_value);
  PPL_DIRTY_TEMP_COEFFICIENT(max_value);
  if (r == UNSIGNED) {
    min_value = 0;
    mul_2exp_assign(max_value, Coefficient_one(), w);
    --max_value;
  }
  else {
    PPL_ASSERT(r == SIGNED_2_COMPLEMENT);
    mul_2exp_assign(max_value, Coefficient_one(), w-1);
    neg_assign(min_value, max_value);
    --max_value;
  }

  // If we are wrapping variables collectively, the ranges for the
  // required translations are saved in `translations' instead of being
  // immediately applied.
  Wrap_Translations translations;

  // Dimensions subject to translation are added to this set if we are
  // wrapping collectively or if `cs_p' is non null.
  Variables_Set dimensions_to_be_translated;

  // This will contain a lower bound to the number of abstractions
  // to be joined in order to obtain the collective wrapping result.
  // As soon as this exceeds `complexity_threshold', counting will be
  // interrupted and the full range will be the result of wrapping
  // any dimension that is not fully contained in quadrant 0.
  unsigned collective_wrap_complexity = 1;

  // This flag signals that the maximum complexity for collective
  // wrapping as been exceeded.
  bool collective_wrap_too_complex = false;

  if (!wrap_individually) {
    translations.reserve(space_dim);
  }

  // We use `full_range_bounds' to delay conversions whenever
  // this delay does not negatively affect precision.
  Constraint_System full_range_bounds;

  PPL_DIRTY_TEMP_COEFFICIENT(l_n);
  PPL_DIRTY_TEMP_COEFFICIENT(l_d);
  PPL_DIRTY_TEMP_COEFFICIENT(u_n);
  PPL_DIRTY_TEMP_COEFFICIENT(u_d);

  for (Variables_Set::const_iterator i = vars.begin(),
         vars_end = vars.end(); i != vars_end; ++i) {

    const Variable x(*i);

    bool extremum;

    if (!pointset.minimize(x, l_n, l_d, extremum)) {
    set_full_range:
      pointset.unconstrain(x);
      full_range_bounds.insert(min_value <= x);
      full_range_bounds.insert(x <= max_value);
      continue;
    }

    if (!pointset.maximize(x, u_n, u_d, extremum))
      goto set_full_range;

    div_assign_r(l_n, l_n, l_d, ROUND_DOWN);
    div_assign_r(u_n, u_n, u_d, ROUND_DOWN);
    l_n -= min_value;
    u_n -= min_value;
    div_2exp_assign_r(l_n, l_n, w, ROUND_DOWN);
    div_2exp_assign_r(u_n, u_n, w, ROUND_DOWN);
    Coefficient& first_quadrant = l_n;
    const Coefficient& last_quadrant = u_n;

    // Special case: this variable does not need wrapping.
    if (first_quadrant == 0 && last_quadrant == 0)
      continue;

    // If overflow is impossible, try not to add useless constraints.
    if (o == OVERFLOW_IMPOSSIBLE) {
      if (first_quadrant < 0)
        full_range_bounds.insert(min_value <= x);
      if (last_quadrant > 0)
        full_range_bounds.insert(x <= max_value);
      continue;
    }

    if (o == OVERFLOW_UNDEFINED || collective_wrap_too_complex)
      goto set_full_range;

    Coefficient& quadrants = u_d;
    quadrants = last_quadrant - first_quadrant + 1;

    PPL_UNINITIALIZED(unsigned, extension);
    Result res = assign_r(extension, quadrants, ROUND_IGNORE);
    if (result_overflow(res) != 0 || extension > complexity_threshold)
      goto set_full_range;

    if (!wrap_individually && !collective_wrap_too_complex) {
      res = mul_assign_r(collective_wrap_complexity,
                         collective_wrap_complexity, extension, ROUND_IGNORE);
      if (result_overflow(res) != 0
          || collective_wrap_complexity > complexity_threshold)
        collective_wrap_too_complex = true;
      if (collective_wrap_too_complex) {
        // Set all the dimensions in `translations' to full range.
        for (Wrap_Translations::const_iterator j = translations.begin(),
               translations_end = translations.end();
             j != translations_end;
             ++j) {
          const Variable y(j->var);
          pointset.unconstrain(y);
          full_range_bounds.insert(min_value <= y);
          full_range_bounds.insert(y <= max_value);
        }
      }
    }

    if (wrap_individually && cs_p == 0) {
      Coefficient& quadrant = first_quadrant;
      // Temporary variable holding the shifts to be applied in order
      // to implement the translations.
      Coefficient& shift = l_d;
      PSET hull(space_dim, EMPTY);
      for ( ; quadrant <= last_quadrant; ++quadrant) {
        PSET p(pointset);
        if (quadrant != 0) {
          mul_2exp_assign(shift, quadrant, w);
          p.affine_image(x, x - shift, 1);
        }
        p.refine_with_constraint(min_value <= x);
        p.refine_with_constraint(x <= max_value);
        hull.upper_bound_assign(p);
      }
      pointset.m_swap(hull);
    }
    else if (wrap_individually || !collective_wrap_too_complex) {
      PPL_ASSERT(!wrap_individually || cs_p != 0);
      dimensions_to_be_translated.insert(x);
      translations
        .push_back(Wrap_Dim_Translations(x, first_quadrant, last_quadrant));
    }
  }

  if (!translations.empty()) {
    if (wrap_individually) {
      PPL_ASSERT(cs_p != 0);
      wrap_assign_ind(pointset, dimensions_to_be_translated,
                      translations.begin(), translations.end(),
                      w, min_value, max_value, *cs_p, l_n, l_d);
    }
    else {
      PSET hull(space_dim, EMPTY);
      wrap_assign_col(hull, pointset, dimensions_to_be_translated,
                      translations.begin(), translations.end(),
                      w, min_value, max_value, cs_p, l_n);
      pointset.m_swap(hull);
    }
  }

  if (cs_p != 0)
    pointset.refine_with_constraints(*cs_p);
  pointset.refine_with_constraints(full_range_bounds);
}

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_wrap_assign_hh)
