build_objc_examples() {
    PKG=gobjc-examples
    PKG_VERSION=1.0
    PKG_DESC="GNU Obj-C examples"
    c_tag ${PKG} && return

    banner "Build objc examples"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/Examples
    cp -a ${TOPDIR}/examples/hello-obj-c ${TMPINST_DIR}/${PKG}/cctools/Examples
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/Examples/$PKG/.svn

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
