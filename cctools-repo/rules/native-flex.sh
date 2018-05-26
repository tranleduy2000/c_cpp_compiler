build_flex() {
    PKG=flex
    PKG_VERSION=2.5.37
    PKG_SUBVERSION=
    PKG_URL="http://downloads.sourceforge.net/project/flex/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="A fast lexical analyzer generator"
    PKG_DEPS="m4"
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $PKG && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    fix_bionic_shell $S_DIR

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TARGET_INST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $STRIP ${TMPINST_DIR}/${PKG}/cctools/bin/*

    mkdir -p ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}

    cp -R ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}/
    cp -R ${TMPINST_DIR}/${PKG}/cctools/lib     ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}/

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    PKG_DESC="A fast lexical analyzer generator (development files)"
    PKG_DEPS="flex"

    local filename="${PKG}-dev_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG}-dev $filename ${PKG}-dev ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}-dev
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
