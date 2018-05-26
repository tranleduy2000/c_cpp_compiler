build_adialog() {
    PKG=adialog
    PKG_VERSION=1.01
    PKG_DESC="CCTools console dialog utility"
    O_DIR=${TOPDIR}/misc/adialog
    B_DIR=${build_dir}/${PKG}

    c_tag ${PKG} && return

    banner "Build $PKG"

    preparesrc $O_DIR $B_DIR

    cd $B_DIR

    make CC=${TARGET_ARCH}-gcc || error "build adialog"

    ${TARGET_ARCH}-strip adialog

    install -D -m 755 adialog ${TMPINST_DIR}/${PKG}/cctools/bin/adialog

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag ${PKG}
}
