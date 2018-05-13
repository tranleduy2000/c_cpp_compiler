#!/bin/bash

TOPDIR=$PWD

TMPDIR=/tmp/sdlplugin$$/

mkdir -p $TMPDIR

rm -rf ${TMPDIR}/include
mkdir -p ${TMPDIR}/include

cp -f jni/SDL/include/*.h		${TMPDIR}/include/
cp -f jni/SDL2_image/SDL_image.h	${TMPDIR}/include/
cp -f jni/SDL2_mixer/SDL_mixer.h	${TMPDIR}/include/
cp -f jni/SDL2_net/SDL_net.h		${TMPDIR}/include/
cp -f jni/SDL2_net/SDLnetsys.h		${TMPDIR}/include/
cp -f jni/SDL2_ttf/SDL_ttf.h		${TMPDIR}/include/

cd ${TMPDIR}
rm -f ${TOPDIR}/assets/headers.zip
zip -r9 ${TOPDIR}/assets/headers.zip include
cd ${TOPDIR}

find obj -name "SDL_android_main.o" | while read f; do

    arch=`echo $f | cut -f3 -d'/'`
    cd `dirname $f`
    rm -f ${TOPDIR}/assets/sdlmain-${arch}.zip
    zip -9 ${TOPDIR}/assets/sdlmain-${arch}.zip SDL_android_main.o
    cd $TOPDIR
done

cd ${TOPDIR}

rm -rf ${TOPDIR}/assets/examples.zip
zip -r9 ${TOPDIR}/assets/examples.zip Examples
