/* Test Box::add_space_dimensions_and_embed():
   we add two variables to a Box.
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

#include "ppl_test.hh"
#include <complex>
#include <cmath>
#include <cstdio>

namespace {

template <typename F>
struct My_Interval {

  struct Floating_Point_Real_Interval_Info_Policy {
    const_bool_nodef(store_special, false);
    const_bool_nodef(store_open, true);
    const_bool_nodef(cache_empty, true);
    const_bool_nodef(cache_singleton, true);
    const_bool_nodef(cache_normalized, false);
    const_int_nodef(next_bit, 0);
    const_bool_nodef(may_be_empty, false);
    const_bool_nodef(may_contain_infinity, false);
    const_bool_nodef(check_empty_result, false);
    const_bool_nodef(check_inexact, false);
  };

  typedef Interval_Info_Bitset<unsigned int,
                               Floating_Point_Real_Interval_Info_Policy>
  Floating_Point_Real_Interval_Info;

  typedef Interval<F, Floating_Point_Real_Interval_Info> interval_type;
};

template<typename F>
bool
test01() {
  typename My_Interval<F>::interval_type x;
  x = 2;
  typename My_Interval<F>::interval_type two;
  two = 2;
  typename My_Interval<F>::interval_type y;

  nout << "x = " << x << endl;

  for (int i = 0; i <= 100; ++i) {
    // Compute x = (x+(2/x))/2.
    y = two;
    y /= x;
    x += y;
    x /= two;
    nout << "x = " << x << endl;
  }

  typename My_Interval<F>::interval_type z(F(1.41420757770538330078125));
  z.join_assign(F(1.41421949863433837890625));

  nout << "z = " << x << endl;

  return !x.is_empty()
#if PPL_CXX_SUPPORTS_IEEE_INEXACT_FLAG
    && x.is_disjoint_from(F(1.41420757770538330078125))
    && x.is_disjoint_from(F(1.41421949863433837890625))
    && z.strictly_contains(x)
#else
    && x.is_topologically_closed()
    && z.contains(x)
#endif
    && !x.contains_integer_point();
}

// Note: this function works both with interval and native floating
// point types.
template <typename N>
void
polynomial_evaluate(const std::vector<N>& P,
                    const std::complex<N>& x,
                    std::complex<N>& P_x) {
  // Note: the coefficient of the leading term is implicitly 1.
  P_x = std::complex<N>(N(1), N(0));
  for (int i = P.size(); i >= 1; --i)
    P_x = P_x*x + P[i-1];
}

// Note: this function works both with interval and native floating
// point types.
template <typename N>
void
solve(const std::vector<N>& P,
      std::vector<std::complex<N> >& roots) {
  const int degree = P.size();
  if (degree < 1)
    throw std::invalid_argument("the polynomial must have degree at least 1");

  // Initial estimates are given by roots of unity.
  std::vector<std::complex<N> > x(5);
  double theta = 2*M_PI/degree;
  for (int i = 0; i < degree; ++i) {
    // Many libm implementations only work with round-to-nearest.
    // See, e.g, http://sources.redhat.com/bugzilla/show_bug.cgi?id=3976
    restore_pre_PPL_rounding();
    N cos_i_theta = N(cos(i*theta));
    N sin_i_theta = N(sin(i*theta));
    set_rounding_for_PPL();

    x[i] = std::complex<N>(cos_i_theta, sin_i_theta);
  }

  for (int iteration = 0; iteration < 50; ++iteration) {
    for (int i = 0; i < degree; ++i)
      nout << "x[" << i << "] = " << x[i] << endl;
    for (int i = 0; i < degree; ++i) {
      std::complex<N> P_x_i;
      polynomial_evaluate(P, x[i], P_x_i);
      std::complex<N> d(N(1), N(0));
      for (int j = 0; j < degree; ++j)
        if (i != j)
          d *= (x[i] - x[j]);
      P_x_i /= d;
      x[i] -= P_x_i;
    }
  }
  roots.resize(degree+1);
  for (int i = 0; i < degree; ++i)
    roots[i] = x[i];
}

template <typename N>
bool test02() {
  std::vector<N> P(4);
  // x^4 + 5*x^3 + 7*x^2 + 134*x + 1
  P[3] = 5;
  P[2] = 7;
  P[1] = 134;
  P[0] = 1;
  std::vector<std::complex<N> > roots;
  solve(P, roots);
  return true;
}

template <typename N>
bool test03() {
  std::vector<N> P(2);
  // x^2 - 1
  P[1] = 0;
  P[0] = -1;
  std::vector<std::complex<N> > roots;
  solve(P, roots);
  return true;
}

template <typename N>
bool test04() {
  std::vector<N> P(2);
  // x^2 - 1
  P[1] = 0;
  P[0] = -1;
  for (int d = 0; d <= 10; ++d) {
    std::complex<N> P_x_i;
    polynomial_evaluate(P,
                        std::complex<N>(N(d), N(0)),
                        P_x_i);
    nout << d << " " << P_x_i << endl;
  }
  return true;
}

template<typename F>
bool
test05() {
  typename My_Interval<F>::interval_type x("123.00123");
  nout << "x = " << x << endl;
  return true;
}

} // namespace

BEGIN_MAIN

#if PPL_SUPPORTED_FLOAT
  DO_TEST(test01<float>);
  DO_TEST(test02<float>);
  DO_TEST(test03<float>);
  DO_TEST(test04<float>);
  DO_TEST(test05<float>);
#endif // PPL_SUPPORTED_FLOAT

#if PPL_SUPPORTED_DOUBLE
  DO_TEST(test01<double>);
  DO_TEST(test02<double>);
  DO_TEST(test03<double>);
  DO_TEST(test04<double>);
  DO_TEST(test05<double>);
#endif // PPL_SUPPORTED_DOUBLE

#if PPL_SUPPORTED_LONG_DOUBLE
  DO_TEST(test01<long double>);
  DO_TEST(test02<long double>);
  DO_TEST(test03<long double>);
  DO_TEST(test04<long double>);
  DO_TEST(test05<long double>);
#endif // PPL_SUPPORTED_LONG_DOUBLE

END_MAIN
