build_makerepo() {
    PKG=makerepo
    PKG_VERSION=1.0
    PKG_DESC="Create local repository from downloaded/backuped packages."
    O_DIR=${TOPDIR}/misc/makerepo
    B_DIR=${build_dir}/${PKG}

    c_tag ${PKG} && return

    banner "Build $PKG"

    $INSTALL -D -m755 ${O_DIR}/makerepo             ${TMPINST_DIR}/${PKG}/cctools/bin/makerepo
    $INSTALL -D -m644 ${O_DIR}/modules/makerepo.xml ${TMPINST_DIR}/${PKG}/cctools/share/modules/makerepo.xml

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "adialog busybox unzip"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag ${PKG}
}
