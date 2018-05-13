build_ndk_sysroot() {
    PKG=ndk-sysroot
    PKG_VERSION=$ndk_version
    PKG_DESC="NDK sysroot libraries and headers"

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .

    cd ${NDK_DIR}/platforms/

    local d=

    for d in *; do

	pushd .

	local vers=${d/*-}

	cd $d

	local a=

	for a in *; do
	    arch=${a/*-}
	    local x=
	    local y=
	    case $arch in
	    mips*)
		y=mipsel
		x=mipsel-linux-android
		;;
	    arm*)
		y=armel
		x=arm-linux-androideabi
		;;
	    x86*)
		y=i686
		x=i686-linux-android
		;;
	    *)
		continue
		;;
	    esac
	    if [ -d $a/usr/lib ]; then
		banner "Build ndk sysroot $arch $vers"

		mkdir -p ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x
		copysrc $PWD/$a/usr/include ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/include
		copysrc $PWD/$a/usr/lib     ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/lib
		rm -f ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/lib/libstdc++*

		mkdir -p ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/usr
		ln -sf ../include ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/usr/include
		ln -sf ../lib     ${TMPINST_DIR}/${PKG}-${y}-${vers}/cctools/$x/usr/lib
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
