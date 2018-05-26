build_zip() {
    PKG=zip
    PKG_VERSION=3.0
    PKG_URL="http://downloads.sourceforge.net/project/infozip/Zip%203.x%20%28latest%29/3.0/zip30.tar.gz"
    PKG_DESC="Archiver for .zip files"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/zip30.tar.gz
    S_DIR=$src_dir/zip30
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

    sh unix/configure "${TARGET_ARCH}-gcc" "-Wall -I. -DUNIX -DNO_UNICODE_SUPPORT" || error "configure zip"

    $MAKE -f unix/Makefile generic || error "build zip"

    $MAKE -f unix/Makefile generic install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/man

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
