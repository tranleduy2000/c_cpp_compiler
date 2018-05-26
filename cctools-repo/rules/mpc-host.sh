build_mpc_host() {
    PKG=mpc
    PKG_VERSION=$mpc_version
    PKG_SUBVERSION=
    PKG_URL="http://ftp.gnu.org/gnu/mpc/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="C library for the arithmetic of complex numbers with arbitrarily high precision and correct rounding of the result"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}

    c_tag $FUNCNAME && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
	--prefix=${TARGET_DIR}-host \
	--with-gmp=${TARGET_DIR}-host \
	--with-mpfr=${TARGET_DIR}-host \
	--disable-werror \
	--enable-static \
	--disable-shared || error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag $FUNCNAME
}
