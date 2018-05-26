build_cppcheck() {
    PKG=cppcheck
    PKG_VERSION=1.64
    PKG_SUBVERSION=
    PKG_URL="http://downloads.sourceforge.net/project/cppcheck/cppcheck/${PKG_VERSION}/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="Static analysis of C/C++ code. Checks for: memory leaks, mismatching allocation-deallocation, buffer overrun, and many more."
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $PKG && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    copysrc $S_DIR $B_DIR

    cd $B_DIR

    # Configure here

#    ${S_DIR}/configure	\
#			--host=${TARGET_ARCH} \
#                        --prefix=$TMPINST_DIR \
#			|| error "Configure $PKG."

    $MAKE $MAKEARGS CXX=${TARGET_ARCH}-g++ CC=${TARGET_ARCH}-gcc HAVE_RULES=yes CFGDIR=${TARGET_INST_DIR}/share/cppcheck || error "make $MAKEARGS"

    $INSTALL -D -m 755 cppcheck ${TMPINST_DIR}/${PKG}/cctools/bin/cppcheck

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/share/cppcheck

    cp -f cfg/* ${TMPINST_DIR}/${PKG}/cctools/share/cppcheck

    $STRIP ${TMPINST_DIR}/${PKG}/cctools/bin/cppcheck

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
