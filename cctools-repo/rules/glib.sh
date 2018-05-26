build_glib() {
    PKG=glib
    PKG_VERSION=2.24.2
    PKG_URL="http://ftp.gnome.org/pub/gnome/sources/glib/2.24/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="GLib library of C routines"
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $FUNCNAME && return

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
    arm*|aarch64*)
	CONF_ARGS="
        glib_cv_stack_grows=no
	glib_cv_uscore=no
	ac_cv_func_nonposix_getpwuid_r=no
	ac_cv_func_nonposix_getgrgid_r=no
	ac_cv_func_posix_getpwuid_r=no
	ac_cv_func_posix_getgrgid_r=no"
	;;
    mips*|mips64el*)
	CONF_ARGS="
        glib_cv_stack_grows=no
	glib_cv_uscore=no
	ac_cv_func_nonposix_getpwuid_r=no
	ac_cv_func_nonposix_getgrgid_r=no
	ac_cv_func_posix_getpwuid_r=no
	ac_cv_func_posix_getgrgid_r=no"
	;;
    i*86*|x86_64*)
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

    patch -p0 < ${patch_dir}/libtool-glib-${PKG_VERSION}.patch || error "patching libtool"

    $MAKE $MAKEARGS G_THREAD_LIBS_FOR_GTHREAD="" || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages

    popd
    s_tag $FUNCNAME
}
