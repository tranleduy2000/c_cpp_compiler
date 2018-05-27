/* Variables_Set class implementation (non-inline functions).
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
#include "Variables_Set_defs.hh"
#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

PPL::Variables_Set::Variables_Set(const Variable v, const Variable w)
  : Base() {
  for (dimension_type d = v.id(), last = w.id(); d <= last; ++d)
    insert(d);
}

bool
PPL::Variables_Set::OK() const {
  for (const_iterator i = begin(), set_end = end(); i != set_end; ++i)
    if (!Variable(*i).OK())
      return false;
  return true;
}

/*! \relates Parma_Polyhedra_Library::Variables_Set */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Variables_Set& vs) {
  s << '{';
  for (Variables_Set::const_iterator i = vs.begin(),
         vs_end = vs.end(); i != vs_end; ) {
    s << ' ' << Variable(*i);
    ++i;
    if (i != vs_end)
      s << ',';
  }
  s << " }";
  return s;
}

void
PPL::Variables_Set::ascii_dump(std::ostream& s) const {
  const dimension_type variables_set_size = size();
  s << "\nvariables( " << variables_set_size << " )\n";
  for (Variables_Set::const_iterator i = begin(),
         i_end = end(); i != i_end; ++i)
    s << *i << " ";
}

PPL_OUTPUT_DEFINITIONS(Variables_Set)

bool
PPL::Variables_Set::ascii_load(std::istream& s) {
  clear();
  std::string str;
  if (!(s >> str) || str != "variables(")
    return false;

  dimension_type size;

  if (!(s >> size))
    return false;

  if (!(s >> str) || str != ")")
    return false;

  for (dimension_type i = 0; i < size; ++i) {
    dimension_type variable_value;
    if (!(s >> variable_value))
      return false;
    insert(variable_value);
  }
  return true;
}
