build_avr_libc() {
    PKG=avr-libc
    PKG_VERSION=1.8.0
    PKG_DESC="The C runtime library for the AVR family of microcontrollers."
    PKG_URL=http://download.savannah.gnu.org/releases/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
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

    export PATH=${TARGET_DIR}-host/bin:$PATH

    ./configure --host=avr --prefix=$TARGET_INST_DIR || error "configure"

    $MAKE $MAKEARGS || error "make"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/avr-man
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/Examples/avr-libc
    cp -a  ${TMPINST_DIR}/${PKG}/cctools/share/doc/avr-libc-1.8.0/examples/* ${TMPINST_DIR}/${PKG}/cctools/Examples/avr-libc/
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
