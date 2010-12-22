MKDIR Hexalate
COPY release\Hexalate.exe Hexalate
STRIP Hexalate\Hexalate.exe
COPY %QTDIR%\bin\mingwm10.dll Hexalate
COPY %QTDIR%\bin\QtCore4.dll Hexalate
COPY %QTDIR%\bin\QtGui4.dll Hexalate
