build_mpc_host() {
    PKG=mpc
    PKG_VERSION=$mpc_version
    PKG_DESC="multiple precision complex floating-point library"
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}

    c_tag ${PKG}-host && return

    banner "Build $PKG host"

    pushd .

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure --prefix=${TARGET_DIR}-host --with-gmp=${TARGET_DIR}-host --with-mpfr=${TARGET_DIR}-host --disable-werror --enable-static --disable-shared || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host
}
