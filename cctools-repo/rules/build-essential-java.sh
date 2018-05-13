build_build_essential_java() {
    PKG=build-essential-java
    PKG_VERSION=1.0
    local SDK_VERSION=19
    PKG_DESC="Informational list of java build-essential packages"
    PKG_DEPS="adialog busybox sdktools sdk-android-${SDK_VERSION} java-examples"
    c_tag ${PKG} && return

    banner "Build java build essential"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local filename="${PKG}_${PKG_VERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag ${PKG}
}
