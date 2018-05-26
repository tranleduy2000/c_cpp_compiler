build_fontconfig() {
    PKG=fontconfig
    PKG_VERSION=2.11.1
    PKG_SUBVERSION=
    PKG_URL="http://www.freedesktop.org/software/fontconfig/release/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="generic font configuration library"
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
			--disable-docs \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    sed -i -e "s|/usr/share/fonts|/system/fonts|" ${TMPINST_DIR}/${PKG}/cctools/etc/fonts/fonts.conf

    make_packages || error "make_packages"

    popd
    s_tag $FUNCNAME
}
