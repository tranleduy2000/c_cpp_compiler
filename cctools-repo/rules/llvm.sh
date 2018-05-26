build_llvm() {
    PKG=llvm
    PKG_VERSION=3.6
    PKG_SUBVERSION="-1"
    PKG_URL="http://ya.ru/${PKG}-${PKG_VERSION}.tgz"
    PKG_DESC="Low-Level Virtual Machine (LLVM), runtime library"
    O_DIR=$SRC_PREFIX/${PKG}-${PKG_VERSION}
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}-${PKG_VERSION}/llvm

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .

    preparesrc $O_DIR $S_DIR

    if [ "$BUILD_PIE_COMPILER" = "yes" ]; then
	cd $S_DIR
	if [ ! -e ${S_DIR}/.pie_patched ]; then
	    patch -p1 < ${patch_dir}/${PKG}-${PKG_VERSION}-pie.patch || error "No pie patch found!"
	    touch ${S_DIR}/.pie_patched
	fi
    fi

    mkdir -p $B_DIR
    cd $B_DIR

    # Configure here

    local CLANG_TARGET=
    case $TARGET_ARCH in
    arm*)
	CLANG_TARGET="arm"
	;;
    mips*)
	CLANG_TARGET="mips"
	;;
    x86*|i*86*|amd64*)
	CLAGS_TARGET="x86"
	;;
    *)
#	CLANG_TARGET="arm,mips,x86"
	CLANG_TARGET="arm,mips,x86,arm64,mips64,x86_64"
	;;
    esac

    #
    # enable all android architectures
    #
    CLANG_TARGET="arm,mips,x86"

    EXTRA_CONFIG_FLAGS=

    ac_cv_func_mmap_fixed_mapped=yes \
    ac_cv_func_mmap_file=yes\
    $S_DIR/llvm/configure \
    --target=$TARGET_ARCH \
    --host=$TARGET_ARCH \
    --prefix=$TMPINST_DIR \
    --enable-targets=$CLANG_TARGET \
    --enable-optimized \
    --disable-assertions \
    --disable-debug-runtime\
    --disable-expensive-checks \
    --with-binutils-include=$SRC_PREFIX/binutils/binutils-$binutils_version/include \
    --enable-keep-symbols \
    CFLAGS="-I${TMPINST_DIR}/include" \
    LDFLAGS="-L${TMPINST_DIR}/lib" \
    $EXTRA_CONFIG_FLAGS

#    --enable-shared
#    --disable-static

    $MAKE $MAKEARGS || error "make $MAKEARGS"

    $MAKE install prefix=$TMPINST_DIR || error "make install"

    $MAKE install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/bin/*
    $TARGET_ARCH-strip ${TMPINST_DIR}/${PKG}/cctools/lib/*.so

    rm -rf ${TMPINST_DIR}/${PKG}/cctools/docs
    #rm -f  ${TMPINST_DIR}/${PKG}/cctools/lib/*.a
    rm -rf ${TMPINST_DIR}/${PKG}/cctools/share

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    local LLVMROOTDIR=${TMPINST_DIR}/${PKG}/cctools

    popd
    s_tag $PKG

    pushd .

    PKG=clang
    PKG_DESC="C, C++ and Objective-C compiler (LLVM based)"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/bin
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/lib
    cp -f ${LLVMROOTDIR}/bin/${TARGET_ARCH}-clang ${TMPINST_DIR}/${PKG}/cctools/bin/clang
    ln -sf clang ${TMPINST_DIR}/${PKG}/cctools/bin/clang++
    cp -Rf ${LLVMROOTDIR}/lib/clang ${TMPINST_DIR}/${PKG}/cctools/lib/

    #cp -f ${LLVMROOTDIR}/lib/libprofile_rt.a  ${TMPINST_DIR}/${PKG}/cctools/lib/
    #cp -f ${LLVMROOTDIR}/lib/libprofile_rt.so ${TMPINST_DIR}/${PKG}/cctools/lib/

    cat >> ${TMPINST_DIR}/${PKG}/cctools/bin/cpp-clang << EOF
#!/system/bin/sh

exec \${CCTOOLSDIR}/bin/clang -E \$@
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/cpp-clang

    cat >> ${TMPINST_DIR}/${PKG}/cctools/bin/cc-clang << EOF
#!/system/bin/sh

exec \${CCTOOLSDIR}/bin/clang -integrated-as \$@
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/cc-clang

    cat >> ${TMPINST_DIR}/${PKG}/cctools/bin/c++-clang << EOF
#!/system/bin/sh

exec \${CCTOOLSDIR}/bin/clang++ -integrated-as \$@
EOF
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/c++-clang

    cat >> ${TMPINST_DIR}/${PKG}/cctools/bin/set-default-compiler-clang << EOF
#!/system/bin/sh

ln -sf cpp-clang \${CCTOOLSDIR}/bin/cpp
ln -sf cc-clang  \${CCTOOLSDIR}/bin/gcc
ln -sf cc-clang  \${CCTOOLSDIR}/bin/cc
ln -sf c++-clang \${CCTOOLSDIR}/bin/g++
ln -sf c++-clang \${CCTOOLSDIR}/bin/c++
EOF

    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/set-default-compiler-clang

    cat >> ${TMPINST_DIR}/${PKG}/postinst << EOF
#!/system/bin/sh

set-default-compiler-clang
EOF

    chmod 755 ${TMPINST_DIR}/${PKG}/postinst

    cat >> ${TMPINST_DIR}/${PKG}/prerm << EOF
#!/system/bin/sh

test \`readlink \${CCTOOLSDIR}/bin/cpp\` = "cpp-clang" && rm -f \${CCTOOLSDIR}/bin/cpp
test \`readlink \${CCTOOLSDIR}/bin/gcc\` = "cc-clang"  && rm -f \${CCTOOLSDIR}/bin/gcc
test \`readlink \${CCTOOLSDIR}/bin/cc\`  = "cc-clang"  && rm -f \${CCTOOLSDIR}/bin/cc
test \`readlink \${CCTOOLSDIR}/bin/g++\` = "c++-clang" && rm -f \${CCTOOLSDIR}/bin/g++
test \`readlink \${CCTOOLSDIR}/bin/c++\` = "c++-clang" && rm -f \${CCTOOLSDIR}/bin/c++

which set-default-compiler-gcc && set-default-compiler-gcc
EOF

    chmod 755 ${TMPINST_DIR}/${PKG}/prerm

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "libgcc-dev|libgcc-compact-dev libstdc++-dev|libstdc++-compact-dev"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

    PKG=libclang
    PKG_DESC="clang library"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/lib
    cp -f ${LLVMROOTDIR}/lib/libclang.so ${TMPINST_DIR}/${PKG}/cctools/lib/

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    PKG=clang-utils
    PKG_DESC="clang utilities"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/bin
    cp -f ${LLVMROOTDIR}/bin/${TARGET_ARCH}-clang-check  ${TMPINST_DIR}/${PKG}/cctools/bin/clang-check
    cp -f ${LLVMROOTDIR}/bin/${TARGET_ARCH}-clang-format ${TMPINST_DIR}/${PKG}/cctools/bin/clang-format
    cp -f ${LLVMROOTDIR}/bin/${TARGET_ARCH}-clang-tblgen ${TMPINST_DIR}/${PKG}/cctools/bin/clang-tblgen

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc

    popd
}
