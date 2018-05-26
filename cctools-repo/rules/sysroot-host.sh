build_sysroot_host() {
    PKG=sysroot

    c_tag ${PKG}-host && return

    banner "Build $PKG host"

    pushd .

    local FROM="${NDK_DIR}/platforms/android"


    fix_sysroot() {
	rm -f ${SYSROOT}/usr/${1}/libstdc++.*
	test -e ${2}/libgnustl_shared.so && cp -f ${2}/libgnustl_shared.so ${SYSROOT}/usr/${1}/
	test -e ${2}/libgnustl_static.a  && cp -f ${2}/libgnustl_static.a  ${SYSROOT}/usr/${1}/
	test -e ${2}/libsupc++.a         && cp -f ${2}/libsupc++.a         ${SYSROOT}/usr/${1}/

	if test -e ${SYSROOT}/usr/${1}/libc.so; then

	    cp ${SYSROOT}/usr/${1}/libc.so ${SYSROOT}/usr/${1}/libc.so.1
	    rm -f ${SYSROOT}/usr/${1}/libc.so

	    cat > ${SYSROOT}/usr/${1}/libc.so <<EOF
OUTPUT_FORMAT($3)
GROUP ( libc.so.1 libc.a )
EOF
	fi

	if test -e ${SYSROOT}/usr/${1}/libgnustl_shared.so; then

	    cat > ${SYSROOT}/usr/${1}/libstdc++.so <<EOF
OUTPUT_FORMAT($3)
GROUP ( libgnustl_shared.so )
EOF
	fi

	if test -e ${SYSROOT}/usr/${1}/libgnustl_static.a; then
	    cat > ${SYSROOT}/usr/${1}/libstdc++.a <<EOF
OUTPUT_FORMAT($3)
GROUP ( libgnustl_static.a )
EOF
	fi
    }


    case $TARGET_ARCH in
    arm64*|aarch64*)
	copysrc ${FROM}-21/arch-arm64 $SYSROOT

	fix_sysroot lib ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/arm64-v8a elf64-littleaarch64
	;;
    mips64*)
	copysrc ${FROM}-21/arch-mips64 $SYSROOT

	fix_sysroot lib     ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/mips   elf32-tradlittlemips
	fix_sysroot libr2   ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/mips   elf32-tradlittlemips
	fix_sysroot libr6   ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/mips   elf32-tradlittlemips
	fix_sysroot lib64   ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/mips64 elf64-tradlittlemips
	fix_sysroot lib64r2 ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/mips64 elf64-tradlittlemips
	;;
    x86_64*|amd64*)
	copysrc ${FROM}-21/arch-x86_64 $SYSROOT

	fix_sysroot lib    ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/x86    elf32-i386
	fix_sysroot lib64  ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/x86_64 elf64-x86-64
	fix_sysroot libx32 ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/x32    elf32-x86-64
	;;
    arm*)
	copysrc ${FROM}-9/arch-arm $SYSROOT
	mkdir -p ${SYSROOT}/usr/lib/armeabi-v7a

	fix_sysroot lib             ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/armeabi     elf32-littlearm
	fix_sysroot lib/armeabi-v7a ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/armeabi-v7a elf32-littlearm
	;;
    mips*)
	copysrc ${FROM}-9/arch-mips $SYSROOT

	fix_sysroot lib   ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/mips elf32-tradlittlemips
	fix_sysroot libr2 ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/mips elf32-tradlittlemips
	fix_sysroot libr6 ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/mips elf32-tradlittlemips
	;;
    *86*)
	copysrc ${FROM}-9/arch-x86 $SYSROOT

	fix_sysroot lib ${NDK_DIR}/sources/cxx-stl/gnu-libstdc++/${gcc_version}/libs/x86 elf32-i386
	;;
    *)
	error "host sysroot - unknown target"
	;;
    esac

    popd
    s_tag ${PKG}-host
}
