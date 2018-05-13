build_glib() {
    PKG=glib
    PKG_VERSION=2.24.2
    PKG_URL="http://ftp.gnome.org/pub/gnome/sources/glib/2.24/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="GLib library of C routines"
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .

    #download $PKG_URL $O_FILE

    #unpack $src_dir $O_FILE

    #patchsrc $S_DIR $PKG $PKG_VERSION

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    local CONF_ARGS=
    case $TARGET_ARCH in
    arm*)
	CONF_ARGS="
        glib_cv_stack_grows=no
	glib_cv_uscore=no
	ac_cv_func_nonposix_getpwuid_r=no
	ac_cv_func_nonposix_getgrgid_r=no
	ac_cv_func_posix_getpwuid_r=no
	ac_cv_func_posix_getgrgid_r=no"
	;;
    mips*)
	CONF_ARGS="
        glib_cv_stack_grows=no
	glib_cv_uscore=no
	ac_cv_func_nonposix_getpwuid_r=no
	ac_cv_func_nonposix_getgrgid_r=no
	ac_cv_func_posix_getpwuid_r=no
	ac_cv_func_posix_getgrgid_r=no"
	;;
    i*86*)
	CONF_ARGS="
        glib_cv_stack_grows=no
	glib_cv_uscore=no
	ac_cv_func_nonposix_getpwuid_r=no
	ac_cv_func_nonposix_getgrgid_r=no
	ac_cv_func_posix_getpwuid_r=no
	ac_cv_func_posix_getgrgid_r=no"
	;;
    esac

    eval $CONF_ARGS \
    ${S_DIR}/configure \
	--host=${TARGET_ARCH} \
	--prefix=${TMPINST_DIR} \
	--with-libiconv=gnu \
	--disable-nls \
	CPPFLAGS="-I${TMPINST_DIR}/include" \
	LDFLAGS="-L${TMPINST_DIR}/lib" \
	|| error "Configure $PKG."

#	LIBFFI_CFLAGS="-I${TMPINST_DIR}/include" \
#	LIBFFI_LIBS="-L${TMPINST_DIR}/lib" \

    $MAKE $MAKEARGS G_THREAD_LIBS_FOR_GTHREAD="" || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install DESTDIR=${TMPINST_DIR}/${PKG}-tmp || error "package install"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/bin
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/lib

    cp -a ${TMPINST_DIR}/${PKG}-tmp/${TMPINST_DIR}/lib/*.so*   ${TMPINST_DIR}/${PKG}/cctools/lib/

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/lib/*.so*

    local filename="lib${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename lib${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
