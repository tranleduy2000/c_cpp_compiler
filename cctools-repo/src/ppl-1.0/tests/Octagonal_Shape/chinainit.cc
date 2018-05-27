/* Simulate the initialization phase of the China analyzer.
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

#include "ppl_test.hh"

namespace {

TOctagonal_Shape
init(dimension_type num_vars) {
  TOctagonal_Shape oc(num_vars);
  if (num_vars > 0) {
    Constraint_System cs;
    for (dimension_type i = num_vars; i-- > 0; )
      cs.insert(Variable(i) >= 0);
    oc = TOctagonal_Shape(cs);
#if NOISY
    print_constraints(oc, "*** oc ***");
#endif
  }
  return oc;
}

bool
test01(){
  Partial_Function function;
  function.insert(0, 1);
  function.insert(1, 0);
  function.insert(3, 2);
  function.insert(5, 3);
  function.insert(7, 4);
  function.insert(9, 5);

  print_function(function, "*** function ***");


  TOctagonal_Shape known_result = init(6);

  for (dimension_type i = 10; i < 50; ++i) {
    TOctagonal_Shape oc = init(i);
    oc.map_space_dimensions(function);
    if (oc != known_result) {
      print_constraints(oc, "*** oc ***");
      print_constraints(known_result, "*** known_result ***");
      return false;
    }
  }

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
END_MAIN

