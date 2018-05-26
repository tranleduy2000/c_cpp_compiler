build_busybox() {
    PKG=busybox
    PKG_VERSION=1.25.0
    PKG_SUBVERSION="-1"
    PKG_DESC="BusyBox provides several stripped-down Unix tools in a single executable."
    PKG_URL="http://busybox.net/downloads/busybox-$PKG_VERSION.tar.bz2"
    O_FILE=$SRC_PREFIX/busybox/busybox-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/busybox-$PKG_VERSION
    B_DIR=$build_dir/busybox
    c_tag $PKG && return

    banner "Build $PKG"

    pushd .
    mkdir -p $SRC_PREFIX/busybox
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/busybox-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    case $TARGET_ARCH in
    aarch64*|mips64*|x86_64*)
	cp -f ${TOPDIR}/configs/busybox-${PKG_VERSION} .config || error "no config file"
	;;
    *)
	patch -p1 < $patch_dir/busybox-$PKG_VERSION-pre21.patch || error "patch"
	cp -f ${TOPDIR}/configs/busybox-${PKG_VERSION}-pre21 .config || error "no config file"
	#sed -i -e 's|CONFIG_LFS=y|# CONFIG_LFS is not set|' .config
	;;
    esac

    if [ "$BUILD_PIE_COMPILER" = "yes" ]; then

	sed -i -e "s|# CONFIG_PIE is not set|CONFIG_PIE=y|" .config

    fi

    $MAKE CROSS_COMPILE=${TARGET_ARCH}- oldconfig

    $MAKE $MAKEARGS CROSS_COMPILE=${TARGET_ARCH}- || error "make $MAKEARGS"

    install -D -m 755 busybox ${TMPINST_DIR}/bin/busybox

    $MAKE install \
	CROSS_COMPILE=${TARGET_ARCH}- \
	CONFIG_PREFIX=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -f ${TMPINST_DIR}/linuxrc

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
