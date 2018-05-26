build_unzip() {
    PKG=unzip
    PKG_VERSION=6.0
    PKG_URL="http://downloads.sourceforge.net/project/infozip/UnZip%206.x%20%28latest%29/UnZip%206.0/unzip60.tar.gz"
    PKG_DESC="De-archiver for .zip files"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/unzip60.tar.gz
    S_DIR=$src_dir/unzip60
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

#    sh unix/configure "${TARGET_ARCH}-gcc" "-Wall -I. -DUNIX -DBIONIC_WCTOMB_FIX" || error "configure unzip"

    local DEFINES=" -DACORN_FTYPE_NFS -DWILD_STOP_AT_DIR -DLARGE_FILE_SUPPORT \
 -DUNICODE_SUPPORT -DUNICODE_WCHAR -DUTF8_MAYBE_NATIVE -DNO_LCHMOD \
 -DDATE_FORMAT=DF_YMD -DUSE_BZIP2 -DIZ_HAVE_UXUIDGID -DNOMEMCPY \
 -DNO_WORKING_ISPRINT"

    $MAKE -f unix/Makefile generic CC=${TARGET_ARCH}-gcc CF="-Wall -I. $DEFINES" || error "build zip"

    $MAKE -f unix/Makefile generic install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/man

    ln -sf unzip ${TMPINST_DIR}/${PKG}/cctools/bin/zipinfo

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
