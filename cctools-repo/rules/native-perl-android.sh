build_native_perl_android() {
    PKG=perl
    PKG_VERSION=5.18.2
    PKG_URL="https://github.com/dex4er/perl5/archive/dexter/android/v${PKG_VERSION}.zip"
    PKG_DESC="Larry Wall's Practical Extraction and Report Language"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/perl5-dexter-android-v${PKG_VERSION}.zip
    S_DIR=$src_dir/perl5-dexter-android-v${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag native-${PKG}-android && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    copysrc $S_DIR $B_DIR

    cd $B_DIR

    # Configure here

    ash ./Configure -des \
	-Dlibpth="/system/lib ${TARGET_INST_DIR}/lib ${TARGET_INST_DIR}/${TARGET_ARCH}/lib" \
	-Dusrinc=${TARGET_INST_DIR}/${TARGET_ARCH}/include \
	-Dprefix=${TARGET_INST_DIR} || error "Configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install DESTDIR=${TMPINST_DIR}/${PKG}-fakeroot || error "package install"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    cp -a ${TMPINST_DIR}/${PKG}-fakeroot/${TARGET_INST_DIR}/* ${TMPINST_DIR}/${PKG}/cctools || error "copying to target dir"

    $STRIP ${TMPINST_DIR}/${PKG}/cctools/bin/*

    find ${TMPINST_DIR}/${PKG}/cctools/ -name "*.so" -type f -exec chmod 755 {} \; -exec $STRIP {} \;

    find ${TMPINST_DIR}/${PKG}/cctools/lib -name "libperl.so" -type f -exec mv {} ${TMPINST_DIR}/${PKG}/cctools/lib/ \;

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag native-${PKG}-android
}
