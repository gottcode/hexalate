@ECHO ON>..\hexalate\windows\dirs.nsh
@ECHO ON>..\hexalate\windows\files.nsh
@ECHO OFF

SET SRCDIR=..\hexalate
SET APP=Hexalate
SET VERSION=1.1.6

ECHO Copying executable
MKDIR %SRCDIR%\%APP%
COPY release\%APP%.exe %SRCDIR%\%APP% >nul
CD %SRCDIR%

ECHO Copying translations
SET TRANSLATIONS=%APP%\translations
MKDIR %TRANSLATIONS%
COPY translations\*.qm %TRANSLATIONS% >nul

ECHO Copying Qt
%QTDIR%\bin\windeployqt.exe --verbose 0 --no-opengl-sw --no-system-d3d-compiler --no-svg %APP%\%APP%.exe
RMDIR /S /Q %APP%\imageformats

ECHO Creating ReadMe
TYPE README >> %APP%\ReadMe.txt
ECHO. >> %APP%\ReadMe.txt
ECHO. >> %APP%\ReadMe.txt
ECHO CREDITS >> %APP%\ReadMe.txt
ECHO ======= >> %APP%\ReadMe.txt
ECHO. >> %APP%\ReadMe.txt
TYPE CREDITS >> %APP%\ReadMe.txt
ECHO. >> %APP%\ReadMe.txt
ECHO. >> %APP%\ReadMe.txt
ECHO NEWS >> %APP%\ReadMe.txt
ECHO ==== >> %APP%\ReadMe.txt
ECHO. >> %APP%\ReadMe.txt
TYPE ChangeLog >> %APP%\ReadMe.txt

ECHO Creating installer
CD %APP%
SETLOCAL EnableDelayedExpansion
SET "parentfolder=%__CD__%"
FOR /R . %%F IN (*) DO (
  SET "var=%%F"
  ECHO Delete "$INSTDIR\!var:%parentfolder%=!" >> ..\windows\files.nsh
)
FOR /R /D %%F IN (*) DO (
  TYPE ..\windows\dirs.nsh > temp.txt
  SET "var=%%F"
  ECHO RMDir "$INSTDIR\!var:%parentfolder%=!" > ..\windows\dirs.nsh
  TYPE temp.txt >> ..\windows\dirs.nsh
)
DEL temp.txt
ENDLOCAL
CD ..
makensis.exe /V0 windows\installer.nsi

ECHO Cleaning up
RMDIR /S /Q %APP%
DEL windows\dirs.nsh
DEL windows\files.nsh
