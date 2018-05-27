/* Test helper class of the Parma Polyhedra Library Java interface.
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

import java.util.Map;

public class PPL_Test {

    static boolean NOISY = false;
    static boolean VERY_NOISY = false;

    // Common initialization.
    public static void initialize() {
        String noisy_value = System.getenv("PPL_NOISY_TESTS");
        String vnoisy_value = System.getenv("PPL_VERY_NOISY_TESTS");
        if (vnoisy_value != null) {
               VERY_NOISY = true;
               NOISY = true;
        }
        if (noisy_value != null)
               NOISY = true;
    }

    public static void print_if_noisy(String str) {
        if (NOISY)
               System.out.print(str);
    }

    public static void println_if_noisy(String str) {
        if (NOISY)
               System.out.println(str);
    }

    public static void print_if_noisy(long l) {
        if (NOISY)
            System.out.print(l);
    }

    public static void println_if_noisy(long l) {
        if (NOISY)
            System.out.println(l);
    }

    public static void println_if_noisy() {
        if (NOISY)
               System.out.println();
    }

    public static void print_if_vnoisy(String str) {
        if (VERY_NOISY)
               System.out.print(str);
    }

    public static void println_if_vnoisy(String str) {
        if (VERY_NOISY)
               System.out.println(str);
    }

    public static void print_if_vnoisy(long l) {
        if (VERY_NOISY)
            System.out.print(l);
    }

    public static void println_if_vnoisy(long l) {
        if (VERY_NOISY)
            System.out.println(l);
    }

    public static void println_if_vnoisy() {
        if (VERY_NOISY)
               System.out.println();
    }
}
