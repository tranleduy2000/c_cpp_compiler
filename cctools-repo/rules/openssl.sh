build_openssl() {
    PKG=openssl
    PKG_VERSION=1.0.1c
    PKG_DESC="The OpenSSL Project is a collaborative effort to develop a robust, commercial-grade, full-featured, and Open Source toolkit implementing the Secure Sockets Layer (SSL v2/v3) and Transport Layer Security (TLS v1) protocols as well as a full-strength general purpose cryptography library."
    PKG_URL=http://www.openssl.org/source/$PKG-$PKG_VERSION.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $FUNCNAME && return

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
    aarch64*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-generic64
	;;
    arm*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-armv4
	;;
    mips64*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-generic64
	;;
    mips*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-generic32
	;;
    x86_64*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-x86_64
	;;
    i*86*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-elf
	;;
    esac

    $MAKE CC=${TARGET_ARCH}-gcc AR="${TARGET_ARCH}-ar rcs" RANLIB=${TARGET_ARCH}-ranlib || error "openssl"
    cp -RL include/openssl ${TMPINST_DIR}/include/openssl
    install -D -m 644 libcrypto.a ${TMPINST_DIR}/lib/libcrypto.a
    install -D -m 644 libssl.a    ${TMPINST_DIR}/lib/libssl.a

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/include
    cp -RL include/openssl ${TMPINST_DIR}/${PKG}/cctools/include/openssl
    install -D -m 644 libcrypto.a ${TMPINST_DIR}/${PKG}/cctools/lib/libcrypto.a
    install -D -m 644 libssl.a    ${TMPINST_DIR}/${PKG}/cctools/lib/libssl.a
    install -D -m 755 apps/openssl ${TMPINST_DIR}/${PKG}/cctools/bin/openssl

    make_packages

    popd
    s_tag $FUNCNAME
}
