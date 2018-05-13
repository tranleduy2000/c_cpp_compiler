build_gcc_mingw32_host() {
    PKG=gcc
    PKG_VERSION=$gcc_version
    PKG_DESC="The GNU C compiler (cross compiler for mingw32)"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}-mingw32-${1}

    c_tag ${PKG}-mingw32-host-${1} && return

    banner "Build $PKG mingw32 host"

    pushd .

    preparesrc $O_DIR $S_DIR

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

#    --enable-targets=i686-w64-mingw32,x86_64-w64-mingw32

    $S_DIR/configure \
	--target=${1} \
	--disable-multilib \
	--disable-win32-registry \
	--enable-version-specific-runtime-libs \
	--prefix=${TARGET_DIR}-host \
	--enable-languages=c,c++ \
	--disable-nls \
	--with-gmp=${TARGET_DIR}-host \
	--with-mpfr=${TARGET_DIR}-host \
	--with-mpc=${TARGET_DIR}-host \
	--with-cloog=${TARGET_DIR}-host \
	--with-isl=${TARGET_DIR}-host \
	--with-ppl=${TARGET_DIR}-host \
	--disable-ppl-version-check \
	--disable-cloog-version-check \
	--disable-isl-version-check \
	--enable-cloog-backend=isl \
	--with-binutils-version=$binutils_version \
	--with-mpfr-version=$mpfr_version \
	--with-mpc-version=$mpc_version \
	--with-gmp-version=$gmp_version \
	--with-gcc-version=$gcc_version \
	--with-cloog-version=$cloog_version \
	--with-isl-version=$isl_version \
	|| error "configure"

#	--enable-multilib \
#	--enable-64bit \
#	--disable-libstdc__-v3 \
#	--with-gxx-include-dir=${TARGET_DIR}-host/${1}/include/c++/${gcc_version} \

    $MAKE $MAKEARGS all-gcc || error "make all-gcc"

    $MAKE install-gcc || error "make install-gcc"

    build_mingw_w64 ${1}

    PKG=gcc

    $MAKE $MAKEARGS || error "make $MAKEARGS"
    $MAKE install || error "make install"

    cp  -f ${TARGET_DIR}-host/lib/gcc/${1}/lib/libgcc_s.a ${TARGET_DIR}-host/lib/gcc/${1}/${gcc_version}/
    rm -rf ${TARGET_DIR}-host/lib/gcc/${1}/lib

#    mkdir libstdc++-v3-build
#    cd libstdc++-v3-build
#    $S_DIR/libstdc++-v3/configure \
#	--enable-version-specific-runtime-libs \
#	--host=${1} \
#	--target=${1} \
#	--prefix=${TARGET_DIR}-host \
#	--bindir=${TARGET_DIR}-host/${1}/bin \
#    || error "configure libstdc++-v3"
#
#    $MAKE $MAKEARGS || error "make libstdc++-v3"
#    $MAKE install || error "make libstdc++-v3 install"
#
#    local WARCH
#    local WTARG
#    case $WARCH in
#    i*86*)
#	WTARG=Win32
#	WARCH=${1/-*}
#	;;
#    *)
#	WTARG=Win64
#	WARCH=x86-64
#	;;
#    esac
#
#    $MAKE install prefix=${TMPINST_DIR}/${PKG}-mingw-w64-${WARCH}/cctools/ || error "prepackage install"

    popd
    s_tag ${PKG}-mingw32-host-${1}
}
