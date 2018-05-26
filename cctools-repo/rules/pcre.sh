build_pcre() {
    PKG=pcre
    PKG_VERSION=8.35
    PKG_SUBVERSION=
    PKG_URL="http://downloads.sourceforge.net/project/pcre/pcre/${PKG_VERSION}/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="Perl 5 Compatible Regular Expression Library - runtime files"
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

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
			--host=${TARGET_ARCH} \
			--enable-utf8 \
			--enable-unicode-properties \
                        --prefix=$TMPINST_DIR \
			|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    ln -s ${TMPINST_DIR}/bin/pcre-config ${TARGET_DIR}-host/bin/

    $INSTALL -D -m 755 .libs/libpcre.so.1.2.3 ${TMPINST_DIR}/lib${PKG}/cctools/lib/libpcre.so.1.2.3
    ln -sf libpcre.so.1.2.3 ${TMPINST_DIR}/lib${PKG}/cctools/lib/libpcre.so.1
    ln -sf libpcre.so.1.2.3 ${TMPINST_DIR}/lib${PKG}/cctools/lib/libpcre.so
    $STRIP ${TMPINST_DIR}/lib${PKG}/cctools/lib/*

    local filename="lib${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/lib${PKG} $filename lib${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/lib${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    cd $B_DIR
    mkdir -p ${TMPINST_DIR}/lib${PKG}-dev/cctools/${TARGET_ARCH}/include
    mkdir -p ${TMPINST_DIR}/lib${PKG}-dev/cctools/${TARGET_ARCH}/lib
    $INSTALL -m 644 pcre.h                  ${TMPINST_DIR}/lib${PKG}-dev/cctools/${TARGET_ARCH}/include
    $INSTALL -m 644 ${S_DIR}/pcre_scanner.h ${TMPINST_DIR}/lib${PKG}-dev/cctools/${TARGET_ARCH}/include
    $INSTALL -m 644 pcre_stringpiece.h      ${TMPINST_DIR}/lib${PKG}-dev/cctools/${TARGET_ARCH}/include
    $INSTALL -m 644 .libs/libpcre.a         ${TMPINST_DIR}/lib${PKG}-dev/cctools/${TARGET_ARCH}/lib

    PKG_DESC="Perl 5 Compatible Regular Expression Library - development files"
    local filename="lib${PKG}-dev_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/lib${PKG}-dev $filename lib${PKG}-dev ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/lib${PKG}-dev
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    cd $B_DIR
    $INSTALL -D -m 755 .libs/libpcrecpp.so.0.0.0 ${TMPINST_DIR}/lib${PKG}cpp/cctools/lib/libpcrecpp.so.0.0.0
    ln -sf libpcrecpp.so.0.0.0 ${TMPINST_DIR}/lib${PKG}cpp/cctools/lib/libpcrecpp.so.0
    ln -sf libpcrecpp.so.0.0.0 ${TMPINST_DIR}/lib${PKG}cpp/cctools/lib/libpcrecpp.so
    $STRIP ${TMPINST_DIR}/lib${PKG}cpp/cctools/lib/*

    PKG_DESC="C++ wrapper class for pcre (runtime)"
    local filename="lib${PKG}cpp_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/lib${PKG}cpp $filename lib${PKG}cpp ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/lib${PKG}cpp
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    cd $B_DIR
    mkdir -p ${TMPINST_DIR}/lib${PKG}cpp-dev/cctools/${TARGET_ARCH}/include
    mkdir -p ${TMPINST_DIR}/lib${PKG}cpp-dev/cctools/${TARGET_ARCH}/lib
    $INSTALL -m 644 ${S_DIR}/pcrecpp.h      ${TMPINST_DIR}/lib${PKG}cpp-dev/cctools/${TARGET_ARCH}/include
    $INSTALL -m 644 pcrecpparg.h            ${TMPINST_DIR}/lib${PKG}cpp-dev/cctools/${TARGET_ARCH}/include
    $INSTALL -m 644 .libs/libpcrecpp.a      ${TMPINST_DIR}/lib${PKG}cpp-dev/cctools/${TARGET_ARCH}/lib

    PKG_DESC="C++ wrapper class for pcre (development)"
    local filename="lib${PKG}cpp-dev_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/lib${PKG}cpp-dev $filename lib${PKG}cpp-dev ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/lib${PKG}cpp-dev
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    cd $B_DIR
    $INSTALL -D -m 755 .libs/libpcreposix.so.0.0.2 ${TMPINST_DIR}/lib${PKG}posix/cctools/lib/libpcreposix.so.0.0.2
    ln -sf libpcreposix.so.0.0.2 ${TMPINST_DIR}/lib${PKG}posix/cctools/lib/libpcreposix.so.0
    ln -sf libpcreposix.so.0.0.2 ${TMPINST_DIR}/lib${PKG}posix/cctools/lib/libpcreposix.so
    $STRIP ${TMPINST_DIR}/lib${PKG}posix/cctools/lib/*

    PKG_DESC="POSIX wrapper class for pcre (runtime)"
    local filename="lib${PKG}posix_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/lib${PKG}posix $filename lib${PKG}posix ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/lib${PKG}posix
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    cd $B_DIR
    mkdir -p ${TMPINST_DIR}/lib${PKG}posix-dev/cctools/${TARGET_ARCH}/include
    mkdir -p ${TMPINST_DIR}/lib${PKG}posix-dev/cctools/${TARGET_ARCH}/lib
    $INSTALL -m 644 ${S_DIR}/pcreposix.h    ${TMPINST_DIR}/lib${PKG}posix-dev/cctools/${TARGET_ARCH}/include
    $INSTALL -m 644 .libs/libpcreposix.a    ${TMPINST_DIR}/lib${PKG}posix-dev/cctools/${TARGET_ARCH}/lib

    PKG_DESC="POSIX wrapper class for pcre (development)"
    local filename="lib${PKG}posix-dev_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/lib${PKG}posix-dev $filename lib${PKG}posix-dev ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/lib${PKG}posix-dev
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
