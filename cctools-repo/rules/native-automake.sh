build_automake() {
    PKG=automake
    PKG_VERSION=1.14
    PKG_URL="http://ftp.gnu.org/gnu/automake/${PKG}-${PKG_VERSION}.tar.xz"
    PKG_DESC="Tool for generating GNU Standards-compliant Makefiles"
    PKG_DEPS="autoconf"
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

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TARGET_INST_DIR \
			PERL=${TARGET_INST_DIR}/bin/perl \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    #$STRIP ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/aclocal
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/automake

    cat >> ${TMPINST_DIR}/${PKG}/postinst << EOF
#!/system/bin/sh

ln -sf aclocal-${PKG_VERSION}  \${CCTOOLSDIR}/bin/aclocal
ln -sf automake-${PKG_VERSION} \${CCTOOLSDIR}/bin/automake

EOF

    chmod 755 ${TMPINST_DIR}/${PKG}/postinst

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
