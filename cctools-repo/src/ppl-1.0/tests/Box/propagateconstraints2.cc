/* Test Box::propagate_constraints().
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

struct Edge {
  unsigned int from;
  unsigned int to;
  unsigned long distance;
};

Edge hawaii[] = {
  { 0, 1, 6191 },
  { 1, 0, 6191 },
  { 2, 3, 4147 },
  { 3, 2, 4147 },
  { 4, 5, 3884 },
  { 5, 4, 3884 },
  { 5, 6, 1609 },
  { 6, 5, 1609 },
  { 7, 0, 12521 },
  { 0, 7, 12521 },
  { 8, 1, 2872 },
  { 1, 8, 2872 },
  { 9, 10, 4036 },
  { 10, 9, 4036 },
  { 4, 11, 2959 },
  { 11, 4, 2959 },
  { 12, 13, 27415 },
  { 13, 12, 27415 },
  { 14, 15, 6068 },
  { 15, 14, 6068 },
  { 15, 16, 7337 },
  { 16, 15, 7337 },
  { 17, 18, 2894 },
  { 18, 17, 2894 },
  { 18, 19, 5425 },
  { 19, 18, 5425 },
  { 20, 21, 2265 },
  { 21, 20, 2265 },
  { 19, 20, 3008 },
  { 20, 19, 3008 },
  { 21, 12, 6654 },
  { 12, 21, 6654 },
  { 22, 23, 1563 },
  { 23, 22, 1563 },
  { 23, 24, 3470 },
  { 24, 23, 3470 },
  { 25, 26, 6536 },
  { 26, 25, 6536 },
  { 27, 28, 10373 },
  { 28, 27, 10373 },
  { 28, 29, 7866 },
  { 29, 28, 7866 },
  { 29, 30, 8961 },
  { 30, 29, 8961 },
  { 31, 32, 6954 },
  { 32, 31, 6954 },
  { 33, 34, 24135 },
  { 34, 33, 24135 },
  { 34, 35, 5943 },
  { 35, 34, 5943 },
  { 33, 36, 2389 },
  { 36, 33, 2389 },
  { 35, 37, 3374 },
  { 37, 35, 3374 },
  { 34, 38, 4494 },
  { 38, 34, 4494 },
  { 37, 38, 8295 },
  { 38, 37, 8295 },
  { 38, 39, 12091 },
  { 39, 38, 12091 },
  { 39, 40, 4356 },
  { 40, 39, 4356 },
  { 39, 30, 3986 },
  { 30, 39, 3986 },
  { 29, 41, 3937 },
  { 41, 29, 3937 },
  { 41, 28, 7159 },
  { 28, 41, 7159 },
  { 42, 43, 3887 },
  { 43, 42, 3887 },
  { 43, 44, 3685 },
  { 44, 43, 3685 },
  { 43, 45, 20903 },
  { 45, 43, 20903 },
  { 45, 46, 2226 },
  { 46, 45, 2226 },
  { 47, 44, 7487 },
  { 44, 47, 7487 },
  { 44, 48, 22511 },
  { 48, 44, 22511 },
  { 46, 48, 7280 },
  { 48, 46, 7280 },
  { 48, 49, 3112 },
  { 49, 48, 3112 },
  { 49, 50, 2530 },
  { 50, 49, 2530 },
  { 50, 51, 17408 },
  { 51, 50, 17408 },
  { 45, 52, 15249 },
  { 52, 45, 15249 },
  { 53, 54, 36470 },
  { 54, 53, 36470 },
  { 54, 49, 7694 },
  { 49, 54, 7694 },
  { 51, 55, 11400 },
  { 55, 51, 11400 },
  { 52, 46, 15473 },
  { 46, 52, 15473 },
  { 55, 56, 37433 },
  { 56, 55, 37433 },
  { 53, 57, 3855 },
  { 57, 53, 3855 },
  { 57, 58, 1508 },
  { 58, 57, 1508 },
  { 59, 60, 3574 },
  { 60, 59, 3574 },
  { 59, 53, 7377 },
  { 53, 59, 7377 },
  { 58, 60, 6801 },
  { 60, 58, 6801 },
  { 60, 61, 9191 },
  { 61, 60, 9191 },
  { 61, 62, 3940 },
  { 62, 61, 3940 },
  { 62, 63, 1941 },
  { 63, 62, 1941 },
  { 63, 64, 17861 },
  { 64, 63, 17861 },
  { 65, 66, 12138 },
  { 66, 65, 12138 },
  { 66, 67, 5764 },
  { 67, 66, 5764 },
  { 68, 62, 2365 },
  { 62, 68, 2365 },
  { 69, 70, 14756 },
  { 70, 69, 14756 },
  { 70, 71, 3931 },
  { 71, 70, 3931 },
  { 71, 72, 6955 },
  { 72, 71, 6955 },
  { 69, 73, 4265 },
  { 73, 69, 4265 },
  { 73, 56, 5278 },
  { 56, 73, 5278 },
  { 74, 75, 1088 },
  { 75, 74, 1088 },
  { 76, 77, 2549 },
  { 77, 76, 2549 },
  { 78, 79, 2029 },
  { 79, 78, 2029 },
  { 80, 81, 4607 },
  { 81, 80, 4607 },
  { 81, 82, 995 },
  { 82, 81, 995 },
  { 82, 83, 5708 },
  { 83, 82, 5708 },
  { 83, 84, 1015 },
  { 84, 83, 1015 },
  { 85, 86, 2592 },
  { 86, 85, 2592 },
  { 86, 75, 9272 },
  { 75, 86, 9272 },
  { 86, 87, 1549 },
  { 87, 86, 1549 },
  { 87, 88, 1155 },
  { 88, 87, 1155 },
  { 85, 84, 8375 },
  { 84, 85, 8375 },
  { 84, 89, 3022 },
  { 89, 84, 3022 },
  { 89, 90, 6455 },
  { 90, 89, 6455 },
  { 80, 91, 2912 },
  { 91, 80, 2912 },
  { 78, 92, 2087 },
  { 92, 78, 2087 },
  { 91, 76, 7736 },
  { 76, 91, 7736 },
  { 88, 85, 2143 },
  { 85, 88, 2143 },
  { 93, 8, 9507 },
  { 8, 93, 9507 },
  { 89, 88, 9385 },
  { 88, 89, 9385 },
  { 64, 65, 24795 },
  { 65, 64, 24795 },
  { 54, 94, 32035 },
  { 94, 54, 32035 },
  { 94, 56, 35025 },
  { 56, 94, 35025 },
  { 69, 95, 25243 },
  { 95, 69, 25243 },
  { 67, 96, 30757 },
  { 96, 67, 30757 },
  { 96, 97, 859 },
  { 97, 96, 859 },
  { 97, 95, 3230 },
  { 95, 97, 3230 },
  { 1, 9, 16155 },
  { 9, 1, 16155 },
  { 98, 51, 11706 },
  { 51, 98, 11706 },
  { 75, 99, 51590 },
  { 99, 75, 51590 },
  { 11, 100, 23417 },
  { 100, 11, 23417 },
  { 100, 101, 12368 },
  { 101, 100, 12368 },
  { 102, 103, 4846 },
  { 103, 102, 4846 },
  { 103, 104, 2901 },
  { 104, 103, 2901 },
  { 10, 104, 7849 },
  { 104, 10, 7849 },
  { 104, 2, 2147 },
  { 2, 104, 2147 },
  { 2, 102, 1761 },
  { 102, 2, 1761 },
  { 102, 4, 4444 },
  { 4, 102, 4444 },
  { 74, 105, 8183 },
  { 105, 74, 8183 },
  { 106, 107, 7075 },
  { 107, 106, 7075 },
  { 89, 107, 1178 },
  { 107, 89, 1178 },
  { 108, 81, 1312 },
  { 81, 108, 1312 },
  { 81, 109, 1793 },
  { 109, 81, 1793 },
  { 82, 108, 1001 },
  { 108, 82, 1001 },
  { 108, 80, 7421 },
  { 80, 108, 7421 },
  { 110, 92, 434 },
  { 92, 110, 434 },
  { 92, 79, 3285 },
  { 79, 92, 3285 },
  { 79, 99, 2706 },
  { 99, 79, 2706 },
  { 111, 78, 3052 },
  { 78, 111, 3052 },
  { 77, 111, 2036 },
  { 111, 77, 2036 },
  { 24, 25, 2634 },
  { 25, 24, 2634 },
  { 25, 21, 1832 },
  { 21, 25, 1832 },
  { 33, 112, 14461 },
  { 112, 33, 14461 },
  { 113, 114, 7998 },
  { 114, 113, 7998 },
  { 41, 115, 6937 },
  { 115, 41, 6937 },
  { 32, 116, 47595 },
  { 116, 32, 47595 },
  { 99, 117, 3887 },
  { 117, 99, 3887 },
  { 117, 118, 736 },
  { 118, 117, 736 },
  { 85, 83, 9281 },
  { 83, 85, 9281 },
  { 118, 80, 4823 },
  { 80, 118, 4823 },
  { 77, 119, 22341 },
  { 119, 77, 22341 },
  { 56, 120, 1612 },
  { 120, 56, 1612 },
  { 79, 121, 3258 },
  { 121, 79, 3258 },
  { 52, 57, 25357 },
  { 57, 52, 25357 },
  { 99, 121, 1646 },
  { 121, 99, 1646 },
  { 121, 76, 1359 },
  { 76, 121, 1359 },
  { 37, 27, 3890 },
  { 27, 37, 3890 },
  { 27, 31, 5066 },
  { 31, 27, 5066 },
  { 122, 123, 2556 },
  { 123, 122, 2556 },
  { 90, 122, 947 },
  { 122, 90, 947 },
  { 122, 124, 911 },
  { 124, 122, 911 },
  { 124, 125, 23829 },
  { 125, 124, 23829 },
  { 91, 119, 7911 },
  { 119, 91, 7911 },
};

Edge hawaii1[] = {
  { 1, 0, 600 },
  { 2, 1, 2872 },
};

Edge hawaii2[] = {
  { 0, 1, 600 },
  { 1, 2, 2872 },
};

const mpq_class&
perturbate(unsigned long a) {
  static mpq_class q;
  q = a;
  q = (q*q)/(q-1);
  return q;
}

template <typename T>
void
propagate_edges(Box<T>& box, const Edge* edges, unsigned n) {
  Constraint_System cs;
  for (unsigned i = 0; i < n; ++i) {
    const mpq_class& q = perturbate(edges[i].distance);
    Coefficient a;
    a = q.get_den();
    Coefficient b;
    b = q.get_num();

    vnout << "a = " << a << "; b = " << b << endl;

    cs.insert(a*Variable(edges[i].from) - a*Variable(edges[i].to) <= b);
  }
  box.propagate_constraints(cs);
}

} // namespace

bool test01() {
  Rational_Box qbox1(126);

  qbox1.add_constraint(Variable(0) >= 100000);
  qbox1.add_constraint(Variable(0) <= 100001);
  qbox1.add_constraint(Variable(12) >= 110000);
  qbox1.add_constraint(Variable(12) <= 110001);
  qbox1.add_constraint(Variable(14) >= 120000);
  qbox1.add_constraint(Variable(14) <= 120001);
  qbox1.add_constraint(Variable(27) >= 130000);
  qbox1.add_constraint(Variable(27) <= 130001);
  qbox1.add_constraint(Variable(42) >= 140000);
  qbox1.add_constraint(Variable(42) <= 140001);
  qbox1.add_constraint(Variable(113) >= 150000);
  qbox1.add_constraint(Variable(113) <= 150001);
  qbox1.add_constraint(Variable(125) >= 200000);
  qbox1.add_constraint(Variable(125) <= 200001);

  Rational_Box qbox2(qbox1);

  print_constraints(qbox1, "*** qbox1, qbox2 ***");

  propagate_edges(qbox2, hawaii, sizeof(hawaii)/sizeof(Edge));

  print_constraints(qbox2, "*** qbox2.propagate_edges() ***");

  TBox tbox(qbox1);

  print_constraints(tbox, "*** tbox ***");

  propagate_edges(tbox, hawaii, sizeof(hawaii)/sizeof(Edge));

  print_constraints(tbox, "*** tbox.propagate_edges() ***");

#if PPL_SUPPORTED_DOUBLE
  bool ok = check_result(tbox, qbox2, "18.36", "2.83", "2.79");
#else
  bool ok = check_result(tbox, qbox2, "30.96", "2.83", "2.79");
#endif

  return ok;
}

bool test02() {
  Rational_Box qbox1(3);

  qbox1.add_constraint(Variable(0) <= 0);

  Rational_Box qbox2(qbox1);

  print_constraints(qbox1, "*** qbox1, qbox2 ***");

  propagate_edges(qbox2, hawaii1, sizeof(hawaii1)/sizeof(Edge));

  print_constraints(qbox2, "*** qbox2.propagate_edges() ***");

  TBox tbox(qbox1);

  print_constraints(tbox, "*** tbox ***");

  propagate_edges(tbox, hawaii1, sizeof(hawaii1)/sizeof(Edge));

  print_constraints(tbox, "*** tbox.propagate_edges() ***");

  bool ok = check_result(tbox, qbox2, "3.28", "2.83", "2.79");

  return ok;
}

bool test03() {
  Rational_Box qbox1(3);

  qbox1.add_constraint(Variable(0) >= 0);

  Rational_Box qbox2(qbox1);

  print_constraints(qbox1, "*** qbox1, qbox2 ***");

  propagate_edges(qbox2, hawaii2, sizeof(hawaii2)/sizeof(Edge));

  print_constraints(qbox2, "*** qbox2.propagate_edges() ***");

  TBox tbox(qbox1);

  print_constraints(tbox, "*** tbox ***");

  propagate_edges(tbox, hawaii2, sizeof(hawaii2)/sizeof(Edge));

  print_constraints(tbox, "*** tbox.propagate_edges() ***");

  bool ok = check_result(tbox, qbox2, "3.28", "2.83", "2.78");

  return ok;
}

BEGIN_MAIN
  DO_TEST_F32(test01);
  DO_TEST_F16(test02);
  DO_TEST_F16(test03);
END_MAIN
