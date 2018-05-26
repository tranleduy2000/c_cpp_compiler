build_gcc_avr_host() {
    PKG=gcc
    PKG_VERSION=$gcc_avr_version
    PKG_URL="http://mirrors-usa.go-parts.com/gcc/releases/gcc-${PKG_VERSION}/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="The GNU C compiler (cross compiler for avr)"
    O_FILE=$SRC_PREFIX/gnu/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/gnu/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}-avr-host

    c_tag ${PKG}-avr-host && return

    banner "Build $PKG avr host"

    pushd .

    download $PKG_URL $O_FILE

    if test ! -e ${S_DIR}/.unpacked; then
	unpack $src_dir/gnu $O_FILE

	patchsrc $S_DIR $PKG $PKG_VERSION
	touch ${S_DIR}/.unpacked
    fi

#    preparesrc $O_DIR $S_DIR

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
	--with-cloog=${TARGET_DIR}-host \
	--with-isl=${TARGET_DIR}-host \
	--with-ppl=${TARGET_DIR}-host \
	--enable-long-long \
	--disable-libssp \
	--disable-nls \
	--disable-shared \
	--with-binutils-version=$binutils_version \
	--with-mpfr-version=$mpfr_version \
	--with-mpc-version=$mpc_version \
	--with-gmp-version=$gmp_version \
	--with-cloog-version=$cloog_version \
	--with-isl-version=$isl_version \
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
