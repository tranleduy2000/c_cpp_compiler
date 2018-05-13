build_luajit() {
    PKG=LuaJIT
    PKG_VERSION=2.0.0
    PKG_DESC="LuaJIT is a Just-In-Time Compiler (JIT) for the Lua programming language. \
Lua is a powerful, dynamic and light-weight programming language. \
It may be embedded or used as a general-purpose, stand-alone language."
    PKG_URL=http://luajit.org/download/$PKG-$PKG_VERSION.tar.gz
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

    $MAKE $MAKEARGS CROSS=${TARGET_ARCH}- HOST_CC="i686-pc-linux-gnu-gcc" || error "make $MAKEARGS"

    $MAKE install DPREFIX=$TMPINST_DIR || error "make install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools

    $MAKE install DPREFIX=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib
    mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mv ${TMPINST_DIR}/${PKG}/cctools/lib/*.a ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.so*
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/luajit
    mv ${TMPINST_DIR}/${PKG}/cctools/bin/luajit-2.0.0 ${TMPINST_DIR}/${PKG}/cctools/bin/luajit
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/luajit

    if test -e ${TOPDIR}/examples/$PKG ; then
	mkdir -p ${TMPINST_DIR}/${PKG}/cctools/Examples
	cp -a ${TOPDIR}/examples/$PKG ${TMPINST_DIR}/${PKG}/cctools/Examples
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/Examples/$PKG/.svn
    fi

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
