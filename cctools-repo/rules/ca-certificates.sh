build_ca_certificates() {
    PKG=ca-certificates
    PKG_VERSION=1.0
    PKG_DESC="Common CA certificates"

    c_tag ${PKG} && return

    banner "Build $PKG"

    install -D -m 644 /etc/ssl/certs/ca-certificates.crt ${TMPINST_DIR}/${PKG}/cctools/etc/ssl/certs/ca-certificates.crt

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag ${PKG}
}
