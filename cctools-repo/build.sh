#!/bin/bash

WRKDIR=$PWD/tmp
NDKDIR=/opt/android-ndk
NDKSRC=/home/sash/Work/android/ndk-source

for d in binutils gcc gmp mpc mpfr cloog isl ppl llvm-3.3 llvm-3.4; do
    ln -sf ${NDKSRC}/${d} src/
done

export PATH=/opt/CodeSourcery/bin:$PATH

./build-shell-utils.sh ${PWD}/src arm-linux-androideabi ${WRKDIR}/arm-repo  || exit 1

./build-shell-utils.sh ${PWD}/src mipsel-linux-android  ${WRKDIR}/mips-repo || exit 1

./build-shell-utils.sh ${PWD}/src i686-linux-android    ${WRKDIR}/i686-repo || exit 1

test -e ${WRKDIR}/repo/armeabi-v7a || ln -sf armeabi ${WRKDIR}/repo/armeabi-v7a
test -e ${WRKDIR}/repo/mips-r2     || ln -sf mips    ${WRKDIR}/repo/mips-r2

for d in armeabi mips x86; do
    pushd .
    cp -f make_packages.sh ${WRKDIR}/repo/${d}/
    cd ${WRKDIR}/repo/${d}
    ./make_packages.sh
    popd
done

echo "DONE!"
