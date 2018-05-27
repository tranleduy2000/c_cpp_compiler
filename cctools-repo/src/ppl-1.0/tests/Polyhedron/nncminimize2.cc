/* Test NNC_Polyhedron::ph.minimized_generators().
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
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);

  NNC_Polyhedron ph(5, EMPTY);
  ph.add_generator(point(52131*A + 11369*B - 483036*C - 776212*D - 401649*E));
  ph.add_generator(point(-80461*A + 86836*B + 88698*C - 269590*D + 951545*E));
  ph.add_generator(point(215169*A - 42532*B + 234755*C + 705077*D - 632177*E));
  ph.add_generator(point(-294493*A - 44546*B + 861680*C + 344228*D + 224283*E));
  ph.add_generator(point(-158536*A + 273936*B - 854715*C - 151614*D + 382491*E));
  ph.add_generator(point(-207881*A - 205091*B - 385540*C + 729319*D + 483915*E));
  ph.add_generator(point(-1084*A + 2507*B - 28428*C + 1882*D - 999590*E));

  ph.add_generator(point(450943*A - 416394*B + 883*C + 680432*D - 400345*E));
  ph.add_generator(point(-483589*A - 16983*B - 824243*C + 106969*D - 273921*E));
  ph.add_generator(point(554952*A - 155460*B - 572886*C - 91530*D + 575571*E));
  ph.ascii_dump(vnout);
  vnout << endl;

  const Generator_System& gs = ph.minimized_generators();
  ph.ascii_dump(vnout);
  vnout << endl;

  if (!ph.OK())
    return false;

  bool ok = true;
  unsigned num_points = 0;
  for (Generator_System::const_iterator i = gs.begin(), gs_end = gs.end();
       i != gs_end;
       ++i) {
    using namespace IO_Operators;
    nout << *i << endl;
    if (i->type() != Generator::POINT) {
      nout << "i->type() == " << i->type() << endl;
      ok = false;
    }
    ++num_points;
  }

  nout << "points = " << num_points << endl;

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F64(test01);
END_MAIN
