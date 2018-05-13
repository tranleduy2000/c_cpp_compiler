build_subversion() {
    PKG=subversion
    PKG_VERSION=1.7.14
    PKG_DESC="Subversion exists to be universally recognized and adopted as an open-source, centralized version control system characterized by its reliability as a safe haven for valuable data; the simplicity of its model and usage; and its ability to support the needs of a wide variety of users and projects, from individuals to large-scale enterprise operations."
    PKG_URL=http://www.eu.apache.org/dist/$PKG/$PKG-$PKG_VERSION.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG
    c_tag $PKG && return

    banner "Build $PKG"

    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    #patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    CFLAGS="-I$TMPINST_DIR/include" LDFLAGS="-L$TMPINST_DIR/lib" \
	./configure --host=$TARGET_ARCH --prefix=$TARGET_INST_DIR \
	--with-apr=$TMPINST_DIR --with-apr-util=$TMPINST_DIR \
	--with-neon=$TMPINST_DIR \
	--with-sqlite=$TMPINST_DIR --enable-static --disable-shared || error "configure"


    $MAKE $MAKEARGS || error "make $MAKEARGS"

    #$MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/lib

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
