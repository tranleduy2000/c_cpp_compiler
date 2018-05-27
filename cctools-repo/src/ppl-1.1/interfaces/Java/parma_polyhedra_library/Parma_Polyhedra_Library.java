/* Parma_Polyhedra_Library Java class declaration and implementation.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

/*!
  \defgroup PPL_Java_interface Java Language Interface

  The Parma Polyhedra Library comes equipped with an interface for the
  Java language.
*/

/*! \mainpage
  The Parma Polyhedra Library comes equipped with an interface for the
  Java language.
  The Java interface provides access to the numerical abstractions
  (convex polyhedra, BD shapes, octagonal shapes, etc.) implemented
  by the PPL library.
  A general introduction to the numerical abstractions,
  their representation in the PPL and the operations provided
  by the PPL is given in the main \extref{preamble, PPL user manual}.
  Here we just describe those aspects that are specific to the Java interface.
  In the sequel, <CODE>prefix</CODE> is the path prefix under which
  the library has been installed (typically <CODE>/usr</CODE> or
  <CODE>/usr/local</CODE>).

  <H2>Overview</H2>

  Here is a list of notes with general information and advice
  on the use of the Java interface.

  - When the Parma Polyhedra Library is configured, it will automatically
    test for the existence of the Java system (unless configuration options
    are passed to disable the build of the Java interface;
    see configuration option <CODE>--enable-interfaces</CODE>).
    If Java is correctly installed in a standard location, things will be
    arranged so that the Java interface is built and installed
    (see configuration option <CODE>--with-java</CODE> if you need to
    specify a non-standard location for the Java system).

  - The Java interface files are all installed in the directory
    <CODE>prefix/lib/ppl</CODE>.  Since this includes shared and
    dynamically loaded libraries, you must make your dynamic
    linker/loader aware of this fact.  If you use a GNU/Linux system,
    try the commands <CODE>man ld.so</CODE> and <CODE>man ldconfig</CODE>
    for more information.

  - Any application using the PPL should:
      - Load the PPL interface library by calling <CODE>System.load</CODE>
        and passing the full path of the dynamic shared object;
      - Make sure that only the intended version(s) of the library has
        been loaded, e.g., by calling static method <CODE>version()</CODE>
        in class <CODE>parma_polyhedra_library.Parma_Polyhedra_Library</CODE>;
      - Starting from version 0.11, initialize the interface by calling
        static method <CODE>initialize_library()</CODE>;
        when all library work is done, finalize the interface by calling
        <CODE>finalize_library()</CODE>.

  - The numerical abstract domains available to the Java user as
    Java classes consist of the <EM>simple</EM> domains,
    <EM>powersets</EM> of a simple domain and <EM>products</EM>
    of simple domains. Note that the default configuration will
    only enable a subset of these domains (if you need a different
    set of domains, see configuration option
    <CODE>--enable-instantiations</CODE>).
    - The simple domains are:
      - convex polyhedra, which consist of C_Polyhedron and
        NNC_Polyhedron;<BR>
      - weakly relational, which consist of BD_Shape_N and
        Octagonal_Shape_N
        where N is one of the numeric types
        signed_char, short, int, long, long_long,
        mpz_class, mpq_class;<BR>
      - boxes which consist of
        Int8_Box, Int16_Box, Int32_Box, Int64_Box,
        Uint8_Box, Uint16_Box, Uint32_Box, Uint64_Box,
        Float_Box, Double_Box, Long_Double_Box,
        Z_Box, Rational_Box; and<BR>
      - the Grid domain.
    - The powerset domains are Pointset_Powerset_S where S is
      a simple domain.
    - The product domains consist of
      Direct_Product_S_T,
      Smash_Product_S_T and
      Constraints_Product_S_T where S
      and T are simple domains.

  - In the following, any of the above numerical
    abstract domains  is called a PPL <EM>domain</EM>
    and any element of a PPL domain is called a PPL <EM>object</EM>.

  - A Java program can create a new object for a PPL domain by
    using the constructors for the class corresponding to the domain.

  - For a PPL object with space dimension \p k,
    the identifiers used for the PPL variables
    must lie between 0 and \f$k-1\f$ and correspond to the indices of the
    associated Cartesian axes.
    For example, when using methods that combine PPL polyhedra
    or add constraints or generators to a representation of
    a PPL polyhedron,
    the polyhedra referenced and any constraints or generators in the call
    should follow all the (space) dimension-compatibility rules stated in
    Section \extref{representation, Representations of Convex Polyhedra}
    of the main PPL user manual.

  - As explained above, a polyhedron has a fixed topology C or NNC,
    that is determined at the time of its initialization.
    All subsequent operations on the polyhedron must respect all the
    topological compatibility rules stated in Section
    \extref{representation, Representations of Convex Polyhedra}
    of the main PPL user manual.

  - A <EM>system</EM> of constraints (i.e., an instance of class
    <CODE>parma_polyhedra_library.Constraint_System</CODE>)
    is implemented by extending class <CODE>java.util.ArrayList</CODE>
    (note: <CODE>java.util.Vector</CODE> was used up to version 1.0.)
    As a consequence, it is possible to iterate over the constraints
    in the system by using corresponding inherited methods. Similarly,
    it is possible to modify a system of constraints by using methods
    such as <CODE>add</CODE>; be warned, however, that the constraint
    system obtained from a PPL object is a <EM>copy</EM> of the (C++)
    data structure used in the object itself: any modification will not
    directly affect the original PPL object; clearly, the modified
    constraint system can be used with appropriate methods to,
    e.g., build a new PPL object or modify an existing one.
    The same observations apply to systems of congruences and
    systems of (grid) generators.

*/ /* \mainpage */

/*! \brief The PPL Java interface package.

  \ingroup PPL_Java_interface
  All classes, interfaces and enums related to the Parma Polyhedra Library
  Java interface are included in this package.
*/
package parma_polyhedra_library;

//! A class collecting library-level functions.
/*! \ingroup PPL_Java_interface */
public class Parma_Polyhedra_Library {

    //! \name Library initialization and finalization
    //@{

    /*! \brief
      Initializes the Parma Polyhedra Library.

      This method must be called after loading the library and
      before calling any other method from any other PPL package class.
    */
    public static native void initialize_library();

    /*! \brief
      Finalizes the Parma Polyhedra Library.

      This method must be called when work with the library is done.
      After finalization, no other library method can be called
      (except those in class Parma_Polyhedra_Library), unless the library
      is re-initialized by calling <CODE>initialize_library()</CODE>.
    */
    public static native void finalize_library();

    //@} // Library initialization and finalization

    //! \name Version Checking
    //@{

    //! Returns the major number of the PPL version.
    public static native int version_major();

    //! Returns the minor number of the PPL version.
    public static native int version_minor();

    //! Returns the revision number of the PPL version.
    public static native int version_revision();

    //! Returns the beta number of the PPL version.
    public static native int version_beta();

    //! Returns a string containing the PPL version.
    public static native String version();

    //! Returns a string containing the PPL banner.
    /*!
      The banner provides information about the PPL version, the licensing,
      the lack of any warranty whatsoever, the C++ compiler used to build
      the library, where to report bugs and where to look for further
      information.
    */
    public static native String banner();

    //@} // Version Checking

    //! \name Floating-point rounding and precision settings.
    //@{

    /*! \brief
      Sets the FPU rounding mode so that the PPL abstractions based on
      floating point numbers work correctly.

      This is performed automatically at initialization-time.  Calling
      this function is needed only if restore_pre_PPL_rounding() has been
      previously called.
    */
    public static native void set_rounding_for_PPL();

    /*! \brief
      Sets the FPU rounding mode as it was before initialization of the PPL.

      After calling this function it is absolutely necessary to call
      set_rounding_for_PPL() before using any PPL abstractions based on
      floating point numbers.
      This is performed automatically at finalization-time.
    */
    public static native void restore_pre_PPL_rounding();

    //! Returns the precision parameter for irrational calculations.
    public static native int irrational_precision();

    //! Sets the precision parameter used for irrational calculations.
    /*!
      If \p p is less than or equal to <CODE>INT_MAX</CODE>, sets the
      precision parameter used for irrational calculations to \p p.
      Then, in the irrational calculations returning an unbounded rational,
      (e.g., when computing a square root), the lesser between numerator
      and denominator will be limited to 2**\p p.
    */
    public static native void set_irrational_precision(int p);

    //@} // Floating-point rounding and precision settings.

    //! \name Timeout handling
    //@{

    /*! \brief
      Sets the timeout for computations whose completion could require
      an exponential amount of time.

      \param csecs
      The number of centiseconds sometimes after which a timeout will occur;
      it must be strictly greater than zero.

      Computations taking exponential time will be interrupted some time
      after \p csecs centiseconds have elapsed since the call to
      the timeout setting function, by throwing a
      <CODE>Timeout_Exception</CODE> object.
      Otherwise, if the computation completes without being interrupted,
      then the timeout should be reset by calling
      <CODE>reset_timeout()</CODE>.
    */
    public static native void set_timeout(int csecs);

    /*! \brief
      Resets the timeout time so that the computation is not interrupted.
    */
    public static native void reset_timeout();

    /*! \brief
      Sets a threshold for computations whose completion could require
      an exponential amount of time.

      If \p unscaled_weight has value \f$u\f$ and \p scale has value \f$s\f$,
      then the (scaled) weight threshold is computed as \f$w = u \cdot 2^s\f$.
      Computations taking exponential time will be interrupted some time
      after reaching the complexity threshold \f$w\f$, by throwing a
      <CODE>Timeout_Exception</CODE> object.
      Otherwise, if the computation completes without being interrupted,
      then the deterministic timeout should be reset by calling
      <CODE>reset_deterministic_timeout()</CODE>.

      \param unscaled_weight
      The unscaled maximum computational weight; it has to be strictly
      greater than zero.

      \param scale
      The scaling factor to be applied to \p unscaled_weight; it has
      to be non-negative.

      \exception Invalid_Argument_Exception
      Thrown if the computation of the weight threshold exceeds the
      maximum allowed value.

      \note
      This "timeout" checking functionality is said to be \e deterministic
      because it is not based on actual elapsed time. Its behavior will
      only depend on (some of the) computations performed in the PPL library
      and it will be otherwise independent from the computation environment
      (CPU, operating system, compiler, etc.).

      \warning
      The weight mechanism is under beta testing. In particular,
      there is still no clear relation between the weight threshold and
      the actual computational complexity. As a consequence, client
      applications should be ready to reconsider the tuning of these
      weight thresholds when upgrading to newer version of the PPL.
    */
    public static native void set_deterministic_timeout(int unscaled_weight,
                                                        int scale);

    /*! \brief
      Resets the deterministic timeout so that the computation is not
      interrupted.
    */
    public static native void reset_deterministic_timeout();

    //@} // Timeout handling.
}
