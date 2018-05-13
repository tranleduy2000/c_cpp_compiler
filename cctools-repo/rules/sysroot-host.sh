build_sysroot_host() {
    PKG=sysroot

    c_tag ${PKG}-host && return

    banner "Build $PKG host"

    pushd .

    local FROM="${NDK_DIR}/platforms/android"

    case $TARGET_ARCH in
    arm*)
	FROM="${FROM}-9/arch-arm"
	;;
    mips*)
	FROM="${FROM}-9/arch-mips"
	;;
    *86*)
	FROM="${FROM}-9/arch-x86"
	;;
    *)
	error "host sysroot - unknown target"
	;;
    esac

    copysrc $FROM $SYSROOT

    rm -rf ${SYSROOT}/usr/lib/libstdc++.*

    popd
    s_tag ${PKG}-host
}
