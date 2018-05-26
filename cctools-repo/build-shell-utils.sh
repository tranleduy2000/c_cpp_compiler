#!/bin/bash

ndk_version="r12b"

binutils_version="2.25"
gcc_version="4.9"
gmp_version="5.0.5"
mpc_version="1.0.1"
mpfr_version="3.1.1"
cloog_version="0.18.0"
isl_version="0.11.1"
ppl_version="1.0"

make_version="4.0"
nano_version="2.2.6"
emacs_version="24.2"

binutils_avr_version="2.25"
gcc_avr_version="4.9.3"

gcc_mingw_version="4.9.3"

TARGET_INST_DIR="/data/data/com.pdaxrom.cctools/root/cctools"
#TARGET_INST_DIR="/data/data/com.pdaxrom.cctools/cache/cctools"

SRC_PREFIX="$1"
TARGET_ARCH="$2"
HOST_ARCH="$2"
WORK_DIR="$3"
NDK_DIR="$4"
SDK_DIR="$5"

if [ "x$SRC_PREFIX" = "x" ]; then
    echo "No source dir"
    exit 1
fi

if [ "x$TARGET_ARCH" = "x" ]; then
    echo "No target arch"
    exit 1
fi

if [ "x$WORK_DIR" = "x" ]; then
    WORK_DIR="/tmp/native-ndk-${TARGET_ARCH}-${USER}"
fi

if [ "x$NDK_DIR" = "x" ]; then
    NDK_DIR=/mnt/devel/Work/NDK/android-ndk-r10e
fi

if [ "x$SDK_DIR" = "x" ]; then
    SDK_DIR=/mnt/devel/Work/NDK/android-sdk-linux
fi

if [ "x$MAKEARGS" = "x" ]; then
    MAKEARGS=-j4
fi

TOPDIR="$PWD"

build_dir="${WORK_DIR}/build"
src_dir="${WORK_DIR}/src"
patch_dir="${TOPDIR}/patches"

TARGET_DIR="${WORK_DIR}/cctools"
TMPINST_DIR="${build_dir}/tmpinst"

SYSROOT="${TARGET_DIR}-host/sysroot"

MAKE=make
INSTALL=install

XBUILD_ARCH=`uname -m`
BUILD_SYSTEM=`uname`

case $BUILD_SYSTEM in
Linux)
    BUILD_ARCH=${XBUILD_ARCH}-unknown-linux
    ;;
Darwin)
    BUILD_ARCH=${XBUILD_ARCH}-unknown-darwin
    ;;
CYGWIN*)
    BUILD_ARCH=${XBUILD_ARCH}-unknown-cygwin
    ;;
*)
    BUILD_ARCH=
    echo "unknown host system!"
    exit 1
    ;;
esac

case $TARGET_ARCH in
arm*|aarch64*)
    TARGET_ARCH_GLIBC=arm-none-linux-gnueabi
    ;;
mips*|mips64el*)
    TARGET_ARCH_GLIBC=mips-linux-gnu
    ;;
i*86*|x86*)
    TARGET_ARCH_GLIBC=i686-pc-linux-gnu
    ;;
*)
    echo "unknown arch $TARGET_ARCH"
    exit 1
    ;;
esac

echo "Target arch: $TARGET_ARCH"
echo "Host   arch: $HOST_ARCH"
echo "Build  arch: $BUILD_ARCH"

banner() {
    echo
    echo "*********************************************************************************"
    echo "$1"
    echo
    if [ "$TERM" = "xterm-color" -o "$TERM" = "xterm" ]; then
	echo -ne "\033]0;${1}\007"
    fi
}

trap "banner ''" 2

if ! $SHELL -c 'pushd .; popd' 2>&1 >/dev/null ; then
alias pushd='ASHDIRSTACK="$PWD
$ASHDIRSTACK"; cd'
alias popd='ASHDIRSLINE=`echo "\$ASHDIRSTACK" | sed -ne "1p"`;[ "$ASHDIRSLINE" != "" ] && cd $ASHDIRSLINE; ASHDIRSTACK=`echo "\$ASHDIRSTACK" | sed -e "1d"`'
fi

error() {
    echo
    echo "*********************************************************************************"
    echo "Error: $@"
    echo
    exit 1
}

makedirs() {
    mkdir -p $src_dir
    mkdir -p ${WORK_DIR}/tags
    mkdir -p ${TMPINST_DIR}/libso

    mkdir -p ${WORK_DIR}/../repo/armeabi
    mkdir -p ${WORK_DIR}/../repo/mips
    mkdir -p ${WORK_DIR}/../repo/x86
    mkdir -p ${WORK_DIR}/../repo/arm64-v8a
    mkdir -p ${WORK_DIR}/../repo/mips64
    mkdir -p ${WORK_DIR}/../repo/x86_64
}

s_tag() {
    touch ${WORK_DIR}/tags/$1
}

c_tag() {
    test -e ${WORK_DIR}/tags/$1
}

copysrc() {
    mkdir -p $2
    tar -C "$1" -c . | tar -C $2 -xv || error "copysrc $1 $2"
}

preparesrc() {
    if [ ! -d $2 ]; then
	pushd .
	copysrc $1 $2
	if [ -f ${patch_dir}/`basename $2`.patch ]; then
	    cd $2
	    patch -p1 < ${patch_dir}/`basename $2`.patch
	fi
	popd
    fi
}

download() {
    if [ ! -e $2 ]; then
	mkdir -p `dirname $2`
	echo "Downloading..."
	local opt
	case $1 in
	https*)
	    opt="--no-check-certificate"
	    ;;
	esac
	if wget --no-check-certificate $opt $1 -O $2 ; then
	    return
	fi
	rm -f $2

	local mirror
	for mirror in "http://mirror.cctools.info/packages/src" "http://cctools.info/packages/src"; do
	    local f=${1/*\/}
	    if wget --no-check-certificate -c ${mirror}/${f} -O $2 ; then
		return
	    fi
	    rm -f $2
	done

	error "downloading $PKG_URL"
    fi
}

unpack() {
    local cmd=

    echo "Unpacking..."

    mkdir -p $1

    case $2 in
    *.tar.gz|*.tgz)
	cmd="tar zxf $2 -C $1"
	;;
    *.tar.bz2| *.tbz)
	cmd="tar jxf $2 -C $1"
	;;
    *.tar.xz)
	cmd="tar Jxf $2 -C $1"
	;;
    *.zip)
	cmd="unzip $2 -d $1"
	;;
    *)
	error "Unknown archive type."
	;;
    esac

    $cmd || error "Corrupted archive $2."
}

patchsrc() {
    if [ -f ${patch_dir}/${2}-${3}.patch ]; then
	pushd .
	cd $1
	patch -p1 < ${patch_dir}/${2}-${3}.patch || error "Correpted patch file."
	popd
    fi
}

#
# find deps
#

get_pkg_libso_list() {
    local f
    find $1 -type f -name "*.so" -o -name "*.so.*" | while read f; do
	if ${TARGET_ARCH}-readelf -h $f 2>/dev/null | grep -q "DYN"; then
	    echo -n "`basename ${f}` "
	fi
    done
}

get_pkg_exec_list() {
    local f
    find $1 -type f | while read f; do
	if ${TARGET_ARCH}-readelf -h $f &>/dev/null; then
	    echo $f
	fi
    done
}

get_libso_list() {
    ${TARGET_ARCH}-objdump -x $1 | awk '/NEEDED/ { print $2 }'
}

get_pkg_external_libso() {
    local exes=`get_pkg_exec_list $1`
    ( for f in $exes; do
	get_libso_list $f
    done ) | sort | uniq
}

get_dep_packages() {
    #echo "Package $1"
    local f
    for f in `get_pkg_external_libso $2`; do
	local d
	for d in ${TMPINST_DIR}/libso/${1}.txt ${TMPINST_DIR}/libso/*.txt; do
	    if grep -q $f $d; then
		local p=`cat $d | cut -f1 -d:`
		if [ "$p" = "$1" ]; then
		    break
		else
		    if [[ ! "$p" =~ ^ndk-sysroot ]]; then
			echo $p
			break
		    fi
		fi
	    fi
	done
    done
}

get_pkg_deps() {
    local list=`get_pkg_libso_list $2 | sort`
    echo "$1: $list" > ${TMPINST_DIR}/libso/$1.txt
    local pkgs=`get_dep_packages $1 $2 | sort | uniq`
    echo $pkgs
}

string_to_lower() {
    echo $@ | tr '[:upper:]' '[:lower:]'
}

#
# remove RPATH from ELF
#

remove_rpath() {
    local f

    if which chrpath 2>/dev/null >/dev/null; then
	for f in $(find $1 -executable -type f); do
	    if file $f | grep -q ELF; then
		echo -n "Checking $f for RPATH ... "
		if chrpath -l $f | grep -q RPATH; then
		    echo "removed"
		    chrpath -d $f
		else
		    echo "clean"
		fi
	    fi
	done
    fi
}

#
# build_package_desc <path> <filename> <name> <version> <arch> <description> [<depends> [<replaces>]]
#

build_package_desc() {
    local filename=$2
    local name=$(string_to_lower $3)
    local vers=$4
    local arch=$5
    local desc=$6
    local replaces=$8

    local unpacked_size=`find ${1}/cctools -type f | xargs stat -c "%s" | awk '{s+=$1} END {print s}'`

    local deps="`get_pkg_deps $name $1`"
    if [ "x$7" != "x" ]; then
	if [ "x$deps" = "x" ]; then
	    deps="$7"
	else
	    deps="$deps $7"
	fi
    fi

cat >$1/pkgdesc << EOF
    <package>
	<name>$name</name>
	<replaces>$replaces</replaces>
	<version>$vers</version>
	<arch>$arch</arch>
	<description>$desc</description>
	<depends>$deps</depends>
	<size>$unpacked_size</size>
	<file>$filename</file>
	<filesize>@SIZE@</filesize>
    </package>
EOF

}

fix_bionic_shell() {
    local p="$1"
    local f=""
    if [ "x$p" = "x" ]; then
	p="."
    fi

    for f in $(find $p -type f); do
        if file $f | grep -q 'ASCII text\|shell script'; then
	    if [ ! "$(grep -q '/system/bin/sh' $f || grep '/bin/sh' $f)" = "" ]; then
		echo "fix bionic shell in $f"
		touch -r $f ${f}.timestamp
		sed -i -e 's|/bin/sh|/system/bin/sh|g' $f
		touch -r ${f}.timestamp $f
		rm -f ${f}.timestamp
	    fi
	fi
    done
}

replace_string() {
    local p="$1"
    local f=""
    if [ "x$p" = "x" ]; then
	p="."
    fi

    for f in $(find $p -type f); do
        if file $f | grep -q 'ASCII text\|shell script\|libtool library file\|XML document text'; then
	    if grep -q "$2" $f; then
		echo "replace string in $f"
		touch -r $f ${f}.timestamp
		sed -i -e "s|$2|$3|g" $f
		touch -r ${f}.timestamp $f
		rm -f ${f}.timestamp
	    fi
	fi
    done
}

make_packages() {
    local n
    local nomain=""
    local nodev=""
    local noman=""
    local nodoc=""
    local noinfo=""
    local nodeldev=""
    local pkg_new_dep="$(string_to_lower $PKG)"

    while [ ! "$1" = "" ]; do
	case $1 in
	nomain)
	    nomain="1"
	    pkg_new_dep="$PKG_DEPS"
	    ;;
	nodev)
	    nodev="1"
	    ;;
	noman)
	    noman="1"
	    ;;
	nodoc)
	    nodoc="1"
	    ;;
	noinfo)
	    noinfo="1"
	    ;;
	nodeldev)
	    nodeldev="1"
	    ;;
	esac
	shift
    done

    remove_rpath ${TMPINST_DIR}/${PKG}/cctools
    fix_bionic_shell ${TMPINST_DIR}/${PKG}/cctools
    replace_string   ${TMPINST_DIR}/${PKG}/cctools "${TMPINST_DIR}" "${TARGET_INST_DIR}"

    pushd .
    cd ${TMPINST_DIR}/${PKG}

    find . -type l | while read n; do
	if readlink $n | grep -q "$TMPINST_DIR"; then
	    local n1=$(readlink $n | sed "s|${TMPINST_DIR}/${PKG}/cctools|$TARGET_INST_DIR|")
	    echo "FIX SUMLINK: $n1 $n"
	    ln -sf "$n1" "$n"
	fi
    done

    if [ -d cctools/include -o -d cctools/lib/pkgconfig -o -d cctools/share/aclocal ]; then
	if [ ! "$nodeldev" = "1" ]; then
	    rm -rf ${TMPINST_DIR}/${PKG}-dev
	fi
	mkdir -p ${TMPINST_DIR}/${PKG}-dev
	for n in cctools/include cctools/lib/pkgconfig cctools/share/aclocal; do
	    if [ -d "$n" ]; then
		mkdir -p "$(dirname ${TMPINST_DIR}/${PKG}-dev/${n})"
		cp -R "$n" "$(dirname ${TMPINST_DIR}/${PKG}-dev/${n})"
		rm -rf "$n"
	    fi
	done
	find . -type f \( -name "*.h" -o -name "*.a" -o -name "*.la" -o -name "*.pc" -o -name "*.m4" -o -name "*.o" \) | while read n; do
	    mkdir -p ${TMPINST_DIR}/${PKG}-dev/$(dirname "$n")
	    mv "$n" ${TMPINST_DIR}/${PKG}-dev/$(dirname "$n")
	done

	if [ ! "$nodev" = "1" ]; then
	    local filename="$(string_to_lower ${PKG})-dev_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
	    build_package_desc ${TMPINST_DIR}/${PKG}-dev $filename ${PKG}-dev ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC (development files)" "$pkg_new_dep"
	    pushd .
	    cd ${TMPINST_DIR}/${PKG}-dev
	    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *
	    popd
	    if [ "$nomain" = "1" ]; then
		pkg_new_dep="$(string_to_lower ${PKG})-dev"
	    fi
	fi
    fi

    if [ -d cctools/share/man -o -d cctools/man ]; then
	for n in cctools/share/man cctools/man; do
	    if [ -d "$n" ]; then
		mkdir -p "$(dirname ${TMPINST_DIR}/${PKG}-man/${n})"
		cp -R "$n" "$(dirname ${TMPINST_DIR}/${PKG}-man/${n})"
		rm -rf "$n"
	    fi
	done

	if [ ! "$noman" = "1" ]; then
	    local filename="$(string_to_lower ${PKG})-man_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
	    build_package_desc ${TMPINST_DIR}/${PKG}-man $filename ${PKG}-man ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC (manual files)" "$pkg_new_dep"
	    pushd .
	    cd ${TMPINST_DIR}/${PKG}-man
	    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *
	    popd
	fi
    fi

    if [ -d cctools/share/doc -o -d cctools/doc -o -d cctools/share/gtk-doc ]; then
	for n in cctools/share/doc cctools/doc cctools/share/gtk-doc; do
	    if [ -d "$n" ]; then
		mkdir -p "$(dirname ${TMPINST_DIR}/${PKG}-doc/${n})"
		cp -R "$n" "$(dirname ${TMPINST_DIR}/${PKG}-doc/${n})"
		rm -rf "$n"
	    fi
	done

	if [ ! "$nodoc" = "1" ]; then
	    local filename="$(string_to_lower ${PKG})-doc_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
	    build_package_desc ${TMPINST_DIR}/${PKG}-doc $filename ${PKG}-doc ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC (doc files)" "$pkg_new_dep"
	    pushd .
	    cd ${TMPINST_DIR}/${PKG}-doc
	    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *
	    popd
	fi
    fi

    if [ -d cctools/share/info -o -d cctools/info ]; then
	for n in cctools/share/info cctools/info; do
	    if [ -d "$n" ]; then
		mkdir -p "$(dirname ${TMPINST_DIR}/${PKG}-info/${n})"
		cp -R "$n" "$(dirname ${TMPINST_DIR}/${PKG}-info/${n})"
		rm -rf "$n"
	    fi
	done

	if [ ! "$noinfo" = "1" ]; then
	    local filename="$(string_to_lower ${PKG})-info_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
	    build_package_desc ${TMPINST_DIR}/${PKG}-info $filename ${PKG}-info ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC (info files)" "$pkg_new_dep"
	    pushd .
	    cd ${TMPINST_DIR}/${PKG}-info
	    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *
	    popd
	fi
    fi

    popd

    ${STRIP} ${TMPINST_DIR}/${PKG}/cctools/bin/*
    ${STRIP} ${TMPINST_DIR}/${PKG}/cctools/lib/*.so*
    test -d ${TMPINST_DIR}/${PKG}/cctools/sbin && ${STRIP} ${TMPINST_DIR}/${PKG}/cctools/sbin/*

    if [ ! "$nomain" = "1" ]; then
	local filename="$(string_to_lower ${PKG})_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
	build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "$PKG_DEPS"
	cd ${TMPINST_DIR}/${PKG}
	rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *
    fi
}

case $TARGET_ARCH in
aarch64*)
    PKG_ARCH="aarch64"
    REPO_DIR="${WORK_DIR}/../repo/arm64-v8a"
    ;;
mips64el*)
    PKG_ARCH="mips64el"
    REPO_DIR="${WORK_DIR}/../repo/mips64"
    ;;
x86_64*)
    PKG_ARCH="x86-64"
    REPO_DIR="${WORK_DIR}/../repo/x86_64"
    ;;
arm*)
    PKG_ARCH="armel"
    REPO_DIR="${WORK_DIR}/../repo/armeabi"
    ;;
mips*)
    PKG_ARCH="mipsel"
    REPO_DIR="${WORK_DIR}/../repo/mips"
    ;;
i*86*)
    PKG_ARCH="i686"
    REPO_DIR="${WORK_DIR}/../repo/x86"
    ;;
*)
    error "Can't set PKG_ARCH from $TARGET_ARCH"
    ;;
esac

for f in $(find rules -type f -name "*.sh"); do
    echo "Include $f"
    . $f
done

export PATH=${TARGET_DIR}-host/bin:$PATH

if [ -f /system/bin/sh ]; then
    USE_NATIVE_BUILD="yes"
    STRIP="strip"
else
    STRIP=${TARGET_ARCH}-strip
fi

makedirs

if [ "$USE_NATIVE_BUILD" = "yes" ]; then

    TARGET_STRIP=strip

    build_native_perl
    build_m4

    if ! which pkgman 2>/dev/null >/dev/null; then
	echo "Please install pkgman package and restart build."
	exit 0
    fi

    makerepo -p $REPO_DIR
    pkgman install perl m4

    build_autoconf

    makerepo -p $REPO_DIR
    pkgman install autoconf

    if ! which autoconf 2>/dev/null >/dev/null; then
	echo "Please install autoconf package and restart build."
	exit 0
    fi

    build_automake

    makerepo -p $REPO_DIR
    pkgman install automake

    if ! which automake 2>/dev/null >/dev/null; then
	echo "Please install automake package and restart build."
	exit 0
    fi

    build_gawk
    build_bison
    build_flex

    makerepo -p $REPO_DIR
    pkgman install gawk bison flex

    build_gmp
    build_mpfr
    build_mpc
    build_isl
    build_ppl
    build_cloog

    build_findutils

    build_native_binutils

    build_native_gcc
exit 1

#    build_nano
#    build_vim

    exit 0
fi

# ----------------------------------------------------------

build_sysroot_host

build_gmp_host
build_mpfr_host
build_mpc_host
build_isl_host
build_ppl_host
build_cloog_host

build_binutils_host
build_gcc_host

# ----------------------------------------------------------

# Toolchain support libs
build_gmp
build_mpfr
build_mpc
build_isl
build_ppl
build_cloog

# CCTools native tools moved from bundle
build_binutils
build_gcc

#build_cxxstl

build_ndk_misc
build_ndk_sysroot
build_cctools_examples
build_fortran_examples
build_objc_examples

# Clang
#build_zlib
#build_llvm

# presets
#build_build_essential_clang
#build_build_essential_clang_objc
build_build_essential_gcc
build_build_essential_fortran
build_build_essential_gcc_avr
build_build_essential_gcc_objc
build_build_essential_gcc_objc_fortran

#build_build_essential_clang_compact
#build_build_essential_clang_objc_compact
build_build_essential_gcc_compact
build_build_essential_fortran_compact
build_build_essential_gcc_objc_compact
build_build_essential_gcc_objc_fortran_compact
build_build_essential_luajit

build_build_helper

# utils
build_busybox
build_make
build_adialog

# Addons
build_ncurses
build_libiconv
#build_libffi
#build_gettext
build_glib_host
build_glib
#build_slang
build_mc
build_htop
build_luajit
build_openssl
build_expat
build_sqlite
build_apr
build_apr_util
build_neon
build_subversion
build_curl
build_wget
build_git

build_ca_certificates
build_dropbear

##build_fpc
##build_nano
##build_emacs

build_binutils_avr_host
build_binutils_avr
build_gcc_avr_host
build_gcc_avr
build_avr_libc

build_netcat
build_file_host
build_file

build_zip
build_unzip

# Bug fixes for firmware
build_acer_a200_ics403_libc_fix
build_android_pre_233_libc_fix

# MinGW
build_binutils_mingw32_host i686-w64-mingw32
###build_binutils_mingw32_host x86_64-w64-mingw32
build_gcc_mingw32_host i686-w64-mingw32
###build_gcc_mingw32_host x86_64-w64-mingw32
build_binutils_mingw32 i686-w64-mingw32
build_gcc_mingw32 i686-w64-mingw32
build_mingw_w64_examples
build_build_essential_mingw_w64

build_libpng
build_pkgman
build_fastjar
build_sdktools
build_sdk_android
build_java_examples
build_build_essential_java

build_pcre
build_cppcheck
build_cppcheck_module

build_makerepo
build_kernel_dev_fix
build_kernel_dev

build_project_ctl

export PKG_CONFIG_PATH=${TMPINST_DIR}/lib/pkgconfig

build_libuuid
build_freetype
build_fontconfig
build_android_shmem

build_SDL2

exit 0

# Xorg
if true; then
build_util_macros
build_xproto
build_bigreqsproto
build_compositeproto
build_damageproto
build_dmxproto
build_dri2proto
build_fixesproto
build_fontsproto
build_glproto
build_inputproto
build_kbproto
build_randrproto
build_recordproto
build_renderproto
build_resourceproto
build_scrnsaverproto
build_videoproto
build_windowswmproto
build_xcb_proto
build_xcmiscproto
build_xextproto
build_xf86bigfontproto
build_xf86dgaproto
build_xf86driproto
build_xf86vidmodeproto
build_xineramaproto

build_libpthread_stubs
build_xtrans
build_libXau
build_libXdmcp
build_libxcb
build_libX11
build_libXext
build_libXxf86vm
build_libfontenc
build_libFS
build_libICE
build_libSM
build_libXt
build_libXpm
build_libXfixes
build_libXcomposite
build_libXrender
build_libXdamage
build_libXcursor
build_libXinerama
build_libXfont
build_libXft
build_libXi
build_libxkbfile
build_libXrandr
build_libXres
build_libXtst
build_libXv
build_libXmu
build_libXScrnSaver
build_libXaw
build_libXvMC
build_libXxf86dga
build_libdmx
#build_libpciaccess

build_xbitmaps
build_xcursor_themes
build_xkeyboard_config

build_bdftopcf
build_luit
build_mkfontdir
build_mkfontscale
build_sessreg
build_setxkbmap
build_smproxy
build_x11perf
build_xauth
build_xbacklight
build_xcmsdb
build_xcursorgen
build_xdpyinfo
#build_xdriinfo
build_xev
build_xgamma
build_xhost
build_xinput
build_xkbcomp
build_xkbevd
build_xkbutils
build_xkill
build_xlsatoms
build_xlsclients
build_xmodmap
build_xpr
build_xprop
build_xrandr
build_xrdb
build_xrefresh
build_xset
build_xsetroot
build_xvinfo
build_xwd
build_xwininfo
build_xwud

build_font_util
build_font_encodings
build_font_alias
build_font_adobe_100dpi
build_font_adobe_75dpi
build_font_adobe_utopia_100dpi
build_font_adobe_utopia_75dpi
build_font_adobe_utopia_type1
build_font_alias
build_font_arabic_misc
build_font_bh_100dpi
build_font_bh_75dpi
build_font_bh_lucidatypewriter_100dpi
build_font_bh_lucidatypewriter_75dpi
build_font_bh_ttf
build_font_bh_type1
build_font_bitstream_100dpi
build_font_bitstream_75dpi
build_font_bitstream_type1
build_font_cronyx_cyrillic
build_font_cursor_misc
build_font_daewoo_misc
build_font_dec_misc
build_font_ibm_type1
build_font_isas_misc
build_font_jis_misc
build_font_micro_misc
build_font_misc_cyrillic
build_font_misc_ethiopic
build_font_misc_meltho
build_font_misc_misc
build_font_mutt_misc
build_font_schumacher_misc
build_font_screen_cyrillic
build_font_sony_misc
build_font_sun_misc
build_font_util
build_font_winitzki_cyrillic
build_font_xfree86_type1

build_cpufeatures
build_pixman

build_xorg_server

build_rxvt_unicode
fi
