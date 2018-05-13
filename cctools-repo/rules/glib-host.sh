build_glib_host() {
    PKG=glib
    PKG_VERSION=2.24.2
    PKG_URL="http://ftp.gnome.org/pub/gnome/sources/glib/2.24/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="GLib library of C routines"
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}-host

    c_tag ${PKG}-host && return

    banner "Build $PKG-host"

    pushd .

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    local CONF_ARGS=

    ${S_DIR}/configure \
	--prefix=${TARGET_DIR}-host \
	|| error "Configure $PKG."

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host
}
