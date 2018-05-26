build_subversion() {
    PKG=subversion
    PKG_VERSION=1.9.4
    PKG_SUBVERSION=
    PKG_URL="http://www-us.apache.org/dist/subversion/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="Advanced version control system"
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

    local OPT_ARGS=

    case $TARGET_ARCH in
    aarch64*|mips64*|x86_64*)
	;;
    *)
	OPT_ARGS="CPPFLAGS=-Dinline=__inline__"
	;;
    esac

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TMPINST_DIR \
			--with-apr=$TMPINST_DIR \
			--with-apr-util=$TMPINST_DIR \
			--with-neon=$TMPINST_DIR \
			--with-sqlite=$TMPINST_DIR \
			$OPT_ARGS \
			|| error "Configure $PKG."

#			--enable-static \
#			--disable-shared \

    patch -p0 < ${patch_dir}/libtool-${PKG}-${PKG_VERSION}.patch || error "patching libtool"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages

    popd
    s_tag $FUNCNAME
}
