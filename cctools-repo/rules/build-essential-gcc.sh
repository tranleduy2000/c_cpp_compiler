build_build_essential_gcc() {
    PKG=build-essential-gcc
    PKG_VERSION=1.0
    PKG_DESC="Informational list of gcc build-essential packages"
    PKG_DEPS="busybox binutils libgcc-dev libstdc++-dev gcc make ndk-misc ndk-sysroot-\${HOSTNDKARCH}-\${HOSTNDKVERSION} cctools-examples"
    c_tag ${PKG} && return

    banner "Build gcc build essential"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS" "build-essential-gcc-compact"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
