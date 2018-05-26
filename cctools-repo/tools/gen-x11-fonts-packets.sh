#!/bin/bash

#encodings-1.0.4.tar.bz2
#font-alias-1.0.3.tar.bz2
#font-util-1.3.0.tar.bz2

fl="
font-adobe-75dpi-1.0.3.tar.bz2
font-adobe-100dpi-1.0.3.tar.bz2
font-adobe-utopia-75dpi-1.0.4.tar.bz2
font-adobe-utopia-100dpi-1.0.4.tar.bz2
font-adobe-utopia-type1-1.0.4.tar.bz2
font-arabic-misc-1.0.3.tar.bz2
font-bh-75dpi-1.0.3.tar.bz2
font-bh-100dpi-1.0.3.tar.bz2
font-bh-lucidatypewriter-75dpi-1.0.3.tar.bz2
font-bh-lucidatypewriter-100dpi-1.0.3.tar.bz2
font-bh-ttf-1.0.3.tar.bz2
font-bh-type1-1.0.3.tar.bz2
font-bitstream-75dpi-1.0.3.tar.bz2
font-bitstream-100dpi-1.0.3.tar.bz2
font-bitstream-type1-1.0.3.tar.bz2
font-cronyx-cyrillic-1.0.3.tar.bz2
font-cursor-misc-1.0.3.tar.bz2
font-daewoo-misc-1.0.3.tar.bz2
font-dec-misc-1.0.3.tar.bz2
font-ibm-type1-1.0.3.tar.bz2
font-isas-misc-1.0.3.tar.bz2
font-jis-misc-1.0.3.tar.bz2
font-micro-misc-1.0.3.tar.bz2
font-misc-cyrillic-1.0.3.tar.bz2
font-misc-ethiopic-1.0.3.tar.bz2
font-misc-meltho-1.0.3.tar.bz2
font-misc-misc-1.1.2.tar.bz2
font-mutt-misc-1.0.3.tar.bz2
font-schumacher-misc-1.1.2.tar.bz2
font-screen-cyrillic-1.0.4.tar.bz2
font-sony-misc-1.0.3.tar.bz2
font-sun-misc-1.0.3.tar.bz2
font-winitzki-cyrillic-1.0.3.tar.bz2
font-xfree86-type1-1.0.4.tar.bz2
"

echo $fl

for f in $fl; do
    name=${f/-1\.*}
    suffix="tar.bz2"
    version=${f/$name-}
    version=${version/\.$suffix}
    url="http://www.x.org/releases/X11R7.7/src/font"
    description="Xorg X11 font $name"
    echo "$name $version"

cp TEMPLATE-FONT ${name}.sh
sed -i "s|@PKG_NAME@|$name|"		${name}.sh
sed -i "s|@PKG_VERSION@|$version|"	${name}.sh
sed -i "s|@PKG_URL@|$url|"		${name}.sh
sed -i "s|@PKG_EXT@|$suffix|"		${name}.sh
sed -i "s|@PKG_DESC@|$description|"	${name}.sh

sed -i '/^build/s/-/_/g' ${name}.sh
done
