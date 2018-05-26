build_htop() {
    PKG=htop
    PKG_VERSION=1.0.2
    PKG_DESC="htop is an interactive text-mode process viewer for Linux. It aims to be a better top."
    PKG_URL=http://downloads.sourceforge.net/project/htop/htop/$PKG_VERSION/$PKG-$PKG_VERSION.tar.gz
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

    CFLAGS="-I$TMPINST_DIR/include" LDFLAGS="-L$TMPINST_DIR/lib" \
	./configure --host=$TARGET_ARCH --prefix=$TMPINST_DIR --disable-unicode --enable-native-affinity=no


    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    #rm -rf $TARGET_DIR/share

    install -D -m 755 htop ${TMPINST_DIR}/bin/htop

    #make install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $INSTALL -D -m 755 htop ${TMPINST_DIR}/${PKG}/cctools/bin/htop
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/htop
    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
