build_dropbear() {
    PKG=dropbear
    PKG_VERSION=2016.74
    PKG_SUBVERSION=
    PKG_URL="https://matt.ucc.asn.au/dropbear/${PKG}-${PKG_VERSION}.tar.bz2"
    PKG_DESC="Small SSH client and server."
    PKG_DEPS=""
    O_FILE=$SRC_PREFIX/${PKG}/${PKG}-${PKG_VERSION}.tar.bz2
    S_DIR=$src_dir/${PKG}-${PKG_VERSION}
    B_DIR=$build_dir/${PKG}

    c_tag $FUNCNAME && return

    pushd .

    banner "Build $PKG"

    download $PKG_URL $O_FILE

    unpack $src_dir $O_FILE

    patchsrc $S_DIR $PKG $PKG_VERSION

    copysrc $S_DIR $B_DIR

    cd $B_DIR

    # Configure here

    ./configure	\
		--host=${TARGET_ARCH} \
		--prefix=$TMPINST_DIR \
		--disable-loginfunc \
		--disable-shadow \
		--disable-utmp \
		--disable-wtmp \
		--disable-pututline \
		--disable-pututxline \
		--disable-lastlog \
			|| error "Configure $PKG."

    echo "#define USE_DEV_PTMX 1" >> config.h

    #sed -i -e 's|\/\*#define DEBUG_TRACE\*\/|#define DEBUG_TRACE 1|' debug.h

    $MAKE $MAKEARGS PROGRAMS="dropbear dbclient scp dropbearkey dropbearconvert" SCPPROGRESS=1 || error "make $MAKEARGS"

    $MAKE PROGRAMS="dropbear dbclient scp dropbearkey dropbearconvert" SCPPROGRESS=1 install prefix=${TMPINST_DIR}/${PKG}/cctools || error "package install"

    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/services
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/etc/dropbear
    mkdir -p ${TMPINST_DIR}/${PKG}/cctools/var/run

    echo "alpine" >> ${TMPINST_DIR}/${PKG}/cctools/etc/dropbear/dropbear.passwd

    cat > ${TMPINST_DIR}/${PKG}/cctools/services/dropbear << EOF
#!/system/bin/sh

case \$1 in
start)
    if [ ! -f \${CCTOOLSDIR}/etc/dropbear/dropbear_rsa_host_key ]; then
	dropbearkey -t rsa -f \${CCTOOLSDIR}/etc/dropbear/dropbear_rsa_host_key
	dropbearconvert dropbear openssh \${CCTOOLSDIR}/etc/dropbear/dropbear_rsa_host_key \${EXTERNAL_STORAGE}/CCTools/cctools_id_rsa_openssh
    fi

    dropbear -A -N cctools -C \`cat \${CCTOOLSDIR}/etc/dropbear/dropbear.passwd\` -r \${CCTOOLSDIR}/etc/dropbear/dropbear_rsa_host_key -p 22022 -P \${CCTOOLSDIR}/var/run/dropbear.pid

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

    cat > ${TMPINST_DIR}/${PKG}/postinst << EOF
#!/system/bin/sh

fret=\${CCTOOLSDIR}/tmp/adialog\$\$

am start \$(am 2>&1 | grep -q '\-\-user' && echo '--user 0') -n com.pdaxrom.cctools/.ADialog \\
    --es type editbox \\
    --ez password true \\
    --es title "Dropbear SSH server" \\
    --es message "Create a new password" \\
    --es return \$fret

sleep 1

if [ -f \${fret}.lock ]; then

    while [ -f \${fret}.lock ]; do
	sleep 1
    done
    status=\$(cat \$fret)

    case \$status in
    ok)
	cat \${fret}.out > \${CCTOOLSDIR}/etc/dropbear/dropbear.passwd
	;;
    *)
	;;
    esac

    rm -f \${fret}
    rm -f \${fret}.out


    iface=\`ip r l | grep default | head -n1 | awk '{ for (i = 1; \$i != ""; i++) { if (\$i == "dev") print \$(i + 1) } }'\`
    ip=\`ifconfig \$iface | grep 'inet addr:' | cut -d: -f2 | awk '{ print \$1}'\`

    am start \$(am 2>&1 | grep -q '\-\-user' && echo '--user 0') -n com.pdaxrom.cctools/.ADialog \\
	--es type textview \\
	--es title "Dropbear SSH server" \\
	--es message "SSH access to CCTools shell: ssh -p 22022 cctools@\${ip}" \\
	--es text "Use command 'dropbearpasswd <new password>' to change SSH password from console."

else

    #
    # Obsolete and removed from CCTools 1.21+
    #

    ret=\`adialog --editbox --title "Dropbear SSH server" --message "Create a new password"\`
    status=\`echo \$ret | cut -f1 -d' '\`

    case \$status in
    ok)
	passwd=\`echo \$ret | cut -f2 -d' '\`
	echo \$passwd > \${CCTOOLSDIR}/etc/dropbear/dropbear.passwd
	;;
    *)
	;;
    esac

    iface=\`ip r l | grep default | head -n1 | awk '{ for (i = 1; \$i != ""; i++) { if (\$i == "dev") print \$(i + 1) } }'\`
    ip=\`ifconfig \$iface | grep 'inet addr:' | cut -d: -f2 | awk '{ print \$1}'\`

    adialog --msgbox --title "Dropbear SSH server" --message "SSH access to CCTools shell: ssh -p 22022 alpine@\${ip}" --text "Use dropbearpasswd to change SSH password from console."

fi

\${CCTOOLSDIR}/services/dropbear start

EOF

    cat > ${TMPINST_DIR}/${PKG}/prerm << EOF
#!/system/bin/sh

\${CCTOOLSDIR}/services/dropbear stop

EOF

    cat > ${TMPINST_DIR}/${PKG}/cctools/bin/dropbearpasswd << EOF
#!/system/bin/sh

echo "\$1" > \${CCTOOLSDIR}/etc/dropbear/dropbear.passwd

\${CCTOOLSDIR}/services/dropbear stop
\${CCTOOLSDIR}/services/dropbear start

EOF

    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/services/dropbear
    chmod 755 ${TMPINST_DIR}/${PKG}/postinst
    chmod 755 ${TMPINST_DIR}/${PKG}/prerm
    chmod 755 ${TMPINST_DIR}/${PKG}/cctools/bin/dropbearpasswd

    make_packages

    popd
    s_tag $FUNCNAME
}
