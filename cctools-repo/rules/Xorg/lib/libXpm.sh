build_libXpm() {
    PKG=libXpm
    PKG_VERSION=3.5.10
    PKG_SUBVERSION=
    PKG_URL="http://www.x.org/releases/X11R7.7/src/lib/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="X11 pixmap library"
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

    LDFLAGS="-L$TMPINST_DIR/lib -Wl,-rpath-link,${SYSROOT}/usr/lib" \
    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TMPINST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    local BINPKG=xpmutils
    $INSTALL -D -m 755 ${TMPINST_DIR}/${PKG}/cctools/bin/cxpm ${TMPINST_DIR}/${BINPKG}/cctools/bin/cxpm
    $INSTALL -D -m 755 ${TMPINST_DIR}/${PKG}/cctools/bin/sxpm ${TMPINST_DIR}/${BINPKG}/cctools/bin/sxpm
    $STRIP ${TMPINST_DIR}/${BINPKG}/cctools/bin/*
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/bin

    make_packages

    local filename="${BINPKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    PKG_DESC="X11 pixmap utilities"
    build_package_desc ${TMPINST_DIR}/${BINPKG} $filename ${BINPKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${BINPKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $FUNCNAME
}
