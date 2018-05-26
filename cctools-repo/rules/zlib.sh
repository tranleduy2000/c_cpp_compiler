build_zlib() {
    PKG=zlib
    PKG_VERSION=1.2.8
    PKG_URL="http://zlib.net/${PKG}-${PKG_VERSION}.tar.xz"
    PKG_DESC="compression library - development"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.xz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $PKG && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    copysrc $S_DIR $B_DIR

    cd $B_DIR

    # Configure here

    ./configure	\
			--static \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS CC=${TARGET_ARCH}-gcc AR=${TARGET_ARCH}-ar RANLIB=${TARGET_ARCH}-ranlib || error "make $MAKEARGS"

    install -D -m 644 libz.a ${TMPINST_DIR}/lib/libz.a

    install -D -m 644 libz.a ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/libz.a || error "package install"

    local filename="${PKG}-dev_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-dev $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
