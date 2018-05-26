build_cctools_examples() {
    PKG=cctools-examples
    PKG_VERSION=1.0
    PKG_DESC="Few C, C++ and shell examples"
    c_tag ${PKG} && return

    banner "Build C, C++ and shell examples"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/Examples
    copysrc ${TOPDIR}/examples/Examples ${TMPINST_DIR}/${PKG}/cctools/Examples

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION all "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
