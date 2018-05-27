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

import java.lang.reflect.Method;

public class Test_Executor {

    /*! \brief
      Executes all the static methods of class \p c whose name starts
      with "test". It is expected that these methods have no parameters
      and return a Boolean value.

      \return
      \c true if and only if all the tests defined in class \p c
      return \c true.

      \param c
      The class whose test methods are execute.
    */
    public static boolean executeTests(Class c) {
        PPL_Test.initialize();
	boolean global_test_result_ok = true;
	System.out.println("Checking " + c.getName());
	Method methods[] = c.getDeclaredMethods();
        Object[] no_args = new Object[0];
	for (Method currentMethod:methods) {
	    try {
		if (currentMethod.getName().startsWith("test")) {
		    System.out.println("Executing " + currentMethod.getName());
		    boolean ok = (Boolean) currentMethod.invoke(null, no_args);
		    if (!ok) {
			global_test_result_ok = false;
			System.out.println(currentMethod.getName() + " failed");
		    }
		}
	    }
	    catch (Exception e) {
                System.out.println("Unexpected exception caught.");
                System.out.println(e.getMessage());
		return false;
	    }
	}
	return global_test_result_ok;
    }
}
