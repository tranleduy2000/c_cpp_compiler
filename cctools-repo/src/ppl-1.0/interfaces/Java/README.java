# Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
# Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.

The Java interface of the Parma Polyhedra Library has been tested
with both Sun's JDK 6 (http://java.sun.com/javase/downloads/)
and the open-source OpenJDK 6 (http://openjdk.java.net/).

The Java interface provides the `parma_polyhedra_library' package:
after installing the Parma Polyhedra Library in <PPL prefix>,
the ppl_java.jar JAR file and the JNI shared object will be installed
in directory <PPL prefix>/<libdir>/ppl.

The Java compilation command for `My_Test.java' should then be
something like:

$ javac -classpath .:<PPL prefix>/<libdir>/ppl/ppl_java.jar My_Test.java

This will produce class file My_Test.class, which can then be executed
by issuing a command like:

$ java -classpath .:<PPL prefix>/<libdir>/ppl/ppl_java.jar My_Test

Note that the source code in My_Test.java should take care of:

a) Load the PPL interface library, by calling `System.load' and
   passing the full path of the dynamic shared object. For instance,
   on a Linux machine and assuming <PPL prefix>=/usr/local/, the call
   will be something like:

     System.load("/usr/local/lib/ppl/libppl_java.so");

b) Make sure that only the intended version(s) of the library has been
   loaded, e.g., by calling static method

     Parma_Polyhedra_Library.version();

c) Starting from PPL version 0.11, before calling any other method from
   other PPL package classes, initialize the Java interface by calling
   the static method

     Parma_Polyhedra_Library.initialize_library();

   When done using the library, finalize it by calling the static method

     Parma_Polyhedra_Library.finalize_library();

   After finalization no other method of the library may be used (except
   for those in class Parma_Polyhedra_Library), unless the library
   is re-initialized by calling initialize_library().
