/* Division_Floating_Point_Expression class implementation:
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

#ifndef PPL_Division_Floating_Point_Expression_templates_hh
#define PPL_Division_Floating_Point_Expression_templates_hh 1

namespace Parma_Polyhedra_Library {

template <typename FP_Interval_Type, typename FP_Format>
bool Division_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::linearize(const FP_Interval_Abstract_Store& int_store,
            const FP_Linear_Form_Abstract_Store& lf_store,
            FP_Linear_Form& result) const {
  FP_Linear_Form linearized_second_operand;
  if (!second_operand->linearize(int_store, lf_store,
                                linearized_second_operand))
    return false;
  FP_Interval_Type intervalized_second_operand;
  this->intervalize(linearized_second_operand, int_store,
                    intervalized_second_operand);

  // Check if we may divide by zero.
  if (intervalized_second_operand.lower() <= 0
      && intervalized_second_operand.upper() >= 0)
    return false;

  if (!first_operand->linearize(int_store, lf_store, result))
    return false;
  FP_Linear_Form rel_error;
  relative_error(result, rel_error);
  result /= intervalized_second_operand;
  rel_error /= intervalized_second_operand;
  result += rel_error;
  result += this->absolute_error;
  return !this->overflows(result);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Division_Floating_Point_Expression_templates_hh)
