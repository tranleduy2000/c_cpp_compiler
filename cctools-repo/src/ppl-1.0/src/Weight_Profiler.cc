/* Weight_Profiler class implementation.
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

#include "ppl-config.h"
#include "globals.defs.hh"

#if PPL_PROFILE_ADD_WEIGHT
#include <iostream>
#include <cmath>
#include "Weight_Profiler.defs.hh"

namespace Parma_Polyhedra_Library {

void Weight_Profiler::output_stats() {
  std::cout << file << ":" << line << ": Weight_Profiler ";
  if (stat[VALID].samples == 0 && stat[DISCARDED].samples == 0)
    std::cout << "never reached.";
  else {
    if (stat[VALID].samples > 0) {
      double average
        = stat[VALID].sum / stat[VALID].count;
      double variance
        = stat[VALID].squares_sum / stat[VALID].count - average * average;
      std::cout << " samples(" << stat[VALID].samples << ")"
		<< " count(" << stat[VALID].count << ")"
		<< " min( " << stat[VALID].min << ")"
		<< " max( " << stat[VALID].max << ")"
		<< " average(" << average << ")";
      std::cout << " variance( " << variance << ")"
		<< " stddev( " << sqrt(variance) << ")";
    }
    if (stat[DISCARDED].samples > 0) {
      std::cout << " min_threshold( " << min_threshold << ")"
		<< " max_threshold( " << max_threshold << ")";
      double average = stat[DISCARDED].sum / stat[DISCARDED].count;
      std::cout << " samples(" << stat[DISCARDED].samples << ")"
		<< " count(" << stat[DISCARDED].count << ")"
		<< " min( " << stat[DISCARDED].min << ")"
		<< " max( " << stat[DISCARDED].max << ")"
		<< " average(" << average << ")";
    }
  }
  std::cout << std::endl;
}

double Weight_Profiler::tune_adjustment() {
  begin();
  adjustment = 0;
  static Weight_Profiler weight_profiler(__FILE__, __LINE__, 0, 0, 0);
  for (int i = 0; i < 1000; ++i)
    weight_profiler.end(1);
  return weight_profiler.stat[VALID].min;
}

struct timespec Weight_Profiler::stamp;
double Weight_Profiler::adjustment = Weight_Profiler::tune_adjustment();

} // namespace Parma_Polyhedra_Library

#endif
