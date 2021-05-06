@ECHO OFF

PUSHD stb-21.5.6
IF NOT EXIST ..\..\include (XCOPY /Y/I/F *.h ..\..\include\)
POPD
