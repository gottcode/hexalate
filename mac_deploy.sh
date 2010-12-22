#!/bin/bash

APP='Hexalate'
BUNDLE="$APP.app"
VERSION='1.0.1'

macdeployqt $BUNDLE -dmg -no-plugins
mv "$APP.dmg" "${APP}_$VERSION.dmg"
