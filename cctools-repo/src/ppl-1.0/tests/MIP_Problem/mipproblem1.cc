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
  Variable X01(0);
  Variable X02(1);
  Variable X03(2);
  Variable X04(3);
  Variable X05(4);
  Variable X06(5);
  Variable X07(6);
  Variable X08(7);
  Variable X09(8);
  Variable X10(9);
  Variable X11(10);
  Variable X12(11);
  Variable X13(12);
  Variable X14(13);
  Variable X15(14);
  Variable X16(15);
  Variable X17(16);
  Variable X18(17);
  Variable X19(18);
  Variable X20(19);
  Variable X21(20);
  Variable X22(21);
  Variable X23(22);
  Variable X24(23);
  Variable X25(24);
  Variable X26(25);
  Variable X27(26);
  Variable X28(27);
  Variable X29(28);
  Variable X30(29);
  Variable X31(30);
  Variable X32(31);
  Variable X33(32);
  Variable X34(33);
  Variable X35(34);
  Variable X36(35);
  Variable X37(36);
  Variable X38(37);
  Variable X39(38);

  Constraint_System cs;
  cs.insert(X01 - X02 - X03 + 0*X39 == 0);
  cs.insert(Coefficient("2386907802506363")*X01 - X04 == 0);
  cs.insert(-X01 >= -80);
  cs.insert(X02 - Coefficient("3152519739159347")*X14 >= 0);
  cs.insert(X06 + X07 + X08 + X09 - X14 - X15 == 0);
  cs.insert(Coefficient("2386907802506363")*X06
	    + Coefficient("2386907802506363")*X07
	    + Coefficient("1080863910568919")*X08
	    + Coefficient("7746191359077253")*X09
	    - X16 == 0);
  cs.insert(-X06 + X10 >= -80);
  cs.insert(-X07 + X11 >= 0);
  cs.insert(-X08 + X12 >= 0);
  cs.insert(-X09 + X13 >= 0);
  cs.insert(X22 - X23 - X24 - X25 == 0);
  cs.insert(Coefficient("7746191359077253")*X22 - X26 == 0);
  cs.insert(-X22 >= -500);
  cs.insert(X23 - Coefficient("3152519739159347")*X36 >= 0);
  cs.insert(Coefficient("7746191359077253")*X28
	    + Coefficient("7746191359077253")*X29
	    + Coefficient("3512807709348987")*X30
	    + Coefficient("3332663724254167")*X31
	    - X38 == 0);
  cs.insert(X28 + X29 + X30 + X31 - X36 + X37 + X39 == 44);
  cs.insert(-X28 + X32 >= -500);
  cs.insert(-X29 + X33 >= 0);
  cs.insert(-X30 + X34 >= 0);
  cs.insert(-X31 + X35 >= 0);
  cs.insert(Coefficient("-2661627379775963")*X10
	    - Coefficient("2686397177726501")*X11
	    - Coefficient("5422333951354077")*X12
	    - Coefficient("5469621747441467")*X13
	    + X25
	    - Coefficient("2466846695892189")*X32
	    - Coefficient("4996743786567565")*X33
	    - Coefficient("5064297780978123")*X34
	    - Coefficient("641481471923585")*X35 >= 0);
  cs.insert(X03 - Coefficient("7854277750134145")*X22 >= 0);
  cs.insert(X15
	    - Coefficient("7854277750134145")*X28
	    - Coefficient("7782220156096217")*X29
	    - Coefficient("7782220156096217")*X30
	    - Coefficient("7710162562058289")*X31 >= 0);
  cs.insert(Coefficient("-5422333951354077")*X01 + X24 >= 0);
  cs.insert(X21 >= 2);
  cs.insert(-X16 - X38 >= -300);
  for (dimension_type i = X01.id(); i <= X39.id(); ++i)
    cs.insert(Variable(i) >= 0);

  // Cost function.
  Linear_Expression cost(-10*X02 - 8*X14 - 15*X23 - 12*X36 + 250*X39);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  // Computed numerator and denominator.
  Coefficient num;
  Coefficient den;
  mip.optimal_value(num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  Coefficient num_kr = 11000;
  Coefficient den_kr = 1;
  if (num != num_kr || den != den_kr)
    return false;

  // The feasible / optimizing point.
  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);

  // Making mip unfeasible.
  Constraint_System further_cs;
  further_cs.insert(X05 >= 5);
  further_cs.insert(X05 <= 3);
  mip.add_constraints(further_cs);

  return !mip.is_satisfiable();
}

bool
test02() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  Variable G(6);
  Variable H(7);

  // Cost function
  Linear_Expression cost(-26*A + 343*B + 1233*D - C + F);

  // Feasible region.
  Constraint_System cs;
  cs.insert(A - B + C >= 24);
  cs.insert(B <= 320);
  cs.insert(A + B + 2*D == 23);
  cs.insert(A + 2*B + E == 4112);
  cs.insert(7*A + 5*B + F <= 200);
  cs.insert(138*A + 2*G == 25);
  cs.insert(23*A + 342*B - 34*H == 99);
  for (dimension_type i = A.id(); i <= H.id(); ++i)
    cs.insert(Variable(i) >= 0);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);
  Generator pg_kr = point();
  pg_kr = point(22*B + 1846*C + 863*D + 312468*E + 15090*F + 950*G + 0*H, 76);
  if (pg != pg_kr)
    return false;

  Coefficient num;
  Coefficient den;
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  Coefficient num_kr = 1084869;
  Coefficient den_kr = 76;
  if (num != num_kr || den != den_kr)
    return false;

  // Reoptimize using another objective function.
  Linear_Expression new_cost = -51*A + 632*B;
  mip.set_objective_function(new_cost);
  pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);
  pg_kr = point(782*B + 1598*C + 138244*E + 2890*F + 425*G + 7767*H, 34);
  if (pg != pg_kr)
    return false;

  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  num_kr = 14536;
  den_kr = 1;
  if (num != num_kr || den != den_kr)
    return false;

  // Reoptimize after changing optimization mode.
  mip.set_optimization_mode(MINIMIZATION);
  pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);
  pg_kr = point(17100*A + 26174*B + 2274482*C
		+ 1063871*D + 388070456*E + 18627830*F + 0*H,
		94392);
  if (pg != pg_kr)
    return false;

  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  num_kr = 3917467;
  den_kr = 23598;

  return num == num_kr && den == den_kr;
}

bool
test03() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);

  Constraint_System cs;
  cs.insert(Coefficient("2251799813685248")*A
	    >= Coefficient("-5895288448651847"));
  cs.insert(Coefficient("5895288437392848")*A
	    + Coefficient("3643488632714799")*B
	    - Coefficient("2251799813685248")*C
	    >= Coefficient("-19077554137963492"));
  cs.insert(Coefficient("5895288437392848")*A +
	    Coefficient("3643488632714799")*B
	    + Coefficient("2251799813685248")*C >=
	    Coefficient("-19077554137963492"));
  cs.insert(Coefficient("11790576874785696")*A
	    + Coefficient("4503599627370496")*B
	    + Coefficient("7286977274436797")*D
	    >= Coefficient("-38155108284934184"));
  cs.insert(Coefficient("11790576874785696")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("7286977274436797")*D
	    >= Coefficient("-38155108284934184"));
  cs.insert(Coefficient("11790576879289294")*A
	    + Coefficient("7286977274436797")*C
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    + Coefficient("7286977274436797")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    - Coefficient("7286977274436797")*C
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    - Coefficient("7286977274436797")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("2947644225451823")*A
	    - Coefficient("1125899906842624")*B
	    + Coefficient("1821744319735099")*D
	    >= Coefficient("-9538777088122044"));
  cs.insert(Coefficient("11790576892800094")*A
	    - Coefficient("4503599627370496")*B
	    - Coefficient("7286977274436797")*D
	    >= Coefficient("-38155108325466584"));
  cs.insert(Coefficient("5895288437392848")*A
	    - Coefficient("3643488630462999")*B
	    + Coefficient("2251799813685248")*C
	    >= Coefficient("-19077554133459892"));
  cs.insert(Coefficient("2947644218696424")*A
	    - Coefficient("1821744320860999")*B
	    - Coefficient("1125899906842624")*C
	    >= Coefficient("-9538777072359446"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("11790576924325290")*B
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108379509776"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("11790576924325290")*B
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108379509776"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= Coefficient("-2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644225451823"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= Coefficient("-2947644225451823"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("4503599627370496")*B
	    + Coefficient("11790576865778496")*C
	    >= Coefficient("-38155108266919784"));
  cs.insert(Coefficient("7286977251918799")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("11790576870282096")*C
	    >= Coefficient("-38155108244401792"));
  cs.insert(Coefficient("1821744320860999")*A
	    + Coefficient("1125899906842624")*C
	    + Coefficient("2947644226577723")*D
	    >= Coefficient("-9538777093751544"));
  cs.insert(Coefficient("1821744320860999")*A
	    + Coefficient("1125899906842624")*C
	    - Coefficient("2947644226577723")*
	    D >= Coefficient("-9538777093751544"));
  cs.insert(Coefficient("1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    + Coefficient("2947644228829523")*D
	    >= Coefficient("-9538777096003344"));
  cs.insert(Coefficient("1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    - Coefficient("2947644228829523")*D
	    >= Coefficient("-9538777096003344"));
  cs.insert(Coefficient("3643488664239996")*A
	    - Coefficient("2251799813685248")*B
	    + Coefficient("5895288468918045")*C
	    >= Coefficient("-19077554257308884"));
  cs.insert(Coefficient("3643488652980997")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("5895288468918045")*C
	    >= Coefficient("-19077554232539084"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= Coefficient("-2947644229392473"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644227140673"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= Coefficient("-2947644227703623"));
  cs.insert(Coefficient("7286977314969193")*A
	    - Coefficient("11790576906310892")*B
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108447063768"));
  cs.insert(Coefficient("3643488655232797")*A
	    - Coefficient("5895288446400047")*B
	    - Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554203265688"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576753188506")*B
	    + Coefficient("7286977179861205")*C
	    >= Coefficient("-38155107920142616"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576766699304")*B
	    - Coefficient("7286977179861205")*C
	    >= Coefficient("-38155107965178608"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("7286977157343207")*B
	    + Coefficient("11790576712656108")*D
	    >= Coefficient("-38155107816559824"));
  cs.insert(Coefficient("2251799813685248")*A
	    + Coefficient("3643488592182402")*B
	    - Coefficient("5895288374342453")*D
	    >= Coefficient("-19077553960071308"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576753188506")*C
	    + Coefficient("7286977175357605")*D
	    >= Coefficient("-38155107924646216"));
  cs.insert(Coefficient("2251799813685248")*A
	    + Coefficient("5895288390105051")*C
	    - Coefficient("3643488594434202")*D
	    >= Coefficient("-19077553996100104"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("5895288421630249")*C
	    + Coefficient("3643488619204000")*D
	    >= Coefficient("-19077554088423896"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("11790576865778496")*C
	    - Coefficient("7286977247415199")*D
	    >= Coefficient("-38155108244401792"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("7286977247415199")*B
	    + Coefficient("11790576888296494")*D
	    >= Coefficient("-38155108307452184"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("3643488639470198")*B
	    - Coefficient("5895288464414445")*D
	    >= Coefficient("-19077554210021088"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("5895288428385648")*B
	    + Coefficient("3643488630462999")*C
	    >= Coefficient("-19077554131208092"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("11790576843260498")*B
	    - Coefficient("7286977224897201")*C
	    >= Coefficient("-38155108163336992"));
  cs.insert(Coefficient("1125899906842624")*B
	    >= Coefficient("-2947644227703623"));
  cs.insert(Coefficient("5895288459910846")*B
	    + Coefficient("2251799813685248")*C
	    + Coefficient("3643488630462999")*D
	    >= Coefficient("-19077554198762088"));
  cs.insert(Coefficient("5895288457659046")*B
	    + Coefficient("2251799813685248")*C
	    - Coefficient("3643488628211199")*D
	    >= Coefficient("-19077554189754888"));
  cs.insert(Coefficient("11790576915318092")*B
	    - Coefficient("4503599627370496")*C
	    + Coefficient("7286977269933197")*D
	    >= Coefficient("-38155108393020576"));
  cs.insert(Coefficient("5895288457659046")*B
	    - Coefficient("2251799813685248")*C
	    - Coefficient("3643488632714799")*D
	    >= Coefficient("-19077554187503088"));
  cs.insert(Coefficient("7286977292451195")*B
	    + Coefficient("11790576919821692")*C
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108433552976"));
  cs.insert(Coefficient("3643488664239996")*B
	    + Coefficient("5895288486932443")*C
	    - Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554304596680"));
  cs.insert(Coefficient("3643488643973798")*B
	    - Coefficient("5895288446400047")*C
	    + Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554180747688"));
  cs.insert(Coefficient("7286977314969193")*B
	    - Coefficient("11790576937836090")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108510114168"));
  cs.insert(Coefficient("4503599627370496")*B
	    + Coefficient("7286977247415199")*C
	    + Coefficient("11790576883792894")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("4503599627370496")*B
	    + Coefficient("7286977251918799")*C
	    - Coefficient("11790576883792894")*D
	    >= Coefficient("-38155108280430584"));
  cs.insert(Coefficient("4503599627370496")*B
	    - Coefficient("7286977229400801")*C
	    + Coefficient("11790576852267696")*D
	    >= Coefficient("-38155108181351392"));
  cs.insert(Coefficient("1125899906842624")*D
	    >= Coefficient("-2947644225451823"));
  cs.insert(Coefficient("4503599627370496")*B
	    - Coefficient("7286977229400801")*C
	    - Coefficient("11790576852267696")*D
	    >= Coefficient("-38155108167840592"));
  cs.insert(Coefficient("-2251799813685248")*D
	    >= Coefficient("-5895288448651847"));
  cs.insert(Coefficient("2251799813685248")*C
	    >= Coefficient("-5895288446400047"));
  cs.insert(Coefficient("-2251799813685248")*C
	    >= Coefficient("-5895288444148247"));
  cs.insert(Coefficient("-1125899906842624")*B
	    + Coefficient("1821744321986899")*C
	    + Coefficient("2947644226577723")*D
	    >= Coefficient("-9538777088122044"));
  cs.insert(Coefficient("-3643488607945001")*B
	    + Coefficient("5895288414874849")*C
	    + Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554059150500"));
  cs.insert(Coefficient("-4503599627370496")*B
	    + Coefficient("7286977292451195")*C
	    - Coefficient("11790576906310892")*D
	    >= Coefficient("-38155108343480984"));
  cs.insert(Coefficient("-7286977220393601")*B
	    + Coefficient("11790576829749698")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108086775800"));
  cs.insert(Coefficient("-4503599627370496")*B
	    - Coefficient("7286977274436797")*C
	    + Coefficient("11790576901807292")*D
	    >= Coefficient("-38155108325466584"));
  cs.insert(Coefficient("-3643488605693201")*B
	    - Coefficient("5895288414874849")*C
	    + Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554059150500"));
  cs.insert(Coefficient("-1125899906842624")*B
	    - Coefficient("1821744319735099")*C
	    - Coefficient("2947644225451823")*D
	    >= Coefficient("-9538777079114846"));
  cs.insert(Coefficient("-7286977220393601")*B
	    - Coefficient("11790576834253298")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108113797400"));
  cs.insert(Coefficient("-5895288462162645")*B
	    + Coefficient("2251799813685248")*C
	    + Coefficient("3643488639470198")*D
	    >= Coefficient("-19077554144718892"));
  cs.insert(Coefficient("-11790576924325290")*B
	    - Coefficient("4503599627370496")*C
	    + Coefficient("7286977292451195")*D
	    >= Coefficient("-38155108320962984"));
  cs.insert(Coefficient("-5895288468918045")*B
	    + Coefficient("2251799813685248")*C
	    - Coefficient("3643488641721998")*D
	    >= Coefficient("-19077554160481492"));
  cs.insert(Coefficient("-11790576928828890")*B
	    - Coefficient("4503599627370496")*C
	    - Coefficient("7286977292451195")*D
	    >= Coefficient("-38155108329970184"));
  cs.insert(Coefficient("-281474976710656")*B
	    >= Coefficient("-736911053829681"));
  cs.insert(Coefficient("-4503599627370496")*A
	    + Coefficient("11790576658612912")*B
	    + Coefficient("7286977125818009")*C
	    >= Coefficient("-38155107627408640"));
  cs.insert(Coefficient("-2251799813685248")*A
	    + Coefficient("5895288336061856")*B
	    - Coefficient("3643488560657205")*C
	    >= Coefficient("-19077553829466920"));
  cs.insert(Coefficient("-2251799813685248")*A
	    + Coefficient("3643488535887407")*B
	    + Coefficient("5895288288774060")*D
	    >= Coefficient("-19077553683099932"));
  cs.insert(Coefficient("-7286977274436797")*A
	    + Coefficient("11790576766699304")*B
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108032732608"));
  cs.insert(Coefficient("-4503599627370496")*A
	    + Coefficient("7286977098796411")*B
	    - Coefficient("11790576609073318")*D
	    >= Coefficient("-38155107483293448"));
  cs.insert(Coefficient("-7286977301458395")*A
	    + Coefficient("11790576735174106")*B
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155107983193008"));
  cs.insert(Coefficient("-4503599627370496")*A
	    + Coefficient("11790576708152508")*C
	    + Coefficient("7286977148336007")*D
	    >= Coefficient("-38155107771523824"));
  cs.insert(Coefficient("-281474976710656")*A
	    + Coefficient("281474976710656")*B
	    + Coefficient("281474976710656")*C
	    + Coefficient("281474976710656")*D
	    >= Coefficient("-1473822119481311"));
  cs.insert(Coefficient("-1125899906842624")*A
	    + Coefficient("2947644178164027")*C
	    - Coefficient("1821744285958102")*D
	    >= Coefficient("-9538776941755056"));
  cs.insert(Coefficient("-1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    + Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288471169845"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("11790576856771296")*C
	    + Coefficient("7286977247415199")*D
	    >= Coefficient("-38155108221883792"));
  cs.insert(Coefficient("-1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    + Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288471169845"));
  cs.insert(Coefficient("-140737488355328")*A
	    - Coefficient("368455526774103")*C
	    - Coefficient("227718038700250")*D
	    >= Coefficient("-1192347131793131"));
  cs.insert(Coefficient("-1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288464414445"));
  cs.insert(Coefficient("-3643488643973798")*A
	    + Coefficient("2251799813685248")*B
	    + Coefficient("5895288441896447")*C
	    >= Coefficient("-19077554158229692"));
  cs.insert(Coefficient("-7286977296954795")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("11790576892800094")*C
	    >= Coefficient("-38155108352488176"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("7286977269933197")*B
	    + Coefficient("11790576924325290")*D
	    >= Coefficient("-38155108411034976"));
  cs.insert(Coefficient("-3643488639470198")*A
	    + Coefficient("2251799813685248")*C
	    + Coefficient("5895288466666245")*D
	    >= Coefficient("-19077554219028288"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("7286977296954795")*B
	    - Coefficient("11790576955850488")*D
	    >= Coefficient("-38155108514617768"));
  cs.insert(Coefficient("-7286977251918799")*A
	    + Coefficient("4503599627370496")*C
	    - Coefficient("11790576892800094")*D
	    >= Coefficient("-38155108311955784"));
  cs.insert(Coefficient("-3643488655232797")*A
	    - Coefficient("2251799813685248")*C
	    + Coefficient("5895288480177044")*D
	    >= Coefficient("-19077554264064284"));
  cs.insert(Coefficient("-1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    - Coefficient("2947644229955423")*D
	    >= Coefficient("-9538777099381044"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("11790576874785696")*B
	    + Coefficient("7286977269933197")*C
	    >= Coefficient("-38155108302948584"));
  cs.insert(Coefficient("-7286977274436797")*A
	    - Coefficient("4503599627370496")*B
	    + Coefficient("11790576937836090")*C
	    >= Coefficient("-38155108424545776"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("11790576802728102")*B
	    - Coefficient("7286977197875603")*C
	    >= Coefficient("-38155108019221808"));
  cs.insert(Coefficient("-3643488664239996")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("5895288493687843")*C
	    >= Coefficient("-19077554284330480"));
  cs.insert(Coefficient("-562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644250784571"));
  cs.insert(Coefficient("-281474976710656")*A
	    - Coefficient("281474976710656")*B
	    + Coefficient("281474976710656")*C
	    - Coefficient("281474976710656")*D
	    >= Coefficient("-1473822131021785"));
  cs.insert(Coefficient("-1125899906842624")*A
	    - Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    + Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288464414445"));
  cs.insert(Coefficient("-1125899906842624")*A
	    - Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288468918045"));
  cs.insert(Coefficient("-3643488412038417")*A
	    - Coefficient("5895288318047457")*B
	    + Coefficient("2251799813685248")*D
	    >= Coefficient("-19077553665085532"));
  cs.insert(Coefficient("-1821744199263809")*A
	    - Coefficient("2947644153394229")*B
	    - Coefficient("1125899906842624")*D
	    >= Coefficient("-9538776813402468"));
  cs.insert(Coefficient("-5895288378846052")*A
	    + Coefficient("3643488632714799")*B
	    + Coefficient("2251799813685248")*C
	    >= Coefficient("-19077554023121704"));
  cs.insert(Coefficient("-11790576834253298")*A
	    + Coefficient("7286977314969193")*B
	    - Coefficient("4503599627370496")*C
	    >= Coefficient("-38155108302948584"));
  cs.insert(Coefficient("-736911041726257")*A
	    + Coefficient("281474976710656")*B
	    + Coefficient("455436077400500")*D
	    >= Coefficient("-2384694241068264"));
  cs.insert(Coefficient("-5895288347320855")*A
	    + Coefficient("2251799813685248")*B
	    - Coefficient("3643488616952200")*D
	    >= Coefficient("-19077553951064108"));
  cs.insert(Coefficient("-2947644201807925")*A
	    + Coefficient("1821744319735099")*C
	    + Coefficient("1125899906842624")*D
	    >= Coefficient("-9538777048715548"));
  cs.insert(Coefficient("-11790576820742500")*A
	    + Coefficient("7286977296954795")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108248905384"));
  cs.insert(Coefficient("-11790576996382886")*A
	    - Coefficient("7286977251918799")*C
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108523624968"));
  cs.insert(Coefficient("-5895288507198642")*A
	    - Coefficient("3643488632714799")*C
	    - Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554291085880"));
  cs.insert(Coefficient("-11790577113476476")*A
	    - Coefficient("4503599627370496")*B
	    + Coefficient("7286977319472793")*D
	    >= Coefficient("-38155108861394936"));
  cs.insert(Coefficient("-5895288572500836")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("3643488652980997")*D
	    >= Coefficient("-19077554450963668"));
  cs.insert(Coefficient("-5895288484680644")*A
	    - Coefficient("3643488607945001")*B
	    + Coefficient("2251799813685248")*C
	    >= Coefficient("-19077554212272888"));
  cs.insert(Coefficient("-2947644274991419")*A
	    - Coefficient("1821744320860999")*B
	    - Coefficient("1125899906842624")*C
	    >= Coefficient("-9538777190578936"));
  cs.insert(Coefficient("-2251799813685248")*A
	    >= Coefficient("-5895288448651847"));

  // Cost function
  Linear_Expression cost(10*A + 21*B + 31*C + 45*D);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point obtained by simplex:\n";
  print_generator(pg);

  Coefficient num;
  Coefficient den;
  mip.evaluate_objective_function(pg, num, den);
  nout << "\nOptimum value = " << num << "/" << den << endl;

  C_Polyhedron ph(cs);
  Coefficient num1;
  Coefficient den1;
  bool maximum;
  Generator pg1 = point();
  ph.maximize(cost, num1, den1, maximum, pg1);

  nout << "\nOptimizing point obtained by enumeration:\n";
  print_generator(pg1);
  nout << "\nOptimum value = " << num1 << "/" << den1 << endl;

  return maximum && num == num1 && den == den1 && pg == pg1;
}

bool
test04() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Variable E(4);
  Variable F(5);
  Variable G(6);
  Variable H(7);

  // Cost function
  Linear_Expression cost(-26*A + 343*B + 1233*D - C + F);

  // Feasible region.
  Constraint_System cs;
  cs.insert(A - B + C >= 24);
  cs.insert(B <= 320);
  cs.insert(A + B + 2*D == 23);
  cs.insert(A + 2*B + E == 4112);
  cs.insert(7*A + 5*B + F <= 200);
  cs.insert(138*A + 2*G == 25);
  cs.insert(23*A + 342*B - 34*H == 99);
  for (dimension_type i = A.id(); i <= H.id(); ++i)
    cs.insert(Variable(i) >= 0);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);
  Generator pg_kr = point();
  pg_kr = point(22*B + 1846*C + 863*D + 312468*E + 15090*F + 950*G + 0*H, 76);
  if (pg != pg_kr)
    return false;

  Coefficient num;
  Coefficient den;
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  Coefficient num_kr = 1084869;
  Coefficient den_kr = 76;
  if (num != num_kr || den != den_kr)
    return false;

  // Reoptimize using another objective function.
  Linear_Expression new_cost = -51*A + 632*B;
  mip.set_objective_function(new_cost);
  pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);
  pg_kr = point(782*B + 1598*C + 138244*E + 2890*F + 425*G + 7767*H, 34);
  if (pg != pg_kr)
    return false;

  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  num_kr = 14536;
  den_kr = 1;
  if (num != num_kr || den != den_kr)
    return false;

  // Reoptimize after changing optimization mode.
  mip.set_optimization_mode(MINIMIZATION);
  pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  print_generator(pg);
  pg_kr = point(17100*A + 26174*B + 2274482*C
		+ 1063871*D + 388070456*E + 18627830*F + 0*H,
		94392);
  if (pg != pg_kr)
    return false;

  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  num_kr = 3917467;
  den_kr = 23598;
  if (num != num_kr || den != den_kr)
    return false;

  return true;
}

bool
test05() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Constraint_System cs;
  cs.insert(Coefficient("2251799813685248")*A
	    >= Coefficient("-5895288448651847"));
  cs.insert(Coefficient("5895288437392848")*A
	    + Coefficient("3643488632714799")*B
	    - Coefficient("2251799813685248")*C
	    >= Coefficient("-19077554137963492"));
  cs.insert(Coefficient("5895288437392848")*A +
	    Coefficient("3643488632714799")*B
	    + Coefficient("2251799813685248")*C >=
	    Coefficient("-19077554137963492"));
  cs.insert(Coefficient("11790576874785696")*A
	    + Coefficient("4503599627370496")*B
	    + Coefficient("7286977274436797")*D
	    >= Coefficient("-38155108284934184"));
  cs.insert(Coefficient("11790576874785696")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("7286977274436797")*D
	    >= Coefficient("-38155108284934184"));
  cs.insert(Coefficient("11790576879289294")*A
	    + Coefficient("7286977274436797")*C
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    + Coefficient("7286977274436797")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    - Coefficient("7286977274436797")*C
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("11790576879289294")*A
	    - Coefficient("7286977274436797")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("2947644225451823")*A
	    - Coefficient("1125899906842624")*B
	    + Coefficient("1821744319735099")*D
	    >= Coefficient("-9538777088122044"));
  cs.insert(Coefficient("11790576892800094")*A
	    - Coefficient("4503599627370496")*B
	    - Coefficient("7286977274436797")*D
	    >= Coefficient("-38155108325466584"));
  cs.insert(Coefficient("5895288437392848")*A
	    - Coefficient("3643488630462999")*B
	    + Coefficient("2251799813685248")*C
	    >= Coefficient("-19077554133459892"));
  cs.insert(Coefficient("2947644218696424")*A
	    - Coefficient("1821744320860999")*B
	    - Coefficient("1125899906842624")*C
	    >= Coefficient("-9538777072359446"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("11790576924325290")*B
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108379509776"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("11790576924325290")*B
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108379509776"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= Coefficient("-2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644225451823"));
  cs.insert(Coefficient("562949953421312")*A
	    + Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= Coefficient("-2947644225451823"));
  cs.insert(Coefficient("7286977269933197")*A
	    + Coefficient("4503599627370496")*B
	    + Coefficient("11790576865778496")*C
	    >= Coefficient("-38155108266919784"));
  cs.insert(Coefficient("7286977251918799")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("11790576870282096")*C
	    >= Coefficient("-38155108244401792"));
  cs.insert(Coefficient("1821744320860999")*A
	    + Coefficient("1125899906842624")*C
	    + Coefficient("2947644226577723")*D
	    >= Coefficient("-9538777093751544"));
  cs.insert(Coefficient("1821744320860999")*A
	    + Coefficient("1125899906842624")*C
	    - Coefficient("2947644226577723")*
	    D >= Coefficient("-9538777093751544"));
  cs.insert(Coefficient("1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    + Coefficient("2947644228829523")*D
	    >= Coefficient("-9538777096003344"));
  cs.insert(Coefficient("1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    - Coefficient("2947644228829523")*D
	    >= Coefficient("-9538777096003344"));
  cs.insert(Coefficient("3643488664239996")*A
	    - Coefficient("2251799813685248")*B
	    + Coefficient("5895288468918045")*C
	    >= Coefficient("-19077554257308884"));
  cs.insert(Coefficient("3643488652980997")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("5895288468918045")*C
	    >= Coefficient("-19077554232539084"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644226577723"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= Coefficient("-2947644229392473"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644227140673"));
  cs.insert(Coefficient("562949953421312")*A
	    - Coefficient("562949953421312")*B
	    - Coefficient("562949953421312")*C
	    - Coefficient("562949953421312")*D
	    >= Coefficient("-2947644227703623"));
  cs.insert(Coefficient("7286977314969193")*A
	    - Coefficient("11790576906310892")*B
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108447063768"));
  cs.insert(Coefficient("3643488655232797")*A
	    - Coefficient("5895288446400047")*B
	    - Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554203265688"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576753188506")*B
	    + Coefficient("7286977179861205")*C
	    >= Coefficient("-38155107920142616"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576766699304")*B
	    - Coefficient("7286977179861205")*C
	    >= Coefficient("-38155107965178608"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("7286977157343207")*B
	    + Coefficient("11790576712656108")*D
	    >= Coefficient("-38155107816559824"));
  cs.insert(Coefficient("2251799813685248")*A
	    + Coefficient("3643488592182402")*B
	    - Coefficient("5895288374342453")*D
	    >= Coefficient("-19077553960071308"));
  cs.insert(Coefficient("4503599627370496")*A
	    + Coefficient("11790576753188506")*C
	    + Coefficient("7286977175357605")*D
	    >= Coefficient("-38155107924646216"));
  cs.insert(Coefficient("2251799813685248")*A
	    + Coefficient("5895288390105051")*C
	    - Coefficient("3643488594434202")*D
	    >= Coefficient("-19077553996100104"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("5895288421630249")*C
	    + Coefficient("3643488619204000")*D
	    >= Coefficient("-19077554088423896"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("11790576865778496")*C
	    - Coefficient("7286977247415199")*D
	    >= Coefficient("-38155108244401792"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("7286977247415199")*B
	    + Coefficient("11790576888296494")*D
	    >= Coefficient("-38155108307452184"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("3643488639470198")*B
	    - Coefficient("5895288464414445")*D
	    >= Coefficient("-19077554210021088"));
  cs.insert(Coefficient("2251799813685248")*A
	    - Coefficient("5895288428385648")*B
	    + Coefficient("3643488630462999")*C
	    >= Coefficient("-19077554131208092"));
  cs.insert(Coefficient("4503599627370496")*A
	    - Coefficient("11790576843260498")*B
	    - Coefficient("7286977224897201")*C
	    >= Coefficient("-38155108163336992"));
  cs.insert(Coefficient("1125899906842624")*B
	    >= Coefficient("-2947644227703623"));
  cs.insert(Coefficient("5895288459910846")*B
	    + Coefficient("2251799813685248")*C
	    + Coefficient("3643488630462999")*D
	    >= Coefficient("-19077554198762088"));
  cs.insert(Coefficient("5895288457659046")*B
	    + Coefficient("2251799813685248")*C
	    - Coefficient("3643488628211199")*D
	    >= Coefficient("-19077554189754888"));
  cs.insert(Coefficient("11790576915318092")*B
	    - Coefficient("4503599627370496")*C
	    + Coefficient("7286977269933197")*D
	    >= Coefficient("-38155108393020576"));
  cs.insert(Coefficient("5895288457659046")*B
	    - Coefficient("2251799813685248")*C
	    - Coefficient("3643488632714799")*D
	    >= Coefficient("-19077554187503088"));
  cs.insert(Coefficient("7286977292451195")*B
	    + Coefficient("11790576919821692")*C
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108433552976"));
  cs.insert(Coefficient("3643488664239996")*B
	    + Coefficient("5895288486932443")*C
	    - Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554304596680"));
  cs.insert(Coefficient("3643488643973798")*B
	    - Coefficient("5895288446400047")*C
	    + Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554180747688"));
  cs.insert(Coefficient("7286977314969193")*B
	    - Coefficient("11790576937836090")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108510114168"));
  cs.insert(Coefficient("4503599627370496")*B
	    + Coefficient("7286977247415199")*C
	    + Coefficient("11790576883792894")*D
	    >= Coefficient("-38155108289437784"));
  cs.insert(Coefficient("4503599627370496")*B
	    + Coefficient("7286977251918799")*C
	    - Coefficient("11790576883792894")*D
	    >= Coefficient("-38155108280430584"));
  cs.insert(Coefficient("4503599627370496")*B
	    - Coefficient("7286977229400801")*C
	    + Coefficient("11790576852267696")*D
	    >= Coefficient("-38155108181351392"));
  cs.insert(Coefficient("1125899906842624")*D
	    >= Coefficient("-2947644225451823"));
  cs.insert(Coefficient("4503599627370496")*B
	    - Coefficient("7286977229400801")*C
	    - Coefficient("11790576852267696")*D
	    >= Coefficient("-38155108167840592"));
  cs.insert(Coefficient("-2251799813685248")*D
	    >= Coefficient("-5895288448651847"));
  cs.insert(Coefficient("2251799813685248")*C
	    >= Coefficient("-5895288446400047"));
  cs.insert(Coefficient("-2251799813685248")*C
	    >= Coefficient("-5895288444148247"));
  cs.insert(Coefficient("-1125899906842624")*B
	    + Coefficient("1821744321986899")*C
	    + Coefficient("2947644226577723")*D
	    >= Coefficient("-9538777088122044"));
  cs.insert(Coefficient("-3643488607945001")*B
	    + Coefficient("5895288414874849")*C
	    + Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554059150500"));
  cs.insert(Coefficient("-4503599627370496")*B
	    + Coefficient("7286977292451195")*C
	    - Coefficient("11790576906310892")*D
	    >= Coefficient("-38155108343480984"));
  cs.insert(Coefficient("-7286977220393601")*B
	    + Coefficient("11790576829749698")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108086775800"));
  cs.insert(Coefficient("-4503599627370496")*B
	    - Coefficient("7286977274436797")*C
	    + Coefficient("11790576901807292")*D
	    >= Coefficient("-38155108325466584"));
  cs.insert(Coefficient("-3643488605693201")*B
	    - Coefficient("5895288414874849")*C
	    + Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554059150500"));
  cs.insert(Coefficient("-1125899906842624")*B
	    - Coefficient("1821744319735099")*C
	    - Coefficient("2947644225451823")*D
	    >= Coefficient("-9538777079114846"));
  cs.insert(Coefficient("-7286977220393601")*B
	    - Coefficient("11790576834253298")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108113797400"));
  cs.insert(Coefficient("-5895288462162645")*B
	    + Coefficient("2251799813685248")*C
	    + Coefficient("3643488639470198")*D
	    >= Coefficient("-19077554144718892"));
  cs.insert(Coefficient("-11790576924325290")*B
	    - Coefficient("4503599627370496")*C
	    + Coefficient("7286977292451195")*D
	    >= Coefficient("-38155108320962984"));
  cs.insert(Coefficient("-5895288468918045")*B
	    + Coefficient("2251799813685248")*C
	    - Coefficient("3643488641721998")*D
	    >= Coefficient("-19077554160481492"));
  cs.insert(Coefficient("-11790576928828890")*B
	    - Coefficient("4503599627370496")*C
	    - Coefficient("7286977292451195")*D
	    >= Coefficient("-38155108329970184"));
  cs.insert(Coefficient("-281474976710656")*B
	    >= Coefficient("-736911053829681"));
  cs.insert(Coefficient("-4503599627370496")*A
	    + Coefficient("11790576658612912")*B
	    + Coefficient("7286977125818009")*C
	    >= Coefficient("-38155107627408640"));
  cs.insert(Coefficient("-2251799813685248")*A
	    + Coefficient("5895288336061856")*B
	    - Coefficient("3643488560657205")*C
	    >= Coefficient("-19077553829466920"));
  cs.insert(Coefficient("-2251799813685248")*A
	    + Coefficient("3643488535887407")*B
	    + Coefficient("5895288288774060")*D
	    >= Coefficient("-19077553683099932"));
  cs.insert(Coefficient("-7286977274436797")*A
	    + Coefficient("11790576766699304")*B
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108032732608"));
  cs.insert(Coefficient("-4503599627370496")*A
	    + Coefficient("7286977098796411")*B
	    - Coefficient("11790576609073318")*D
	    >= Coefficient("-38155107483293448"));
  cs.insert(Coefficient("-7286977301458395")*A
	    + Coefficient("11790576735174106")*B
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155107983193008"));
  cs.insert(Coefficient("-4503599627370496")*A
	    + Coefficient("11790576708152508")*C
	    + Coefficient("7286977148336007")*D
	    >= Coefficient("-38155107771523824"));
  cs.insert(Coefficient("-281474976710656")*A
	    + Coefficient("281474976710656")*B
	    + Coefficient("281474976710656")*C
	    + Coefficient("281474976710656")*D
	    >= Coefficient("-1473822119481311"));
  cs.insert(Coefficient("-1125899906842624")*A
	    + Coefficient("2947644178164027")*C
	    - Coefficient("1821744285958102")*D
	    >= Coefficient("-9538776941755056"));
  cs.insert(Coefficient("-1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    + Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288471169845"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("11790576856771296")*C
	    + Coefficient("7286977247415199")*D
	    >= Coefficient("-38155108221883792"));
  cs.insert(Coefficient("-1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    + Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288471169845"));
  cs.insert(Coefficient("-140737488355328")*A
	    - Coefficient("368455526774103")*C
	    - Coefficient("227718038700250")*D
	    >= Coefficient("-1192347131793131"));
  cs.insert(Coefficient("-1125899906842624")*A
	    + Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288464414445"));
  cs.insert(Coefficient("-3643488643973798")*A
	    + Coefficient("2251799813685248")*B
	    + Coefficient("5895288441896447")*C
	    >= Coefficient("-19077554158229692"));
  cs.insert(Coefficient("-7286977296954795")*A
	    + Coefficient("4503599627370496")*B
	    - Coefficient("11790576892800094")*C
	    >= Coefficient("-38155108352488176"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("7286977269933197")*B
	    + Coefficient("11790576924325290")*D
	    >= Coefficient("-38155108411034976"));
  cs.insert(Coefficient("-3643488639470198")*A
	    + Coefficient("2251799813685248")*C
	    + Coefficient("5895288466666245")*D
	    >= Coefficient("-19077554219028288"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("7286977296954795")*B
	    - Coefficient("11790576955850488")*D
	    >= Coefficient("-38155108514617768"));
  cs.insert(Coefficient("-7286977251918799")*A
	    + Coefficient("4503599627370496")*C
	    - Coefficient("11790576892800094")*D
	    >= Coefficient("-38155108311955784"));
  cs.insert(Coefficient("-3643488655232797")*A
	    - Coefficient("2251799813685248")*C
	    + Coefficient("5895288480177044")*D
	    >= Coefficient("-19077554264064284"));
  cs.insert(Coefficient("-1821744320860999")*A
	    - Coefficient("1125899906842624")*C
	    - Coefficient("2947644229955423")*D
	    >= Coefficient("-9538777099381044"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("11790576874785696")*B
	    + Coefficient("7286977269933197")*C
	    >= Coefficient("-38155108302948584"));
  cs.insert(Coefficient("-7286977274436797")*A
	    - Coefficient("4503599627370496")*B
	    + Coefficient("11790576937836090")*C
	    >= Coefficient("-38155108424545776"));
  cs.insert(Coefficient("-4503599627370496")*A
	    - Coefficient("11790576802728102")*B
	    - Coefficient("7286977197875603")*C
	    >= Coefficient("-38155108019221808"));
  cs.insert(Coefficient("-3643488664239996")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("5895288493687843")*C
	    >= Coefficient("-19077554284330480"));
  cs.insert(Coefficient("-562949953421312")*A
	    - Coefficient("562949953421312")*B
	    + Coefficient("562949953421312")*C
	    + Coefficient("562949953421312")*D
	    >= Coefficient("-2947644250784571"));
  cs.insert(Coefficient("-281474976710656")*A
	    - Coefficient("281474976710656")*B
	    + Coefficient("281474976710656")*C
	    - Coefficient("281474976710656")*D
	    >= Coefficient("-1473822131021785"));
  cs.insert(Coefficient("-1125899906842624")*A
	    - Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    + Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288464414445"));
  cs.insert(Coefficient("-1125899906842624")*A
	    - Coefficient("1125899906842624")*B
	    - Coefficient("1125899906842624")*C
	    - Coefficient("1125899906842624")*D
	    >= Coefficient("-5895288468918045"));
  cs.insert(Coefficient("-3643488412038417")*A
	    - Coefficient("5895288318047457")*B
	    + Coefficient("2251799813685248")*D
	    >= Coefficient("-19077553665085532"));
  cs.insert(Coefficient("-1821744199263809")*A
	    - Coefficient("2947644153394229")*B
	    - Coefficient("1125899906842624")*D
	    >= Coefficient("-9538776813402468"));
  cs.insert(Coefficient("-5895288378846052")*A
	    + Coefficient("3643488632714799")*B
	    + Coefficient("2251799813685248")*C
	    >= Coefficient("-19077554023121704"));
  cs.insert(Coefficient("-11790576834253298")*A
	    + Coefficient("7286977314969193")*B
	    - Coefficient("4503599627370496")*C
	    >= Coefficient("-38155108302948584"));
  cs.insert(Coefficient("-736911041726257")*A
	    + Coefficient("281474976710656")*B
	    + Coefficient("455436077400500")*D
	    >= Coefficient("-2384694241068264"));
  cs.insert(Coefficient("-5895288347320855")*A
	    + Coefficient("2251799813685248")*B
	    - Coefficient("3643488616952200")*D
	    >= Coefficient("-19077553951064108"));
  cs.insert(Coefficient("-2947644201807925")*A
	    + Coefficient("1821744319735099")*C
	    + Coefficient("1125899906842624")*D
	    >= Coefficient("-9538777048715548"));
  cs.insert(Coefficient("-11790576820742500")*A
	    + Coefficient("7286977296954795")*C
	    - Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108248905384"));
  cs.insert(Coefficient("-11790576996382886")*A
	    - Coefficient("7286977251918799")*C
	    + Coefficient("4503599627370496")*D
	    >= Coefficient("-38155108523624968"));
  cs.insert(Coefficient("-5895288507198642")*A
	    - Coefficient("3643488632714799")*C
	    - Coefficient("2251799813685248")*D
	    >= Coefficient("-19077554291085880"));
  cs.insert(Coefficient("-11790577113476476")*A
	    - Coefficient("4503599627370496")*B
	    + Coefficient("7286977319472793")*D
	    >= Coefficient("-38155108861394936"));
  cs.insert(Coefficient("-5895288572500836")*A
	    - Coefficient("2251799813685248")*B
	    - Coefficient("3643488652980997")*D
	    >= Coefficient("-19077554450963668"));
  cs.insert(Coefficient("-5895288484680644")*A
	    - Coefficient("3643488607945001")*B
	    + Coefficient("2251799813685248")*C
	    >= Coefficient("-19077554212272888"));
  cs.insert(Coefficient("-2947644274991419")*A
	    - Coefficient("1821744320860999")*B
	    - Coefficient("1125899906842624")*C
	    >= Coefficient("-9538777190578936"));
  cs.insert(Coefficient("-2251799813685248")*A
	    >= Coefficient("-5895288448651847"));

  // Cost function
  Linear_Expression cost(10*A + 21*B + 31*C + 45*D);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

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
test06() {
  Variable A(0);
  Variable B(1);
  Linear_Expression cost(A + B);
  Constraint_System cs;
  cs.insert(-A - B >= -8);
  cs.insert(-A - 3*B >= -18);
  cs.insert(-A + B >= -4);
  cs.insert(A >= 0);
  cs.insert(A <= 3);
  cs.insert(B >= 0);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

  const Coefficient num_kr = 8;
  const Coefficient den_kr = 1;
  Coefficient num;
  Coefficient den;

  Generator pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;

  nout << "Optimizing point = ";
  print_generator(pg);
  Generator pg_kr = point(3*A + 5*B);
  if (pg != pg_kr)
    return false;

  // Set variable A to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(A));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set variable B to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(B));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;

  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  return true;
}

bool
test07() {
  Variable A(0);
  Variable B(1);
  Linear_Expression cost(A + B);
  Constraint_System cs;
  cs.insert(-A - 3*B >= -4);
  cs.insert(-5*A - B >= -5);
  cs.insert(-3*A - 2*B >= -2);
  cs.insert(A + 3*B >= -1);
  cs.insert(2*A - B >= -2);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

  Coefficient num_kr = 8;
  Coefficient den_kr = 7;
  Coefficient num;
  Coefficient den;

  Generator pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;

  nout << "Optimizing point = ";
  print_generator(pg);
  Generator pg_kr = point(-2*A + 10*B, 7);
  if (pg != pg_kr)
    return false;

  // Set variable A to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(A));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  num_kr = 1;
  den_kr = 1;
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  pg_kr = point(B);
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set variable B to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(B));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;

  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  return true;
}

bool
test08() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Variable D(3);
  Linear_Expression cost(12*A + 6*B + 4*C + 3*D);

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

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

  Coefficient num_kr = Coefficient("8231960000");
  Coefficient den_kr = 581120267;
  Coefficient num;
  Coefficient den;

  Generator pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  Generator pg_kr = point(679355000*A + 19925000*C + 0*D, 581120267);
  if (pg != pg_kr)
    return false;

  // Set variable A to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(A));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  num_kr = Coefficient("81926256268");
  den_kr = Coefficient("6651564805");
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  pg_kr = point(Coefficient("6651564805")*A + 196469466*B + 232165453*C + 0*D,
		Coefficient("6651564805"));
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set variable B to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(B));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  num_kr = 1646392;
  den_kr = 135871;
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  pg_kr = point(135871*A + 3985*C + 0*D, 135871);
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set variable C to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(C));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  num_kr = 2335041;
  den_kr = 193393;
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  pg_kr = point(193393*A + 4775*D, 193393);
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set variable D to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(D));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  num_kr = 12;
  den_kr = 1;
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  pg_kr = point(A + 0*D);
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  return true;
}

bool
test09() {
  Variable A(0);
  Variable B(1);
  Variable C(2);
  Linear_Expression cost(A + B + C);
  Constraint_System cs;
  cs.insert(A >= -1);
  cs.insert(B >= -1);
  cs.insert(C >= -1);
  cs.insert(-A >= -1);
  cs.insert(-B >= -1);
  cs.insert(-C >= -1);

  MIP_Problem mip = MIP_Problem(cs.space_dimension(), cs, cost, MAXIMIZATION);

  Coefficient num_kr = 3;
  Coefficient den_kr = 1;
  Coefficient num;
  Coefficient den;

  Generator pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);
  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  Generator pg_kr = point(A + B + C);
  if (pg != pg_kr)
    return false;

  // Set variable A to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(A));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set variable B to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(B));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  // Set variable C to be constrained to have an integer value.
  mip.add_to_integer_space_dimensions(Variables_Set(C));
  pg = mip.optimizing_point();
  mip.evaluate_objective_function(pg, num, den);

  nout << "Optimum value = " << num << "/" << den << endl;
  if (num != num_kr || den != den_kr)
    return false;
  nout << "Optimizing point = ";
  print_generator(pg);
  if (pg != pg_kr)
    return false;

  return true;
}

bool
test10() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  // Feasible region.
  Constraint_System cs;
  cs.insert(A + B <= 1);
  cs.insert(A + C <= 1);
  cs.insert(B + C <= 1);

  // All integer variables.
  Variables_Set ivs(A, C);

  // Cost function.
  Linear_Expression cost(-2*A - 3*B - 4*C);

  MIP_Problem mip(cs.space_dimension(), cs.begin(), cs.end(), ivs, cost,
		  MINIMIZATION);

  if (mip.solve() != OPTIMIZED_MIP_PROBLEM)
    return false;

  Generator pg = mip.optimizing_point();
  nout << "Optimizing point = ";
  using namespace Parma_Polyhedra_Library::IO_Operators;
  nout << pg << endl;

  return true;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F64(test01);
  DO_TEST_F32(test02);
  DO_TEST_F64(test03);
  DO_TEST_F32(test04);
  DO_TEST_F64(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST_F64(test08);
  DO_TEST(test09);
  DO_TEST(test10);
END_MAIN
