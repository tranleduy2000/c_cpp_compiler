/* Test the MIP_Problem class.
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

  // Feasible region.
  Constraint_System cs;
  cs.insert(-2*A - B >= -5);
  cs.insert(4*A -4*B >= -5);
  cs.insert(A >= 0);
  cs.insert(B >= 0);

  // All integer variables.
  Variables_Set ivs(A, B);

  // Objective function.
  Linear_Expression cost(A - 2*B);

  MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(), ivs, cost,
		  MAXIMIZATION);
  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  mip.set_optimization_mode(MINIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  return true;
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Constraint_System cs;
  cs.insert(Coefficient("-3152519739159347")*A
	    - Coefficient("4503599627370496")*B
	    >= Coefficient("-2837267765243412480"));
  cs.insert(Coefficient("-14411518807585588")*A
	    - Coefficient("3602879701896397")*B
	    >= Coefficient("-19455550390240542720"));
  cs.insert(Coefficient("6325070415853456823515479584966165845298645305129441198653167438357198111499854590373761990669910140474596183259900372230931523043306046152094168748148078435047419508642698792639590866940413010663742739952273283392562733857021646831815729864036236135650314266011211548510419206725953204130822734645187695728365866909171712")*A
	    >= Coefficient("134217729"));
  cs.insert(B >= 20);
  cs.insert(-B >= -500);

  // Integer variables.
  Variables_Set ivs(A);

  // Cost function
  Linear_Expression cost(-4*A - B);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(),
				cs.begin(), cs.end(),
				ivs,
				cost,
				MINIMIZATION);

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);

  Coefficient num;
  Coefficient den;
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;

  return true;
}

bool
test03() {
  MIP_Problem mip = MIP_Problem();

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);
  Generator pg_kr = point();
  if (pg != pg_kr)
    return false;
  return true;
}

bool
test04() {
  MIP_Problem mip = MIP_Problem();
  mip.add_constraint(Linear_Expression::zero() <= 1);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  Generator pg_kr = point();
  if (pg != pg_kr)
    return false;
  mip.add_constraint(Linear_Expression::zero() >= 1);
  return !mip.is_satisfiable();
}

bool
test05() {
  Variable A(0);
  Variable B(1);

  // Feasible region.
  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(A <= 3);
  cs.insert(B >= 0);
  cs.insert(-A - B >= -8);
  cs.insert(-A - 3*B >= -18);
  cs.insert(-A + B >= -4);

  // All integer variables.
  Variables_Set ivs(A, B);

  // Cost function.
  Linear_Expression cost(A + B);

  MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(), ivs, cost,
		  MAXIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  return pg == point(3*A + 5*B);
}

bool
test06() {
  Variable A(0);
  Variable B(1);

  // Feasible region.
  Constraint_System cs;
  cs.insert(-A - 3*B >= -4);
  cs.insert(-5*A - B >= -5);
  cs.insert(-3*A - 2*B >= -2);
  cs.insert(A + 3*B >= -1);
  cs.insert(2*A - B >= -2);

  // Cost function.
  Linear_Expression cost(A + B);

  MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(), cost,
		  MAXIMIZATION);
  // Disallow floating point based steepest-edge pricing, so that
  // predictable overflow behavior is obtained when configured
  // to use checked 8-bit integers as coefficients.
  mip.set_control_parameter(MIP_Problem::PRICING_STEEPEST_EDGE_EXACT);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  return pg == point(-2*A + 10*B, 7);
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  // Feasible region.
  Constraint_System cs;
  cs.insert(A + B >= 0);
  cs.insert(B >= 0);
  cs.insert(B == 3);
  cs.insert(2*C + 2*D == 9);

  // Cost function.
  Linear_Expression cost(10*A + 6*B);

  MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(), cost,
		  MINIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  Coefficient num;
  Coefficient den;
  mip.optimal_value(num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  Coefficient num_kr = -12;
  Coefficient den_kr = 1;

  return num == num_kr && den == den_kr;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  // Feasible region.
  Constraint_System cs;
  cs.insert(Coefficient("2251799813685248")*A
	    >= -Coefficient("5895288448651847"));
  cs.insert(Coefficient("5895288437392848")*A
	    + Coefficient("3643488632714799")*B
	    - Coefficient("2251799813685248")*C
	    >= -Coefficient("19077554137963492"));
  cs.insert(Coefficient("5895288437392848")*A
	    + Coefficient("3643488632714799")*B
	    + Coefficient("2251799813685248")*C
	    >= -Coefficient("19077554137963492"));
  cs.insert(Coefficient("11790576874785696")*A
	    + Coefficient("4503599627370496")*B
	    + Coefficient("7286977274436797")*D
	    >= -Coefficient("38155108284934184"));
  cs.insert(Coefficient("11790576874785696")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("7286977274436797")*D
	    >= -Coefficient("38155108284934184"));
  cs.insert(Coefficient("11790576879289294")*A
	    + Coefficient("7286977274436797")*C
	    + Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    + Coefficient("7286977274436797")*C
	    - Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    - Coefficient("7286977274436797")*C
	    + Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    - Coefficient("7286977274436797")*C
	    - Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108289437784"));
  cs.insert(Coefficient("2947644225451823")*A
	    - Coefficient("1125899906842624")*B
	    + Coefficient("1821744319735099")*D
	    >= -Coefficient("9538777088122044"));
  cs.insert(Coefficient("11790576892800094")*A
	    - Coefficient("4503599627370496")*B
	    - Coefficient("7286977274436797")*D
	    >= -Coefficient("38155108325466584"));
  cs.insert(Coefficient("5895288437392848")*A
	    - Coefficient("3643488630462999")*B
	    + Coefficient("2251799813685248")*C
	    >= -Coefficient("19077554133459892"));
  cs.insert(Coefficient("2947644218696424")*A
	    - Coefficient("1821744320860999")*B
	    - Coefficient("1125899906842624")*C
	    >= -Coefficient("9538777072359446"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("11790576924325290")*B
	    + Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108379509776"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("11790576924325290")*B
	    - Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108379509776"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= -Coefficient("2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= -Coefficient("2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= -Coefficient("2947644225451823"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= -Coefficient("2947644225451823"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("4503599627370496")*B
	    + Coefficient("11790576865778496")*C
	    >= -Coefficient("38155108266919784"));
  cs.insert(Coefficient("7286977251918799")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("11790576870282096")*C
	    >= -Coefficient("38155108244401792"));
  cs.insert(Coefficient("1821744320860999")*A
	    + Coefficient("1125899906842624")*C
	    + Coefficient("2947644226577723")*D
	    >= -Coefficient("9538777093751544"));
  cs.insert(Coefficient("1821744320860999")*A
	    + Coefficient("1125899906842624")*C
	    - Coefficient("2947644226577723")*D
	    >= -Coefficient("9538777093751544"));
  cs.insert(Coefficient("1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    + Coefficient("2947644228829523")*D
	    >= -Coefficient("9538777096003344"));
  cs.insert(Coefficient("1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    - Coefficient("2947644228829523")*D
	    >= -Coefficient("9538777096003344"));
  cs.insert(Coefficient("3643488664239996")*A
	    - Coefficient("2251799813685248")*B
	    + Coefficient("5895288468918045")*C
	    >= -Coefficient("19077554257308884"));
  cs.insert(Coefficient("3643488652980997")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("5895288468918045")*C
	    >= -Coefficient("19077554232539084"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= -Coefficient("2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= -Coefficient("2947644229392473"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= -Coefficient("2947644227140673"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= -Coefficient("2947644227703623"));
  cs.insert(Coefficient("7286977314969193")*A
	    - Coefficient("11790576906310892")*B
	    + Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108447063768"));
  cs.insert(Coefficient("3643488655232797")*A
	    - Coefficient("5895288446400047")*B
	    - Coefficient("2251799813685248")*D
	    >= -Coefficient("19077554203265688"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576753188506")*B
	    + Coefficient("7286977179861205")*C
	    >= -Coefficient("38155107920142616"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576766699304")*B
	    - Coefficient("7286977179861205")*C
	    >= -Coefficient("38155107965178608"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("7286977157343207")*B
	    + Coefficient("11790576712656108")*D
	    >= -Coefficient("38155107816559824"));
  cs.insert(Coefficient("2251799813685248")*A
	    + Coefficient("3643488592182402")*B
	    - Coefficient("5895288374342453")*D
	    >= -Coefficient("19077553960071308"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576753188506")*C
	    + Coefficient("7286977175357605")*D
	    >= -Coefficient("38155107924646216"));
  cs.insert(Coefficient("2251799813685248")*A
	    + Coefficient("5895288390105051")*C
	    - Coefficient("3643488594434202")*D
	    >= -Coefficient("19077553996100104"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("5895288421630249")*C
	    + Coefficient("3643488619204000")*D
	    >= -Coefficient("19077554088423896"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("11790576865778496")*C
	    - Coefficient("7286977247415199")*D
	    >= -Coefficient("38155108244401792"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("7286977247415199")*B
	    + Coefficient("11790576888296494")*D
	    >= -Coefficient("38155108307452184"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("3643488639470198")*B
	    - Coefficient("5895288464414445")*D
	    >= -Coefficient("19077554210021088"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("5895288428385648")*B
	    + Coefficient("3643488630462999")*C
	    >= -Coefficient("19077554131208092"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("11790576843260498")*B
	    - Coefficient("7286977224897201")*C
	    >= -Coefficient("38155108163336992"));
  cs.insert(Coefficient("1125899906842624")*B
	    >= -Coefficient("2947644227703623"));
  cs.insert(Coefficient("5895288459910846")*B
	    + Coefficient("2251799813685248")*C
	    + Coefficient("3643488630462999")*D
	    >= -Coefficient("19077554198762088"));
  cs.insert(Coefficient("5895288457659046")*B
	    + Coefficient("2251799813685248")*C
	    - Coefficient("3643488628211199")*D
	    >= -Coefficient("19077554189754888"));
  cs.insert(Coefficient("11790576915318092")*B
	    - Coefficient("4503599627370496")*C
	    + Coefficient("7286977269933197")*D
	    >= -Coefficient("38155108393020576"));
  cs.insert(Coefficient("5895288457659046")*B
	    - Coefficient("2251799813685248")*C
	    - Coefficient("3643488632714799")*D
	    >= -Coefficient("19077554187503088"));
  cs.insert(Coefficient("7286977292451195")*B
	    + Coefficient("11790576919821692")*C
	    + Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108433552976"));
  cs.insert(Coefficient("3643488664239996")*B
	    + Coefficient("5895288486932443")*C
	    - Coefficient("2251799813685248")*D
	    >= -Coefficient("19077554304596680"));
  cs.insert(Coefficient("3643488643973798")*B
	    - Coefficient("5895288446400047")*C
	    + Coefficient("2251799813685248")*D
	    >= -Coefficient("19077554180747688"));
  cs.insert(Coefficient("7286977314969193")*B
	    - Coefficient("11790576937836090")*C
	    - Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108510114168"));
  cs.insert(Coefficient("4503599627370496")*B
	    + Coefficient("7286977247415199")*C
	    + Coefficient("11790576883792894")*D
	    >= -Coefficient("38155108289437784"));
  cs.insert(Coefficient("4503599627370496")*B
	    + Coefficient("7286977251918799")*C
	    - Coefficient("11790576883792894")*D
	    >= -Coefficient("38155108280430584"));
  cs.insert(Coefficient("4503599627370496")*B
	    - Coefficient("7286977229400801")*C
	    + Coefficient("11790576852267696")*D
	    >= -Coefficient("38155108181351392"));
  cs.insert(Coefficient("1125899906842624")*D
	    >= -Coefficient("2947644225451823"));
  cs.insert(Coefficient("4503599627370496")*B
	    - Coefficient("7286977229400801")*C
	    - Coefficient("11790576852267696")*D
	    >= -Coefficient("38155108167840592"));
  cs.insert(-Coefficient("2251799813685248")*D
	    >= -Coefficient("5895288448651847"));
  cs.insert(Coefficient("2251799813685248")*C
	    >= -Coefficient("5895288446400047"));
  cs.insert(-Coefficient("2251799813685248")*C
	    >= -Coefficient("5895288444148247"));
  cs.insert(-Coefficient("1125899906842624")*B
	    + Coefficient("1821744321986899")*C
	    + Coefficient("2947644226577723")*D
	    >= -Coefficient("9538777088122044"));
  cs.insert(-Coefficient("3643488607945001")*B
	    + Coefficient("5895288414874849")*C
	    + Coefficient("2251799813685248")*D
	    >= -Coefficient("19077554059150500"));
  cs.insert(-Coefficient("4503599627370496")*B
	    + Coefficient("7286977292451195")*C
	    - Coefficient("11790576906310892")*D
	    >= -Coefficient("38155108343480984"));
  cs.insert(-Coefficient("7286977220393601")*B
	    + Coefficient("11790576829749698")*C
	    - Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108086775800"));
  cs.insert(-Coefficient("4503599627370496")*B
	    - Coefficient("7286977274436797")*C
	    + Coefficient("11790576901807292")*D
	    >= -Coefficient("38155108325466584"));
  cs.insert(-Coefficient("3643488605693201")*B
	    - Coefficient("5895288414874849")*C
	    + Coefficient("2251799813685248")*D
	    >= -Coefficient("19077554059150500"));
  cs.insert(-Coefficient("1125899906842624")*B
	    - Coefficient("1821744319735099")*C
	    - Coefficient("2947644225451823")*D
	    >= -Coefficient("9538777079114846"));
  cs.insert(-Coefficient("7286977220393601")*B
	    - Coefficient("11790576834253298")*C
	    - Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108113797400"));
  cs.insert(-Coefficient("5895288462162645")*B
	    + Coefficient("2251799813685248")*C
	    + Coefficient("3643488639470198")*D
	    >= -Coefficient("19077554144718892"));
  cs.insert(-Coefficient("11790576924325290")*B
	    - Coefficient("4503599627370496")*C
	    + Coefficient("7286977292451195")*D
	    >= -Coefficient("38155108320962984"));
  cs.insert(-Coefficient("5895288468918045")*B
	    + Coefficient("2251799813685248")*C
	    - Coefficient("3643488641721998")*D
	    >= -Coefficient("19077554160481492"));
  cs.insert(-Coefficient("11790576928828890")*B
	    - Coefficient("4503599627370496")*C
	    - Coefficient("7286977292451195")*D
	    >= -Coefficient("38155108329970184"));
  cs.insert(-Coefficient("281474976710656")*B
	    >= -Coefficient("736911053829681"));
  cs.insert(-Coefficient("4503599627370496")*A
	    + Coefficient("11790576658612912")*B
	    + Coefficient("7286977125818009")*C
	    >= -Coefficient("38155107627408640"));
  cs.insert(-Coefficient("2251799813685248")*A
	    + Coefficient("5895288336061856")*B
	    - Coefficient("3643488560657205")*C
	    >= -Coefficient("19077553829466920"));
  cs.insert(-Coefficient("2251799813685248")*A
	    + Coefficient("3643488535887407")*B
	    + Coefficient("5895288288774060")*D
	    >= -Coefficient("19077553683099932"));
  cs.insert(-Coefficient("7286977274436797")*A
	    + Coefficient("11790576766699304")*B
	    + Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108032732608"));
  cs.insert(-Coefficient("4503599627370496")*A
	    + Coefficient("7286977098796411")*B
	    - Coefficient("11790576609073318")*D
	    >= -Coefficient("38155107483293448"));
  cs.insert(-Coefficient("7286977301458395")*A
	    + Coefficient("11790576735174106")*B
	    - Coefficient("4503599627370496")*D
	    >= -Coefficient("38155107983193008"));
  cs.insert(-Coefficient("4503599627370496")*A
	    + Coefficient("11790576708152508")*C
	    + Coefficient("7286977148336007")*D
	    >= -Coefficient("38155107771523824"));
  cs.insert(-Coefficient("281474976710656")*A
	    + Coefficient("281474976710656")*B
	    + Coefficient("281474976710656")*C
	    + Coefficient("281474976710656")*D
	    >= -Coefficient("1473822119481311"));
  cs.insert(-Coefficient("1125899906842624")*A
	    + Coefficient("2947644178164027")*C
	    - Coefficient("1821744285958102")*D
	    >= -Coefficient("9538776941755056"));
  cs.insert(-Coefficient("1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    + Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= -Coefficient("5895288471169845"));
  cs.insert(-Coefficient("4503599627370496")*A
	    - Coefficient("11790576856771296")*C
	    + Coefficient("7286977247415199")*D
	    >= -Coefficient("38155108221883792"));
  cs.insert(-Coefficient("1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    + Coefficient("1125899906842624")*D
	    >= -Coefficient("5895288471169845"));
  cs.insert(-Coefficient("140737488355328")*A
	    - Coefficient("368455526774103")*C
	    - Coefficient("227718038700250")*D
	    >= -Coefficient("1192347131793131"));
  cs.insert(-Coefficient("1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= -Coefficient("5895288464414445"));
  cs.insert(-Coefficient("3643488643973798")*A
	    + Coefficient("2251799813685248")*B
	    + Coefficient("5895288441896447")*C
	    >= -Coefficient("19077554158229692"));
  cs.insert(-Coefficient("7286977296954795")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("11790576892800094")*C
	    >= -Coefficient("38155108352488176"));
  cs.insert(-Coefficient("4503599627370496")*A
	    - Coefficient("7286977269933197")*B
	    + Coefficient("11790576924325290")*D
	    >= -Coefficient("38155108411034976"));
  cs.insert(-Coefficient("3643488639470198")*A
	    + Coefficient("2251799813685248")*C
	    + Coefficient("5895288466666245")*D
	    >= -Coefficient("19077554219028288"));
  cs.insert(-Coefficient("4503599627370496")*A
	    - Coefficient("7286977296954795")*B
	    - Coefficient("11790576955850488")*D
	    >= -Coefficient("38155108514617768"));
  cs.insert(-Coefficient("7286977251918799")*A
	    + Coefficient("4503599627370496")*C
	    - Coefficient("11790576892800094")*D
	    >= -Coefficient("38155108311955784"));
  cs.insert(-Coefficient("3643488655232797")*A
	    - Coefficient("2251799813685248")*C
	    + Coefficient("5895288480177044")*D
	    >= -Coefficient("19077554264064284"));
  cs.insert(-Coefficient("1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    - Coefficient("2947644229955423")*D
	    >= -Coefficient("9538777099381044"));
  cs.insert(-Coefficient("4503599627370496")*A
	    - Coefficient("11790576874785696")*B
	    + Coefficient("7286977269933197")*C
	    >= -Coefficient("38155108302948584"));
  cs.insert(-Coefficient("7286977274436797")*A
	    - Coefficient("4503599627370496")*B
	    + Coefficient("11790576937836090")*C
	    >= -Coefficient("38155108424545776"));
  cs.insert(-Coefficient("4503599627370496")*A
	    - Coefficient("11790576802728102")*B
	    - Coefficient("7286977197875603")*C
	    >= -Coefficient("38155108019221808"));
  cs.insert(-Coefficient("3643488664239996")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("5895288493687843")*C
	    >= -Coefficient("19077554284330480"));
  cs.insert(-Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= -Coefficient("2947644250784571"));
  cs.insert(-Coefficient("281474976710656")*A
	    - Coefficient("281474976710656")*B
	    + Coefficient("281474976710656")*C
	    - Coefficient("281474976710656")*D
	    >= -Coefficient("1473822131021785"));
  cs.insert(-Coefficient("1125899906842624")*A
	    - Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    + Coefficient("1125899906842624")*D
	    >= -Coefficient("5895288464414445"));
  cs.insert(-Coefficient("1125899906842624")*A
	    - Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= -Coefficient("5895288468918045"));
  cs.insert(-Coefficient("3643488412038417")*A
	    - Coefficient("5895288318047457")*B
	    + Coefficient("2251799813685248")*D
	    >= -Coefficient("19077553665085532"));
  cs.insert(-Coefficient("1821744199263809")*A
	    - Coefficient("2947644153394229")*B
	    - Coefficient("1125899906842624")*D
	    >= -Coefficient("9538776813402468"));
  cs.insert(-Coefficient("5895288378846052")*A
	    + Coefficient("3643488632714799")*B
	    + Coefficient("2251799813685248")*C
	    >= -Coefficient("19077554023121704"));
  cs.insert(-Coefficient("11790576834253298")*A
	    + Coefficient("7286977314969193")*B
	    - Coefficient("4503599627370496")*C
	    >= -Coefficient("38155108302948584"));
  cs.insert(-Coefficient("736911041726257")*A
	    + Coefficient("281474976710656")*B
	    + Coefficient("455436077400500")*D
	    >= -Coefficient("2384694241068264"));
  cs.insert(-Coefficient("5895288347320855")*A
	    + Coefficient("2251799813685248")*B
	    - Coefficient("3643488616952200")*D
	    >= -Coefficient("19077553951064108"));
  cs.insert(-Coefficient("2947644201807925")*A
	    + Coefficient("1821744319735099")*C
	    + Coefficient("1125899906842624")*D
	    >= -Coefficient("9538777048715548"));
  cs.insert(-Coefficient("11790576820742500")*A
	    + Coefficient("7286977296954795")*C
	    - Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108248905384"));
  cs.insert(-Coefficient("11790576996382886")*A
	    - Coefficient("7286977251918799")*C
	    + Coefficient("4503599627370496")*D
	    >= -Coefficient("38155108523624968"));
  cs.insert(-Coefficient("5895288507198642")*A
	    - Coefficient("3643488632714799")*C
	    - Coefficient("2251799813685248")*D
	    >= -Coefficient("19077554291085880"));
  cs.insert(-Coefficient("11790577113476476")*A
	    - Coefficient("4503599627370496")*B
	    + Coefficient("7286977319472793")*D
	    >= -Coefficient("38155108861394936"));
  cs.insert(-Coefficient("5895288572500836")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("3643488652980997")*D
	    >= -Coefficient("19077554450963668"));
  cs.insert(-Coefficient("5895288484680644")*A
	    - Coefficient("3643488607945001")*B
	    + Coefficient("2251799813685248")*C
	    >= -Coefficient("19077554212272888"));
  cs.insert(-Coefficient("2947644274991419")*A
	    - Coefficient("1821744320860999")*B
	    - Coefficient("1125899906842624")*C
	    >= -Coefficient("9538777190578936"));
  cs.insert(-Coefficient("2251799813685248")*A
	    >= -Coefficient("5895288448651847"));

  // Cost function.
  Linear_Expression cost(A + B + C + D);

  // All integer variables.
  Variables_Set ivs(A, D);

  MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(), ivs, cost,
		  MINIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  Coefficient num;
  Coefficient den;
  mip.optimal_value(num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  Coefficient num_kr = -5;
  Coefficient den_kr = 1;

  return pg == point(-A - 2*B - C - D)
    && num == num_kr && den == den_kr;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  // Feasible region.
  Constraint_System cs;
  cs.insert(A >= 0);
  cs.insert(B >= 0);
  cs.insert(C >= 0);
  cs.insert(D >= 0);
  cs.insert(3008*A + 20980*B - 97775*C - 101225*D >= 0);
  cs.insert(3985*A + 25643*B - 135871*C - 130580*D >= 0);
  cs.insert(4324*A + 26978*B - 133655*C - 168473*D >= 0);
  cs.insert(3534*A + 25361*B - 46243*C - 100407*D >= 0);
  cs.insert(8836*A + 40796*B - 176661*C - 215616*D >= 0);
  cs.insert(5376*A + 37562*B - 182576*C - 217615*D >= 0);
  cs.insert(2491*A + 16544*B - 49440*C - 83639*D >= 0);
  cs.insert(4775*A + 39122*B - 136701*C - 193393*D >= 0);
  cs.insert(8046*A + 42958*B - 225138*C - 256575*D >= 0);
  cs.insert(8554*A + 48955*B - 257370*C - 312877*D >= 0);
  cs.insert(6147*A + 45514*B - 165274*C - 227099*D >= 0);
  cs.insert(8366*A + 55140*B - 203989*C - 321623*D >= 0);
  cs.insert(13479*A + 68037*B - 174270*C - 341743*D >= 0);
  cs.insert(21808*A + 78302*B - 322990*C - 487539*D >= 0);
  cs.insert(-8554*A - 48955*B >= -10000);
  cs.insert(-257370*C - 312877*D >= -10000);

  // Cost function.
  Linear_Expression cost(12*A + 6*B + 4*C + 3*D);

  MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(), cost,
		  MAXIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  Coefficient num;
  Coefficient den;
  mip.optimal_value(num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  Coefficient num_kr = Coefficient("8231960000");
  Coefficient den_kr = 581120267;

  return pg == point(679355000*A + 19925000*C + 0*D, 581120267)
    && num == num_kr && den == den_kr;
}

bool
test10() {
  Variable x1(0);
  Variable x2(1);

  // Feasible region.
  Constraint_System cs;
  cs.insert(Coefficient("-3152519739159347")*x1
	    - Coefficient("4503599627370496")*x2
	    >= Coefficient("-2837267765243412480"));
  cs.insert(Coefficient("-14411518807585588")*x1
	    - Coefficient("3602879701896397")*x2
	    >= Coefficient("-19455550390240542720"));
  cs.insert(Coefficient("25300281663413827294061918339864663381194581220517764794612669753428792445999418361495047962679640561898384733039601488923726092173224184608376674992592313740189678034570795170558363467761652042654970959809093133570250935428086587327262919456144944542601257064044846194041676826903812816523290938580750782913463467636686848")*x1
	    >= Coefficient("17591821432561"));
  cs.insert(x2 >= 20);
  cs.insert(-x2 >= -500);

  // Cost function.
  Linear_Expression cost(4*x1 + x2);

  MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(), cost,
		  MAXIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  Coefficient num;
  Coefficient den;
  mip.optimal_value(num, den);
  nout << "Maximum value = " << num << "/" << den << endl;

  if (pg != point(Coefficient("2747195772696002560")*x1
		  + Coefficient("63050394783186940")*x2,
		  Coefficient("3152519739159347"))
      || num != Coefficient("11051833485567197180")
      || den != Coefficient("3152519739159347"))
    return false;

  mip.set_optimization_mode(MINIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  mip.optimal_value(num, den);
  nout << "Minimum value = " << num << "/" << den << endl;

  if (pg != point(Coefficient("17591821432561")*x1 + Coefficient("506005633268276545881238366797293267623891624410355295892253395068575848919988367229900959253592811237967694660792029778474521843464483692167533499851846274803793560691415903411167269355233040853099419196181862671405018708561731746545258389122898890852025141280896923880833536538076256330465818771615015658269269352733736960")*x2, Coefficient("25300281663413827294061918339864663381194581220517764794612669753428792445999418361495047962679640561898384733039601488923726092173224184608376674992592313740189678034570795170558363467761652042654970959809093133570250935428086587327262919456144944542601257064044846194041676826903812816523290938580750782913463467636686848"))
      || num != Coefficient("126501408317069136470309591699323316905972906102588823973063348767143962229997091807475239813398202809491923665198007444618630460866120923041883374962961568700948390172853975852791817338808260213274854799045465667851254677140432936636314597280724722713006285320224230970208384134519064082616454692903753914567334930004866801")
      || den != Coefficient("6325070415853456823515479584966165845298645305129441198653167438357198111499854590373761990669910140474596183259900372230931523043306046152094168748148078435047419508642698792639590866940413010663742739952273283392562733857021646831815729864036236135650314266011211548510419206725953204130822734645187695728365866909171712"))
    return false;

  return true;
}

}

// namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST_F64(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST_F8(test06);
  DO_TEST(test07);
  DO_TEST_F64(test08);
  DO_TEST_F64(test09);
  DO_TEST_F64(test10);
END_MAIN
