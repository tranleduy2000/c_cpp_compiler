build_project_ctl() {
    PKG=project-ctl
    PKG_VERSION=1.0
    PKG_DESC="Control project inside a project directory."
    PKG_DEPS="adialog busybox"

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    $INSTALL -D -m 755 ${TOPDIR}/misc/project-ctl/project-ctl ${TMPINST_DIR}/${PKG}/cctools/bin/project-ctl

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag $PKG
}
