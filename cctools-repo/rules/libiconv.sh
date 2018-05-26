build_libiconv() {
    PKG=libiconv
    PKG_VERSION=1.14
    PKG_SUBVERSION=
    PKG_URL="http://ftp.gnu.org/gnu/libiconv/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="This library provides an iconv() implementation, for use on systems which don't have one, or whose implementation cannot convert from/to Unicode."
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
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

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TMPINST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/preloadable_libiconv.so

    make_packages

    popd
    s_tag $FUNCNAME
}
