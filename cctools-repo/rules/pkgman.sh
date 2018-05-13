build_pkgman() {
    PKG=pkgman
    PKG_VERSION=1.0
    PKG_DESC="console package manager"
    O_DIR=${TOPDIR}/misc/pkgman
    B_DIR=${build_dir}/${PKG}

    c_tag ${PKG} && return

    banner "Build $PKG"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/bin

    cat > ${TMPINST_DIR}/${PKG}/cctools/bin/pkgman << EOF
#!/system/bin/sh
exec dalvikvm -Xss262912 -Xmx64M -cp \$CCTOOLSRES com.pdaxrom.pkgmanager.Main \$@
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/pkgman

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag ${PKG}
}
