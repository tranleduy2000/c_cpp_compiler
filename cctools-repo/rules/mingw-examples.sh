build_mingw_w64_examples() {
    PKG=mingw-w64-examples
    PKG_VERSION=1.0
    PKG_DESC="Few C, C++ examples for MinGW W64 crosscompiler"
    c_tag ${PKG} && return

    banner "Build C, C++ and shell examples"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/Examples
    copysrc ${TOPDIR}/examples/mingw-w64-i686 ${TMPINST_DIR}/${PKG}/cctools/Examples/mingw-w64-i686

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION all "$PKG_DESC" "gcc-mingw-w64-i686 binutils-mingw-w64-i686 mingw-w64-i686-dev"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag ${PKG}
}
