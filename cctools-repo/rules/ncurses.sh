build_ncurses() {
    PKG=ncurses
    PKG_VERSION=5.9
    PKG_SUBVERSION=
    PKG_URL="https://ftp.gnu.org/gnu/ncurses/${PKG}-${PKG_VERSION}.tar.gz"
    PKG_DESC="Programming library providing an application programming interface (API) that allows the programmer to write text-based user interfaces in a terminal-independent manner"
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

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    ${S_DIR}/configure	\
	--program-transform-name='s&^&&' \
	--target=$TARGET_ARCH \
	--host=$TARGET_ARCH \
	--prefix=$TMPINST_DIR \
	--disable-werror \
	--enable-static \
	--disable-shared \
	--with-normal \
	--without-cxx \
	--without-ada \
	--without-tests \
	--disable-database \
	--with-fallbacks=xterm || error "Configure $PKG."

    sed -i -e "s|#define HAVE_LOCALE_H 1|#undef HAVE_LOCALE_H|" include/ncurses_cfg.h

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    ln -sf ncurses/curses.h  ${TMPINST_DIR}/include/curses.h
    ln -sf ncurses/curses.h  ${TMPINST_DIR}/include/ncurses.h
    ln -sf ncurses/eti.h     ${TMPINST_DIR}/include/eti.h
    ln -sf ncurses/form.h    ${TMPINST_DIR}/include/form.h
    ln -sf ncurses/menu.h    ${TMPINST_DIR}/include/menu.h
    ln -sf ncurses/panel.h   ${TMPINST_DIR}/include/panel.h
    ln -sf ncurses/term.h    ${TMPINST_DIR}/include/term.h
    ln -sf ncurses/termcap.h ${TMPINST_DIR}/include/termcap.h
    ln -sf ncurses/unctrl.h  ${TMPINST_DIR}/include/unctrl.h
    ln -sf libncurses.so     ${TMPINST_DIR}/lib/libcurses.so

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    make_packages

    popd
    s_tag $FUNCNAME
}
