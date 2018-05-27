/* Test Polyhedron::contains(const Polyhedron&).
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

bool
test01() {
  Variable x(0);
  Variable y(1);

  C_Polyhedron segment(2);
  segment.add_constraint(x >= 0);
  segment.add_constraint(x <= 1);
  segment.add_constraint(y == 0);

  print_constraints(segment, "*** segment constraints ***");
  print_generators(segment, "*** segment generators ***");

  C_Polyhedron halfline(2);
  halfline.add_constraint(x >= 0);
  halfline.add_constraint(y == 0);

  print_constraints(halfline, "*** halfline constraints ***");
  print_generators(halfline, "*** halfline generators ***");

  bool segment_includes_halfline = segment.contains(halfline);

  nout << "segment ";
  if (segment_includes_halfline)
    nout << "includes ";
  else
    nout << "does not include ";
  nout << "or is equal to halfline" << endl;

  bool halfline_includes_segment = halfline.contains(segment);

  nout << "halfline ";
  if (halfline_includes_segment)
    nout << "includes ";
  else
    nout << "does not include ";
  nout << "or is equal to segment" << endl;

  return halfline_includes_segment && !segment_includes_halfline;
}

bool
test02() {
  Variable x(0);
  Variable y(1);

  Constraint_System cs;
  cs.insert(x + 0*y >= 2);
  C_Polyhedron p_half_space(cs);

  print_constraints(p_half_space, "*** p_half_space constraints ***");
  print_generators(p_half_space, "*** p_half_space generators ***");

  Generator_System gs;
  gs.insert(point(2*x));
  gs.insert(line(x+y));
  C_Polyhedron p_line(gs);

  print_constraints(p_line, "*** p_line constraints ***");
  print_generators(p_line, "*** p_line generators ***");

  bool p_half_space_includes_p_line = p_half_space.contains(p_line);

  nout << "p_half_space ";
  if (p_half_space_includes_p_line)
    nout << "includes ";
  else
    nout << "does not include ";
  nout << "or is equal to p_line" << endl;

  bool p_line_includes_p_half_space = p_line.contains(p_half_space);

  nout << "p_line ";
  if (p_line_includes_p_half_space)
    nout << "includes ";
  else
    nout << "does not include ";
  nout << "or is equal to p_half_space" << endl;

  return !p_line_includes_p_half_space && !p_half_space_includes_p_line;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
END_MAIN
