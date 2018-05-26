build_git() {
    PKG=git
    PKG_VERSION=2.10.0
    PKG_SUBVERSION="-1"
    PKG_DESC="Git is a distributed revision control and source code management (SCM) system with an emphasis on speed.[3] Git was initially designed and developed by Linus Torvalds for Linux kernel development; it has since been adopted by many other projects."
    PKG_URL=https://www.kernel.org/pub/software/scm/git/${PKG}-${PKG_VERSION}.tar.gz
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

    ac_cv_fread_reads_directories=no \
    ac_cv_snprintf_returns_bogus=no \
    CFLAGS="-O2 -DNO_GECOS_IN_PWENT -DNO_NSEC -DNO_ICONV" \
    LIBS="-L${TMPINST_DIR}/lib -lcurl -lssl -lcrypto -lz" \
	./configure --host=$TARGET_ARCH --prefix=$TARGET_INST_DIR \
	--with-openssl=${TMPINST_DIR} --with-curl=${TMPINST_DIR} --with-expat=${TMPINST_DIR} \
	--without-iconv \
	--without-tcltk --without-python || error "configure"

    $MAKE $MAKEARGS NEEDS_SSL_WITH_CURL=1 NEEDS_CRYPTO_WITH_SSL=1 NEEDS_LIBRT=0 || error "make"

    #$MAKE install NEEDS_SSL_WITH_CURL=1 NEEDS_CRYPTO_WITH_SSL=1 NO_INSTALL_HARDLINKS=1 || error "make install"

    rm -rf ${TMPINST_DIR}/${PKG}
    rm -rf ${TMPINST_DIR}/${PKG}-tmp

    $MAKE install NEEDS_SSL_WITH_CURL=1 NEEDS_CRYPTO_WITH_SSL=1 NEEDS_LIBRT=0 NO_INSTALL_HARDLINKS=1 DESTDIR=${TMPINST_DIR}/${PKG}-tmp || error "package install"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    cp -a ${TMPINST_DIR}/${PKG}-tmp/${TARGET_INST_DIR}/* ${TMPINST_DIR}/${PKG}/cctools/

    rm -rf ${TMPINST_DIR}/${PKG}-tmp

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/libexec/git-core/*

    #rm -rf ${TMPINST_DIR}/${PKG}/cctools/share ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig ${TMPINST_DIR}/${PKG}/cctools/lib/*.la
    #mkdir ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    #mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/
    #mv ${TMPINST_DIR}/${PKG}/cctools/lib     ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/

    fix_bionic_shell ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "ca-certificates"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
