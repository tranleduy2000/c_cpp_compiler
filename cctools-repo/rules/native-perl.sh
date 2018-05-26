build_native_perl() {
    PKG=perl
    PKG_VERSION=5.18.1
    PKG_URL="http://www.cpan.org/src/5.0/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="Larry Wall's Practical Extraction and Report Language"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag native-${PKG} && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    copysrc $S_DIR $B_DIR

    cd $B_DIR

    # Configure here

    ash ./Configure -Uafs -Ud_csh -Duseshrplib -Uusethreads -Ui_db -Ui_gdbm -Ui_ndbm -Ui_dbm -Ui_sdbm -Duseopcode -Dman1dir=none -Dman3dir=none -Dsiteman1=none -Dsiteman3=none -Dvendorman1=none -Dvendorman3=none -des -Accflags=-DNO_LOCALE -Uinstallusrbinperl -Dprefix=$TARGET_INST_DIR

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
    s_tag native-${PKG}
}
