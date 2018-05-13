build_wget() {
    PKG=wget
    PKG_VERSION=1.14
    PKG_DESC="GNU Wget is a free software package for retrieving files using HTTP, HTTPS and FTP, the most widely-used Internet protocols. It is a non-interactive commandline tool, so it may easily be called from scripts, cron jobs, terminals without X-Windows support, etc."
    PKG_URL=http://ftp.gnu.org/gnu/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
#    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

#    gl_cv_func_dup2_works=yes \
#    gl_cv_func_fcntl_f_dupfd_works=yes \
#    gl_cv_func_gettimeofday_clobber=no \
#    gl_cv_func_lstat_dereferences_slashed_symlink=yes \
#    gl_cv_func_memchr_works=yes \
#    gl_cv_func_mkdir_trailing_dot_works=yes \
#    gl_cv_func_mkdir_trailing_slash_works=yes \
#    gl_cv_func_posix_spawn_file_actions_addclose_works=no \
#    gl_cv_func_posix_spawn_file_actions_adddup2_works=no \
#    gl_cv_func_posix_spawn_file_actions_addopen_works=no \
#    gl_cv_func_snprintf_retval_c99=yes \
#    gl_cv_func_stat_file_slash=yes \
#    gl_cv_func_strerror_0_works=yes \
#    gl_cv_func_working_mkstemp=yes \
#    gl_cv_func_working_strerror=yes \
#    gl_cv_func_btowc_nul=yes \
#    gl_cv_func_ftello_works=yes \
#    gl_cv_func_getopt_gnu=no \
#    gl_cv_func_getopt_posix=yes \
#    gl_cv_func_iswcntrl_works=yes \
#    gl_cv_func_open_slash=yes \
#    gl_cv_func_printf_positions=yes \
#    gl_cv_func_select_supports0=yes \
#    gl_cv_func_snprintf_size1=yes \
#    gl_cv_func_stat_dir_slash=yes \
#    gl_cv_func_strcasestr_linear=no \
#    gl_cv_func_strcasestr_works_always=yes \
#    gl_cv_func_strerror_r_works=yes \
#    gl_cv_func_strtok_r_works=yes \
#    gl_cv_func_working_strerror=yes \
#    gl_cv_have_weak=yes \

    ./configure --host=$TARGET_ARCH --prefix=$TARGET_INST_DIR \
	--with-ssl=openssl --with-libssl-prefix=${TMPINST_DIR} || error "configure"

    $MAKE $MAKEARGS || error "make"

    #$MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig ${TMPINST_DIR}/${PKG}/cctools/lib/*.la
    #mkdir ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    #mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/
    #mv ${TMPINST_DIR}/${PKG}/cctools/lib     ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
