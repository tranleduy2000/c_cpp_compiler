dropbear_version=0.52
build_dropbear() {
    PKG=dropbear
    PKG_VERSION=$dropbear_version
    PKG_SUBVERSION="-2"
    PKG_DESC="Dropbear is a software package written by Matt Johnston that provides a Secure Shell-compatible server and client. It is designed as a replacement for standard OpenSSH for environments with low memory and processor resources, such as embedded systems."
    PKG_URL="http://matt.ucc.asn.au/dropbear/releases/dropbear-$dropbear_version.tar.gz"
    O_FILE=$SRC_PREFIX/dropbear/dropbear-$dropbear_version.tar.gz
    S_DIR=$src_dir/dropbear-$dropbear_version
    B_DIR=$build_dir/dropbear
    c_tag $PKG && return

    banner "Build $PKG"

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

ret=\`adialog --editbox --title "Dropbear SSH server" --message "Create a new password"\`

status=\`echo \$ret | cut -f1 -d' '\`

case \$status in
ok)
    passwd=\`echo \$ret | cut -f2 -d' '\`
    echo \$passwd > \${CCTOOLSDIR}/etc/dropbear.passwd
    ;;
*)
    ;;
esac

iface=\`ip r l | grep default | head -n1 | awk '{ for (i = 1; \$i != ""; i++) { if (\$i == "dev") print \$(i + 1) } }'\`
ip=\`ifconfig \$iface | grep 'inet addr:' | cut -d: -f2 | awk '{ print \$1}'\`
adialog --msgbox --title "Dropbear SSH server" --message "SSH access to CCTools shell: ssh -p 22022 alpine@\${ip}" --text "Use dropbearpasswd to change SSH password from console."

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

    local filename="${PKG}_${PKG_VERSION}${PKG_SUBVERSION}_${PKG_ARCH}.zip"
    build_package_desc ${TMPINST_DIR}/${PKG} $filename $PKG ${PKG_VERSION}${PKG_SUBVERSION} $PKG_ARCH "$PKG_DESC" "adialog"
    cd ${TMPINST_DIR}/${PKG}
    rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename cctools pkgdesc postinst prerm

    popd
    s_tag $PKG
}
