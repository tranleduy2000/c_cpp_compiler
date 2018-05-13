build_binutils_avr_host() {
    PKG=binutils
    PKG_VERSION=$binutils_avr_version
    PKG_DESC="Binary utilities supporting Atmel's AVR targets"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-binutils-avr

    c_tag ${PKG}-host-avr && return

    banner "Build $PKG avr host"

    pushd .

    preparesrc $O_DIR $S_DIR

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure \
	--target=avr \
	--prefix=${TARGET_DIR}-host \
	--disable-nls \
	--disable-werror || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host-avr
}
