build_build_essential_clang_objc() {
    PKG=build-essential-clang-objc
    PKG_VERSION=1.1
    PKG_DESC="Informational list of clang ObjC build-essential packages"
    PKG_DEPS="busybox project-ctl binutils libgcc-dev libstdc++-dev clang libobjc-dev make ndk-misc ndk-sysroot-\${HOSTNDKARCH}-\${HOSTNDKVERSION} cctools-examples gobjc-examples"
    c_tag ${PKG} && return

    banner "Build clang objc build essential"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS" "build-essential-clang-objc-compact"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
