build_gcc_host() {
    PKG=gcc
    PKG_VERSION=$gcc_version
    PKG_DESC="The GNU fortran compiler"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}

    c_tag ${PKG}-host && return

    banner "Build $PKG host"

    pushd .

    preparesrc $O_DIR $S_DIR

    if [ "$BUILD_PIE_COMPILER" = "yes" ]; then
	cd $S_DIR
	if [ ! -e ${S_DIR}/.pie_patched ]; then
	    patch -p1 < ${patch_dir}/${PKG}-${PKG_VERSION}-pie.patch || error "No pie patch found!"
	    touch ${S_DIR}/.pie_patched
	fi
    fi

    mkdir -p $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

    local EXTRA_CONF=
    case $TARGET_ARCH in
    aarch64*)
	EXTRA_CONF="--enable-fix-cortex-a53-835769"
	;;
    mips64el*)
	EXTRA_CONF="--with-arch=mips64r6 --disable-fixed-point"
	;;
    x86_64*)
	EXTRA_CONF="--with-arch=x86-64 --with-tune=intel --with-fpmath=sse --with-multilib-list=m32,m64,mx32 --disable-libcilkrts"
	;;
    mips*)
	EXTRA_CONF="--with-arch=mips32 --disable-threads --disable-fixed-point"
	;;
    arm*)
	EXTRA_CONF="--with-arch=armv5te --with-float=soft --with-fpu=vfp"
	#EXTRA_CONF="--with-arch=armv7-a --with-float=softfp --with-fpu=vfpv3-d16"
	;;
    *86*)
	EXTRA_CONF="--disable-libquadmath-support --disable-libcilkrts"
	;;
    *)
	;;
    esac

    $S_DIR/configure \
	--target=$TARGET_ARCH \
	--host=x86_64-linux-gnu \
	--prefix=${TARGET_DIR}-host \
	--build=x86_64-linux-gnu \
	--with-gnu-as \
	--with-gnu-ld \
	--enable-languages=c,c++,fortran,objc,obj-c++ \
	--enable-bionic-libs \
	--enable-libatomic-ifuncs=no \
	--with-gmp=${TARGET_DIR}-host \
	--with-mpfr=${TARGET_DIR}-host \
	--with-mpc=${TARGET_DIR}-host \
	--with-cloog=${TARGET_DIR}-host \
	--with-isl=${TARGET_DIR}-host \
	--with-ppl=${TARGET_DIR}-host \
	--disable-ppl-version-check \
	--disable-cloog-version-check \
	--disable-isl-version-check \
	--enable-cloog-backend=isl \
	--disable-libssp \
	--enable-threads \
	--disable-libmudflap \
	--disable-sjlj-exceptions \
	--disable-shared \
	--disable-tls \
	--disable-libitm \
	--enable-initfini-array \
	--disable-nls \
	--with-binutils-version=$binutils_version \
	--with-mpfr-version=$mpfr_version \
	--with-mpc-version=$mpc_version \
	--with-gmp-version=$gmp_version \
	--with-gcc-version=$gcc_version \
	--with-cloog-version=$cloog_version \
	--with-isl-version=$isl_version \
	--disable-bootstrap \
	--disable-libquadmath \
	--enable-plugins \
	--enable-libgomp \
	--disable-libsanitizer \
	--enable-graphite=yes \
	--with-sysroot=$SYSROOT \
	--enable-objc-gc \
	--enable-eh-frame-hdr-for-static \
	--enable-target-optspace \
	--with-host-libstdcxx='-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm' \
	--with-gxx-include-dir=${TARGET_DIR}-host/${TARGET_ARCH}/include/c++/${gcc_version} \
	$EXTRA_CONF \
	|| error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host
}
