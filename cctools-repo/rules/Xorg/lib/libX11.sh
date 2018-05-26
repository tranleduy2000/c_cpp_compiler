build_libX11() {
    PKG=libX11
    PKG_VERSION=1.5.0
    PKG_SUBVERSION=
    PKG_URL="http://www.x.org/releases/X11R7.7/src/lib/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="X11 client-side library"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
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

    CFLAGS="-I$TMPINST_DIR/include -D_LINUX_IPC_H -Dipc_perm=debian_ipc_perm" \
    CXXFLAGS="-I$TMPINST_DIR/include -D_LINUX_IPC_H -Dipc_perm=debian_ipc_perm" \
    LDFLAGS="-L$TMPINST_DIR/lib -Wl,-rpath-link,${SYSROOT}/usr/lib" \
    LIBS="-landroid-shmem" \
    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TMPINST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS HOST_CC="gcc -I${S_DIR}/include" || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages

    popd
    s_tag $FUNCNAME
}
