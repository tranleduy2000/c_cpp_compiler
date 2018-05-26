build_apr_util() {
    PKG=apr-util
    PKG_VERSION=1.5.4
    PKG_SUBVERSION=
    PKG_URL="http://www.eu.apache.org/dist/apr/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="The mission of the Apache Portable Runtime (APR) project is to create and maintain software libraries that provide a predictable and consistent interface to underlying platform-specific implementations."
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

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TMPINST_DIR \
			--with-apr=$TMPINST_DIR \
			--with-openssl=$TMPINST_DIR \
			--with-sqlite3=$TMPINST_DIR \
			--with-expat=$TMPINST_DIR \
			--disable-shared \
			--enable-static \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages

    popd
    s_tag $FUNCNAME
}
