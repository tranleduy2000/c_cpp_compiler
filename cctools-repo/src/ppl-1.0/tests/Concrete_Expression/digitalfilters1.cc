/* Test Rate Limiter on differents abstract domains.
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
#include "C_Expr.defs.hh"

namespace {

class Test_Oracle : public FP_Oracle<C_Expr,FP_Interval> {
public:
  Test_Oracle() : int_store(0) {}

  Test_Oracle(FP_Interval_Abstract_Store init) : int_store(init) {}

  bool get_interval(dimension_type dim, FP_Interval& result) const {
    result = int_store.get_interval(Variable(dim));
    return true;
  }

  bool get_fp_constant_value(const Floating_Point_Constant<C_Expr>& expr,
                             FP_Interval& result) const {
    result = FP_Interval((const char *)expr.value);
    return true;
  }

  bool get_integer_expr_value(const Concrete_Expression<C_Expr>& expr,
                              FP_Interval& result) const {
    if (expr.kind() == INT_CON) {
      const Integer_Constant<C_Expr>* ic_expr =
        reinterpret_cast< const Integer_Constant<C_Expr>* >(&expr);
      result = FP_Interval(ic_expr->value);
    }
    else {
      const Approximable_Reference<C_Expr>* ar_expr =
        reinterpret_cast< const Approximable_Reference<C_Expr>* >(&expr);
      result = FP_Interval(ar_expr->value);
    }

    return true;
  }

  bool get_associated_dimensions(
       const Approximable_Reference<C_Expr>& expr,
       std::set<dimension_type>& result) const {
    result = expr.dimensions;
    return true;
  }

  FP_Interval_Abstract_Store int_store;
};

using namespace Parma_Polyhedra_Library::IO_Operators;
Concrete_Expression_Type FP_Type
= Concrete_Expression_Type::floating_point(ANALYZED_FP_FORMAT);

typedef Integer_Interval Int_Interval;

/*
This file tests a rate limiter that, given random input flows of floating
point values X and D, bounded respectively by [-128, 128] and [0, 16],
computes an output flow Y that tries to follow X while having a change rate
limited by D. The pseudo-code of such rate limiter is the following:

Y = 0;
for (n = 0; n < N; ++n) {
  X = [-128, 128];
  D = [0, 16];
  S = Y;
  R = X - S;
  Y = X;
  if (R <= -D)
    Y = S - D;
  if (R >= D)
    Y = S + D;
}
*/

void
set_M(Coefficient& M, int m) {
  if (std::numeric_limits<Coefficient>::is_bounded) {
    if (greater_than(std::numeric_limits<Coefficient>::min(),
                     std::numeric_limits<ANALYZER_FP_FORMAT>::min())
        || less_than(std::numeric_limits<Coefficient>::max(),
                     std::numeric_limits<ANALYZER_FP_FORMAT>::max())) {
      // This may still provoke an arithmetic overflow exception:
      // no problem.
      M = m;
      return;
    }
  }
  // Cannot provoke an overflow.
  assign_r(M, m, ROUND_DOWN);
}

// Tests rate limiter using boxes and ignoring rounding errors.
bool
test01() {
  // Input signal.
  Variable X(0);
  // Maximum allowed for |R|.
  Variable D(1);
  // Output signal.
  Variable Y(2);
  // Last output.
  Variable S(3);
  // Actual rate.
  Variable R(4);

  FP_Interval_Abstract_Store abstract_store(5);
  FP_Interval_Abstract_Store as_begin(5);
  FP_Interval tmp(0);
  unsigned short n = 0;

  Constraint_System cs;
  Coefficient M;
  set_M(M, 144);
  cs.insert(Y <= M);
  cs.insert(Y >= -M);

  // Y = 0;
  abstract_store.affine_form_image(Y, FP_Linear_Form(tmp));

  do {

    nout << "*** n = " << n << " ***" << endl;
    as_begin = abstract_store;
    print_constraints(abstract_store, "*** begin loop ***");

    // X = [-128, 128];
    tmp.lower() = -128;
    tmp.upper() = 128;
    abstract_store.affine_form_image(X, FP_Linear_Form(tmp));

    // D = [0, 16];
    tmp.lower() = 0;
    tmp.upper() = 16;
    abstract_store.affine_form_image(D, FP_Linear_Form(tmp));

    // S = Y;
    abstract_store.affine_form_image(S, FP_Linear_Form(Y));

    // R = X - S;
    abstract_store.affine_form_image(R, FP_Linear_Form(X - S));

    // Y = X;
    abstract_store.affine_form_image(Y, FP_Linear_Form(X));

    // if (R <= -D) Y = S - D;
    FP_Interval_Abstract_Store as_then(abstract_store);
    as_then.refine_with_constraint(R <= -D);
    as_then.affine_form_image(Y, FP_Linear_Form(S - D));

    abstract_store.refine_with_constraint(R > -D);
    abstract_store.upper_bound_assign(as_then);
    print_constraints(abstract_store,
      "*** after if (R <= -D) Y = S - D; ***");

    // if (R >= D)  Y = S + D;
    as_then = abstract_store;
    as_then.refine_with_constraint(R >= D);
    as_then.affine_form_image(Y, FP_Linear_Form(S + D));

    abstract_store.refine_with_constraint(R < D);
    abstract_store.upper_bound_assign(as_then);
    print_constraints(abstract_store,
      "*** after if (R >= D)  Y = S + D; ***");

    abstract_store.upper_bound_assign(as_begin);
    abstract_store.limited_CC76_extrapolation_assign(as_begin, cs);
    print_constraints(abstract_store, "*** end loop ***");
    ++n;

  } while (as_begin != abstract_store);

  FP_Interval y = abstract_store.get_interval(Y);
  nout << "*** Y in " << y << " ***" << endl;
  return !y.is_bounded();
}

// Tests rate limiter using bounded differences and ignoring rounding
// errors.
bool
test02() {
  // Input signal.
  Variable X(0);
  // Maximum allowed for |R|.
  Variable D(1);
  // Output signal.
  Variable Y(2);
  // Last output.
  Variable S(3);
  // Actual rate.
  Variable R(4);

  FP_BD_Shape abstract_store(5);
  FP_BD_Shape as_begin(5);
  FP_Interval tmp(0);
  unsigned short n = 0;

  Constraint_System cs;
  Coefficient M;
  set_M(M, 144);
  cs.insert(Y <= M);
  cs.insert(Y >= -M);

  // Y = 0;
  abstract_store.affine_form_image(Y, FP_Linear_Form(tmp));

  do {

    nout << "*** n = " << n << " ***" << endl;
    as_begin = abstract_store;
    print_constraints(abstract_store, "*** begin loop ***");

    // X = [-128, 128];
    tmp.lower() = -128;
    tmp.upper() = 128;
    abstract_store.affine_form_image(X, FP_Linear_Form(tmp));

    // D = [0, 16];
    tmp.lower() = 0;
    tmp.upper() = 16;
    abstract_store.affine_form_image(D, FP_Linear_Form(tmp));

    // S = Y;
    abstract_store.affine_form_image(S, FP_Linear_Form(Y));

    // R = X - S;
    abstract_store.affine_form_image(R, FP_Linear_Form(X - S));

    // Y = X;
    abstract_store.affine_form_image(Y, FP_Linear_Form(X));

    // if (R <= -D) Y = S - D;
    FP_BD_Shape as_then(abstract_store);
    as_then.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                              -FP_Linear_Form(D));
    as_then.affine_form_image(Y, FP_Linear_Form(S - D));

    abstract_store.refine_with_linear_form_inequality(-FP_Linear_Form(D),
                                                       FP_Linear_Form(R));
    abstract_store.upper_bound_assign(as_then);
    print_constraints(abstract_store,
                      "*** after if (R <= -D) Y = S - D; ***");

    // if (R >= D)  Y = S + D;
    as_then = abstract_store;
    as_then.refine_with_linear_form_inequality(FP_Linear_Form(D),
                                               FP_Linear_Form(R));
    as_then.affine_form_image(Y, FP_Linear_Form(S + D));

    abstract_store.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                                      FP_Linear_Form(D));
    abstract_store.upper_bound_assign(as_then);
    print_constraints(abstract_store,
                      "*** after if (R >= D)  Y = S + D; ***");

    abstract_store.upper_bound_assign(as_begin);
    abstract_store.limited_BHMZ05_extrapolation_assign(as_begin, cs);
    print_constraints(abstract_store, "*** end loop ***");
    n++;

  } while (as_begin != abstract_store);

  tmp = (FP_Interval_Abstract_Store(abstract_store)).get_interval(Y);
  nout << "*** Y in " << tmp << " ***" << endl;
  return !tmp.is_bounded();
}

// Tests rate limiter using octagonal shapes and ignoring rounding
// errors.
bool
test03() {
  // Input signal.
  Variable X(0);
  // Maximum allowed for |R|.
  Variable D(1);
  // Output signal.
  Variable Y(2);
  // Last output.
  Variable S(3);
  // Actual rate.
  Variable R(4);

  //FP_Interval_Abstract_Store abstract_store();
  FP_Octagonal_Shape abstract_store(5, UNIVERSE);
  FP_Octagonal_Shape as_begin(5, EMPTY);
  FP_Interval tmp(0);
  unsigned short n = 0;

  Constraint_System cs;
  Coefficient M;
  set_M(M, 136);
  cs.insert(Y <= M);
  cs.insert(Y >= -M);

  // Y = 0;
  abstract_store.affine_form_image(Y, FP_Linear_Form(tmp));

  do {

    nout << "*** n = " << n << " ***" << endl;
    as_begin = abstract_store;
    print_constraints(abstract_store, "*** begin loop ***");

    //X = [-128, 128];
    tmp.lower() = -128;
    tmp.upper() = 128;
    abstract_store.affine_form_image(X, FP_Linear_Form(tmp));

    // D = [0, 16];
    tmp.lower() = 0;
    tmp.upper() = 16;
    abstract_store.affine_form_image(D, FP_Linear_Form(tmp));

    // S = Y;
    abstract_store.affine_form_image(S, FP_Linear_Form(Y));

    // R = X - S;
    abstract_store.affine_form_image(R, FP_Linear_Form(X - S));

    // Y = X;
    abstract_store.affine_form_image(Y, FP_Linear_Form(X));

    //if (R <= -D) Y = S - D;
    FP_Octagonal_Shape as_then(abstract_store);
    as_then.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                              -FP_Linear_Form(D));
    as_then.affine_form_image(Y, FP_Linear_Form(S - D));

    abstract_store.refine_with_linear_form_inequality(-FP_Linear_Form(D),
                                                      FP_Linear_Form(R));
    abstract_store.upper_bound_assign(as_then);
    print_constraints(abstract_store,
         "*** after if (R <= -D) Y = S - D; ***");

    //if (R >= D)  Y = S + D;
    as_then = abstract_store;
    as_then.refine_with_linear_form_inequality(FP_Linear_Form(D),
                                               FP_Linear_Form(R));
    as_then.affine_form_image(Y, FP_Linear_Form(S + D));

    abstract_store.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                          FP_Linear_Form(D));

    abstract_store.upper_bound_assign(as_then);
    print_constraints(abstract_store,
         "*** after (R >= D)  Y = S + D; ***");

    abstract_store.upper_bound_assign(as_begin);
    abstract_store.limited_BHMZ05_extrapolation_assign(as_begin, cs);
    print_constraints(abstract_store, "*** end loop ***");
    ++n;

  } while (as_begin != abstract_store);

  tmp = (FP_Interval_Abstract_Store(abstract_store)).get_interval(Y);
  nout << "*** Y in " << tmp << " ***" << endl;
  return (tmp.lower() == -136 && tmp.upper() == 136);
}

// Tests rate limiter using polyhedra and ignoring rounding errors.
bool
test04() {
  // Input signal.
  Variable X(0);
  // Maximum allowed for |R|.
  Variable D(1);
  // Output signal.
  Variable Y(2);
  // Last output.
  Variable S(3);
  // Actual rate.
  Variable R(4);

  NNC_Polyhedron abstract_store(5);
  NNC_Polyhedron as_begin(5);
  FP_Interval tmp(0);
  unsigned short n = 0;

  Constraint_System cs;
  Coefficient M;
  set_M(M, 128);
  cs.insert(Y <= M);
  cs.insert(Y >= -M);

  // Y = 0;
  abstract_store.affine_form_image(Y, FP_Linear_Form(tmp));

  do {

    nout << "*** n = " << n << " ***" << endl;
    as_begin = abstract_store;
    print_constraints(abstract_store, "*** begin loop ***");

    // X = [-128, 128];
    tmp.lower() = -128;
    tmp.upper() = 128;
    abstract_store.affine_form_image(X, FP_Linear_Form(tmp));

    // D = [0, 16];
    tmp.lower() = 0;
    tmp.upper() = 16;
    abstract_store.affine_form_image(D, FP_Linear_Form(tmp));

    // S = Y;
    abstract_store.affine_form_image(S, FP_Linear_Form(Y));

    // R = X - S;
    abstract_store.affine_form_image(R, FP_Linear_Form(X - S));

    // Y = X;
    abstract_store.affine_form_image(Y, FP_Linear_Form(X));

    // if (R <= -D) Y = S - D;
    NNC_Polyhedron as_then(abstract_store);
    as_then.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                              -FP_Linear_Form(D));
    as_then.affine_form_image(Y, FP_Linear_Form(S - D));

    abstract_store.generalized_refine_with_linear_form_inequality(
      -FP_Linear_Form(D), FP_Linear_Form(R), LESS_THAN);
    abstract_store.upper_bound_assign(as_then);
    print_constraints(abstract_store,
         "*** after if (R <= -D) Y = S - D; ***");

    // if (R >= D)  Y = S + D;
    as_then = abstract_store;
    as_then.refine_with_linear_form_inequality(FP_Linear_Form(D),
                                               FP_Linear_Form(R));
    as_then.affine_form_image(Y, FP_Linear_Form(S + D));

    abstract_store.generalized_refine_with_linear_form_inequality(
      FP_Linear_Form(R), FP_Linear_Form(D), LESS_THAN);
    abstract_store.upper_bound_assign(as_then);
    print_constraints(abstract_store,
         "*** after if (R >= D)  Y = S + D; ***");

    abstract_store.upper_bound_assign(as_begin);
    abstract_store.limited_BHRZ03_extrapolation_assign(as_begin, cs);
    print_constraints(abstract_store, "*** end loop ***");
    ++n;

  } while(as_begin != abstract_store);

  tmp = (FP_Interval_Abstract_Store(abstract_store)).get_interval(Y);
  nout << "*** Y in " << tmp << " ***" << endl;
  return (tmp.lower() == -128 && tmp.upper() == 128);
}


// Tests rate limiter using bounded differences and linearization of
// floating point expressions.
// In order to improve the analysis, the interval domain is used
// in parallel with bounded differences domain.
bool
test05() {
  // Input signal.
  Variable X(0);
  // Maximum allowed for |R|.
  Variable D(1);
  // Output signal.
  Variable Y(2);
  // Last output.
  Variable S(3);
  // Actual rate.
  Variable R(4);

  Test_Oracle oracle(FP_Interval_Abstract_Store(5));
  Test_Oracle or_begin(FP_Interval_Abstract_Store (5));
  FP_Linear_Form_Abstract_Store lf_abstract_store;
  FP_BD_Shape bd(5);
  FP_BD_Shape bd_begin(5);
  unsigned short n = 0;
  FP_Interval tmp(0);
  FP_Linear_Form lx;
  FP_Linear_Form ly;
  FP_Linear_Form lr;
  FP_Linear_Form lk;

  Constraint_System cs;
  Coefficient M;
  set_M(M, 144);
  cs.insert(Y <= M);
  cs.insert(Y >= -M);

  bool lin_success;

  Floating_Point_Constant<C_Expr> con_y("0", 2);
  // The constant floating point expression con_y is linearized into
  // the interval linear form lk. If linearization succeeded, we model
  // the assignment Y = 0, invoking affine_form_image method.
  // In order to refine the analysis, all the transer function are
  // performed in parallel in the interval domain and in the bounded
  // differences domain.
  // Then, we consider the intersection between these abstract domains.

  lin_success = linearize(con_y, oracle, lf_abstract_store, lk);
  oracle.int_store.affine_form_image(Y, FP_Linear_Form(tmp));
  if (lin_success) {
    bd.affine_form_image(Y, lk);
  }
  else {
    bd.intersection_assign(FP_BD_Shape(oracle.int_store));
  }
  oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(bd));

  // This loop iterate until a fixed point is reached.
  do {

    // Iteration no. n+1.
    nout << "*** n = " << n << " ***" << endl;
    bd_begin = bd;
    or_begin.int_store = oracle.int_store;
    print_constraints(oracle.int_store, "*** before loop ***");

    // X = [-128, 128];
    tmp.lower() = -128;
    tmp.upper() = 128;
    oracle.int_store.affine_form_image(X, FP_Linear_Form(tmp));
    bd.affine_form_image(X, FP_Linear_Form(tmp));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(bd));

    // D = [0, 16];
    tmp.lower() = 0;
    tmp.upper() = 16;
    oracle.int_store.affine_form_image(D, FP_Linear_Form(tmp));
    bd.affine_form_image(D, FP_Linear_Form(tmp));    
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(bd));

    // S = Y;
    Approximable_Reference<C_Expr> var_y(FP_Type, Int_Interval(mpz_class(0)),
                                         Y.id());
    lin_success = linearize(var_y, oracle, lf_abstract_store, ly);
    oracle.int_store.affine_form_image(S, FP_Linear_Form(Y));
    if (lin_success) {
      bd.affine_form_image(S, ly);
    }
    else {
      bd.intersection_assign(FP_BD_Shape(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(bd));

    // R = X - S;
    Approximable_Reference<C_Expr> px(FP_Type, Int_Interval(mpz_class(0)),
                                      X.id());
    Approximable_Reference<C_Expr> ps(FP_Type, Int_Interval(mpz_class(0)),
                                      S.id());
    Binary_Operator<C_Expr> x_dif_s(FP_Type, Binary_Operator<C_Expr>::SUB,
                                    &px, &ps);
    lin_success = linearize(x_dif_s, oracle, lf_abstract_store, lr);
    oracle.int_store.affine_form_image(R, FP_Linear_Form(X - S));
    if (lin_success) {
      bd.affine_form_image(R, lr);
    }
    else {
      bd.intersection_assign(FP_BD_Shape(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(bd));

    // Y = X;
    lin_success = linearize(px, oracle, lf_abstract_store, lx);
    oracle.int_store.affine_form_image(Y, FP_Linear_Form(X));
    if (lin_success) {
      bd.affine_form_image(Y, lx);
    }
    else {
      bd.intersection_assign(FP_BD_Shape(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(bd));

    // if (R <= -D)
    FP_BD_Shape bd_then(bd);
    Test_Oracle or_then(oracle.int_store);
    or_then.int_store.refine_with_constraint(R <= -D);
    bd_then.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                               -FP_Linear_Form(D));
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(bd_then));

    // then Y = S - D;
    Approximable_Reference<C_Expr> pd(FP_Type, Int_Interval(mpz_class(0)),
                                      D.id());
    Binary_Operator<C_Expr> s_dif_d(FP_Type, Binary_Operator<C_Expr>::SUB,
                                    &ps, &pd);
    lin_success = linearize(s_dif_d, or_then, lf_abstract_store, ly);
    or_then.int_store.affine_form_image(Y, FP_Linear_Form(S - D));
    if (lin_success) {
      bd_then.affine_form_image(Y, ly);
    }
    else {
      bd_then.intersection_assign(FP_BD_Shape(or_then.int_store));
    }
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(bd_then));

    // else skip;
    oracle.int_store.refine_with_constraint(R > -D);
    bd.refine_with_linear_form_inequality(-FP_Linear_Form(D),
                                          FP_Linear_Form(R));
    bd.intersection_assign(FP_BD_Shape(oracle.int_store));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(bd));

    // LUB between then and else branches.
    bd.upper_bound_assign(bd_then);
    oracle.int_store.upper_bound_assign(or_then.int_store);
    print_constraints(oracle.int_store,
                      "*** after if (R <= -D) Y = S - D; ***");

    // if (R >= D)
    bd_then = bd;
    or_then.int_store = oracle.int_store;
    or_then.int_store.refine_with_constraint(R >= D);
    bd_then.refine_with_linear_form_inequality(FP_Linear_Form(D),
                                               FP_Linear_Form(R));
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(bd_then));

    // then Y = S + D;
    Binary_Operator<C_Expr> s_sum_d(FP_Type, Binary_Operator<C_Expr>::ADD,
                                    &ps, &pd);
    lin_success = linearize(s_sum_d, or_then, lf_abstract_store, ly);
    or_then.int_store.affine_form_image(Y, FP_Linear_Form(S + D));
    if (lin_success) {
      bd_then.affine_form_image(Y, ly);
    }
    else {
      bd_then.intersection_assign(FP_BD_Shape(or_then.int_store));
    }
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(bd_then));

    // else skip;
    bd.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                          FP_Linear_Form(D));
    oracle.int_store.refine_with_constraint(R < D);
    bd.intersection_assign(FP_BD_Shape(oracle.int_store));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(bd));

    // LUB between then and else branches.
    bd.upper_bound_assign(bd_then);
    oracle.int_store.upper_bound_assign(or_then.int_store);
    print_constraints(oracle.int_store,
                      "*** after if (R >= D)  Y = S + D; ***");

    // LUB between the actual abstract domains and the corresponding
    // domains at the beginning of the loop.
    bd.upper_bound_assign(bd_begin);
    oracle.int_store.upper_bound_assign(or_begin.int_store);

    // Limited extrapolation: we enforce the satisfaction
    // of the constraint system cs = {Y <= M; Y >= -M}
    bd.limited_BHMZ05_extrapolation_assign(bd_begin, cs);
    oracle.int_store.limited_CC76_extrapolation_assign(or_begin.int_store, cs);
    print_constraints(oracle.int_store, "*** end loop ***");
    ++n;

  } while(or_begin.int_store != oracle.int_store);

  tmp = oracle.int_store.get_interval(Y);
  nout << "*** Y in " << tmp << " ***" << endl;
  return (tmp.lower() == -144 && tmp.upper() == 144);
}

// Tests rate limiter using octagonal shapes and linearization of
// floating point expressions.
// In order to improve the analysis, the interval domain is used
// in parallel with octagons domain.
bool
test06() {
  // Input signal.
  Variable X(0);
  // Maximum allowed for |R|.
  Variable D(1);
  // Output signal.
  Variable Y(2);
  // Last output.
  Variable S(3);
  // Actual rate.
  Variable R(4);

  Test_Oracle oracle(FP_Interval_Abstract_Store(5));
  Test_Oracle or_begin(FP_Interval_Abstract_Store(5));
  FP_Linear_Form_Abstract_Store lf_abstract_store;
  FP_Octagonal_Shape oc(5);
  FP_Octagonal_Shape oc_begin(5);
  unsigned short n = 0;
  FP_Interval tmp(0);
  FP_Linear_Form lx;
  FP_Linear_Form ly;
  FP_Linear_Form lr;
  FP_Linear_Form lk;

  Constraint_System cs;
  Coefficient M;
  set_M(M, 144);
  cs.insert(Y <= M);
  cs.insert(Y >= -M);

  bool lin_success;

  Floating_Point_Constant<C_Expr> con_y("0", 2);
  // The constant floating point expression con_y is linearized into
  // the interval linear form lk. If linearization succeeded, we model
  // the assignment Y = 0, invoking affine_form_image method.
  // In order to refine the analysis, all the transer function are
  // performed in parallel in the interval domain and in the bounded
  // differences domain.
  // Then, we consider the intersection between these abstract domains.

  lin_success = linearize(con_y, oracle, lf_abstract_store, lk);
  oracle.int_store.affine_form_image(Y, FP_Linear_Form(tmp));
  if (lin_success) {
    oc.affine_form_image(Y, lk);
  }
  else {
    oc.intersection_assign(FP_Octagonal_Shape(oracle.int_store));
  }
  oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(oc));

  // This loop iterates until a fixed point is reached.
  do {

    // Iteration no. n+1.
    nout << "*** n = " << n << " ***" << endl;
    oc_begin = oc;
    or_begin.int_store = oracle.int_store;
    print_constraints(oracle.int_store, "*** before loop ***");

    // X = [-128, 128];
    tmp.lower() = -128;
    tmp.upper() = 128;
    oracle.int_store.affine_form_image(X, FP_Linear_Form(tmp));
    oc.affine_form_image(X, FP_Linear_Form(tmp));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(oc));

    // D = [0, 16];
    tmp.lower() = 0;
    tmp.upper() = 16;
    oracle.int_store.affine_form_image(D, FP_Linear_Form(tmp));
    oc.affine_form_image(D, FP_Linear_Form(tmp));    
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(oc));

    // S = Y;
    Approximable_Reference<C_Expr> var_y(FP_Type, Int_Interval(mpz_class(0)),
                                         Y.id());
    lin_success = linearize(var_y, oracle, lf_abstract_store, ly);
    oracle.int_store.affine_form_image(S, FP_Linear_Form(Y));
    if (lin_success) {
      oc.affine_form_image(S, ly);
    }
    else {
      oc.intersection_assign(FP_Octagonal_Shape(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(oc));

    // R = X - S;
    Approximable_Reference<C_Expr> px(FP_Type, Int_Interval(mpz_class(0)),
                                      X.id());
    Approximable_Reference<C_Expr> ps(FP_Type, Int_Interval(mpz_class(0)),
                                      S.id());
    Binary_Operator<C_Expr> x_dif_s(FP_Type, Binary_Operator<C_Expr>::SUB,
                                    &px, &ps);
    lin_success = linearize(x_dif_s, oracle, lf_abstract_store, lr);
    oracle.int_store.affine_form_image(R, FP_Linear_Form(X - S));
    if (lin_success) {
      oc.affine_form_image(R, lr);
    }
    else {
      oc.intersection_assign(FP_Octagonal_Shape(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(oc));

    // Y = X;
    lin_success = linearize(px, oracle, lf_abstract_store, lx);
    oracle.int_store.affine_form_image(Y, FP_Linear_Form(X));
    if (lin_success) {
      oc.affine_form_image(Y, lx);
    }
    else {
      oc.intersection_assign(FP_Octagonal_Shape(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(oc));

    // if (R <= -D)
    FP_Octagonal_Shape oc_then(oc);
    Test_Oracle or_then(oracle.int_store);
    or_then.int_store.refine_with_constraint(R <= -D);
    oc_then.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                               -FP_Linear_Form(D));
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(oc_then));

    // then Y = S - D;
    Approximable_Reference<C_Expr> pd(FP_Type, Int_Interval(mpz_class(0)),
                                      D.id());
    Binary_Operator<C_Expr> s_dif_d(FP_Type, Binary_Operator<C_Expr>::SUB,
                                    &ps, &pd);
    lin_success = linearize(s_dif_d, or_then, lf_abstract_store, ly);
    or_then.int_store.affine_form_image(Y, FP_Linear_Form(S - D));
    if (lin_success) {
      oc_then.affine_form_image(Y, ly);
    }
    else {
      oc_then.intersection_assign(FP_Octagonal_Shape(or_then.int_store));
    }
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(oc_then));

    // else skip;
    oracle.int_store.refine_with_constraint(R > -D);
    oc.refine_with_linear_form_inequality(-FP_Linear_Form(D),
                                          FP_Linear_Form(R));
    oc.intersection_assign(FP_Octagonal_Shape(oracle.int_store));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(oc));

    // LUB between then and else branches.
    oc.upper_bound_assign(oc_then);
    oracle.int_store.upper_bound_assign(or_then.int_store);
    print_constraints(oracle.int_store,
                      "*** after if (R <= -D) Y = S - D; ***");

    // if (R >= D)
    oc_then = oc;
    or_then.int_store = oracle.int_store;
    or_then.int_store.refine_with_constraint(R >= D);
    oc_then.refine_with_linear_form_inequality(FP_Linear_Form(D),
                                               FP_Linear_Form(R));
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(oc_then));

    // then Y = S + D;
    Binary_Operator<C_Expr> s_sum_d(FP_Type, Binary_Operator<C_Expr>::ADD,
                                    &ps, &pd);
    lin_success = linearize(s_sum_d, or_then, lf_abstract_store, ly);
    or_then.int_store.affine_form_image(Y, FP_Linear_Form(S + D));
    if (lin_success) {
      oc_then.affine_form_image(Y, ly);
    }
    else {
      oc_then.intersection_assign(FP_Octagonal_Shape(or_then.int_store));
    }
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(oc_then));

    // else skip;
    oc.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                          FP_Linear_Form(D));
    oracle.int_store.refine_with_constraint(R < D);
    oc.intersection_assign(FP_Octagonal_Shape(oracle.int_store));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(oc));

    // LUB between then and else branches.
    oc.upper_bound_assign(oc_then);
    oracle.int_store.upper_bound_assign(or_then.int_store);
    print_constraints(oracle.int_store,
                      "*** after if (R >= D)  Y = S + D; ***");

    // LUB between the actual abstract domains and the corresponding
    // domains at the beginning of the loop.
    oc.upper_bound_assign(oc_begin);
    oracle.int_store.upper_bound_assign(or_begin.int_store);

    // Limited extrapolation: we enforce the satisfaction
    // of the constraint system cs = {Y <= M; Y >= -M}
    oc.limited_BHMZ05_extrapolation_assign(oc_begin, cs);
    oracle.int_store.limited_CC76_extrapolation_assign(or_begin.int_store, cs);
    print_constraints(oracle.int_store, "*** end loop ***");
    ++n;

  } while(or_begin.int_store != oracle.int_store);

  tmp = oracle.int_store.get_interval(Y);
  nout << "*** Y in " << tmp << " ***" << endl;
  return (tmp.lower() == -144 && tmp.upper() == 144);
}

// Tests rate limiter using polyhedra domain and linearization of
// floating point expressions.
// In order to improve the analysis, the interval domain is used
// in parallel with poyhedra domain.
bool
test07() {
  // Input signal.
  Variable X(0);
  // Maximum allowed for |R|.
  Variable D(1);
  // Output signal.
  Variable Y(2);
  // Last output.
  Variable S(3);
  // Actual rate.
  Variable R(4);

  Test_Oracle oracle(FP_Interval_Abstract_Store(5));
  Test_Oracle or_begin(FP_Interval_Abstract_Store(5));
  FP_Linear_Form_Abstract_Store lf_abstract_store;
  NNC_Polyhedron ph(5);
  NNC_Polyhedron ph_begin(5);
  unsigned short n = 0;
  FP_Interval tmp(0);
  FP_Linear_Form lx;
  FP_Linear_Form ly;
  FP_Linear_Form lr;
  FP_Linear_Form lk;

  Constraint_System cs;
  Coefficient M;
  set_M(M, 144);
  cs.insert(Y <= M);
  cs.insert(Y >= -M);

  bool lin_success;

  Floating_Point_Constant<C_Expr> con_y("0", 2);
  // The constant floating point expression con_y is linearized into
  // the interval linear form lk. If linearization succeeded, we model
  // the assignment Y = 0, invoking affine_form_image method.
  // In order to refine the analysis, all the transer function are
  // performed in parallel in the interval domain and in the bounded
  // differences domain.
  // Then, we consider the intersection between these abstract domains.

  lin_success = linearize(con_y, oracle, lf_abstract_store, lk);
  oracle.int_store.affine_form_image(Y, FP_Linear_Form(tmp));
  if (lin_success) {
    ph.affine_form_image(Y, lk);
  }
  else {
    ph.intersection_assign(NNC_Polyhedron(oracle.int_store));
  }
  oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(ph));

  // This loop iterate until a fixed point is reached.
  do {

    // Iteration no. n+1.
    nout << "*** n = " << n << " ***" << endl;
    ph_begin = ph;
    or_begin.int_store = oracle.int_store;
    print_constraints(oracle.int_store, "*** before loop ***");

    // X = [-128, 128];
    tmp.lower() = -128;
    tmp.upper() = 128;
    oracle.int_store.affine_form_image(X, FP_Linear_Form(tmp));
    ph.affine_form_image(X, FP_Linear_Form(tmp));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(ph));

    // D = [0, 16];
    tmp.lower() = 0;
    tmp.upper() = 16;
    oracle.int_store.affine_form_image(D, FP_Linear_Form(tmp));
    ph.affine_form_image(D, FP_Linear_Form(tmp));    
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(ph));

    // S = Y;
    Approximable_Reference<C_Expr> var_y(FP_Type, Int_Interval(mpz_class(0)),
                                         Y.id());
    lin_success = linearize(var_y, oracle, lf_abstract_store, ly);
    oracle.int_store.affine_form_image(S, FP_Linear_Form(Y));
    if (lin_success) {
      ph.affine_form_image(S, ly);
    }
    else {
      ph.intersection_assign(NNC_Polyhedron(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(ph));

    // R = X - S;
    Approximable_Reference<C_Expr> px(FP_Type, Int_Interval(mpz_class(0)),
                                      X.id());
    Approximable_Reference<C_Expr> ps(FP_Type, Int_Interval(mpz_class(0)),
                                      S.id());
    Binary_Operator<C_Expr> x_dif_s(FP_Type, Binary_Operator<C_Expr>::SUB,
                                    &px, &ps);
    lin_success = linearize(x_dif_s, oracle, lf_abstract_store, lr);
    oracle.int_store.affine_form_image(R, FP_Linear_Form(X - S));
    if (lin_success) {
      ph.affine_form_image(R, lr);
    }
    else {
      ph.intersection_assign(NNC_Polyhedron(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(ph));

    // Y = X;
    lin_success = linearize(px, oracle, lf_abstract_store, lx);
    oracle.int_store.affine_form_image(Y, FP_Linear_Form(X));
    if (lin_success) {
      ph.affine_form_image(Y, lx);
    }
    else {
      ph.intersection_assign(NNC_Polyhedron(oracle.int_store));
    }
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(ph));

    // if (R <= -D)
    NNC_Polyhedron ph_then(ph);
    Test_Oracle or_then(oracle.int_store);
    or_then.int_store.refine_with_constraint(R <= -D);
    ph_then.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                               -FP_Linear_Form(D));
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(ph_then));

    // then Y = S - D;
    Approximable_Reference<C_Expr> pd(FP_Type, Int_Interval(mpz_class(0)),
                                      D.id());
    Binary_Operator<C_Expr> s_dif_d(FP_Type, Binary_Operator<C_Expr>::SUB,
                                    &ps, &pd);
    lin_success = linearize(s_dif_d, or_then, lf_abstract_store, ly);
    or_then.int_store.affine_form_image(Y, FP_Linear_Form(S - D));
    if (lin_success) {
      ph_then.affine_form_image(Y, ly);
    }
    else {
      ph_then.intersection_assign(NNC_Polyhedron(or_then.int_store));
    }
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(ph_then));

    // else skip;
    oracle.int_store.refine_with_constraint(R > -D);
    ph.refine_with_linear_form_inequality(-FP_Linear_Form(D),
                                          FP_Linear_Form(R));
    ph.intersection_assign(NNC_Polyhedron(oracle.int_store));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(ph));

    // LUB between then and else branches.
    ph.upper_bound_assign(ph_then);
    oracle.int_store.upper_bound_assign(or_then.int_store);
    print_constraints(oracle.int_store, "*** after if (R <= -D) Y = S - D; ***");

    // if (R >= D)
    ph_then = ph;
    or_then.int_store = oracle.int_store;
    or_then.int_store.refine_with_constraint(R >= D);
    ph_then.refine_with_linear_form_inequality(FP_Linear_Form(D),
                                               FP_Linear_Form(R));
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(ph_then));

    // then Y = S + D;
    Binary_Operator<C_Expr> s_sum_d(FP_Type, Binary_Operator<C_Expr>::ADD,
                                    &ps, &pd);
    lin_success = linearize(s_sum_d, or_then, lf_abstract_store, ly);
    or_then.int_store.affine_form_image(Y, FP_Linear_Form(S + D));
    if (lin_success) {
      ph_then.affine_form_image(Y, ly);
    }
    else {
      ph_then.intersection_assign(NNC_Polyhedron(or_then.int_store));
    }
    or_then.int_store.intersection_assign(FP_Interval_Abstract_Store(ph_then));

    // else skip;
    ph.refine_with_linear_form_inequality(FP_Linear_Form(R),
                                          FP_Linear_Form(D));
    oracle.int_store.refine_with_constraint(R < D);
    ph.intersection_assign(NNC_Polyhedron(oracle.int_store));
    oracle.int_store.intersection_assign(FP_Interval_Abstract_Store(ph));

    // LUB between then and else branches.
    ph.upper_bound_assign(ph_then);
    oracle.int_store.upper_bound_assign(or_then.int_store);
    print_constraints(oracle.int_store,
                      "*** after if (R >= D)  Y = S + D; ***");

    // LUB between the actual abstract domains and the corresponding
    // domains at the beginning of the loop.
    ph.upper_bound_assign(ph_begin);
    oracle.int_store.upper_bound_assign(or_begin.int_store);

    // Limited extrapolation: we enforce the satisfaction
    // of the constraint system cs = {Y <= M; Y >= -M}
    ph.limited_BHRZ03_extrapolation_assign(ph_begin, cs);
    oracle.int_store.limited_CC76_extrapolation_assign(or_begin.int_store, cs);
    print_constraints(oracle.int_store, "*** end loop ***");
    ++n;

  } while(or_begin.int_store != oracle.int_store);

  tmp = oracle.int_store.get_interval(Y);
  nout << "*** Y in " << tmp << " ***" << endl;
  return (tmp.lower() == -144 && tmp.upper() == 144);
}

} // namespace

BEGIN_MAIN
  DO_TEST_F8(test01);
  DO_TEST_F8(test02);
  DO_TEST_F8(test03);
  DO_TEST_F64A(test04);

#define COND_float  PPL_CPP_EQ(PPL_CPP_FP_FORMAT(ANALYZER_FP_FORMAT), 1)
#define COND_double PPL_CPP_EQ(PPL_CPP_FP_FORMAT(ANALYZER_FP_FORMAT), 2)
#define COND_float_or_double PPL_CPP_OR(COND_float, COND_double)
#define PPL_CUSTOM_COND_32 \
  PPL_CPP_AND(PPL_CPP_EQ(PPL_CPP_LOGBITS, 5), COND_float)
#define PPL_CUSTOM_COND_64 \
  PPL_CPP_AND(PPL_CPP_EQ(PPL_CPP_LOGBITS, 6), COND_float_or_double)
#define PPL_CUSTOM_COND \
  PPL_CPP_OR(COND_F64, PPL_CPP_OR(PPL_CUSTOM_COND_32, PPL_CUSTOM_COND_64))

  COND_DO_TEST(PPL_CUSTOM_COND, test05);
  COND_DO_TEST(PPL_CUSTOM_COND, test06);

  DO_TEST_F64(test07);

END_MAIN
