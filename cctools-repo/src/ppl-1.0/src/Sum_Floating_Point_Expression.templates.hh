/* Sum_Floating_Point_Expression class implementation:
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

#ifndef PPL_Sum_Floating_Point_Expression_templates_hh
#define PPL_Sum_Floating_Point_Expression_templates_hh 1

namespace Parma_Polyhedra_Library {

template <typename FP_Interval_Type, typename FP_Format>
bool Sum_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::linearize(const FP_Interval_Abstract_Store& int_store,
            const FP_Linear_Form_Abstract_Store& lf_store,
            FP_Linear_Form& result) const {
  if (!first_operand->linearize(int_store, lf_store, result))
    return false;
  FP_Linear_Form rel_error;
  relative_error(result, rel_error);
  result += rel_error;
  FP_Linear_Form linearized_second_operand;
  if (!second_operand->linearize(int_store, lf_store,
                                linearized_second_operand))
    return false;
  result += linearized_second_operand;
  relative_error(linearized_second_operand, rel_error);
  result += rel_error;
  result += this->absolute_error;
  return !this->overflows(result);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Sum_Floating_Point_Expression_templates_hh)
