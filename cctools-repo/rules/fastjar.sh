build_fastjar() {
    PKG=fastjar
    PKG_VERSION=0.98
    PKG_SUBVERSION=
    PKG_URL="http://download.savannah.gnu.org/releases/fastjar/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="Fastjar is an implementation of Sun's jar utility that comes with the JDK, written entirely in C, and runs in a fraction of the time while being 100% feature compatible."
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
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

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
                        --prefix=$TMPINST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib

    cat > ${TMPINST_DIR}/${PKG}/postinst <<EOF
#!/system/bin/sh

test -e \${CCTOOLSDIR}/bin/jar || ln -s fastjar \${CCTOOLSDIR}/bin/jar
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/postinst

    cat > ${TMPINST_DIR}/${PKG}/prerm <<EOF
#!/system/bin/sh

test \`readlink \${CCTOOLSDIR}/bin/jar\` = "fastjar" && rm -f \${CCTOOLSDIR}/bin/jar
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/prerm

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
