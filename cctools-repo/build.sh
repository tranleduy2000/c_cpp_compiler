#!/bin/bash

BUILD_START=$(date +%s)

WRKDIR=$PWD/tmp
#WRKDIR=/mnt/nfs/Work/cctools-pro/tmp

#
# uncomment to build pie compiler
# -------------------------------
export BUILD_PIE_COMPILER="yes"
WRKDIR=${WRKDIR}-pie
# -------------------------------
#

SDKDIR=/home/sash/Android/android-sdk
NDKDIR=/home/sash/Android/android-ndk
NDKSRC=/home/sash/Android/ndk-src

for d in binutils gcc gmp mpc mpfr cloog isl ppl llvm-3.6; do
    ln -sf ${NDKSRC}/${d} src/
done

export PATH=~/bin/CodeSourcery/bin:$PATH

#./build-shell-utils.sh ${PWD}/src arm-linux-androideabi  ${WRKDIR}/arm-repo $NDKDIR $SDKDIR      || exit 1

#./build-shell-utils.sh ${PWD}/src mipsel-linux-android   ${WRKDIR}/mips-repo $NDKDIR $SDKDIR     || exit 1

#./build-shell-utils.sh ${PWD}/src i686-linux-android     ${WRKDIR}/i686-repo $NDKDIR $SDKDIR     || exit 1

./build-shell-utils.sh ${PWD}/src aarch64-linux-android  ${WRKDIR}/arm64-repo $NDKDIR $SDKDIR  || exit 1

#./build-shell-utils.sh ${PWD}/src mips64el-linux-android ${WRKDIR}/mips64-repo $NDKDIR $SDKDIR || exit 1

./build-shell-utils.sh ${PWD}/src x86_64-linux-android   ${WRKDIR}/x86_64-repo $NDKDIR $SDKDIR   || exit 1

test -e ${WRKDIR}/repo/armeabi-v7a || ln -sf armeabi ${WRKDIR}/repo/armeabi-v7a
test -e ${WRKDIR}/repo/mips-r2     || ln -sf mips    ${WRKDIR}/repo/mips-r2

for d in armeabi mips x86 arm64-v8a mips64 x86_64; do
    pushd .
    cp -f make_packages.sh ${WRKDIR}/repo/${d}/
    cd ${WRKDIR}/repo/${d}
    ./make_packages.sh
    popd
done

mkdir -p ${WRKDIR}/repo/src

find `find src -type d` -type f -exec cp -f {} ${WRKDIR}/repo/src/ \;

BUILD_END=$(date +%s)

TOTAL_TIME=$(($BUILD_END - $BUILD_START))

echo
echo
echo "Build time: $(date -u -d @$TOTAL_TIME +%T)"
echo
echo "DONE!"
