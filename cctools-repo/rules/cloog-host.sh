build_cloog_host() {
    PKG=cloog
    PKG_VERSION=$cloog_version
    PKG_SUBVERSION=
    PKG_URL="http://www.bastoul.net/cloog/pages/download/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="The CLooG Code Generator in the Polyhedral Model's Home"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}

    c_tag $FUNCNAME && return

    pushd .

    banner "Build $PKG"

    #Instead of download, using source from android repo (tag ndk_12_release)
    CURRENT_DIR=`pwd`
    cd $SRC_PREFIX/${PKG}/
    tar -czvf $O_FILE ${PKG}-${PKG_VERSION}
    cd $CURRENT_DIR
    #download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
	--prefix=${TARGET_DIR}-host \
	--with-gmp-prefix=${TARGET_DIR}-host \
	--with-isl-prefix=${TARGET_DIR}-host \
	--disable-werror \
	--enable-static \
	--disable-shared || error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag $FUNCNAME
}
