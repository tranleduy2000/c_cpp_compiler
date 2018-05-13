build_gmp() {
    PKG=gmp
    PKG_VERSION=$gmp_version
    PKG_DESC="Multiprecision arithmetic library."
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .

#    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure \
	--target=$TARGET_ARCH \
	--host=$TARGET_ARCH \
	--prefix=$TMPINST_DIR \
	--enable-cxx \
	--disable-werror \
	--disable-static \
	--enable-shared || error "configure"

    patch -p0 < ${patch_dir}/libtool-gmp.patch

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.*a

    ${TARGET_ARCH}-strip ${TMPINST_DIR}/${PKG}/cctools/lib/*.so*

    rm -rf   ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}
    mkdir -p ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}
    mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}-dev/cctools/${TARGET_ARCH}/

    local filename="lib${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename lib${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    PKG_DESC="Multiprecision arithmetic library (development files)."
    local filename="lib${PKG}-dev_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG}-dev $filename lib${PKG}-dev $PKG_VERSION $PKG_ARCH "$PKG_DESC" "lib${PKG}"
    cd ${TMPINST_DIR}/${PKG}-dev
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
