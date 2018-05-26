build_ndk_sysroot() {
    PKG=ndk-sysroot
    PKG_VERSION=$ndk_version
    PKG_DESC="NDK sysroot libraries and headers"

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .

    is_invalid() {
	if [[ "$1" =~ "$2" ]]; then
	    false
	else
	    true
	fi
    }


    cd ${NDK_DIR}/platforms/

    local d=

    for d in *; do

	pushd .

	local vers=${d/*-}

	cd $d

	local a=

	for a in *; do
	    arch=${a/*-}
	    local dirs=lib
	    local x=
	    local y=
	    local z=
	    case $arch in
	    aarch64*|arm64*)
		y=aarch64
		x=aarch64-linux-android
		z=elf64-littleaarch64
		if is_invalid $TARGET_ARCH "aarch64"; then
		    continue
		fi
		;;
	    mips64*)
		dirs="lib libr2 libr6 lib64 lib64r2"
		y=mips64el
		x=mips64el-linux-android
		z=(elf32-tradlittlemips elf32-tradlittlemips elf32-tradlittlemips elf64-tradlittlemips elf64-tradlittlemips)
		if is_invalid $TARGET_ARCH "mips64"; then
		    continue
		fi
		;;
	    x86_64*|amd64*)
		dirs="lib lib64 libx32"
		y=x86-64
		x=x86_64-linux-android
		z=(elf32-i386 elf64-x86-64 elf32-x86-64)
		if is_invalid $TARGET_ARCH "x86_64"; then
		    continue
		fi
		;;
	    mips*)
		dirs="lib libr2 libr6"
		y=mipsel
		x=mipsel-linux-android
		z=(elf32-tradlittlemips elf32-tradlittlemips elf32-tradlittlemips)
		if is_invalid $TARGET_ARCH "mips"; then
		    continue
		fi
		;;
	    arm*)
		y=armel
		x=arm-linux-androideabi
		z=elf32-littlearm
		if is_invalid $TARGET_ARCH "arm"; then
		    continue
		fi
		;;
	    x86*)
		y=i686
		x=i686-linux-android
		z=elf32-i386
		if is_invalid $TARGET_ARCH "i686"; then
		    continue
		fi
		;;
	    *)
		continue
		;;
	    esac

	    if [ -d $a/usr/lib ]; then
		banner "Build ndk sysroot $arch $vers"

		rm -rf ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools

		mkdir -p ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x
		copysrc $PWD/$a/usr/include ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/include

		mkdir -p ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/usr
		ln -sf ../include ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/usr/include

		local count=0

		for l in $dirs; do
		    if test ! -d $PWD/$a/usr/$l ; then
			continue
		    fi
		    copysrc $PWD/$a/usr/$l ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/$l
		    rm -f ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/$l/libstdc++*

		    cp ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/$l/libc.so ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/$l/libc.so.1
		    rm -f ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/$l/libc.so

		    cat > ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/$l/libc.so <<EOF
OUTPUT_FORMAT(${z[$count]})
GROUP ( libc.so.1 libc.a )
EOF

		    ln -sf ../$l ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/usr/$l
		    count=$(( $count + 1))
		done

		pushd .

		local filename="${PKG}-${y}-${vers}_${PKG_VERSION}_all.zip"
		build_package_desc ${TMPINST_DIR}/${PKG}-${y}-${vers} $filename ${PKG}-${y}-${vers} $PKG_VERSION all "$PKG_DESC for $y."
		cd ${TMPINST_DIR}/${PKG}-${y}-${vers}
		rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

		popd
	    fi
	done
	popd
    done

    popd

    s_tag $PKG
}
