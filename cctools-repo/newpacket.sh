#!/bin/bash

echo -ne "\nCreate new rules file from template:\n"
echo -ne "------------------------------------\n\n"

echo -ne "Name of new packet..........: "
read name
echo -ne "Version number..............: "
read version
echo -ne "Archive file suffix.........: "
read suffix
echo -ne "URL of download directory...: "
read url
echo -ne "Description.................: "
read description
#echo -ne "Packet Author...............: "
#read author

cp TEMPLATE ${name}.sh
sed -i "s|@PKG_NAME@|$name|"		${name}.sh
sed -i "s|@PKG_VERSION@|$version|"	${name}.sh
sed -i "s|@PKG_URL@|$url|"		${name}.sh
sed -i "s|@PKG_EXT@|$suffix|"		${name}.sh
sed -i "s|@PKG_DESC@|$description|"	${name}.sh
