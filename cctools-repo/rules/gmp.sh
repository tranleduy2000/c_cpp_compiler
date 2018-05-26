build_gmp() {
    PKG=gmp
    PKG_VERSION=$gmp_version
    PKG_SUBVERSION=
    PKG_URL="https://gmplib.org/download/gmp/${PKG}-${PKG_VERSION}.tar.xz"
    PKG_DESC="The GNU Multiple Precision Arithmetic Library"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.xz
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
	--host=$TARGET_ARCH \
	--prefix=$TMPINST_DIR \
	--target=$TARGET_ARCH \
	--enable-cxx \
	--disable-werror \
	--disable-static \
	--enable-shared || error "Configure $PKG."

    patch -p0 < ${patch_dir}/libtool-gmp-${PKG_VERSION}.patch || error "patching libtool"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages

    popd
    s_tag $FUNCNAME
}
