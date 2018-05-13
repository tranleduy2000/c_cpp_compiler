build_cxxstl() {
    PKG=cxxstl
    PKG_VERSION=$gcc_version
    PKG_DESC="GNU libstdc++-v3 C++ Standard Template Library implementation"

    c_tag $PKG && return

    banner "Build cxxstl"

    local src_dir="${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/$gcc_version"
    local inc_dir="${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/c++/$gcc_version"

    copysrc $src_dir/include $inc_dir
    case $TARGET_ARCH in
    mips*)
	copysrc $src_dir/libs/mips/include/bits $inc_dir/$TARGET_ARCH/bits
	$INSTALL -D -m 644 $src_dir/libs/mips/libgnustl_shared.so ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libgnustl_shared.so
	$INSTALL -D -m 644 $src_dir/libs/mips/libgnustl_static.a  ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libstdc++.a
	$INSTALL -D -m 644 $src_dir/libs/mips/libsupc++.a         ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libsupc++.a
	;;
    arm*)
	copysrc $src_dir/libs/armeabi/include/bits $inc_dir/$TARGET_ARCH/bits
	$INSTALL -D -m 644 $src_dir/libs/armeabi/libgnustl_shared.so ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libgnustl_shared.so
	$INSTALL -D -m 644 $src_dir/libs/armeabi/libgnustl_static.a  ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libstdc++.a
	$INSTALL -D -m 644 $src_dir/libs/armeabi/libsupc++.a         ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libsupc++.a

	mkdir -p $inc_dir/$TARGET_ARCH/thumb
	ln -sf ../bits $inc_dir/$TARGET_ARCH/thumb/bits
	$INSTALL -D -m 644 $src_dir/libs/armeabi/thumb/libgnustl_shared.so ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/thumb/libgnustl_shared.so
	$INSTALL -D -m 644 $src_dir/libs/armeabi/thumb/libgnustl_static.a  ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/thumb/libstdc++.a
	$INSTALL -D -m 644 $src_dir/libs/armeabi/thumb/libsupc++.a         ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/thumb/libsupc++.a

	copysrc $src_dir/libs/armeabi-v7a/include/bits $inc_dir/$TARGET_ARCH/armv7-a/bits
	$INSTALL -D -m 644 $src_dir/libs/armeabi-v7a/libgnustl_shared.so ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/armv7-a/libgnustl_shared.so
	$INSTALL -D -m 644 $src_dir/libs/armeabi-v7a/libgnustl_static.a  ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/armv7-a/libstdc++.a
	$INSTALL -D -m 644 $src_dir/libs/armeabi-v7a/libsupc++.a         ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/armv7-a/libsupc++.a

	mkdir -p $inc_dir/$TARGET_ARCH/armv7-a/thumb
	ln -sf ../bits $inc_dir/$TARGET_ARCH/armv7-a/thumb/bits
	$INSTALL -D -m 644 $src_dir/libs/armeabi-v7a/thumb/libgnustl_shared.so ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/armv7-a/thumb/libgnustl_shared.so
	$INSTALL -D -m 644 $src_dir/libs/armeabi-v7a/thumb/libgnustl_static.a  ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/armv7-a/thumb/libstdc++.a
	$INSTALL -D -m 644 $src_dir/libs/armeabi-v7a/thumb/libsupc++.a         ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/armv7-a/thumb/libsupc++.a
	;;
    i*86*)
	copysrc $src_dir/libs/x86/include/bits $inc_dir/$TARGET_ARCH/bits
	$INSTALL -D -m 644 $src_dir/libs/x86/libgnustl_shared.so ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libgnustl_shared.so
	$INSTALL -D -m 644 $src_dir/libs/x86/libgnustl_static.a  ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libstdc++.a
	$INSTALL -D -m 644 $src_dir/libs/x86/libsupc++.a         ${TMPINST_DIR}/${PKG}/cctools/$TARGET_ARCH/lib/libsupc++.a
	;;
    *)
	error "unknown arch!"
	;;
    esac

    local filename="${PKG}-dev_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-dev $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    # Cross package
    local filename="${PKG}-dev-${PKG_ARCH}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-dev-${PKG_ARCH} $PKG_VERSION all "$PKG_DESC"
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    if [ "$PKG_ARCH" = "armel" ]; then
	cp -f ${REPO_DIR}/$filename ${REPO_DIR}/../mips/
	cp -f ${REPO_DIR}/$filename ${REPO_DIR}/../x86/
    fi

    if [ "$PKG_ARCH" = "mipsel" ]; then
	cp -f ${REPO_DIR}/$filename ${REPO_DIR}/../armeabi/
	cp -f ${REPO_DIR}/$filename ${REPO_DIR}/../x86/
    fi

    if [ "$PKG_ARCH" = "i686" ]; then
	cp -f ${REPO_DIR}/$filename ${REPO_DIR}/../armeabi/
	cp -f ${REPO_DIR}/$filename ${REPO_DIR}/../mips/
    fi

    rm -f ${REPO_DIR}/$filename

    s_tag $PKG
}
