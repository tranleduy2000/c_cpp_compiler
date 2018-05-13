build_mingw_w64() {
    PKG=mingw-w64
    PKG_VERSION=v3.0.0
    PKG_SUBVERSION=
    PKG_URL="https://downloads.sourceforge.net/project/mingw-w64/mingw-w64/mingw-w64-release/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="$1 development files"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}-${1}

    c_tag ${PKG}-${1} && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    #cp -R ${S_DIR}/mingw-w64-headers/include ${TARGET_DIR}-host/${1}/ || error "headers"

    mkdir -p mingw-w64-headers
    cd mingw-w64-headers

    ${S_DIR}/mingw-w64-headers/configure	\
	--host=$1 \
	--target=$1 \
	--prefix=${TARGET_DIR}-host/${1} \
	|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"
    $MAKE install || error "make install"

    cd ..

    ${S_DIR}/configure	\
	--host=$1 \
	--target=$1 \
	--prefix=${TARGET_DIR}-host/${1} \
	|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"
    $MAKE install || error "make install"

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

    $MAKE install prefix=${TMPINST_DIR}/${PKG}-${WARCH}-dev/cctools/${1} || error "package install"

    PKG_DESC="Development files for MinGW-w64 targeting $WTARG"

    local filename="${PKG}-${WARCH}-dev_${PKG_VERSION}${PKG_SUBVERSION}_all.zip"
    build_package_desc ${TMPINST_DIR}/${PKG}-${WARCH}-dev $filename ${PKG}-${WARCH}-dev ${PKG_VERSION}${PKG_SUBVERSION} all "$PKG_DESC" "$PKG_DEPS"
    cd ${TMPINST_DIR}/${PKG}-${WARCH}-dev
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    popd
    s_tag ${PKG}-${1}
}
