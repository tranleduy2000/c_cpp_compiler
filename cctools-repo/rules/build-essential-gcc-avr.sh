build_build_essential_gcc_avr() {
    PKG=build-essential-gcc-avr
    PKG_VERSION=1.1
    PKG_DESC="Informational list of avr gcc build-essential packages"
    PKG_DEPS="busybox project-ctl binutils-avr gcc-avr avr-libc make"
    c_tag ${PKG} && return

    banner "Build gcc-avr build essential"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
