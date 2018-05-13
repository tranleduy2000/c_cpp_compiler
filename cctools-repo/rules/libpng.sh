build_libpng() {
    PKG=libpng
    PKG_VERSION=1.6.8
    PKG_SUBVERSION=
    PKG_URL="http://sourceforge.net/projects/libpng/files/libpng16/${PKG_VERSION}/${PKG}-${PKG_VERSION}.tar.xz"
    PKG_DESC="Reference library for supporting the Portable Network Graphics (PNG) format."
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

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    local OPTS
    case $TARGET_ARCH in
    arm*)
	OPTS="--enable-arm-neon=api"
	;;
    esac

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TMPINST_DIR \
			$OPTS \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/bin
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/*.la

    rm -rf  ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}
    copysrc ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}/include
    mkdir ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}/lib
    cp -R ${TMPINST_DIR}/${PKG}/cctools/lib/*.a ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}/lib
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include
    rm -f  ${TMPINST_DIR}/${PKG}/cctools/lib/*.a

    $STRIP ${TMPINST_DIR}/${PKG}/cctools/lib/*.so*

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    PKG_DESC="Reference library for supporting the Portable Network Graphics (PNG) format (development files)."
    local filename="${PKG}-dev_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG}-dev $filename ${PKG}-dev ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "libpng $PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}-dev
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
