build_busybox() {
    PKG=busybox
    PKG_VERSION=$busybox_version
    PKG_SUBVERSION="-2"
    PKG_DESC="BusyBox provides several stripped-down Unix tools in a single executable."
    PKG_URL="http://busybox.net/downloads/busybox-$busybox_version.tar.bz2"
    O_FILE=$SRC_PREFIX/busybox/busybox-$busybox_version.tar.bz2
    S_DIR=$src_dir/busybox-$busybox_version
    B_DIR=$build_dir/busybox
    c_tag $PKG && return

    banner "Build $PKG"

    pushd .
    mkdir -p $SRC_PREFIX/busybox
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/busybox-$busybox_version.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR
    cp -f ${TOPDIR}/configs/busybox .config
    sed -i -e "s|@CROSS@|${TARGET_ARCH_GLIBC}-|" .config
    $MAKE oldconfig

    case "$TARGET_ARCH" in
    mips*)
	EXTRA_CFLAGS="-EL"
	EXTRA_LDFLAGS="-EL"
	;;
    esac

    $MAKE $MAKEARGS CONFIG_EXTRA_CFLAGS="$EXTRA_CFLAGS" CONFIG_EXTRA_LDFLAGS="$EXTRA_LDFLAGS" || error "make $MAKEARGS"

    #$MAKE install || error "make install"

    #rm -rf $TARGET_DIR/share

    install -D -m 755 busybox ${TMPINST_DIR}/bin/busybox

    $MAKE install \
	CONFIG_EXTRA_CFLAGS="$EXTRA_CFLAGS" \
	CONFIG_EXTRA_LDFLAGS="$EXTRA_LDFLAGS" \
	CONFIG_PREFIX=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
