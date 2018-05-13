#!/bin/bash

ncurses_version=5.9
nano_version=2.2.6
busybox_version=1.20.2
emacs_version=24.2

TARGET_INST_DIR="/data/data/com.pdaxrom.cctools/root/cctools"
#TARGET_INST_DIR="/data/data/com.pdaxrom.cctools/cache/cctools"

SRC_PREFIX="$1"

TARGET_ARCH="$2"
HOST_ARCH="$2"

WORK_DIR="$3"

SYSROOT="$4"

if [ "x$SRC_PREFIX" = "x" ]; then
    echo "No source dir"
    exit 1
fi

if [ "x$TARGET_ARCH" = "x" ]; then
    echo "No target arch"
    exit 1
fi

if [ "x$WORK_DIR" = "x" ]; then
    work_dir="/tmp/native-ndk-$TARGET_ARCH-$USER"
else
    work_dir="$WORK_DIR"
fi

if [ "x$MAKEARGS" = "x" ]; then
    MAKEARGS=-j4
fi

TOPDIR="$PWD"

build_dir="$work_dir/build"
src_dir="$work_dir/src"
patch_dir="$TOPDIR/patches"

TARGET_DIR="$work_dir/cctools"
TMPINST_DIR="$build_dir/tmpinst"

MAKE=make
INSTALL=install

XBUILD_ARCH=`uname -m`
BUILD_SYSTEM=`uname`

case $BUILD_SYSTEM in
Linux)
    BUILD_ARCH=${XBUILD_ARCH}-unknown-linux
    ;;
Darwin)
    BUILD_ARCH=${XBUILD_ARCH}-unknown-darwin
    ;;
CYGWIN*)
    BUILD_ARCH=${XBUILD_ARCH}-unknown-cygwin
    ;;
*)
    BUILD_ARCH=
    echo "unknown host system!"
    exit 1
    ;;
esac

case $TARGET_ARCH in
arm*)
    TARGET_ARCH_GLIBC=arm-none-linux-gnueabi
    ;;
mips*)
    TARGET_ARCH_GLIBC=mips-linux-gnu
    ;;
i*86*|x86*)
    TARGET_ARCH_GLIBC=i686-pc-linux-gnu
    ;;
*)
    echo "unknown arch $TARGET_ARCH"
    exit 1
    ;;
esac

echo "Target arch: $TARGET_ARCH"
echo "Host   arch: $HOST_ARCH"
echo "Build  arch: $BUILD_ARCH"

error() {
    echo "Error: $@"
    exit 1
}

makedirs() {
    mkdir -p $src_dir
    mkdir -p $work_dir/tags
}

s_tag() {
    touch $work_dir/tags/$1
}

c_tag() {
    test -e $work_dir/tags/$1
}

copysrc() {
    mkdir -p $2
    tar -C "$1" -c . | tar -C $2 -xv || error "copysrc $1 $2"
}

#
# build_package_desc <path> <filename> <name> <version> <arch> <description>
#

build_package_desc() {
    local filename=$2
    local name=$3
    local vers=$4
    local arch=$5
    local desc=$6
cat >$1/pkgdesc << EOF
    <package>
	<name>$name</name>
	<version>$vers</version>
	<arch>$arch</arch>
	<description>$desc</description>
	<size>@SIZE@</size>
	<file>$filename</file>
    </package>
EOF

}

case $TARGET_ARCH in
arm*)
    PKG_ARCH="armel"
    ;;
mips*)
    PKG_ARCH="mipsel"
    ;;
i*86*)
    PKG_ARCH="i686"
    ;;
*)
    error "Can't set PKG_ARCH from $TARGET_ARCH"
    ;;
esac

build_ncurses() {
    PKG=ncurses
    PKG_VERSION=$ncurses_version
    PKG_DESC="Programming library that provides an API which allows the programmer to write text-based user interfaces in a terminal-independent manner."
    PKG_URL="http://ftp.gnu.org/gnu/ncurses/ncurses-$ncurses_version.tar.gz"
    O_FILE=$SRC_PREFIX/ncurses/ncurses-$ncurses_version.tar.bz2
    S_DIR=$src_dir/ncurses-$ncurses_version
    B_DIR=$build_dir/ncurses
    c_tag $PKG && return
    echo "build $PKG"
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
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

dropbear_version=0.52
build_dropbear() {
    PKG=dropbear
    PKG_VERSION=$dropbear_version
    PKG_DESC="Dropbear is a software package written by Matt Johnston that provides a Secure Shell-compatible server and client. It is designed as a replacement for standard OpenSSH for environments with low memory and processor resources, such as embedded systems."
    PKG_URL="http://matt.ucc.asn.au/dropbear/releases/dropbear-$dropbear_version.tar.gz"
    O_FILE=$SRC_PREFIX/dropbear/dropbear-$dropbear_version.tar.gz
    S_DIR=$src_dir/dropbear-$dropbear_version
    B_DIR=$build_dir/dropbear
    c_tag $PKG && return
    echo "build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/dropbear
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/dropbear-$dropbear_version.patch || error "patch"

    mkdir -p $B_DIR
    cd $B_DIR

    case "$TARGET_ARCH" in
    mips*)
	EXTRA_CPPFLAGS="-EL"
	EXTRA_LDFLAGS="-EL"
	;;
    *)
	EXTRA_CPPFLAGS=
	EXTRA_LDFLAGS=
	;;
    esac

    CPPFLAGS="$EXTRA_CPPFLAGS" \
    LDFLAGS="-static $EXTRA_LDFLAGS" \
    $S_DIR/configure --target=$TARGET_ARCH_GLIBC --host=$TARGET_ARCH_GLIBC --prefix=$TMPINST_DIR \
	--disable-largefile \
	--disable-loginfunc \
	--disable-shadow \
	--disable-utmp \
	--disable-utmpx \
	--disable-wtmp \
	--disable-wtmpx \
	--disable-pututline \
	--disable-pututxline \
	--disable-lastlog \
	--disable-zlib

    $MAKE $MAKEARGS PROGRAMS="dropbear dropbearkey scp dbclient dropbearconvert" || error "make $MAKEARGS"

    $MAKE install PROGRAMS="dropbear dropbearkey scp dbclient dropbearconvert" || error "make install"

    rm -rf $TARGET_DIR/share

    make prefix=${TMPINST_DIR}/${PKG}/cctools/ install PROGRAMS="dropbear dropbearkey scp dbclient dropbearconvert" || error "pkg install"
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share

    ${TARGET_ARCH_GLIBC}-strip ${TMPINST_DIR}/${PKG}/cctools/bin/* ${TMPINST_DIR}/${PKG}/cctools/sbin/*

    mkdir ${TMPINST_DIR}/${PKG}/cctools/services
    mkdir ${TMPINST_DIR}/${PKG}/cctools/etc
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/var/run

    echo "alpine" >> ${TMPINST_DIR}/${PKG}/cctools/etc/dropbear.passwd

    cat >> ${TMPINST_DIR}/${PKG}/cctools/services/dropbear << EOF
#!/system/bin/sh

case \$1 in
start)
    if [ ! -f \${CCTOOLSDIR}/etc/dropbear_rsa_host_key ]; then
	dropbearkey -t rsa -f \${CCTOOLSDIR}/etc/dropbear_rsa_host_key
	dropbearconvert dropbear openssh \${CCTOOLSDIR}/etc/dropbear_rsa_host_key \${EXTERNAL_STORAGE}/CCTools/cctools_id_rsa_openssh
    fi

    dropbear -A -N cctools -C \`cat \${CCTOOLSDIR}/etc/dropbear.passwd\` -r \${CCTOOLSDIR}/etc/dropbear_rsa_host_key -p 22022 -P \${CCTOOLSDIR}/var/run/dropbear.pid

    ;;
stop)
    if [ -f \${CCTOOLSDIR}/var/run/dropbear.pid ]; then
	kill \`cat \${CCTOOLSDIR}/var/run/dropbear.pid\`
    fi

    ;;
*)
    echo "Usage: \$0 [start|stop]"
    ;;
esac

EOF

    cat >> ${TMPINST_DIR}/${PKG}/postinst << EOF
#!/system/bin/sh

\${CCTOOLSDIR}/services/dropbear start

EOF

    cat >> ${TMPINST_DIR}/${PKG}/prerm << EOF
#!/system/bin/sh

\${CCTOOLSDIR}/services/dropbear stop

EOF

    cat >> ${TMPINST_DIR}/${PKG}/cctools/bin/dropbearpasswd << EOF
#!/system/bin/sh

echo "\$1" > \${CCTOOLSDIR}/etc/dropbear.passwd

\${CCTOOLSDIR}/services/dropbear stop
\${CCTOOLSDIR}/services/dropbear start

EOF

    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/services/dropbear
    chmod 755 ${TMPINST_DIR}/${PKG}/postinst
    chmod 755 ${TMPINST_DIR}/${PKG}/prerm
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/dropbearpasswd

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc postinst prerm

    popd
    s_tag $PKG
}

build_busybox() {
    PKG=busybox
    PKG_VERSION=$busybox_version
    PKG_DESC="BusyBox provides several stripped-down Unix tools in a single executable."
    PKG_URL="http://busybox.net/downloads/busybox-$busybox_version.tar.bz2"
    O_FILE=$SRC_PREFIX/busybox/busybox-$busybox_version.tar.bz2
    S_DIR=$src_dir/busybox-$busybox_version
    B_DIR=$build_dir/busybox
    c_tag $PKG && return
    echo "build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/busybox
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/busybox-$busybox_version.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR
    cp -f ${TOPDIR}/configs/busybox .config
    sed -i -e "s|@CROSS@|${TARGET_ARCH_GLIBC}-|" .config
    $MAKE oldconfig

    case "$TARGET_ARCH" in
    mips*)
	EXTRA_CFLAGS="-EL"
	EXTRA_LDFLAGS="-EL"
	;;
    esac

    $MAKE $MAKEARGS CONFIG_EXTRA_CFLAGS="$EXTRA_CFLAGS" CONFIG_EXTRA_LDFLAGS="$EXTRA_LDFLAGS" || error "make $MAKEARGS"

    #$MAKE install || error "make install"

    #rm -rf $TARGET_DIR/share

    install -D -m 755 busybox ${TMPINST_DIR}/bin/busybox

    $MAKE install \
	CONFIG_EXTRA_CFLAGS="$EXTRA_CFLAGS" \
	CONFIG_EXTRA_LDFLAGS="$EXTRA_LDFLAGS" \
	CONFIG_PREFIX=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_htop() {
    PKG=htop
    PKG_VERSION=1.0.2
    PKG_DESC="htop is an interactive text-mode process viewer for Linux. It aims to be a better top."
    PKG_URL=http://downloads.sourceforge.net/project/htop/htop/$PKG_VERSION/$PKG-$PKG_VERSION.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG
    c_tag $PKG && return
    echo "build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    CFLAGS="-I$TMPINST_DIR/include" LDFLAGS="-L$TMPINST_DIR/lib" \
	./configure --host=$TARGET_ARCH --prefix=$TMPINST_DIR --disable-unicode --enable-native-affinity=no


    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    #rm -rf $TARGET_DIR/share

    install -D -m 755 htop ${TMPINST_DIR}/bin/htop

    #make install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $INSTALL -D -m 755 htop ${TMPINST_DIR}/${PKG}/cctools/bin/htop
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/htop
    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_fpc() {
    PKG=fpc
    PKG_VERSION=2.6.0
    PKG_DESC="Free Pascal Compiler (FPC for short) is a free Pascal and Object Pascal compiler."
    PKG_URL=http://downloads.sourceforge.net/project/freepascal/Source/$PKG_VERSION/$PKG-$PKG_VERSION.source.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.source.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG
    c_tag $PKG && return
    echo "build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    local cpu_target=
    locat opt=
    local crossbindir=

    crossbindir="`which ${TARGET_ARCH}-as`"
    crossbindir="`dirname $crossbindir`"
    crossbindir="`dirname $crossbindir`/xbin"
    mkdir -p "$crossbindir" || error "mkdir -p $crossbindir"
    ln -sf `which ${TARGET_ARCH}-as` "$crossbindir/as"
    ln -sf `which ${TARGET_ARCH}-ld` "$crossbindir/ld"

    case $TARGET_ARCH in
    arm*)
	cpu_target=arm
	opt=-dFPC_ARMEL
	;;
    mips*)
	cpu_target=mipsel
	opt=-dFPC_MIPSEL
	s_tag $PKG
	return
	;;
    i*86*|x86*)
	cpu_target=i686
	opt=-dFPC_I686
	s_tag $PKG
	return
	;;
    *)
	error "FPC bad CPU target - $TARGET_ARCH"
	;;
    esac

    make build				\
	CPU_TARGET=$cpu_target		\
	OS_TARGET=linux			\
	CROSSBINDIR="$crossbindir"	\
	OPT=$opt			\
	INSTALL_PREFIX=/data/data/com.pdaxrom.cctools/root/cctools || error "FPC build"
#	PP=fpc				

    make zipinstall			\
	CPU_TARGET=$cpu_target		\
	OS_TARGET=linux			\
	CROSSBINDIR="$crossbindir"	\
	OPT=$opt			\
	INSTALL_PREFIX=/data/data/com.pdaxrom.cctools/root/cctools || error "FPC tgzinstall"
#	PP=fpc				

    error "asd"

    $INSTALL -D -m 755 htop ${TMPINST_DIR}/${PKG}/cctools/bin/htop
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/htop
    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_luajit() {
    PKG=LuaJIT
    PKG_VERSION=2.0.0
    PKG_DESC="LuaJIT is a Just-In-Time Compiler (JIT) for the Lua programming language. \
Lua is a powerful, dynamic and light-weight programming language. \
It may be embedded or used as a general-purpose, stand-alone language."
    PKG_URL=http://luajit.org/download/$PKG-$PKG_VERSION.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG
    c_tag $PKG && return
    echo "build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    $MAKE $MAKEARGS CROSS=${TARGET_ARCH}- HOST_CC="gcc -m32" || error "make $MAKEARGS"

    $MAKE install DPREFIX=$TMPINST_DIR || error "make install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools

    $MAKE install DPREFIX=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib
    mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mv ${TMPINST_DIR}/${PKG}/cctools/lib/*.a ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.so*
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/luajit
    mv ${TMPINST_DIR}/${PKG}/cctools/bin/luajit-2.0.0 ${TMPINST_DIR}/${PKG}/cctools/bin/luajit
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/luajit

    if test -e ${TOPDIR}/examples/$PKG ; then
	mkdir -p ${TMPINST_DIR}/${PKG}/cctools/Examples
	cp -a ${TOPDIR}/examples/$PKG ${TMPINST_DIR}/${PKG}/cctools/Examples
	rm -rf ${TMPINST_DIR}/${PKG}/cctools/Examples/$PKG/.svn
    fi

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_openssl() {
    PKG=openssl
    PKG_VERSION=1.0.1c
    PKG_DESC="The OpenSSL Project is a collaborative effort to develop a robust, commercial-grade, full-featured, and Open Source toolkit implementing the Secure Sockets Layer (SSL v2/v3) and Transport Layer Security (TLS v1) protocols as well as a full-strength general purpose cryptography library."
    PKG_URL=http://www.openssl.org/source/$PKG-$PKG_VERSION.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    case $TARGET_ARCH in
    arm*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-armv4
	;;
    mips*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-generic32
	;;
    i*86*)
	CC=${TARGET_ARCH}-gcc ./Configure no-shared linux-elf
	;;
    esac

    $MAKE CC=${TARGET_ARCH}-gcc AR="${TARGET_ARCH}-ar rcs" RANLIB=${TARGET_ARCH}-ranlib || error "openssl"
    cp -RL include/openssl ${TMPINST_DIR}/include/openssl
    install -D -m 644 libcrypto.a ${TMPINST_DIR}/lib/libcrypto.a
    install -D -m 644 libssl.a    ${TMPINST_DIR}/lib/libssl.a

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include
    cp -RL include/openssl ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/include/openssl
    install -D -m 644 libcrypto.a ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/libcrypto.a
    install -D -m 644 libssl.a    ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/lib/libssl.a
    install -D -m 755 apps/openssl ${TMPINST_DIR}/${PKG}/cctools/bin/openssl

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_apr() {
    PKG=apr
    PKG_VERSION=1.4.6
    PKG_DESC="The mission of the Apache Portable Runtime (APR) project is to create and maintain software libraries that provide a predictable and consistent interface to underlying platform-specific implementations."
    PKG_URL=http://mirrors.besplatnyeprogrammy.ru/apache/$PKG/$PKG-$PKG_VERSION.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
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
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_aprutil() {
    PKG=apr-util
    PKG_DESC="The mission of the Apache Portable Runtime (APR) project is to create and maintain software libraries that provide a predictable and consistent interface to underlying platform-specific implementations."
    PKG_VERSION=1.5.1
    PKG_URL=http://mirrors.besplatnyeprogrammy.ru/apache/apr/$PKG-$PKG_VERSION.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    #patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

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
	./configure --host=$TARGET_ARCH --prefix=$TMPINST_DIR \
	--with-apr=$TMPINST_DIR --with-openssl=$TMPINST_DIR \
	--with-sqlite3=$TMPINST_DIR --with-expat=$TMPINST_DIR \
	--disable-shared --enable-static || error "configure"

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
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_neon() {
    PKG=neon
    PKG_DESC="neon is an HTTP and WebDAV client library, with a C interface."
    PKG_VERSION=0.29.6
    PKG_URL=http://www.webdav.org/neon/${PKG}-${PKG_VERSION}.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

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
	./configure --host=$TARGET_ARCH --prefix=$TMPINST_DIR \
	--with-ssl=openssl --with-libs=${TMPINST_DIR} --with-expat \
	--disable-shared --enable-static || error "configure"

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
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_sqlite() {
    PKG=sqlite-autoconf
    PKG_VERSION=3071502
    PKG_DESC="SQLite is a software library that implements a self-contained, serverless, zero-configuration, transactional SQL database engine."
    PKG_URL=http://www.sqlite.org/$PKG-$PKG_VERSION.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

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

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig ${TMPINST_DIR}/${PKG}/cctools/lib/*.la
    mkdir ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/
    mv ${TMPINST_DIR}/${PKG}/cctools/lib     ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/

    local filename="sqlite_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_expat() {
    PKG=expat
    PKG_VERSION=2.1.0
    PKG_DESC="Expat is an XML parser library written in C. It is a stream-oriented parser in which an application registers handlers for things the parser might find in the XML document (like start tags)."
    PKG_URL=http://downloads.sourceforge.net/project/expat/expat/$PKG_VERSION/$PKG-$PKG_VERSION.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
#    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

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
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_subversion() {
    PKG=subversion
    PKG_VERSION=1.7.9
    PKG_DESC="Subversion exists to be universally recognized and adopted as an open-source, centralized version control system characterized by its reliability as a safe haven for valuable data; the simplicity of its model and usage; and its ability to support the needs of a wide variety of users and projects, from individuals to large-scale enterprise operations."
    PKG_URL=http://mirrors.besplatnyeprogrammy.ru/apache/$PKG/$PKG-$PKG_VERSION.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG
    c_tag $PKG && return
    echo "build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    #patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    CFLAGS="-I$TMPINST_DIR/include" LDFLAGS="-L$TMPINST_DIR/lib" \
	./configure --host=$TARGET_ARCH --prefix=$TARGET_INST_DIR \
	--with-apr=$TMPINST_DIR --with-apr-util=$TMPINST_DIR \
	--with-neon=$TMPINST_DIR \
	--with-sqlite=$TMPINST_DIR --enable-static --disable-shared || error "configure"


    $MAKE $MAKEARGS || error "make $MAKEARGS"

    #$MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/lib

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_curl() {
    PKG=curl
    PKG_VERSION=7.28.1
    PKG_DESC="cURL is a computer software project providing a library and command-line tool for transferring data using various protocols."
    PKG_URL=http://curl.haxx.se/download/${PKG}-${PKG_VERSION}.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
#    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    ./configure --host=$TARGET_ARCH --prefix=$TMPINST_DIR --disable-shared --enable-static \
	--with-ssl=${TMPINST_DIR} || error "configure"

    $MAKE $MAKEARGS || error "make"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig ${TMPINST_DIR}/${PKG}/cctools/lib/*.la
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/curl-config
    mkdir ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/
    mv ${TMPINST_DIR}/${PKG}/cctools/lib     ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_wget() {
    PKG=wget
    PKG_VERSION=1.14
    PKG_DESC="GNU Wget is a free software package for retrieving files using HTTP, HTTPS and FTP, the most widely-used Internet protocols. It is a non-interactive commandline tool, so it may easily be called from scripts, cron jobs, terminals without X-Windows support, etc."
    PKG_URL=http://ftp.gnu.org/gnu/${PKG}/${PKG}-${PKG_VERSION}.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
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

    case $TARGET_ARCH in
    i*86*|x86*)
	s_tag $PKG
	return
	;;
    esac

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
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_git() {
    PKG=git
    PKG_VERSION=1.8.1.2
    PKG_DESC="Git is a distributed revision control and source code management (SCM) system with an emphasis on speed.[3] Git was initially designed and developed by Linus Torvalds for Linux kernel development; it has since been adopted by many other projects."
    PKG_URL=http://git-core.googlecode.com/files/${PKG}-${PKG_VERSION}.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
#    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    ac_cv_fread_reads_directories=no \
    ac_cv_snprintf_returns_bogus=no \
    CFLAGS="-O2 -DNO_GECOS_IN_PWENT -DNO_NSEC -DNO_ICONV" \
    LIBS="-L${TMPINST_DIR}/lib -lcurl -lssl -lcrypto -lz" \
	./configure --host=$TARGET_ARCH --prefix=$TARGET_INST_DIR \
	--with-openssl=${TMPINST_DIR} --with-curl=${TMPINST_DIR} --with-expat=${TMPINST_DIR} \
	--without-iconv \
	--without-tcltk --without-python || error "configure"

    $MAKE $MAKEARGS NEEDS_SSL_WITH_CURL=1 NEEDS_CRYPTO_WITH_SSL=1 || error "make"

    #$MAKE install NEEDS_SSL_WITH_CURL=1 NEEDS_CRYPTO_WITH_SSL=1 NO_INSTALL_HARDLINKS=1 || error "make install"

    $MAKE install NEEDS_SSL_WITH_CURL=1 NEEDS_CRYPTO_WITH_SSL=1 NO_INSTALL_HARDLINKS=1 prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    #rm -rf ${TMPINST_DIR}/${PKG}/cctools/share ${TMPINST_DIR}/${PKG}/cctools/lib/pkgconfig ${TMPINST_DIR}/${PKG}/cctools/lib/*.la
    #mkdir ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}
    #mv ${TMPINST_DIR}/${PKG}/cctools/include ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/
    #mv ${TMPINST_DIR}/${PKG}/cctools/lib     ${TMPINST_DIR}/${PKG}/cctools/${TARGET_ARCH}/

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_binutils_avr_host() {
    PKG=binutils
    PKG_VERSION=2.22
    PKG_DESC="Binary utilities supporting Atmel's AVR targets"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-binutils-avr

    c_tag ${PKG}-host-avr && return

    echo "build $PKG"

    pushd .

    copysrc $O_DIR $S_DIR

    cd $S_DIR
    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure \
	--target=avr \
	--prefix=${TARGET_DIR}-host \
	--disable-werror || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host-avr
}

build_binutils_avr() {
    PKG=binutils
    PKG_VERSION=2.22
    PKG_DESC="Binary utilities supporting Atmel's AVR targets"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/$PKG-avr

    c_tag ${PKG}-avr && return

    echo "build $PKG"

    pushd .

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure \
	--target=avr \
	--host=$TARGET_ARCH \
	--prefix=$TARGET_DIR \
	--disable-werror || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/avr/bin/*
    ln -sf ../../bin/avr-ar ${TMPINST_DIR}/${PKG}/cctools/avr/bin/ar
    ln -sf ../../bin/avr-as ${TMPINST_DIR}/${PKG}/cctools/avr/bin/as
    ln -sf ../../bin/avr-ld ${TMPINST_DIR}/${PKG}/cctools/avr/bin/ld
    ln -sf ../../bin/avr-ld.bfd ${TMPINST_DIR}/${PKG}/cctools/avr/bin/ld.bfd
    ln -sf ../../bin/avr-nm ${TMPINST_DIR}/${PKG}/cctools/avr/bin/nm
    ln -sf ../../bin/avr-objcopy ${TMPINST_DIR}/${PKG}/cctools/avr/bin/objcopy
    ln -sf ../../bin/avr-objdump ${TMPINST_DIR}/${PKG}/cctools/avr/bin/objdump
    ln -sf ../../bin/avr-ranlib ${TMPINST_DIR}/${PKG}/cctools/avr/bin/ranlib
    ln -sf ../../bin/avr-strip ${TMPINST_DIR}/${PKG}/cctools/avr/bin/strip

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.a
    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/avr-ld.bfd
    ln -sf avr-ld ${TMPINST_DIR}/${PKG}/cctools/bin/avr-ld.bfd

    local filename="${PKG}-avr_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-avr $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag ${PKG}-avr
}

build_gmp_host() {
    PKG=gmp
    PKG_VERSION=5.0.5
    PKG_DESC="Multiprecision arithmetic library."
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}

    c_tag ${PKG}-host && return

    echo "build $PKG"

    pushd .

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure --prefix=${TARGET_DIR}-host --disable-werror --enable-static --disable-shared || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host
}

build_gmp() {
    PKG=gmp
    PKG_VERSION=5.0.5
    PKG_DESC="Multiprecision arithmetic library."
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return

    echo "build $PKG"

    pushd .

#    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure --target=$TARGET_ARCH --host=$TARGET_ARCH --prefix=$TMPINST_DIR --disable-werror --enable-static --disable-shared || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.la

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_mpfr_host() {
    PKG=mpfr
    PKG_VERSION=2.4.2
    PKG_DESC="Multiple precision floating-point computation."
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}

    c_tag ${PKG}-host && return

    echo "build $PKG"

    pushd .

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure --prefix=${TARGET_DIR}-host --with-gmp=${TARGET_DIR}-host --disable-werror --enable-static --disable-shared || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host
}

build_mpfr() {
    PKG=mpfr
    PKG_VERSION=2.4.2
    PKG_DESC="Multiple precision floating-point computation."
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return

    echo "build $PKG"

    pushd .

    #tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    #cd $S_DIR
    #patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure --target=$TARGET_ARCH --host=$TARGET_ARCH --prefix=$TMPINST_DIR --with-gmp=$TMPINST_DIR --disable-werror --enable-static --disable-shared || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.la

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_mpc_host() {
    PKG=mpc
    PKG_VERSION=0.8.1
    PKG_DESC="multiple precision complex floating-point library"
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}

    c_tag ${PKG}-host && return

    echo "build $PKG"

    pushd .

    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure --prefix=${TARGET_DIR}-host --with-gmp=${TARGET_DIR}-host --with-mpfr=${TARGET_DIR}-host --disable-werror --enable-static --disable-shared || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-host
}

build_mpc() {
    PKG=mpc
    PKG_VERSION=0.8.1
    PKG_DESC="multiple precision complex floating-point library"
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return

    echo "build $PKG"

    pushd .

#    tar zxf $O_FILE -C $src_dir || error "tar zxf $O_FILE"

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    $S_DIR/configure --target=$TARGET_ARCH --host=$TARGET_ARCH --prefix=$TMPINST_DIR --with-gmp=$TMPINST_DIR --with-mpfr=$TMPINST_DIR --disable-werror --enable-static --disable-shared || error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/*.la

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_gcc_avr_host() {
    PKG=gcc
    PKG_VERSION=4.7
    PKG_DESC="The GNU C compiler (cross compiler for avr)"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}-avr

    c_tag ${PKG}-avr-host && return

    echo "build $PKG"

    pushd .

    copysrc $O_DIR $S_DIR

    cd $S_DIR
    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

    $S_DIR/configure \
	--target=avr \
	--prefix=${TARGET_DIR}-host \
	--enable-languages=c,c++ \
	--with-gmp=${TARGET_DIR}-host \
	--with-mpfr=${TARGET_DIR}-host \
	--with-mpc=${TARGET_DIR}-host \
	--enable-long-long \
	--disable-libssp \
	--disable-nls \
	--disable-shared \
	--with-binutils-version=2.22 \
	--with-mpfr-version=2.4.2 \
	--with-mpc-version=0.8.1 \
	--with-gmp-version=5.0.5 \
	--with-gcc-version=4.7 \
	--disable-bootstrap \
	--disable-libquadmath \
	--disable-plugin \
	|| error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    popd
    s_tag ${PKG}-avr-host
}

build_gcc_avr() {
    PKG=gcc
    PKG_VERSION=4.7
    PKG_DESC="The GNU C compiler (cross compiler for avr)"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/$PKG

    c_tag ${PKG}-avr && return

    echo "build $PKG"

    pushd .

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

    $S_DIR/configure \
	--target=avr \
	--host=$TARGET_ARCH \
	--prefix=$TARGET_DIR \
	--build=`uname -m`-unknown-linux-gnu \
	--enable-languages=c,c++ \
	--with-gmp=$TMPINST_DIR \
	--with-mpfr=$TMPINST_DIR \
	--with-mpc=$TMPINST_DIR \
	--enable-long-long \
	--disable-libssp \
	--disable-nls \
	--disable-shared \
	--with-binutils-version=2.22 \
	--with-mpfr-version=2.4.2 \
	--with-mpc-version=0.8.1 \
	--with-gmp-version=5.0.5 \
	--with-gcc-version=4.7 \
	--disable-bootstrap \
	--disable-libquadmath \
	--disable-plugin \
	|| error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/libiberty.a

    ln -sf avr-g++ ${TMPINST_DIR}/${PKG}/cctools/bin/avr-c++
    ln -sf avr-gcc ${TMPINST_DIR}/${PKG}/cctools/bin/avr-gcc-4.7
    ln -sf avr-gcc ${TMPINST_DIR}/${PKG}/cctools/bin/avr-cc

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/avr/4.7/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/avr/4.7/install-tools/*

    local filename="${PKG}-avr_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG}-avr $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag ${PKG}-avr
}

build_avr_libc() {
    PKG=avr-libc
    PKG_VERSION=1.8.0
    PKG_DESC="The C runtime library for the AVR family of microcontrollers."
    PKG_URL=http://download.savannah.gnu.org/releases/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.bz2
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
#    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

    ./configure --host=avr --prefix=$TARGET_INST_DIR || error "configure"

    $MAKE $MAKEARGS || error "make"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -f ${TMPINST_DIR}/${PKG}/cctools/bin/avr-man
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/Examples/avr-libc
    cp -a  ${TMPINST_DIR}/${PKG}/cctools/share/doc/avr-libc-1.8.0/examples/* ${TMPINST_DIR}/${PKG}/cctools/Examples/avr-libc/
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_fortran_host() {
    PKG=gcc
    PKG_VERSION=4.7
    PKG_DESC="The GNU C compiler (cross compiler for avr)"
    O_DIR=$SRC_PREFIX/$PKG/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/host-${PKG}-fortran

    c_tag ${PKG}-host && return

    echo "build $PKG"

    pushd .

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

    local EXTRA_CONF=
    case $TARGET_ARCH in
    mips*)
	EXTRA_CONF="--with-arch=mips32 --disable-threads --disable-fixed-point"
	;;
    arm*)
	EXTRA_CONF="--with-arch=armv5te --with-float=soft --with-fpu=vfp"
	;;
    *)
	;;
    esac

    $S_DIR/configure \
	--target=$TARGET_ARCH \
	--prefix=${TARGET_DIR}-host \
	--enable-languages=fortran \
	--with-gmp=${TARGET_DIR}-host \
	--with-mpfr=${TARGET_DIR}-host \
	--with-mpc=${TARGET_DIR}-host \
	--with-gnu-as \
	--with-gnu-ld \
	--without-ppl \
	--without-cloog \
	--disable-libssp \
	--enable-threads \
	--disable-nls \
	--disable-libmudflap \
	--disable-libgomp \
	--disable-libstdc__-v3 \
	--disable-sjlj-exceptions \
	--disable-shared \
	--disable-tls \
	--disable-libitm \
	--enable-initfini-array \
	--disable-nls \
	--with-binutils-version=2.22 \
	--with-mpfr-version=2.4.2 \
	--with-mpc-version=0.8.1 \
	--with-gmp-version=5.0.5 \
	--with-gcc-version=4.7 \
	--disable-bootstrap \
	--disable-libquadmath \
	--disable-libquadmath-support \
	--disable-plugin \
	$EXTRA_CONF \
	--with-sysroot=$SYSROOT \
	|| error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install || error "make install"

    ln -sf ${SYSROOT}/../${TARGET_ARCH}/bin ${TARGET_DIR}-host/${TARGET_ARCH}/bin

    popd
    s_tag ${PKG}-host
}

build_fortran() {
    PKG=gfortran
    PKG_VERSION=4.7
    PKG_DESC="The GNU fortran compiler"
    O_DIR=$SRC_PREFIX/gcc/gcc-${PKG_VERSION}
    S_DIR=$src_dir/gcc-${PKG_VERSION}
    B_DIR=$build_dir/$PKG

    c_tag ${PKG} && return

    echo "build $PKG"

    pushd .

#    copysrc $O_DIR $S_DIR

#    cd $S_DIR
#    patch -p1 < $patch_dir/${PKG}-${PKG_VERSION}.patch

    mkdir -p $B_DIR
    cd $B_DIR

    export PATH=${TARGET_DIR}-host/bin:$PATH

    local EXTRA_CONF=
    case $TARGET_ARCH in
    mips*)
	EXTRA_CONF="--with-arch=mips32 --disable-threads --disable-fixed-point"
	;;
    arm*)
	EXTRA_CONF="--with-arch=armv5te --with-float=soft --with-fpu=vfp"
	;;
    *)
	;;
    esac

    $S_DIR/configure \
	--target=$TARGET_ARCH \
	--host=$TARGET_ARCH \
	--prefix=$TARGET_DIR \
	--with-gnu-as \
	--with-gnu-ld \
	--enable-languages=fortran \
	--with-gmp=$TMPINST_DIR \
	--with-mpfr=$TMPINST_DIR \
	--without-ppl \
	--without-cloog \
	--disable-libssp \
	--enable-threads \
	--disable-nls \
	--disable-libmudflap \
	--disable-libgomp \
	--disable-libstdc__-v3 \
	--disable-sjlj-exceptions \
	--disable-shared \
	--disable-tls \
	--disable-libitm \
	--enable-initfini-array \
	--disable-nls \
	--with-binutils-version=2.22 \
	--with-mpfr-version=2.4.2 \
	--with-mpc-version=0.8.1 \
	--with-gmp-version=5.0.5 \
	--with-gcc-version=4.7 \
	--disable-bootstrap \
	--disable-libquadmath \
	--disable-libquadmath-support \
	--disable-plugin \
	--with-sysroot=$SYSROOT_DIR \
	$EXTRA_CONF \
	|| error "configure"

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/include

    for f in bin/gcc-nm bin/*-gcc-ar bin/*-gfortran bin/*-gcc bin/gcc-ar bin/*-gcc-nm \
	bin/*-ranlib bin/gcc bin/cpp bin/*-gcc-4.7 bin/gcc-ranlib bin/gcov; do
	rm -f ${TMPINST_DIR}/${PKG}/cctools/${f}
    done

    find ${TMPINST_DIR}/${PKG}/cctools/lib -name "*.la" | xargs rm -f
    rm -f ${TMPINST_DIR}/${PKG}/cctools/lib/libiberty.a

    mv ${TMPINST_DIR}/${PKG}/cctools/lib/gcc ${TMPINST_DIR}/${PKG}/cctools/

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/gcc/${TARGET_ARCH}/${PKG_VERSION}/include
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/gcc/${TARGET_ARCH}/${PKG_VERSION}/include-fixed
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/gcc/${TARGET_ARCH}/${PKG_VERSION}/install-tools

    find ${TMPINST_DIR}/${PKG}/cctools/gcc/${TARGET_ARCH}/${PKG_VERSION} -name "*.o" | xargs rm -f
    find ${TMPINST_DIR}/${PKG}/cctools/gcc/${TARGET_ARCH}/${PKG_VERSION} -name "libgcc.*" | xargs rm -f
    find ${TMPINST_DIR}/${PKG}/cctools/gcc/${TARGET_ARCH}/${PKG_VERSION} -name "libgcov.*" | xargs rm -f

    cp -a ${TMPINST_DIR}/${PKG}/cctools/lib/* ${TMPINST_DIR}/${PKG}/cctools/gcc/${TARGET_ARCH}/${PKG_VERSION}/
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/lib/*

    mv ${TMPINST_DIR}/${PKG}/cctools/gcc ${TMPINST_DIR}/${PKG}/cctools/lib/

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/install-tools
    for f in cc1  collect2  liblto_plugin.la  liblto_plugin.so  liblto_plugin.so.0  liblto_plugin.so.0.0.0  lto1  lto-wrapper; do
	rm -f ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/${f}
    done

    ${TARGET_ARCH}-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*
    ${TARGET_ARCH}-strip ${TMPINST_DIR}/${PKG}/cctools/libexec/gcc/${TARGET_ARCH}/${PKG_VERSION}/*

    ln -sf gfortran ${TMPINST_DIR}/${PKG}/cctools/bin/f77

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag ${PKG}
}

build_fortran_examples() {
    PKG=gfortran-examples
    PKG_VERSION=1.0
    PKG_DESC="Few fortran examples"
    c_tag ${PKG} && return

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/Examples
    cp -a ${TOPDIR}/examples/fortran ${TMPINST_DIR}/${PKG}/cctools/Examples
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/Examples/$PKG/.svn

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    s_tag ${PKG}
}

build_netcat_gnu() {
    PKG=netcat
    PKG_VERSION=0.7.1
    PKG_DESC="GNU netcat is a featured networking utility which reads and writes data across network connections, using the TCP/IP protocol."
    PKG_URL=http://downloads.sourceforge.net/project/netcat/${PKG}/${PKG_VERSION}/${PKG}-${PKG_VERSION}.tar.bz2
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG

    c_tag $PKG && return
    echo "Build $PKG"
    pushd .
    mkdir -p $SRC_PREFIX/$PKG
    test -e $O_FILE || wget $PKG_URL -O $O_FILE || error "download $PKG_URL"

    tar jxf $O_FILE -C $src_dir || error "tar jxf $O_FILE"

    cd $S_DIR
    patch -p1 < $patch_dir/$PKG-$PKG_VERSION.patch || error "patch"

#    mkdir -p $B_DIR
    copysrc $S_DIR  $B_DIR
    cd $B_DIR

    ./configure --host=$TARGET_ARCH --prefix=$TARGET_INST_DIR || error "configure"

    $MAKE $MAKEARGS || error "make"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/info
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/man

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    popd
    s_tag $PKG
}

build_netcat() {
    PKG=netcat
    PKG_VERSION=1.10
    PKG_DESC="Netcat is a featured networking utility which reads and writes data across network connections, using the TCP/IP protocol."
    c_tag ${PKG} && return

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/bin

    ${TARGET_ARCH}-gcc ${SRC_PREFIX}/nc/netcat.c -s -o ${TMPINST_DIR}/${PKG}/cctools/bin/netcat || error "compilation"
    ln -s netcat ${TMPINST_DIR}/${PKG}/cctools/bin/nc

    local filename="${PKG}_${PKG_VERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename ${PKG} $PKG_VERSION $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    zip -r9y ../$filename cctools pkgdesc

    s_tag ${PKG}
}

makedirs
build_ncurses
build_busybox
build_htop
build_luajit
build_openssl
build_expat
build_sqlite
build_apr
build_aprutil
build_neon
build_subversion
build_curl
build_wget
build_git
build_dropbear
#build_fpc
#build_nano
#build_emacs
build_binutils_avr_host
build_binutils_avr
build_gmp_host
build_gmp
build_mpfr_host
build_mpfr
build_mpc_host
build_mpc
build_gcc_avr_host
build_gcc_avr
build_avr_libc
build_fortran_host
build_fortran
build_fortran_examples
build_netcat
