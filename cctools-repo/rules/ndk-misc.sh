build_ndk_misc() {
    PKG=ndk-misc
    PKG_VERSION=$ndk_version
    PKG_DESC="NDK misc files"

    c_tag $PKG && return

    banner "Build $PKG"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    copysrc ${NDK_DIR}/sources/android/cpufeatures     ${TMPINST_DIR}/${PKG}/cctools/sources/cpufeatures
    copysrc ${NDK_DIR}/sources/android/native_app_glue ${TMPINST_DIR}/${PKG}/cctools/sources/native_app_glue

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/bin
    echo "#!/system/bin/sh" 			   > ${TMPINST_DIR}/${PKG}/cctools/bin/run_na
    echo "echo \"\$PWD/\$1\" > \$TMPDIR/runme_na" >> ${TMPINST_DIR}/${PKG}/cctools/bin/run_na
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/run_na

    echo "#!/system/bin/sh" 			   > ${TMPINST_DIR}/${PKG}/cctools/bin/run_ca
    echo "echo \"\$PWD/\$1\" > \$TMPDIR/runme_ca" >> ${TMPINST_DIR}/${PKG}/cctools/bin/run_ca
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/run_ca

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    s_tag $PKG
}
