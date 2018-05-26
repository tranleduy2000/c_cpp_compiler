build_gcc_mingw32() {
    PKG=gcc-mingw-w64
    PKG_VERSION=$gcc_mingw_version
    PKG_URL="http://mirrors-usa.go-parts.com/gcc/releases/gcc-${PKG_VERSION}/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="The GNU C compiler (cross compiler for mingw32)"
    O_FILE=$SRC_PREFIX/gnu/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/gnu/gcc-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}-mingw32-${1}

    c_tag ${PKG}-${1} && return

    banner "Build $PKG"

    pushd .

#    preparesrc $O_DIR $S_DIR

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

    $S_DIR/configure \
	--host=$TARGET_ARCH \
	--build=`uname -m`-unknown-linux-gnu \
	--target=${1} \
	--disable-multilib \
	--disable-win32-registry \
	--enable-version-specific-runtime-libs \
	--prefix=${TARGET_DIR} \
	--enable-languages=c,c++ \
	--disable-nls \
	--with-gmp=$TMPINST_DIR \
	--with-mpfr=$TMPINST_DIR \
	--with-mpc=$TMPINST_DIR \
	--with-cloog=$TMPINST_DIR \
	--with-isl=$TMPINST_DIR \
	--with-ppl=$TMPINST_DIR \
	--disable-ppl-version-check \
	--disable-cloog-version-check \
	--disable-isl-version-check \
	--enable-cloog-backend=isl \
	--with-binutils-version=$binutils_version \
	--with-mpfr-version=$mpfr_version \
	--with-mpc-version=$mpc_version \
	--with-gmp-version=$gmp_version \
	--with-gcc-version=$gcc_mingw_version \
	--with-cloog-version=$cloog_version \
	--with-isl-version=$isl_version \
	--disable-libquadmath-support \
	--disable-libcilkrts \
	|| error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    local WARCH
    local WTARG
    case $1 in
    i*86*)
	WTARG=Win32
	WARCH=${1/-*}
	;;
    *)
	WTARG=Win64
	WARCH=x86-64
	;;
    esac

    $MAKE install prefix=${TMPINST_DIR}/${PKG}-${WARCH}/cctools || error "package install"

    PKG_DESC="GNU C compiler for MinGW-w64 targeting $WTARG"

    rm -rf ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/include
    rm -rf ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/share
    rm -f  ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/lib/libiberty.a

    ln -sf ${1}-g++ ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/bin/${1}-c++
    ln -sf ${1}-gcc-${gcc_mingw_version} ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/bin/${1}-gcc
    ln -sf ${1}-gcc-${gcc_mingw_version} ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/bin/${1}-cc

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/bin/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/libexec/gcc/${1}/${gcc_mingw_version}/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/libexec/gcc/${1}/${gcc_mingw_version}/plugin/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/libexec/gcc/${1}/${gcc_mingw_version}/install-tools/*

    cp -f ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/lib/gcc/${1}/lib/*.a ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/lib/gcc/${1}/${gcc_mingw_version}/
    rm -rf ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/lib/gcc/${1}/lib

    local f
    mkdir -p ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/${1}/dll
    find ${TMPINST_DIR}/${PKG}-${WARCH}/cctools -type f -name "*.dll" | while read f; do
	cp -f $f ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/${1}/dll
	rm -f $f
    done

    find ${TMPINST_DIR}/${PKG}-${WARCH}/cctools -type f -name "*.la" -exec rm -f {} \;

    local filename="${PKG}-${WARCH}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG}-${WARCH} $filename ${PKG}-${WARCH} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}-${WARCH}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag ${PKG}-${1}
}
