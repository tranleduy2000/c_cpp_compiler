build_openssl() {
    PKG=openssl
    PKG_VERSION=1.0.1c
    PKG_DESC="The OpenSSL Project is a collaborative effort to develop a robust, commercial-grade, full-featured, and Open Source toolkit implementing the Secure Sockets Layer (SSL v2/v3) and Transport Layer Security (TLS v1) protocols as well as a full-strength general purpose cryptography library."
    PKG_URL=http://www.openssl.org/source/$PKG-$PKG_VERSION.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    case $TARGET_ARCH in
    arm*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-armv4
	;;
    mips*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-generic32
	;;
    i*86*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-elf
	;;
    esac

    $MAKE CC=${TARGET_ARCH}-gcc AR="${TARGET_ARCH}-ar rcs" RANLIB=${TARGET_ARCH}-ranlib || error "openssl"
    cp -RL include/openssl ${TMPINST_DIR}/include/openssl
    install -D -m 644 libcrypto.a ${TMPINST_DIR}/lib/libcrypto.a
    install -D -m 644 libssl.a    ${TMPINST_DIR}/lib/libssl.a

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include
    cp -RL include/openssl ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/openssl
    install -D -m 644 libcrypto.a ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/libcrypto.a
    install -D -m 644 libssl.a    ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/libssl.a
    install -D -m 755 apps/openssl ${TMPINST_DIR}/${PKG}/cctools/bin/openssl

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
