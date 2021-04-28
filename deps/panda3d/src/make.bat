@ECHO OFF

PUSHD Panda3D-1.10.9-x64
IF NOT EXIST ..\..\lib (XCOPY /E/Y/I/F lib ..\..\lib)
IF NOT EXIST ..\..\include (XCOPY /E/Y/I/F include ..\..\include)
POPD
