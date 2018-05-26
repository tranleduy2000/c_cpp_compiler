build_cloog() {
    PKG=cloog
    PKG_VERSION=$cloog_version
    PKG_SUBVERSION=
    PKG_URL="http://www.bastoul.net/cloog/pages/download/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="The CLooG Code Generator in the Polyhedral Model's Home"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $FUNCNAME && return

    pushd .

    banner "Build $PKG"

    if [ ! -f $O_FILE ]; then
	download $PKG_URL $O_FILE
    fi

    unpack $src_dir $O_FILE
    patchsrc $S_DIR $PKG $PKG_VERSION

    if [ "$USE_NATIVE_BUILD" = "yes" ]; then
	fix_bionic_shell $S_DIR
    fi

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
	--target=$TARGET_ARCH \
	--host=$TARGET_ARCH \
	--prefix=$TMPINST_DIR \
	--with-gmp=system \
	--with-gmp-prefix=$TMPINST_DIR \
	--with-isl=system \
	--with-isl-prefix=$TMPINST_DIR \
	--disable-werror \
	--disable-static \
	--enable-shared || error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/bin

    make_packages

    popd
    s_tag $FUNCNAME
}
