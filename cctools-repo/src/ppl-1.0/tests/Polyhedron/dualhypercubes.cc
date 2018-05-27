/* Exploit smf when computing the intersection of NNC dual hypercubes.
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
#include "timings.hh"
#include <vector>
#include <map>

// Define EXP_EVAL to 1 if you want to reproduce the results
// of the experimental evaluation reported in Table 2 of the paper:
//   R. Bagnara, P.M. Hill, E. Zaffanella
//   Not Necessarily Closed Convex Polyhedra and the Double Description Method.
//   Formal Aspects of Computing, 17, 2 (2005), pp. 222-257.
#ifndef EXP_EVAL
#define EXP_EVAL 0
#endif

namespace {

void
closure_points_dual_hypercube(const dimension_type dims,
			      const Linear_Expression& weight_center,
			      const Coefficient& half_diagonal,
			      Generator_System& gs) {
  // An ill-formed (it has no points at all) generator system
  // for a dual hypercube.
  for (dimension_type axis = dims; axis-- > 0; ) {
    gs.insert(closure_point(weight_center + half_diagonal * Variable(axis)));
    gs.insert(closure_point(weight_center - half_diagonal * Variable(axis)));
  }
}

void
add_facets(dimension_type& to_be_added,
	   Generator_System& gs,
	   const Linear_Expression& expr,
	   const dimension_type axis,
	   const dimension_type dims,
	   const Linear_Expression& weight_center,
	   const Coefficient& half_diagonal) {
  // Return if we have already added all facets.
  if (to_be_added == 0)
    return;

  Linear_Expression expr1 = expr;
  expr1 += half_diagonal * Variable(axis);
  Linear_Expression expr2 = expr;
  expr2 -= half_diagonal * Variable(axis);

  if (axis == 0) {
    gs.insert(point(dims * weight_center + expr1, dims));
    --to_be_added;
    if (to_be_added == 0)
      return;
    gs.insert(point(dims * weight_center + expr2, dims));
    --to_be_added;
    return;
  }

  // Here axis > 0.
  // First recursive call with variable with index `axis'
  // having coordinate 1/dims.
  add_facets(to_be_added, gs, expr1,
	     axis-1, dims, weight_center, half_diagonal);
  if (to_be_added == 0)
    return;
  // Second recursive call with variable with index `axis'
  // having coordinate -1/dims.
  add_facets(to_be_added, gs, expr2,
	     axis-1, dims, weight_center, half_diagonal);
}

NNC_Polyhedron
NNC_dual_hypercube(const dimension_type dims,
		   const Linear_Expression& weight_center,
		   const Coefficient& half_diagonal,
		   const int facet_percentage) {
  Generator_System gs;
  closure_points_dual_hypercube(dims, weight_center, half_diagonal, gs);
  // Number of facets in the closed dual hypercube.
  dimension_type num_facets = 1;
  for (dimension_type axis = dims; axis-- > 0; )
    num_facets *= 2;
  dimension_type facets_to_be_added = (num_facets * facet_percentage) / 100;
  if (facets_to_be_added == 0)
    // There has to be a point, at least.
    gs.insert(point(weight_center));
  else
    add_facets(facets_to_be_added, gs, Linear_Expression(0),
	       dims-1, dims, weight_center, half_diagonal);
  // Actually build the polyhedron.
  return NNC_Polyhedron(gs);
}

void
build_polyhedra(const dimension_type dims,
		const int percentage,
		std::vector<NNC_Polyhedron>& ph) {

  Linear_Expression weight_center;

  // 1st-polyhedron.
  weight_center = Linear_Expression(0);
  for (dimension_type axis = dims; axis-- > 0; )
    weight_center += Variable(axis);
  ph.push_back(NNC_dual_hypercube(dims, weight_center, 5, percentage));

  // 2nd-polyhedron.
  weight_center = Linear_Expression(0);
  for (dimension_type axis = dims; axis-- > 0; )
    weight_center += 2*Variable(axis);
  ph.push_back(NNC_dual_hypercube(dims, weight_center, 4, percentage));

  // 3rd-polyhedron.
  weight_center = Linear_Expression(0);
  for (dimension_type axis = dims; axis-- > 0; )
    if (axis % 2 == 0)
      weight_center += 10*Variable(axis);
    else
      weight_center += 2*Variable(axis);
  ph.push_back(NNC_dual_hypercube(dims, weight_center, 5, percentage));

  // 4th-polyhedron.
  weight_center = Linear_Expression(0);
  for (dimension_type axis = dims; axis-- > 0; )
    if (axis % 2 == 0)
      weight_center += 10*Variable(axis);
    else
      weight_center += Variable(axis);
  ph.push_back(NNC_dual_hypercube(dims, weight_center, 4, percentage));
}

long
computation(std::vector<NNC_Polyhedron>& ph, bool enhanced) {
  nout << endl;
  if (enhanced)
    nout << "Enhanced computation: ";
  else
    nout << "Standard computation: ";
  nout << "working with 4 NNC dual hypercubes of dimension "
       << ph[0].space_dimension() << endl;

  start_clock();

  /**** Compute the intersection of ph[0] and ph[1]. ****/

  // Print cardinalities of arguments.
  nout << "  - Computing intersection of ph[0] and ph[1]:" << endl;
  const Generator_System& gs_0 = ph[0].generators();
  nout << "    # ph[0].generators() = "
       << std::distance(gs_0.begin(), gs_0.end()) << endl;
  const Generator_System& gs_1 = ph[1].generators();
  nout << "    # ph[1].generators() = "
       << std::distance(gs_1.begin(), gs_1.end()) << endl;
  // Very noisy dump of arguments.
  vnout << "*** ph[0] generators ***" << endl;
  gs_0.ascii_dump(vnout);
  vnout << "*** ph[1] generators ***" << endl;
  gs_1.ascii_dump(vnout);
  vnout << endl;

  const Constraint_System& cs_0 = enhanced
    ? ph[0].minimized_constraints()
    : ph[0].constraints();
  const Constraint_System& cs_1 = enhanced
    ? ph[1].minimized_constraints()
    : ph[1].constraints();

  // Print cardinalities of constraint systems.
  nout << "    # ph[0].constraints() = "
       << std::distance(cs_0.begin(), cs_0.end()) << endl;
  nout << "    # ph[1].constraints() = "
       << std::distance(cs_1.begin(), cs_1.end()) << endl;
  // Very noisy dump of arguments.
  vnout << "*** ph[0] constraints ***" << endl;
  cs_0.ascii_dump(vnout);
  vnout << "*** ph[1] constraints ***" << endl;
  cs_1.ascii_dump(vnout);
  vnout << endl;

  ph[0].intersection_assign(ph[1]);

  /**** Compute the intersection of ph[2] and ph[3]. ****/

  // Print cardinalities of arguments.
  nout << "  - Computing intersection of ph[2] and ph[3]:" << endl;
  const Generator_System& gs_2 = ph[2].generators();
  nout << "    # ph[2].generators() = "
       << std::distance(gs_2.begin(), gs_2.end()) << endl;
  const Generator_System& gs_3 = ph[3].generators();
  nout << "    # ph[3].generators() = "
       << std::distance(gs_3.begin(), gs_3.end()) << endl;
  // Very noisy dump of arguments.
  vnout << "*** ph[2] generators ***" << endl;
  gs_2.ascii_dump(vnout);
  vnout << "*** ph[3] generators ***" << endl;
  gs_3.ascii_dump(vnout);
  vnout << endl;

  const Constraint_System& cs_2 = enhanced
    ? ph[2].minimized_constraints()
    : ph[2].constraints();
  const Constraint_System& cs_3 = enhanced
    ? ph[3].minimized_constraints()
    : ph[3].constraints();

  // Print cardinalities of constraint systems.
  nout << "    # ph[2].constraints() = "
       << std::distance(cs_2.begin(), cs_2.end()) << endl;
  nout << "    # ph[3].constraints() = "
       << std::distance(cs_3.begin(), cs_3.end()) << endl;
  // Very noisy dump of arguments.
  vnout << "*** ph[2] constraints ***" << endl;
  cs_2.ascii_dump(vnout);
  vnout << "*** ph[3] constraints ***" << endl;
  cs_3.ascii_dump(vnout);
  vnout << endl;

  ph[2].intersection_assign(ph[3]);

  /**** Compute the poly-hull of ph[0] and ph[2]. ****/

  const Generator_System& gs_01 = enhanced
    ? ph[0].minimized_generators()
    : ph[0].generators();
  const Generator_System& gs_23 = enhanced
    ? ph[2].minimized_generators()
    : ph[2].generators();

  // Print cardinalities of arguments.
  nout << "  - Computing poly-hull of ph[0] and ph[2]:" << endl;
  nout << "    # ph[0].generators() = "
       << std::distance(gs_01.begin(), gs_01.end()) << endl;
  nout << "    # ph[2].generators() = "
       << std::distance(gs_23.begin(), gs_23.end()) << endl;
  // Very noisy dump of arguments.
  vnout << "*** ph[0] generators ***" << endl;
  gs_01.ascii_dump(vnout);
  vnout << "*** ph[2] generators ***" << endl;
  gs_23.ascii_dump(vnout);
  vnout << endl;

  ph[0].upper_bound_assign(ph[2]);

  /**** Final conversion ****/

  const Constraint_System& cs = ph[0].constraints();

  nout << "Wmf final result timing: ";
  print_clock(nout);
  nout << endl;

  // How many constraints obtained?
  const long cs_cardinality = std::distance(cs.begin(), cs.end());

  // Print cardinality of weakly-minimized final result.
  nout << "  - Final (wmf) result is ph[0]:" << endl;
  nout << "    # ph[0].constraints() = " << cs_cardinality << endl;
  // Very noisy dump of weakly-minimized final result.
  vnout << "*** ph[0] constraints ***" << endl;
  cs.ascii_dump(vnout);
  vnout << endl;

  /**** Final strong minimization ****/

  nout << "Smf (cons) final result timing: ";
  start_clock();
  const Constraint_System& min_cs = ph[0].minimized_constraints();
  print_clock(nout);
  nout << endl;

  // How many constraints obtained?
  const long min_cs_cardinality = std::distance(min_cs.begin(), min_cs.end());

  // Print cardinality of strongly-minimized final result.
  nout << "  - Final (smf) result is ph[0]:" << endl;
  nout << "    # ph[0].minimized_constraints() = "
       << min_cs_cardinality << endl;
  // Very noisy dump of strongly-minimized final result.
  vnout << "*** ph[0] minimized constraints ***" << endl;
  min_cs.ascii_dump(vnout);
  vnout << endl;

  return enhanced ? min_cs_cardinality : cs_cardinality;
}

bool
test01() {
  std::vector<NNC_Polyhedron> ph;

#if EXP_EVAL
  dimension_type first_dim = 4;
  dimension_type last_dim = 5;
#else
  dimension_type first_dim = 2;
  dimension_type last_dim = 4;
#endif

  // Storing cardinalities of known results.
  // NOTE: the numbers reported here differ a little bit from those
  // in the FAC paper in that here we do not count low-level constraints
  // related to the epsilon dimension. The difference is at most 2
  // (the eps_geq_zero and eps_leq_one constraints).
  typedef std::map<std::pair<dimension_type, int>, long> My_Map;
  My_Map::const_iterator known_result;
  My_Map standard_cardinalities;
  My_Map enhanced_cardinalities;

  using std::make_pair;

  standard_cardinalities[make_pair(4, 25)] = 331;  // FAC  332
  enhanced_cardinalities[make_pair(4, 25)] =  31;  // FAC   33

  standard_cardinalities[make_pair(4, 50)] = 519;  // FAC  520
  enhanced_cardinalities[make_pair(4, 50)] =  41;  // FAC   43

  standard_cardinalities[make_pair(5, 25)] = 2692; // FAC 2693
  enhanced_cardinalities[make_pair(5, 25)] =  125; // FAC  127

  standard_cardinalities[make_pair(5, 50)] = 4993; // FAC 4994
  enhanced_cardinalities[make_pair(5, 50)] =  150; // FAC  152

  int num_errors = 0;

  for (dimension_type dims = first_dim; dims <= last_dim; dims++)
    for (int percentage = 25; percentage <= 50; percentage += 25) {

      nout << endl
	   << "++++++++ DIMENSIONS = " << dims << "  ++++++++"
	   << endl
	   << "++++++++ PERCENTAGE = " << percentage << " ++++++++"
	   << endl;

      // Standard evaluation strategy.
      ph.clear();
      build_polyhedra(dims, percentage, ph);
      const long standard_eval_card = computation(ph, false);

      // Check if there is a known result.
      known_result = standard_cardinalities.find(make_pair(dims, percentage));
      if (known_result != standard_cardinalities.end()
	  && known_result->second != standard_eval_card) {
	++num_errors;
	nout << "Cardinality mismatch: "
	     << "expected " << known_result->second << ", "
	     << "obtained " << standard_eval_card << ".\n";
      }

      // Enhanced evaluation strategy.
      ph.clear();
      build_polyhedra(dims, percentage, ph);
      const long enhanced_eval_card = computation(ph, true);

      // Check if there is a known result.
      known_result = enhanced_cardinalities.find(make_pair(dims, percentage));
      if (known_result != enhanced_cardinalities.end()
	  && known_result->second != enhanced_eval_card) {
	++num_errors;
	nout << "Cardinality mismatch: "
	     << "expected " << known_result->second << ", "
	     << "obtained " << enhanced_eval_card <<".\n";
      }

    }
  return num_errors == 0;
}

} // namespace

BEGIN_MAIN
  DO_TEST_F64A(test01);
END_MAIN
