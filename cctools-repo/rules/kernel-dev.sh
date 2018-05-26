build_kernel_dev() {
    PKG=kernel-dev
    PKG_VERSION=1.0
    PKG_DESC="Install packages for native kernel compilation"
    PKG_DEPS="build-essential-gcc ncurses perl kernel-dev-fix"

    c_tag $PKG && return

    banner "Build $PKG"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag $PKG
}
