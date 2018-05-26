build_font_dec_misc() {
    PKG=font-dec-misc
    PKG_VERSION=1.0.3
    PKG_SUBVERSION=
    PKG_URL="http://www.x.org/releases/X11R7.7/src/font/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="Xorg X11 font dec-misc"
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
			--with-fontrootdir=${TMPINST_DIR}/${PKG}/cctools/share/fonts/X11 \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install FONTROOTDIR=${TMPINST_DIR}/${PKG}/cctools/share/fonts/X11 prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages

    popd
    s_tag $FUNCNAME
}
