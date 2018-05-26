build_native_binutils() {
    PKG=binutils
    PKG_VERSION=2.29.1
    PKG_URL="http://ftp.gnu.org/gnu/binutils/${PKG}-${PKG_VERSION}.tar.xz"
    PKG_DESC="GNU assembler, linker and binary utilities"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.xz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $FUNCNAME && return

    banner "Build $PKG"

    pushd .

    if [ ! -f $O_FILE ]; then
	download $PKG_URL $O_FILE
    fi

    unpack $src_dir $O_FILE
    patchsrc $S_DIR $PKG $PKG_VERSION

    if [ "$USE_NATIVE_BUILD" = "yes" ]; then
	fix_bionic_shell $S_DIR
    fi

    mkdir -p $B_DIR
    cd $B_DIR

#    export ac_cv_func_strtod=no
    LIBS=-lm \
    $S_DIR/configure \
	--prefix=$TARGET_DIR \
	--target=$TARGET_ARCH \
	--with-sysroot=$SYSROOT \
	--enable-targets=arm-linux-androideabi,mipsel-linux-android,i686-linux-android,aarch64-linux-android,mips64el-linux-android,x86_64-linux-android \
	--enable-multilib \
	--disable-nls \
	--disable-static \
	--enable-shared \
	--disable-werror \
	LDFLAGS="-Wl,-rpath-link,${SYSROOT}/usr/lib" \
	|| error "configure"


#	--host=$TARGET_ARCH \
#	--target=$TARGET_ARCH \


    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    strip ${TMPINST_DIR}/${PKG}/cctools/bin/*
    strip ${TMPINST_DIR}/${PKG}/cctools/lib/*.so

    ln -sf ld ${TMPINST_DIR}/${PKG}/cctools/bin/ld.bfd

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/bin
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/libbfd.*a
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/libiberty.a
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/libopcodes.*a

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "" "binutils-compact"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $FUNCNAME

    PKG=binutils-compact
    PKG_DESC="GNU assembler and linker only, compact version"
    rm -rf ${TMPINST_DIR}/binutils-compact
    cp -a ${TMPINST_DIR}/binutils ${TMPINST_DIR}/binutils-compact
    for f in addr2line ar c++filt elfedit gprof nm objcopy objdump ranlib readelf size strings strip; do
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/bin/$f
    done
    case $TARGET_ARCH in
    arm*)
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/*btsmip*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/*ltsmip*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/elf_*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/elf32_*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/i386*
	;;
    mips*)
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/armelf*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/elf_*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/elf32_*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/i386*
	;;
    i*86*|x86*)
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/armelf*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/*btsmip*
	rm -f ${TMPINST_DIR}/binutils-compact/cctools/${TARGET_ARCH}/lib/ldscripts/*ltsmip*
	;;
    esac
    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC" "" "binutils"
    cd ${TMPINST_DIR}/${PKG}
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc
}
