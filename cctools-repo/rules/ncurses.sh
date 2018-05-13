build_ncurses() {
    PKG=ncurses
    PKG_VERSION=$ncurses_version
    PKG_DESC="Programming library that provides an API which allows the programmer to write text-based user interfaces in a terminal-independent manner."
    PKG_URL="http://ftp.gnu.org/gnu/ncurses/ncurses-$ncurses_version.tar.gz"
    O_FILE=$SRC_PREFIX/ncurses/ncurses-$ncurses_version.tar.bz2
    S_DIR=$src_dir/ncurses-$ncurses_version
    B_DIR=$build_dir/ncurses
    c_tag $PKG && return

    banner "Build $PKG"

    pushd .
    mkdir -p $SRC_PREFIX/ncurses
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

#    cd $S_DIR
#    patch -p1 < $patch_dir/ncurses-$ncurses_version.patch || error "patch"

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure --target=$TARGET_ARCH --host=$TARGET_ARCH --prefix=$TMPINST_DIR --disable-werror --enable-static --disable-shared \
                     --with-normal --without-cxx --without-ada --without-manpages --without-progs --without-tests \
                     --disable-database --with-fallbacks=xterm || error "configure"
#--enable-widec

    sed -i -e "s|#define HAVE_LOCALE_H 1|#undef HAVE_LOCALE_H|" include/ncurses_cfg.h

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    ln -sf ncurses/curses.h ${TMPINST_DIR}/include/curses.h
    ln -sf ncurses/curses.h ${TMPINST_DIR}/include/ncurses.h
    ln -sf ncurses/eti.h    ${TMPINST_DIR}/include/eti.h
    ln -sf ncurses/form.h   ${TMPINST_DIR}/include/form.h
    ln -sf ncurses/menu.h   ${TMPINST_DIR}/include/menu.h
    ln -sf ncurses/panel.h  ${TMPINST_DIR}/include/panel.h
    ln -sf ncurses/term.h   ${TMPINST_DIR}/include/term.h
    ln -sf ncurses/termcap.h ${TMPINST_DIR}/include/termcap.h
    ln -sf ncurses/unctrl.h ${TMPINST_DIR}/include/unctrl.h
    ln -sf libncurses.so    ${TMPINST_DIR}/lib/libcurses.so

    rm -rf $TARGET_DIR/share

    make prefix=${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH} install || error "pkg install"
    ln -sf ncurses/curses.h ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/curses.h
    ln -sf ncurses/curses.h ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/ncurses.h
    ln -sf ncurses/eti.h    ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/eti.h
    ln -sf ncurses/form.h   ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/form.h
    ln -sf ncurses/menu.h   ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/menu.h
    ln -sf ncurses/panel.h  ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/panel.h
    ln -sf ncurses/term.h   ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/term.h
    ln -sf ncurses/termcap.h ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/termcap.h
    ln -sf ncurses/unctrl.h ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/unctrl.h
    ln -sf libncurses.a    ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/libcurses.a
    rm -f ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/lib*_g.a
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/share

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
