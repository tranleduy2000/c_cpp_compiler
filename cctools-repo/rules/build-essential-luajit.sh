build_build_essential_luajit() {
    PKG=build-essential-luajit
    PKG_VERSION=1.1
    PKG_DESC="Informational list of Lua JIT build-essential packages"
    PKG_DEPS="busybox project-ctl LuaJIT"
    c_tag ${PKG} && return

    banner "Build lua jit build essential"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
