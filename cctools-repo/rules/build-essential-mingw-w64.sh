build_build_essential_mingw_w64() {
    PKG=build-essential-mingw-w64
    PKG_VERSION=1.0
    PKG_DESC="Informational list of mingw-w64 i686 build-essential packages"
    PKG_DEPS="busybox make binutils-mingw-w64-i686 gcc-mingw-w64-i686 mingw-w64-i686-dev mingw-w64-examples"
    c_tag ${PKG} && return

    banner "Build mingw build essential"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
