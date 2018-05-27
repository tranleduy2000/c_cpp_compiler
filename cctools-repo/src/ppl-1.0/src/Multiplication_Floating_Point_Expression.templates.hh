/* Multiplication_Floating_Point_Expression class implementation:
   non-inline template functions.
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

#ifndef PPL_Multiplication_Floating_Point_Expression_templates_hh
#define PPL_Multiplication_Floating_Point_Expression_templates_hh 1

namespace Parma_Polyhedra_Library {

template <typename FP_Interval_Type, typename FP_Format>
bool Multiplication_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::linearize(const FP_Interval_Abstract_Store& int_store,
            const FP_Linear_Form_Abstract_Store& lf_store,
            FP_Linear_Form& result) const {
  /*
    FIXME: We currently adopt the "Interval-Size Local" strategy in order to
    decide which of the two linear forms must be intervalized, as described
    in Section 6.2.4 ("Multiplication Strategies") of Antoine Mine's Ph.D.
    thesis "Weakly Relational Numerical Abstract Domains".
    In this Section are also described other multiplication strategies, such
    as All-Cases, Relative-Size Local, Simplification-Driven Global and
    Homogeneity Global.
  */

  // Here we choose which of the two linear forms must be intervalized.

  // true if we intervalize the first form, false if we intervalize the second.
  bool intervalize_first;
  FP_Linear_Form linearized_first_operand;
  if (!first_operand->linearize(int_store, lf_store,
                               linearized_first_operand))
    return false;
  FP_Interval_Type intervalized_first_operand;
  this->intervalize(linearized_first_operand, int_store,
                    intervalized_first_operand);
  FP_Linear_Form linearized_second_operand;
  if (!second_operand->linearize(int_store, lf_store,
                                linearized_second_operand))
    return false;
  FP_Interval_Type intervalized_second_operand;
  this->intervalize(linearized_second_operand, int_store,
                    intervalized_second_operand);

  // FIXME: we are not sure that what we do here is policy-proof.
  if (intervalized_first_operand.is_bounded()) {
    if (intervalized_second_operand.is_bounded()) {
      boundary_type first_interval_size
        = intervalized_first_operand.upper()
        - intervalized_first_operand.lower();
      boundary_type second_interval_size
        = intervalized_second_operand.upper()
        - intervalized_second_operand.lower();
      if (first_interval_size <= second_interval_size)
        intervalize_first = true;
      else
        intervalize_first = false;
    }
    else
      intervalize_first = true;
  }
  else {
    if (intervalized_second_operand.is_bounded())
      intervalize_first = false;
    else
      return false;
  }

  // Here we do the actual computation.
  // For optimizing, we store the relative error directly into result.
  if (intervalize_first) {
    relative_error(linearized_second_operand, result);
    linearized_second_operand *= intervalized_first_operand;
    result *= intervalized_first_operand;
    result += linearized_second_operand;
  }
  else {
    relative_error(linearized_first_operand, result);
    linearized_first_operand *= intervalized_second_operand;
    result *= intervalized_second_operand;
    result += linearized_first_operand;
  }

  result += this->absolute_error;
  return !this->overflows(result);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Multiplication_Floating_Point_Expression_templates_hh)
