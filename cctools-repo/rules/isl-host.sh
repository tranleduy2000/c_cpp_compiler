build_isl_host() {
    PKG=isl
    PKG_VERSION=$isl_version
    PKG_SUBVERSION=
    PKG_URL="http://isl.gforge.inria.fr/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="Integer Set Library"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
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
	--with-gmp-prefix=${TARGET_DIR}-host \
	--disable-werror \
	--enable-static \
	--disable-shared || error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag $FUNCNAME
}
