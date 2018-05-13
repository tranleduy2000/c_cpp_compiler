build_binutils_host() {
    PKG=binutils
    PKG_VERSION=$binutils_version
    PKG_DESC="Binary utilities"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-binutils

    c_tag ${PKG}-host && return

    banner "Build $PKG host"

    pushd .

    preparesrc $O_DIR $S_DIR

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure \
	--target=${TARGET_ARCH} \
	--prefix=${TARGET_DIR}-host \
	--with-sysroot=$SYSROOT \
	--enable-multilib \
	--disable-nls \
	--disable-werror || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host
}
