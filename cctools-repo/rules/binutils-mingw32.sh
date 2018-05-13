build_binutils_mingw32() {
    PKG=binutils-mingw-w64
    PKG_VERSION=$binutils_version
    PKG_DESC="Cross-binutils for Win32 (x86) using MinGW-w64"
    O_DIR=$SRC_PREFIX/binutils/binutils-${PKG_VERSION}
    S_DIR=$src_dir/binutils-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}-${1}

    c_tag ${PKG}-${1} && return

    banner "Build $PKG"

    pushd .

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure \
	--target=$1 \
	--host=$TARGET_ARCH \
	--prefix=$TARGET_DIR \
	--disable-nls \
	--disable-werror \
	LDFLAGS="-Wl,-rpath-link,${SYSROOT}/usr/lib" \
	|| error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    local WARCH
    local WTARG
    case $1 in
    i*86*)
	WTARG=Win32
	WARCH=${1/-*}
	;;
    *)
	WTARG=Win64
	WARCH=x86-64
	;;
    esac

    $MAKE install prefix=${TMPINST_DIR}/${PKG}-${WARCH}/cctools || error "package install"

    for f in ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/${1}/bin/*; do
	ln -sf ../../bin/${1}-${f/*\/} ${f}
    done

    ln -sf ${1}-ld ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/bin/${1}-ld.bfd

    rm -rf ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/lib
    rm -rf ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/share

    ${STRIP} ${TMPINST_DIR}/${PKG}-${WARCH}/cctools/bin/*

    PKG_DESC="Cross-binutils for $WTARG ($WARCH) using MinGW-w64"

    local filename="${PKG}-${WARCH}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG}-${WARCH} $filename ${PKG}-${WARCH} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}-${WARCH}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag ${PKG}-${1}
}
