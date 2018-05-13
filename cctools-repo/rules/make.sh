build_make() {
    PKG=make
    PKG_VERSION=$make_version
    PKG_URL="http://ftp.gnu.org/gnu/make/make-$make_version.tar.bz2"
    PKG_DESC="An utility for Directing compilation."
    O_FILE=$SRC_PREFIX/make/make-$make_version.tar.bz2
    S_DIR=$src_dir/make-$make_version
    B_DIR=$build_dir/make

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    mkdir -p $B_DIR
    cd $B_DIR

    case $TARGET_ARCH in
    arm*)
	cp -f ${TOPDIR}/configs/config.cache-arm config.cache
	;;
    mips*)
	cp -f ${TOPDIR}/configs/config.cache-mips config.cache
	;;
    i*86*|x86*)
	cp -f ${TOPDIR}/configs/config.cache-x86 config.cache
	;;
    esac

    CFLAGS="-g -O2 -DNO_ARCHIVES" $S_DIR/configure --target=$TARGET_ARCH --host=$TARGET_ARCH --prefix=$TARGET_DIR --disable-werror -C || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
