build_libiconv() {
    PKG=libiconv
    PKG_VERSION=1.14
    PKG_URL="http://ftp.gnu.org/gnu/libiconv/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="This library provides an iconv() implementation, for use on systems which don't have one, or whose implementation cannot convert from/to Unicode."
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $PKG && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
			--prefix=$TMPINST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "$PKG: make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/lib/*.so*
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.la
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib
    mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mv ${TMPINST_DIR}/${PKG}/cctools/lib/*.a ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib

    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/preloadable_libiconv.so

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
