build_gcc_avr() {
    PKG=gcc-avr
    PKG_VERSION=$gcc_avr_version
    PKG_SUBVERSION="-2"
    PKG_DESC="The GNU C compiler (cross compiler for avr)"
    O_DIR=$SRC_PREFIX/gcc/gcc-${PKG_VERSION}
    S_DIR=$src_dir/gcc-${PKG_VERSION}
    B_DIR=$build_dir/$PKG

    c_tag ${PKG} && return

    banner "Build $PKG"

    pushd .

    preparesrc $O_DIR $S_DIR

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

    $S_DIR/configure \
	--target=avr \
	--host=$TARGET_ARCH \
	--prefix=$TARGET_DIR \
	--build=`uname -m`-unknown-linux-gnu \
	--enable-languages=c,c++ \
	--with-gmp=$TMPINST_DIR \
	--with-mpfr=$TMPINST_DIR \
	--with-mpc=$TMPINST_DIR \
	--enable-long-long \
	--disable-libssp \
	--disable-nls \
	--disable-shared \
	--with-binutils-version=$binutils_version \
	--with-mpfr-version=$mpfr_version \
	--with-mpc-version=$mpc_version \
	--with-gmp-version=$gmp_version \
	--with-gcc-version=$gcc_version \
	--disable-bootstrap \
	--disable-libquadmath \
	--disable-plugin \
	|| error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/libiberty.a

    ln -sf avr-g++ ${TMPINST_DIR}/${PKG}/cctools/bin/avr-c++
    ln -sf avr-gcc ${TMPINST_DIR}/${PKG}/cctools/bin/avr-gcc-${PKG_VERSION}
    ln -sf avr-gcc ${TMPINST_DIR}/${PKG}/cctools/bin/avr-cc

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/avr/${PKG_VERSION}/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/avr/${PKG_VERSION}/install-tools/*

    find ${TMPINST_DIR}/${PKG}/cctools/lib -type f -name "*.a" -exec avr-strip --strip-debug '{}' \;

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag ${PKG}
}
