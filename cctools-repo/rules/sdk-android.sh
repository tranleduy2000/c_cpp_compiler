build_sdk_android() {
    PKG=sdk-android
    PKG_VERSION=1.0
    PKG_SUBVERSION=
    PKG_DESC="Android java sdk"

    c_tag $PKG && return

    banner "Build $PKG"

    pushd .

    cd ${SDK_DIR}/platforms/

    local d=

    for d in *; do

	pushd .

	local vers=${d/*-}

	cd $d

	banner "Build sdk $vers"

	rm -rf   ${TMPINST_DIR}/${PKG}-${vers}/cctools/sdk
	mkdir -p ${TMPINST_DIR}/${PKG}-${vers}/cctools/sdk/${d}

	copysrc templates  ${TMPINST_DIR}/${PKG}-${vers}/cctools/sdk/${d}/template
	cp -f *.jar        ${TMPINST_DIR}/${PKG}-${vers}/cctools/sdk/${d}
	cp -f *.aidl       ${TMPINST_DIR}/${PKG}-${vers}/cctools/sdk/${d}
	cp -f *.prop       ${TMPINST_DIR}/${PKG}-${vers}/cctools/sdk/${d}
	cp -f *.properties ${TMPINST_DIR}/${PKG}-${vers}/cctools/sdk/${d}

	local filename="${PKG}-${vers}_${PKG_VERSION}${PKG_SUBVERSION}_all.zip"
	build_package_desc ${TMPINST_DIR}/${PKG}-${vers} $filename ${PKG}-${vers} ${PKG_VERSION}${PKG_SUBVERSION} all "$PKG_DESC"
	cd ${TMPINST_DIR}/${PKG}-${vers}
	rm -f ${REPO_DIR}/$filename; zip -r9y ${REPO_DIR}/$filename *

	popd
    done

    popd

    s_tag $PKG
}
