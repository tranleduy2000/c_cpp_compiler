build_binutils_mingw32_host() {
    PKG=binutils
    PKG_VERSION=$binutils_version
    PKG_DESC="Binary utilities supporting mingw32 targets"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-binutils-mingw32-${1}

    c_tag ${PKG}-host-mingw32-${1} && return

    banner "Build $PKG mingw32 host"

    pushd .

    preparesrc $O_DIR $S_DIR

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

#	--enable-targets=i686-w64-mingw32,x86_64-w64-mingw32

    $S_DIR/configure \
	--target=${1} \
	--prefix=${TARGET_DIR}-host \
	--disable-multilib \
	--disable-nls \
	--disable-werror || error "configure"

#	--enable-multilib \
#	--enable-targets=x86_64-w64-mingw32,i686-w64-mingw32 \

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host-mingw32-${1}
}
