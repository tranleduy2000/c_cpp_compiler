/* Test operator<<(std::ostream& s, Variable v)
   and the related machinery.
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
#include "files.hh"
#include <string>
#include <fstream>

using std::string;
using std::fstream;
using std::ios_base;

using namespace Parma_Polyhedra_Library::IO_Operators;

namespace {

const char* my_file = "writevariable1.dat";

void
write_variables() {
  fstream f;
  open(f, my_file, ios_base::out);
  for (dimension_type id = 0; id <= 100; id += 5)
    f << Variable(id) << " ";
  f << endl;
  close(f);
}

void
read_variables_and_check(const string& s) {
  fstream f;
  open(f, my_file, ios_base::in);
  string t;
  getline(f, t);
  close(f);
  if (s != t)
    exit(1);
}

void
my_output_function(std::ostream& s, const Variable v) {
  s << "x" << v.id();
}

} // namespace

int
main() TRY {
  set_handlers();

  // Default output function: write...
  write_variables();
  // ... read back.
  read_variables_and_check("A F K P U Z E1 J1 O1 T1 Y1 "
			   "D2 I2 N2 S2 X2 C3 H3 M3 R3 W3 ");

  // Save the default output function.
  Variable::output_function_type* p_default_output_function
    = Variable::get_output_function();

  // Install an alternate output function.
  Variable::set_output_function(my_output_function);

  // Check that the installation worked as expected.
  if (Variable::get_output_function() != my_output_function)
    return 1;

  // Alternate output function: write...
  write_variables();
  // ... read back.
  read_variables_and_check("x0 x5 x10 x15 x20 x25 x30 x35 x40 x45 "
			   "x50 x55 x60 x65 x70 x75 x80 x85 x90 x95 x100 ");

  // Restore the default output function.
  Variable::set_output_function(p_default_output_function);

  // Check that the restoration worked as expected.
  if (Variable::get_output_function() != p_default_output_function)
    return 1;

  return 0;
}
CATCH
