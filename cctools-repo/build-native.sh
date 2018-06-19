#!/bin/ash

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

TARGET_INST_DIR="/data/data/com.duy.c.cpp.compiler/root/cctools"
#TARGET_INST_DIR="/data/data/com.pdaxrom.cctools/root/cctools"
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

if [ ! -f /system/bin/sh ]; then

    echo "Only for on-device native execution."

    exit 0
fi

WRKDIR=${HOME}/tmp

mkdir -p ${WRKDIR}/repo/arm64-v8a
mkdir -p ${WRKDIR}/repo/mips64
mkdir -p ${WRKDIR}/repo/x86_64
mkdir -p ${WRKDIR}/repo/armeabi
mkdir -p ${WRKDIR}/repo/mips
mkdir -p ${WRKDIR}/repo/x86
test -h ${WRKDIR}/repo/armeabi-v7a || ln -sf armeabi ${WRKDIR}/repo/armeabi-v7a
test -h ${WRKDIR}/repo/mips-r2     || ln -sf mips    ${WRKDIR}/repo/mips-r2

if ! cat ${CCTOOLSDIR}/etc/repos.list | grep -q "${WRKDIR}/repo"; then
    echo "Adding local repo to list"
    echo -n -e "\nfile://${WRKDIR}/repo\n" >> ${CCTOOLSDIR}/etc/repos.list
fi

case `uname -m` in
aarch64*)
    ash ./build-shell-utils.sh ${PWD}/src aarch64-linux-android  ${WRKDIR}/arm64-repo  || exit 1
    ;;
arm*)
    ash ./build-shell-utils.sh ${PWD}/src arm-linux-androideabi  ${WRKDIR}/arm-repo      || exit 1
    ;;
mips64el*)
    ash ./build-shell-utils.sh ${PWD}/src mips64el-linux-android ${WRKDIR}/mips64-repo || exit 1
    ;;
mips*)
    ash ./build-shell-utils.sh ${PWD}/src mipsel-linux-android   ${WRKDIR}/mips-repo     || exit 1
    ;;
x86_64*)
    ash ./build-shell-utils.sh ${PWD}/src x86_64-linux-android   ${WRKDIR}/x86_64-repo   || exit 1
    ;;
*)
    bash ./build-shell-utils.sh ${PWD}/src i686-linux-android     ${WRKDIR}/i686-repo     || exit 1
    ;;
esac

echo "DONE!"
