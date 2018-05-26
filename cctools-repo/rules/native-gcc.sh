build_native_gcc() {
    PKG=gcc
    PKG_VERSION=6.4.0
    PKG_SUBVERSION="-1"
    PKG_URL="http://mirrors-usa.go-parts.com/gcc/releases/gcc-${PKG_VERSION}/${PKG}-${PKG_VERSION}.tar.xz"
    PKG_DESC="The GNU C compiler"
    O_FILE=$SRC_PREFIX/gnu/${PKG}/${PKG}-${PKG_VERSION}.tar.xz
    S_DIR=$src_dir/gcc-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $FUNCNAME && return

    banner "Build $PKG"

    pushd .

if true; then

    if [ ! -f $O_FILE ]; then
	download $PKG_URL $O_FILE
    fi

    unpack $src_dir $O_FILE
    patchsrc $S_DIR $PKG $PKG_VERSION

    if [ "$USE_NATIVE_BUILD" = "yes" ]; then
	fix_bionic_shell $S_DIR
    fi

    cd $S_DIR

    ./contrib/download_prerequisites
fi

#    preparesrc $O_DIR $S_DIR

#    if [ "$BUILD_PIE_COMPILER" = "yes" ]; then
#	cd $S_DIR
#	if [ ! -e ${S_DIR}/.pie_patched ]; then
#	    patch -p1 < ${patch_dir}/${PKG}-${PKG_VERSION}-pie.patch || error "No pie patch found!"
#	    touch ${S_DIR}/.pie_patched
#	fi
#    fi

    mkdir -p $B_DIR
    cd $B_DIR

    local EXTRA_CONF=
    case $TARGET_ARCH in
    aarch64*)
	EXTRA_CONF="--enable-fix-cortex-a53-835769"
	;;
    mips64el*)
	EXTRA_CONF="--with-arch=mips64r6 --disable-fixed-point"
	;;
    x86_64*)
	EXTRA_CONF="--with-arch=x86-64 --with-tune=intel --with-fpmath=sse --with-multilib-list=m32,m64,mx32 --disable-libcilkrts"
	;;
    mips*)
	EXTRA_CONF="--with-arch=mips32 --disable-threads --disable-fixed-point"
	;;
    arm*)
	EXTRA_CONF="--with-arch=armv5te --with-float=soft --with-fpu=vfp"
	;;
    *86*)
	EXTRA_CONF="--disable-libquadmath-support --disable-libcilkrts"
	SYSROOT="/data/data/com.pdaxrom.cctools/root/cctools/i686-linux-android"
	;;
    esac

    if [ "$BUILD_PIE_COMPILER" = "yes" ]; then
	EXTRA_CONF="$EXTRA_CONF --enable-default-pie"
    else
	EXTRA_CONF="$EXTRA_CONF --disable-default-pie"
    fi

#    ac_cv_func_getc_unlocked=no \
#    ac_cv_func_getchar_unlocked=no \
#    ac_cv_func_putc_unlocked=no \
#    ac_cv_func_putchar_unlocked=no \
#    ac_cv_func_getc_unlocked=no \
#    ac_cv_func_getchar_unlocked=no \
#    ac_cv_func_putc_unlocked=no \
#    ac_cv_func_putchar_unlocked=no

    $S_DIR/configure \
	--prefix=$TARGET_DIR \
	--target=$TARGET_ARCH \
	--with-gnu-as \
	--with-gnu-ld \
	--with-as=$(which as) \
	--with-ld=$(which ld) \
	--with-nm=$(which nm) \
	--enable-languages=c,c++,fortran,objc,obj-c++ \
	--disable-libsanitizer \
	--disable-libssp \
	--enable-threads \
	--disable-libmudflap \
	--disable-sjlj-exceptions \
	--disable-shared \
	--disable-tls \
	--disable-libitm \
	--enable-initfini-array \
	--disable-nls \
	--disable-bootstrap \
	--disable-libquadmath \
	--enable-plugins \
	--enable-libgomp \
	--enable-graphite=yes \
	--enable-objc-gc \
	--enable-eh-frame-hdr-for-static \
	--enable-target-optspace \
	--enable-objc-gc \
	--enable-eh-frame-hdr-for-static \
	--enable-target-optspace \
	--disable-libquadmath-support \
	--disable-libcilkrts \
	--with-sysroot=$SYSROOT \
	--program-suffix=-${PKG_VERSION} \
	--with-host-libstdcxx='-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm' \
	$EXTRA_CONF \
	|| error "configure"

#	--with-as=/data/data/com.pdaxrom.cctools/root/cctools/home/bin/as \
#	--with-ld=/data/data/com.pdaxrom.cctools/root/cctools/home/bin/ld \
#	--with-nm=/data/data/com.pdaxrom.cctools/root/cctools/home/bin/nm

#	--with-binutils-version=$binutils_version \
#	--with-mpfr-version=$mpfr_version \
#	--with-mpc-version=$mpc_version \
#	--with-gmp-version=$gmp_version \
#	--with-gcc-version=$gcc_version \
#	--with-cloog-version=$cloog_version \
#	--with-isl-version=$isl_version \

#	--with-gmp=$TMPINST_DIR \
#	--with-mpfr=$TMPINST_DIR \
#	--with-mpc=$TMPINST_DIR \
#	--with-cloog=$TMPINST_DIR \
#	--with-isl=$TMPINST_DIR \
#	--with-ppl=$TMPINST_DIR \

#	--target=$TARGET_ARCH \
#	--host=$TARGET_ARCH \
#	--build=x86_64-linux-gnu \

#	--with-gdb-version=6.6

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    rm -rf ${TMPINST_DIR}/gcc

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/$TARGET_ARCH-gcc-ar-$PKG_VERSION
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/$TARGET_ARCH-gcc-nm-$PKG_VERSION
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/$TARGET_ARCH-gcc-ranlib-$PKG_VERSION
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/$TARGET_ARCH-c++-$PKG_VERSION
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/$TARGET_ARCH-g++-$PKG_VERSION
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/$TARGET_ARCH-gcc-$PKG_VERSION
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/$TARGET_ARCH-gfortran-$PKG_VERSION
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/c++-$PKG_VERSION
    ln -sf g++-${PKG_VERSION} ${TMPINST_DIR}/${PKG}/cctools/bin/c++-$PKG_VERSION

    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/bin/*
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/cc1
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/cc1obj
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/cc1objplus
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/cc1plus
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/collect2
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/f951
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/lto-wrapper
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/lto1
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/install-tools/fixincl
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/liblto_plugin.so.0.0.0
    $TARGET_STRIP ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/$TARGET_ARCH/$PKG_VERSION/plugin/gengtype

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/info
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/man
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share

    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/libiberty.a
    find ${TMPINST_DIR}/${PKG}/cctools/ -name "*.la"  -exec rm -f {} \;
    find ${TMPINST_DIR}/${PKG}/cctools/ -name "*.py"  -exec rm -f {} \;

    rm -rf ${TMPINST_DIR}/gfortran
    rm -rf ${TMPINST_DIR}/gobjc
    rm -rf ${TMPINST_DIR}/libstdc++-dev
    rm -rf ${TMPINST_DIR}/libgcc-dev
    rm -rf ${TMPINST_DIR}/libgfortran-dev
    rm -rf ${TMPINST_DIR}/libobjc-dev

    cd ${TMPINST_DIR}/${PKG}/cctools

    find . -name "*.la" -delete
    find . -name "libgnustl_shared.a" -delete

    find . -name "libsupc++.*" -type f -exec install -D -m644 {} ${TMPINST_DIR}/libstdc++-dev/cctools/${TARGET_ARCH}/{} \; -exec rm -f {} \;

    find . -name "libatomic.*" -type f -exec install -D -m644 {} ${TMPINST_DIR}/libgcc-dev/cctools/${TARGET_ARCH}/{} \; -exec rm -f {} \;
    find . -name "libgomp.*"   -type f -exec install -D -m644 {} ${TMPINST_DIR}/libgcc-dev/cctools/${TARGET_ARCH}/{} \; -exec rm -f {} \;

    find . -name "libgfortran.*" -type f -exec install -D -m644 {} ${TMPINST_DIR}/libgfortran-dev/cctools/${TARGET_ARCH}/{} \; -exec rm -f {} \;

    find . -name "libobjc.*"    -type f -exec install -D -m644 {} ${TMPINST_DIR}/libobjc-dev/cctools/${TARGET_ARCH}/{} \; -exec rm -f {} \;
    find . -name "libobjc_gc.*" -type f -exec install -D -m644 {} ${TMPINST_DIR}/libobjc-dev/cctools/${TARGET_ARCH}/{} \; -exec rm -f {} \;

    cd ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}

    find . -name "crt*.o"      -type f -exec install -D -m644 {} ${TMPINST_DIR}/libgcc-dev/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/{} \; -exec rm -f {} \;
    find . -name "libgcc.*"    -type f -exec install -D -m644 {} ${TMPINST_DIR}/libgcc-dev/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/{} \; -exec rm -f {} \;
    find . -name "libgcov.*"   -type f -exec install -D -m644 {} ${TMPINST_DIR}/libgcc-dev/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/{} \; -exec rm -f {} \;

    find . -name "libgfortranbegin.*" -type f -exec install -D -m644 {} ${TMPINST_DIR}/libgfortran-dev/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/{} \; -exec rm -f {} \;
    find . -name "libcaf_single.*"    -type f -exec install -D -m644 {} ${TMPINST_DIR}/libgfortran-dev/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/{} \; -exec rm -f {} \;

    cd ${TMPINST_DIR}/${PKG}/cctools

    find . -type d -empty -delete

    copysrc ${TMPINST_DIR}/${PKG}/cctools/include/c++ \
		${TMPINST_DIR}/libstdc++-dev/cctools/${TARGET_ARCH}/include/c++

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include/c++

    copysrc ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/include/objc \
		${TMPINST_DIR}/libobjc-dev/cctools/${TARGET_ARCH}/include/objc

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/include/objc

    install -D -m 755 ${TMPINST_DIR}/${PKG}/cctools/bin/gfortran-${PKG_VERSION} ${TMPINST_DIR}/gfortran/cctools/bin/gfortran-${PKG_VERSION}
    install -D -m 755 ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/f951 \
			${TMPINST_DIR}/gfortran/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/f951

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/bin/gfortran-${PKG_VERSION}
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/f951

    install -D -m 755 ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/cc1obj \
			${TMPINST_DIR}/gobjc/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/cc1obj
    install -D -m 755 ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/cc1objplus \
			${TMPINST_DIR}/gobjc/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/cc1objplus

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/cc1obj
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/cc1objplus

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/thumb
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/armv7-a
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/mips-r2

    cat >> ${TMPINST_DIR}/${PKG}/cctools/bin/set-default-compiler-gcc << EOF
#!/system/bin/sh

ln -sf gcc-${PKG_VERSION}  \${CCTOOLSDIR}/bin/gcc
ln -sf gcc-${PKG_VERSION}  \${CCTOOLSDIR}/bin/cc
ln -sf g++-${PKG_VERSION}  \${CCTOOLSDIR}/bin/g++
ln -sf g++-${PKG_VERSION}  \${CCTOOLSDIR}/bin/c++
ln -sf cpp-${PKG_VERSION}  \${CCTOOLSDIR}/bin/cpp
ln -sf gcov-${PKG_VERSION} \${CCTOOLSDIR}/bin/gcov
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/set-default-compiler-gcc

    cat >> ${TMPINST_DIR}/${PKG}/postinst << EOF
#!/system/bin/sh

set-default-compiler-gcc
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/postinst

    cat >> ${TMPINST_DIR}/${PKG}/prerm << EOF
#!/system/bin/sh

test \`readlink \${CCTOOLSDIR}/bin/gcc\`  = "gcc-${PKG_VERSION}"  && rm -f \${CCTOOLSDIR}/bin/gcc
test \`readlink \${CCTOOLSDIR}/bin/cc\`   = "gcc-${PKG_VERSION}"  && rm -f \${CCTOOLSDIR}/bin/cc
test \`readlink \${CCTOOLSDIR}/bin/g++\`  = "g++-${PKG_VERSION}"  && rm -f \${CCTOOLSDIR}/bin/g++
test \`readlink \${CCTOOLSDIR}/bin/c++\`  = "g++-${PKG_VERSION}"  && rm -f \${CCTOOLSDIR}/bin/c++
test \`readlink \${CCTOOLSDIR}/bin/cpp\`  = "cpp-${PKG_VERSION}"  && rm -f \${CCTOOLSDIR}/bin/cpp
test \`readlink \${CCTOOLSDIR}/bin/gcov\` = "gcov-${PKG_VERSION}" && rm -f \${CCTOOLSDIR}/bin/gcov

which set-default-compiler-clang && set-default-compiler-clang
EOF

    chmod 755 ${TMPINST_DIR}/${PKG}/prerm

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "libgcc-dev|libgcc-compact-dev libstdc++-dev|libstdc++-compact-dev"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $FUNCNAME

    PKG="gobjc"
    PKG_DESC="GNU Objective-C and Objective-C++ compilers"

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "gcc libobjc-dev|libobjc-compact-dev"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    PKG="gfortran"
    PKG_DESC="GNU Fortran compiler"

    cat >> ${TMPINST_DIR}/${PKG}/postinst << EOF
#!/system/bin/sh

ln -sf gfortran-${PKG_VERSION}  \${CCTOOLSDIR}/bin/gfortran
ln -sf gfortran-${PKG_VERSION}  \${CCTOOLSDIR}/bin/f77
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/postinst

    cat >> ${TMPINST_DIR}/${PKG}/prerm << EOF
#!/system/bin/sh

test \`readlink \${CCTOOLSDIR}/bin/gfortran\` = "gfortran-${PKG_VERSION}" && rm -f \${CCTOOLSDIR}/bin/gfortran
test \`readlink \${CCTOOLSDIR}/bin/f77\`      = "gfortran-${PKG_VERSION}" && rm -f \${CCTOOLSDIR}/bin/f77
EOF

    chmod 755 ${TMPINST_DIR}/${PKG}/prerm

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "gcc libgfortran-dev|libgfortran-compact-dev"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    #
    # LIBGCC
    #

    PKG="libgcc-dev"
    PKG_DESC="GCC support library (development files)"

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "" "libgcc-compact-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    # Cross package
    local filename="${PKG}-${PKG_ARCH}_${PKG_VERSION}${PKG_SUBVERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-${PKG_ARCH} ${PKG_VERSION}${PKG_SUBVERSION} all "$PKG_DESC"
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    #
    # LIBGCC-COMPACT
    #

    rm -rf ${TMPINST_DIR}/libgcc-compact-dev
    cp -a ${TMPINST_DIR}/${PKG} ${TMPINST_DIR}/libgcc-compact-dev
    PKG="libgcc-compact-dev"
    PKG_DESC="GCC support library, compact version (development files)"

    case $TARGET_ARCH in
    arm*)
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/thumb
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/armv7-a

	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/thumb
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/armv7-a
	;;
    mips*)
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/mips-r2
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/mips-r6

	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/libr2
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/libr6
	;;
    i*86*|x86*)
	;;
    esac

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "" "libgcc-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    #
    # LIBSTDC++
    #

    PKG="libstdc++-dev"
    PKG_DESC="GNU Standard C++ Library v3 (development files)"

    local src_dir="${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/$PKG_VERSION"

    case $TARGET_ARCH in
    aarch64*)
	OFORM="elf64-littleaarch64"
	ANAME="arm64-v8a"
	;;
    mips64el*)
	OFORM="elf64-tradlittlemips"
	ANAME="mips64"
	;;
    x86_64*)
	OFORM="elf64-x86-64"
	ANAME="x86_64"
	;;
    mips*)
	OFORM="elf32-tradlittlemips"
	ANAME="mips"
	;;
    arm*)
	OFORM="elf32-littlearm"
	ANAME="armeabi"
	;;
    i*86*|x86*)
	OFORM="elf32-i386"
	ANAME="x86"
	;;
    esac

    $INSTALL -D -m 644 $src_dir/libs/${ANAME}/libgnustl_shared.so ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libgnustl_shared.so
    $INSTALL -D -m 644 $src_dir/libs/${ANAME}/libgnustl_static.a  ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libgnustl_static.a
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/lib
    ln -sf ../${TARGET_ARCH}/lib/libgnustl_shared.so ${TMPINST_DIR}/${PKG}/cctools/lib/

    cat > ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libstdc++.so <<EOF
OUTPUT_FORMAT($OFORM)
GROUP ( libgnustl_shared.so )
EOF

    cat > ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libstdc++.a <<EOF
OUTPUT_FORMAT($OFORM)
GROUP ( libgnustl_static.a )
EOF

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "" "libstdc++-compact-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    # Cross package
    local filename="${PKG}-${PKG_ARCH}_${PKG_VERSION}${PKG_SUBVERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-${PKG_ARCH} ${PKG_VERSION}${PKG_SUBVERSION} all "$PKG_DESC"
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    #
    # LIBSTDC++-COMPACT
    #

    rm -rf ${TMPINST_DIR}/libstdc++-compact-dev
    cp -a ${TMPINST_DIR}/${PKG} ${TMPINST_DIR}/libstdc++-compact-dev

    PKG="libstdc++-compact-dev"
    PKG_DESC="GNU Standard C++ Library v3, compact version (development files)"

    case $TARGET_ARCH in
    arm*)
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/c++/${PKG_VERSION}/${TARGET_ARCH}/thumb
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/c++/${PKG_VERSION}/${TARGET_ARCH}/armv7-a

	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/thumb
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/armv7-a
	;;
    mips*)
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/c++/${PKG_VERSION}/${TARGET_ARCH}/mips-r2
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/c++/${PKG_VERSION}/${TARGET_ARCH}/mips-r6

	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/libr2
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/libr6
	;;
    i*86*|x86*)
	;;
    esac

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "" "libstdc++-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc


    #
    # LIBGFORTRAN
    #

    PKG="libgfortran-dev"
    PKG_DESC="Runtime library for GNU Fortran applications (development files)"

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "" "libgfortran-compact-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    # Cross package
    local filename="${PKG}-${PKG_ARCH}_${PKG_VERSION}${PKG_SUBVERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-${PKG_ARCH} ${PKG_VERSION}${PKG_SUBVERSION} all "$PKG_DESC"
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    #
    # LIBGFORTRAN-COMPACT
    #

    rm -rf ${TMPINST_DIR}/libgfortran-compact-dev
    cp -a ${TMPINST_DIR}/${PKG} ${TMPINST_DIR}/libgfortran-compact-dev

    PKG="libgfortran-compact-dev"
    PKG_DESC="Runtime library for GNU Fortran applications, compact version (development files)"

    case $TARGET_ARCH in
    arm*)
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/thumb
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/armv7-a

	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/thumb
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/armv7-a
	;;
    mips*)
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/mips-r2
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/mips-r6

	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/libr2
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/libr6
	;;
    i*86*|x86*)
	;;
    esac

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "" "libgfortran-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    #
    # LIBOBJC
    #

    PKG="libobjc-dev"
    PKG_DESC="Runtime library for GNU Objective-C applications (development files)"

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "" "libobjc-compact-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    # Cross package
    local filename="${PKG}-${PKG_ARCH}_${PKG_VERSION}${PKG_SUBVERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-${PKG_ARCH} ${PKG_VERSION}${PKG_SUBVERSION} all "$PKG_DESC"
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    #
    # LIBOBJC-COMPACT
    #

    rm -rf ${TMPINST_DIR}/libobjc-compact-dev
    cp -a ${TMPINST_DIR}/${PKG} ${TMPINST_DIR}/libobjc-compact-dev

    PKG="libobjc-compact-dev"
    PKG_DESC="Runtime library for GNU Objective-C applications, compact version (development files)"

    case $TARGET_ARCH in
    arm*)
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/thumb
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/armv7-a

	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/thumb
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/armv7-a
	;;
    mips*)
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/mips-r2
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/gcc/${TARGET_ARCH}/${PKG_VERSION}/mips-r6

	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/libr2
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/libr6
	;;
    i*86*|x86*)
	;;
    esac

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "" "libobjc-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc
}
