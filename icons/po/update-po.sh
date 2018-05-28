#!/bin/sh


echo -n 'Preparing files...'
cd ..

rm -f hexalate.desktop.in
cp hexalate.desktop hexalate.desktop.in
sed -e '/^Name\[/ d' \
	-e '/^GenericName\[/ d' \
	-e '/^Comment\[/ d' \
	-e 's/^Name/_Name/' \
	-e 's/^GenericName/_GenericName/' \
	-e 's/^Comment/_Comment/' \
	-i hexalate.desktop.in

rm -f hexalate.appdata.xml.in
cp hexalate.appdata.xml hexalate.appdata.xml.in
sed -e '/p xml:lang/ d' \
	-e '/summary xml:lang/ d' \
	-e '/name xml:lang/ d' \
	-e 's/<p>/<_p>/' \
	-e 's/<\/p>/<\/_p>/' \
	-e 's/<summary>/<_summary>/' \
	-e 's/<\/summary>/<\/_summary>/' \
	-e 's/<name>/<_name>/' \
	-e 's/<\/name>/<\/_name>/' \
	-i hexalate.appdata.xml.in

cd po
echo ' DONE'


echo -n 'Updating translations...'
for POFILE in *.po;
do
	echo -n " $POFILE"
	msgmerge --quiet --update --backup=none $POFILE description.pot
done
echo ' DONE'


echo -n 'Merging translations...'
cd ..

intltool-merge --quiet --desktop-style po hexalate.desktop.in hexalate.desktop
rm -f hexalate.desktop.in

intltool-merge --quiet --xml-style po hexalate.appdata.xml.in hexalate.appdata.xml
echo >> hexalate.appdata.xml
rm -f hexalate.appdata.xml.in

echo ' DONE'
