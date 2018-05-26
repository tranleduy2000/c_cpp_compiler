build_pixman() {
    PKG=pixman
    PKG_VERSION=0.32.6
    PKG_SUBVERSION=
    PKG_URL="http://cairographics.org/releases/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="pixel-manipulation library for X and cairo"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $FUNCNAME && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    LIBS="-lcpufeatures" \
    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TMPINST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages

    popd
    s_tag $FUNCNAME
}
