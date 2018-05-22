#!/bin/bash

echo "<repo>" > Packages

for f in *.zip; do
    unzip $f pkgdesc
    size=`du -b $f | awk '{ print $1}'`
    sed -i -e "s|@SIZE@|$size|" pkgdesc
    cat pkgdesc >> Packages
    rm -f pkgdesc
done

echo "</repo>" >> Packages
