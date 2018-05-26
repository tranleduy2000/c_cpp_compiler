build_curl() {
    PKG=curl
    PKG_VERSION=7.28.1
    PKG_DESC="cURL is a computer software project providing a library and command-line tool for transferring data using various protocols."
    PKG_URL=http://curl.haxx.se/download/${PKG}-${PKG_VERSION}.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
#    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    ./configure --host=$TARGET_ARCH --prefix=$TMPINST_DIR --disable-shared --enable-static \
	--with-ca-bundle=${TARGET_INST_DIR}/etc/ssl/certs/ca-certificates.crt \
	--with-ssl=${TMPINST_DIR} || error "configure"

    $MAKE $MAKEARGS || error "make"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig ${TMPINST_DIR}/${PKG}/cctools/lib/*.la
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/curl-config
    mkdir ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/
    mv ${TMPINST_DIR}/${PKG}/cctools/lib     ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
