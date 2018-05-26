build_android_pre_233_libc_fix() {
    PKG=android-pre-233-libc-fix
    PKG_VERSION=2.3.2
    PKG_DESC="libc/c++ runtime fix for androids older than 2.3.3"

    case $TARGET_ARCH in
    arm*)
	;;
    *)
	return
	;;
    esac

    c_tag $PKG && return

    banner "Build $PKG"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    copysrc ${TOPDIR}/misc/Android-pre-2.3.3-libc-fix/cctools ${TMPINST_DIR}/${PKG}/cctools

    find ${TMPINST_DIR}/${PKG}/cctools -name "*.bin" | while read f; do
	mv $f ${f/\.bin}
    done

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    rm -f ${TMPINST_DIR}/libso/${PKG}.txt

    s_tag $PKG
}
