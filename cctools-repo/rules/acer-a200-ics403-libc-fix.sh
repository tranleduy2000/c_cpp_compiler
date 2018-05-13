build_acer_a200_ics403_libc_fix() {
    PKG=acer-a200-ics403-libc-fix
    PKG_VERSION=4.0.3
    PKG_DESC="ACER Iconia TAB A200 ICS 4.0.3 applications crash fix. Install this package only if you have this device and official firmware."

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

    copysrc ${TOPDIR}/misc/AcerA200-ICS4.0.3/cctools ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    rm -f ${TMPINST_DIR}/libso/${PKG}.txt

    s_tag $PKG
}
