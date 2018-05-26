build_cppcheck_module() {
    PKG=cppcheck-module
    PKG_VERSION=1.0
    PKG_DESC="Add cppcheck modules to CCTools."
    O_DIR=${TOPDIR}/misc/cppcheck
    B_DIR=${build_dir}/${PKG}

    c_tag ${PKG} && return

    banner "Build $PKG"

    $INSTALL -D -m644 ${O_DIR}/modules/cppcheck.xml     ${TMPINST_DIR}/${PKG}/cctools/share/modules/cppcheck.xml
    #$INSTALL -D -m644 ${O_DIR}/modules/cppcheckfile.xml ${TMPINST_DIR}/${PKG}/cctools/share/modules/cppcheckfile.xml

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "cppcheck"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag ${PKG}
}
