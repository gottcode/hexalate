#!/bin/sh


echo -n 'Preparing files...'
cd ..

rm -f hexalate.desktop.in
cp hexalate.desktop hexalate.desktop.in
sed -e '/^Name\[/ d' \
	-e '/^GenericName\[/ d' \
	-e '/^Comment\[/ d' \
	-e '/^Icon/ d' \
	-e '/^Keywords/ d' \
	-i hexalate.desktop.in

rm -f hexalate.appdata.xml.in
cp hexalate.appdata.xml hexalate.appdata.xml.in
sed -e '/p xml:lang/ d' \
	-e '/summary xml:lang/ d' \
	-e '/name xml:lang/ d' \
	-e '/<developer_name>/ d' \
	-i hexalate.appdata.xml.in

cd po
echo ' DONE'


echo -n 'Extracting messages...'
xgettext --from-code=UTF-8 --output=description.pot \
	--package-name='Hexalate' --copyright-holder='Graeme Gott' \
	../*.in
sed 's/CHARSET/UTF-8/' -i description.pot
echo ' DONE'


echo -n 'Cleaning up...'
cd ..

rm -f hexalate.desktop.in
rm -f hexalate.appdata.xml.in

echo ' DONE'
