#!/bin/bash

ndk_version="r9c"

binutils_version="2.23"
gcc_version="4.8"
gmp_version="5.0.5"
mpc_version="1.0.1"
mpfr_version="3.1.1"
cloog_version="0.18.0"
isl_version="0.11.1"
ppl_version="1.0"

make_version="4.0"
ncurses_version="5.9"
nano_version="2.2.6"
busybox_version="1.21.1"
emacs_version="24.2"

binutils_avr_version="2.23"
gcc_avr_version="4.8"

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
    NDK_DIR=/opt/android-ndk
fi

if [ "x$SDK_DIR" = "x" ]; then
    SDK_DIR=/opt/adt-bundle-linux/sdk
fi

if [ "x$MAKEARGS" = "x" ]; then
    MAKEARGS=-j9
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
arm*)
    TARGET_ARCH_GLIBC=arm-none-linux-gnueabi
    ;;
mips*)
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
	if wget $opt $1 -O $2 ; then
	    return
	fi
	rm -f $2
	error "download $PKG_URL"
    fi
}

unpack() {
    local cmd=

    echo "Unpacking..."

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

#
# build_package_desc <path> <filename> <name> <version> <arch> <description> [<depends> [<replaces>]]
#

build_package_desc() {
    local filename=$2
    local name=$3
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
        if file $f | grep -q 'ASCII text'; then
	    if grep -q '/bin/sh' $f; then
		echo "fix bionic shell in $f"
		touch -r $f ${f}.timestamp
		sed -i -e 's|/bin/sh|/system/bin/sh|' $f
		touch -r ${f}.timestamp $f
		rm -f ${f}.timestamp
	    fi
	fi
    done
    echo "Fixed!"
}

replace_string() {
    local p="$1"
    local f=""
    if [ "x$p" = "x" ]; then
	p="."
    fi

    for f in $(find $p -type f); do
        if file $f | grep -q 'ASCII text\|shell script'; then
	    if grep -q "$2" $f; then
		echo "replace string in $f"
		touch -r $f ${f}.timestamp
		sed -i -e "s|$2|$3|" $f
		touch -r ${f}.timestamp $f
		rm -f ${f}.timestamp
	    fi
	fi
    done
    echo "Fixed!"
}

case $TARGET_ARCH in
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

for f in rules/*.sh; do
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

    build_native_perl
    build_m4
    build_autoconf

    if ! which autoconf 2>/dev/null >/dev/null; then
	echo "Please install autoconf package and restart build."
	exit 0
    fi

    build_automake

    if ! which automake 2>/dev/null >/dev/null; then
	echo "Please install automake package and restart build."
	exit 0
    fi

    build_bison
    build_flex

    exit 0
fi

build_sysroot_host

build_gmp_host
build_mpfr_host
build_mpc_host
build_isl_host
build_ppl_host
build_cloog_host

build_binutils_host
build_gcc_host

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
build_llvm

# presets
build_build_essential_clang
build_build_essential_clang_objc
build_build_essential_gcc
build_build_essential_fortran
build_build_essential_gcc_avr
build_build_essential_gcc_objc
build_build_essential_gcc_objc_fortran

build_build_essential_clang_compact
build_build_essential_clang_objc_compact
build_build_essential_gcc_compact
build_build_essential_fortran_compact
build_build_essential_gcc_objc_compact
build_build_essential_gcc_objc_fortran_compact
build_build_essential_luajit

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
build_aprutil
build_neon
build_subversion
build_curl
build_wget
build_git
build_ca_certificates
build_dropbear
#build_fpc
#build_nano
#build_emacs
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
#build_binutils_mingw32_host x86_64-w64-mingw32
build_gcc_mingw32_host i686-w64-mingw32
#build_gcc_mingw32_host x86_64-w64-mingw32
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
