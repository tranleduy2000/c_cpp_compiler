build_vim() {
    PKG=vim
    PKG_VERSION=7.4
    PKG_SUBVERSION=-1
    PKG_URL="ftp://ftp.vim.org/pub/vim/unix/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="Vi IMproved - enhanced vi editor"
    PKG_DEPS="libiconv"
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/vim74
    B_DIR=$build_dir/${PKG}

    c_tag $PKG && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    fix_bionic_shell $S_DIR

    copysrc $S_DIR $B_DIR

    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TARGET_INST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    install -D -m 644 ${TMPINST_DIR}/${PKG}/cctools/share/vim/vim74/vimrc_example.vim ${TMPINST_DIR}/${PKG}/cctools/share/vim/vimrc

    $STRIP ${TMPINST_DIR}/${PKG}/cctools/bin/*

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
