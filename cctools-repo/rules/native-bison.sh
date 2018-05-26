build_bison() {
    PKG=bison
    PKG_VERSION=3.0.2
    PKG_SUBVERSION=
    PKG_URL="http://ftp.gnu.org/gnu/bison/${PKG}-${PKG_VERSION}.tar.xz"
    PKG_DESC="YACC-compatible parser generator"
    PKG_DEPS="m4"
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.xz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $PKG && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    fix_bionic_shell $S_DIR

    cd $S_DIR

    autoreconf || error "bison autoreconfiguration"

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TARGET_INST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    install -D -m 644 ${TMPINST_DIR}/${PKG}/cctools/lib/liby.a  ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/liby.a

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib

    $STRIP ${TMPINST_DIR}/${PKG}/cctools/bin/bison

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
