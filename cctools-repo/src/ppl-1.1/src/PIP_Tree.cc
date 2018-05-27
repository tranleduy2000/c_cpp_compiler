/* PIP_Tree related class implementation: non-inline functions.
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
#include "PIP_Tree_defs.hh"
#include "PIP_Problem_defs.hh"
#include <algorithm>
#include <memory>
#include <map>

// #define NOISY_PIP_TREE_STRUCTURE 1
// #define NOISY_PIP 1
// #define VERY_NOISY_PIP 1

namespace Parma_Polyhedra_Library {

namespace {

//! Assigns to \p x the positive remainder of the division of \p y by \p z.
inline void
pos_rem_assign(Coefficient& x,
               Coefficient_traits::const_reference y,
               Coefficient_traits::const_reference z) {
  rem_assign(x, y, z);
  if (x < 0)
    x += z;
}

class Add_Mul_Assign_Row_Helper1 {
public:
  Add_Mul_Assign_Row_Helper1(Coefficient_traits::const_reference coeff)
    : c(coeff) {
  }

  void
  operator()(Coefficient& x, Coefficient_traits::const_reference y) const {
    add_mul_assign(x, c, y);
  }

private:
  Coefficient c;
}; // class Add_Mul_Assign_Row_Helper1


class Add_Mul_Assign_Row_Helper2 {
public:
  Add_Mul_Assign_Row_Helper2(Coefficient_traits::const_reference coeff)
    : c(coeff) {
  }

  void
  operator()(Coefficient& x, Coefficient_traits::const_reference y) const {
    x = y;
    x *= c;
  }

private:
  Coefficient c;
}; // class Add_Mul_Assign_Row_Helper2

// Compute x += c * y
inline void
add_mul_assign_row(PIP_Tree_Node::Row& x,
                   Coefficient_traits::const_reference c,
                   const PIP_Tree_Node::Row& y) {
  WEIGHT_BEGIN();
  x.combine_needs_second(y,
                         Add_Mul_Assign_Row_Helper1(c),
                         Add_Mul_Assign_Row_Helper2(c));
  WEIGHT_ADD_MUL(108, x.size());
}


struct Sub_Assign_Helper1 {
  void
  operator()(Coefficient& x, Coefficient_traits::const_reference y) const {
    x -= y;
  }
}; // struct Sub_Assign_Helper1

struct Sub_Assign_Helper2 {
  void
  operator()(Coefficient& x, Coefficient_traits::const_reference y) const {
    x = y;
    neg_assign(x);
  }
}; // struct Sub_Assign_Helper2

// Compute x -= y
inline void
sub_assign(PIP_Tree_Node::Row& x, const PIP_Tree_Node::Row& y) {
  WEIGHT_BEGIN();
  x.combine_needs_second(y, Sub_Assign_Helper1(), Sub_Assign_Helper2());
  WEIGHT_ADD_MUL(10, x.size());
}

// Merge constraint system to a matrix-form context such as x = x U y
void
merge_assign(Matrix<PIP_Tree_Node::Row>& x, const Constraint_System& y,
             const Variables_Set& parameters) {
  const dimension_type params_size = parameters.size();
  PPL_ASSERT(params_size == x.num_columns() - 1);
  const dimension_type new_rows = Implementation::num_constraints(y);
  if (new_rows == 0)
    return;
  const dimension_type old_num_rows = x.num_rows();
  x.add_zero_rows(new_rows);

  // Compute once for all.
  const dimension_type cs_space_dim = y.space_dimension();
  const Variables_Set::const_iterator param_begin = parameters.begin();
  const Variables_Set::const_iterator param_end = parameters.end();

  dimension_type i = old_num_rows;
  for (Constraint_System::const_iterator y_i = y.begin(),
         y_end = y.end(); y_i != y_end; ++y_i, ++i) {
    WEIGHT_BEGIN();
    PPL_ASSERT(y_i->is_nonstrict_inequality());
    PIP_Tree_Node::Row& x_i = x[i];
    Coefficient_traits::const_reference inhomogeneous_term
      = y_i->inhomogeneous_term();
    Variables_Set::const_iterator pj = param_begin;
    PIP_Tree_Node::Row::iterator itr = x_i.end();
    if (inhomogeneous_term != 0)
      itr = x_i.insert(0, inhomogeneous_term);
    // itr may still be end() but it can still be used as a hint.
    // TODO: This code could be optimized more (if it's expected that the
    // size of `parameters' will be greater than the number of nonzero
    // coefficients in y_i).
    for (dimension_type j = 1; pj != param_end; ++pj, ++j) {
      const Variable vj(*pj);
      if (vj.space_dimension() > cs_space_dim)
        break;
      Coefficient_traits::const_reference c = y_i->coefficient(vj);
      if (c != 0)
        itr = x_i.insert(itr, j, c);
    }
    WEIGHT_ADD_MUL(102, params_size);
  }
}

#if PPL_USE_SPARSE_MATRIX

// Assigns to row x the negation of row y.
inline void
neg_assign_row(PIP_Tree_Node::Row& x, const PIP_Tree_Node::Row& y) {
  WEIGHT_BEGIN();
  x = y;
  for (PIP_Tree_Node::Row::iterator i = x.begin(),
         i_end = x.end(); i != i_end; ++i) {
    neg_assign(*i);
    WEIGHT_ADD(1);
  }
}

#else // !PPL_USE_SPARSE_MATRIX

inline void
neg_assign_row(PIP_Tree_Node::Row& x, const PIP_Tree_Node::Row& y) {
  WEIGHT_BEGIN();
  const dimension_type x_size = x.size();
  for (dimension_type i = x_size; i-- > 0; )
    neg_assign(x[i], y[i]);
  WEIGHT_ADD_MUL(14, x_size);
}

#endif // !PPL_USE_SPARSE_MATRIX

// Given context row \p y and denominator \p denom,
// to be interpreted as expression expr = y / denom,
// assigns to context row \p x a new value such that
//     x / denom == - expr - 1.
inline void
complement_assign(PIP_Tree_Node::Row& x,
                  const PIP_Tree_Node::Row& y,
                  Coefficient_traits::const_reference denom) {
  PPL_ASSERT(denom > 0);
  neg_assign_row(x, y);
  PIP_Tree_Node::Row::iterator itr = x.insert(0);
  Coefficient& x_0 = *itr;
  if (denom == 1)
    --x_0;
  else {
    PPL_DIRTY_TEMP_COEFFICIENT(mod);
    pos_rem_assign(mod, x_0, denom);
    x_0 -= (mod == 0) ? denom : mod;
  }
  if (x_0 == 0)
    x.reset(itr);
}

// Add to `context' the columns for new artificial parameters.
inline void
add_artificial_parameters(Matrix<PIP_Tree_Node::Row>& context,
                          const dimension_type num_art_params) {
  if (num_art_params > 0)
    context.add_zero_columns(num_art_params);
}

// Add to `params' the indices of new artificial parameters.
inline void
add_artificial_parameters(Variables_Set& params,
                          const dimension_type space_dim,
                          const dimension_type num_art_params) {
  for (dimension_type i = 0; i < num_art_params; ++i)
    params.insert(space_dim + i);
}

// Update `context', `params' and `space_dim' to account for
// the addition of the new artificial parameters.
inline void
add_artificial_parameters(Matrix<PIP_Tree_Node::Row>& context,
                          Variables_Set& params,
                          dimension_type& space_dim,
                          const dimension_type num_art_params) {
  add_artificial_parameters(context, num_art_params);
  add_artificial_parameters(params, space_dim, num_art_params);
  space_dim += num_art_params;
}

/* Compares two columns lexicographically in a revised simplex tableau:
  - returns true if
    <CODE>
      (column ja)*(-cst_a)/pivot_a[ja] < (column jb)*(-cst_b)/pivot_b[jb];
    </CODE>
  - returns false otherwise.
*/
bool
column_lower(const Matrix<PIP_Tree_Node::Row>& tableau,
             const std::vector<dimension_type>& mapping,
             const std::vector<bool>& basis,
             const PIP_Tree_Node::Row& pivot_a, const dimension_type ja,
             const PIP_Tree_Node::Row& pivot_b, const dimension_type jb,
             Coefficient_traits::const_reference cst_a = -1,
             Coefficient_traits::const_reference cst_b = -1) {
  Coefficient_traits::const_reference sij_a = pivot_a.get(ja);
  Coefficient_traits::const_reference sij_b = pivot_b.get(jb);
  PPL_ASSERT(sij_a > 0);
  PPL_ASSERT(sij_b > 0);

  PPL_DIRTY_TEMP_COEFFICIENT(lhs_coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(rhs_coeff);
  lhs_coeff = cst_a * sij_b;
  rhs_coeff = cst_b * sij_a;

  const int lhs_coeff_sign = sgn(lhs_coeff);
  const int rhs_coeff_sign = sgn(rhs_coeff);

  if (ja == jb) {
    // Same column: just compare the ratios.
    // This works since all columns are lexico-positive.
    return lhs_coeff > rhs_coeff;
  }

  PPL_DIRTY_TEMP_COEFFICIENT(lhs);
  PPL_DIRTY_TEMP_COEFFICIENT(rhs);
  const dimension_type num_vars = mapping.size();
  dimension_type k = 0;
  // While loop guard is: (k < num_rows && lhs == rhs).
  // Return value is false, if k >= num_rows; it is equivalent to
  // lhs < rhs, otherwise.
  // Try to optimize the computation of lhs and rhs.
  WEIGHT_BEGIN();
  while (true) {
    const dimension_type mk = mapping[k];
    const bool in_base = basis[k];
    if (++k >= num_vars)
      return false;
    if (in_base) {
      // Reconstitute the identity submatrix part of tableau.
      if (mk == ja) {
        // Optimizing for: lhs == lhs_coeff && rhs == 0;
        if (lhs_coeff == 0)
          continue;
        else
          return lhs_coeff > 0;
      }
      if (mk == jb) {
        // Optimizing for: lhs == 0 && rhs == rhs_coeff;
        if (rhs_coeff == 0)
          continue;
        else
          return 0 > rhs_coeff;
      }
      // Optimizing for: lhs == 0 && rhs == 0;
      continue;
    }
    else {
      // Not in base.
      const PIP_Tree_Node::Row& t_mk = tableau[mk];
      Coefficient_traits::const_reference t_mk_ja = t_mk.get(ja);
      Coefficient_traits::const_reference t_mk_jb = t_mk.get(jb);
      if (t_mk_ja == 0)
        if (t_mk_jb == 0)
          continue;
        else {
          const int rhs_sign = rhs_coeff_sign * sgn(t_mk_jb);
          if (0 == rhs_sign)
            continue;
          else
            return 0 > rhs_sign;
        }
      else
        if (t_mk_jb == 0) {
          const int lhs_sign = lhs_coeff_sign * sgn(t_mk_ja);
          if (lhs_sign == 0)
            continue;
          else
            return lhs_sign > 0;
        }
        else {
          WEIGHT_ADD(2);
          lhs = lhs_coeff * t_mk_ja;
          rhs = rhs_coeff * t_mk_jb;
          if (lhs == rhs)
            continue;
          else
            return lhs > rhs;
        }
    }
  }
  // This point should be unreachable.
  PPL_UNREACHABLE;
  return false;
}

/* Find the column j in revised simplex tableau such that
  - j is in candidates
  - (column j) / pivot_row[j] is lexico-minimal
  When this function returns, candidates contains the minimum(s) column(s)
  index(es).
*/
void
find_lexico_minimal_column_in_set(std::vector<dimension_type>& candidates,
                                  const Matrix<PIP_Tree_Node::Row>& tableau,
                                  const std::vector<dimension_type>& mapping,
                                  const std::vector<bool>& basis,
                                  const PIP_Tree_Node::Row& pivot_row) {
  WEIGHT_BEGIN();
  const dimension_type num_vars = mapping.size();

  PPL_ASSERT(!candidates.empty());
  // This is used as a set, it is always sorted.
  std::vector<dimension_type> new_candidates;
  for (dimension_type var_index = 0; var_index < num_vars; ++var_index) {
    new_candidates.clear();
    std::vector<dimension_type>::const_iterator i = candidates.begin();
    std::vector<dimension_type>::const_iterator i_end = candidates.end();
    PPL_ASSERT(!candidates.empty());
    new_candidates.push_back(*i);
    dimension_type min_column = *i;
    ++i;
    if (i == i_end)
      // Only one candidate left, so it is the minimum.
      break;
    PIP_Tree_Node::Row::const_iterator pivot_itr;
    pivot_itr = pivot_row.find(min_column);
    PPL_ASSERT(pivot_itr != pivot_row.end());
    Coefficient sij_b = *pivot_itr;
    ++pivot_itr;
    const dimension_type row_index = mapping[var_index];
    const bool in_base = basis[var_index];
    if (in_base) {
      for ( ; i != i_end; ++i) {
        pivot_itr = pivot_row.find(pivot_itr, *i);
        PPL_ASSERT(pivot_itr != pivot_row.end());
        Coefficient_traits::const_reference sij_a = *pivot_itr;
        ++pivot_itr;
        PPL_ASSERT(sij_a > 0);
        PPL_ASSERT(sij_b > 0);

        // Reconstitute the identity submatrix part of tableau.
        if (row_index != *i) {
          if (row_index == min_column) {
            // Optimizing for: lhs == 0 && rhs == rhs_coeff;
            new_candidates.clear();
            min_column = *i;
            sij_b = sij_a;
            new_candidates.push_back(min_column);
          }
          else
            // Optimizing for: lhs == 0 && rhs == 0;
            new_candidates.push_back(*i);
        }
      }
      WEIGHT_ADD_MUL(44, candidates.size());
    }
    else {
      // Not in base.
      const PIP_Tree_Node::Row& row = tableau[row_index];
      PIP_Tree_Node::Row::const_iterator row_itr = row.lower_bound(min_column);
      PIP_Tree_Node::Row::const_iterator row_end = row.end();
      PPL_DIRTY_TEMP_COEFFICIENT(row_jb);
      if (row_itr == row_end || row_itr.index() > min_column)
        row_jb = 0;
      else {
        PPL_ASSERT(row_itr.index() == min_column);
        row_jb = *row_itr;
        ++row_itr;
      }
      for ( ; i != i_end; ++i) {
        pivot_itr = pivot_row.find(pivot_itr, *i);
        PPL_ASSERT(pivot_itr != pivot_row.end());
        Coefficient_traits::const_reference sij_a = *pivot_itr;
        PPL_ASSERT(sij_a > 0);
        PPL_ASSERT(sij_b > 0);

        PPL_DIRTY_TEMP_COEFFICIENT(lhs);
        PPL_DIRTY_TEMP_COEFFICIENT(rhs);
        if (row_itr != row_end && row_itr.index() < *i)
          row_itr = row.lower_bound(row_itr, *i);
        PPL_DIRTY_TEMP_COEFFICIENT(row_ja);
        if (row_itr == row_end || row_itr.index() > *i)
          row_ja = 0;
        else {
          PPL_ASSERT(row_itr.index() == *i);
          row_ja = *row_itr;
          ++row_itr;
        }

        // lhs is actually the left-hand side with toggled sign.
        // rhs is actually the right-hand side with toggled sign.
        lhs = sij_b * row_ja;
        rhs = sij_a * row_jb;
        if (lhs == rhs)
          new_candidates.push_back(*i);
        else
          if (lhs < rhs) {
            new_candidates.clear();
            min_column = *i;
            row_jb = row_ja;
            sij_b = sij_a;
            new_candidates.push_back(min_column);
          }
      }
      WEIGHT_ADD_MUL(68, candidates.size());
    }
    using std::swap;
    swap(candidates, new_candidates);
  }
}

/* Find the column j in revised simplex tableau such that
  - pivot_row[j] is positive;
  - (column j) / pivot_row[j] is lexico-minimal.
*/
bool
find_lexico_minimal_column(const Matrix<PIP_Tree_Node::Row>& tableau,
                           const std::vector<dimension_type>& mapping,
                           const std::vector<bool>& basis,
                           const PIP_Tree_Node::Row& pivot_row,
                           const dimension_type start_j,
                           dimension_type& j_out) {
  WEIGHT_BEGIN();
  const dimension_type num_columns = tableau.num_columns();

  PPL_ASSERT(start_j <= pivot_row.size());
  if (start_j == pivot_row.size())
    // There are no candidates, so there is no minimum.
    return false;

  // This is used as a set, it is always sorted.
  std::vector<dimension_type> candidates;
  for (PIP_Tree_Node::Row::const_iterator i = pivot_row.lower_bound(start_j),
         i_end = pivot_row.end(); i != i_end; ++i) {
    if (*i > 0)
      candidates.push_back(i.index());
  }
  WEIGHT_ADD_MUL(201, candidates.size());

  if (candidates.empty()) {
    j_out = num_columns;
    return false;
  }

  find_lexico_minimal_column_in_set(candidates, tableau,
                                    mapping, basis, pivot_row);
  PPL_ASSERT(!candidates.empty());
  j_out = *(candidates.begin());

  return true;
}

// Computes into gcd the GCD of gcd and all coefficients in [first, last).
template <typename Iter>
void
gcd_assign_iter(Coefficient& gcd, Iter first, Iter last) {
  PPL_ASSERT(gcd != 0);
  if (gcd < 0)
    neg_assign(gcd);
  if (gcd == 1)
    return;
  WEIGHT_BEGIN();
  for ( ; first != last; ++first) {
    Coefficient_traits::const_reference coeff = *first;
    if (coeff != 0) {
      WEIGHT_ADD(24);
      gcd_assign(gcd, coeff, gcd);
      if (gcd == 1)
        return;
    }
  }
}

// Simplify row by exploiting variable integrality.
void
integral_simplification(PIP_Tree_Node::Row& row) {
  if (row[0] != 0) {
    PIP_Tree_Node::Row::const_iterator j_begin = row.begin();
    PIP_Tree_Node::Row::const_iterator j_end = row.end();
    PPL_ASSERT(j_begin != j_end && j_begin.index() == 0 && *j_begin != 0);
    /* Find next column with a non-zero value (there should be one). */
    ++j_begin;
    PPL_ASSERT(j_begin != j_end);
    for ( ; *j_begin == 0; ++j_begin)
      PPL_ASSERT(j_begin != j_end);
    /* Use it to initialize gcd. */
    PPL_DIRTY_TEMP_COEFFICIENT(gcd);
    gcd = *j_begin;
    ++j_begin;
    gcd_assign_iter(gcd, j_begin, j_end);
    if (gcd != 1) {
      PPL_DIRTY_TEMP_COEFFICIENT(mod);
      pos_rem_assign(mod, row[0], gcd);
      row[0] -= mod;
    }
  }
  /* Final normalization. */
  row.normalize();
}

// Divide all coefficients in row x and denominator y by their GCD.
void
row_normalize(PIP_Tree_Node::Row& x, Coefficient& denom) {
  if (denom == 1)
    return;
  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  gcd = denom;
  gcd_assign_iter(gcd, x.begin(), x.end());

  // Divide the coefficients by the GCD.
  WEIGHT_BEGIN();
  for (PIP_Tree_Node::Row::iterator i = x.begin(),
         i_end = x.end(); i != i_end; ++i) {
    Coefficient& x_i = *i;
    exact_div_assign(x_i, x_i, gcd);
    WEIGHT_ADD(22);
  }
  // Divide the denominator by the GCD.
  exact_div_assign(denom, denom, gcd);
}

// This is here because it is used as a template argument in
// compatibility_check_find_pivot, so it must be a global declaration.
struct compatibility_check_find_pivot_in_set_data {
  dimension_type row_index;
  // We cache cost and value to avoid calling get() multiple times.
  Coefficient cost;
  Coefficient value;
  bool operator==(const compatibility_check_find_pivot_in_set_data& x) const {
    return row_index == x.row_index;
  }
  // Needed by std::vector to sort the values.
  bool operator<(const compatibility_check_find_pivot_in_set_data& x) const {
    return row_index < x.row_index;
  }
};

void
compatibility_check_find_pivot_in_set(
    std::vector<std::pair<dimension_type,
                          compatibility_check_find_pivot_in_set_data> >&
        candidates,
    const Matrix<PIP_Tree_Node::Row>& s,
    const std::vector<dimension_type>& mapping,
    const std::vector<bool>& basis) {

  typedef compatibility_check_find_pivot_in_set_data data_struct;
  typedef std::vector<std::pair<dimension_type, data_struct> > candidates_t;
  // This is used as a set, it is always sorted.
  candidates_t new_candidates;
  const dimension_type num_vars = mapping.size();
  for (dimension_type var_index = 0; var_index < num_vars; ++var_index) {
    const dimension_type row_index = mapping[var_index];
    const bool in_base = basis[var_index];
    candidates_t::const_iterator i = candidates.begin();
    candidates_t::const_iterator i_end = candidates.end();
    PPL_ASSERT(i != i_end);
    dimension_type pj = i->first;
    Coefficient cost = i->second.cost;
    Coefficient value = i->second.value;
    new_candidates.clear();
    new_candidates.push_back(*i);
    if (in_base) {
      for (++i; i != i_end; ++i) {
        bool found_better_pivot = false;

        const dimension_type challenger_j = i->first;
        Coefficient_traits::const_reference challenger_cost = i->second.cost;
        PPL_ASSERT(value > 0);
        PPL_ASSERT(i->second.value > 0);
        PPL_ASSERT(pj < challenger_j);

        const int lhs_coeff_sgn = sgn(cost);
        const int rhs_coeff_sgn = sgn(challenger_cost);

        PPL_ASSERT(pj != challenger_j);

        // Reconstitute the identity submatrix part of tableau.
        if (row_index == pj) {
          // Optimizing for: lhs == lhs_coeff && rhs == 0;
          if (lhs_coeff_sgn == 0)
            new_candidates.push_back(*i);
          else
            found_better_pivot = (lhs_coeff_sgn > 0);
        }
        else {
          if (row_index == challenger_j) {
            // Optimizing for: lhs == 0 && rhs == rhs_coeff;
            if (rhs_coeff_sgn == 0)
              new_candidates.push_back(*i);
            else
              found_better_pivot = (0 > rhs_coeff_sgn);
          }
          else
            // Optimizing for: lhs == 0 && rhs == 0;
            new_candidates.push_back(*i);
        }
        if (found_better_pivot) {
          pj = challenger_j;
          cost = challenger_cost;
          value = i->second.value;
          new_candidates.clear();
          new_candidates.push_back(*i);
        }
      }
    }
    else {
      // Not in base.
      const PIP_Tree_Node::Row& row = s[row_index];
      PIP_Tree_Node::Row::const_iterator row_itr = row.lower_bound(pj);
      PIP_Tree_Node::Row::const_iterator new_row_itr;
      PIP_Tree_Node::Row::const_iterator row_end = row.end();
      PPL_DIRTY_TEMP_COEFFICIENT(row_value);
      if (row_itr != row_end && row_itr.index() == pj) {
        row_value = *row_itr;
        ++row_itr;
      }
      else
        row_value = 0;
      PPL_DIRTY_TEMP_COEFFICIENT(row_challenger_value);
      for (++i; i != i_end; ++i) {
        const dimension_type challenger_j = i->first;
        Coefficient_traits::const_reference challenger_cost = i->second.cost;
        Coefficient_traits::const_reference challenger_value
          = i->second.value;
        PPL_ASSERT(value > 0);
        PPL_ASSERT(challenger_value > 0);
        PPL_ASSERT(pj < challenger_j);

        new_row_itr = row.find(row_itr, challenger_j);
        if (new_row_itr != row.end()) {
          row_challenger_value = *new_row_itr;
          // Use new_row_itr as a hint in next iterations
          row_itr = new_row_itr;
        }
        else {
          row_challenger_value = 0;
          // Using end() as a hint is not useful, keep the current hint.
        }
        PPL_ASSERT(row_challenger_value == row.get(challenger_j));

        // Before computing and comparing the actual values, the signs are
        // compared. This speeds up the code, because the values' computation
        // is a bit expensive.

        const int lhs_sign = sgn(cost) * sgn(row_value);
        const int rhs_sign = sgn(challenger_cost) * sgn(row_challenger_value);

        if (lhs_sign != rhs_sign) {
          if (lhs_sign > rhs_sign) {
            pj = challenger_j;
            cost = challenger_cost;
            value = challenger_value;
            row_value = row_challenger_value;
            new_candidates.clear();
            new_candidates.push_back(*i);
          }
        }
        else {

          // Sign comparison is not enough this time.
          // Do the full computation.

          PPL_DIRTY_TEMP_COEFFICIENT(lhs);
          lhs = cost;
          lhs *= challenger_value;
          PPL_DIRTY_TEMP_COEFFICIENT(rhs);
          rhs = challenger_cost;
          rhs *= value;

          lhs *= row_value;
          rhs *= row_challenger_value;

          if (lhs == rhs)
            new_candidates.push_back(*i);
          else {
            if (lhs > rhs) {
              pj = challenger_j;
              cost = challenger_cost;
              value = challenger_value;
              row_value = row_challenger_value;
              new_candidates.clear();
              new_candidates.push_back(*i);
            }
          }
        }
      }
    }
    using std::swap;
    swap(candidates, new_candidates);
  }
}

// Returns false if there is not a positive pivot candidate.
// Otherwise, it sets pi, pj to the coordinates of the pivot in s.
bool
compatibility_check_find_pivot(const Matrix<PIP_Tree_Node::Row>& s,
                               const std::vector<dimension_type>& mapping,
                               const std::vector<bool>& basis,
                               dimension_type& pi, dimension_type& pj) {
  // Look for a negative RHS (i.e., constant term, stored in column 0),
  // maximizing pivot column.
  const dimension_type num_rows = s.num_rows();
  typedef compatibility_check_find_pivot_in_set_data data_struct;
  // This is used as a set, it is always sorted.
  typedef std::vector<std::pair<dimension_type, data_struct> > candidates_t;
  typedef std::map<dimension_type,data_struct> candidates_map_t;
  candidates_map_t candidates_map;
  for (dimension_type i = 0; i < num_rows; ++i) {
    const PIP_Tree_Node::Row& s_i = s[i];
    Coefficient_traits::const_reference s_i0 = s_i.get(0);
    if (s_i0 < 0) {
      dimension_type j;
      if (!find_lexico_minimal_column(s, mapping, basis, s_i, 1, j)) {
        // No positive pivot candidate: unfeasible problem.
        return false;
      }
      Coefficient_traits::const_reference s_ij = s_i.get(j);
      candidates_map_t::iterator itr = candidates_map.find(j);
      if (itr == candidates_map.end()) {
        data_struct& current_data = candidates_map[j];
        current_data.row_index = i;
        current_data.cost = s_i0;
        current_data.value = s_ij;
      }
      else {
        data_struct& current_data = candidates_map[j];
        PPL_ASSERT(current_data.value > 0);

        // Before computing and comparing the actual values, the signs are
        // compared. This speeds up the code, because the values' computation
        // is a bit expensive.
        const int lhs_coeff_sgn = sgn(current_data.cost);
        const int rhs_coeff_sgn = sgn(s_i0);

        if (lhs_coeff_sgn != rhs_coeff_sgn) {
          // Same column: just compare the ratios.
          // This works since all columns are lexico-positive:
          // return cst_a * sij_b > cst_b * sij_a.
          if (lhs_coeff_sgn > rhs_coeff_sgn) {
            // Found better pivot
            current_data.row_index = i;
            current_data.cost = s_i0;
            current_data.value = s_ij;
          }
          // Otherwise, keep current pivot for this column.
        }
        else {
          // Sign comparison is not enough this time.
          // Do the full computation.
          Coefficient_traits::const_reference value_b = s_i.get(j);
          PPL_ASSERT(value_b > 0);

          PPL_DIRTY_TEMP_COEFFICIENT(lhs_coeff);
          lhs_coeff = current_data.cost;
          lhs_coeff *= value_b;

          PPL_DIRTY_TEMP_COEFFICIENT(rhs_coeff);
          rhs_coeff = s_i0;
          rhs_coeff *= current_data.value;

          // Same column: just compare the ratios.
          // This works since all columns are lexico-positive:
          // return cst_a * sij_b > cst_b * sij_a.
          if (lhs_coeff > rhs_coeff) {
            // Found better pivot
            current_data.row_index = i;
            current_data.cost = s_i0;
            current_data.value = s_ij;
          }
          // Otherwise, keep current pivot for this column.
        }
      }
    }
  }
  candidates_t candidates;
  for (candidates_map_t::iterator
         i = candidates_map.begin(), i_end = candidates_map.end();
       i != i_end; ++i)
    candidates.push_back(*i);
  if (!candidates.empty()) {
    compatibility_check_find_pivot_in_set(candidates, s, mapping, basis);
    PPL_ASSERT(!candidates.empty());
    pi = candidates.begin()->second.row_index;
    pj = candidates.begin()->first;
  }
  else {
    pi = s.num_rows();
    pj = 0;
  }

  return true;
}

} // namespace

namespace IO_Operators {

std::ostream&
operator<<(std::ostream& os, const PIP_Tree_Node& x) {
  x.print(os);
  return os;
}

std::ostream&
operator<<(std::ostream& os, const PIP_Tree_Node::Artificial_Parameter& x) {
  const Linear_Expression& expr = static_cast<const Linear_Expression&>(x);
  os << "(" << expr << ") div " << x.denominator();
  return os;
}

} // namespace IO_Operators

PIP_Tree_Node::PIP_Tree_Node(const PIP_Problem* owner)
  : owner_(owner),
    parent_(0),
    constraints_(),
    artificial_parameters() {
}

PIP_Tree_Node::PIP_Tree_Node(const PIP_Tree_Node& y)
  : owner_(y.owner_),
    parent_(0), // NOTE: parent is not copied.
    constraints_(y.constraints_),
    artificial_parameters(y.artificial_parameters) {
}

PIP_Tree_Node::Artificial_Parameter
::Artificial_Parameter(const Linear_Expression& expr,
                       Coefficient_traits::const_reference d)
  : Linear_Expression(expr), denom(d) {
  if (denom == 0)
    throw std::invalid_argument("PIP_Tree_Node::Artificial_Parameter(e, d): "
                                "denominator d is zero.");

  // Normalize if needed.
  // FIXME: Provide a proper normalization helper.
  Linear_Expression& param_expr = *this;
  if (denom < 0) {
    neg_assign(denom);
    neg_assign(param_expr);
  }

  // Compute GCD of parameter expression and denominator.

  Coefficient gcd = param_expr.gcd(0, space_dimension() + 1);

  if (gcd == 1)
    return;

  if (gcd == 0)
    gcd = denom;
  else
    gcd_assign(gcd, denom, gcd);

  if (gcd == 1)
    return;

  // Divide coefficients and denominator by their (non-trivial) GCD.
  PPL_ASSERT(gcd > 1);
  param_expr.exact_div_assign(gcd, 0, space_dimension() + 1);
  Parma_Polyhedra_Library::exact_div_assign(denom, denom, gcd);

  PPL_ASSERT(OK());
}

bool
PIP_Tree_Node::Artificial_Parameter
::operator==(const PIP_Tree_Node::Artificial_Parameter& y) const {
  const Artificial_Parameter& x = *this;
  if (x.space_dimension() != y.space_dimension())
    return false;
  if (x.denom != y.denom)
    return false;
  return x.is_equal_to(y);
}

bool
PIP_Tree_Node::Artificial_Parameter
::operator!=(const PIP_Tree_Node::Artificial_Parameter& y) const {
  return !operator==(y);
}

bool
PIP_Tree_Node::Artificial_Parameter::OK() const {
  if (denom <= 0) {
#ifndef NDEBUG
    std::cerr << "PIP_Tree_Node::Artificial_Parameter "
              << "has a non-positive denominator.\n";
#endif
    return false;
  }
  return true;
}

void
PIP_Tree_Node::Artificial_Parameter::ascii_dump(std::ostream& s) const {
  s << "artificial_parameter ";
  Linear_Expression::ascii_dump(s);
  s << " / " << denom << "\n";
}

bool
PIP_Tree_Node::Artificial_Parameter::ascii_load(std::istream& s) {
  std::string str;
  if (!(s >> str) || str != "artificial_parameter")
    return false;
  if (!Linear_Expression::ascii_load(s))
    return false;
  if (!(s >> str) || str != "/")
    return false;
  if (!(s >> denom))
    return false;
  PPL_ASSERT(OK());
  return true;
}

PPL_OUTPUT_DEFINITIONS(PIP_Tree_Node::Artificial_Parameter)

PIP_Solution_Node::PIP_Solution_Node(const PIP_Problem* owner)
  : PIP_Tree_Node(owner),
    tableau(),
    basis(),
    mapping(),
    var_row(),
    var_column(),
    special_equality_row(0),
    big_dimension(not_a_dimension()),
    sign(),
    solution(),
    solution_valid(false) {
}

PIP_Solution_Node::PIP_Solution_Node(const PIP_Solution_Node& y)
  : PIP_Tree_Node(y),
    tableau(y.tableau),
    basis(y.basis),
    mapping(y.mapping),
    var_row(y.var_row),
    var_column(y.var_column),
    special_equality_row(y.special_equality_row),
    big_dimension(y.big_dimension),
    sign(y.sign),
    solution(y.solution),
    solution_valid(y.solution_valid) {
}

PIP_Solution_Node::PIP_Solution_Node(const PIP_Solution_Node& y,
                                     No_Constraints)
  : PIP_Tree_Node(y.owner_), // NOTE: only copy owner.
    tableau(y.tableau),
    basis(y.basis),
    mapping(y.mapping),
    var_row(y.var_row),
    var_column(y.var_column),
    special_equality_row(y.special_equality_row),
    big_dimension(y.big_dimension),
    sign(y.sign),
    solution(y.solution),
    solution_valid(y.solution_valid) {
}

PIP_Solution_Node::~PIP_Solution_Node() {
}

PIP_Decision_Node::PIP_Decision_Node(const PIP_Problem* owner,
                                     PIP_Tree_Node* fcp,
                                     PIP_Tree_Node* tcp)
  : PIP_Tree_Node(owner),
    false_child(fcp),
    true_child(tcp) {
  if (false_child != 0)
    false_child->set_parent(this);
  if (true_child != 0)
    true_child->set_parent(this);
}

PIP_Decision_Node::PIP_Decision_Node(const PIP_Decision_Node& y)
  : PIP_Tree_Node(y),
    false_child(0),
    true_child(0) {
  if (y.false_child != 0) {
    false_child = y.false_child->clone();
    false_child->set_parent(this);
  }
  // Protect false_child from exception safety issues via std::auto_ptr.
  std::auto_ptr<PIP_Tree_Node> wrapped_node(false_child);
  if (y.true_child != 0) {
    true_child = y.true_child->clone();
    true_child->set_parent(this);
  }
  // It is now safe to release false_child.
  wrapped_node.release();
}

PIP_Decision_Node::~PIP_Decision_Node() {
  delete false_child;
  delete true_child;
}

void
PIP_Solution_Node::set_owner(const PIP_Problem* owner) {
  owner_ = owner;
}

void
PIP_Decision_Node::set_owner(const PIP_Problem* owner) {
  owner_ = owner;
  if (false_child != 0)
    false_child->set_owner(owner);
  if (true_child != 0)
    true_child->set_owner(owner);
}

bool
PIP_Solution_Node::check_ownership(const PIP_Problem* owner) const {
  return get_owner() == owner;
}

bool
PIP_Decision_Node::check_ownership(const PIP_Problem* owner) const {
  return get_owner() == owner
    && (false_child == 0 || false_child->check_ownership(owner))
    && (true_child == 0 || true_child->check_ownership(owner));
}

const PIP_Decision_Node*
PIP_Decision_Node::as_decision() const {
  return this;
}

const PIP_Decision_Node*
PIP_Solution_Node::as_decision() const {
  return 0;
}

const PIP_Solution_Node*
PIP_Decision_Node::as_solution() const {
  return 0;
}

const PIP_Solution_Node*
PIP_Solution_Node::as_solution() const {
  return this;
}

bool
PIP_Solution_Node::Tableau::OK() const {
  if (s.num_rows() != t.num_rows()) {
#ifndef NDEBUG
    std::cerr << "PIP_Solution_Node::Tableau matrices "
              << "have a different number of rows.\n";
#endif
    return false;
  }

  if (!s.OK() || !t.OK()) {
#ifndef NDEBUG
    std::cerr << "A PIP_Solution_Node::Tableau matrix is broken.\n";
#endif
    return false;
  }

  if (denom <= 0) {
#ifndef NDEBUG
    std::cerr << "PIP_Solution_Node::Tableau with non-positive "
              << "denominator.\n";
#endif
    return false;
  }

  // All tests passed.
  return true;
}

bool
PIP_Tree_Node::OK() const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  // Parameter constraint system should contain no strict inequalities.
  for (Constraint_System::const_iterator
         i = constraints_.begin(), i_end = constraints_.end(); i != i_end; ++i)
    if (i->is_strict_inequality()) {
#ifndef NDEBUG
      cerr << "The feasible region of the PIP_Problem parameter context"
           << "is defined by a constraint system containing strict "
           << "inequalities."
           << endl;
      ascii_dump(cerr);
#endif
      return false;
    }
  return true;
}

void
PIP_Tree_Node
::add_constraint(const Row& row, const Variables_Set& parameters) {
  // Compute the expression for the parameter constraint.
  Linear_Expression expr = Linear_Expression(row.get(0));
  Variables_Set::const_iterator j = parameters.begin();
  if (!parameters.empty()) {
    // Needed to avoid reallocations in expr when iterating upward.
    add_mul_assign(expr, 0, Variable(*(parameters.rbegin())));
    // The number of increments of j plus one.
    dimension_type j_index = 1;
    Row::const_iterator i = row.begin();
    Row::const_iterator i_end = row.end();
    if (i != i_end && i.index() == 0)
      ++i;
    // NOTE: iterating in [1..num_params].
    WEIGHT_BEGIN();
    for ( ; i != i_end; ++i) {
      PPL_ASSERT(i.index() <= parameters.size());
      std::advance(j, i.index() - j_index);
      j_index = i.index();
      WEIGHT_ADD(74);
      add_mul_assign(expr, *i, Variable(*j));
    }
  }
  // Add the parameter constraint.
  constraints_.insert(expr >= 0);
}

void
PIP_Tree_Node::parent_merge() {
  const PIP_Decision_Node& parent = *parent_;

  // Merge the parent's artificial parameters.
  artificial_parameters.insert(artificial_parameters.begin(),
                               parent.art_parameter_begin(),
                               parent.art_parameter_end());

  PPL_ASSERT(OK());
}

bool
PIP_Solution_Node::OK() const {
#ifndef NDEBUG
  using std::cerr;
#endif
  if (!PIP_Tree_Node::OK())
    return false;

  // Check that every member used is OK.

  if (!tableau.OK())
    return false;

  // Check coherency of basis, mapping, var_row and var_column
  if (basis.size() != mapping.size()) {
#ifndef NDEBUG
    cerr << "The PIP_Solution_Node::basis and PIP_Solution_Node::mapping "
         << "vectors do not have the same number of elements.\n";
#endif
    return false;
  }
  if (basis.size() != var_row.size() + var_column.size()) {
#ifndef NDEBUG
    cerr << "The sum of number of elements in the PIP_Solution_Node::var_row "
         << "and PIP_Solution_Node::var_column vectors is different from the "
         << "number of elements in the PIP_Solution_Node::basis vector.\n";
#endif
    return false;
  }
  if (var_column.size() != tableau.s.num_columns()) {
#ifndef NDEBUG
    cerr << "The number of elements in the PIP_Solution_Node::var_column "
         << "vector is different from the number of columns in the "
         << "PIP_Solution_Node::tableau.s matrix.\n";
#endif
    return false;
  }
  if (var_row.size() != tableau.s.num_rows()) {
#ifndef NDEBUG
    cerr << "The number of elements in the PIP_Solution_Node::var_row "
         << "vector is different from the number of rows in the "
         << "PIP_Solution_Node::tableau.s matrix.\n";
#endif
    return false;
  }
  for (dimension_type i = mapping.size(); i-- > 0; ) {
    const dimension_type row_column = mapping[i];
    if (basis[i] && var_column[row_column] != i) {
#ifndef NDEBUG
      cerr << "Variable " << i << " is basic and corresponds to column "
           << row_column << " but PIP_Solution_Node::var_column["
           << row_column << "] does not correspond to variable " << i
           << ".\n";
#endif
      return false;
    }
    if (!basis[i] && var_row[row_column] != i) {
#ifndef NDEBUG
      cerr << "Variable " << i << " is nonbasic and corresponds to row "
           << row_column << " but PIP_Solution_Node::var_row["
           << row_column << "] does not correspond to variable " << i
           << ".\n";
#endif
      return false;
    }
  }
  // All checks passed.
  return true;
}

bool
PIP_Decision_Node::OK() const {
  // Perform base class well-formedness check on this node.
  if (!PIP_Tree_Node::OK())
    return false;

  // Recursively check if child nodes are well-formed.
  if (false_child != 0 && !false_child->OK())
    return false;
  if (true_child != 0 && !true_child->OK())
    return false;

  // Decision nodes should always have a true child.
  if (true_child == 0) {
#ifndef NDEBUG
    std::cerr << "PIP_Decision_Node with no 'true' child.\n";
#endif
    return false;
  }

  // Decision nodes with a false child must have exactly one constraint.
  if (false_child != 0) {
    const dimension_type dist = Implementation::num_constraints(constraints_);
    if (dist != 1) {
#ifndef NDEBUG
      std::cerr << "PIP_Decision_Node with a 'false' child has "
                << dist << " parametric constraints (should be 1).\n";
#endif
      return false;
    }
  }

  // All checks passed.
  return true;
}

void
PIP_Decision_Node::update_tableau(
    const PIP_Problem& pip,
    const dimension_type external_space_dim,
    const dimension_type first_pending_constraint,
    const Constraint_Sequence& input_cs,
    const Variables_Set& parameters) {

  true_child->update_tableau(pip,
                             external_space_dim,
                             first_pending_constraint,
                             input_cs,
                             parameters);
  if (false_child != 0)
    false_child->update_tableau(pip,
                                external_space_dim,
                                first_pending_constraint,
                                input_cs,
                                parameters);
  PPL_ASSERT(OK());
}

PIP_Tree_Node*
PIP_Decision_Node::solve(const PIP_Problem& pip,
                         const bool check_feasible_context,
                         const Matrix<Row>& context,
                         const Variables_Set& params,
                         dimension_type space_dim,
                         const int indent_level) {
  PPL_ASSERT(indent_level >= 0);
#ifdef NOISY_PIP_TREE_STRUCTURE
  indent_and_print(std::cerr, indent_level, "=== SOLVING DECISION NODE\n");
#else
  PPL_USED(indent_level);
#endif
  PPL_ASSERT(true_child != 0);
  Matrix<Row> context_true(context);
  Variables_Set all_params(params);
  const dimension_type num_art_params = artificial_parameters.size();
  add_artificial_parameters(context_true, all_params, space_dim,
                            num_art_params);
  merge_assign(context_true, constraints_, all_params);
  const bool has_false_child = (false_child != 0);
  const bool has_true_child = (true_child != 0);
#ifdef NOISY_PIP_TREE_STRUCTURE
  indent_and_print(std::cerr, indent_level,
                   "=== DECISION: SOLVING THEN CHILD\n");
#endif
  true_child = true_child->solve(pip, check_feasible_context,
                                 context_true, all_params, space_dim,
                                 indent_level + 1);

  if (has_false_child) {
    // Decision nodes with false child must have exactly one constraint
    PPL_ASSERT(1 == Implementation::num_constraints(constraints_));
    // NOTE: modify context_true in place, complementing its last constraint.
    Matrix<Row>& context_false = context_true;
    Row& last = context_false[context_false.num_rows() - 1];
    complement_assign(last, last, 1);
#ifdef NOISY_PIP_TREE_STRUCTURE
    indent_and_print(std::cerr, indent_level,
                     "=== DECISION: SOLVING ELSE CHILD\n");
#endif
    false_child = false_child->solve(pip, check_feasible_context,
                                     context_false, all_params, space_dim,
                                     indent_level + 1);
  }

  if (true_child == 0 && false_child == 0) {
    // No children: the whole subtree is unfeasible.
#ifdef NOISY_PIP_TREE_STRUCTURE
    indent_and_print(std::cerr, indent_level,
                     "=== DECISION: BOTH BRANCHES NOW UNFEASIBLE: _|_\n");
#endif
    delete this;
    return 0;
  }

  if (has_false_child && false_child == 0) {
    // False child has become unfeasible: merge this node's artificials with
    // the true child, while removing the local parameter constraints, which
    // are no longer discriminative.
#ifdef NOISY_PIP_TREE_STRUCTURE
    indent_and_print(std::cerr, indent_level,
                     "=== DECISION: ELSE BRANCH NOW UNFEASIBLE\n");
    indent_and_print(std::cerr, indent_level,
                     "==> merge then branch with parent.\n");
#endif
    PIP_Tree_Node* const node = true_child;
    node->parent_merge();
    node->set_parent(parent());
    true_child = 0;
    delete this;
    PPL_ASSERT(node->OK());
    return node;
  }
  else if (has_true_child && true_child == 0) {
    // True child has become unfeasible: merge this node's artificials
    // with the false child.
#ifdef NOISY_PIP_TREE_STRUCTURE
    indent_and_print(std::cerr, indent_level,
                     "=== DECISION: THEN BRANCH NOW UNFEASIBLE\n");
    indent_and_print(std::cerr, indent_level,
                     "==> merge else branch with parent.\n");
#endif
    PIP_Tree_Node* const node = false_child;
    node->parent_merge();
    node->set_parent(parent());
    false_child = 0;
    delete this;
    PPL_ASSERT(node->OK());
    return node;
  }
  else if (check_feasible_context) {
    // Test all constraints for redundancy with the context, and eliminate
    // them if not necessary.
    Constraint_System cs;
    swap(cs, constraints_);
    for (Constraint_System::const_iterator ci = cs.begin(),
           ci_end = cs.end(); ci != ci_end; ++ci) {
      Matrix<Row> ctx_copy(context);
      merge_assign(ctx_copy, Constraint_System(*ci), all_params);
      Row& last = ctx_copy[ctx_copy.num_rows()-1];
      complement_assign(last, last, 1);
      if (compatibility_check(ctx_copy)) {
        // The constraint is not redundant with the context: keep it.
        constraints_.insert(*ci);
      }
    }
    // If the constraints set has become empty, only keep the true child.
    if (constraints_.empty()) {
#ifdef NOISY_PIP_TREE_STRUCTURE
      indent_and_print(std::cerr, indent_level,
                       "=== DECISION: NO BRANCHING CONSTRAINTS LEFT\n");
      indent_and_print(std::cerr, indent_level,
                       "==> merge then branch with parent.\n");
#endif
      PIP_Tree_Node* const node = true_child;
      node->parent_merge();
      node->set_parent(parent());
      true_child = 0;
      delete this;
      PPL_ASSERT(node->OK());
      return node;
    }
  }
  PPL_ASSERT(OK());
  return this;
}

void
PIP_Decision_Node::ascii_dump(std::ostream& s) const {
  // Dump base class info.
  PIP_Tree_Node::ascii_dump(s);

  // Dump true child (if any).
  s << "\ntrue_child: ";
  if (true_child == 0) {
    // Note: this branch should normally be unreachable code, since a
    // well-formed decision node always has a true child. We keep this code
    // for debugging purposes (since we want to dump broken nodes).
    s << "BOTTOM\n";
  }
  else if (const PIP_Decision_Node* const dec = true_child->as_decision()) {
    s << "DECISION\n";
    dec->ascii_dump(s);
  }
  else {
    const PIP_Solution_Node* const sol = true_child->as_solution();
    PPL_ASSERT(sol != 0);
    s << "SOLUTION\n";
    sol->ascii_dump(s);
  }

  // Dump false child (if any).
  s << "\nfalse_child: ";
  if (false_child == 0)
    s << "BOTTOM\n";
  else if (const PIP_Decision_Node* const dec = false_child->as_decision()) {
    // Note: this branch should normally be unreachable code.
    // Since a well-formed decision node having a false child should have
    // a single context constraint, its false child will have no context
    // constraints at all, so that no further branch is possible.
    // We keep this code for debugging purposes.
    s << "DECISION\n";
    dec->ascii_dump(s);
  }
  else {
    const PIP_Solution_Node* const sol = false_child->as_solution();
    PPL_ASSERT(sol != 0);
    s << "SOLUTION\n";
    sol->ascii_dump(s);
  }
}

bool
PIP_Decision_Node::ascii_load(std::istream& s) {
  std::string str;

  // Load base class info.
  if (!PIP_Tree_Node::ascii_load(s))
    return false;

  // Release the "true" subtree (if any).
  delete true_child;
  true_child = 0;

  // Load true child (if any).
  if (!(s >> str) || str != "true_child:")
    return false;
  if (!(s >> str))
    return false;
  if (str == "BOTTOM")
    // Note: normally unreachable code (see comment on ascii_dump).
    true_child = 0;
  else if (str == "DECISION") {
    PIP_Decision_Node* const dec = new PIP_Decision_Node(0, 0, 0);
    true_child = dec;
    if (!dec->ascii_load(s))
      return false;
  }
  else if (str == "SOLUTION") {
    PIP_Solution_Node* const sol = new PIP_Solution_Node(0);
    true_child = sol;
    if (!sol->ascii_load(s))
      return false;
  }
  else
    // Unknown node kind.
    return false;

  // Release the "false" subtree (if any).
  delete false_child;
  false_child = 0;

  // Load false child (if any).
  if (!(s >> str) || str != "false_child:")
    return false;
  if (!(s >> str))
    return false;
  if (str == "BOTTOM")
    false_child = 0;
  else if (str == "DECISION") {
    // Note: normally unreachable code (see comment on ascii_dump).
    PIP_Decision_Node* const dec = new PIP_Decision_Node(0, 0, 0);
    false_child = dec;
    if (!dec->ascii_load(s))
      return false;
  }
  else if (str == "SOLUTION") {
    PIP_Solution_Node* const sol = new PIP_Solution_Node(0);
    false_child = sol;
    if (!sol->ascii_load(s))
      return false;
  }
  else
    // Unknown node kind.
    return false;

  // Loaded all info.
  PPL_ASSERT(OK());
  return true;
}


void
PIP_Solution_Node::Tableau::normalize() {
  if (denom == 1)
    return;

  const dimension_type num_rows = s.num_rows();

  // Compute global gcd.
  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  gcd = denom;
  for (dimension_type i = num_rows; i-- > 0; ) {
    WEIGHT_BEGIN();
    const Row& s_i = s[i];
    for (Row::const_iterator
           j = s_i.begin(), j_end = s_i.end(); j != j_end; ++j) {
      Coefficient_traits::const_reference s_ij = *j;
      if (s_ij != 0) {
        WEIGHT_ADD(30);
        gcd_assign(gcd, s_ij, gcd);
        if (gcd == 1)
          return;
      }
    }
    WEIGHT_BEGIN();
    const Row& t_i = t[i];
    for (Row::const_iterator
           j = t_i.begin(), j_end = t_i.end(); j != j_end; ++j) {
      Coefficient_traits::const_reference t_ij = *j;
      if (t_ij != 0) {
        WEIGHT_ADD(14);
        gcd_assign(gcd, t_ij, gcd);
        if (gcd == 1)
          return;
      }
    }
  }
  PPL_ASSERT(gcd > 1);
  // Normalize all coefficients.
  WEIGHT_BEGIN();
  for (dimension_type i = num_rows; i-- > 0; ) {
    Row& s_i = s[i];
    for (Row::iterator j = s_i.begin(), j_end = s_i.end(); j != j_end; ++j) {
      Coefficient& s_ij = *j;
      WEIGHT_ADD(19);
      exact_div_assign(s_ij, s_ij, gcd);
    }
    Row& t_i = t[i];
    for (Row::iterator j = t_i.begin(), j_end = t_i.end(); j != j_end; ++j) {
      Coefficient& t_ij = *j;
      WEIGHT_ADD(27);
      exact_div_assign(t_ij, t_ij, gcd);
    }
  }
  // Normalize denominator.
  exact_div_assign(denom, denom, gcd);
}

void
PIP_Solution_Node::Tableau::scale(Coefficient_traits::const_reference ratio) {
  WEIGHT_BEGIN();
  for (dimension_type i = s.num_rows(); i-- > 0; ) {
    Row& s_i = s[i];
    for (Row::iterator j = s_i.begin(), j_end = s_i.end(); j != j_end; ++j) {
      WEIGHT_ADD(19);
      *j *= ratio;
    }
    Row& t_i = t[i];
    for (Row::iterator j = t_i.begin(), j_end = t_i.end(); j != j_end; ++j) {
      WEIGHT_ADD(25);
      *j *= ratio;
    }
  }
  denom *= ratio;
}

bool
PIP_Solution_Node::Tableau
::is_better_pivot(const std::vector<dimension_type>& mapping,
                  const std::vector<bool>& basis,
                  const dimension_type row_0,
                  const dimension_type col_0,
                  const dimension_type row_1,
                  const dimension_type col_1) const {
  const dimension_type num_params = t.num_columns();
  const dimension_type num_rows = s.num_rows();
  const Row& s_0 = s[row_0];
  const Row& s_1 = s[row_1];
  Coefficient_traits::const_reference s_0_0 = s_0.get(col_0);
  Coefficient_traits::const_reference s_1_1 = s_1.get(col_1);
  const Row& t_0 = t[row_0];
  const Row& t_1 = t[row_1];
  PPL_DIRTY_TEMP_COEFFICIENT(product_0);
  PPL_DIRTY_TEMP_COEFFICIENT(product_1);
  WEIGHT_BEGIN();
  // On exit from the loop, if j_mismatch == num_params then
  // no column mismatch was found.
  dimension_type j_mismatch = num_params;
  Row::const_iterator j0 = t_0.end();
  Row::const_iterator j0_end = t_0.end();
  Row::const_iterator j1 = t_1.end();
  Row::const_iterator j1_end = t_1.end();
  for (dimension_type i = 0; i < num_rows; ++i) {
    const Row& s_i = s[i];
    Coefficient_traits::const_reference s_i_col_0 = s_i.get(col_0);
    Coefficient_traits::const_reference s_i_col_1 = s_i.get(col_1);
    j0 = t_0.begin();
    j1 = t_1.begin();
    while (j0 != j0_end && j1 != j1_end) {
      if (j0.index() == j1.index()) {
        WEIGHT_ADD(1361);
        product_0 = (*j0) * s_1_1 * s_i_col_0;
        product_1 = (*j1) * s_0_0 * s_i_col_1;
        if (product_0 != product_1) {
          // Mismatch found: exit from both loops.
          j_mismatch = j0.index();
          goto end_loop;
        }
        ++j0;
        ++j1;
      }
      else
        if (j0.index() < j1.index()) {
          if (*j0 != 0 && s_1_1 != 0 && s_i_col_0 != 0) {
            // Mismatch found: exit from both loops.
            j_mismatch = j0.index();
            goto end_loop;
          }
          ++j0;
        }
        else {
          PPL_ASSERT(j0.index() > j1.index());
          if (*j1 != 0 && s_0_0 != 0 && s_i_col_1 != 0) {
            // Mismatch found: exit from both loops.
            j_mismatch = j1.index();
            goto end_loop;
          }
          ++j1;
        }
    }
    while (j0 != j0_end) {
      if (*j0 != 0 && s_1_1 != 0 && s_i_col_0 != 0) {
        // Mismatch found: exit from both loops.
        j_mismatch = j0.index();
        goto end_loop;
      }
      ++j0;
    }
    while (j1 != j1_end) {
      if (*j1 != 0 && s_0_0 != 0 && s_i_col_1 != 0) {
        // Mismatch found: exit from both loops.
        j_mismatch = j1.index();
        goto end_loop;
      }
    }
  }

 end_loop:
  return (j_mismatch != num_params)
    && column_lower(s, mapping, basis, s_0, col_0, s_1, col_1, *j0, *j1);
}

void
PIP_Tree_Node::ascii_dump(std::ostream& s) const {
  s << "constraints_\n";
  constraints_.ascii_dump(s);
  const dimension_type artificial_parameters_size
    = artificial_parameters.size();
  s << "\nartificial_parameters( " << artificial_parameters_size << " )\n";
  for (dimension_type i = 0; i < artificial_parameters_size; ++i)
    artificial_parameters[i].ascii_dump(s);
}

bool
PIP_Tree_Node::ascii_load(std::istream& s) {
  std::string str;
  if (!(s >> str) || str != "constraints_")
    return false;
  constraints_.ascii_load(s);

  if (!(s >> str) || str != "artificial_parameters(")
    return false;
  dimension_type artificial_parameters_size;
  if (!(s >> artificial_parameters_size))
    return false;
  if (!(s >> str) || str != ")")
    return false;
  Artificial_Parameter ap;
  for (dimension_type i = 0; i < artificial_parameters_size; ++i) {
    if (!ap.ascii_load(s))
      return false;
    artificial_parameters.push_back(ap);
  }

  // Note: do not assert OK() here.
  // The node invariants should be checked on derived nodes.
  return true;
}

PIP_Tree_Node*
PIP_Solution_Node::clone() const {
  return new PIP_Solution_Node(*this);
}

PIP_Tree_Node*
PIP_Decision_Node::clone() const {
  return new PIP_Decision_Node(*this);
}

void
PIP_Solution_Node::Tableau::ascii_dump(std::ostream& os) const {
  os << "denominator " << denom << "\n";
  os << "variables ";
  s.ascii_dump(os);
  os << "parameters ";
  t.ascii_dump(os);
}

bool
PIP_Solution_Node::Tableau::ascii_load(std::istream& is) {
  std::string str;
  if (!(is >> str) || str != "denominator")
    return false;
  Coefficient d;
  if (!(is >> d))
    return false;
  denom = d;
  if (!(is >> str) || str != "variables")
    return false;
  if (!s.ascii_load(is))
    return false;
  if (!(is >> str) || str != "parameters")
    return false;
  if (!t.ascii_load(is))
    return false;
  PPL_ASSERT(OK());
  return true;
}

void
PIP_Solution_Node::ascii_dump(std::ostream& os) const {
  PIP_Tree_Node::ascii_dump(os);

  os << "\ntableau\n";
  tableau.ascii_dump(os);

  os << "\nbasis ";
  const dimension_type basis_size = basis.size();
  os << basis_size;
  for (dimension_type i = 0; i < basis_size; ++i)
    os << (basis[i] ? " true" : " false");

  os << "\nmapping ";
  const dimension_type mapping_size = mapping.size();
  os << mapping_size;
  for (dimension_type i = 0; i < mapping_size; ++i)
    os << " " << mapping[i];

  os << "\nvar_row ";
  const dimension_type var_row_size = var_row.size();
  os << var_row_size;
  for (dimension_type i = 0; i < var_row_size; ++i)
    os << " " << var_row[i];

  os << "\nvar_column ";
  const dimension_type var_column_size = var_column.size();
  os << var_column_size;
  for (dimension_type i = 0; i < var_column_size; ++i)
    os << " " << var_column[i];
  os << "\n";

  os << "special_equality_row " << special_equality_row << "\n";
  os << "big_dimension " << big_dimension << "\n";

  os << "sign ";
  const dimension_type sign_size = sign.size();
  os << sign_size;
  for (dimension_type i = 0; i < sign_size; ++i) {
    os << " ";
    switch (sign[i]) {
    case UNKNOWN:
      os << "UNKNOWN";
      break;
    case ZERO:
      os << "ZERO";
      break;
    case POSITIVE:
      os << "POSITIVE";
      break;
    case NEGATIVE:
      os << "NEGATIVE";
      break;
    case MIXED:
      os << "MIXED";
      break;
    }
  }
  os << "\n";

  const dimension_type solution_size = solution.size();
  os << "solution " << solution_size << "\n";
  for (dimension_type i = 0; i < solution_size; ++i)
    solution[i].ascii_dump(os);
  os << "\n";

  os << "solution_valid " << (solution_valid ? "true" : "false") << "\n";
}

bool
PIP_Solution_Node::ascii_load(std::istream& is) {
  if (!PIP_Tree_Node::ascii_load(is))
    return false;

  std::string str;
  if (!(is >> str) || str != "tableau")
    return false;
  if (!tableau.ascii_load(is))
    return false;

  if (!(is >> str) || str != "basis")
    return false;
  dimension_type basis_size;
  if (!(is >> basis_size))
    return false;
  basis.clear();
  for (dimension_type i = 0; i < basis_size; ++i) {
    if (!(is >> str))
      return false;
    bool val = false;
    if (str == "true")
      val = true;
    else if (str != "false")
      return false;
    basis.push_back(val);
  }

  if (!(is >> str) || str != "mapping")
    return false;
  dimension_type mapping_size;
  if (!(is >> mapping_size))
    return false;
  mapping.clear();
  for (dimension_type i = 0; i < mapping_size; ++i) {
    dimension_type val;
    if (!(is >> val))
      return false;
    mapping.push_back(val);
  }

  if (!(is >> str) || str != "var_row")
    return false;
  dimension_type var_row_size;
  if (!(is >> var_row_size))
    return false;
  var_row.clear();
  for (dimension_type i = 0; i < var_row_size; ++i) {
    dimension_type val;
    if (!(is >> val))
      return false;
    var_row.push_back(val);
  }

  if (!(is >> str) || str != "var_column")
    return false;
  dimension_type var_column_size;
  if (!(is >> var_column_size))
    return false;
  var_column.clear();
  for (dimension_type i = 0; i < var_column_size; ++i) {
    dimension_type val;
    if (!(is >> val))
      return false;
    var_column.push_back(val);
  }

  if (!(is >> str) || str != "special_equality_row")
    return false;
  if (!(is >> special_equality_row))
    return false;

  if (!(is >> str) || str != "big_dimension")
    return false;
  if (!(is >> big_dimension))
    return false;

  if (!(is >> str) || str != "sign")
    return false;
  dimension_type sign_size;
  if (!(is >> sign_size))
    return false;
  sign.clear();
  for (dimension_type i = 0; i < sign_size; ++i) {
    if (!(is >> str))
      return false;
    Row_Sign val;
    if (str == "UNKNOWN")
      val = UNKNOWN;
    else if (str == "ZERO")
      val = ZERO;
    else if (str == "POSITIVE")
      val = POSITIVE;
    else if (str == "NEGATIVE")
      val = NEGATIVE;
    else if (str == "MIXED")
      val = MIXED;
    else
      return false;
    sign.push_back(val);
  }

  if (!(is >> str) || str != "solution")
    return false;
  dimension_type solution_size;
  if (!(is >> solution_size))
    return false;
  solution.clear();
  for (dimension_type i = 0; i < solution_size; ++i) {
    Linear_Expression val;
    if (!val.ascii_load(is))
      return false;
    solution.push_back(val);
  }

  if (!(is >> str) || str != "solution_valid")
    return false;
  if (!(is >> str))
    return false;
  if (str == "true")
    solution_valid = true;
  else if (str == "false")
    solution_valid = false;
  else
    return false;

  PPL_ASSERT(OK());
  return true;
}

PIP_Solution_Node::Row_Sign
PIP_Solution_Node::row_sign(const Row& x,
                            const dimension_type big_dimension) {
  if (big_dimension != not_a_dimension()) {
    // If a big parameter has been set and its coefficient is not zero,
    // then return the sign of the coefficient.
    Coefficient_traits::const_reference x_big = x.get(big_dimension);
    if (x_big > 0)
      return POSITIVE;
    if (x_big < 0)
      return NEGATIVE;
    // Otherwise x_big == 0, then no big parameter involved.
  }

  PIP_Solution_Node::Row_Sign sign = ZERO;
  for (Row::const_iterator i = x.begin(), i_end = x.end(); i != i_end; ++i) {
    Coefficient_traits::const_reference x_i = *i;
    if (x_i > 0) {
      if (sign == NEGATIVE)
        return MIXED;
      sign = POSITIVE;
    }
    else if (x_i < 0) {
      if (sign == POSITIVE)
        return MIXED;
      sign = NEGATIVE;
    }
  }
  return sign;
}

bool
PIP_Tree_Node::compatibility_check(const Matrix<Row>& context, const Row& row) {
  // CHECKME: do `context' and `row' have compatible (row) capacity?
  Matrix<Row> s(context);
  s.add_row(row);
  return compatibility_check(s);
}

bool
PIP_Tree_Node::compatibility_check(Matrix<Row>& s) {
  PPL_ASSERT(s.OK());
  // Note: num_rows may increase.
  dimension_type num_rows = s.num_rows();
  const dimension_type num_columns = s.num_columns();
  const dimension_type num_vars = num_columns - 1;

  std::vector<Coefficient> scaling(num_rows, 1);
  std::vector<bool> basis;
  basis.reserve(num_vars + num_rows);
  std::vector<dimension_type> mapping;
  mapping.reserve(num_vars + num_rows);
  std::vector<dimension_type> var_row;
  var_row.reserve(num_rows);
  std::vector<dimension_type> var_column;
  var_column.reserve(num_columns);

  // Column 0 is the constant term, not a variable
  var_column.push_back(not_a_dimension());
  for (dimension_type j = 1; j <= num_vars; ++j) {
    basis.push_back(true);
    mapping.push_back(j);
    var_column.push_back(j - 1);
  }
  for (dimension_type i = 0; i < num_rows; ++i) {
    basis.push_back(false);
    mapping.push_back(i);
    var_row.push_back(i + num_vars);
  }

  // Scaling factor (i.e., denominator) for pivot coefficients.
  PPL_DIRTY_TEMP_COEFFICIENT(pivot_denom);
  // Allocate once and for all: short life temporaries.
  PPL_DIRTY_TEMP_COEFFICIENT(product);
  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  PPL_DIRTY_TEMP_COEFFICIENT(scale_factor);

  // Perform simplex pivots on the context
  // until we find an empty solution or an optimum.
  while (true) {
    // Check if the client has requested abandoning all expensive
    // computations. If so, the exception specified by the client
    // is thrown now.
    maybe_abandon();

    // pi is the pivot's row index.
    dimension_type pi = num_rows;
    // pj is the pivot's column index.
    dimension_type pj = 0;

    const bool found_positive_pivot_candidate
      = compatibility_check_find_pivot(s, mapping, basis, pi, pj);

    if (!found_positive_pivot_candidate)
      return false;

    if (pj == 0) {
      // No negative RHS: fractional optimum found.
      // If it is integer, then the test is successful.
      // Otherwise, generate a new cut.
      bool all_integer_vars = true;
      // NOTE: iterating downwards would be correct, but it would change
      // the ordering of cut generation.
      WEIGHT_BEGIN();
      for (dimension_type i = 0; i < num_vars; ++i) {
        if (basis[i])
          // Basic variable = 0, hence integer.
          continue;
        // Not a basic variable.
        WEIGHT_ADD(70);
        const dimension_type mi = mapping[i];
        Coefficient_traits::const_reference denom = scaling[mi];
        if (s[mi].get(0) % denom == 0)
          continue;
        // Here constant term is not integer.
        all_integer_vars = false;
        // Generate a new cut.
        var_row.push_back(mapping.size());
        basis.push_back(false);
        mapping.push_back(num_rows);
        s.add_zero_rows(1);
        Row& cut = s[num_rows];
        ++num_rows;
        const Row& s_mi = s[mi];
        cut = s_mi;
        for (Row::iterator
               j = cut.begin(), j_end = cut.end(); j != j_end; ++j) {
          WEIGHT_ADD(32);
          pos_rem_assign(*j, *j, denom);
        }
        cut[0] -= denom;
        scaling.push_back(denom);
      }
      // Check if an integer solution was found.
      if (all_integer_vars)
        return true;
      else
        continue;
    }

    // Here we have a positive s[pi][pj] pivot.

    // Normalize the tableau before pivoting.
    for (dimension_type i = num_rows; i-- > 0; )
      row_normalize(s[i], scaling[i]);

    // Update basis.
    {
      const dimension_type var_pi = var_row[pi];
      const dimension_type var_pj = var_column[pj];
      var_row[pi] = var_pj;
      var_column[pj] = var_pi;
      basis[var_pi] = true;
      basis[var_pj] = false;
      mapping[var_pi] = pj;
      mapping[var_pj] = pi;
    }

    // Create an identity row corresponding to basic variable pj.
    s.add_zero_rows(1);
    Row& pivot = s[num_rows];
    pivot[pj] = 1;

    // Swap identity row with the pivot row previously found.
    using std::swap;
    swap(pivot, s[pi]);
    // Save original pivot scaling factor in a temporary,
    // then reset scaling factor for identity row.
    pivot_denom = scaling[pi];
    scaling[pi] = 1;

    // Perform a pivot operation on the matrix.
    Coefficient_traits::const_reference pivot_pj = pivot.get(pj);
    {
      for (Row::const_iterator
             j = pivot.begin(), j_end = pivot.end(); j != j_end; ++j) {
        if (j.index() == pj)
          continue;
        Coefficient_traits::const_reference pivot_j = *j;
        // Do nothing if the j-th pivot element is zero.
        if (pivot_j == 0)
          continue;
        WEIGHT_BEGIN();
        for (dimension_type i = num_rows; i-- > 0; ) {
          Row& s_i = s[i];
          product = s_i.get(pj) * pivot_j;
          if (product % pivot_pj != 0) {
            WEIGHT_ADD(103);
            // Must scale row s_i to stay in integer case.
            gcd_assign(gcd, product, pivot_pj);
            exact_div_assign(scale_factor, pivot_pj, gcd);
            for (Row::iterator
                   k = s_i.begin(), k_end = s_i.end(); k != k_end; ++k) {
              WEIGHT_ADD(30);
              *k *= scale_factor;
            }
            product *= scale_factor;
            scaling[i] *= scale_factor;
          }
          PPL_ASSERT(product % pivot_pj == 0);
          exact_div_assign(product, product, pivot_pj);
          s_i[j.index()] -= product;
          WEIGHT_ADD(134);
        }
      }
    }
    // Update column only if pivot coordinate != 1.
    if (pivot_pj != pivot_denom) {
      WEIGHT_BEGIN();
      for (dimension_type i = num_rows; i-- > 0; ) {
        Row& s_i = s[i];
        Coefficient& s_i_pj = s_i[pj];
        product = s_i_pj * pivot_denom;
        if (product % pivot_pj != 0) {
          WEIGHT_ADD(98);
          // As above, perform row scaling.
          gcd_assign(gcd, product, pivot_pj);
          exact_div_assign(scale_factor, pivot_pj, gcd);
          for (Row::iterator
                 k = s_i.begin(), k_end = s_i.end(); k != k_end; ++k) {
            WEIGHT_ADD(26);
            *k *= scale_factor;
          }
          product *= scale_factor;
          scaling[i] *= scale_factor;
        }
        PPL_ASSERT(product % pivot_pj == 0);
        exact_div_assign(s_i_pj, product, pivot_pj);
        WEIGHT_ADD(97);
      }
    }
    // Drop pivot to restore proper matrix size.
    s.remove_trailing_rows(1);
  }

  // This point should be unreachable.
  PPL_UNREACHABLE;
  return false;
}

void
PIP_Solution_Node
::update_tableau(const PIP_Problem& pip,
                 const dimension_type external_space_dim,
                 const dimension_type first_pending_constraint,
                 const Constraint_Sequence& input_cs,
                 const Variables_Set& parameters) {

  // Make sure a parameter column exists, for the inhomogeneous term.
  if (tableau.t.num_columns() == 0)
    tableau.t.add_zero_columns(1);

  // NOTE: here 'params' stands for problem (i.e., non artificial) parameters.
  const dimension_type old_num_vars = tableau.s.num_columns();
  const dimension_type old_num_params
    = pip.internal_space_dim - old_num_vars;
  const dimension_type num_added_dims
    = pip.external_space_dim - pip.internal_space_dim;
  const dimension_type new_num_params = parameters.size();
  const dimension_type num_added_params = new_num_params - old_num_params;
  const dimension_type num_added_vars = num_added_dims - num_added_params;

  // Resize the two tableau matrices.
  if (num_added_vars > 0)
    tableau.s.add_zero_columns(num_added_vars);

  if (num_added_params > 0)
    tableau.t.add_zero_columns(num_added_params, old_num_params + 1);

  dimension_type new_var_column = old_num_vars;
  const dimension_type initial_space_dim = old_num_vars + old_num_params;
  for (dimension_type i = initial_space_dim; i < external_space_dim; ++i) {
    if (parameters.count(i) == 0) {
      // A new problem variable.
      if (tableau.s.num_rows() == 0) {
        // No rows have been added yet
        basis.push_back(true);
        mapping.push_back(new_var_column);
      }
      else {
        /*
          Need to insert the original variable id
          before the slack variable id's to respect variable ordering.
        */
        basis.insert(nth_iter(basis, new_var_column), true);
        mapping.insert(nth_iter(mapping, new_var_column), new_var_column);
        // Update variable id's of slack variables.
        for (dimension_type j = var_row.size(); j-- > 0; )
          if (var_row[j] >= new_var_column)
            ++var_row[j];
        for (dimension_type j = var_column.size(); j-- > 0; )
          if (var_column[j] >= new_var_column)
            ++var_column[j];
        if (special_equality_row > 0)
          ++special_equality_row;
      }
      var_column.push_back(new_var_column);
      ++new_var_column;
    }
  }

  if (big_dimension == not_a_dimension()
      && pip.big_parameter_dimension != not_a_dimension()) {
    // Compute the column number of big parameter in tableau.t matrix.
    Variables_Set::const_iterator pos
      = parameters.find(pip.big_parameter_dimension);
    big_dimension = 1U
      + static_cast<dimension_type>(std::distance(parameters.begin(), pos));
  }

  Coefficient_traits::const_reference denom = tableau.denominator();

  for (Constraint_Sequence::const_iterator
         c_iter = nth_iter(input_cs, first_pending_constraint),
         c_end = input_cs.end(); c_iter != c_end; ++c_iter) {
    const Constraint& constraint = *c_iter;
    // (Tentatively) Add new rows to s and t matrices.
    // These will be removed at the end if they turn out to be useless.
    const dimension_type row_id = tableau.s.num_rows();
    tableau.s.add_zero_rows(1);
    tableau.t.add_zero_rows(1);
    Row& v_row = tableau.s[row_id];
    Row& p_row = tableau.t[row_id];

    {
      dimension_type p_index = 1;
      dimension_type v_index = 0;
      // Setting the inhomogeneous term.
      if (constraint.inhomogeneous_term() != 0) {
        Coefficient& p_row0 = p_row[0];
        p_row0 = constraint.inhomogeneous_term();
        if (constraint.is_strict_inequality())
          // Transform (expr > 0) into (expr - 1 >= 0).
          --p_row0;
        p_row0 *= denom;
      }
      else {
        if (constraint.is_strict_inequality())
          // Transform (expr > 0) into (expr - 1 >= 0).
          neg_assign(p_row[0], denom);
      }
      WEIGHT_BEGIN();
      dimension_type last_dim = 0;
      const Constraint::expr_type e = constraint.expression();
      for (Constraint::expr_type::const_iterator
          i = e.begin(), i_end = e.end(); i != i_end; ++i) {
        const dimension_type dim = i.variable().space_dimension();
        if (dim != last_dim + 1) {
          // We have skipped some zero coefficients.
          // Update p_index and v_index accordingly.
          const dimension_type n
            = std::distance(parameters.lower_bound(last_dim),
                            parameters.lower_bound(dim - 1));
          const dimension_type num_skipped = dim - last_dim - 1;
          p_index += n;
          v_index += (num_skipped - n);
        }
        PPL_ASSERT(p_index + v_index == i.variable().id() + 1);
        const bool is_parameter = (1 == parameters.count(dim - 1));
        Coefficient_traits::const_reference coeff_i = *i;

        WEIGHT_ADD(140);
        if (is_parameter) {
          p_row.insert(p_index, coeff_i * denom);
          ++p_index;
        }
        else {
          const dimension_type mv = mapping[v_index];
          if (basis[v_index]) {
            // Basic variable: add coeff_i * x_i
            add_mul_assign(v_row[mv], coeff_i, denom);
          }
          else {
            // Non-basic variable: add coeff_i * row_i
            add_mul_assign_row(v_row, coeff_i, tableau.s[mv]);
            add_mul_assign_row(p_row, coeff_i, tableau.t[mv]);
          }
          ++v_index;
        }

        last_dim = dim;
      }
    }

    if (row_sign(v_row, not_a_dimension()) == ZERO) {
      // Parametric-only constraints have already been inserted in
      // initial context, so no need to insert them in the tableau.
      tableau.s.remove_trailing_rows(1);
      tableau.t.remove_trailing_rows(1);
    }
    else {
      const dimension_type var_id = mapping.size();
      sign.push_back(row_sign(p_row, big_dimension));
      basis.push_back(false);
      mapping.push_back(row_id);
      var_row.push_back(var_id);
      if (constraint.is_equality()) {
        // Handle equality constraints.
        // After having added the f_i(x,p) >= 0 constraint,
        // we must add -f_i(x,p) to the special equality row.
        if (special_equality_row == 0 || basis[special_equality_row]) {
          // The special constraint has not been created yet
          // FIXME: for now, we do not handle the case where the variable
          // is basic, and we just create a new row.
          // This might be faster however.
          tableau.s.add_zero_rows(1);
          tableau.t.add_zero_rows(1);
          // NOTE: addition of rows invalidates references v_row and p_row
          // due to possible matrix reallocations: recompute them.
          neg_assign_row(tableau.s[1 + row_id], tableau.s[row_id]);
          neg_assign_row(tableau.t[1 + row_id], tableau.t[row_id]);
          sign.push_back(row_sign(tableau.t[1 + row_id], big_dimension));
          special_equality_row = mapping.size();
          basis.push_back(false);
          mapping.push_back(1 + row_id);
          var_row.push_back(1 + var_id);
        }
        else {
          // The special constraint already exists and is nonbasic.
          const dimension_type m_eq = mapping[special_equality_row];
          sub_assign(tableau.s[m_eq], v_row);
          sub_assign(tableau.t[m_eq], p_row);
          sign[m_eq] = row_sign(tableau.t[m_eq], big_dimension);
        }
      }
    }
  }
  PPL_ASSERT(OK());
}

PIP_Tree_Node*
PIP_Solution_Node::solve(const PIP_Problem& pip,
                         const bool check_feasible_context,
                         const Matrix<Row>& context,
                         const Variables_Set& params,
                         dimension_type space_dim,
                         const int indent_level) {
  PPL_ASSERT(indent_level >= 0);
#ifdef NOISY_PIP_TREE_STRUCTURE
  indent_and_print(std::cerr, indent_level, "=== SOLVING NODE\n");
#else
  PPL_USED(indent_level);
#endif
  // Reset current solution as invalid.
  solution_valid = false;

  Matrix<Row> ctx(context);
  Variables_Set all_params(params);
  const dimension_type num_art_params = artificial_parameters.size();
  add_artificial_parameters(ctx, all_params, space_dim, num_art_params);
  merge_assign(ctx, constraints_, all_params);

  // If needed, (re-)check feasibility of context.
  if (check_feasible_context) {
    Matrix<Row> ctx_copy(ctx);
    if (!compatibility_check(ctx_copy)) {
      delete this;
      return 0;
    }
  }

  const dimension_type not_a_dim = not_a_dimension();

  // Main loop of the simplex algorithm.
  while (true) {
    // Check if the client has requested abandoning all expensive
    // computations. If so, the exception specified by the client
    // is thrown now.
    maybe_abandon();

    PPL_ASSERT(OK());

    const dimension_type num_rows = tableau.t.num_rows();
    const dimension_type num_vars = tableau.s.num_columns();
    const dimension_type num_params = tableau.t.num_columns();
    Coefficient_traits::const_reference tableau_denom = tableau.denominator();

#ifdef VERY_NOISY_PIP
    tableau.ascii_dump(std::cerr);
    std::cerr << "context ";
    ctx.ascii_dump(std::cerr);
#endif // #ifdef VERY_NOISY_PIP

    // (Re-) Compute parameter row signs.
    // While at it, keep track of the first parameter rows
    // having negative and mixed sign.
    dimension_type first_negative = not_a_dim;
    dimension_type first_mixed = not_a_dim;
    for (dimension_type i = 0; i < num_rows; ++i) {
      Row_Sign& sign_i = sign[i];
      if (sign_i == UNKNOWN || sign_i == MIXED)
        sign_i = row_sign(tableau.t[i], big_dimension);

      if (sign_i == NEGATIVE && first_negative == not_a_dim)
        first_negative = i;
      else if (sign_i == MIXED && first_mixed == not_a_dim)
        first_mixed = i;
    }

    // If no negative parameter row was found, try to refine the sign of
    // mixed rows using compatibility checks with the current context.
    if (first_negative == not_a_dim && first_mixed != not_a_dim) {
      for (dimension_type i = first_mixed; i < num_rows; ++i) {
        // Consider mixed sign parameter rows only.
        if (sign[i] != MIXED)
          continue;
        const Row& t_i = tableau.t[i];
        Row_Sign new_sign = ZERO;
        // Check compatibility for constraint t_i(z) >= 0.
        if (compatibility_check(ctx, t_i))
          new_sign = POSITIVE;
        // Check compatibility for constraint t_i(z) < 0,
        // i.e., -t_i(z) - 1 >= 0.
        Row t_i_complement(num_params);
        complement_assign(t_i_complement, t_i, tableau_denom);
        if (compatibility_check(ctx, t_i_complement))
          new_sign = (new_sign == POSITIVE) ? MIXED : NEGATIVE;
        // Update sign for parameter row i.
        sign[i] = new_sign;
        // Maybe update first_negative and first_mixed.
        if (new_sign == NEGATIVE && first_negative == not_a_dim) {
          first_negative = i;
          if (i == first_mixed)
            first_mixed = not_a_dim;
        }
        else if (new_sign == MIXED) {
          if (first_mixed == not_a_dim)
            first_mixed = i;
        }
        else if (i == first_mixed)
          first_mixed = not_a_dim;
      }
    }

    // If there still is no negative parameter row and a mixed sign
    // parameter row (first_mixed) such that:
    //  - it has at least one positive variable coefficient;
    //  - constraint t_i(z) > 0 is not compatible with the context;
    // then this parameter row can be considered negative.
    if (first_negative == not_a_dim && first_mixed != not_a_dim) {
      WEIGHT_BEGIN();
      for (dimension_type i = first_mixed; i < num_rows; ++i) {
        // Consider mixed sign parameter rows only.
        if (sign[i] != MIXED)
          continue;
        // Check for a positive variable coefficient.
        const Row& s_i = tableau.s[i];
        bool has_positive = false;
        {
          for (Row::const_iterator
                 j = s_i.begin(), j_end = s_i.end(); j != j_end; ++j)
            if (*j > 0) {
              has_positive = true;
              break;
            }
        }
        if (!has_positive)
          continue;
        // Check compatibility of constraint t_i(z) > 0.
        Row row(tableau.t[i]);
        PPL_DIRTY_TEMP_COEFFICIENT(mod);
        Coefficient& row0 = row[0];
        pos_rem_assign(mod, row0, tableau_denom);
        row0 -= (mod == 0) ? tableau_denom : mod;
        WEIGHT_ADD(210);
        const bool compatible = compatibility_check(ctx, row);
        // Maybe update sign (and first_* indices).
        if (compatible) {
          // Sign is still mixed.
          if (first_mixed == not_a_dim)
            first_mixed = i;
        }
        else {
          // Sign becomes negative (i.e., no longer mixed).
          sign[i] = NEGATIVE;
          if (first_negative == not_a_dim)
            first_negative = i;
          if (first_mixed == i)
            first_mixed = not_a_dim;
        }
      }
    }

#ifdef VERY_NOISY_PIP
    std::cerr << "sign =";
    for (dimension_type i = 0; i < sign.size(); ++i)
      std::cerr << " " << "?0+-*"[sign[i]];
    std::cerr << std::endl;
#endif // #ifdef VERY_NOISY_PIP

    // If we have found a negative parameter row, then
    // either the problem is unfeasible, or a pivoting step is required.
    if (first_negative != not_a_dim) {

      // Search for the best pivot row.
      dimension_type pi = not_a_dim;
      dimension_type pj = not_a_dim;
      for (dimension_type i = first_negative; i < num_rows; ++i) {
        if (sign[i] != NEGATIVE)
          continue;
        dimension_type j;
        if (!find_lexico_minimal_column(tableau.s, mapping, basis,
                                        tableau.s[i], 0, j)) {
          // No positive s_ij was found: problem is unfeasible.
#ifdef NOISY_PIP_TREE_STRUCTURE
          indent_and_print(std::cerr, indent_level,
                           "No positive pivot: Solution = _|_\n");
#endif // #ifdef NOISY_PIP_TREE_STRUCTURE
          delete this;
          return 0;
        }
        if (pj == not_a_dim
            || tableau.is_better_pivot(mapping, basis, i, j, pi, pj)) {
          // Update pivot indices.
          pi = i;
          pj = j;
          if (pip.control_parameters[PIP_Problem::PIVOT_ROW_STRATEGY]
              == PIP_Problem::PIVOT_ROW_STRATEGY_FIRST)
            // Stop at first valid row.
            break;
        }
      }

#ifdef VERY_NOISY_PIP
      std::cerr << "Pivot (pi, pj) = (" << pi << ", " << pj << ")\n";
#endif // #ifdef VERY_NOISY_PIP

      // Normalize the tableau before pivoting.
      tableau.normalize();

      // Perform pivot operation.

      // Update basis.
      {
        const dimension_type var_pi = var_row[pi];
        const dimension_type var_pj = var_column[pj];
        var_row[pi] = var_pj;
        var_column[pj] = var_pi;
        basis[var_pi] = true;
        basis[var_pj] = false;
        mapping[var_pi] = pj;
        mapping[var_pj] = pi;
      }

      PPL_DIRTY_TEMP_COEFFICIENT(product);
      PPL_DIRTY_TEMP_COEFFICIENT(gcd);
      PPL_DIRTY_TEMP_COEFFICIENT(scale_factor);

      // Creating identity rows corresponding to basic variable pj:
      // 1. add them to tableau so as to have proper size and capacity;
      tableau.s.add_zero_rows(1);
      tableau.t.add_zero_rows(1);
      // 2. swap the rows just added with empty ones.
      Row s_pivot(0);
      Row t_pivot(0);
      swap(s_pivot, tableau.s[num_rows]);
      swap(t_pivot, tableau.t[num_rows]);
      // 3. drop rows previously added at end of tableau.
      tableau.s.remove_trailing_rows(1);
      tableau.t.remove_trailing_rows(1);

      // Save current pivot denominator.
      PPL_DIRTY_TEMP_COEFFICIENT(pivot_denom);
      pivot_denom = tableau.denominator();
      // Let the (scaled) pivot coordinate be 1.
      s_pivot[pj] = pivot_denom;

      // Swap identity row with the pivot row previously found.
      s_pivot.m_swap(tableau.s[pi]);
      t_pivot.m_swap(tableau.t[pi]);
      sign[pi] = ZERO;

      PPL_DIRTY_TEMP_COEFFICIENT(s_pivot_pj);
      s_pivot_pj = s_pivot.get(pj);

      // Compute columns s[*][j]:
      //
      // <CODE>
      //   s[i][j] -= s[i][pj] * s_pivot[j] / s_pivot_pj;
      // </CODE>
      for (dimension_type i = num_rows; i-- > 0; ) {
        Row& s_i = tableau.s[i];
        PPL_DIRTY_TEMP_COEFFICIENT(s_i_pj);
        s_i_pj = s_i.get(pj);

        if (s_i_pj == 0)
          continue;

        WEIGHT_BEGIN();
        Row::iterator itr = s_i.end();
        for (Row::const_iterator
               j = s_pivot.begin(), j_end = s_pivot.end(); j != j_end; ++j) {
          if (j.index() != pj) {
            Coefficient_traits::const_reference s_pivot_j = *j;
            // Do nothing if the j-th pivot element is zero.
            if (s_pivot_j != 0) {
              product = s_pivot_j * s_i_pj;
              if (product % s_pivot_pj != 0) {
                // Must scale matrix to stay in integer case.
                gcd_assign(gcd, product, s_pivot_pj);
                exact_div_assign(scale_factor, s_pivot_pj, gcd);
                tableau.scale(scale_factor);
                s_i_pj *= scale_factor;
                product *= scale_factor;
                WEIGHT_ADD(102);
              }
              PPL_ASSERT(product % s_pivot_pj == 0);
              exact_div_assign(product, product, s_pivot_pj);
              WEIGHT_ADD(130);
              if (product != 0) {
                itr = s_i.insert(itr, j.index());
                *itr -= product;
                WEIGHT_ADD(34);
              }
            }
          }
        }
      }

      // Compute columns t[*][j]:
      //
      // <CODE>
      //   t[i][j] -= s[i][pj] * t_pivot[j] / s_pivot_pj;
      // </CODE>
      for (dimension_type i = num_rows; i-- > 0; ) {
        Row& s_i = tableau.s[i];
        Row& t_i = tableau.t[i];

        Row::iterator s_i_pj_itr = s_i.find(pj);

        if (s_i_pj_itr == s_i.end())
          continue;

        // FIXME: the following comment does not make sense.
        // NOTE: This is a Coefficient& instead of a
        // Coefficient_traits::const_reference, because scale() may silently
        // modify it.
        const Coefficient& s_i_pj = *s_i_pj_itr;

        if (s_i_pj == 0)
          continue;

        WEIGHT_BEGIN();
        Row::iterator k = t_i.end();
        for (Row::const_iterator
               j = t_pivot.begin(), j_end = t_pivot.end(); j != j_end; ++j) {
          Coefficient_traits::const_reference t_pivot_j = *j;
          // Do nothing if the j-th pivot element is zero.
          if (t_pivot_j != 0) {
            product = t_pivot_j * s_i_pj;
            if (product % s_pivot_pj != 0) {
              // Must scale matrix to stay in integer case.
              gcd_assign(gcd, product, s_pivot_pj);
              exact_div_assign(scale_factor, s_pivot_pj, gcd);
              tableau.scale(scale_factor);
              product *= scale_factor;
              WEIGHT_ADD(261);
            }
            PPL_ASSERT(product % s_pivot_pj == 0);
            exact_div_assign(product, product, s_pivot_pj);
            WEIGHT_ADD(115);
            if (product != 0) {
              k = t_i.insert(k, j.index());
              *k -= product;
              WEIGHT_ADD(41);
            }

            // Update row sign.
            Row_Sign& sign_i = sign[i];
            switch (sign_i) {
            case ZERO:
              if (product > 0)
                sign_i = NEGATIVE;
              else if (product < 0)
                sign_i = POSITIVE;
              break;
            case POSITIVE:
              if (product > 0)
                sign_i = MIXED;
              break;
            case NEGATIVE:
              if (product < 0)
                sign_i = MIXED;
              break;
            default:
              break;
            }
          }
        }
      }

      // Compute column s[*][pj]: s[i][pj] /= s_pivot_pj;
      // Update column only if pivot coordinate != 1.
      if (s_pivot_pj != pivot_denom) {
        WEIGHT_BEGIN();
        Row::iterator itr;
        for (dimension_type i = num_rows; i-- > 0; ) {
          Row& s_i = tableau.s[i];
          itr = s_i.find(pj);
          if (itr == s_i.end())
            continue;
          WEIGHT_ADD(43);
          product = *itr * pivot_denom;
          if (product % s_pivot_pj != 0) {
            // As above, perform matrix scaling.
            gcd_assign(gcd, product, s_pivot_pj);
            exact_div_assign(scale_factor, s_pivot_pj, gcd);
            tableau.scale(scale_factor);
            product *= scale_factor;
            WEIGHT_ADD(177);
          }
          PPL_ASSERT(product % s_pivot_pj == 0);
          if (product != 0 || *itr != 0) {
            WEIGHT_ADD(106);
            exact_div_assign(*itr, product, s_pivot_pj);
          }
        }
      }

      // Pivoting process ended: jump to next iteration.
      continue;
    } // if (first_negative != not_a_dim)


    PPL_ASSERT(first_negative == not_a_dim);
    // If no negative parameter row was found,
    // but a mixed parameter row was found ...
    if (first_mixed != not_a_dim) {
      // Look for a constraint (i_neg):
      //  - having mixed parameter sign;
      //  - having no positive variable coefficient;
      //  - minimizing the score (sum of parameter coefficients).
      dimension_type i_neg = not_a_dim;
      PPL_DIRTY_TEMP_COEFFICIENT(best_score);
      PPL_DIRTY_TEMP_COEFFICIENT(score);
      for (dimension_type i = first_mixed; i < num_rows; ++i) {
        // Mixed parameter sign.
        if (sign[i] != MIXED)
          continue;
        // No positive variable coefficient.
        bool has_positive = false;
        {
          const Row& s_i = tableau.s[i];
          for (Row::const_iterator
                 j = s_i.begin(), j_end = s_i.end(); j != j_end; ++j)
            if (*j > 0) {
              has_positive = true;
              break;
            }
        }
        if (has_positive)
          continue;
        // Minimize parameter coefficient score,
        // eliminating implicated tautologies (if any).
        score = 0;
        {
          WEIGHT_BEGIN();
          const Row& t_i = tableau.t[i];
          for (Row::const_iterator
                 j = t_i.begin(), j_end = t_i.end(); j != j_end; ++j) {
            WEIGHT_ADD(55);
            score += *j;
          }
        }
        if (i_neg == not_a_dim || score < best_score) {
          i_neg = i;
          best_score = score;
        }
      }

      if (i_neg != not_a_dim) {
        Row tautology = tableau.t[i_neg];
        /* Simplify tautology by exploiting integrality. */
        integral_simplification(tautology);
        ctx.add_row(tautology);
        add_constraint(tautology, all_params);
        sign[i_neg] = POSITIVE;
#ifdef NOISY_PIP
        {
          Linear_Expression expr = Linear_Expression(tautology.get(0));
          dimension_type j = 1;
          for (Variables_Set::const_iterator p = all_params.begin(),
                 p_end = all_params.end(); p != p_end; ++p, ++j)
            add_mul_assign(expr, tautology.get(j), Variable(*p));
          using namespace IO_Operators;
          std::cerr << std::setw(2 * indent_level) << ""
                    << "Row " << i_neg
                    << ": mixed param sign, negative var coeffs\n";
          std::cerr << std::setw(2 * indent_level) << ""
                    << "==> adding tautology: "
                    << Constraint(expr >= 0) << ".\n";
        }
#endif // #ifdef NOISY_PIP
        // Jump to next iteration.
        continue;
      }

      PPL_ASSERT(i_neg == not_a_dim);
      // Heuristically choose "best" (mixed) pivoting row.
      dimension_type best_i = not_a_dim;
      for (dimension_type i = first_mixed; i < num_rows; ++i) {
        if (sign[i] != MIXED)
          continue;
        score = 0;
        {
          WEIGHT_BEGIN();
          const Row& t_i = tableau.t[i];
          for (Row::const_iterator
                 j = t_i.begin(), j_end = t_i.end(); j != j_end; ++j) {
            WEIGHT_ADD(51);
            score += *j;
          }
        }
        if (best_i == not_a_dim || score < best_score) {
          best_score = score;
          best_i = i;
        }
      }

      Row t_test(tableau.t[best_i]);
      /* Simplify t_test by exploiting integrality. */
      integral_simplification(t_test);

#ifdef NOISY_PIP
      {
        Linear_Expression expr = Linear_Expression(t_test.get(0));
        dimension_type j = 1;
        for (Variables_Set::const_iterator p = all_params.begin(),
               p_end = all_params.end(); p != p_end; ++p, ++j)
          add_mul_assign(expr, t_test.get(j), Variable(*p));
        using namespace IO_Operators;
        std::cerr << std::setw(2 * indent_level) << ""
                  << "Row " << best_i << ": mixed param sign\n";
        std::cerr << std::setw(2 * indent_level) << ""
                  << "==> depends on sign of " << expr << ".\n";
      }
#endif // #ifdef NOISY_PIP

      // Create a solution node for the "true" version of current node.
      PIP_Tree_Node* t_node = new PIP_Solution_Node(*this, No_Constraints());
      // Protect it from exception safety issues via std::auto_ptr.
      std::auto_ptr<PIP_Tree_Node> wrapped_node(t_node);

      // Add parametric constraint to context.
      ctx.add_row(t_test);
      // Recursively solve true node with respect to updated context.
#ifdef NOISY_PIP_TREE_STRUCTURE
      indent_and_print(std::cerr, indent_level, "=== SOLVING THEN CHILD\n");
#endif
      t_node = t_node->solve(pip, check_feasible_context,
                             ctx, all_params, space_dim,
                             indent_level + 1);
      // Resolution may have changed t_node: in case, re-wrap it.
      if (t_node != wrapped_node.get()) {
        wrapped_node.release();
        wrapped_node.reset(t_node);
      }

      // Modify *this in place to become the "false" version of current node.
      PIP_Tree_Node* f_node = this;
      // Swap aside constraints and artificial parameters
      // (these will be later restored if needed).
      Constraint_System cs;
      Artificial_Parameter_Sequence aps;
      swap(cs, f_node->constraints_);
      swap(aps, f_node->artificial_parameters);
      // Compute the complement of the constraint used for the "true" node.
      Row& f_test = ctx[ctx.num_rows() - 1];
      complement_assign(f_test, t_test, 1);

      // Recursively solve false node with respect to updated context.
#ifdef NOISY_PIP_TREE_STRUCTURE
      indent_and_print(std::cerr, indent_level, "=== SOLVING ELSE CHILD\n");
#endif
      f_node = f_node->solve(pip, check_feasible_context,
                             ctx, all_params, space_dim,
                             indent_level + 1);

      // Case analysis on recursive resolution calls outcome.
      if (t_node == 0) {
        if (f_node == 0) {
          // Both t_node and f_node unfeasible.
#ifdef NOISY_PIP_TREE_STRUCTURE
          indent_and_print(std::cerr, indent_level,
                           "=== EXIT: BOTH BRANCHES UNFEASIBLE: _|_\n");
#endif
          return 0;
        }
        else {
          // t_node unfeasible, f_node feasible:
          // restore cs and aps into f_node (i.e., this).
          PPL_ASSERT(f_node == this);
          swap(f_node->constraints_, cs);
          swap(f_node->artificial_parameters, aps);
          // Add f_test to constraints.
          f_node->add_constraint(f_test, all_params);
#ifdef NOISY_PIP_TREE_STRUCTURE
          indent_and_print(std::cerr, indent_level,
                           "=== EXIT: THEN BRANCH UNFEASIBLE: SWAP BRANCHES\n");
#endif
          return f_node;
        }
      }
      else if (f_node == 0) {
        // t_node feasible, f_node unfeasible.
#ifdef NOISY_PIP_TREE_STRUCTURE
        indent_and_print(std::cerr, indent_level,
                         "=== EXIT: THEN BRANCH FEASIBLE\n");
#endif
        // NOTE: in principle, we could merge t_node into its parent.
        // However, if t_node is a decision node having both children,
        // then we would obtain a node violating the PIP_Decision_Node
        // invariant saying that t_node should have a single constraint:
        // it will have, at least, the two splitting constraints.
        const PIP_Decision_Node* const decision_node_p
          = dynamic_cast<PIP_Decision_Node*>(t_node);
        if (decision_node_p != 0 && decision_node_p->false_child != 0) {
          // Do NOT merge: create a new decision node.
          PIP_Tree_Node* const parent
            = new PIP_Decision_Node(t_node->get_owner(), 0, t_node);
          // Previously wrapped 't_node' is now safe: release it
          // and protect new 'parent' node from exception safety issues.
          wrapped_node.release();
          wrapped_node.reset(parent);
          // Restore into parent `cs' and `aps'.
          swap(parent->constraints_, cs);
          swap(parent->artificial_parameters, aps);
          // Add t_test to parent's constraints.
          parent->add_constraint(t_test, all_params);
          // It is now safe to release previously wrapped parent pointer
          // and return it to caller.
          return wrapped_node.release();
        }
        else {
          // Merge t_node with its parent:
          // a) append into `cs' the constraints of t_node;
          for (Constraint_System::const_iterator
                 i = t_node->constraints_.begin(),
                 i_end = t_node->constraints_.end(); i != i_end; ++i)
            cs.insert(*i);
          // b) append into `aps' the parameters of t_node;
          aps.insert(aps.end(),
                     t_node->artificial_parameters.begin(),
                     t_node->artificial_parameters.end());
          // c) swap the updated `cs' and `aps' into t_node.
          swap(cs, t_node->constraints_);
          swap(aps, t_node->artificial_parameters);
          // d) add t_test to t_nodes's constraints.
          t_node->add_constraint(t_test, all_params);
          // It is now safe to release previously wrapped t_node pointer
          // and return it to caller.
          return wrapped_node.release();
        }
      }

      // Here both t_node and f_node are feasible:
      // create a new decision node.
#ifdef NOISY_PIP_TREE_STRUCTURE
      indent_and_print(std::cerr, indent_level,
                       "=== EXIT: BOTH BRANCHES FEASIBLE: NEW DECISION NODE\n");
#endif
      PIP_Tree_Node* parent
        = new PIP_Decision_Node(f_node->get_owner(), f_node, t_node);
      // Previously wrapped 't_node' is now safe: release it
      // and protect new 'parent' node from exception safety issues.
      wrapped_node.release();
      wrapped_node.reset(parent);

      // Add t_test to the constraints of the new decision node.
      parent->add_constraint(t_test, all_params);

      if (!cs.empty()) {
#ifdef NOISY_PIP_TREE_STRUCTURE
        indent_and_print(std::cerr, indent_level,
                         "=== NODE HAS BOTH BRANCHES AND TAUTOLOGIES:\n");
        indent_and_print(std::cerr, indent_level,
                         "=== CREATE NEW PARENT FOR TAUTOLOGIES\n");
#endif
        // If node to be solved had tautologies,
        // store them in a new decision node.
        parent = new PIP_Decision_Node(parent->get_owner(), 0, parent);
        // Previously wrapped 'parent' node is now safe: release it
        // and protect new 'parent' node from exception safety issues.
        wrapped_node.release();
        wrapped_node.reset(parent);
        swap(parent->constraints_, cs);
      }
      swap(parent->artificial_parameters, aps);
      // It is now safe to release previously wrapped decision node
      // and return it to the caller.
      return wrapped_node.release();
    } // if (first_mixed != not_a_dim)


    PPL_ASSERT(first_negative == not_a_dim);
    PPL_ASSERT(first_mixed == not_a_dim);
    // Here all parameters are positive: we have found a continuous
    // solution. If the solution happens to be integer, then it is the
    // solution of the  integer problem. Otherwise, we may need to generate
    // a new cut to try and get back into the integer case.
#ifdef NOISY_PIP
    indent_and_print(std::cerr, indent_level,
                     "All parameters are positive.\n");
#endif // #ifdef NOISY_PIP
    tableau.normalize();

    // Look for any row having non integer parameter coefficients.
    Coefficient_traits::const_reference denom = tableau.denominator();
    for (dimension_type k = 0; k < num_vars; ++k) {
      if (basis[k])
        // Basic variable = 0, hence integer.
        continue;
      const dimension_type i = mapping[k];
      const Row& t_i = tableau.t[i];
      WEIGHT_BEGIN();
      for (Row::const_iterator
             j = t_i.begin(), j_end = t_i.end(); j != j_end; ++j) {
        WEIGHT_ADD(27);
        if (*j % denom != 0)
          goto non_integer;
      }
    }
    // The goto was not taken, the solution is integer.
#ifdef NOISY_PIP_TREE_STRUCTURE
    indent_and_print(std::cerr, indent_level,
                     "EXIT: solution found.\n");
#endif // #ifdef NOISY_PIP
    return this;

  non_integer:
    // The solution is non-integer: generate a cut.
    PPL_DIRTY_TEMP_COEFFICIENT(mod);
    dimension_type best_i = not_a_dim;
    dimension_type best_pcount = not_a_dim;

    const PIP_Problem::Control_Parameter_Value cutting_strategy
      = pip.control_parameters[PIP_Problem::CUTTING_STRATEGY];

    if (cutting_strategy == PIP_Problem::CUTTING_STRATEGY_FIRST) {
      // Find the first row with simplest parametric part.
      for (dimension_type k = 0; k < num_vars; ++k) {
        if (basis[k])
          continue;
        const dimension_type i = mapping[k];
        // Count the number of non-integer parameter coefficients.
        WEIGHT_BEGIN();
        dimension_type pcount = 0;
        const Row& t_i = tableau.t[i];
        for (Row::const_iterator
               j = t_i.begin(), j_end = t_i.end(); j != j_end; ++j) {
          WEIGHT_ADD(18);
          pos_rem_assign(mod, *j, denom);
          if (mod != 0)
            ++pcount;
        }
        if (pcount > 0 && (best_i == not_a_dim || pcount < best_pcount)) {
          best_pcount = pcount;
          best_i = i;
        }
      }
      // Generate cut using 'best_i'.
      generate_cut(best_i, all_params, ctx, space_dim, indent_level);
    }
    else {
      PPL_ASSERT(cutting_strategy == PIP_Problem::CUTTING_STRATEGY_DEEPEST
                 || cutting_strategy == PIP_Problem::CUTTING_STRATEGY_ALL);
      // Find the row with simplest parametric part
      // which will generate the "deepest" cut.
      PPL_DIRTY_TEMP_COEFFICIENT(best_score);
      best_score = 0;
      PPL_DIRTY_TEMP_COEFFICIENT(score);
      PPL_DIRTY_TEMP_COEFFICIENT(s_score);
      std::vector<dimension_type> all_best_is;

      for (dimension_type k = 0; k < num_vars; ++k) {
        if (basis[k])
          continue;
        const dimension_type i = mapping[k];
        // Compute score and pcount.
        score = 0;
        dimension_type pcount = 0;
        {
          WEIGHT_BEGIN();
          const Row& t_i = tableau.t[i];
          for (Row::const_iterator
                 j = t_i.begin(), j_end = t_i.end(); j != j_end; ++j) {
            WEIGHT_ADD(46);
            pos_rem_assign(mod, *j, denom);
            if (mod != 0) {
              WEIGHT_ADD(94);
              score += denom;
              score -= mod;
              ++pcount;
            }
          }
        }

        // Compute s_score.
        s_score = 0;
        {
          WEIGHT_BEGIN();
          const Row& s_i = tableau.s[i];
          for (Row::const_iterator
                 j = s_i.begin(), j_end = s_i.end(); j != j_end; ++j) {
            WEIGHT_ADD(94);
            pos_rem_assign(mod, *j, denom);
            s_score += denom;
            s_score -= mod;
          }
        }
        // Combine 'score' and 's_score'.
        score *= s_score;
        /*
          Select row i if it is non integer AND
            - no row has been chosen yet; OR
            - it has fewer non-integer parameter coefficients; OR
            - it has the same number of non-integer parameter coefficients,
              but its score is greater.
        */
        if (pcount != 0
            && (best_i == not_a_dim
                || pcount < best_pcount
                || (pcount == best_pcount && score > best_score))) {
          if (pcount < best_pcount)
            all_best_is.clear();
          best_i = i;
          best_pcount = pcount;
          best_score = score;
        }
        if (pcount > 0)
          all_best_is.push_back(i);
      }
      if (cutting_strategy == PIP_Problem::CUTTING_STRATEGY_DEEPEST)
        generate_cut(best_i, all_params, ctx, space_dim, indent_level);
      else {
        PPL_ASSERT(cutting_strategy == PIP_Problem::CUTTING_STRATEGY_ALL);
        for (dimension_type k = all_best_is.size(); k-- > 0; )
          generate_cut(all_best_is[k], all_params, ctx,
                       space_dim, indent_level);
      }
    } // End of processing for non-integer solutions.

  } // Main loop of the simplex algorithm

  // This point should be unreachable.
  PPL_UNREACHABLE;
  return 0;
}

void
PIP_Solution_Node::generate_cut(const dimension_type index,
                                Variables_Set& parameters,
                                Matrix<Row>& context,
                                dimension_type& space_dimension,
                                const int indent_level) {
  PPL_ASSERT(indent_level >= 0);
#ifdef NOISY_PIP
  std::cerr << std::setw(2 * indent_level) << ""
            << "Row " << index << " requires cut generation.\n";
#else
  PPL_USED(indent_level);
#endif // #ifdef NOISY_PIP

  const dimension_type num_rows = tableau.t.num_rows();
  PPL_ASSERT(index < num_rows);
  const dimension_type num_vars = tableau.s.num_columns();
  const dimension_type num_params = tableau.t.num_columns();
  PPL_ASSERT(num_params == 1 + parameters.size());
  Coefficient_traits::const_reference denom = tableau.denominator();

  PPL_DIRTY_TEMP_COEFFICIENT(mod);
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);

  // Test if cut to be generated must be parametric or not.
  bool generate_parametric_cut = false;
  {
    // Limiting the scope of reference row_t (may be later invalidated).
    const Row& row_t = tableau.t[index];
    Row::const_iterator j = row_t.begin();
    Row::const_iterator j_end = row_t.end();
    // Skip the element with index 0.
    if (j != j_end && j.index() == 0)
      ++j;
    WEIGHT_BEGIN();
    for ( ; j != j_end; ++j) {
      WEIGHT_ADD(7);
      if (*j % denom != 0) {
        generate_parametric_cut = true;
        break;
      }
    }
  }

  // Column index of already existing Artificial_Parameter.
  dimension_type ap_column = not_a_dimension();

  if (generate_parametric_cut) {
    // Fractional parameter coefficient found: generate parametric cut.
    bool reuse_ap = false;
    Linear_Expression expr;

    // Limiting the scope of reference row_t (may be later invalidated).
    {
      const Row& row_t = tableau.t[index];
      Row::const_iterator j = row_t.begin();
      Row::const_iterator j_end = row_t.end();
      if (j != j_end && j.index() == 0) {
        pos_rem_assign(mod, *j, denom);
        ++j;
        if (mod != 0) {
          // Optimizing computation: expr += (denom - mod);
          expr += denom;
          expr -= mod;
        }
      }
      if (!parameters.empty()) {
        // To avoid reallocations of expr.
        add_mul_assign(expr, 0, Variable(*(parameters.rbegin())));
        Variables_Set::const_iterator p_j = parameters.begin();
        dimension_type last_index = 1;
        WEIGHT_BEGIN();
        for ( ; j != j_end; ++j) {
          WEIGHT_ADD(69);
          pos_rem_assign(mod, *j, denom);
          if (mod != 0) {
            // Optimizing computation: expr += (denom - mod) * Variable(*p_j);
            WEIGHT_ADD(164);
            coeff = denom - mod;
            PPL_ASSERT(last_index <= j.index());
            std::advance(p_j, j.index() - last_index);
            last_index = j.index();
            add_mul_assign(expr, coeff, Variable(*p_j));
          }
        }
      }
    }
    // Generate new artificial parameter.
    const Artificial_Parameter ap(expr, denom);

    // Search if the Artificial_Parameter has already been generated.
    ap_column = space_dimension;
    const PIP_Tree_Node* node = this;
    do {
      for (dimension_type j = node->artificial_parameters.size(); j-- > 0; ) {
        --ap_column;
        if (node->artificial_parameters[j] == ap) {
          reuse_ap = true;
          break;
        }
      }
      node = node->parent();
    } while (!reuse_ap && node != 0);

    if (reuse_ap) {
      // We can re-use an existing Artificial_Parameter.
#ifdef NOISY_PIP
      using namespace IO_Operators;
      std::cerr << std::setw(2 * indent_level) << ""
                << "Re-using parameter " << Variable(ap_column)
                << " = " << ap << std::endl;
#endif // #ifdef NOISY_PIP
      ap_column = ap_column - num_vars + 1;
    }
    else {
      // Here reuse_ap == false: the Artificial_Parameter does not exist yet.
      // Beware: possible reallocation invalidates row references.
      tableau.t.add_zero_columns(1);
      context.add_zero_columns(1);
      artificial_parameters.push_back(ap);
      parameters.insert(space_dimension);
#ifdef NOISY_PIP
      using namespace IO_Operators;
      std::cerr << std::setw(2 * indent_level) << ""
                << "New parameter " << Variable(space_dimension)
                << " = " << ap << std::endl;
#endif // #ifdef NOISY_PIP
      ++space_dimension;
      ap_column = num_params;

      // Update current context with constraints on the new parameter.
      const dimension_type ctx_num_rows = context.num_rows();
      context.add_zero_rows(2);
      Row& ctx1 = context[ctx_num_rows];
      Row& ctx2 = context[ctx_num_rows+1];
      // Recompute row reference after possible reallocation.
      const Row& row_t = tableau.t[index];
      {
        Row::const_iterator j = row_t.begin();
        Row::const_iterator j_end = row_t.end();
        Row::iterator itr1 = ctx1.end();
        Row::iterator itr2 = ctx2.end();
        if (j != j_end && j.index() == 0) {
          pos_rem_assign(mod, *j, denom);
          if (mod != 0) {
            itr1 = ctx1.insert(0, denom);
            *itr1 -= mod;
            itr2 = ctx2.insert(0, *itr1);
            neg_assign(*itr2);
            // Compute <CODE> ctx2[0] += denom-1; </CODE>
            *itr2 += denom;
            --(*itr2);
          }
          else {
            // Compute <CODE> ctx2[0] += denom-1; </CODE>
            itr2 = ctx2.insert(0, denom);
            --(*itr2);
          }
          ++j;
        }
        else {
          // Compute <CODE> ctx2[0] += denom-1; </CODE>
          itr2 = ctx2.insert(0, denom);
          --(*itr2);
        }
        WEIGHT_BEGIN();
        for ( ; j != j_end; ++j) {
          pos_rem_assign(mod, *j, denom);
          if (mod != 0) {
            const dimension_type j_index = j.index();
            itr1 = ctx1.insert(itr1, j_index, denom);
            *itr1 -= mod;
            itr2 = ctx2.insert(itr2, j_index, *itr1);
            neg_assign(*itr2);
            WEIGHT_ADD(294);
          }
        }
        itr1 = ctx1.insert(itr1, num_params, denom);
        neg_assign(*itr1);
        itr2 = ctx2.insert(itr2, num_params, denom);
        WEIGHT_ADD(122);
      }

#ifdef NOISY_PIP
      {
        using namespace IO_Operators;
        Variables_Set::const_iterator p = parameters.begin();
        Linear_Expression expr1(ctx1.get(0));
        Linear_Expression expr2(ctx2.get(0));
        for (dimension_type j = 1; j <= num_params; ++j, ++p) {
          add_mul_assign(expr1, ctx1.get(j), Variable(*p));
          add_mul_assign(expr2, ctx2.get(j), Variable(*p));
        }
        std::cerr << std::setw(2 * indent_level) << ""
                  << "Adding to context: "
                  << Constraint(expr1 >= 0) << " ; "
                  << Constraint(expr2 >= 0) << std::endl;
      }
#endif // #ifdef NOISY_PIP
    }
  }

  // Generate new cut.
  tableau.s.add_zero_rows(1);
  tableau.t.add_zero_rows(1);
  Row& cut_s = tableau.s[num_rows];
  Row& cut_t = tableau.t[num_rows];
  // Recompute references after possible reallocation.
  const Row& row_s = tableau.s[index];
  const Row& row_t = tableau.t[index];
  WEIGHT_BEGIN();
  {
    Row::iterator itr = cut_s.end();
    for (Row::const_iterator
           j = row_s.begin(), j_end = row_s.end(); j != j_end; ++j) {
      WEIGHT_ADD(55);
      itr = cut_s.insert(itr, j.index(), *j);
      pos_rem_assign(*itr, *itr, denom);
    }
  }
  {
    Row::iterator cut_t_itr = cut_t.end();
    for (Row::const_iterator
           j = row_t.begin(), j_end = row_t.end(); j!=j_end; ++j) {
      WEIGHT_ADD(37);
      pos_rem_assign(mod, *j, denom);
      if (mod != 0) {
        WEIGHT_ADD(108);
        cut_t_itr = cut_t.insert(cut_t_itr, j.index(), mod);
        *cut_t_itr -= denom;
      }
    }
  }
  if (ap_column != not_a_dimension())
    // If we re-use an existing Artificial_Parameter
    cut_t[ap_column] = denom;

#ifdef NOISY_PIP
  {
    using namespace IO_Operators;
    Linear_Expression expr;
    dimension_type ti = 1;
    dimension_type si = 0;
    for (dimension_type j = 0; j < space_dimension; ++j) {
      if (parameters.count(j) == 1) {
        add_mul_assign(expr, cut_t.get(ti), Variable(j));
        ++ti;
      }
      else {
        add_mul_assign(expr, cut_s.get(si), Variable(j));
        ++si;
      }
    }
    std::cerr << std::setw(2 * indent_level) << ""
              << "Adding cut: "
              << Constraint(expr + cut_t.get(0) >= 0)
              << std::endl;
  }
#endif // #ifdef NOISY_PIP
  var_row.push_back(num_rows + num_vars);
  basis.push_back(false);
  mapping.push_back(num_rows);
  sign.push_back(NEGATIVE);
}


memory_size_type
PIP_Tree_Node::Artificial_Parameter::external_memory_in_bytes() const {
  return Linear_Expression::external_memory_in_bytes()
    + Parma_Polyhedra_Library::external_memory_in_bytes(denom);
}

memory_size_type
PIP_Tree_Node::Artificial_Parameter::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

memory_size_type
PIP_Tree_Node::external_memory_in_bytes() const {
  memory_size_type n = constraints_.external_memory_in_bytes();
  // Adding the external memory for `artificial_parameters'.
  n += artificial_parameters.capacity() * sizeof(Artificial_Parameter);
  for (Artificial_Parameter_Sequence::const_iterator
         ap = art_parameter_begin(),
         ap_end = art_parameter_end(); ap != ap_end; ++ap)
    n += (ap->external_memory_in_bytes());

  return n;
}

memory_size_type
PIP_Decision_Node::external_memory_in_bytes() const {
  memory_size_type n = PIP_Tree_Node::external_memory_in_bytes();
  PPL_ASSERT(true_child != 0);
  n += true_child->total_memory_in_bytes();
  if (false_child != 0)
    n += false_child->total_memory_in_bytes();
  return n;
}

memory_size_type
PIP_Decision_Node::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

memory_size_type
PIP_Solution_Node::Tableau::external_memory_in_bytes() const {
  return Parma_Polyhedra_Library::external_memory_in_bytes(denom)
    + s.external_memory_in_bytes()
    + t.external_memory_in_bytes();
}

memory_size_type
PIP_Solution_Node::external_memory_in_bytes() const {
  memory_size_type n = PIP_Tree_Node::external_memory_in_bytes();
  n += tableau.external_memory_in_bytes();
  // FIXME: size of std::vector<bool> ?
  n += basis.capacity() * sizeof(bool);
  n += sizeof(dimension_type)
    * (mapping.capacity() + var_row.capacity() + var_column.capacity());
  n += sign.capacity() * sizeof(Row_Sign);
  // FIXME: Adding the external memory for `solution'.
  n += solution.capacity() * sizeof(Linear_Expression);
  for (std::vector<Linear_Expression>::const_iterator
         i = solution.begin(), i_end = solution.end(); i != i_end; ++i)
    n += (i->external_memory_in_bytes());

  return n;
}

memory_size_type
PIP_Solution_Node::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

void
PIP_Tree_Node::indent_and_print(std::ostream& s,
                                const int indent,
                                const char* str) {
  PPL_ASSERT(indent >= 0);
  s << std::setw(2 * indent) << "" << str;
}

void
PIP_Tree_Node::print(std::ostream& s, const int indent) const {
  const dimension_type pip_space_dim = get_owner()->space_dimension();
  const Variables_Set& pip_params = get_owner()->parameter_space_dimensions();

  std::vector<bool> pip_dim_is_param(pip_space_dim);
  for (Variables_Set::const_iterator p = pip_params.begin(),
         p_end = pip_params.end(); p != p_end; ++p)
    pip_dim_is_param[*p] = true;

  dimension_type first_art_dim = pip_space_dim;
  for (const PIP_Tree_Node* node = parent(); node != 0; node = node->parent())
    first_art_dim += node->art_parameter_count();

  print_tree(s, indent, pip_dim_is_param, first_art_dim);
}

void
PIP_Tree_Node::print_tree(std::ostream& s, const int indent,
                          const std::vector<bool>&,
                          dimension_type first_art_dim) const {
  using namespace IO_Operators;

  // Print artificial parameters.
  for (Artificial_Parameter_Sequence::const_iterator
         api = art_parameter_begin(),
         api_end = art_parameter_end(); api != api_end; ++api) {
    indent_and_print(s, indent, "Parameter ");
    s << Variable(first_art_dim) << " = " << *api << "\n";
    ++first_art_dim;
  }

  // Print constraints, if any.
  if (!constraints_.empty()) {
    indent_and_print(s, indent, "if ");

    Constraint_System::const_iterator ci = constraints_.begin();
    Constraint_System::const_iterator ci_end = constraints_.end();
    PPL_ASSERT(ci != ci_end);
    s << *ci;
    for (++ci; ci != ci_end; ++ci)
      s << " and " << *ci;

    s << " then\n";
  }
}

void
PIP_Decision_Node::print_tree(std::ostream& s, const int indent,
                              const std::vector<bool>& pip_dim_is_param,
                              const dimension_type first_art_dim) const {
  // First print info common to decision and solution nodes.
  PIP_Tree_Node::print_tree(s, indent, pip_dim_is_param, first_art_dim);

  // Then print info specific of decision nodes.
  const dimension_type child_first_art_dim
    = first_art_dim + art_parameter_count();

  PPL_ASSERT(true_child != 0);
  true_child->print_tree(s, indent+1, pip_dim_is_param, child_first_art_dim);

  indent_and_print(s, indent, "else\n");

  if (false_child != 0)
    false_child->print_tree(s, indent+1, pip_dim_is_param,
                            child_first_art_dim);
  else
    indent_and_print(s, indent+1, "_|_\n");
}

void
PIP_Solution_Node::print_tree(std::ostream& s, const int indent,
                              const std::vector<bool>& pip_dim_is_param,
                              const dimension_type first_art_dim) const {
  // Print info common to decision and solution nodes.
  PIP_Tree_Node::print_tree(s, indent, pip_dim_is_param, first_art_dim);

  // Print info specific of solution nodes:
  // first update solution if needed ...
  update_solution(pip_dim_is_param);
  // ... and then actually print it.
  const bool no_constraints = constraints_.empty();
  indent_and_print(s, indent + (no_constraints ? 0 : 1), "{");
  const dimension_type pip_space_dim = pip_dim_is_param.size();
  for (dimension_type i = 0, num_var = 0; i < pip_space_dim; ++i) {
    if (pip_dim_is_param[i])
      continue;
    if (num_var > 0)
      s << " ; ";
    using namespace IO_Operators;
    s << solution[num_var];
    ++num_var;
  }
  s << "}\n";

  if (!no_constraints) {
    indent_and_print(s, indent, "else\n");
    indent_and_print(s, indent+1, "_|_\n");
  }
}

const Linear_Expression&
PIP_Solution_Node::parametric_values(const Variable var) const {
  const PIP_Problem* const pip = get_owner();
  PPL_ASSERT(pip != 0);

  const dimension_type space_dim = pip->space_dimension();
  if (var.space_dimension() > space_dim) {
    std::ostringstream s;
    s << "PPL::PIP_Solution_Node::parametric_values(v):\n"
      << "v.space_dimension() == " << var.space_dimension()
      << " is incompatible with the owning PIP_Problem "
      << " (space dim == " << space_dim << ").";
    throw std::invalid_argument(s.str());
  }

  dimension_type solution_index = var.id();
  const Variables_Set& params = pip->parameter_space_dimensions();
  for (Variables_Set::const_iterator p = params.begin(),
         p_end = params.end(); p != p_end; ++p) {
    const dimension_type param_index = *p;
    if (param_index < var.id())
      --solution_index;
    else if (param_index == var.id())
      throw std::invalid_argument("PPL::PIP_Solution_Node"
                                  "::parametric_values(v):\n"
                                  "v is a problem parameter.");
    else
      break;
  }

  update_solution();
  return solution[solution_index];
}


void
PIP_Solution_Node::update_solution() const {
  // Avoid doing useless work.
  if (solution_valid)
    return;

  const PIP_Problem* const pip = get_owner();
  PPL_ASSERT(pip != 0);
  std::vector<bool> pip_dim_is_param(pip->space_dimension());
  const Variables_Set& params = pip->parameter_space_dimensions();
  for (Variables_Set::const_iterator p = params.begin(),
         p_end = params.end(); p != p_end; ++p)
    pip_dim_is_param[*p] = true;

  update_solution(pip_dim_is_param);
}

void
PIP_Solution_Node
::update_solution(const std::vector<bool>& pip_dim_is_param) const {
  // Avoid doing useless work.
  if (solution_valid)
    return;

  // const_cast required so as to refresh the solution cache.
  PIP_Solution_Node& x = const_cast<PIP_Solution_Node&>(*this);

  const dimension_type num_pip_dims = pip_dim_is_param.size();
  const dimension_type num_pip_vars = tableau.s.num_columns();
  const dimension_type num_pip_params = num_pip_dims - num_pip_vars;
  const dimension_type num_all_params = tableau.t.num_columns() - 1;
  const dimension_type num_art_params = num_all_params - num_pip_params;

  if (solution.size() != num_pip_vars)
    x.solution.resize(num_pip_vars);

  // Compute external "names" (i.e., indices) for all parameters.
  std::vector<dimension_type> all_param_names(num_all_params);

  // External indices for problem parameters.
  for (dimension_type i = 0, p_index = 0; i < num_pip_dims; ++i)
    if (pip_dim_is_param[i]) {
      all_param_names[p_index] = i;
      ++p_index;
    }
  // External indices for artificial parameters.
  for (dimension_type i = 0; i < num_art_params; ++i)
    all_param_names[num_pip_params + i] = num_pip_dims + i;


  PPL_DIRTY_TEMP_COEFFICIENT(norm_coeff);
  Coefficient_traits::const_reference denom = tableau.denominator();
  for (dimension_type i = num_pip_vars; i-- > 0; ) {
    Linear_Expression& sol_i = x.solution[i];
    sol_i = Linear_Expression(0);
    if (basis[i])
      continue;
    const Row& row = tableau.t[mapping[i]];

    // Start from index 1 to skip the inhomogeneous term.
    Row::const_iterator j = row.begin();
    Row::const_iterator j_end = row.end();
    // Skip the element with index 0.
    if (j != j_end && j.index() == 0)
      ++j;
    for ( ; j != j_end; ++j) {
      Coefficient_traits::const_reference coeff = *j;
      if (coeff == 0)
        continue;
      norm_coeff = coeff / denom;
      if (norm_coeff != 0)
        add_mul_assign(sol_i, norm_coeff,
                      Variable(all_param_names[j.index() - 1]));
    }
    norm_coeff = row.get(0) / denom;
    sol_i += norm_coeff;
  }

  // Mark solution as valid.
  x.solution_valid = true;
}

} // namespace Parma_Polyhedra_Library
