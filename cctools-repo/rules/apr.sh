build_apr() {
    PKG=apr
    PKG_VERSION=1.5.0
    PKG_DESC="The mission of the Apache Portable Runtime (APR) project is to create and maintain software libraries that provide a predictable and consistent interface to underlying platform-specific implementations."
    PKG_URL=http://www.eu.apache.org/dist/$PKG/$PKG-$PKG_VERSION.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    ac_cv_file__dev_zero=yes \
    ac_cv_func_setpgrp_void=yes \
    apr_cv_process_shared_works=yes \
    apr_cv_mutex_robust_shared=no \
    apr_cv_tcp_nodelay_with_cork=yes \
    ac_cv_func_pthread_rwlock_init=no \
    ac_cv_func_fdatasync=no \
    ac_cv_o_nonblock_inherited=no \
    ac_cv_sizeof_off_t=4 \
    ac_cv_sizeof_pid_t=4 \
    ac_cv_sizeof_size_t=4 \
    ac_cv_sizeof_ssize_t=4 \
    ac_cv_sizeof_struct_iovec=8 \
    ac_cv_strerror_r_rc_int=yes \
    ac_cv_struct_rlimit=yes \
    ap_cv_atomic_builtins=yes \
    apr_cv_epoll=yes \
    apr_cv_pthreads_cflags=none \
    apr_cv_type_rwlock_t=yes \
    apr_cv_gai_addrconfig=yes \
    apr_cv_mutex_recursive=yes \
	./configure --host=$TARGET_ARCH --prefix=$TMPINST_DIR --disable-shared --enable-static || error "configure"

    $MAKE $MAKEARGS || error "make"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    #$TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig ${TMPINST_DIR}/${PKG}/cctools/lib/*.la ${TMPINST_DIR}/${PKG}/cctools/bin
    mkdir ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/
    mv ${TMPINST_DIR}/${PKG}/cctools/lib     ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
