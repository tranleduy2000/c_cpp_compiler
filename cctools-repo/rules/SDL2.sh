build_SDL2() {
    PKG=SDL2
    PKG_VERSION=2.0.3
    PKG_SUBVERSION=
    PKG_URL="http://www.libsdl.org/release/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="Simple DirectMedia Layer"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $FUNCNAME && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    copysrc $S_DIR $B_DIR
    cd $B_DIR

    cp -f ${TOPDIR}/tools/micro-ndk/Makefile* .

    # delay to flush file buffer to fix Makefiles stamps
    sleep 1

    make CROSS_COMPILE=${TARGET_ARCH}- $MAKEARGS || error "make SDL2"

    mkdir -p ${TMPINST_DIR}/include/SDL2
    mkdir -p ${TMPINST_DIR}/lib/pkgconfig
    mkdir -p ${TMPINST_DIR}/share/aclocal

    cp -f libSDL2.so libSDL2_static.a src/main/android/SDL_android_main.o ${TMPINST_DIR}/lib || error "install SDL2 libs"
    cp -R include/*.h ${TMPINST_DIR}/include/SDL2 || error "install SDL2 headers"
    cp -f sdl2.m4 ${TMPINST_DIR}/share/aclocal || error "install SDL2 m4"

cat > ${TMPINST_DIR}/lib/pkgconfig/sdl2.pc << EOF
# sdl pkg-config source file

prefix=$TMPINST_DIR
exec_prefix=\${prefix}
libdir=\${prefix}/lib
includedir=\${prefix}/include

Name: sdl2
Description: Simple DirectMedia Layer is a cross-platform multimedia library designed to provide low level access to audio, keyboard, mouse, joystick, 3D hardware via OpenGL, and 2D video framebuffer.
Version: $PKG_VERSION
Requires:
Conflicts:
Libs: -L\${libdir} \${libdir}/SDL_android_main.o -lSDL2 -ldl -lGLESv1_CM -lGLESv2 -llog -landroid
Libs.private: -Wl,--undefined=Java_org_libsdl_app_SDLActivity_nativeInit -lSDL2_static -ldl -lGLESv1_CM -lGLESv2 -llog -landroid
Cflags: -I\${includedir}/SDL2 -DGL_GLEXT_PROTOTYPES
EOF

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/include/SDL2
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/share/aclocal

    cp -f libSDL2.so libSDL2_static.a src/main/android/SDL_android_main.o ${TMPINST_DIR}/${PKG}/cctools/lib || error "install SDL2 libs"
    cp -R include/*.h ${TMPINST_DIR}/${PKG}/cctools/include/SDL2  || error "install SDL2 headers"
    cp -f sdl2.m4     ${TMPINST_DIR}/${PKG}/cctools/share/aclocal || error "install SDL2 m4"

    cp -f ${TMPINST_DIR}/lib/pkgconfig/sdl2.pc ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig/

    rm -rf ${TMPINST_DIR}/${PKG}-dev

    mkdir -p ${TMPINST_DIR}/${PKG}-dev/cctools/share/SDL2
    cp -R android-project ${TMPINST_DIR}/${PKG}-dev/cctools/share/SDL2

    make_packages nodeldev

    popd
    s_tag $FUNCNAME
}
