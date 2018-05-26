build_fpc() {
    PKG=fpc
    PKG_VERSION=2.6.0
    PKG_DESC="Free Pascal Compiler (FPC for short) is a free Pascal and Object Pascal compiler."
    PKG_URL=http://downloads.sourceforge.net/project/freepascal/Source/$PKG_VERSION/$PKG-$PKG_VERSION.source.tar.gz
    O_FILE=$SRC_PREFIX/$PKG/$PKG-$PKG_VERSION.source.tar.gz
    S_DIR=$src_dir/$PKG-$PKG_VERSION
    B_DIR=$build_dir/$PKG
    c_tag $PKG && return

    banner "Build $PKG"

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
    remove_rpath cctools
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
    s_tag $PKG
}
