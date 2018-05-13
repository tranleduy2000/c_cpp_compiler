build_build_essential_fortran() {
    PKG=build-essential-fortran
    PKG_VERSION=1.0
    PKG_DESC="Informational list of fortran build-essential packages"
    PKG_DEPS="busybox binutils libgcc-dev libstdc++-dev libgfortran-dev gfortran make ndk-misc ndk-sysroot-\${HOSTNDKARCH}-\${HOSTNDKVERSION} cctools-examples gfortran-examples"
    c_tag ${PKG} && return

    banner "Build fortran build essential"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS" "build-essential-fortran-compact"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
