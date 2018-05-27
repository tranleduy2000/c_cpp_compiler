/* Grid class implementation: conversion().
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
#include "Grid_defs.hh"
#include <cstddef>

namespace Parma_Polyhedra_Library {

// X 0 0 0  upside down, so  x x x X
// x X 0 0                   x x X 0
// x x X 0                   x X 0 0
// x x x X                   X 0 0 0
//
// Where X is greater than zero and x is an integer.
bool
Grid::lower_triangular(const Congruence_System& sys,
                       const Dimension_Kinds& dim_kinds) {
  const dimension_type num_columns = sys.space_dimension() + 1;

  // Check for easy square failure case.
  if (sys.num_rows() > num_columns)
    return false;

  // Check triangularity.

  dimension_type row = 0;
  for (dimension_type dim = num_columns; dim-- > 0; ) {
    if (dim_kinds[dim] == CON_VIRTUAL)
      continue;
    const Congruence& cg = sys[row];
    ++row;
    // Check diagonal.
    if (cg.expr.get(dim) <= 0)
      return false;
    if (!cg.expr.all_zeroes(dim + 1, num_columns))
      return false;
  }

  // Check squareness.
  return row == sys.num_rows();
}

// X x x x
// 0 X x x
// 0 0 X x
// 0 0 0 X
//
// Where X is greater than zero and x is an integer.
bool
Grid::upper_triangular(const Grid_Generator_System& sys,
                       const Dimension_Kinds& dim_kinds) {
  dimension_type num_columns = sys.space_dimension() + 1;
  dimension_type row = sys.num_rows();

  // Check for easy square fail case.
  if (row > num_columns)
    return false;

  // Check triangularity.
  while (num_columns > 0) {
    --num_columns;
    if (dim_kinds[num_columns] == GEN_VIRTUAL)
      continue;
    const Grid_Generator& gen = sys[--row];
    // Check diagonal.
    if (gen.expr.get(num_columns) <= 0)
      return false;
    // Check elements preceding diagonal.
    if (!gen.expr.all_zeroes(0, num_columns))
      return false;
  }

  // Check for squareness.
  return num_columns == row;
}

void
Grid::multiply_grid(const Coefficient& multiplier, Grid_Generator& gen,
                    Swapping_Vector<Grid_Generator>& dest_rows,
                    const dimension_type num_rows) {
  if (multiplier == 1)
    return;

  if (gen.is_line()) {
    // Multiply every element of the line.
    gen.expr *= multiplier;
  }
  else {
    PPL_ASSERT(gen.is_parameter_or_point());
    // Multiply every element of every parameter.

    for (dimension_type index = num_rows; index-- > 0; ) {
      Grid_Generator& generator = dest_rows[index];
      if (generator.is_parameter_or_point())
        generator.expr *= multiplier;
    }
  }
}

void
Grid::multiply_grid(const Coefficient& multiplier, Congruence& cg,
                    Swapping_Vector<Congruence>& dest,
                    const dimension_type num_rows) {
  if (multiplier == 1)
    return;

  if (cg.is_proper_congruence()) {
    // Multiply every element of every congruence.
    for (dimension_type index = num_rows; index-- > 0; ) {
      Congruence& congruence = dest[index];
      if (congruence.is_proper_congruence())
        congruence.scale(multiplier);
    }
  }
  else {
    PPL_ASSERT(cg.is_equality());
    // Multiply every element of the equality.
    cg.scale(multiplier);
  }
}

// TODO: Rename the next two methods to convert and this file to
//       Grid_convert.cc (and equivalently for Polyhedron) to use
//       verbs consistently as function and method names.

void
Grid::conversion(Grid_Generator_System& source, Congruence_System& dest,
                 Dimension_Kinds& dim_kinds) {
  // Quite similar to the congruence to generator version below.
  // Changes here may be needed there too.

  PPL_ASSERT(upper_triangular(source, dim_kinds));

  // Initialize matrix row number counters and compute the LCM of the
  // diagonal entries of the parameters in `source'.
  //
  // The top-down order of the generator system rows corresponds to
  // the left-right order of the dimensions, while the congruence
  // system rows have a bottom-up ordering.
  dimension_type dest_num_rows = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(diagonal_lcm);
  diagonal_lcm = 1;
  const dimension_type dims = source.space_dimension() + 1;
  dimension_type source_index = source.num_rows();
  for (dimension_type dim = dims; dim-- > 0; )
    if (dim_kinds[dim] == GEN_VIRTUAL)
      // Virtual generators map to equalities.
      ++dest_num_rows;
    else {
      --source_index;
      if (dim_kinds[dim] == PARAMETER) {
        // Dimension `dim' has a parameter row at `source_index' in
        // `source', so include in `diagonal_lcm' the `dim'th element
        // of that row.
        lcm_assign(diagonal_lcm, diagonal_lcm, source[source_index].expr.get(dim));
        // Parameters map to proper congruences.
        ++dest_num_rows;
      }
      // Lines map to virtual congruences.
    }
  PPL_ASSERT(source_index == 0);

  // `source' must be regular.
  PPL_ASSERT(diagonal_lcm != 0);

  dest.clear();
  dest.set_space_dimension(dims - 1);

  // In `dest' initialize row types and elements, including setting
  // the diagonal elements to the inverse ratio of the `source'
  // diagonal elements.
  source_index = source.num_rows();
  for (dimension_type dim = dims; dim-- > 0; ) {
    if (dim_kinds[dim] == LINE)
      --source_index;
    else {
      Linear_Expression le;
      le.set_space_dimension(dest.space_dimension());

      if (dim_kinds[dim] == GEN_VIRTUAL) {
        le.set(dim, Coefficient_one());
        Congruence cg(le, Coefficient_zero(), Recycle_Input());
        dest.insert_verbatim(cg, Recycle_Input());
      }
      else {
        PPL_ASSERT(dim_kinds[dim] == PARAMETER);
        --source_index;
        PPL_DIRTY_TEMP_COEFFICIENT(tmp);
        exact_div_assign(tmp, diagonal_lcm,
                         source[source_index].expr.get(dim));
        le.set(dim, tmp);
        Congruence cg(le, Coefficient_one(), Recycle_Input());
        dest.insert_verbatim(cg, Recycle_Input());
      }
    }
  }

  PPL_ASSERT(source_index == 0);
  PPL_ASSERT(lower_triangular(dest, dim_kinds));

  // Convert.
  //
  // `source_index' and `dest_index' hold the positions of pivot rows
  // in `source' and `dest'.  The order of the rows in `dest' is the
  // reverse of the order in `source', so the rows are iterated from
  // last to first (index 0) in `source' and from first to last in
  // `dest'.
  source_index = source.num_rows();
  dimension_type dest_index = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(multiplier);

  for (dimension_type dim = dims; dim-- > 0; ) {
    if (dim_kinds[dim] != GEN_VIRTUAL) {
      --source_index;
      const Coefficient& source_dim = source[source_index].expr.get(dim);

      // In the rows in `dest' above `dest_index' divide each element
      // at column `dim' by `source_dim'.
      for (dimension_type row = dest_index; row-- > 0; ) {
        Congruence& cg = dest.rows[row];

        // Multiply the representation of `dest' such that entry `dim'
        // of `g' is a multiple of `source_dim'.  This ensures that
        // the result of the division that follows is a whole number.
        gcd_assign(multiplier, cg.expression().get(dim), source_dim);
        exact_div_assign(multiplier, source_dim, multiplier);
        multiply_grid(multiplier, cg, dest.rows, dest_num_rows);

        cg.expr.exact_div_assign(source_dim, dim, dim + 1);
      }
    }

    // Invert and transpose the source row at `source_index' into the
    // destination row at `dest_index'.
    //
    // Consider each dimension `dim_prec' that precedes `dim', as the
    // rows in `dest' that follow `dim_index' have zeroes at index
    // `dim'.
    dimension_type tmp_source_index = source_index;
    if (dim_kinds[dim] != LINE)
      ++dest_index;
    for (dimension_type dim_prec = dim; dim_prec-- > 0; ) {
      if (dim_kinds[dim_prec] != GEN_VIRTUAL) {
        --tmp_source_index;
        const Coefficient& source_dim = source[tmp_source_index].expr.get(dim);
        
        // In order to compute the transpose of the inverse of
        // `source', subtract source[tmp_source_index][dim] times the
        // column vector in `dest' at `dim' from the column vector in
        // `dest' at `dim_prec'.
        //
        // I.e., for each row `dest_index' in `dest' that is above the
        // row `dest_index', subtract dest[tmp_source_index][dim]
        // times the entry `dim' from the entry at `dim_prec'.
        PPL_DIRTY_TEMP_COEFFICIENT(tmp);
        for (dimension_type row = dest_index; row-- > 0; ) {
          PPL_ASSERT(row < dest_num_rows);
          Congruence& cg = dest.rows[row];
          tmp = cg.expr.get(dim_prec);
          sub_mul_assign(tmp, source_dim, cg.expression().get(dim));
          cg.expr.set(dim_prec, tmp);
        }
      }
    }
  }
  // Set the modulus in every congruence.
  Coefficient_traits::const_reference modulus
    = dest.rows[dest_num_rows - 1].inhomogeneous_term();
  for (dimension_type row = dest_num_rows; row-- > 0; ) {
    Congruence& cg = dest.rows[row];
    if (cg.is_proper_congruence())
      cg.set_modulus(modulus);
  }

  PPL_ASSERT(lower_triangular(dest, dim_kinds));

  // Since we are reducing the system to "strong minimal form",
  // reduce the coefficients in the congruence system
  // using "diagonal" values.
  for (dimension_type dim = dims, i = 0; dim-- > 0; )
    if (dim_kinds[dim] != CON_VIRTUAL)
      // Factor the "diagonal" congruence out of the preceding rows.
      reduce_reduced<Congruence_System>
        (dest.rows, dim, i++, 0, dim, dim_kinds, false);

#ifndef NDEBUG
  // Make sure that all the rows are now OK.
  for (dimension_type i = dest.num_rows(); i-- > 0; )
    PPL_ASSERT(dest[i].OK());
#endif

  PPL_ASSERT(dest.OK());
}

void
Grid::conversion(Congruence_System& source, Grid_Generator_System& dest,
                 Dimension_Kinds& dim_kinds) {
  // Quite similar to the generator to congruence version above.
  // Changes here may be needed there too.

  PPL_ASSERT(lower_triangular(source, dim_kinds));

  // Initialize matrix row number counters and compute the LCM of the
  // diagonal entries of the proper congruences in `source'.
  dimension_type source_num_rows = 0;
  dimension_type dest_num_rows = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(diagonal_lcm);
  diagonal_lcm = 1;
  const dimension_type dims = source.space_dimension() + 1;
  for (dimension_type dim = dims; dim-- > 0; )
    if (dim_kinds[dim] == CON_VIRTUAL)
      // Virtual congruences map to lines.
      ++dest_num_rows;
    else {
      if (dim_kinds[dim] == PROPER_CONGRUENCE) {
        // Dimension `dim' has a proper congruence row at
        // `source_num_rows' in `source', so include in `diagonal_lcm'
        // the `dim'th element of that row.
        lcm_assign(diagonal_lcm, diagonal_lcm, source[source_num_rows].expr.get(dim));
        // Proper congruences map to parameters.
        ++dest_num_rows;
      }
      // Equalities map to virtual generators.
      ++source_num_rows;
    }

  // `source' must be regular.
  PPL_ASSERT(diagonal_lcm != 0);

  dest.clear();
  PPL_ASSERT(dims > 0);
  dest.set_space_dimension(dims - 1);

  // In `dest' initialize row types and elements, including setting
  // the diagonal elements to the inverse ratio of the `source'
  // diagonal elements.
  //
  // The top-down order of the congruence system rows corresponds to
  // the right-left order of the dimensions.
  dimension_type source_index = source_num_rows;
  // The generator system has a bottom-up ordering.
  for (dimension_type dim = 0; dim < dims; ++dim) {
    if (dim_kinds[dim] == EQUALITY) {
      --source_index;
    }
    else {
      Grid_Generator g(dest.representation());
      g.set_topology(dest.topology());
      g.expr.set_space_dimension(dims);

      if (dim_kinds[dim] == CON_VIRTUAL) {
        g.set_is_line();
        g.expr.set(0, Coefficient_zero());
        g.expr.set(dim, Coefficient_one());
      }
      else {
        PPL_ASSERT(dim_kinds[dim] == PROPER_CONGRUENCE);
        g.set_is_parameter_or_point();
        --source_index;
        PPL_DIRTY_TEMP_COEFFICIENT(tmp);
        exact_div_assign(tmp, diagonal_lcm,
                         source[source_index].expr.get(dim));
        g.expr.set(0, Coefficient_zero());
        g.expr.set(dim, tmp);
      }
      // Don't assert g.OK() here, because it may fail.
      // All the rows in `dest' are checked at the end of this function.
      dest.insert_verbatim(g);
    }
  }

  PPL_ASSERT(dest.num_rows() == dest_num_rows);
  PPL_ASSERT(dest.first_pending_row() == dest_num_rows);
  PPL_ASSERT(upper_triangular(dest, dim_kinds));

  // Convert.
  //
  // `source_index' and `dest_index' hold the positions of pivot rows
  // in `source' and `dest'.  The order of the rows in `dest' is the
  // reverse of the order in `source', so the rows are iterated from
  // last to first (index 0) in `source' and from first to last in
  // `dest'.
  source_index = source_num_rows;
  dimension_type dest_index = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(reduced_source_dim);

  for (dimension_type dim = 0; dim < dims; ++dim) {
    if (dim_kinds[dim] != CON_VIRTUAL) {
      --source_index;
      Coefficient_traits::const_reference source_dim
        = source[source_index].expr.get(dim);

      // In the rows in `dest' above `dest_index' divide each element
      // at column `dim' by `source_dim'.

      for (dimension_type i = dest_index; i-- > 0; ) {
        Grid_Generator& g = dest.sys.rows[i];

        // Multiply the representation of `dest' such that entry `dim'
        // of `g' is a multiple of `source_dim'.  This ensures that
        // the result of the division that follows is a whole number.
        gcd_assign(reduced_source_dim, g.expr.get(dim), source_dim);
        exact_div_assign(reduced_source_dim, source_dim, reduced_source_dim);
        multiply_grid(reduced_source_dim, g, dest.sys.rows, dest_num_rows);

        g.expr.exact_div_assign(source_dim, dim, dim + 1);
        // Don't assert g.OK() here, because it may fail.
        // All the rows in `dest' are checked at the end of this function.
      }
    }

    // Invert and transpose the source row at `source_index' into the
    // destination row at `dest_index'.
    //
    // Consider each dimension `dim_fol' that follows `dim', as the
    // rows in `dest' that follow row `dest_index' are zero at index
    // `dim'.
    dimension_type tmp_source_index = source_index;
    if (dim_kinds[dim] != EQUALITY)
      ++dest_index;
    for (dimension_type dim_fol = dim + 1; dim_fol < dims; ++dim_fol) {
      if (dim_kinds[dim_fol] != CON_VIRTUAL) {
        --tmp_source_index;
        Coefficient_traits::const_reference source_dim
          = source[tmp_source_index].expr.get(dim);
        // In order to compute the transpose of the inverse of
        // `source', subtract source[tmp_source_index][dim] times the
        // column vector in `dest' at `dim' from the column vector in
        // `dest' at `dim_fol'.
        //
        // I.e., for each row `dest_index' in `dest' that is above the
        // row `dest_index', subtract dest[tmp_source_index][dim]
        // times the entry `dim' from the entry at `dim_fol'.

        PPL_DIRTY_TEMP_COEFFICIENT(tmp);
        for (dimension_type i = dest_index; i-- > 0; ) {
          PPL_ASSERT(i < dest_num_rows);
          Grid_Generator& row = dest.sys.rows[i];
          tmp = row.expr.get(dim_fol);
          sub_mul_assign(tmp, source_dim,
                         row.expr.get(dim));
          row.expr.set(dim_fol, tmp);
          // Don't assert row.OK() here, because it may fail.
          // All the rows in `dest' are checked at the end of this function.
        }
      }
    }
  }

  PPL_ASSERT(upper_triangular(dest, dim_kinds));

  // Since we are reducing the system to "strong minimal form",
  // reduce the coordinates in the grid_generator system
  // using "diagonal" values.
  for (dimension_type dim = 0, i = 0; dim < dims; ++dim) {
    if (dim_kinds[dim] != GEN_VIRTUAL)
      // Factor the "diagonal" generator out of the preceding rows.
      reduce_reduced<Grid_Generator_System>
        (dest.sys.rows, dim, i++, dim, dims - 1, dim_kinds);
  }

  // Ensure that the parameter divisors are the same as the divisor of
  // the point.
  const Coefficient& system_divisor
    = dest.sys.rows[0].expr.inhomogeneous_term();

  for (dimension_type i = dest.sys.rows.size() - 1, dim = dims; dim-- > 1; ) {
    switch (dim_kinds[dim]) {
    case PARAMETER:
      dest.sys.rows[i].set_divisor(system_divisor);
      // Intentionally fall through.
    case LINE:
      --i;
      break;
    case GEN_VIRTUAL:
      break;
    }
  }

#ifndef NDEBUG
  // The previous code can modify the rows' fields, exploiting the friendness.
  // Check that all rows are OK now.
  for (dimension_type i = dest.sys.rows.size(); i-- > 0; )
    PPL_ASSERT(dest.sys.rows[i].OK());
#endif

  PPL_ASSERT(dest.sys.OK());
}

} // namespace Parma_Polyhedra_Library
