build_android_shmem() {
    PKG=android-shmem
    PKG_VERSION=0.1
    PKG_SUBVERSION=
    PKG_URL="http://cctools.info/packages/src/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="System V shared memory emulation on Android, using ashmem"
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
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

    # Configure here

    #${S_DIR}/configure	\
    #			--host=${TARGET_ARCH} \
    #                    --prefix=$TMPINST_DIR \
    #			|| error "Configure $PKG."

    $MAKE $MAKEARGS CC=${TARGET_ARCH}-gcc AR=${TARGET_ARCH}-ar RANLIB=${TARGET_ARCH}-ranlib || error "make $MAKEARGS"

    $INSTALL -D -m 644 sys/shm.h           ${TMPINST_DIR}/include/sys/shm.h
    $INSTALL -D -m 644 libandroid-shmem.a  ${TMPINST_DIR}/lib/libandroid-shmem.a
    $INSTALL -D -m 755 libandroid-shmem.so ${TMPINST_DIR}/lib/libandroid-shmem.so

    PKG=lib${PKG}

    $INSTALL -D -m 644 sys/shm.h           ${TMPINST_DIR}/${PKG}/cctools/include/sys/shm.h
    $INSTALL -D -m 644 libandroid-shmem.a  ${TMPINST_DIR}/${PKG}/cctools/lib/libandroid-shmem.a
    $INSTALL -D -m 755 libandroid-shmem.so ${TMPINST_DIR}/${PKG}/cctools/lib/libandroid-shmem.so

    make_packages

    popd
    s_tag $FUNCNAME
}
