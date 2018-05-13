build_build_essential_gcc_objc_compact() {
    PKG=build-essential-gcc-objc-compact
    PKG_VERSION=1.0
    PKG_DESC="Informational list of objc compact build-essential packages"
    PKG_DEPS="busybox binutils-compact libgcc-compact-dev libstdc++-compact-dev libobjc-compact-dev gobjc make ndk-misc ndk-sysroot-\${HOSTNDKARCH}-\${HOSTNDKVERSION} cctools-examples gobjc-examples"
    c_tag ${PKG} && return

    banner "Build gcc build essential"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS" "build-essential-gcc-objc"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
