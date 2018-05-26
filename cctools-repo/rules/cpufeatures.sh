build_cpufeatures() {
    PKG=cpufeatures
    PKG_VERSION=1.0
    PKG_DESC="Detect cpu features"
    PKG_DEPS=""

    c_tag $FUNCNAME && return

    banner "Build $PKG"

    pushd .

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools

    local SRCDIR=${TOP_DIR}/misc/cpufeatures
    if [ -d ${NDK_DIR}/sources/android/cpufeatures ]; then
	SRCDIR=${NDK_DIR}/sources/android/cpufeatures
    fi
    ${TARGET_ARCH}-gcc \
        -O2 -Wall \
        -I$SRCDIR  \
        ${SRCDIR}/cpu-features.c -shared \
        -o ${SYSROOT}/usr/lib/libcpufeatures.so
    $INSTALL -D -m 644 ${SRCDIR}/cpu-features.h ${SYSROOT}/usr/include/cpu-features.h

    $INSTALL -D -m 755 ${SYSROOT}/usr/lib/libcpufeatures.so ${TMPINST_DIR}/${PKG}/cctools/lib/libcpufeatures.so
    $INSTALL -D -m 644 ${SRCDIR}/cpu-features.h             ${TMPINST_DIR}/${PKG}/cctools/include/cpu-features.h

    make_packages

    popd
    s_tag $FUNCNAME
}
