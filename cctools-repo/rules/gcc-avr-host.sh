build_gcc_avr_host() {
    PKG=gcc
    PKG_VERSION=$gcc_avr_version
    PKG_DESC="The GNU C compiler (cross compiler for avr)"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}-avr

    c_tag ${PKG}-avr-host && return

    banner "Build $PKG avr host"

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
	--prefix=${TARGET_DIR}-host \
	--enable-languages=c,c++ \
	--with-gmp=${TARGET_DIR}-host \
	--with-mpfr=${TARGET_DIR}-host \
	--with-mpc=${TARGET_DIR}-host \
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

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-avr-host
}
