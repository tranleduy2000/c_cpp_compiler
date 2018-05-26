build_build_helper() {
    PKG=build-helper
    PKG_VERSION=1.00
    PKG_DESC="CCTools helpers utilities and modules"
    B_DIR=${build_dir}/${PKG}

    c_tag ${PKG} && return

    banner "Build $PKG"

    install -D -m 755 ${TOPDIR}/misc/build-helper/build-helper ${TMPINST_DIR}/${PKG}/cctools/bin/build-helper
    install -D -m 755 ${TOPDIR}/misc/build-helper/run-helper   ${TMPINST_DIR}/${PKG}/cctools/bin/run-helper
    install -D -m 644 ${TOPDIR}/misc/build-helper/compile.xml ${TMPINST_DIR}/${PKG}/cctools/share/modules/compile.xml
    install -D -m 644 ${TOPDIR}/misc/build-helper/runargs.xml ${TMPINST_DIR}/${PKG}/cctools/share/modules/runargs.xml

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag ${PKG}
}
