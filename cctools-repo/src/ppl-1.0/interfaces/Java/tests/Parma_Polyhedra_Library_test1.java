/* Parma_Polyhedra_Library Java test class of the
   Parma Polyhedra Library Java interface.
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

import java.math.BigInteger;
import java.util.Iterator;
import java.util.Vector;
import parma_polyhedra_library.*;


public class Parma_Polyhedra_Library_test1 {
static {
    try {
        System.loadLibrary("ppl_java");
    }
    catch (UnsatisfiedLinkError  e) {
        System.out.println("Unable to load the library");
        System.out.println(e.getMessage());
        System.exit(-1);
    }
}

    // This code tests the Parma_Polyhedra_Library methods.
    public static boolean test01() {

    PPL_Test.print_if_noisy("Version Major: ");
    PPL_Test.println_if_noisy(Parma_Polyhedra_Library.version_major());
    PPL_Test.print_if_noisy("Version Minor: ");
    PPL_Test.println_if_noisy(Parma_Polyhedra_Library.version_minor());
    PPL_Test.print_if_noisy("Version Revision: ");
    PPL_Test.println_if_noisy(Parma_Polyhedra_Library.version_revision());
    PPL_Test.print_if_noisy("Version Beta: ");
    PPL_Test.println_if_noisy(Parma_Polyhedra_Library.version_beta());
    PPL_Test.print_if_noisy("Version: ");
    PPL_Test.println_if_noisy(Parma_Polyhedra_Library.version());
    PPL_Test.print_if_vnoisy("Banner: ");
    PPL_Test.println_if_vnoisy(Parma_Polyhedra_Library.banner());
    Parma_Polyhedra_Library.set_rounding_for_PPL();
    Parma_Polyhedra_Library.restore_pre_PPL_rounding();
    PPL_Test.print_if_noisy("Irrational precision: ");
    PPL_Test.println_if_noisy(Parma_Polyhedra_Library.irrational_precision());

    return true;
    }


    public static void main(String[] args) {
        Parma_Polyhedra_Library.initialize_library();
	boolean test_result_ok =
	    Test_Executor.executeTests(Parma_Polyhedra_Library_test1.class);
        Parma_Polyhedra_Library.finalize_library();
	if (!test_result_ok)
	    System.exit(1);
	System.exit(0);
    }
}
