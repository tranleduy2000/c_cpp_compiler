/* Test Pointset_Powerset<PH>::add_space_dimensions(),
        Pointset_Powerset<PH>::remove_higher_space_dimensions(),
        Pointset_Powerset<PH>::remove_space_dimensions(),
        Pointset_Powerset<PH>::expand_space_dimensions(),
        Pointset_Powerset<PH>::fold_space_dimensions().
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

// Powerset of C polyhedra: add_space_dimensions_and_embed(),
// add_space_dimensions_and_project().
bool
test01() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x == 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x <= 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  dimension_type m = 2;

  ps.add_space_dimensions_and_embed(m);
  bool ok = (ps.space_dimension() == 3 && ps.affine_dimension() == 3);

  ps.add_space_dimensions_and_project(m);
  bool ok1 = (ps.space_dimension() == 5 && ps.affine_dimension() == 3);

  ps.remove_higher_space_dimensions(4);
  bool ok2 = (ps.space_dimension() == 4 && ps.affine_dimension() == 3);

  Pointset_Powerset<Rational_Box> psb(7, EMPTY);
  Rational_Box b(7);
  b.add_constraint(x >= 1);
  b.add_constraint(x <= 0);
  psb.add_disjunct(b);
  bool ok3 = (psb.space_dimension() == 7 && psb.affine_dimension() == 0);

  Pointset_Powerset<Grid> psg(7, EMPTY);
  Grid g(7);
  g.add_congruence((x %= 0) / 2);
  g.add_congruence((x %= 1) / 2);
  psg.add_disjunct(g);
  bool ok4 = (psg.space_dimension() == 7 && psg.affine_dimension() == 0);

  return ok && ok1 && ok2 && ok3 && ok4 && ps.OK();
}

// Powerset of C polyhedra: remove_higher_space_dimensions().
bool
test02() {
  Variable x(0);

  C_Polyhedron ph1(1);
  ph1.add_constraint(x == 1);

  C_Polyhedron ph2(1);
  ph2.add_constraint(x <= 2);
  Pointset_Powerset<C_Polyhedron> ps(1, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  dimension_type m = 2;

  ps.add_space_dimensions_and_embed(m);
  bool ok = (ps.space_dimension() == 3 && ps.affine_dimension() == 3);

  ps.add_space_dimensions_and_project(m);
  bool ok1 = (ps.space_dimension() == 5 && ps.affine_dimension() == 3);

  ps.remove_higher_space_dimensions(4);
  bool ok2 = (ps.space_dimension() == 4 && ps.affine_dimension() == 3);

  Pointset_Powerset<Rational_Box> psb(7, EMPTY);
  Rational_Box b(7);
  b.add_constraint(x >= 1);
  b.add_constraint(x <= 0);
  psb.add_disjunct(b);
  bool ok3 = (psb.space_dimension() == 7 && psb.affine_dimension() == 0);

  Pointset_Powerset<Grid> psg(7, EMPTY);
  Grid g(7);
  g.add_congruence((x %= 0) / 2);
  g.add_congruence((x %= 1) / 2);
  psg.add_disjunct(g);
  bool ok4 = (psg.space_dimension() == 7 && psg.affine_dimension() == 0);

  return ok && ok1 && ok2 && ok3 && ok4 && ps.OK();
}

// Powerset of C polyhedra: remove_space_dimensions().
bool
test03() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph1(4);
  ph1.add_constraint(x == 1);
  ph1.add_constraint(z == 1);

  C_Polyhedron ph2(4);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(z == 1);
  Pointset_Powerset<C_Polyhedron> ps(4, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Variables_Set to_be_removed;
  to_be_removed.insert(y);
  to_be_removed.insert(w);

  ps.remove_space_dimensions(to_be_removed);
  bool ok = (ps.space_dimension() == 2 && ps.affine_dimension() == 1);

  return ok && ps.OK();
}

// Powerset of C polyhedra: expand_space_dimension().
bool
test04() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph1(4);
  ph1.add_constraint(x == 1);
  ph1.add_constraint(z == 1);

  C_Polyhedron ph2(4);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(z == 1);
  Pointset_Powerset<C_Polyhedron> ps(4, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  ps.expand_space_dimension(y, 2);
  bool ok = (ps.space_dimension() == 6 && ps.affine_dimension() == 5);

  return ok && ps.OK();
}

// Powerset of C polyhedra: fold_space_dimensions().
bool
test05() {
  Variable x(0);
  Variable y(1);
  Variable z(2);
  Variable w(3);

  C_Polyhedron ph1(4);
  ph1.add_constraint(x == 1);
  ph1.add_constraint(z == 1);

  C_Polyhedron ph2(4);
  ph2.add_constraint(x <= 2);
  ph2.add_constraint(z == 1);
  Pointset_Powerset<C_Polyhedron> ps(4, EMPTY);

  ps.add_disjunct(ph1);
  ps.add_disjunct(ph2);

  Variables_Set to_be_folded;
  to_be_folded.insert(y);
  to_be_folded.insert(w);

  ps.fold_space_dimensions(to_be_folded, z);
  bool ok = (ps.space_dimension() == 2 && ps.affine_dimension() == 2);

  return ok && ps.OK();
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
END_MAIN
