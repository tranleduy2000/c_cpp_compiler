build_binutils_avr() {
    PKG=binutils-avr
    PKG_VERSION=$binutils_avr_version
    PKG_DESC="Binary utilities supporting Atmel's AVR targets"
    O_DIR=$SRC_PREFIX/binutils/binutils-${PKG_VERSION}
    S_DIR=$src_dir/binutils-${PKG_VERSION}
    B_DIR=$build_dir/$PKG

    c_tag ${PKG} && return

    banner "Build $PKG"

    pushd .

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

#    export ac_cv_func_strtod=no
    $S_DIR/configure \
	--target=avr \
	--host=$TARGET_ARCH \
	--prefix=$TARGET_DIR \
	--disable-nls \
	--disable-werror || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/avr/bin/*
    ln -sf ../../bin/avr-ar ${TMPINST_DIR}/${PKG}/cctools/avr/bin/ar
    ln -sf ../../bin/avr-as ${TMPINST_DIR}/${PKG}/cctools/avr/bin/as
    ln -sf ../../bin/avr-ld ${TMPINST_DIR}/${PKG}/cctools/avr/bin/ld
    ln -sf ../../bin/avr-ld.bfd ${TMPINST_DIR}/${PKG}/cctools/avr/bin/ld.bfd
    ln -sf ../../bin/avr-nm ${TMPINST_DIR}/${PKG}/cctools/avr/bin/nm
    ln -sf ../../bin/avr-objcopy ${TMPINST_DIR}/${PKG}/cctools/avr/bin/objcopy
    ln -sf ../../bin/avr-objdump ${TMPINST_DIR}/${PKG}/cctools/avr/bin/objdump
    ln -sf ../../bin/avr-ranlib ${TMPINST_DIR}/${PKG}/cctools/avr/bin/ranlib
    ln -sf ../../bin/avr-strip ${TMPINST_DIR}/${PKG}/cctools/avr/bin/strip

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.a
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/avr-ld.bfd
    ln -sf avr-ld ${TMPINST_DIR}/${PKG}/cctools/bin/avr-ld.bfd

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag ${PKG}
}
