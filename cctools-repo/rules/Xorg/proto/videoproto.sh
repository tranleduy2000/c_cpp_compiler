build_videoproto() {
    PKG=videoproto
    PKG_VERSION=2.3.1
    PKG_SUBVERSION=
    PKG_URL="http://www.x.org/releases/X11R7.7/src/proto/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="X11 Video extension wire protocol"
    PKG_DEPS="xorg-util-macros-dev"
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
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    PKG=xorg-${PKG}

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages nomain

    popd
    s_tag $FUNCNAME
}
