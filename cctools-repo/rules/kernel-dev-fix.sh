build_kernel_dev_fix() {
    PKG=kernel-dev-fix
    PKG_VERSION=1.0
    PKG_DESC="Fix native kernel compilation with bionic toolchain"
    PKG_DEPS="ndk-sysroot-\${HOSTNDKARCH}-\${HOSTNDKVERSION}"

    c_tag $PKG && return

    banner "Build $PKG"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    $INSTALL -D -m 644 ${TOPDIR}/misc/kernel-dev-fix/elf-fix.h ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/elf-fix.h

    cat >> ${TMPINST_DIR}/${PKG}/postinst << EOF
#!/system/bin/sh

if [ ! -f \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf-bionic.h ]; then
    mv \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf.h     \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf-bionic.h
    cp \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf-fix.h \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf.h
fi
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/postinst

    cat >> ${TMPINST_DIR}/${PKG}/prerm << EOF
#!/system/bin/sh

if [ -f \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf-bionic.h ]; then
    rm -f \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf.h
    mv    \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf-bionic.h \${CCTOOLSDIR}/${TARGET_ARCH}/include/elf.h
fi
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/prerm

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    s_tag $PKG
}
