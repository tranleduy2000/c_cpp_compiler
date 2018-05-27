/* Weight_Profiler class declaration.
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

#ifndef Weight_Profiler_defs_hh
#define Weight_Profiler_defs_hh 1

#include <cassert>

namespace Parma_Polyhedra_Library {

class Weight_Profiler {
private:
  enum { DISCARDED = 0, VALID = 1 };

public:
  Weight_Profiler(const char* file, int line,
		  Weightwatch_Traits::Delta delta,
		  double min_threshold = 0, double max_threshold = 0)
    : file(file), line(line), delta(delta),
      min_threshold(min_threshold), max_threshold(max_threshold) {
    for (int i = 0; i < 2; ++i) {
      stat[i].samples = 0;
      stat[i].count = 0;
      stat[i].sum = 0;
      stat[i].squares_sum = 0;
      stat[i].min = 0;
      stat[i].max = 0;
    }
  }

  ~Weight_Profiler() {
    output_stats();
  }

  void output_stats();

  static void begin() {
#ifndef NDEBUG
    int r = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stamp);
    assert(r >= 0);
#else
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stamp);
#endif
  }

  void end(unsigned int factor = 1) {
    Weightwatch_Traits::weight
      += (Weightwatch_Traits::Threshold) delta * factor;
    struct timespec start = stamp;
    begin();
    double elapsed;
    if (stamp.tv_nsec >= start.tv_nsec) {
      elapsed = (stamp.tv_nsec - start.tv_nsec)
	+ (stamp.tv_sec - start.tv_sec) * 1e9;
    }
    else {
      elapsed = (1000000000 - start.tv_nsec + stamp.tv_nsec )
	+ (stamp.tv_sec - start.tv_sec - 1) * 1e9;
    }
    elapsed -= adjustment;
    double elapsed1 = elapsed / factor;
    int i = (elapsed1 < min_threshold
             || (max_threshold > 0 && elapsed1 > max_threshold))
      ? DISCARDED
      : VALID;
    ++stat[i].samples;
    if (stat[i].count == 0)
      stat[i].min = stat[i].max = elapsed1;
    else if (stat[i].min > elapsed1)
      stat[i].min = elapsed1;
    else if (stat[i].max < elapsed1)
      stat[i].max = elapsed1;
    stat[i].sum += elapsed;
    stat[i].squares_sum += elapsed * elapsed1;
    stat[i].count += factor;
  }

  static double tune_adjustment();

 private:
  //! File of this profiling point.
  const char *file;

  //! Line of this profiling point.
  int line;

  //! Computational weight to be added at each iteration.
  Weightwatch_Traits::Delta delta;

  //! Times less than this value are discarded.
  double min_threshold;

  //! Times greater than this value are discarded.
  double max_threshold;

  //! Statistical data for samples (both DISCARDED and VALID)
  struct {
    //! Number of collected samples.
    unsigned int samples;

    /*! \brief
      Number of collected iterations.

      \note
      Multiple iterations are possibly collected for each sample.
    */
    unsigned int count;

    //! Sum of the measured times.
    double sum;

    //! Sum of the squares of the measured times (to compute variance).
    double squares_sum;

    //! Minimum measured time.
    double min;

    //! Maximum measured time.
    double max;
  } stat[2];

  //! Holds the time corresponding to last time begin() was called.
  static struct timespec stamp;

  /*! \brief
    Time quantity used to adjust the elapsed times so as not to take
    into account the time spent by the measurement infrastructure.
  */
  static double adjustment;
};

}

#endif // Weight_Profiler_defs_hh
