#!/bin/bash

oldrepo=$1
newrepo=$2

usage() {
    echo "Usage: $0 <old repo dir> <new repo dir>"
    exit 0
}

if [ "$oldrepo" = "" ]; then
    usage
fi

if [ "$newrepo" = "" ]; then
    usage
fi


check_package() {
    local oldrepo=$1
    local newrepo=$2
    local fil=$3
    local pkg=${fil/.zip}

    local name=`echo $pkg | cut -f1 -d'_'`
    local vers=`echo $pkg | cut -f2 -d'_'`
    local arch=`echo $pkg | cut -f3 -d'_'`

#    echo "$name $vers $arch"
#    echo "$oldrepo $newrepo"


    local f
    local oldpkg
    find ${oldrepo} -name "${name}_*_${arch}.zip" | while read f; do
#	echo "found $f"
	oldpkg=`basename ${f/.zip}`
	if [ "$pkg" != "$oldpkg" ]; then
	    echo "Old package $oldpkg ($pkg)"
	    if [ ! -d ${oldrepo}-oldfiles ]; then
		mkdir -p ${oldrepo}-oldfiles/
	    fi
	    cp -f $f ${oldrepo}-oldfiles/
	    rm -f $f
	    cp -f ${newrepo}/${fil} ${oldrepo}/
	fi
    done

}

for d in armeabi mips x86; do

    if test -d ${oldrepo}/${d} && test -d ${newrepo}/${d} ; then
	echo "Processing $d"

	for f in ${newrepo}/${d}/*.zip; do
	    fil=`basename $f`
	    check_package ${oldrepo}/${d} ${newrepo}/${d} $fil
	done

	pushd .
	cd ${oldrepo}/${d}
	bash make_packages.sh
	popd

    fi

done
